#include "localisation_manager.h"

LocalisationManager *LocalisationManager::instance = nullptr;
std::mutex LocalisationManager::mtx;

LocalisationManager::LocalisationManager() = default;

LocalisationManager *LocalisationManager::getInstance() {
    if (instance == nullptr) {
        std::lock_guard<std::mutex> lock(mtx);
        if (instance == nullptr)
            instance = new LocalisationManager();
    }
    return instance;
}

void LocalisationManager::loadLocalisation(LocalisationType localisation_type) {
    std::lock_guard<std::mutex> lock(mtx);
    if (localisation_type == LocalisationType::RU) {
        localisationMap.emplace("flauncher.init", "FLauncher Installer инициализирован");
        localisationMap.emplace("flauncher.installed", "FLauncher успешно установлен");
        localisationMap.emplace("flauncher.not.installed",
                                "FLauncher не установлен, обратитесь в службу поддержки(дискордик)");
        localisationMap.emplace("flauncher.off", "FLauncher Installer завершил работу");
        localisationMap.emplace("flauncher.wait.exit", "Нажмите любую клавишу чтобы продолжить...");

        localisationMap.emplace("install.manager.install.process.start", "Начинаю установку entity");
        localisationMap.emplace("install.manager.install.process.end", "Завершена установка entity");
    }
}

std::unordered_map<std::string, std::string> LocalisationManager::getLocalisationMap() {
    std::lock_guard<std::mutex> lock(mtx);
    return localisationMap;
}

// void loadLocalisation(LocalisationType localisation_type);
