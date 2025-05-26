#pragma once
#include <iostream>
#include <map>
#include <string>

enum class LogStatus { Status, Correct, Error, InstallManager };

inline std::map<LogStatus, std::string> log_status_map = {
        {LogStatus::Status, "[СТАТУС]"},
        {LogStatus::Error, "[ОШИБКА]"},
        {LogStatus::Correct, "[УСПЕШНО]"},
        {LogStatus::InstallManager, "[МЕНЕДЖЕР УСТАНОВКИ]"},
};

void installer_file_log(const std::string &text, const std::string &file_path, LogStatus log_type = LogStatus::Status);
void installer_file_log(const std::string &text, const std::string &file_path, const std::string &text_2,
                        LogStatus log_type = LogStatus::Status);
void installer_url_log(const std::string &text, const std::string &url, LogStatus log_type = LogStatus::Status);
void installer_url_log(const std::string &text, const std::string &url, const std::string &text_2,
                       LogStatus log_type = LogStatus::Status);
void installer_log(const std::string &text, LogStatus log_type = LogStatus::Status);
void installer_error_code_log(int error_code);
void installer_im_log(const std::string &text, const std::string &entity_title_name);
