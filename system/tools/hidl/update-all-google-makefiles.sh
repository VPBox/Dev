#!/bin/bash

# TODO(b/35570956): replace makefile generation with something like 'hidl_interface' in a Soong module

$ANDROID_BUILD_TOP/system/libhidl/update-makefiles.sh
$ANDROID_BUILD_TOP/hardware/interfaces/update-makefiles.sh
$ANDROID_BUILD_TOP/frameworks/hardware/interfaces/update-makefiles.sh
$ANDROID_BUILD_TOP/system/hardware/interfaces/update-makefiles.sh

$ANDROID_BUILD_TOP/system/tools/hidl/test/vendor/update-makefile.sh
