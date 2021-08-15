LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE:= rstest-compute-shared

LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk

LOCAL_SRC_FILES:= \
	mono.rs \
	compute.cpp

LOCAL_SHARED_LIBRARIES := \
	libRScpp

include frameworks/rs/tests/cpp_api/common.mk
include $(BUILD_EXECUTABLE)
