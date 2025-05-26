#include "install_entity.h"

InstallEntity::InstallEntity() = default;
InstallEntity::~InstallEntity() = default;

int InstallEntity::_install() { return 0; }
void InstallEntity::_errorCatchHandler(int error_code) {}

void InstallEntity::_defaultErrorCatchHandler(int error_code) {
    if (error_code == 1) installer_log("Файл не доступен для записи или чтения", LogStatus::Error);
    else if (error_code == 2) installer_log("Нет подключения к интернету", LogStatus::Error);
    else if (error_code == 3) installer_log("Ресурс не доступен", LogStatus::Error);
    _errorCatchHandler(error_code);
}


int InstallEntity::install() {
    if (!_entityData.contains("entity.wait") || !_entityData.contains("entity.installed") ||
        !_entityData.contains("entity.notinstalled")) {
        std::cout << "Install entity localisation keys is not defined" << std::endl;
        exit(1);
    }

    installer_log(_entityData.at("entity.wait"));
    int install_entity_exit_code = this->_install();
    if (install_entity_exit_code != 0) {
        _defaultErrorCatchHandler(install_entity_exit_code);
        installer_log(_entityData.at("entity.notinstalled"), LogStatus::Error);
        return install_entity_exit_code;
    }
    installer_log(_entityData.at("entity.installed"), LogStatus::Correct);
    return install_entity_exit_code;
}

std::string InstallEntity::getData(std::string key) {
    using std::cout, std::endl;
    if (!_entityData.contains(key)) {
        cout << "Key: " << key << " is not defined in install entity" << endl;
        exit(1);
    }
    return _entityData.at(key);
}

void InstallEntity::addData(std::string key, std::string value) {
    using std::cout, std::endl;
    if (_entityData.contains(key)) {
        cout << "Key: " << key << " the key is defined, but should not be" << endl;
        exit(1);
    }
    _entityData.emplace(key, value);
}
