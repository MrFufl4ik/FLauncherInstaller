#include <iostream>
#include <curl/curl.h>
#include <fstream>
#include <filesystem>
#include <windows.h>
#include <map>
#include <zip.h>
#include "localisation.h"
#include "log_system.h"
#include "python_system.h"
#include "src/install_entity/python_install_entity.h"

int main();

bool download_file(const std::string& url, const std::string& file_path);
bool unzip_archive_file(const std::string& archive_file_path, const std::string& destination_path);
bool unzip_archive_file(const std::string& archive_file_path, const std::string& destination_path, const std::string& archive_target_folder);
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* buffer);

bool flauncher_install(const std::string& flauncher_path);
bool flauncher_update(const std::string& flauncher_path);

std::string* get_exists_flauncher_folder_path_in_user_space();
std::string get_flauncher_folder_path_in_user_space();
std::string search_flauncher_folder_path();
bool python_install(const std::string& python_url, const std::string& python_file_name, const std::string& flauncher_path);
bool flauncher_clone(const std::string& flauncher_path, const std::string& flauncher_archive_url, const std::string& flauncher_archive_file_name);


const std::string FLAUNCHER_FOLDER_NAME = "FLauncher";
const std::string FLAUNCHER_ARCHIVE_URL = "https://github.com/MrFufl4ik/FLauncher/archive/refs/heads/main.zip";
const std::string FLAUNCHER_ARCHIVE_FILE_NAME = "main.zip";
const std::string PYTHON_URL = "https://www.python.org/ftp/python/3.13.3/python-3.13.3-amd64.exe";
const std::string PYTHON_FILE_NAME = "python.exe";

int main() {
    using std::string;
    using std::cout;
    using std::cin;
    using std::cerr;
    using std::endl;
    using std::unordered_map;

    namespace fs = std::filesystem;

    SetConsoleOutputCP(CP_UTF8);

    Localisation::getInstance()->loadLocalisation(LocalisationType::RU);
    //installer_log(Localisation::getInstance()->getLocalisationMap().at("test"), LogStatus::Correct);
    PythonInstallEntity install_entity {};
    install_entity.install();
    return 0;

    unordered_map<string, string> localisation_map = Localisation::getInstance()->getLocalisationMap();

    installer_log(localisation_map.at("flauncher.init"), LogStatus::Correct);

    std::string flauncher_path = search_flauncher_folder_path();

    if (!fs::exists(flauncher_path)) {
        //FLauncher install
        fs::create_directory(flauncher_path);
        bool is_flauncher_installed = flauncher_install(flauncher_path);
        if (is_flauncher_installed) installer_log(localisation_map.at("flauncher.installed"), LogStatus::Correct);
        else installer_log(localisation_map.at("flauncher.notinstalled"), LogStatus::Error);
    }
    else {
        //FLauncher update
        flauncher_update(flauncher_path);
    }
    installer_log(localisation_map.at("flauncher.off"));
    installer_log(localisation_map.at("flauncher.wait.exit"));
    cin.get();
    return 0;
}

bool flauncher_install(const std::string& flauncher_path) {
    using std::string;
    using std::cout;
    using std::endl;
    using std::unordered_map;

    unordered_map<string, string> localisation_map = Localisation::getInstance()->getLocalisationMap();

    {
        installer_log(localisation_map.at("python.start.install"));
        bool is_python_install = python_install(PYTHON_URL, PYTHON_FILE_NAME, flauncher_path);
        if (!is_python_install) {
            installer_log(localisation_map.at("python.notinstalled"), LogStatus::Error);
            return false;
        }
        installer_log(localisation_map.at("python.installed"), LogStatus::Correct);
    }
    cout << endl;
    {
        installer_log(localisation_map.at("flauncher.git.start.clone"));
        bool is_flauncher_cloned = flauncher_clone(flauncher_path, FLAUNCHER_ARCHIVE_URL, FLAUNCHER_ARCHIVE_FILE_NAME);
        if (!is_flauncher_cloned) {
            installer_log(localisation_map.at("flauncher.git.notcloned"), LogStatus::Error);
            return false;
        }
        installer_log(localisation_map.at("flauncher.git.cloned"), LogStatus::Correct);
    }
    cout << endl;
    {
        installer_log(localisation_map.at("python.pip.start.install"));
        const string python_venv_path = flauncher_path + "/" + ".venv";
        python_create_venv(python_venv_path);
        bool is_pip_requirements_installed = python_pip_install(
            python_venv_path,
            flauncher_path + "/" + "requirements.txt"
        );
        if (!is_pip_requirements_installed) {
            installer_log(localisation_map.at("python.pip.notinstalled"), LogStatus::Error);
            return false;
        }
        installer_log(localisation_map.at("python.pip.installed"), LogStatus::Correct);
    }
    return true;
}

