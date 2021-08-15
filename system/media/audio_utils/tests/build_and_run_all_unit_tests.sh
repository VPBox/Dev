#!/bin/bash
#
# Run tests in this directory.
#

if [ -z "$ANDROID_BUILD_TOP" ]; then
    echo "Android build environment not set"
    exit -1
fi

# ensure we have mm
. $ANDROID_BUILD_TOP/build/envsetup.sh

mm

echo "waiting for device"

adb root && adb wait-for-device remount

echo "========================================"

echo "testing fdtostring"
adb push $OUT/data/nativetest/fdtostring_tests/fdtostring_tests /system/bin
adb shell /system/bin/fdtostring_tests

echo "testing primitives"
adb push $OUT/system/lib/libaudioutils.so /system/lib
adb push $OUT/data/nativetest/primitives_tests/primitives_tests /system/bin
adb shell /system/bin/primitives_tests

echo "testing power"
adb push $OUT/data/nativetest/power_tests/power_tests /system/bin
adb shell /system/bin/power_tests

echo "testing channels"
adb push $OUT/data/nativetest/channels_tests/channels_tests /system/bin
adb shell /system/bin/channels_tests

echo "string test"
adb push $OUT/data/nativetest/string_tests/string_tests /system/bin
adb shell /system/bin/string_tests

echo "format tests"
adb push $OUT/data/nativetest/format_tests/format_tests /system/bin
adb shell /system/bin/format_tests

echo "simplelog tests"
adb push $OUT/data/nativetest/simplelog_tests/simplelog_tests /system/bin
adb shell /system/bin/simplelog_tests

echo "statistics tests"
adb push $OUT/data/nativetest/statistics_tests/statistics_tests /system/bin
adb shell /system/bin/statistics_tests

echo "timestampverifier tests"
adb push $OUT/data/nativetest/timestampverifier_tests/timestampverifier_tests /system/bin
adb shell /system/bin/timestampverifier_tests

echo "variadic tests"
adb push $OUT/data/nativetest/variadic_tests/variadic_tests /system/bin
adb shell /system/bin/variadic_tests

echo "logplot tests"
adb push $OUT/data/nativetest/logplot_tests/logplot_tests /system/bin
adb shell /system/bin/logplot_tests

echo "benchmarking_statistics"
adb push $OUT/system/bin/statistics_benchmark /system/bin
adb shell /system/bin/statistics_benchmark

echo "benchmarking primitives"
adb push $OUT/system/bin/primitives_benchmark /system/bin
adb shell /system/bin/primitives_benchmark
