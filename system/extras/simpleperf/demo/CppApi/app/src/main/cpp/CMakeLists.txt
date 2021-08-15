cmake_minimum_required(VERSION 3.4.1)
project(CppApiDemo LANGUAGES CXX)

add_library(
        native-lib
        SHARED
        native-lib.cpp
        ../../../../../../app_api/cpp/simpleperf.cpp)

target_include_directories(native-lib
        PRIVATE
        ../../../../../../app_api/cpp/)

find_library(
        log-lib
        log)

target_link_libraries(
        native-lib
        PRIVATE
        ${log-lib})