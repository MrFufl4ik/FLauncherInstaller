#pragma once
#include <mutex>
#include <string>
#include <unordered_map>
#include "localisationtype.h"

class Localisation {
private:
    std::unordered_map<std::string, std::string> localisationMap;

    static Localisation* instance;
    static std::mutex mtx;

    Localisation();

public:
    static Localisation* getInstance();
    void loadLocalisation(LocalisationType localisation_type);
    std::unordered_map<std::string, std::string> getLocalisationMap();

    Localisation(const Localisation&) = delete;
    Localisation& operator = (const Localisation&) = delete;
};

