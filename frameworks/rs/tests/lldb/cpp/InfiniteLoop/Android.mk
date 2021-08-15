LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := CppInfiniteLoop

LOCAL_SRC_FILES := \
	InfiniteLoop.cpp \
	infiniteloop.rs

include frameworks/rs/tests/lldb/cpp/common.mk
include $(BUILD_EXECUTABLE)
