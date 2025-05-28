#include "flauncher_repo_clone_install_entity.h"
#include "../operation_system_manager/operation_system_manager.h"


FlauncherRepoCloneInstallEntity::FlauncherRepoCloneInstallEntity() {
    addData("entity.wait", "Начинаю клонирование репозитория FLauncher'a");
    addData("entity.installed", "Репозиторий успешно склонирован");
    addData("entity.notinstalled", "Репозиторий не может быть склонирован. Исправте проблему и попробуйте сново");
    addData("entity.titlename", "FlauncherRepoCloneInstallEntity");
    addData("flauncher.repo.archive.url", "http://github.com/MrFufl4ik/FLauncher/archive/refs/heads/main.zip");
}

int FlauncherRepoCloneInstallEntity::_install() {
    OperationSystemManager *operation_system_manager = OperationSystemManager::getInstance();
    installer_log("Получаю данные об установке");
    const std::string flauncher_path = getData("flauncher.path");
    installer_log("Данные об установке получены");
    installer_log("Начинаю скачивание архива с FLauncher");
    const std::string flauncher_archive_file_path = std::format(
        "{}{}.zip",
        operation_system_manager->getTempDir(),
        operation_system_manager->generateUUID4()
    );
    {
        int error_code = operation_system_manager->downloadFile(
            getData("flauncher.repo.archive.url"),
            flauncher_archive_file_path
        );
        if (error_code != 0) return error_code;
    }
    installer_log("Архив с FLauncher успешно скачен", LogStatus::Correct);
    const std::string destination_path = std::format("{}\\{}", flauncher_path, "update");
    if (!std::filesystem::exists(destination_path)) std::filesystem::create_directory(destination_path);
    {
        int error_code = operation_system_manager->extractZipArchive(
            flauncher_archive_file_path,
            destination_path,
            "FLauncher-main/"
        );
        if (error_code != 0) return error_code;
    }
    return 0;
}

void FlauncherRepoCloneInstallEntity::_errorCatchHandler(int error_code) {

}
