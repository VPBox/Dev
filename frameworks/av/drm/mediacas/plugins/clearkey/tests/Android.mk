#
# Copyright (C) 2017 The Android Open Source Project
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
LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    ClearKeyFetcherTest.cpp

LOCAL_MODULE := ClearKeyFetcherTest
LOCAL_VENDOR_MODULE := true

# LOCAL_LDFLAGS is needed here for the test to use the plugin, because
# the plugin is not in standard library search path. Without this .so
# loading fails at run-time (linking is okay).
LOCAL_LDFLAGS := \
    -Wl,--rpath,\$${ORIGIN}/../../../system/vendor/lib/mediacas -Wl,--enable-new-dtags

LOCAL_SHARED_LIBRARIES := \
    libutils libclearkeycasplugin libstagefright_foundation libprotobuf-cpp-lite liblog

LOCAL_C_INCLUDES += \
    $(TOP)/frameworks/av/drm/mediacas/plugins/clearkey \
    $(TOP)/frameworks/av/include \
    $(TOP)/frameworks/native/include/media \

LOCAL_MODULE_TAGS := tests

include $(BUILD_NATIVE_TEST)



