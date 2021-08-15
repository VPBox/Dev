################################################################################################
#
# @NOTE:
# Audio Policy Engine configurable example for generic device build
#
# Any vendor shall have its own configuration within the corresponding device folder
#
################################################################################################

ifeq ($(BUILD_AUDIO_POLICY_EXAMPLE_CONFIGURATION), phone_configurable)

LOCAL_PATH := $(call my-dir)

PFW_CORE := external/parameter-framework
PFW_DEFAULT_SCHEMAS_DIR := $(PFW_CORE)/upstream/schemas
PFW_SCHEMAS_DIR := $(PFW_DEFAULT_SCHEMAS_DIR)

TOOLS := frameworks/av/services/audiopolicy/engineconfigurable/tools
BUILD_PFW_SETTINGS := $(TOOLS)/build_audio_pfw_settings.mk

##################################################################
# CONFIGURATION FILES
##################################################################
########## Policy PFW Structures #########
######### Policy PFW Settings #########
include $(CLEAR_VARS)
LOCAL_MODULE := parameter-framework.policy
LOCAL_MODULE_STEM := PolicyConfigurableDomains.xml
LOCAL_MODULE_CLASS := ETC
LOCAL_VENDOR_MODULE := true
LOCAL_MODULE_RELATIVE_PATH := parameter-framework/Settings/Policy
LOCAL_REQUIRED_MODULES := libpolicy-subsystem

PFW_EDD_FILES := \
        $(LOCAL_PATH)/../Settings/device_for_input_source.pfw \
        $(LOCAL_PATH)/../Settings/volumes.pfw \
        $(LOCAL_PATH)/Settings/device_for_product_strategy_media.pfw \
        $(LOCAL_PATH)/Settings/device_for_product_strategy_accessibility.pfw \
        $(LOCAL_PATH)/Settings/device_for_product_strategy_dtmf.pfw \
        $(LOCAL_PATH)/Settings/device_for_product_strategy_enforced_audible.pfw \
        $(LOCAL_PATH)/Settings/device_for_product_strategy_phone.pfw \
        $(LOCAL_PATH)/Settings/device_for_product_strategy_sonification.pfw \
        $(LOCAL_PATH)/Settings/device_for_product_strategy_sonification_respectful.pfw \
        $(LOCAL_PATH)/Settings/device_for_product_strategy_transmitted_through_speaker.pfw \
        $(LOCAL_PATH)/Settings/device_for_product_strategy_rerouting.pfw \
        $(LOCAL_PATH)/Settings/device_for_product_strategy_patch.pfw
PFW_CRITERION_TYPES_FILE := $(TARGET_OUT_VENDOR_ETC)/audio_policy_engine_criterion_types.xml
PFW_CRITERIA_FILE := $(TARGET_OUT_VENDOR_ETC)/audio_policy_engine_criteria.xml
PFW_TOPLEVEL_FILE := $(TARGET_OUT_VENDOR_ETC)/parameter-framework/ParameterFrameworkConfigurationPolicy.xml
PFW_SCHEMAS_DIR := $(PFW_DEFAULT_SCHEMAS_DIR)

include $(BUILD_PFW_SETTINGS)

endif #ifeq ($(BUILD_AUDIO_POLICY_EXAMPLE_CONFIGURATION), phone_configurable)
