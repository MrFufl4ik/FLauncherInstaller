#pragma once
#include <map>

enum class LogStatus {
    Status,
    Correct,
    Error,
};

inline std::map<LogStatus, std::string> log_status_map = {
    {LogStatus::Status, "[СТАТУС]"},
    {LogStatus::Correct, "[УСПЕШНО]"},
    {LogStatus::Error, "[ОШИБКА]"},
};

void installer_file_log(const std::string& text, const std::string& file_path, LogStatus log_type = LogStatus::Status);
void installer_file_log(const std::string& text, const std::string& file_path, const std::string& text_2, LogStatus log_type = LogStatus::Status);
void installer_log(const std::string& text, LogStatus log_type = LogStatus::Status);