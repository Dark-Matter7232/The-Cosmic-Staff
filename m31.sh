#! bin/bash


echo "Setting Up Environment"
echo ""
export ARCH=arm64
export SUBARCH=arm64
export ANDROID_MAJOR_VERSION=r
export PLATFORM_VERSION=11.0.0
export M21CHAT_ID="-1001364246024"

# Export KBUILD flags
export KBUILD_BUILD_USER=neel0210
export KBUILD_BUILD_HOST=hell

# CCACHE
export CCACHE="$(which ccache)"
export USE_CCACHE=1
export CCACHE_EXEC="/home/neel/Desktop/ccache"
ccache -M 50G
export CCACHE_COMPRESS=1

# TC LOCAL PATH
export CROSS_COMPILE=/home/neel/Desktop/toolchain/gcc/bin/aarch64-linux-android-
export CLANG_TRIPLE=/home/neel/Desktop/toolchain/clang/bin/aarch64-linux-gnu-
export CC=/home/neel/Desktop/toolchain/clang/bin/clang

echo "===="
echo "M21"
echo "===="
make clean
make mrproper
rm -rf M31
make M31_defconfig O=M31
make -j$(nproc --all) O=M31 | tee M31_Compile.log
echo "Kernel Compiled"
echo ""
rm ./PRISH/AK/Image
rm ./output/*.zip
#cp -r ./M21/arch/arm64/boot/Image ./PRISH/AK/Image
#cd PRISH/AK
#. zip.sh
#cd ../..
#cp -r ./PRISH/AK/1*.zip ./output/PrishKernel-ONEUI-R3-Ak-M21.zip
#rm ./PRISH/AK/*.zip
#rm ./PRISH/AK/Image

#changelog=`cat PRISH/changelog.txt`
#for i in output/*.zip
#do
#curl -F "document=@$i" --form-string "caption=$changelog" "https://api.telegram.org/bot${BOT_ID}/sendDocument?chat_id=${M21CHAT_ID}&parse_mode=HTML"
#done