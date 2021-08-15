LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := hidl_test_java
LOCAL_MODULE_CLASS := NATIVE_TESTS
LOCAL_SRC_FILES := hidl_test_java

LOCAL_MODULE_PATH := $(TARGET_OUT_DATA)/nativetest64

LOCAL_REQUIRED_MODULES :=                       \
    hidl_test_java_java                         \
    hidl_test_java_native

ifneq ($(TARGET_2ND_ARCH),)
LOCAL_REQUIRED_MODULES += hidl_test_java_native$(TARGET_2ND_ARCH_MODULE_SUFFIX)
endif

include $(BUILD_PREBUILT)
