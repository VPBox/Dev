LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := tests

LOCAL_SRC_FILES := $(call all-java-files-under, src) \
                   $(call all-renderscript-files-under, src)

LOCAL_PACKAGE_NAME := JNIAllocations
LOCAL_SDK_VERSION := current

LOCAL_JNI_SHARED_LIBRARIES := libjniallocations

LOCAL_RENDERSCRIPT_FLAGS := -g -O0 -target-api 0

include $(BUILD_PACKAGE)
include $(LOCAL_PATH)/jniallocations/Android.mk

