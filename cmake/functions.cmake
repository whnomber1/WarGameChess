function(add_WGCtarget target_name target_type)
    # 剩下的参数是源文件列表
    set(options)
    set(oneValueArgs OUTPUTDIR)
    set(multiValueArgs PSRCS PHEADERS PUIS INCLUDEDIRS LKLIBS PROLKLIBS)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # 获取剩余的源文件（排除前两个参数）
    set(srcs) # 先清空
    if(ARG_PSRCS)
        list(APPEND srcs ${ARG_PSRCS})
    endif()
    if(ARG_PHEADERS)
        # list(TRANSFORM ARG_PHEADERS PREPEND "${CMAKE_SOURCE_DIR}/inc/${target_name}/")
        list(APPEND srcs ${ARG_PHEADERS})
    endif()
    if(ARG_PUIS)
        # list(TRANSFORM ARG_PUIS PREPEND "${CMAKE_SOURCE_DIR}/ui/${target_name}/")
        list(APPEND srcs ${ARG_PUIS})
    endif()

    list(LENGTH srcs srcs_length)
    if(srcs_length EQUAL 0)
        # 用户没用 SRCS，那就把 ${ARGN} 全部当作源文件
        set(srcs ${ARGN})
    endif()
    message("${target_name} src: ${srcs}")
    # 创建目标
    if(${target_type} STREQUAL "EXECUTABLE")
        add_executable(${target_name} ${srcs})
    elseif(${target_type} STREQUAL "STATIC")
        add_library(${target_name} STATIC ${srcs})
    elseif(${target_type} STREQUAL "SHARED")
        add_library(${target_name} SHARED ${srcs})
    else()
        message(FATAL_ERROR "Unsupported target type: ${target_type}")
    endif()
    target_include_directories(${target_name} PRIVATE "${CMAKE_SOURCE_DIR}/inc")
    target_include_directories(${target_name} PRIVATE "${CMAKE_SOURCE_DIR}/inc/WGCCommon")
    target_include_directories(${target_name} PRIVATE "${CMAKE_SOURCE_DIR}/inc/${target_name}")
    target_include_directories(${target_name} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/inc")
    target_link_directories(${target_name} PRIVATE "${CMAKE_SOURCE_DIR}/libs")
    if(ARG_INCLUDEDIRS)
        list(TRANSFORM ARG_INCLUDEDIRS PREPEND "${CMAKE_SOURCE_DIR}/")
        target_include_directories(${target_name} PRIVATE ${ARG_INCLUDEDIRS})
    endif()
    if(ARG_LKLIBS)
        target_link_libraries(${target_name} PRIVATE ${ARG_LKLIBS})
    endif()

    if(ARG_PROLKLIBS)
        foreach(PROLB ${ARG_PROLKLIBS})
            target_include_directories(${target_name} PRIVATE
                "${CMAKE_SOURCE_DIR}/src/${PROLB}/inc"
                "${CMAKE_SOURCE_DIR}/inc/${PROLB}"
            )
        endforeach()
        target_link_libraries(${target_name} PRIVATE ${ARG_PROLKLIBS})
    endif()

    set_target_properties(${target_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
    set_target_properties(${target_name} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    )
    set_target_properties(${target_name} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    )
    if(ARG_PUIS)
        set_target_properties(${target_name} PROPERTIES
            AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui
        )
    endif()
    if(${target_type} STREQUAL "SHARED")
        # set(LIBRARY_NAME ${target_name})
        string(TOUPPER ${target_name} LIBRARY_NAME)

        # 生成导出头文件
        configure_file(
          ${CMAKE_SOURCE_DIR}/cmake/mylib_export.h.in
          ${CMAKE_SOURCE_DIR}/inc/${target_name}/${target_name}_export.h
          @ONLY
        )
        target_compile_definitions(${target_name} PRIVATE ${LIBRARY_NAME}_BUILD)
    endif()

    # if(ARG_OUTPUTDIR)
    #     if(${target_type} STREQUAL "EXECUTABLE")
    #         set_target_properties(${target_name} PROPERTIES
    #             RUNTIME_OUTPUT_DIRECTORY ${ARG_OUTPUTDIR}
    #         )
    #     elseif(${target_type} STREQUAL "SHARED")
    #         set_target_properties(${target_name} PROPERTIES
    #             LIBRARY_OUTPUT_DIRECTORY ${ARG_OUTPUTDIR}
    #         )
    #     elseif(${target_type} STREQUAL "STATIC")
    #         set_target_properties(${target_name} PROPERTIES
    #             ARCHIVE_OUTPUT_DIRECTORY ${ARG_OUTPUTDIR}
    #         )
    #     endif()
    # endif()
endfunction()

