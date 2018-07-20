macro(set_internal_variable varName varDesc)
    if("${${varName}}" STREQUAL "")
        if("$ENV{${varName}}" STREQUAL "")
            message(FATAL_ERROR
                "Please define environment variable ${varName}")
        endif()
        set(${varName} $ENV{${varName}} CACHE INTERNAL ${varDesc})
    endif()
endmacro()

macro(import_toolchain_config)
    get_property(IN_TRY_COMPILE GLOBAL PROPERTY IN_TRY_COMPILE)
    set(TOOLCHAIN_CONFIG_FILE "toolchain.config.cmake")
    if(IN_TRY_COMPILE)
        # inherit settings in recursive loads
        include("${CMAKE_CURRENT_SOURCE_DIR}/../../${TOOLCHAIN_CONFIG_FILE}"
            OPTIONAL)
    endif()
endmacro()

macro(export_toolchain_config)
    if(NOT IN_TRY_COMPILE)
        # export toolchain settings for the try_compile() command
        set(__toolchain_config "")
        foreach(__var ${ARGN})
            if(DEFINED ${__var})
                if(${__var} MATCHES " ")
                    set(__set_var
                        "set(${__var} \"${${__var}}\" CACHE INTERNAL \"\")")
                else()
                    set(__set_var
                        "set(${__var} ${${__var}} CACHE INTERNAL \"\")")
                endif()
                set(__toolchain_config
                    "${__toolchain_config}${__set_var}\n")
            endif()
        endforeach()
        file(WRITE "${CMAKE_BINARY_DIR}/${TOOLCHAIN_CONFIG_FILE}"
            "${__toolchain_config}")
        unset(__toolchain_config)
    endif()
endmacro()

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)

if(CMAKE_TOOLCHAIN_FILE)
    # touch toolchain variable to suppress "unused variable" warning
endif()

set(CMAKE_SIZEOF_VOID_P 32)

# Neet to enable, otherwise always update binaries during installation
set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_RPATH}"
    "${TARGET_ROOTFS_DIR}/lib"
    "${TARGET_ROOTFS_DIR}/usr/lib"
    "${TARGET_ROOTFS_DIR}/usr/local/lib")

set(CMAKE_FIND_ROOT_PATH "${TARGET_ROOTFS_DIR}")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)

include_directories(SYSTEM "${TARGET_ROOTFS_DIR}/usr/include")

set(IMX6_FLAGS
    "-mthumb"
    "-march=armv7-a"
    "-mcpu=cortex-a9"
    "-mtune=cortex-a9"
    "-mfpu=neon"
    "-mfloat-abi=softfp"
    CACHE STRING "i.MX6 related flags")
string(REPLACE ";" " " IMX6_FLAGS "${IMX6_FLAGS}")

set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

set(ELINUX TRUE)
add_definitions(-DLINUX)
add_definitions(-DIMX6)
