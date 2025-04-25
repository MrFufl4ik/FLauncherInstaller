#include <iostream>
#include <curl/curl.h>
#include <fstream>
#include <filesystem>
#include <windows.h>
#include <cstdlib>
#include <zip.h>

int main();

std::string* get_flauncher_folder();
bool download_file(const std::string& url, const std::string& file_path);
bool unzip_archive_file(const std::string& archive_file_path, const std::string& destination_path);
bool unzip_archive_file(const std::string& archive_file_path, const std::string& destination_path, const std::string& archive_target_folder);
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* buffer);

const std::string FLAUNCHER_FOLDER_NAME = "FLauncher";
const std::string FLAUNCHER_ARCHIVE_URL = "https://github.com/MrFufl4ik/FLauncher/archive/refs/heads/main.zip";
const std::string FLAUNCHER_ARCHIVE_FILE_NAME = "main.zip";
const std::string PYTHON_URL = "https://www.python.org/ftp/python/3.13.3/python-3.13.3-amd64.exe";
const std::string PYTHON_FILE_NAME = "python.exe";

int main() {
    using namespace std;
    namespace fs = std::filesystem;

    SetConsoleOutputCP(CP_UTF8);

    string* ptr_launcher_folder_path = get_flauncher_folder();
    if (ptr_launcher_folder_path != nullptr) {

    }
    else {
        char disk_char = '-';
        {
            while (!fs::exists(string(1, disk_char) + ":/")) {
                string temp_str;
                while (!isalpha(temp_str[0])) {
                    cout << "Введите букву диска, где будет находится FLauncher: ";

                    getline(cin, temp_str);
                }
                disk_char = static_cast<char>(toupper(temp_str[0]));
            }
        }

        string flauncher_path = string(1, disk_char) + ":/" + FLAUNCHER_FOLDER_NAME;
        fs::create_directory(flauncher_path);
        cout << "Создана папка лаунчера: " << "\"" << flauncher_path << "\"" << endl;


        cout << "Начинаю скачивание файла: " << "\"" << PYTHON_FILE_NAME << "\"" << "..." << endl;
        string python_installer_path = flauncher_path + "/" + PYTHON_FILE_NAME;
        if (download_file(PYTHON_URL, python_installer_path)) {
            cout << "Файл: " << "\"" << python_installer_path << "\"" << " успешно загружен" << endl;
            cout << "Начинаю устанавливать: " << "\"" << python_installer_path << "\"" << "..." << endl;
            string python_installer_cmd = python_installer_path + " /quiet PrependPath=1";
            system(python_installer_cmd.c_str());
            string win_user_name = getenv("USERNAME");
            string python_executable_path = "C:/Users/" + win_user_name + "/AppData/Local/Programs/Python/Python313/python.exe";
            cout << "Python успешно установлен" << endl;
            fs::remove(python_installer_path);

            string flauncher_archive_path = flauncher_path + "/" + FLAUNCHER_ARCHIVE_FILE_NAME;
            cout << "Начинаю скачивание файла: " << "\"" << FLAUNCHER_ARCHIVE_FILE_NAME << "\"" << "..." << endl;
            if (download_file(FLAUNCHER_ARCHIVE_URL, flauncher_archive_path)) {
                cout << "Файл: " << "\"" << flauncher_archive_path << "\"" << " успешно загружен" << endl;
                cout << "Начинаем распаковку лаунчера..." << endl;
                if (unzip_archive_file(flauncher_archive_path, flauncher_path, "FLauncher-main/")) {
                    cout << "Архив: " << "\"" << flauncher_archive_path << "\"" << " успешно распакован" << endl;
                }
                else {
                    cerr << "Не удалось открыть архив: " << "\"" << flauncher_archive_path << "\"" << endl;
                    fs::remove(flauncher_path);
                    cout << "Для закрытия установщика нажмите любую клавишу...";
                    cin.get();
                    return 1;
                }
            }
            else {
                fs::remove(flauncher_path);
                cout << "Для закрытия установщика нажмите любую клавишу...";
                cin.get();
                return 1;
            }
        }
        else {
            fs::remove(flauncher_path);
            cout << "Для закрытия установщика нажмите любую клавишу...";
            cin.get();
            return 1;
        }

    }


    return 0;
}

std::string* get_flauncher_folder(){
    using namespace std;
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

bool download_file(const std::string & url, const std::string & file_path) {
    CURL* curl;
    CURLcode res;
    std::string read_buffer;
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            std::cerr << "Не удалось скачать файл :(" << std::endl << "Проверьте подключение к интернету!";
            curl_easy_cleanup(curl);
            return false;
        }

        curl_easy_cleanup(curl);

        std::ofstream out_file(file_path, std::ios::binary);
        if(!out_file) {
            std::cerr << "Не удалось записать данные в файл: " << "\"" << file_path << "\"" << std::endl;
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
    namespace fs = std::filesystem;

    int error_code = 0;
    zip* archive = zip_open(archive_file_path.c_str(), 0, &error_code);
    if (archive) {
        int num_entries = static_cast<int>(zip_get_num_entries(archive, 0));
        for (int i = 0; i < num_entries; i++) {
            const char* name = zip_get_name(archive, i, 0);
            if (!name) continue;

            std::cout << i << ": " << name << std::endl;

            std::string full_path = destination_path + "/" + name;

            size_t last_slash = full_path.find_last_of('/');
            if (last_slash != std::string::npos) {
                std::string dir_path = full_path.substr(0, last_slash);
                fs::create_directory(dir_path);
            }


            zip_file* file = zip_fopen_index(archive, i, 0);
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
        return true;
    }
    else {
        std::cerr << "Не удалось открыть архив" << std::endl;
        return false;
    }
    return false;
}

bool unzip_archive_file(const std::string& archive_file_path, const std::string& destination_path, const std::string& archive_target_folder) {
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
            std::string entry_name = std::string(name);
            if (entry_name.find(archive_target_folder) == std::string::npos) continue;

            //Get relative path of entry name
            std::string relative_path = entry_name.substr(archive_target_folder.length());

            //Get full path
            std::string full_path = destination_path + "/";
            full_path += relative_path;

            //std::cout << "entry_name: " << entry_name << std::endl;
            //std::cout << "relative_path: " << relative_path << std::endl;
            //std::cout << "full_path: " << full_path << std::endl;


            //Create directory if will need
            size_t last_slash = full_path.find_last_of('/');
            if (last_slash != std::string::npos) {
                std::string dir_path = full_path.substr(0, last_slash);
                fs::create_directory(dir_path);
            }
            //If entry name is directory will skip
            if (entry_name.back() == '/') continue;

            //Open file in archive
            zip_file* file = zip_fopen_index(archive, i, 0);
            if (!file) {
                std::cerr << "Не удалось открыть файл в архиве: " << "\"" << full_path << "\"";
                continue;
            }

            //Create a write buffer
            std::ofstream out_file(full_path, std::ios::binary);
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