bool flauncher_update(const std::string& flauncher_path) {
    return true;
}

bool flauncher_clone(const std::string& flauncher_path, const std::string& flauncher_archive_url, const std::string& flauncher_archive_file_name) {
    using std::string;
    using std::unordered_map;
    namespace fs = std::filesystem;

    unordered_map<string, string> localisation_map = Localisation::getInstance()->getLocalisationMap();

    const string flauncher_archive_file_path = flauncher_path + "/" + flauncher_archive_file_name;
    {
        bool is_flauncher_archive_downloaded = download_file(flauncher_archive_url,flauncher_archive_file_path);
        if (!is_flauncher_archive_downloaded) {
            installer_log(localisation_map.at("flauncher.archive.notinstalled"), LogStatus::Error);
            return false;
        }
        installer_log(localisation_map.at("flauncher.archive.downloaded"), LogStatus::Correct);
    }
    {
        bool is_flauncher_archive_uziped = unzip_archive_file(
            flauncher_archive_file_path,
            flauncher_path,
            "FLauncher-main/");
        if (!is_flauncher_archive_uziped) {
            installer_log(localisation_map.at("flauncher.archive.notunziped"), LogStatus::Error);
            return false;
        }
        installer_log(localisation_map.at("flauncher.archive.unziped"), LogStatus::Correct);
    }
    fs::remove(flauncher_archive_file_path);
    return true;
}

//Search FLauncher folder path
std::string* get_exists_flauncher_folder_path_in_user_space() {
    using std::string;
    namespace fs = std::filesystem;
    string* ptr_to_folder_string = nullptr;
    for (char ch = 'C'; ch <= 'Z'; ch++) {
        string file_path = string(1, ch);
        file_path += ":/" + FLAUNCHER_FOLDER_NAME;
        if (fs::exists(file_path)) {
            ptr_to_folder_string = new string(file_path);
            return ptr_to_folder_string;
        }
    }
    return ptr_to_folder_string;
}
std::string get_flauncher_folder_path_in_user_space() {
    using std::string;
    using std::cout;
    using std::cin;
    using std::endl;
    namespace fs = std::filesystem;
    char disk_char = '-';
    {
        while (!fs::exists(string(1, disk_char) + ":/")) {
            string temp_str;
            while (!isalpha(temp_str[0])) {
                cout << "[ВВОД] Введите букву диска, где будет находится FLauncher: ";

                getline(cin, temp_str);
            }
            cout << endl;
            disk_char = static_cast<char>(toupper(temp_str[0]));
        }
    }
    return string(1, disk_char) + ":/" + FLAUNCHER_FOLDER_NAME;
}
std::string search_flauncher_folder_path() {
    using std::string;
    using std::unordered_map;
    namespace fs = std::filesystem;

    unordered_map<string, string> localisation_map = Localisation::getInstance()->getLocalisationMap();

    string* ptr_exists_flauncher_folder_path_in_user_space = get_exists_flauncher_folder_path_in_user_space();
    if (ptr_exists_flauncher_folder_path_in_user_space != nullptr) {
        installer_file_log(localisation_map.at("flauncher.exists.path.found"), *ptr_exists_flauncher_folder_path_in_user_space);
        return *ptr_exists_flauncher_folder_path_in_user_space;
    }
    string flauncher_folder_path = get_flauncher_folder_path_in_user_space();
    installer_file_log(localisation_map.at("flauncher.path.found"), flauncher_folder_path);
    return flauncher_folder_path;
}

bool python_install(const std::string& python_url, const std::string& python_file_name, const std::string& flauncher_path) {
    using std::cout;
    using std::string;
    using std::endl;
    using std::unordered_map;
    namespace fs = std::filesystem;

    unordered_map<string, string> localisation_map = Localisation::getInstance()->getLocalisationMap();

    const string python_installer_path = flauncher_path + "/" + python_file_name;
    bool is_python_installer_downloaded = download_file(python_url, python_installer_path);
    if (!is_python_installer_downloaded) {
        installer_log(localisation_map.at("python.installer.notdownloaded"), LogStatus::Error);
        return false;
    }
    installer_log(localisation_map.at("python.installer.downloaded"), LogStatus::Correct);
    const string python_installer_cmd = python_installer_path + " /quiet PrependPath=1";
    system(python_installer_cmd.c_str());
    fs::remove(python_installer_path);
    return true;
}

