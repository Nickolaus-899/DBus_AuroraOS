cmake_minimum_required(VERSION 3.20)
project(configurationManager)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(sdbus-c++ REQUIRED)

add_subdirectory(src)
