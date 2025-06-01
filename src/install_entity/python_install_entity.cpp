#include "python_install_entity.h"

PythonInstallEntity::PythonInstallEntity() {
    addData("entity.wait", "Начинаю установку Python");
    addData("entity.installed", "Python успешно установлен");
    addData("entity.notinstalled", "Python не может быть установлен. Исправте проблему и попробуйте сново");
    addData("entity.titlename", "PythonInstallEntity");
    addData("python.url", "https://www.python.org/ftp/python/3.13.3/python-3.13.3-amd64.exe");
}

int PythonInstallEntity::_install() {
    OperationSystemManager *operation_system_manager = OperationSystemManager::getInstance();
    std::unordered_map<std::string, std::string> localisation_map = LocalisationManager::getInstance()->getLocalisationMap();
    if (!operation_system_manager->isPythonInstalled()) {
        installer_log(localisation_map.at("install.manager.wait.get.data"));
        const std::string python_url = getData("python.url");
        const std::string flauncher_path = getData("flauncher.path");
        const std::string destination_python_installer_path = std::format(
            "{}{}.exe",
            operation_system_manager->getTempDir(),
            operation_system_manager->generateUUID4()
        );
        installer_log(localisation_map.at("install.manager.get.data"), LogStatus::Correct);
        installer_log("Начинаю скачивание установщика Python");
        {
            int result_code = operation_system_manager->downloadFile(
               python_url,
               destination_python_installer_path
            );
            if (result_code != 0) return result_code;
        }
        installer_log("Установщик Python'а скачен", LogStatus::Correct);
        installer_log("Запускаю установщик Python'а");
        {
            int result_code = operation_system_manager->executeCommand(
                std::format("{} /quiet PrependPath=1", destination_python_installer_path)
            );
            if (result_code != 0) {
                installer_error_code_log(result_code);
                return 7;
            }
        }
        installer_log("Установщик Python'a благополучно завершил работу", LogStatus::Correct);
    }
    installer_log("Получаю данные об установке");
    const std::string flauncher_path = getData("flauncher.path");
    const std::string destination_requirements_file_path = std::format("{}\\requirements.txt", flauncher_path);
    const std::string virtual_python_path = std::format("{}\\.venv", flauncher_path);
    const std::string virtual_python_interpreter_path = std::format(R"({}\Scripts\python.exe)",
        virtual_python_path
    );
    installer_log("Данные об установке получены", LogStatus::Correct);
    if (!std::filesystem::exists(virtual_python_path)) {
        installer_log("Создаю виртуальное окружение");
        int result_code = operation_system_manager->pythonCreateVenv(virtual_python_path);
        if (result_code != 0) return result_code;
        installer_log("Виртуальное окружение создано", LogStatus::Correct);
    }
    addData("virtual.python.path", virtual_python_path);
    installer_log("Начинаю установку библиотек pip");
    if (!std::filesystem::exists(destination_requirements_file_path)) return 1;
    {
        int result_code = operation_system_manager->pythonPipModulesInstall(
            virtual_python_interpreter_path,
            destination_requirements_file_path
        );
        if (result_code != 0) return 8;
    }
    installer_log("Установка библиотек Python'a завершина", LogStatus::Correct);
    return 0;
}

void PythonInstallEntity::_errorCatchHandler(int error_code) {
    if (error_code == 5) installer_log("Не найден установленный Python", LogStatus::Error);
    else if (error_code == 6) installer_log("Не найден виртуальный интерпритатор Python", LogStatus::Error);
    else if (error_code == 7) installer_log("При установке Python'а случилась ошибка", LogStatus::Error);
    else if (error_code == 8) installer_log("При установке pip библиотек Python'a случилась ошибка", LogStatus::Error);
    else if (error_code == 9) installer_log("Ошибка при чтение файла с зависимостями", LogStatus::Error);
}

