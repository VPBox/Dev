LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := CppDebugWaitAttach

LOCAL_SRC_FILES := \
	WaitAttach.cpp \
	simple.rs

include frameworks/rs/tests/lldb/cpp/common.mk
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)

LOCAL_MODULE := CppNoDebugWaitAttach

LOCAL_SRC_FILES := \
	WaitAttach.cpp \
	simple.rs

include frameworks/rs/tests/lldb/cpp/common.mk
LOCAL_RENDERSCRIPT_FLAGS := $(filter-out -g,$(LOCAL_RENDERSCRIPT_FLAGS))
include $(BUILD_EXECUTABLE)
