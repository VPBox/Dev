################################################################################################
#
# @NOTE:
# Audio Policy Engine configurable example for generic device build
#
# Any vendor shall have its own configuration within the corresponding device folder
#
################################################################################################

LOCAL_PATH := $(call my-dir)

ifdef BUILD_AUDIO_POLICY_EXAMPLE_CONFIGURATION

ifeq ($(BUILD_AUDIO_POLICY_EXAMPLE_CONFIGURATION),$(filter $(BUILD_AUDIO_POLICY_EXAMPLE_CONFIGURATION),phone_configurable automotive_configurable caremu_configurable no-output_configurable no-input_configurable))

PFW_CORE := external/parameter-framework
#@TODO: upstream new domain generator
#BUILD_PFW_SETTINGS := $(PFW_CORE)/support/android/build_pfw_settings.mk
PFW_DEFAULT_SCHEMAS_DIR := $(PFW_CORE)/upstream/schemas
PFW_SCHEMAS_DIR := $(PFW_DEFAULT_SCHEMAS_DIR)

TOOLS := frameworks/av/services/audiopolicy/engineconfigurable/tools
BUILD_PFW_SETTINGS := $(TOOLS)/build_audio_pfw_settings.mk

PROVISION_STRATEGIES_STRUCTURE := $(TOOLS)/provision_strategies_structure.mk

endif

##################################################################
# CONFIGURATION FILES
##################################################################
######### Policy PFW top level file #########

ifeq ($(BUILD_AUDIO_POLICY_EXAMPLE_CONFIGURATION),$(filter $(BUILD_AUDIO_POLICY_EXAMPLE_CONFIGURATION),phone_configurable automotive_configurable caremu_configurable))

include $(CLEAR_VARS)
LOCAL_MODULE := ParameterFrameworkConfigurationPolicy.xml
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := ETC
LOCAL_VENDOR_MODULE := true
LOCAL_MODULE_RELATIVE_PATH := parameter-framework
LOCAL_SRC_FILES := $(LOCAL_MODULE).in
LOCAL_REQUIRED_MODULES := \
    PolicySubsystem.xml \
    PolicyClass.xml

# external/parameter-framework prevents from using debug interface
AUDIO_PATTERN = @TUNING_ALLOWED@
ifeq ($(TARGET_BUILD_VARIANT),user)
AUDIO_VALUE = false
else
AUDIO_VALUE = true
endif

LOCAL_POST_INSTALL_CMD := $(hide) sed -i -e 's|$(AUDIO_PATTERN)|$(AUDIO_VALUE)|g' $(TARGET_OUT_VENDOR_ETC)/$(LOCAL_MODULE_RELATIVE_PATH)/$(LOCAL_MODULE)

include $(BUILD_PREBUILT)

########## Policy PFW Common Structures #########

include $(CLEAR_VARS)
LOCAL_MODULE := PolicySubsystem.xml
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := ETC
LOCAL_VENDOR_MODULE := true
LOCAL_REQUIRED_MODULES := \
    PolicySubsystem-CommonTypes.xml \
    ProductStrategies.xml

LOCAL_MODULE_RELATIVE_PATH := parameter-framework/Structure/Policy
LOCAL_SRC_FILES := common/Structure/$(LOCAL_MODULE)
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := PolicySubsystem-CommonTypes.xml
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := ETC
LOCAL_VENDOR_MODULE := true
LOCAL_MODULE_RELATIVE_PATH := parameter-framework/Structure/Policy
LOCAL_SRC_FILES := common/Structure/$(LOCAL_MODULE)
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := PolicyClass.xml
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := ETC
LOCAL_VENDOR_MODULE := true
LOCAL_MODULE_RELATIVE_PATH := parameter-framework/Structure/Policy
LOCAL_SRC_FILES := common/Structure/$(LOCAL_MODULE)
include $(BUILD_PREBUILT)


include $(CLEAR_VARS)
LOCAL_MODULE := ProductStrategies.xml
LOCAL_MODULE_CLASS := ETC
LOCAL_VENDOR_MODULE := true
LOCAL_MODULE_RELATIVE_PATH := parameter-framework/Structure/Policy

AUDIO_POLICY_ENGINE_CONFIGURATION_FILE := \
    $(TARGET_OUT_VENDOR_ETC)/audio_policy_engine_configuration.xml
STRATEGIES_STRUCTURE_FILE := $(LOCAL_PATH)/common/Structure/$(LOCAL_MODULE).in

include $(PROVISION_STRATEGIES_STRUCTURE)

endif #ifeq ($(BUILD_AUDIO_POLICY_EXAMPLE_CONFIGURATION),$(filter $(BUILD_AUDIO_POLICY_EXAMPLE_CONFIGURATION),phone_configurable automotive_configurable caremu_configurable))

