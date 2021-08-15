LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := libjninodebugwaitattach

LOCAL_SRC_FILES := jninodebugwaitattach.cpp simple.rs

include frameworks/rs/tests/lldb/jni/common.mk
include $(BUILD_SHARED_LIBRARY)
