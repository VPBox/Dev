LOCAL_PATH := $(call my-dir)

##################################################################
# CONFIGURATION TOP FILE
##################################################################

ifeq ($(BUILD_AUDIO_POLICY_EXAMPLE_CONFIGURATION), phone_default)

include $(CLEAR_VARS)
LOCAL_MODULE := audio_policy_engine_configuration.xml

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := ETC
LOCAL_VENDOR_MODULE := true
LOCAL_SRC_FILES := phone/$(LOCAL_MODULE)

LOCAL_REQUIRED_MODULES := \
    audio_policy_engine_product_strategies.xml \
    audio_policy_engine_stream_volumes.xml \
    audio_policy_engine_default_stream_volumes.xml

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

endif # ifeq ($(BUILD_AUDIO_POLICY_EXAMPLE_CONFIGURATION), phone_default)
