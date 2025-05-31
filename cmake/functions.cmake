function(add_WGCtarget target_name target_type)
    # 剩下的参数是源文件列表
    set(options)
    set(oneValueArgs)
    set(multiValueArgs PSRCS PHEADERS PUIS)
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
    target_include_directories(${target_name} PRIVATE "${CMAKE_SOURCE_DIR}/inc/${target_name}")
    target_include_directories(${target_name} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/inc")
endfunction()

