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
# This is the shared library included by the JNI test app.
#
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk

LOCAL_MODULE := libhellocomputendk
LOCAL_MODULE_TAGS := tests
LOCAL_SRC_FILES := helloComputeNDK.cpp mono.rs

LOCAL_C_INCLUDES := $(JNI_H_INCLUDE)

LOCAL_CFLAGS := -Wall -Werror
LOCAL_LDFLAGS := -Wl,-Bsymbolic
LOCAL_SHARED_LIBRARIES := libdl liblog libjnigraphics
LOCAL_STATIC_LIBRARIES := libRScpp_static

LOCAL_SDK_VERSION := current

LOCAL_NDK_STL_VARIANT := c++_static

include $(BUILD_SHARED_LIBRARY)
