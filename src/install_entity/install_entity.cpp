#include "install_entity.h"
#include <iostream>
#include "../../log_system.h"

InstallEntity::InstallEntity() = default;
InstallEntity::~InstallEntity() = default;
bool InstallEntity::_install() { return true; }


bool InstallEntity::install() {
    if (
        !_entityData.contains("entity.wait") ||
        !_entityData.contains("entity.installed") ||
        !_entityData.contains("entity.notinstalled")
    ) {
        std::cout << "Install entity keys is not defined" << std::endl;
        exit(1);
    }

    installer_log(_entityData.at("entity.wait"));
    if (!_install()) {
        installer_log(_entityData.at("entity.notinstalled"), LogStatus::Error);
        return false;
    }
    installer_log(_entityData.at("entity.installed"), LogStatus::Correct);
    return true;
}


