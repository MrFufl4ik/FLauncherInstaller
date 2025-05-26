#include <curl/curl.h>

#include "../install_entity/python_install_entity.h"
#include "../install_manager/install_manager.h"
#include "../localisation_manager/localisation_manager.h"
#include "../log_manager/log_manager.h"
#include "../operation_system_manager/operation_system_manager.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);

    LocalisationManager *localisation_manager = LocalisationManager::getInstance();
    localisation_manager->loadLocalisation(LocalisationType::RU);
    std::unordered_map<std::string, std::string> localisation_map = localisation_manager->getLocalisationMap();
    installer_log(localisation_map.at("flauncher.init"));

    InstallManager::getInstance()->installEntity(std::make_unique<PythonInstallEntity>());

    OperationSystemManager *operation_system_manager = OperationSystemManager::getInstance();
    std::cout << operation_system_manager->isPythonInstalled() << std::endl;


    installer_log(localisation_map.at("flauncher.off"));
    std::cin.get();
    return 0;
}
