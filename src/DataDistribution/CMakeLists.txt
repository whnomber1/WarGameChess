set(CURPRONAME DataDistribution)

set(PROJECT_HEADERS
    inc/CJsonVariant.h
    inc/DataDistributionClient.h
    # inc/DeduceMain.h
    # inc/SimulationEngine.h
    # inc/SimulationAdvancedEngine.h
)

set(PROJECT_SOURCES
    # src/main.cpp
    src/CJsonVariant.cpp
    src/DataDistributionClient.cpp
    # test/test2.cpp
    # test/test3.cpp
)

set(PROJECT_UIS
)

set(ExternLibsSuffix)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(ExternLibsSuffix "d")
endif()

add_WGCtarget(${CURPRONAME} SHARED
   PHEADERS ${PROJECT_HEADERS}
    PSRCS ${PROJECT_SOURCES}
#    PUIS ${PROJECT_UIS}
    INCLUDEDIRS inc/QtQmqtt
    LKLIBS Qt5::Core Qt5::Widgets Qt5::Network Qt5Qmqtt${ExternLibsSuffix}
    OUTPUTDIR ${CMAKE_BINARY_DIR}/bin
)

# target_link_libraries(${CURPRONAME} PRIVATE Qt5::Widgets)
# target_link_libraries(${CURPRONAME} PRIVATE winmm)
# target_link_libraries(${CURPRONAME} PRIVATE Qt5Qmqtt)

