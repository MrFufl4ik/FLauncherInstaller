#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include "../log_manager/log_manager.h"

class InstallEntity {
protected:
    std::pmr::unordered_map<std::string, std::string> _entityData;

private:
    virtual int _install();
    virtual void _error_catch_handler(int error_code);

public:
    virtual ~InstallEntity();
    InstallEntity();

    std::string getData(std::string key);
    void addData(std::string key, std::string value);
    int install();
};