bool download_file(const std::string & url, const std::string & file_path) {
    using std::string;
    using std::endl;
    using std::ofstream;
    using std::ios;
    using std::unordered_map;

    unordered_map<string, string> localisation_map = Localisation::getInstance()->getLocalisationMap();

    CURL* curl;
    CURLcode res;
    string read_buffer;
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            installer_url_log(localisation_map.at("curl.server.timeout"), url, LogStatus::Error);
            curl_easy_cleanup(curl);
            return false;
        }

        curl_easy_cleanup(curl);

        ofstream out_file(file_path, ios::binary);
        if(!out_file) {
            installer_file_log(localisation_map.at("fs.file.open.error"), file_path, LogStatus::Error);
            return false;
        }
        out_file << read_buffer;
        out_file.close();

        return true;
    }

    return false;
}
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* buffer) {
    size_t realsize = size * nmemb;
    if (buffer == nullptr) {
        return 0;
    }
    buffer->append(static_cast<char*>(contents), realsize);
    return realsize;
}

bool unzip_archive_file(const std::string& archive_file_path, const std::string& destination_path) {
    using std::cerr;
    using std::cout;
    using std::string;
    using std::endl;
    using std::ios;
    using std:: ofstream;
    namespace fs = std::filesystem;

    int error_code = 0;
    zip* archive = zip_open(archive_file_path.c_str(), 0, &error_code);
    if (archive) {
        int num_entries = static_cast<int>(zip_get_num_entries(archive, 0));
        for (int i = 0; i < num_entries; i++) {
            const char* name = zip_get_name(archive, i, 0);
            if (!name) continue;

            cout << i << ": " << name << endl;

            string full_path = destination_path + "/" + name;

            size_t last_slash = full_path.find_last_of('/');
            if (last_slash != string::npos) {
                string dir_path = full_path.substr(0, last_slash);
                fs::create_directory(dir_path);
            }


            zip_file* file = zip_fopen_index(archive, i, 0);
            if (!file) continue;


            ofstream out_file(full_path, ios::binary);
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
        return true;
    }
    else {
        cerr << "Не удалось открыть архив" << endl;
        return false;
    }
    return false;
}

bool unzip_archive_file(const std::string& archive_file_path, const std::string& destination_path, const std::string& archive_target_folder) {
    using std::cerr;
    using std::cout;
    using std::string;
    using std::endl;
    using std::ios;
    using std:: ofstream;
    namespace fs = std::filesystem;

    int error_code = 0;
    //Open an archive
    zip* archive = zip_open(archive_file_path.c_str(), 0, &error_code);
    //If archive is open - unpack it
    if (archive) {
        //Get all files path list in archive
        int num_entries = static_cast<int>(zip_get_num_entries(archive, 0));
        //Iterate it
        for (int i = 0; i < num_entries; i++) {
            //Get file in archive path
            const char* name = zip_get_name(archive, i, 0);
            if (!name) continue;

            //if entry name not contain archive target path skip it
            string entry_name = string(name);
            if (entry_name.find(archive_target_folder) == string::npos) continue;

            //Get relative path of entry name
            string relative_path = entry_name.substr(archive_target_folder.length());

            //Get full path
            string full_path = destination_path + "/";
            full_path += relative_path;

            //std::cout << "entry_name: " << entry_name << std::endl;
            //std::cout << "relative_path: " << relative_path << std::endl;
            //std::cout << "full_path: " << full_path << std::endl;


            //Create directory if will need
            size_t last_slash = full_path.find_last_of('/');
            if (last_slash != string::npos) {
                string dir_path = full_path.substr(0, last_slash);
                fs::create_directory(dir_path);
            }
            //If entry name is directory will skip
            if (entry_name.back() == '/') continue;

            //Open file in archive
            zip_file* file = zip_fopen_index(archive, i, 0);
            if (!file) {
                cerr << "Не удалось открыть файл в архиве: " << "\"" << full_path << "\"";
                continue;
            }

            //Create a write buffer
            ofstream out_file(full_path, ios::binary);
            if (!out_file) continue;

            //Write archive file data to buffer
            char buffer[4096];
            zip_int64_t bytes_read;
            while ((bytes_read = zip_fread(file, buffer, sizeof(buffer))) > 0) {
                out_file.write(buffer, bytes_read);
            }

            out_file.close();
            zip_fclose(file);
        }

        zip_close(archive);
        return true;
    }
    return false;
}


