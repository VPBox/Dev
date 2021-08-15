# Build the unit tests.
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk

LOCAL_MODULE := sf_foundation_test

LOCAL_MODULE_TAGS := tests

LOCAL_SRC_FILES := \
	AData_test.cpp \
	Base64_test.cpp \
	Flagged_test.cpp \
	TypeTraits_test.cpp \
	Utils_test.cpp \

LOCAL_SHARED_LIBRARIES := \
	liblog \
	libstagefright_foundation \
	libutils \

LOCAL_C_INCLUDES := \
	frameworks/av/include \

LOCAL_CFLAGS += -Werror -Wall
LOCAL_CLANG := true

include $(BUILD_NATIVE_TEST)

# Include subdirectory makefiles
# ============================================================

# If we're building with ONE_SHOT_MAKEFILE (mm, mmm), then what the framework
# team really wants is to build the stuff defined by this makefile.
ifeq (,$(ONE_SHOT_MAKEFILE))
include $(call first-makefiles-under,$(LOCAL_PATH))
endif
