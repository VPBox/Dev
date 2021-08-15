#!/bin/bash

if [ -z "$ANDROID_BUILD_TOP" ]; then
    echo "Android build environment not set"
    exit -1
fi

echo "waiting for device"
adb root && adb wait-for-device remount

adb shell /data/nativetest/resampler_tests/resampler_tests
adb shell /data/nativetest64/resampler_tests/resampler_tests
