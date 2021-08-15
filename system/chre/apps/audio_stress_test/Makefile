#
# Audio Stress Test Nanoapp Makefile
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

NANOAPP_NAME_STRING = \"Audio\ Stress\ Test\"
NANOAPP_NAME = audio_stress_test
NANOAPP_ID = 0x012345678900000e
NANOAPP_VERSION = 0x00000001

# Common Compiler Flags ########################################################

# Defines.
COMMON_CFLAGS += -DCHRE_NANOAPP_DISABLE_BACKCOMPAT
COMMON_CFLAGS += -DNANOAPP_MINIMUM_LOG_LEVEL=CHRE_LOG_LEVEL_DEBUG

# Common Source Files ##########################################################

COMMON_SRCS += audio_stress_test.cc
COMMON_SRCS += $(CHRE_PREFIX)/util/nanoapp/audio.cc

# Makefile Includes ############################################################

include $(CHRE_PREFIX)/build/nanoapp/app.mk
