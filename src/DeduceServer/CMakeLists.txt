set(CURPRONAME DeduceServer)

set(PROJECT_HEADERS
    MainWindow.h
    DeduceMain.h
)

set(PROJECT_SOURCES
        main.cpp
        MainWindow.cpp
        DeduceMain.cpp
)

set(PROJECT_UIS
    MainWindow.ui
)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt5 REQUIRED COMPONENTS Widgets)

set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/ui/${CURPRONAME})



add_WGCtarget(${CURPRONAME} EXECUTABLE
    PHEADERS ${PROJECT_HEADERS}
    PSRCS ${PROJECT_SOURCES}
    PUIS ${PROJECT_UIS}
)

target_link_libraries(${CURPRONAME} PRIVATE Qt5::Widgets)
target_link_libraries(${CURPRONAME} PRIVATE winmm)

