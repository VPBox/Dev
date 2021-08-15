#!/bin/bash

# Quit if any command produces an error.
set -e

# Build and run the CHRE simulator.
CHRE_HOST_OS=`uname`
if [[ $CHRE_HOST_OS == 'Darwin' ]]; then
JOB_COUNT=`sysctl -n hw.ncpu`
else
JOB_COUNT=$((`grep -c ^processor /proc/cpuinfo`))
fi

# Export the variant Makefile.
export CHRE_VARIANT_MK_INCLUDES=variant/android/variant.mk

make google_arm64_android_debug -j$JOB_COUNT
adb shell setenforce 0
adb push out/google_arm64_android_debug/libchre /system/bin/chre
adb push variant/android/chre-stub /vendor/bin/chre
