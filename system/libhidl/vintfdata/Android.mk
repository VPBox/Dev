#
# Copyright (C) 2018 The Android Open Source Project
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

# DEVICE_FRAMEWORK_MANIFEST_FILE is a device-specific framework manifest file
# that installed to the system image. HALs entries should be written to
# DEVICE_FRAMEWORK_MANIFEST_FILE or PRODUCT_MANIFEST_FILES depend on the path of
# the module. It is recommended that such device-specific modules to be
# installed on product partition.

FRAMEWORK_MANIFEST_INPUT_FILES := $(LOCAL_PATH)/manifest.xml
ifdef DEVICE_FRAMEWORK_MANIFEST_FILE
  FRAMEWORK_MANIFEST_INPUT_FILES += $(DEVICE_FRAMEWORK_MANIFEST_FILE)
endif

# VNDK Version in device compatibility matrix and framework manifest
ifeq ($(BOARD_VNDK_VERSION),current)
VINTF_VNDK_VERSION := $(PLATFORM_VNDK_VERSION)
else
VINTF_VNDK_VERSION := $(BOARD_VNDK_VERSION)
endif

# Device Compatibility Matrix
ifdef DEVICE_MATRIX_FILE
DEVICE_MATRIX_INPUT_FILE := $(DEVICE_MATRIX_FILE)
else
DEVICE_MATRIX_INPUT_FILE := $(LOCAL_PATH)/device_compatibility_matrix.default.xml
endif

include $(CLEAR_VARS)
LOCAL_MODULE        := device_compatibility_matrix.xml
LOCAL_MODULE_STEM   := compatibility_matrix.xml
LOCAL_MODULE_CLASS  := ETC
LOCAL_MODULE_PATH   := $(TARGET_OUT_VENDOR)/etc/vintf

GEN := $(local-generated-sources-dir)/compatibility_matrix.xml

$(GEN): PRIVATE_VINTF_VNDK_VERSION := $(VINTF_VNDK_VERSION)
$(GEN): PRIVATE_DEVICE_MATRIX_INPUT_FILE := $(DEVICE_MATRIX_INPUT_FILE)
$(GEN): $(DEVICE_MATRIX_INPUT_FILE) $(HOST_OUT_EXECUTABLES)/assemble_vintf
	REQUIRED_VNDK_VERSION=$(PRIVATE_VINTF_VNDK_VERSION) \
	BOARD_SYSTEMSDK_VERSIONS="$(BOARD_SYSTEMSDK_VERSIONS)" \
		$(HOST_OUT_EXECUTABLES)/assemble_vintf \
		-i $(call normalize-path-list,$(PRIVATE_DEVICE_MATRIX_INPUT_FILE)) \
		-o $@

LOCAL_PREBUILT_MODULE_FILE := $(GEN)
include $(BUILD_PREBUILT)
BUILT_VENDOR_MATRIX := $(LOCAL_BUILT_MODULE)

# System Manifest
include $(CLEAR_VARS)
LOCAL_MODULE        := system_manifest.xml
LOCAL_MODULE_STEM   := manifest.xml
LOCAL_MODULE_CLASS  := ETC
LOCAL_MODULE_PATH   := $(TARGET_OUT)/etc/vintf

GEN := $(local-generated-sources-dir)/manifest.xml

$(GEN): PRIVATE_VINTF_VNDK_VERSION := $(VINTF_VNDK_VERSION)
$(GEN): PRIVATE_FRAMEWORK_MANIFEST_INPUT_FILES := $(FRAMEWORK_MANIFEST_INPUT_FILES)
$(GEN): $(FRAMEWORK_MANIFEST_INPUT_FILES) $(HOST_OUT_EXECUTABLES)/assemble_vintf
	PROVIDED_VNDK_VERSIONS="$(PRIVATE_VINTF_VNDK_VERSION) $(PRODUCT_EXTRA_VNDK_VERSIONS)" \
	PLATFORM_SYSTEMSDK_VERSIONS="$(PLATFORM_SYSTEMSDK_VERSIONS)" \
		$(HOST_OUT_EXECUTABLES)/assemble_vintf \
		-i $(call normalize-path-list,$(PRIVATE_FRAMEWORK_MANIFEST_INPUT_FILES)) \
		-o $@

LOCAL_PREBUILT_MODULE_FILE := $(GEN)
include $(BUILD_PREBUILT)
BUILT_SYSTEM_MANIFEST := $(LOCAL_BUILT_MODULE)

# Product Manifest
ifneq ($(PRODUCT_MANIFEST_FILES),)
include $(CLEAR_VARS)
LOCAL_MODULE := product_manifest.xml
LOCAL_MODULE_STEM := manifest.xml
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_RELATIVE_PATH := vintf
GEN := $(local-generated-sources-dir)/manifest.xml
$(GEN): PRIVATE_PRODUCT_MANIFEST_FILES := $(PRODUCT_MANIFEST_FILES)
$(GEN): $(PRODUCT_MANIFEST_FILES) $(HOST_OUT_EXECUTABLES)/assemble_vintf
	$(HOST_OUT_EXECUTABLES)/assemble_vintf \
		-i $(call normalize-path-list,$(PRIVATE_PRODUCT_MANIFEST_FILES)) \
		-o $@

LOCAL_PREBUILT_MODULE_FILE := $(GEN)
include $(BUILD_PREBUILT)
BUILT_PRODUCT_MANIFEST := $(LOCAL_BUILT_MODULE)
endif

VINTF_VNDK_VERSION :=
FRAMEWORK_MANIFEST_INPUT_FILES :=
DEVICE_MATRIX_INPUT_FILE :=
PRODUCT_MANIFEST_INPUT_FILES :=
