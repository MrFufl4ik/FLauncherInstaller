#pragma once
#include "install_entity.h"
#include <string>
#include <unordered_map>
#include "../../log_system.h"


class PythonInstallEntity : public InstallEntity{
private:
    int _install() override;
    void _error_catch_handler(int error_code) override;
public:
    PythonInstallEntity();
};


