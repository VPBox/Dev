# Copyright 2006-2014 The Android Open Source Project

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= kmsg.cpp
LOCAL_SHARED_LIBRARIES := liblog
LOCAL_MODULE := ckmsg
LOCAL_MODULE_OWNER := cells
LOCAL_MODULE_TAGS := optional
LOCAL_CFLAGS := -Werror

include $(BUILD_EXECUTABLE)

include $(call first-makefiles-under,$(LOCAL_PATH))
