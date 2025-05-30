#include "flauncher_shortcut_install_entity.h"

#include "../operation_system_manager/operation_system_manager.h"

FlauncherShortcutInstallEntity::FlauncherShortcutInstallEntity() {
    addData("entity.wait", "Начинаю создание ярлыка");
    addData("entity.installed", "Ярлык успешно создан");
    addData("entity.notinstalled", "Ярлык не может быть создан. Исправте проблему и попробуйте сново");
    addData("entity.titlename", "FlauncherShortcutInstallEntity");
}

int FlauncherShortcutInstallEntity::_install() {
    OperationSystemManager *operation_system_manager = OperationSystemManager::getInstance();
    installer_log("Получаю данные об установке");
    const std::string flauncher_path = getData("flauncher.path");
    const std::string virtual_python_path = getData("virtual.python.path");
    installer_log("Данные об установке получены", LogStatus::Correct);
    {
        bool exit_state = operation_system_manager->createLinkFile(
            std::format("{}\\{}", operation_system_manager->getDesktopPath(), "FLauncher.lnk"),
            std::format("{}\\{}\\{}", virtual_python_path, "Scripts", "pythonw.exe"),
            std::format("{}\\{}", flauncher_path, "main.py"),
            flauncher_path,
            ""
        );
        if (exit_state == false) return 5;
    }
    return 0;
}

void FlauncherShortcutInstallEntity::_errorCatchHandler(int error_code) {
    if (error_code == 5) installer_log("Невозможно создать ярлык", LogStatus::Error);
}



