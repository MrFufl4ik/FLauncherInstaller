#pragma once
#include "install_entity.h"
#include <vector>

class FlauncherFindPathInstallEntity : public InstallEntity {
private:
    int _install() override;
    void _errorCatchHandler(int error_code) override;
public:
    FlauncherFindPathInstallEntity();
};
