#include <iostream>
#include "log_system.h"

void installer_file_log(const std::string& text, const std::string& file_path, LogStatus log_type) {
    std::cout << log_status_map[log_type] << ' ' << text << ':' << ' ' << '"' << file_path << '"';
}
void installer_file_log(const std::string& text, const std::string& file_path, const std::string& text_2, LogStatus log_type) {
    std::cout << log_status_map[log_type] << ' ' << text << ':' << ' ' << '"' << file_path << '"' << ' ' << text_2;
}
void installer_log(const std::string& text, LogStatus log_type) {
    std::cout << log_status_map[log_type] << ' ' << text << std::endl;
}

