#pragma once
#include "install_entity.h"
#include "../operation_system_manager/operation_system_manager.h"
#include "../localisation_manager/localisation_manager.h"

class FlauncherShortcutInstallEntity : public InstallEntity{
private:
    int _install() override;
    void _errorCatchHandler(int error_code) override;
public:
    FlauncherShortcutInstallEntity();
};
