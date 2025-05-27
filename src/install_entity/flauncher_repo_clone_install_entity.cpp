#include "flauncher_repo_clone_install_entity.h"

FlauncherRepoCloneInstallEntity::FlauncherRepoCloneInstallEntity() {
    addData("entity.wait", "Начинаю клонирование репозитория FLauncher'a");
    addData("entity.installed", "Репозиторий успешно склонирован");
    addData("entity.notinstalled", "Репозиторий не может быть склонирован. Исправте проблему и попробуйте сново");
    addData("entity.titlename", "FlauncherRepoCloneInstallEntity");
}

int FlauncherRepoCloneInstallEntity::_install() {

}

void FlauncherRepoCloneInstallEntity::_errorCatchHandler(int error_code) {

}
