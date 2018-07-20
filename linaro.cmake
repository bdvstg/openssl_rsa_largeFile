include(${CMAKE_CURRENT_LIST_DIR}/prosense.cmake)

if(CMAKE_VERSION VERSION_LESS 3.6)
    include(CMakeForceCompiler)
    CMAKE_FORCE_C_COMPILER(arm-linux-gnueabi-gcc GNU)
    CMAKE_FORCE_CXX_COMPILER(arm-linaro-gnueabi-g++ GNU)
endif()

import_toolchain_config()

set_internal_variable(LINARO_TOOLCHAIN_DIR "Linaro toolchain directory")
set_internal_variable(TARGET_ROOTFS_DIR "Target root filesystem")

set(TARGET_TOOLCHAIN_PREFIX
    "${LINARO_TOOLCHAIN_DIR}/bin/arm-linux-gnueabi-")

set(CMAKE_C_COMPILER "${TARGET_TOOLCHAIN_PREFIX}gcc")
set(CMAKE_CXX_COMPILER "${TARGET_TOOLCHAIN_PREFIX}g++")

set(TARGET_LINKER_FLAGS         "-L${TARGET_ROOTFS_DIR}/usr/lib"
    CACHE STRING "Prosense specifig linker flags")
set(TARGET_LINKER_FLAGS
    "${TARGET_LINKER_FLAGS} -Wl,-rpath -Wl,${TARGET_ROOTFS_DIR}/usr/lib")
set(TARGET_LINKER_FLAGS
    "${TARGET_LINKER_FLAGS} -L${TARGET_ROOTFS_DIR}/lib")
set(TARGET_LINKER_FLAGS
    "${TARGET_LINKER_FLAGS} -Wl,-rpath -Wl,${TARGET_ROOTFS_DIR}/lib")

set(CMAKE_CXX_FLAGS "${IMX6_CXX_FLAGS}" CACHE STRING "C++ flags")

set(CMAKE_EXE_LINKER_FLAGS      "${TARGET_LINKER_FLAGS}"
    CACHE STRING "Executable linker flags")
set(CMAKE_SHARED_LINKER_FLAGS   "${TARGET_LINKER_FLAGS}"
    CACHE STRING "Shared linker flags")
set(CMAKE_MODULE_LINKER_FLAGS   "${TARGET_LINKER_FLAGS}"
    CACHE STRING "Module linker flags")

set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

set(ELINUX TRUE)
add_definitions(-DLINUX)

export_toolchain_config(
    LINARO_TOOLCHAIN_DIR
    TARGET_ROOTFS_DIR
    TARGET_LINKER_FLAGS
)
