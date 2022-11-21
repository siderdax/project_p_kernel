#!/bin/bash
export ARCH=arm64

CPU_JOB_NUM=$(grep processor /proc/cpuinfo | awk '{field=$NF};END{print field+1}')
CLIENT=$(whoami)

KERNEL_CROSS_COMPILE_PATH="/opt/toolchains/aarch64-linux-android-4.8/bin/aarch64-linux-android-"
#make -j$CPU_JOB_NUM ARCH=arm64 mv7420_cmdlcd_release_defconfig
make  -j$CPU_JOB_NUM ARCH=arm64 CROSS_COMPILE=$KERNEL_CROSS_COMPILE_PATH

echo
echo "done"

