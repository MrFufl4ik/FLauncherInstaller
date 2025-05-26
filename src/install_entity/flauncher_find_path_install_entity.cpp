#include "flauncher_find_path_install_entity.h"

#include "../operation_system_manager/operation_system_manager.h"

FlauncherFindPathInstallEntity::FlauncherFindPathInstallEntity() {
    addData("entity.wait", "Начинаю поиск папки с FLauncher'ом");
    addData("entity.installed", "FLauncher успешно найден");
    addData("entity.notinstalled", "Возникли ошибки при поиске папки с FLauncher'ом");
    addData("entity.titlename", "FlauncherFindPathInstallEntity");
    addData("flauncher.folder.name", "FLauncher");
}

int FlauncherFindPathInstallEntity::_install() {
    installer_log("Получаю список дисков");
    OperationSystemManager *operation_system_manager = OperationSystemManager::getInstance();
    std::vector<std::string> logical_drives = operation_system_manager->getLogicalDrives();
    for (int i = 0; i < logical_drives.size(); i++) {
        const std::string flauncher_path = std::format("{}\\{}",
            logical_drives[i],
            getData("flauncher.folder.name")
        );
        if (std::filesystem::exists(flauncher_path)) {
            addData("flauncher.path", flauncher_path);
            return 0;
        }
    }
    installer_log("Вывожу список дисков");
    for (int i = 0; i < logical_drives.size(); i++) std::cout << i + 1 << ':' << ' ' << logical_drives[i] << std::endl;
    std::cout << std::endl;
    for (std::string user_drive; true;) {
        std::cout << "Введите букву диска" << ':' << ' ';
        std::getline(std::cin, user_drive);
    }
    return 0;
}

void FlauncherFindPathInstallEntity::_errorCatchHandler(int error_code) {

}

