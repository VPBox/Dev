LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := CppMultipleRSFiles

LOCAL_SRC_FILES := \
	MultipleRSFiles.cpp \
	first.rs \
	second.rs

include frameworks/rs/tests/lldb/cpp/common.mk
include $(BUILD_EXECUTABLE)
