cmake_minimum_required(VERSION 3.16)
include(cmake/functions.cmake)

set(CMAKE_PREFIX_PATH "F:/wh/Qt/QtAll/5.15.2/msvc2019_64" CACHE PATH "Qt path")

project(WarGameChess VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt5 REQUIRED COMPONENTS Widgets Core Network)


add_subdirectory(src)

