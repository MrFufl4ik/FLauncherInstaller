#pragma once
#include <iostream>
#include <memory>
#include "../install_entity/install_entity.h"
#include "../localisation_manager/localisation_manager.h"
#include "../log_manager/log_manager.h"

class InstallManager {
private:
    static InstallManager *instance;
    static std::mutex mtx;

    InstallManager();

public:
    static InstallManager *getInstance();
    std::pair<bool, std::unique_ptr<InstallEntity>> installEntity(std::unique_ptr<InstallEntity> install_entity);

    InstallManager(const InstallManager &) = delete;
    InstallManager &operator=(const InstallManager &) = delete;
};
