LOCAL_PATH := $(call my-dir)


#
# cells service
#
include $(CLEAR_VARS)

LOCAL_CFLAGS :=

LOCAL_SRC_FILES:= \
	CellsPrivateService.cpp \
	ICellsPrivateService.cpp \
	main_cells.cpp \
	cellnet.c

LOCAL_MODULE := cellsservice
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_OWNER := cells
LOCAL_SHARED_LIBRARIES := libm libcutils libc libbinder libutils libgui libpowermanager liblog
#LOCAL_MODULE_PATH := $(TARGET_ROOT_OUT_SBIN)
include $(BUILD_EXECUTABLE)


#sync
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	cellssync.cpp \
	ICellsPrivateService.cpp

LOCAL_MODULE:= cellssync
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_OWNER := cells
#LOCAL_MODULE_PATH := $(TARGET_ROOT_OUT_SBIN)
LOCAL_SHARED_LIBRARIES := libm libcutils libc libbinder libutils liblog
include $(BUILD_EXECUTABLE)

#cswitch
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	cswitch.cpp \
	ICellsPrivateService.cpp

LOCAL_MODULE:= cswitch
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_OWNER := cells
#LOCAL_MODULE_PATH := $(TARGET_ROOT_OUT_SBIN)
LOCAL_SHARED_LIBRARIES := libm libcutils libc libbinder libutils liblog
include $(BUILD_EXECUTABLE)



#sync
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	cellsupdownvm.cpp

LOCAL_MODULE:= cellsupdownvm
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_OWNER := cells
#LOCAL_MODULE_PATH := $(TARGET_ROOT_OUT_SBIN)
LOCAL_SHARED_LIBRARIES := libm libcutils libc libutils liblog
include $(BUILD_EXECUTABLE)


