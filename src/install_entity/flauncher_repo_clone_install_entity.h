#pragma once
#include "install_entity.h"
#include "../operation_system_manager/operation_system_manager.h"

class FlauncherRepoCloneInstallEntity : public InstallEntity {
private:
    int _install() override;
    void _errorCatchHandler(int error_code) override;
public:
    FlauncherRepoCloneInstallEntity();
};
