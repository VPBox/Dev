LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := CppKernelVariables

LOCAL_SRC_FILES := \
	KernelVariables.cpp \
	simple.rs

include frameworks/rs/tests/lldb/cpp/common.mk
include $(BUILD_EXECUTABLE)
