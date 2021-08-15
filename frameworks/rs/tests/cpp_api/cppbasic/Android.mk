LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE:= rstest-compute

LOCAL_SDK_VERSION := 21
LOCAL_NDK_STL_VARIANT := c++_static

LOCAL_SRC_FILES:= \
	mono.rs \
	compute.cpp

LOCAL_STATIC_LIBRARIES := \
	libRScpp_static

include frameworks/rs/tests/cpp_api/common.mk
include $(BUILD_EXECUTABLE)
