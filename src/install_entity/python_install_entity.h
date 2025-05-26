#pragma once
#include <string>
#include <unordered_map>
#include "../log_manager/log_manager.h"
#include "install_entity.h"


class PythonInstallEntity : public InstallEntity {
private:
    int _install() override;
    void _errorCatchHandler(int error_code) override;

public:
    PythonInstallEntity();
};
