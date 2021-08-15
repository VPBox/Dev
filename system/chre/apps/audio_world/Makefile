#
# Audio World Nanoapp Makefile
#

# Environment Checks ###########################################################

ifeq ($(CHRE_PREFIX),)
ifneq ($(ANDROID_BUILD_TOP),)
CHRE_PREFIX = $(ANDROID_BUILD_TOP)/system/chre
else
$(error "You must run 'lunch' to setup ANDROID_BUILD_TOP, or explicitly define \
         the CHRE_PREFIX environment variable to point to the CHRE root \
         directory.")
endif
endif

# Nanoapp Configuration ########################################################

NANOAPP_NAME = audio_world
NANOAPP_ID = 0x012345678900000c
NANOAPP_VERSION = 0x00000001

NANOAPP_NAME_STRING = \"Audio\ World\"

# Common Compiler Flags ########################################################

# Include paths.
COMMON_CFLAGS += -I.
COMMON_CFLAGS += -I$(CHRE_PREFIX)/external/kiss_fft
COMMON_CFLAGS += -I$(CHRE_PREFIX)/util/include

# Defines.
COMMON_CFLAGS += -DNANOAPP_MINIMUM_LOG_LEVEL=CHRE_LOG_LEVEL_DEBUG
COMMON_CFLAGS += -DFIXED_POINT

# Common Source Files ##########################################################

COMMON_SRCS += audio_world.cc
COMMON_SRCS += $(CHRE_PREFIX)/external/kiss_fft/kiss_fft.c
COMMON_SRCS += $(CHRE_PREFIX)/external/kiss_fft/kiss_fftr.c
COMMON_SRCS += $(CHRE_PREFIX)/util/nanoapp/audio.cc

# Makefile Includes ############################################################

include $(CHRE_PREFIX)/build/nanoapp/app.mk
