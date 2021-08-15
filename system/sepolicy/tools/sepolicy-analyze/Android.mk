LOCAL_PATH:= $(call my-dir)

###################################
include $(CLEAR_VARS)

LOCAL_MODULE := sepolicy-analyze
LOCAL_MODULE_TAGS := optional
LOCAL_CFLAGS := -Wall -Werror
LOCAL_SRC_FILES := sepolicy-analyze.c dups.c neverallow.c perm.c typecmp.c booleans.c attribute.c utils.c
LOCAL_STATIC_LIBRARIES := libsepol
LOCAL_CXX_STL := none

LOCAL_COMPATIBILITY_SUITE := ats cts gts vts sts

include $(BUILD_HOST_EXECUTABLE)
