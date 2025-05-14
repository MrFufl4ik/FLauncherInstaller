#include <string>
#include <unordered_map>

class InstallEntity {
protected:
    std::pmr::unordered_map<std::string, std::string> _entityData;
private:
    virtual bool _install();
public:
    virtual ~InstallEntity();
    InstallEntity();

    bool install();
};

