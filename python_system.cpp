#include "python_system.h"
#include "log_system.h"

void python_create_venv(const std::string& python_venv_path) {
    using std::string;
    namespace fs = std::filesystem;
    if (fs::exists(python_venv_path)) fs::remove(python_venv_path);
    const string python_venv_create_command = string("python -m venv") + string(" ") + python_venv_path;
    system(python_venv_create_command.c_str());
}
bool python_pip_install(const std::string& python_venv_path, const std::string& requirements_file_path) {
    using std::string;
    namespace fs = std::filesystem;
    if (!fs::exists(requirements_file_path)) {
        installer_file_log("Не удалось найти файл", requirements_file_path);
        return false;
    }
    const string pip_executable_file_path = python_venv_path
        + string("/")
        + string("Scripts")
        + string("/")
        + string("pip.exe");
    const string python_pip_install_command = pip_executable_file_path
        + string(" ")
        + string("install -r")
        + string(" ")
        + string(requirements_file_path);
    system(python_pip_install_command.c_str());

    return true;
}
