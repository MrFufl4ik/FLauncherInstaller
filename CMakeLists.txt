cmake_minimum_required(VERSION 3.30)
project(FLauncherInstaller)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

add_executable(FLauncherInstaller 
        src/test/main.cpp
        src/install_entity/install_entity.cpp
        src/install_entity/install_entity.h
        src/install_entity/python_install_entity.cpp
        src/install_entity/python_install_entity.h
        src/install_manager/install_manager.cpp
        src/install_manager/install_manager.h
        log_system.cpp
        log_system.h
        python_system.cpp
        python_system.h
        localisation.h
        localisation.cpp
        localisationtype.h
)
