#include "localisation.h"

Localisation* Localisation::instance = nullptr;
std::mutex Localisation::mtx;

Localisation* Localisation::getInstance() {
    if (instance == nullptr) {
        std::lock_guard<std::mutex> lock(mtx);
        if (instance == nullptr) instance = new Localisation();
    }
    return instance;
}

Localisation::Localisation() = default;

void Localisation::loadLocalisation(LocalisationType localisation_type) {
    std::lock_guard<std::mutex> lock(mtx);
    if (localisation_type == LocalisationType::RU) {
        localisationMap.emplace("test", "Это тестовая строка");
    }
}
std::unordered_map<std::string, std::string> Localisation::getLocalisationMap() {
    std::lock_guard<std::mutex> lock(mtx);
    return localisationMap;
}

//void loadLocalisation(LocalisationType localisation_type);


