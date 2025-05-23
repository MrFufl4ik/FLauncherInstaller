#include "python_install_entity.h"

PythonInstallEntity::PythonInstallEntity() {
    _entityData.emplace("entity.wait", "Начинаю установку Python");
    _entityData.emplace("entity.installed", "Python успешно установлен");
    _entityData.emplace("entity.notinstalled", "Python не может быть установлен. Исправте проблему и попробуйте сново");
    _entityData.emplace("entity.titlename", "PythonInstallEntity");
}

int PythonInstallEntity::_install() {
    installer_log("Очень сложный процесс установки...");
    return 1;
}

void PythonInstallEntity::_error_catch_handler(int error_code) {
    if (error_code == 1) {
        installer_log("Файл не доступен для записи или чтения", LogStatus::Error);
    }
}
