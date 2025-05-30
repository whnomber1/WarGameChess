cmake_minimum_required(VERSION 3.16)
include(cmake/functions.cmake)

project(WarGameChess VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)

