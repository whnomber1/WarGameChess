set(CURPRONAME DeduceServer)

set(PROJECT_HEADERS
    inc/MainWindow.h
    inc/DeduceMain.h
    inc/SimulationEngine.h
    inc/SimulationAdvancedEngine.h
)

set(PROJECT_SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/DeduceMain.cpp
    src/SimulationEngine.cpp
    src/SimulationAdvancedEngine.cpp
    # test/test1.cpp
    # test/test2.cpp
    # test/test3.cpp
)

set(PROJECT_UIS
    ui/MainWindow.ui
)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt5 REQUIRED COMPONENTS Widgets)

# set(CMAKE_AUTOUIC_SEARCH_PATHS ui)



add_WGCtarget(${CURPRONAME} EXECUTABLE
    PHEADERS ${PROJECT_HEADERS}
    PSRCS ${PROJECT_SOURCES}
    PUIS ${PROJECT_UIS}
    LKLIBS Qt5::Widgets
    PROLKLIBS DataDistribution
)

# target_link_libraries(${CURPRONAME} PRIVATE Qt5::Widgets)
# target_link_libraries(${CURPRONAME} PRIVATE winmm)
# target_link_libraries(${CURPRONAME} PRIVATE Qt5Qmqtt)

