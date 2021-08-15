LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

MODULE_SRCS := \
	$(LOCAL_DIR)/gatekeeper_messages.cpp \
	$(LOCAL_DIR)/gatekeeper.cpp

GLOBAL_INCLUDES += $(LOCAL_DIR)/include/

MODULE_CPPFLAGS := -std=c++11

MODULE_INCLUDES := \
	$(LOCAL_DIR)/../../hardware/libhardware/include

MODULE_DEPS := \
	trusty/user/base/lib/libc-trusty \

include make/module.mk
