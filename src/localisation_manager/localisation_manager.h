#pragma once
#include <mutex>
#include <string>
#include <unordered_map>
#include "localisation_type.h"

class LocalisationManager {
private:
    std::unordered_map<std::string, std::string> localisationMap;

    static LocalisationManager *instance;
    static std::mutex mtx;

    LocalisationManager();

public:
    static LocalisationManager *getInstance();
    void loadLocalisation(LocalisationType localisation_type);
    std::unordered_map<std::string, std::string> getLocalisationMap();

    LocalisationManager(const LocalisationManager &) = delete;
    LocalisationManager &operator=(const LocalisationManager &) = delete;
};
