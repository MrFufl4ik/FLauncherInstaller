#include "install_manager.h"

InstallManager::InstallManager(){

}

bool InstallManager::installEntity(std::unique_ptr<InstallEntity> install_entity){
  std::string entity_title_name = install_entity->getData("entity.titlename");
  std::unordered_map<std::string, std::string> localisation_map = Localisation::getInstance()->getLocalisationMap();

  if(
    !localisation_map.contains("install.manager.install.process.start") ||
    !localisation_map.contains("install.manager.install.process.end")
  )
  {
    std::cout << "Install manager localisation keys is not defined!" << std::endl;
    exit(1);
  }
  installer_im_log(localisation_map.at("install.manager.install.process.start"), entity_title_name);
  bool process_exit_code = install_entity->install();
  //end of start
  installer_im_log(localisation_map.at("install.manager.install.process.end"), entity_title_name);
  if(process_exit_code != 0) return false;
  return true;
  //begin of end
  //end of end
}
