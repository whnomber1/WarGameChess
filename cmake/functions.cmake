function(add_WGCtarget target_name target_type)
    # 剩下的参数是源文件列表
    set(options)
    set(oneValueArgs)
    set(multiValueArgs SRCS)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # 获取剩余的源文件（排除前两个参数）
    if(ARG_SRCS)
        set(srcs ${ARG_SRCS})
    else()
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
endfunction()
