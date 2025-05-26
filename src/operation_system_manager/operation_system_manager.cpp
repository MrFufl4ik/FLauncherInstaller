#include "operation_system_manager.h"

OperationSystemManager *OperationSystemManager::_instance = nullptr;
std::mutex OperationSystemManager::_mtx;

OperationSystemManager::OperationSystemManager() = default;

OperationSystemManager *OperationSystemManager::getInstance() {
    if (_instance == nullptr) {
        std::lock_guard<std::mutex> lock(_mtx);
        if (_instance == nullptr)
            _instance = new OperationSystemManager();
    }
    return _instance;
}

size_t OperationSystemManager::_writeCallBack(void *contents, size_t size, size_t nmemb, std::string *buffer) {
    size_t realsize = size * nmemb;
    if (buffer == nullptr) return 0;

    buffer->append(static_cast<char *>(contents), realsize);
    return realsize;
}

int OperationSystemManager::downloadFile(const std::string &url, const std::string &destination_file_path) {
    using std::string, std::ofstream, std::ios;

    CURL *curl = curl_easy_init();
    if (curl) {
        string read_buffer;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _writeCallBack);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);

        if (curl_easy_perform(curl) != CURLE_OK) { curl_easy_cleanup(curl); return 3; }
        curl_easy_cleanup(curl);

        ofstream out_file{destination_file_path, ios::binary};
        if (!out_file) return 1;
        out_file << read_buffer;
        out_file.close();

        return 0;
    }
    return 2;
}

std::string OperationSystemManager::strip(const std::string &str) {
    const std::string::size_type start = str.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos) return "";
    const std::string::size_type end = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(start, end - start + 1);
}

int OperationSystemManager::executeCommand(const std::string &command) {
    return system(command.c_str());
}

std::string OperationSystemManager::executeCommandWithStdOut(const std::string &command) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) return "";
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) result += buffer.data();
    return result;
}

bool OperationSystemManager::isPythonInstalled() {
    std::string command_result = strip(executeCommandWithStdOut("python --version 2>&1"));
    return command_result.rfind("Python ", 0) == 0;
}

int OperationSystemManager::pythonCreateVenv(const std::string &destination_path) {
    if (!isPythonInstalled()) return 5;
    if (std::filesystem::exists(destination_path)) std::filesystem::remove(destination_path);
    const std::string python_venv_create_command = std::string("python -m venv ").append(destination_path);
    system(python_venv_create_command.c_str());
    return 0;
}

int OperationSystemManager::pythonPipModulesInstall(const std::string &python_interpreter_path,
                                                    const std::string &requirements_list_path) {
    if (!std::filesystem::exists(requirements_list_path)) return 1;
    if (!std::filesystem::exists(python_interpreter_path)) return 6;
    const std::string python_venv_requirements_install_command =
        std::format("{} -m pip install -r {}", python_interpreter_path, requirements_list_path);
    return system(python_venv_requirements_install_command.c_str());
}
