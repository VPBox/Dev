LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := libjniallocations

LOCAL_SRC_FILES := jniallocations.cpp allocs.rs

LOCAL_RENDERSCRIPT_FLAGS := -g

LOCAL_STATIC_LIBRARIES := libcompiler_rt

include frameworks/rs/tests/lldb/jni/common.mk
include $(BUILD_SHARED_LIBRARY)
