#pragma once

#include <array>
#include <curl/curl.h>
#include <filesystem>
#include <format>
#include <fstream>
#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include <zip.h>
#include <iostream>
#include <random>
#include <algorithm>
#include <regex>
#include "../log_manager/log_manager.h"
#include "windows.h"
#include "winnls.h"
#include "shobjidl.h"
#include "objbase.h"
#include "objidl.h"
#include "shlguid.h"
#include <shlobj.h>

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
    int executeCommand(const std::string &command);
    int extractZipArchive(
        const std::string &archive_file_path,
        const std::string &destination_path,
        const std::string &archive_target_folder
    );
    int getVersionOfVersionFile(const std::string &version_file_path);
    bool isPythonInstalled();
    bool createLinkFile(
        const std::string &link_file_path,
        const std::string &executable_file_path,
        const std::string &executable_arguments,
        const std::string &work_directory_path,
        const std::string &icon_file_path
    );
    std::string executeCommandWithStdOut(const std::string &command);
    std::string strip(const std::string &str);
    std::string generateUUID4();
    std::string getTempDir();
    std::string normalisePath(const std::string &path);
    std::string getDesktopPath();
    std::string wStringToString(const std::wstring &wstring);
    std::wstring stringToWString(const std::string &string);
    std::vector<std::string> getLogicalDrives();



    OperationSystemManager(const OperationSystemManager &) = delete;
    OperationSystemManager &operator=(const OperationSystemManager &) = delete;
};