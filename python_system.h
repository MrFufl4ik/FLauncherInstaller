#pragma once
#include <string>
#include <filesystem>

void python_create_venv(const std::string& python_venv_path);
bool python_pip_install(const std::string& python_venv_path, const std::string& requirements_file_path);
