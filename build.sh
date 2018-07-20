#!/bin/bash


# gcc is in /D/linaro/gcc-linaro-4.7-arm-linux-gnueabi/bin/arm-linux-gnueabi-gcc
#export TARGET_ROOTFS_DIR=$PROSENSE_ROOTFS_DIR
#export LINARO_TOOLSCHAIN_DIR="/D/linaro/gcc-linaro-4.7-arm-linux-gnueabi"
#unset TARGET_ROOTFS_DIR
#unset LINARO_TOOLSCHAIN_DIR

CMAKE_INSTALL_PREFIX=""

POSITIONAL=()
while [[ $# -gt 0 ]]
do
key="$1"

case $key in
    clean)
    do_clean=true
    shift
    ;;
    config)
    do_config=true
    shift
    ;;
    build)
    do_build=true
    shift
    ;;
    install)
    do_install=true
    shift
    ;;
    prefix|--prefix)
    CMAKE_INSTALL_PREFIX="$2"
    shift
    shift
    ;;
    CROSS_COMPILE)
    CMAKE_CROSS_COMPILE="-DCMAKE_TOOLCHAIN_FILE=../linaro.cmake"
    shift
    ;;
    *)    # unknown option
    POSITIONAL+=("$1") # save it in an array for later
    shift # past argument
    ;;
esac
done
set -- "${POSITIONAL[@]}" # restore positional parameters

if [ $do_clean ]; then
    rm -r build
    mkdir build
fi

cd build

if [ $do_config ]; then
    cmake $CMAKE_CROSS_COMPILE \
        -DCMAKE_INSTALL_PREFIX=$CMAKE_INSTALL_PREFIX \
        -DTRACE_ENABLED=ON \
        ..
fi

if [ $do_build ]; then
    make
fi

if [ $do_install ]; then
    make install
fi
