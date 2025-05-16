#pragma once
#include <string>
#include <unordered_map>
#include <iostream>
#include "../../log_system.h"

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
    int install();
};
