LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := CppAllocations

LOCAL_SRC_FILES := \
	Allocations.cpp \
	allocs.rs

LOCAL_STATIC_LIBRARIES := libcompiler_rt

include frameworks/rs/tests/lldb/cpp/common.mk
include $(BUILD_EXECUTABLE)
