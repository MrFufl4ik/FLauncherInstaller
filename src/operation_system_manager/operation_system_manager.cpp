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
    CURL *curl = curl_easy_init();
    if (curl) {
        std::string read_buffer;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _writeCallBack);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);

        if (curl_easy_perform(curl) != CURLE_OK) { curl_easy_cleanup(curl); return 3; }
        curl_easy_cleanup(curl);

        std::ofstream out_file{destination_file_path, std::ios::binary};
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

std::vector<std::string> OperationSystemManager::getLogicalDrives() {
    std::vector<std::string> logical_drives = {};
    for (char drive = 'C'; drive <= 'Z'; ++drive) {
        std::string logical_drive_path = std::format("{}:\\", std::string("") += drive);
        if (std::filesystem::exists(logical_drive_path)) logical_drives.push_back(logical_drive_path);
    }
    return logical_drives;
}

int OperationSystemManager::extractZipArchive(const std::string &archive_file_path, const std::string &destination_path,
                                              const std::string &archive_target_folder) {
    int error_code = 0;
    zip *archive = zip_open(archive_file_path.c_str(), 0, &error_code);
    if (!archive) return 4;
    for (int i = 0; i < static_cast<int>(zip_get_num_entries(archive, 0)); i++) {
        const char *name = zip_get_name(archive, i, 0);
        if (!name) continue;
        std::string entry_name = std::string(name);
        if (entry_name.find(archive_target_folder) == std::string::npos) continue;
        std::string full_path = std::format(
            "{}/{}",
            destination_path,
            entry_name.substr(archive_target_folder.length())
        );
        size_t last_slash = full_path.find_last_of('/');
        if (last_slash != std::string::npos) {
            std::string dir_path = full_path.substr(0, last_slash);
            std::filesystem::create_directory(dir_path);
        }
        if (entry_name.back() == '/') continue;
        zip_file *file = zip_fopen_index(archive, i, 0);
        if (!file) continue;
        std::ofstream out_file(full_path, std::ios::binary);
        if (!out_file) continue;
        char buffer[4096];
        zip_int64_t bytes_read;
        while ((bytes_read = zip_fread(file, buffer, sizeof(buffer))) > 0) {
            out_file.write(buffer, bytes_read);
        }
        out_file.close();
        zip_fclose(file);
    }
    zip_close(archive);
    return 0;
}

std::string OperationSystemManager::generateUUID4() {
    std::random_device random_device;
    std::mt19937 generator {random_device()};
    std::uniform_int_distribution<> first_distribution {0, 15};
    std::uniform_int_distribution<> second_distribution {8, 11};
    std::stringstream result;
    for (int i = 0; i < 32; i++) {
        int number = 4;
        if (i == 16) number = second_distribution(generator);
        else number = first_distribution(generator);
        result << std::hex << number;
        if (i == 7 || i == 11 || i == 15 || i == 19) result << '-';
    }
    return result.str();
}

std::string OperationSystemManager::getTempDir() {
    return std::filesystem::temp_directory_path().generic_string();
}

int OperationSystemManager::getVersionOfVersionFile(const std::string &version_file_path) {
    if (!std::filesystem::exists(version_file_path)) return 0;
    std::ifstream version_file_input_stream {version_file_path};
    std::string result;
    std::getline(version_file_input_stream, result);
    return std::stoi(result);
}

std::string OperationSystemManager::normalisePath(const std::string &path) {
    return std::regex_replace(path, std::regex(R"([\\/]+)"), "/");
}

std::string OperationSystemManager::wStringToString(const std::wstring &wstring) {
    if (wstring.empty()) return "";
    std::string string = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(wstring);
    return string;
}


std::wstring OperationSystemManager::stringToWString(const std::string &string) {
    if (string.empty()) return L"";
    std::wstring wstring = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(string);
    return wstring;
}

bool OperationSystemManager::createLinkFile(const std::string &link_file_path, const std::string &executable_file_path,
                                            const std::string &executable_arguments,
                                            const std::string &work_directory_path, const std::string &icon_file_path) {
    HRESULT hresult = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    if (FAILED(hresult)) return false;
    std::wstring w_link_file_path = stringToWString(link_file_path);
    std::wstring w_icon_file_path = stringToWString(icon_file_path);
    IShellLink *p_shell_link = nullptr;
    hresult = CoCreateInstance(
        CLSID_ShellLink,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_IShellLink,
        (void**) &p_shell_link
    );
    if (SUCCEEDED(hresult)) {
        p_shell_link->SetPath(executable_file_path.c_str());
        p_shell_link->SetArguments(executable_arguments.c_str());
        if (!w_icon_file_path.empty())
            p_shell_link->SetIconLocation(icon_file_path.c_str(), 0);
        p_shell_link->SetWorkingDirectory(work_directory_path.c_str());
        IPersistFile *p_persist_file = nullptr;
        hresult = p_shell_link->QueryInterface(IID_IPersistFile, (void**) &p_persist_file);
        if (SUCCEEDED(hresult)) {
            hresult = p_persist_file->Save(w_link_file_path.c_str(), TRUE);
            p_persist_file->Release();
        }
        p_persist_file->Release();
    }
    CoUninitialize();
    return SUCCEEDED(hresult);
}

std::string OperationSystemManager::getDesktopPath() {
    PWSTR path = nullptr;
    HRESULT hresult = SHGetKnownFolderPath(FOLDERID_Desktop, 0, nullptr, &path);
    if (SUCCEEDED(hresult)) {
        std::wstring desktop_path {path};
        CoTaskMemFree(path);
        return wStringToString(desktop_path);
    }
    else return "";
}
