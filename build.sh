#!/bin/bash

# Initialize variables

BOLD='\033[1m'
GRN='\033[01;32m'
CYAN='\033[0;36m'
YELLOW='\033[1;33m'
RED='\033[01;31m'
RST='\033[0m'
ORIGIN_DIR=$(pwd)
BUILD_PREF_COMPILER='clang'
BUILD_PREF_COMPILER_VERSION='proton'
TOOLCHAIN=$(pwd)/build-shit/toolchain
# export environment variables
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
    ccache -M 5GB
    export CROSS_COMPILE=aarch64-linux-gnu-
    export CROSS_COMPILE_ARM32=arm-linux-gnueabi-
    export CC=${BUILD_PREF_COMPILER}
}

script_echo() {
    echo "  $1"
}
exit_script() {
    kill -INT $$
}
add_deps() {
    echo -e "${CYAN}"
    if [ ! -d $(pwd)/build-shit ]
    then
        script_echo "Create build-shit folder"
        mkdir $(pwd)/build-shit
    fi
    
    if [ ! -d $(pwd)/build-shit/toolchain ]
    then
        script_echo "Downloading proton-clang...."
        git clone https://github.com/TenSeventy7/exynos9610_toolchains_fresh.git ${TOOLCHAIN} --single-branch -b ${BUILD_PREF_COMPILER_VERSION} --depth 1 2>&1 | sed 's/^/     /'
        sudo mkdir -p /root/build/install/aarch64-linux-gnu
        sudo cp -r "${TOOLCHAIN}/lib" /root/build/install/aarch64-linux-gnu/
        sudo chown $(whoami) /root
        sudo chown $(whoami) /root/build
        sudo chown $(whoami) /root/build/install
        sudo chown $(whoami) /root/build/install/aarch64-linux-gnu
        sudo chown $(whoami) /root/build/install/aarch64-linux-gnu/lib
    fi
    verify_toolchain_install
}
verify_toolchain_install() {
    sleep 2
    script_echo " "
    if [[ -d "${TOOLCHAIN}" ]]; then
        script_echo "I: Toolchain found at default location"
        export PATH="${TOOLCHAIN}/bin:$PATH"
        export LD_LIBRARY_PATH="${TOOLCHAIN}/lib:$LD_LIBRARY_PATH"
    else
        script_echo "I: Toolchain not found"
        script_echo "   Downloading recommended toolchain at ${TOOLCHAIN}..."
        add_deps
    fi
}
build_kernel_image() {
    sleep 3
    script_echo " "
    read -p "Enter Device name: " DEVICE
    read -p "Write the Kernel version: " KV
    
    if [[ ${DEVICE} == 'M21' ]]; then
        script_echo 'Building CosmicStaff Kernel For M21'
        sleep 3
        make -C $(pwd) CC=${BUILD_PREF_COMPILER} AR=llvm-ar NM=llvm-nm OBJCOPY=llvm-objcopy OBJDUMP=llvm-objdump STRIP=llvm-strip -j$((`nproc`+1)) M21_defconfig 2>&1 | sed 's/^/     /'
        make -C $(pwd) CC=${BUILD_PREF_COMPILER} AR=llvm-ar NM=llvm-nm OBJCOPY=llvm-objcopy OBJDUMP=llvm-objdump STRIP=llvm-strip -j$((`nproc`+1)) 2>&1 | sed 's/^/     /'
    else
        script_echo 'Building CosmicStaff Kernel For M31'
        sleep 3
        make -C $(pwd) CC=${BUILD_PREF_COMPILER} AR=llvm-ar NM=llvm-nm OBJCOPY=llvm-objcopy OBJDUMP=llvm-objdump STRIP=llvm-strip -j$((`nproc`+1)) M31_defconfig 2>&1 | sed 's/^/     /'
        make -C $(pwd) CC=${BUILD_PREF_COMPILER} AR=llvm-ar NM=llvm-nm OBJCOPY=llvm-objcopy OBJDUMP=llvm-objdump STRIP=llvm-strip -j$((`nproc`+1)) 2>&1 | sed 's/^/     /'
    fi
}
build_flashable_zip() {
    if [[ -e "$(pwd)/arch/arm64/boot/Image" ]]; then
        script_echo " "
        script_echo "I: Building kernel image..."
        echo -e "${GRN}"
        rm -rf $(pwd)/output/*
        rm -rf $(pwd)/CosmicStaff/AK/Image
        rm -rf $(pwd)/CosmicStaff/AK/*.zip
        cp -r $(pwd)/arch/arm64/boot/Image $(pwd)/CosmicStaff/AK/Image
        cd $(pwd)/CosmicStaff/AK
        bash zip.sh
        cd ../..
        if [[ ${DEVICE} == 'M21' ]]; then
            cp -r $(pwd)/CosmicStaff/AK/1*.zip $(pwd)/output/CosmicStaff-ONEUI-$KV-M21.zip
            cd $(pwd)/output
            wget -q https://temp.sh/up.sh
            chmod +x up.sh
            echo -e "${RED}"
            ./up.sh Cos* 2>&1 | sed 's/^/     /'
            cd ../
        else
            cp -r $(pwd)/CosmicStaff/AK/1*.zip $(pwd)/output/CosmicStaff-ONEUI-$KV-M31.zip
            cd $(pwd)/output
            wget -q https://temp.sh/up.sh
            chmod +x up.sh
            echo -e "${RED}"
            ./up.sh Cos* 2>&1 | sed 's/^/     /'
            cd ../
        fi
        if [[ ! -f ${ORIGIN_DIR}/CosmicStaff/AK/Image ]]; then
            echo -e "${RED}"
            script_echo " "
            script_echo "E: Kernel image not built successfully!"
            script_echo "   Errors can be fround from above."
            sleep 3
            exit_script
        else
            rm -f $(pwd)/arch/arm64/boot/Image
            rm -f $(pwd)/CosmicStaff/AK/{Image, *.zip}
            rm -f $(pwd)/output/*
        fi
        
    else
        echo -e "${RED}"
        script_echo "E: Image not built!"
        script_echo "   Errors can be fround from above."
        sleep 3
        exit_script
    fi
}
add_deps
export_env_vars
build_kernel_image
build_flashable_zip