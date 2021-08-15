LOCAL_PATH := $(call my-dir)

ifdef BUILD_AUDIO_POLICY_EXAMPLE_CONFIGURATION

TOOLS := frameworks/av/services/audiopolicy/engineconfigurable/tools
PROVISION_CRITERION_TYPES := $(TOOLS)/provision_criterion_types_from_android_headers.mk

##################################################################
# CONFIGURATION TOP FILE
##################################################################

ifeq ($(BUILD_AUDIO_POLICY_EXAMPLE_CONFIGURATION), phone_configurable)

include $(CLEAR_VARS)
LOCAL_MODULE := audio_policy_engine_configuration.xml

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := ETC
LOCAL_VENDOR_MODULE := true
LOCAL_SRC_FILES := phone/$(LOCAL_MODULE)

LOCAL_REQUIRED_MODULES := \
    audio_policy_engine_product_strategies.xml  \
    audio_policy_engine_stream_volumes.xml \
    audio_policy_engine_default_stream_volumes.xml \
    audio_policy_engine_criteria.xml \
    audio_policy_engine_criterion_types.xml

include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := audio_policy_engine_product_strategies.xml
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := ETC
LOCAL_VENDOR_MODULE := true
LOCAL_SRC_FILES := phone/$(LOCAL_MODULE)
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := audio_policy_engine_stream_volumes.xml
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := ETC
LOCAL_VENDOR_MODULE := true
LOCAL_SRC_FILES := phone/$(LOCAL_MODULE)
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := audio_policy_engine_default_stream_volumes.xml
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := ETC
LOCAL_VENDOR_MODULE := true
LOCAL_SRC_FILES := phone/$(LOCAL_MODULE)
include $(BUILD_PREBUILT)

endif # ifeq ($(BUILD_AUDIO_POLICY_EXAMPLE_CONFIGURATION), phone_configurable)


ifeq ($(BUILD_AUDIO_POLICY_EXAMPLE_CONFIGURATION),$(filter $(BUILD_AUDIO_POLICY_EXAMPLE_CONFIGURATION),automotive_configurable caremu_configurable))

##################################################################
# AUTOMOTIVE CONFIGURATION TOP FILE
##################################################################
include $(CLEAR_VARS)
LOCAL_MODULE := audio_policy_engine_configuration.xml
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := ETC
LOCAL_VENDOR_MODULE := true
LOCAL_SRC_FILES := automotive/$(LOCAL_MODULE)

LOCAL_REQUIRED_MODULES := \
    audio_policy_engine_product_strategies.xml \
    audio_policy_engine_criteria.xml \
    audio_policy_engine_criterion_types.xml \
    audio_policy_engine_volumes.xml

include $(BUILD_PREBUILT)

endif #ifeq ($(BUILD_AUDIO_POLICY_EXAMPLE_CONFIGURATION),$(filter $(BUILD_AUDIO_POLICY_EXAMPLE_CONFIGURATION),automotive_configurable caremu_configurable))

ifeq ($(BUILD_AUDIO_POLICY_EXAMPLE_CONFIGURATION), automotive_configurable)

##################################################################
# CONFIGURATION FILES
##################################################################

include $(CLEAR_VARS)
LOCAL_MODULE := audio_policy_engine_product_strategies.xml
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := ETC
LOCAL_VENDOR_MODULE := true
LOCAL_SRC_FILES := automotive/$(LOCAL_MODULE)
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := audio_policy_engine_volumes.xml
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := ETC
LOCAL_VENDOR_MODULE := true
LOCAL_SRC_FILES := automotive/$(LOCAL_MODULE)
include $(BUILD_PREBUILT)

endif #ifeq ($(BUILD_AUDIO_POLICY_EXAMPLE_CONFIGURATION), automotive_configurable)

ifeq ($(BUILD_AUDIO_POLICY_EXAMPLE_CONFIGURATION), caremu_configurable)

##################################################################
# CONFIGURATION FILES
##################################################################

include $(CLEAR_VARS)
LOCAL_MODULE := audio_policy_engine_product_strategies.xml
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := ETC
LOCAL_VENDOR_MODULE := true
LOCAL_SRC_FILES := caremu/$(LOCAL_MODULE)
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := audio_policy_engine_volumes.xml
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := ETC
LOCAL_VENDOR_MODULE := true
LOCAL_SRC_FILES := caremu/$(LOCAL_MODULE)
include $(BUILD_PREBUILT)

endif #ifeq ($(BUILD_AUDIO_POLICY_EXAMPLE_CONFIGURATION), caremu_configurable)

ifeq ($(BUILD_AUDIO_POLICY_EXAMPLE_CONFIGURATION),$(filter $(BUILD_AUDIO_POLICY_EXAMPLE_CONFIGURATION),phone_configurable automotive_configurable caremu_configurable))

include $(CLEAR_VARS)
LOCAL_MODULE := audio_policy_engine_criteria.xml
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := ETC
LOCAL_VENDOR_MODULE := true
LOCAL_SRC_FILES := common/$(LOCAL_MODULE)
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := audio_policy_engine_criterion_types.xml
LOCAL_MODULE_CLASS := ETC
LOCAL_VENDOR_MODULE := true
LOCAL_ADDITIONAL_DEPENDENCIES := $(TARGET_OUT_VENDOR_ETC)/primary_audio_policy_configuration.xml
ANDROID_AUDIO_BASE_HEADER_FILE := system/media/audio/include/system/audio-base.h
AUDIO_POLICY_CONFIGURATION_FILE := $(TARGET_OUT_VENDOR_ETC)/audio_policy_configuration.xml
CRITERION_TYPES_FILE := $(LOCAL_PATH)/common/$(LOCAL_MODULE).in

include $(PROVISION_CRITERION_TYPES)

endif #ifeq ($(BUILD_AUDIO_POLICY_EXAMPLE_CONFIGURATION),$(filter $(BUILD_AUDIO_POLICY_EXAMPLE_CONFIGURATION),phone_configurable automotive_configurable caremu_configurable))

endif #ifdef BUILD_AUDIO_POLICY_EXAMPLE_CONFIGURATION
