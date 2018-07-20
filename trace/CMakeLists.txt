project(TRACE)
cmake_minimum_required(VERSION 3.0)

# Options

    option(TRACE_ENABLED "Enable tracing" OFF)

# Targets

    add_library(trace
        source/utils.cpp
    )

# Flags

    target_compile_definitions(trace
        PUBLIC "TRACE_ENABLED=$<BOOL:${TRACE_ENABLED}>")

    target_compile_options(trace
        PRIVATE
            -fPIC
            -std=c++11
    )

# Includes

    target_include_directories(trace PUBLIC include)
