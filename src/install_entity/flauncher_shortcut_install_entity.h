#include "install_entity.h"

class FlauncherShortcutInstallEntity : public InstallEntity{
private:
    int _install() override;
    void _errorCatchHandler(int error_code) override;
public:
    FlauncherShortcutInstallEntity();
};