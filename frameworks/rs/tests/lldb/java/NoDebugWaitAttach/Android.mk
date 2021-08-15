LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := tests

LOCAL_SRC_FILES := $(call all-java-files-under, src) $(call all-renderscript-files-under, src)

LOCAL_PACKAGE_NAME := JavaNoDebugWaitAttach
LOCAL_PRIVATE_PLATFORM_APIS := true

include $(BUILD_PACKAGE)

