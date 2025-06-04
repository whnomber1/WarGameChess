set(CURPRONAME DeduceClient)

set(PROJECT_HEADERS
    inc/MainWindow.h
)

set(PROJECT_SOURCES
    src/main.cpp
    src/MainWindow.cpp
)

set(PROJECT_UIS
    ui/MainWindow.ui
)

set(ExternLibsSuffix)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(ExternLibsSuffix "d")
endif()

add_WGCtarget(${CURPRONAME} EXECUTABLE
    PHEADERS ${PROJECT_HEADERS}
    PSRCS ${PROJECT_SOURCES}
    PUIS ${PROJECT_UIS}
    INCLUDEDIRS inc/QtQmqtt
    LKLIBS Qt5::Core Qt5::Widgets Qt5::Network
    OUTPUTDIR ${CMAKE_BINARY_DIR}/bin
)

# target_link_libraries(${CURPRONAME} PRIVATE Qt5::Widgets)
# target_link_libraries(${CURPRONAME} PRIVATE winmm)
# target_link_libraries(${CURPRONAME} PRIVATE Qt5Qmqtt)

