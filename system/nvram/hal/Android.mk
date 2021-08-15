#
# Copyright (C) 2016 The Android Open-Source Project
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

LOCAL_PATH := $(call my-dir)

# fake_nvram is a system daemon that provides a software-only access-controlled
# NVRAM implementation. This is only for illustration and in order to get code
# using access-controlled NVRAM running on emulators. It *DOES NOT* meet the
# tamper evidence requirements, so can't be used on production devices.
include $(CLEAR_VARS)
LOCAL_MODULE := fake-nvram
LOCAL_SRC_FILES := \
	fake_nvram.cpp \
	fake_nvram_storage.cpp
LOCAL_CLANG := true
LOCAL_CFLAGS := -Wall -Werror -Wextra
LOCAL_STATIC_LIBRARIES := libnvram-core
LOCAL_SHARED_LIBRARIES := \
	libnvram-messages \
	libcrypto \
	libminijail \
	liblog \
	libcutils \
	libbase
LOCAL_INIT_RC := fake-nvram.rc

LOCAL_REQUIRED_MODULES_arm := fake-nvram-seccomp.policy
LOCAL_REQUIRED_MODULES_arm64 := fake-nvram-seccomp.policy
LOCAL_REQUIRED_MODULES_x86 := fake-nvram-seccomp.policy
LOCAL_REQUIRED_MODULES_x86_64 := fake-nvram-seccomp.policy

LOCAL_MODULE_TAGS := optional
include $(BUILD_EXECUTABLE)

# seccomp policy for fake_nvram.
include $(CLEAR_VARS)
LOCAL_MODULE := fake-nvram-seccomp.policy
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT)/usr/share/policy/
LOCAL_SRC_FILES := fake-nvram-seccomp-$(TARGET_ARCH).policy
LOCAL_MODULE_TARGET_ARCH := arm arm64 x86 x86_64
include $(BUILD_PREBUILT)
