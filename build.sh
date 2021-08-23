#!/bin/bash
BOLD='\033[1m'
GRN='\033[01;32m'
CYAN='\033[0;36m'
YELLOW='\033[1;33m'
RED='\033[01;31m'
RST='\033[0m'

ORIGIN_DIR=$(pwd)
# Toolchain options
BUILD_PREF_COMPILER='clang'
BUILD_PREF_COMPILER_VERSION='proton'
# Local toolchain directory
TOOLCHAIN=$(pwd)/build-shit/toolchain

export_env_vars() {
    export KBUILD_BUILD_USER=Dark-Matter7232
    export KBUILD_BUILD_HOST=darkmachine
    
    export ARCH=arm64
    export SUBARCH=arm64
    export ANDROID_MAJOR_VERSION=r
    export PLATFORM_VERSION=11.0.0
    export $ARCH
    
    # CCACHE
    export CCACHE="$(which ccache)"
    export USE_CCACHE=1
    export CCACHE_EXEC="build-shit/ccache"
    ccache -M 4GB
}

script_echo() {
    echo "  $1"
}

exit_script() {
    kill -INT $$
}

add_deps() {
    echo -e "${CYAN}"
    if [ ! -d build-shit ]
    then
        script_echo "Create build-shit folder"
        mkdir build-shit
    fi
    
    if [ ! -d build-shit/ccache ]; then
        script_echo "create folder for ccache...."
        mkdir build-shit/ccache
    fi
    
    if [ ! -d build-shit/toolchain ]
    then
        script_echo "Downloading proton-clang...."
        script_echo $(wget -q --show-progress https://github.com/kdrag0n/proton-clang/archive/refs/tags/20201212.tar.gz -O clang.tar.gz);
        bsdtar xf clang.tar.gz
        rm -rf clang.tar.gz
        mv proton-clang* build-shit/toolchain 
    fi
    verify_toolchain
}

verify_toolchain() {
    sleep 2
    script_echo " "
    if [ ! -d build-shit ]
    then
        mkdir -p build-shit/ccache
    fi
    if [[ -d "${TOOLCHAIN}" ]]; then
        script_echo "I: Toolchain found at default location"
        export PATH="${TOOLCHAIN}/bin:$PATH"
        export LD_LIBRARY_PATH="${TOOLCHAIN}/lib:$LD_LIBRARY_PATH"
    else
        script_echo "I: Toolchain not found at default location"
        script_echo "   Downloading recommended toolchain at ${TOOLCHAIN}..."
        add_deps
    fi
    
    # Proton Clang 13
    # export CLANG_TRIPLE=aarch64-linux-gnu-
    export CROSS_COMPILE=aarch64-linux-gnu-
    export CROSS_COMPILE_ARM32=arm-linux-gnueabi-
    export CC=${BUILD_PREF_COMPILER}
}

build_kernel() {
    sleep 3
    script_echo " "
    
    if [[ ${BUILD_PREF_COMPILER_VERSION} == 'proton' ]]; then
        make -C $(pwd) CC=${BUILD_PREF_COMPILER} AR=llvm-ar NM=llvm-nm OBJCOPY=llvm-objcopy OBJDUMP=llvm-objdump STRIP=llvm-strip -j$((`nproc`+1)) M21_defconfig 2>&1 | sed 's/^/     /'
        make -C $(pwd) CC=${BUILD_PREF_COMPILER} AR=llvm-ar NM=llvm-nm OBJCOPY=llvm-objcopy OBJDUMP=llvm-objdump STRIP=llvm-strip -j$((`nproc`+1)) 2>&1 | sed 's/^/     /'
    else
        make -C $(pwd) CC=${BUILD_PREF_COMPILER} LLVM=1 ${BUILD_DEVICE_TMP_CONFIG} LOCALVERSION="${LOCALVERSION}" 2>&1 | sed 's/^/     /'
        make -C $(pwd) CC=${BUILD_PREF_COMPILER} LLVM=1 -j$(nproc --all) LOCALVERSION="${LOCALVERSION}" 2>&1 | sed 's/^/     /'
    fi
}

build_image() {
    if [[ -e "$(pwd)/arch/arm64/boot/Image" ]]; then
        script_echo " "
        read -p "Write the Kernel version: " KV
        script_echo "I: Building kernel image..."
        echo -e "${GRN}"
        rm -rf output/*
        rm -rf CosmicStaff/AK/Image
        rm -rf output/Cos*
        cp -r arch/arm64/boot/Image CosmicStaff/AK/Image
        cd CosmicStaff/AK
        bash zip.sh
        cd ../..
        cp -r CosmicStaff/AK/1*.zip output/CosmicStaff-ONEUI-$KV-M21.zip
        cd output
        wget -q https://temp.sh/up.sh 
        chmod +x up.sh
        echo -e "${RED}"
        ./up.sh Cos* 2>&1 | sed 's/^/     /'
        cd ../
        if [[ ! -f ${ORIGIN_DIR}/CosmicStaff/AK/Image ]]; then
            echo -e "${RED}"
            script_echo " "
            script_echo "E: Kernel image not built successfully!"
            script_echo "   Errors can be fround from above."
            sleep 3
            exit_script
        else
            rm -f $(pwd)/arch/arm64/boot/Image
            rm -f $(pwd)/CosmicStaff/AK/Image
            rm CosmicStaff/AK/*.zip
        fi
        
    else
        echo -e "${RED}"
        script_echo "E: Image not built!"
        script_echo "   Errors can be fround from above."
        sleep 3
        exit_script
    fi
}

export_env_vars
add_deps
build_kernel
build_image

# Build variables - DO NOT CHANGE
VERSION=$(grep -m 1 VERSION "$(pwd)/Makefile" | sed 's/^.*= //g')
PATCHLEVEL=$(grep -m 1 PATCHLEVEL "$(pwd)/Makefile" | sed 's/^.*= //g')
SUBLEVEL=$(grep -m 1 SUBLEVEL "$(pwd)/Makefile" | sed 's/^.*= //g')
