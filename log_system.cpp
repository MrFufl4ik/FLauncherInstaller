#include <iostream>
#include "log_system.h"

void installer_file_log(const std::string& text, const std::string& file_path, LogStatus log_type) {
    std::cout << log_status_map[log_type] << ' ' << text << ':' << ' ' << '"' << file_path << '"' << std::endl;
}
void installer_file_log(const std::string& text, const std::string& file_path, const std::string& text_2, LogStatus log_type) {
    std::cout << log_status_map[log_type] << ' ' << text << ':' << ' ' << '"' << file_path << '"' << ' ' << text_2 << std::endl;
}
void installer_url_log(const std::string& text, const std::string& url, LogStatus log_type) {
    std::cout << log_status_map[log_type] << ' ' << text << ':' << ' ' << '"' << url << '"' << std::endl;
}
void installer_url_log(const std::string& text, const std::string& url, const std::string& text_2, LogStatus log_type) {
    std::cout << log_status_map[log_type] << ' ' << text << ':' << ' ' << '"' << url << '"' << ' ' << text_2 << std::endl;
}
void installer_log(const std::string& text, LogStatus log_type) {
    std::cout << log_status_map[log_type] << ' ' << text << std::endl;
}


