ifndef NANOAPP_NAME
$(error NANOAPP_NAME unset)
endif

ifndef NANOAPP_SRC_FILES
$(error NANOAPP_SRC_FILES unset)
endif

ifndef ANDROID_BUILD_TOP
$(error Must set Android build environment first)
endif

NANOAPP_DIR_NAME ?= $(NANOAPP_NAME)

# This path is actually relative to one level deeper as this file
# gets included from Makefile of each test subdirectory
NANOAPP_SRC_PATH = ../../src

SHARED_LIB_FILES = abort.cc \
  dumb_allocator.cc \
  nano_endian.cc \
  nano_string.cc \
  send_message.cc \
  test_success_marker.cc

COMMON_SRCS += \
  $(addprefix $(NANOAPP_SRC_PATH)/$(NANOAPP_DIR_NAME)/, $(NANOAPP_SRC_FILES)) \
  $(addprefix $(NANOAPP_SRC_PATH)/shared/, $(SHARED_LIB_FILES))

COMMON_CFLAGS += -DCHRE_NO_ENDIAN_H \
  -D__LITTLE_ENDIAN=1 \
  -D__BYTE_ORDER=1 \
  -D__BIG_ENDIAN=2

COMMON_CFLAGS += -I$(NANOAPP_SRC_PATH)

OPT_LEVEL=2

include ${ANDROID_BUILD_TOP}/system/chre/build/nanoapp/app.mk
