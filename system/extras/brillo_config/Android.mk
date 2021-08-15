#
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
#

# Build time settings used by system services
# ========================================================
ifdef OSRELEASED_DIRECTORY

include $(CLEAR_VARS)
LOCAL_MODULE := product_id
LOCAL_MODULE_CLASS := FAKE
LOCAL_MODULE_PATH := $(TARGET_OUT_OEM)/$(OSRELEASED_DIRECTORY)
include $(BUILD_SYSTEM)/base_rules.mk

$(LOCAL_BUILT_MODULE):
	$(hide) mkdir -p $(dir $@)
	echo $(BRILLO_PRODUCT_ID) > $@
	$(hide) mkdir -p $(TARGET_OUT_ETC)/$(OSRELEASED_DIRECTORY)
	ln -sf /oem/$(OSRELEASED_DIRECTORY)/product_id $(TARGET_OUT_ETC)/$(OSRELEASED_DIRECTORY)

include $(CLEAR_VARS)
LOCAL_MODULE := system_id
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT_ETC)/$(OSRELEASED_DIRECTORY)
include $(BUILD_SYSTEM)/base_rules.mk

$(LOCAL_BUILT_MODULE):
	$(hide) mkdir -p $(dir $@)
	echo $(BRILLO_SYSTEM_ID) > $@


include $(CLEAR_VARS)
LOCAL_MODULE := product_version
LOCAL_MODULE_CLASS := FAKE
LOCAL_MODULE_PATH := $(TARGET_OUT_OEM)/$(OSRELEASED_DIRECTORY)
include $(BUILD_SYSTEM)/base_rules.mk

# The version is set to 0.0.0.0 if the user did not set the actual version.
# This allows us to have a valid version number while being easy to filter.
BRILLO_PRODUCT_VERSION ?= "0.0.0.0"
ifeq ($(shell echo $(BRILLO_PRODUCT_VERSION) | grep -E '^[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+$$'),)
$(error Invalid BRILLO_PRODUCT_VERSION "$(BRILLO_PRODUCT_VERSION)", must be \
  four numbers separated by dots. Example: "1.2.0.0")
endif

$(LOCAL_BUILT_MODULE):
	$(hide) mkdir -p $(dir $@)
	echo $(BRILLO_PRODUCT_VERSION) > $@
	$(hide) mkdir -p $(TARGET_OUT_ETC)/$(OSRELEASED_DIRECTORY)
	ln -sf /oem/$(OSRELEASED_DIRECTORY)/product_version $(TARGET_OUT_ETC)/$(OSRELEASED_DIRECTORY)


include $(CLEAR_VARS)
LOCAL_MODULE := system_version
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT_ETC)/$(OSRELEASED_DIRECTORY)
include $(BUILD_SYSTEM)/base_rules.mk

# The version is set to 0.0.0 if the user did not set the actual version.
# This allows us to have a valid version number while being easy to filter.
BRILLO_SYSTEM_VERSION ?= "0.0.0"
ifeq ($(shell echo $(BRILLO_SYSTEM_VERSION) | grep -E '^[0-9]+\.[0-9]+\.[0-9]+$$'),)
$(error Invalid BRILLO_SYSTEM_VERSION "$(BRILLO_SYSTEM_VERSION)", must be \
  three numbers separated by dots. Example: "1.2.0")
endif

# Append BUILD_NUMBER if it is a number or a build timestamp otherwise.
# If you don' want this to change at every build, you can define BUILD_NUMBER in
# your product makefile and increase it manually.
$(LOCAL_BUILT_MODULE):
	$(hide) mkdir -p $(dir $@)
ifeq ($(strip $(HAS_BUILD_NUMBER)),true)
	echo $(BRILLO_SYSTEM_VERSION).$(shell cat $(BUILD_NUMBER_FILE)) > $@
else
	echo $(BRILLO_SYSTEM_VERSION).$(BUILD_DATETIME_FROM_FILE) > $@
endif

endif
