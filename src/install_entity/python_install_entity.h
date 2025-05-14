#include "install_entity.h"

class PythonInstallEntity : public InstallEntity{
private:
    bool _install() override;
public:
    PythonInstallEntity();
};


