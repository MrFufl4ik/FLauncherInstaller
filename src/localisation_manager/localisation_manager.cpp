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
        localisationMap.emplace("flauncher.notinstalled",
                                "FLauncher не установлен, обратитесь в службу поддержки(дискордик)");
        localisationMap.emplace("flauncher.off", "FLauncher Installer завершил работу");
        localisationMap.emplace("flauncher.wait.exit", "Нажмите любую клавишу чтобы продолжить...");

        localisationMap.emplace("install.manager.install.process.start", "Начинаю установку entity");
        localisationMap.emplace("install.manager.install.process.end", "Завершена установка entity");

        localisationMap.emplace("python.start.install", "Начинаю установку Python'a...");
        localisationMap.emplace("python.notinstalled", "Python не установлен");
        localisationMap.emplace("python.installed", "Python установлен");

        localisationMap.emplace("flauncher.git.start.clone", "Начинаю клонирование репозитория FLauncher'a");
        localisationMap.emplace("flauncher.git.notcloned", "FLauncher не склонирован");
        localisationMap.emplace("flauncher.git.cloned", "FLauncher успешно склонирован");

        localisationMap.emplace("python.pip.start.install", "Начинаю установку зависимостей Python");
        localisationMap.emplace("python.pip.notinstalled", "Не удалось установить зависимости Python");
        localisationMap.emplace("python.pip.installed", "Зависимости Python успешно установлены");

        localisationMap.emplace("flauncher.archive.notinstalled", "Архив FLauncher'a не скачен");
        localisationMap.emplace("flauncher.archive.downloaded", "Архив FLanucher'a успешно скачен");
        localisationMap.emplace("flauncher.archive.notunziped", "Архив FLauncher'a не распакован");
        localisationMap.emplace("flauncher.archive.unziped", "Архив FLauncher'a успешно распакован");

        localisationMap.emplace("flauncher.exists.path.found", "Найден установленный FLauncher");
        localisationMap.emplace("flauncher.path.found", "FLauncher будет установлен в папку");

        localisationMap.emplace("python.installer.notdownloaded", "Установщик Python'a не скачен");
        localisationMap.emplace("python.installer.downloaded", "Установщик Python'a успешно скачен");

        localisationMap.emplace("curl.server.timeout", "Невозможно подключится к серверу");
        localisationMap.emplace("fs.file.open.error", "Невозможно записать данные в файл");
    }
}

std::unordered_map<std::string, std::string> LocalisationManager::getLocalisationMap() {
    std::lock_guard<std::mutex> lock(mtx);
    return localisationMap;
}

// void loadLocalisation(LocalisationType localisation_type);
