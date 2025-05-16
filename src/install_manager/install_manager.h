#pragma once
#include "../install_entity/install_entity.h"
#include "../../log_system.h"
#include "../../localisation.h"
#include <iostream>
#include <memory>

class InstallManager{
public:
  InstallManager();

  bool installEntity(std::unique_ptr<InstallEntity> install_entity);
};
