cmake_minimum_required(VERSION 3.0)

find_package(OpenSSL REQUIRED)

add_subdirectory(trace)

add_compile_options(-std=c++11)

include_directories(${OPENSSL_INCLUDE_DIR})

add_executable(ossl_rsautl2
    main.cpp
    openssl_rsa.cpp
    openssl_rsa.h
    file.cpp
    file.h
    )

target_link_libraries(ossl_rsautl2
    stdc++
    ${OPENSSL_LIBRARIES}
    trace
    )

install(TARGETS ossl_rsautl2 DESTINATION bin)
