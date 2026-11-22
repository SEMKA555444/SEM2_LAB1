cmake_minimum_required(VERSION 3.10)
project(dbms LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(dbms main.cpp)
target_include_directories(dbms PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
