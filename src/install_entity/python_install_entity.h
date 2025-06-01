#pragma once
#include <string>
#include <unordered_map>
#include "install_entity.h"
#include "../log_manager/log_manager.h"
#include "../operation_system_manager/operation_system_manager.h"
#include "../localisation_manager/localisation_manager.h"

class PythonInstallEntity : public InstallEntity {
private:
    int _install() override;
    void _errorCatchHandler(int error_code) override;

public:
    PythonInstallEntity();
};
