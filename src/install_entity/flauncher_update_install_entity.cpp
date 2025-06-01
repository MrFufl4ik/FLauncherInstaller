#include "flauncher_update_install_entity.h"

FlauncherUpdateInstallEntity::FlauncherUpdateInstallEntity() {
    addData("entity.wait", "Начинаю установку/обновление");
    addData("entity.installed", "FLauncher успешно установлен/обновлен");
    addData("entity.notinstalled", "При устоновке/обновлении произашла ошибка");
    addData("entity.titlename", "FlauncherUpdateInstallEntity");
    addData("flauncher.version.file.name", "project.version");
}

int FlauncherUpdateInstallEntity::_install() {
    OperationSystemManager *operation_system_manager = OperationSystemManager::getInstance();
    std::unordered_map<std::string, std::string> localisation_map = LocalisationManager::getInstance()->getLocalisationMap();
    installer_log(localisation_map.at("install.manager.wait.get.data"));
    const std::string flauncher_path = getData("flauncher.path");
    const std::string flauncher_update_path = getData("flauncher.update.path");
    installer_log(localisation_map.at("install.manager.get.data"), LogStatus::Correct);
    const int current_flauncher_version = operation_system_manager->getVersionOfVersionFile(std::format(
        "{}\\{}",
        flauncher_path,
        getData("flauncher.version.file.name")
    ));
    const int update_flauncher_version = operation_system_manager->getVersionOfVersionFile(std::format(
        "{}\\{}",
        flauncher_update_path,
        getData("flauncher.version.file.name")
    ));
    if (update_flauncher_version == current_flauncher_version) {
        installer_log("Обновлений не найдено", LogStatus::Correct);
        return 0;
    }
    installer_log(std::format(
        "{}: 1.{}",
        "Теущая версия FLauncher'а",
        std::to_string(current_flauncher_version)
    ));
    installer_log(std::format(
       "{}: 1.{}",
       "Новая версия FLauncher'a",
       std::to_string(update_flauncher_version)
    ));
    for (const std::filesystem::directory_entry &entry : std::filesystem::directory_iterator(flauncher_path)) {
        const std::string path = entry.path().generic_string();
        if (path == operation_system_manager->normalisePath(flauncher_update_path))
            nullptr;
        else if (path == operation_system_manager->normalisePath(std::format(
            "{}\\{}\\",
            flauncher_path,
            "modpacks"
        )))
            nullptr;
        else {
            if (std::filesystem::is_directory(path)) std::filesystem::remove_all(path);
            else std::filesystem::remove(path);
        }
    }
    for (const std::filesystem::directory_entry &entry : std::filesystem::directory_iterator(flauncher_update_path)) {
        const std::string path = entry.path().generic_string();
        std::filesystem::rename(path, std::format(
            "{}/{}",
            flauncher_path,
            entry.path().filename().generic_string()
        ));
    }
    std::filesystem::remove_all(flauncher_update_path);
    return 0;
}

void FlauncherUpdateInstallEntity::_errorCatchHandler(int error_code) {

}