########## Policy PFW Example Structures #########
ifeq ($(BUILD_AUDIO_POLICY_EXAMPLE_CONFIGURATION),$(filter $(BUILD_AUDIO_POLICY_EXAMPLE_CONFIGURATION),no-output_configurable no-input_configurable))

include $(CLEAR_VARS)
LOCAL_MODULE := PolicySubsystem.xml
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := ETC
LOCAL_VENDOR_MODULE := true
LOCAL_REQUIRED_MODULES := PolicySubsystem-CommonTypes.xml

LOCAL_MODULE_RELATIVE_PATH := parameter-framework/Structure/Policy
LOCAL_SRC_FILES := common/Structure/$(LOCAL_MODULE)
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := ParameterFrameworkConfigurationPolicy-no-strategy.xml
LOCAL_MODULE_STEM := ParameterFrameworkConfigurationPolicy.xml
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := ETC
LOCAL_VENDOR_MODULE := true
LOCAL_MODULE_RELATIVE_PATH := parameter-framework
LOCAL_SRC_FILES := $(LOCAL_MODULE).in
LOCAL_REQUIRED_MODULES := \
    PolicySubsystem.xml \
    PolicyClass.xml
AUDIO_VALUE = false
LOCAL_POST_INSTALL_CMD := $(hide) sed -i -e 's|$(AUDIO_PATTERN)|$(AUDIO_VALUE)|g' $(TARGET_OUT_VENDOR_ETC)/$(LOCAL_MODULE_RELATIVE_PATH)/$(LOCAL_MODULE)

include $(BUILD_PREBUILT)

endif # ifeq ($(BUILD_AUDIO_POLICY_EXAMPLE_CONFIGURATION),$(filter $(BUILD_AUDIO_POLICY_EXAMPLE_CONFIGURATION),no-output_configurable no-input_configurable))

######### Policy PFW Settings - No Output #########
ifeq ($(BUILD_AUDIO_POLICY_EXAMPLE_CONFIGURATION),no-output_configurable)

include $(CLEAR_VARS)
LOCAL_MODULE := parameter-framework.policy
LOCAL_MODULE_STEM := PolicyConfigurableDomains-NoOutputDevice.xml
LOCAL_MODULE_CLASS := ETC
LOCAL_VENDOR_MODULE := true
LOCAL_MODULE_RELATIVE_PATH := parameter-framework/Settings/Policy

PFW_TOPLEVEL_FILE := $(TARGET_OUT_VENDOR_ETC)/parameter-framework/ParameterFrameworkConfigurationPolicy.xml
PFW_CRITERION_TYPES_FILE := $(TARGET_OUT_VENDOR_ETC)/audio_policy_engine_criterion_types.xml
PFW_CRITERIA_FILE := $(TARGET_OUT_VENDOR_ETC)/audio_policy_engine_criteria.xml
PFW_EDD_FILES := \
        $(LOCAL_PATH)/SettingsNoOutput/device_for_strategies.pfw \
        $(LOCAL_PATH)/Settings/device_for_input_source.pfw \
        $(LOCAL_PATH)/Settings/volumes.pfw        
LOCAL_REQUIRED_MODULES := libpolicy-subsystem
include $(BUILD_PFW_SETTINGS)

endif # ifeq ($(BUILD_AUDIO_POLICY_EXAMPLE_CONFIGURATION),no-output_configurable)
######### Policy PFW Settings - No Input #########
ifeq ($(BUILD_AUDIO_POLICY_EXAMPLE_CONFIGURATION),no-input_configurable)

include $(CLEAR_VARS)
LOCAL_MODULE := parameter-framework.policy
LOCAL_MODULE_STEM := PolicyConfigurableDomains-NoInputDevice.xml
LOCAL_MODULE_CLASS := ETC
LOCAL_VENDOR_MODULE := true
LOCAL_MODULE_RELATIVE_PATH := parameter-framework/Settings/Policy

PFW_TOPLEVEL_FILE := $(TARGET_OUT_VENDOR_ETC)/parameter-framework/ParameterFrameworkConfigurationPolicy.xml
PFW_CRITERION_TYPES_FILE := $(TARGET_OUT_VENDOR_ETC)/audio_policy_engine_criterion_types.xml
PFW_CRITERIA_FILE := $(TARGET_OUT_VENDOR_ETC)/audio_policy_engine_criteria.xml
PFW_EDD_FILES := \
        $(LOCAL_PATH)/SettingsNoInput/device_for_input_source.pfw \
        $(LOCAL_PATH)/Settings/volumes.pfw
LOCAL_REQUIRED_MODULES := libpolicy-subsystem
include $(BUILD_PFW_SETTINGS)

endif #ifeq ($(BUILD_AUDIO_POLICY_EXAMPLE_CONFIGURATION),no-input_configurable)
#######################################################################
# Recursive call sub-folder Android.mk
#######################################################################

include $(call all-makefiles-under,$(LOCAL_PATH))

endif #ifdef BUILD_AUDIO_POLICY_EXAMPLE_CONFIGURATION

