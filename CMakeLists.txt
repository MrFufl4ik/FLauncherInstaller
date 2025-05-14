cmake_minimum_required(VERSION 3.31)
project(FLauncherInstaller)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXE_LINKER_FLAGS "-static")

find_package(CURL REQUIRED)
find_package(libzip REQUIRED)

add_executable(FLauncherInstaller main.cpp
        log_system.cpp
        log_system.h
        python_system.cpp
        python_system.h
        localisation.h
        localisation.cpp
        localisationtype.h
        src/install_entity/install_entity.cpp
        src/install_entity/install_entity.h
        src/install_entity/python_install_entity.cpp
        src/install_entity/python_install_entity.h
)
target_link_libraries(FLauncherInstaller PRIVATE CURL::libcurl_static)
target_link_libraries(FLauncherInstaller PRIVATE libzip::zip)
target_link_libraries(FLauncherInstaller PRIVATE ${ZLIB_LIBRARIES})