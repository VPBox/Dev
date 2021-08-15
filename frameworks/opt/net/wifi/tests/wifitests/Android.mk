# Copyright (C) 2015 The Android Open Source Project
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

LOCAL_PATH:= $(call my-dir)

# Make test APK
# ============================================================
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := tests

LOCAL_SRC_FILES := $(call all-subdir-java-files)

# Provide jack a list of classes to exclude form code coverage
# This list is generated from the java source files in this module
# The list is a comma separated list of class names with * matching zero or more characters.
# Example:
#   Input files: src/com/android/server/wifi/Test.java src/com/android/server/wifi/AnotherTest.java
#   Generated exclude list: com.android.server.wifi.Test*,com.android.server.wifi.AnotherTest*

# Filter all src files to just java files
local_java_files := $(filter %.java,$(LOCAL_SRC_FILES))
# Transform java file names into full class names.
# This only works if the class name matches the file name and the directory structure
# matches the package.
local_classes := $(subst /,.,$(patsubst src/%.java,%,$(local_java_files)))
# Convert class name list to jacoco exclude list
# This appends a * to all classes and replace the space separators with commas.
# These patterns will match all classes in this module and their inner classes.
jacoco_exclude := $(subst $(space),$(comma),$(patsubst %,%*,$(local_classes)))

jacoco_include := com.android.server.wifi.*

LOCAL_JACK_COVERAGE_INCLUDE_FILTER := $(jacoco_include)
LOCAL_JACK_COVERAGE_EXCLUDE_FILTER := $(jacoco_exclude)

LOCAL_DX_FLAGS := --multi-dex
LOCAL_JACK_FLAGS := --multi-dex native

# wifi-service and services must be included here so that the latest changes
# will be used when tests. Otherwise the tests would run against the installed
# system.
# TODO figure out if this is the correct thing to do, this seems to not be right
# since neither is declared a static java library.
LOCAL_STATIC_JAVA_LIBRARIES := \
	androidx.test.rules hamcrest-library \
	mockito-target-extended-minus-junit4 \
	frameworks-base-testutils \
	services \
	wifi-service \
	truth-prebuilt \

LOCAL_JAVA_LIBRARIES := \
	android.test.runner \
	android.hidl.manager-V1.2-java \
	android.test.base \
	android.test.mock

# These must be explicitly included because they are not normally accessible
# from apps.
LOCAL_JNI_SHARED_LIBRARIES := \
	libcrypto \
	libwifi-service \
	libEGL \
	libGLESv2 \
	libaudioutils \
	libbacktrace \
	libbase \
	libbinder \
	libbinderthreadstate \
	libc++ \
	ld-android \
	libdl_android \
	libcamera_client \
	libcamera_metadata \
	libcgrouprc \
	libcutils \
	libexpat \
	libgui \
	libhardware \
	libandroidicu \
	libjsoncpp \
	liblzma \
	libmedia \
	libnativehelper \
	libnbaio \
	libnetutils \
	libnl \
	libpowermanager \
	libprocessgroup \
	libsonivox \
	libstagefright_foundation \
	libstdc++ \
	libsync \
	libwifi-system \
	libui \
	libunwindstack \
	libutils \
	libvndksupport \
	libdexmakerjvmtiagent \
	libstaticjvmtiagent

ifdef WPA_SUPPLICANT_VERSION
LOCAL_JNI_SHARED_LIBRARIES += libwpa_client
endif

LOCAL_PACKAGE_NAME := FrameworksWifiTests
LOCAL_PRIVATE_PLATFORM_APIS := true

LOCAL_COMPATIBILITY_SUITE := device-tests

include $(BUILD_PACKAGE)
