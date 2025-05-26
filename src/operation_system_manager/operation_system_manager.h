#pragma once

#include <mutex>
#include <string>
#include <curl/curl.h>
#include <fstream>
#include <array>
#include <filesystem>
#include <memory>
#include <format>

class OperationSystemManager {
private:
    static OperationSystemManager *_instance;
    static std::mutex _mtx;

    static size_t _writeCallBack(void* contents, size_t size, size_t nmemb, std::string* buffer);

    OperationSystemManager();

public:
    static OperationSystemManager *getInstance();
    int downloadFile(const std::string &url, const std::string &destination_file_path);
    int pythonCreateVenv(const std::string &destination_path);
    int pythonPipModulesInstall(const std::string &python_interpreter_path, const std::string &requirements_list_path);
    bool isPythonInstalled();
    std::string executeCommand(const std::string &command);
    std::string strip(const std::string &str);


    OperationSystemManager(const OperationSystemManager &) = delete;
    OperationSystemManager &operator=(const OperationSystemManager &) = delete;
};