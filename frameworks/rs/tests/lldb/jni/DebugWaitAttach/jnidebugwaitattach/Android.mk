LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := libjnidebugwaitattach

LOCAL_SRC_FILES := jnidebugwaitattach.cpp simple.rs

LOCAL_RENDERSCRIPT_FLAGS := -g

include frameworks/rs/tests/lldb/jni/common.mk
include $(BUILD_SHARED_LIBRARY)
