#!/bin/bash

set -ex

if [[ "$@" != *"--no-reboot"* ]]; then
    adb reboot recovery
    echo "Waiting for device to boot into recovery..."
    adb wait-for-recovery
fi
adb root
adb wait-for-recovery

# There is no /data in recovery unless mounted.
# Push test to /system directory in the recovery ramdisk
adb push ${ANDROID_PRODUCT_OUT}/data/nativetest64/vintf_object_recovery_test/vintf_object_recovery_test \
       /system/bin/vintf_object_recovery_test

adb shell /system/bin/vintf_object_recovery_test
