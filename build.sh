#!/bin/bash
BOLD='\033[1m'
GRN='\033[01;32m'
CYAN='\033[0;36m'
YELLOW='\033[1;33m'
RED='\033[01;31m'
RST='\033[0m'


add_deps() {
  RED='\033[01;31m'
  echo "Cloning dependencies if they don't exist...."
  sudo apt-get install -y ccache cpio libarchive-tools 
  if [ ! -d build-shit ]
  then
    mkdir build-shit
  fi

  if [ ! -d build-shit/clang ]
  then
    echo "Downloading proton-clang...."
    cd build-shit;
    wget https://github.com/kdrag0n/proton-clang/archive/refs/tags/20201212.tar.gz -O clang.tar.gz; 
    bsdtar xf clang.tar.gz;
    mv proton-clang-20201212 clang
    cd ../
  fi

  if [ ! -d build-shit/gcc32 ]
  then
    echo "Downloading gcc32...."
    git clone --depth=1 git://github.com/LineageOS/android_prebuilts_gcc_linux-x86_arm_arm-linux-androideabi-4.9 build-shit/gcc32
  fi

  if [ ! -d build-shit/gcc ]; then
    echo "Downloading gcc...."
    git clone --depth=1 https://android.googlesource.com/platform/prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9 build-shit/gcc
  fi

  if [ ! -d build-shit/ccache ]; then
    echo "create folder for ccache...."
    mkdir build-shit/ccache
  fi   

  echo "Done"
}
setup_env() {
  # TC LOCAL PATH
  echo -e "${CYAN}"
  echo "Setting Up Environment"
  echo ""
  export ARCH=arm64
  export SUBARCH=arm64
  export ANDROID_MAJOR_VERSION=r
  export PLATFORM_VERSION=11.0.0

  # Export KBUILD flags
  export KBUILD_BUILD_USER=Dark-Matter7232
  export KBUILD_BUILD_HOST=darkmachine

  # CCACHE
  export CCACHE="$(which ccache)"
  export USE_CCACHE=1
  export CCACHE_EXEC="build-shit/ccache"
  ccache -M 50G
  echo "done"
}
function compile() {
  read -p "Write the Kernel version: " KV
  local IMAGE="$(pwd)/arch/arm64/boot/Image"
  make clean
  make mrproper
  make -j$((`nproc`+1)) M21_defconfig
  make -j$((`nproc`+1)) | tee $(date +"%H-%M")-log.txt
  SUCCESS=$?
  echo -e "${RST}"

  if [ $SUCCESS -eq 0 ] && [ -f "$IMAGE" ]
  then
    echo -e "${GRN}"
    echo "------------------------------------------------------------"
    echo "Compilation successful..."
    echo "Image can be found at arch/arm64/boot/Image"
    echo  "------------------------------------------------------------"
    echo -e "${RST}"
  else
    echo -e "${RED}"
    echo "------------------------------------------------------------"
    echo "Compilation failed..check build logs for errors"
    echo "------------------------------------------------------------"
    echo -e "${RST}"
  fi

}
zip() {
  echo -e "${GRN}"
  rm -rf output/*
  rm -rf CosmicStaff/AK/Image
  rm -rf output/Cos*
  cp -r arch/arm64/boot/Image CosmicStaff/AK/Image
  cd CosmicStaff/AK
  bash zip.sh
  cd ../..
  cp -r CosmicStaff/AK/1*.zip output/CosmicStaff-ONEUI-$KV-M21.zip
  rm CosmicStaff/AK/*.zip
  rm CosmicStaff/AK/Image
}

upload() {
  cd output
  wget https://temp.sh/up.sh
  chmod +x up.sh
  echo -e "${RED}"
  ./up.sh Cos*
  cd ../
}
add_deps
setup_env
compile
zip
upload