#include "python_install_entity.h"

#include "../../log_system.h"

PythonInstallEntity::PythonInstallEntity() {
    _entityData.emplace("entity.wait", "Мы вас тестово просим подождать");
    _entityData.emplace("entity.installed", "Тестовая программа установлена");
    _entityData.emplace("entity.notinstalled", "Тестовая программа не установлена");
}


bool PythonInstallEntity::_install() {
    installer_log("Очень сложный процесс установки...");
    return true;
}
