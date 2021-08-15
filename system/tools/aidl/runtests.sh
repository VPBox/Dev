#!/usr/bin/env bash

# Copyright (C) 2016 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

if [ -z $ANDROID_BUILD_TOP ]; then
  echo "You need to source and lunch before you can use this script"
  exit 1
fi

echo "Running tests"
set -e # fail early

# NOTE We can't actually run these commands, since they rely on functions added
#      by build/envsetup.sh to the bash shell environment.
echo "+ mmma $ANDROID_BUILD_TOP/system/tools/aidl"
$ANDROID_BUILD_TOP/build/soong/soong_ui.bash --make-mode \
    MODULES-IN-system-tools-aidl

set -x # print commands

${ANDROID_HOST_OUT}/nativetest64/aidl_unittests/aidl_unittests

adb root
adb wait-for-device
adb remount
adb sync
adb install -r \
    ${ANDROID_PRODUCT_OUT}/system/app/aidl_test_services/aidl_test_services.apk
${ANDROID_BUILD_TOP}/system/tools/aidl/tests/integration-test.py
