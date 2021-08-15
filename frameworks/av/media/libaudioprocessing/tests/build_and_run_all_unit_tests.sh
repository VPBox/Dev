#!/bin/bash

if [ -z "$ANDROID_BUILD_TOP" ]; then
    echo "Android build environment not set"
    exit -1
fi

# ensure we have mm
. $ANDROID_BUILD_TOP/build/envsetup.sh

pushd $ANDROID_BUILD_TOP/frameworks/av/media/libaudioprocessing
pwd
mm

echo "waiting for device"
adb root && adb wait-for-device remount
adb push $OUT/system/lib/libaudioprocessing.so /system/lib
adb push $OUT/system/lib64/libaudioprocessing.so /system/lib64
adb push $OUT/data/nativetest/resampler_tests/resampler_tests /data/nativetest/resampler_tests/resampler_tests
adb push $OUT/data/nativetest64/resampler_tests/resampler_tests /data/nativetest64/resampler_tests/resampler_tests

sh $ANDROID_BUILD_TOP/frameworks/av/media/libaudioprocessing/tests/run_all_unit_tests.sh

popd
