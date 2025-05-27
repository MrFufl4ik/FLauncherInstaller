#pragma once
#include "install_entity.h"

class FlauncherRepoCloneInstallEntity : public InstallEntity {
private:
    int _install() override;
    void _errorCatchHandler(int error_code) override;
public:
    FlauncherRepoCloneInstallEntity();
};
