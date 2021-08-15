
LOCAL_PATH:= $(call my-dir)
#########################################
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    librildp.c

LOCAL_SHARED_LIBRARIES := \
    liblog \
    libcutils \

LOCAL_MODULE_OWNER := cells
LOCAL_MODULE:= librildp
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)
##########################################
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    rildp.c

LOCAL_SHARED_LIBRARIES := \
    liblog \
    libcutils \
    librildp \

LOCAL_MODULE_OWNER := cells
LOCAL_MODULE:= rildpd
LOCAL_MODULE_TAGS := optional

include $(BUILD_EXECUTABLE)

###############################################
