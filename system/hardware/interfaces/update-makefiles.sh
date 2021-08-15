#!/bin/bash

source $ANDROID_BUILD_TOP/system/tools/hidl/update-makefiles-helper.sh

do_makefiles_update \
  "android.system:system/hardware/interfaces" \
  "android.hidl:system/libhidl/transport"

