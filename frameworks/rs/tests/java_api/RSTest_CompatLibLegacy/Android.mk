#
# Copyright (C) 2013 The Android Open Source Project
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
#

# Bug: 22693954
# Temporarily disable this build because it doesn't work for 64-bit builds
# with mma on even a separate directory.
ifeq (0,1)

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := tests

LOCAL_SRC_FILES := $(call all-java-files-under, src) $(call all-renderscript-files-under, src)

LOCAL_PACKAGE_NAME := RSTest_CompatLegacy

LOCAL_STATIC_JAVA_LIBRARIES := \
    android-support-v8-renderscript

LOCAL_SDK_VERSION := 8
LOCAL_RENDERSCRIPT_TARGET_API := 18
LOCAL_RENDERSCRIPT_COMPATIBILITY := 18

LOCAL_RENDERSCRIPT_CC := $(LLVM_RS_CC)
LOCAL_RENDERSCRIPT_INCLUDES_OVERRIDE := \
    $(TOPDIR)external/clang/lib/Headers \
    $(TOPDIR)frameworks/rs/script_api/include

LOCAL_RENDERSCRIPT_FLAGS := -rs-package-name=androidx.renderscript
LOCAL_JNI_SHARED_LIBRARIES := librsjni
LOCAL_32_BIT_ONLY := true

include $(BUILD_PACKAGE)

endif  # ifeq (0,1)
