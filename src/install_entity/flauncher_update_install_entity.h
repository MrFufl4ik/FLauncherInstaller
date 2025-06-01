#include "install_entity.h"
#include "../operation_system_manager/operation_system_manager.h"

class FlauncherUpdateInstallEntity : public InstallEntity {
private:
    int _install() override;
    void _errorCatchHandler(int error_code) override;
public:
    FlauncherUpdateInstallEntity();
};
