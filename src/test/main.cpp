#include <curl/curl.h>

#include "../install_entity/flauncher_find_path_install_entity.h"
#include "../install_entity/flauncher_repo_clone_install_entity.h"
#include "../install_entity/flauncher_update_install_entity.h"
#include "../install_entity/python_install_entity.h"
#include "../install_manager/install_manager.h"
#include "../localisation_manager/localisation_manager.h"
#include "../log_manager/log_manager.h"
#include "../operation_system_manager/operation_system_manager.h"

bool run(InstallManager *install_manager);

int main() {
    SetConsoleOutputCP(CP_UTF8);

    LocalisationManager *localisation_manager = LocalisationManager::getInstance();
    localisation_manager->loadLocalisation(LocalisationType::RU);
    std::unordered_map<std::string, std::string> localisation_map = localisation_manager->getLocalisationMap();
    installer_log(localisation_map.at("flauncher.init"));
    if (run(InstallManager::getInstance())) installer_log(localisation_map.at("flauncher.installed"));
    else installer_log(localisation_map.at("flauncher.not.installed"));

    // OperationSystemManager *operation_system_manager = OperationSystemManager::getInstance();
    // std::wcout << operation_system_manager->stringToWString("Hello world") << std::endl;

    installer_log(localisation_map.at("flauncher.off"));
    std::cin.get();
    return 0;
}

bool run(InstallManager *install_manager) {
    std::unique_ptr<InstallEntity> flauncher_find_path_install_entity =
            install_manager->installEntity(std::make_unique<FlauncherFindPathInstallEntity>()).second;
    const std::string flauncher_path = flauncher_find_path_install_entity->getData("flauncher.path");

    std::unique_ptr<InstallEntity> flauncher_repo_clone_install_entity =
        std::make_unique<FlauncherRepoCloneInstallEntity>();
    flauncher_repo_clone_install_entity->addData("flauncher.path", flauncher_path);
    auto flauncher_repo_clone_install_entity_result_pair =
        install_manager->installEntity(std::move(flauncher_repo_clone_install_entity));
    {
        bool exit_state = flauncher_repo_clone_install_entity_result_pair.first;
        if (exit_state == false) return false;
    }
    const std::string flauncher_update_path =
        flauncher_repo_clone_install_entity_result_pair.second->getData("flauncher.update.path");

    std::unique_ptr<InstallEntity> flauncher_update_install_entity = std::make_unique<FlauncherUpdateInstallEntity>();
    flauncher_update_install_entity->addData("flauncher.path", flauncher_path);
    flauncher_update_install_entity->addData("flauncher.update.path", flauncher_update_path);
    install_manager->installEntity(std::move(flauncher_update_install_entity));
    return true;
}

