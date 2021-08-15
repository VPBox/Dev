#
# Hello World Nanoapp Makefile
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

NANOAPP_NAME = hello_world
NANOAPP_ID = 0x0123456789000001
NANOAPP_VERSION = 0x00000001

NANOAPP_NAME_STRING = \"Hello\ World\"

# Common Compiler Flags ########################################################

COMMON_CFLAGS += -I.

# Common Source Files ##########################################################

COMMON_SRCS += hello_world.cc

# Makefile Includes ############################################################

include $(CHRE_PREFIX)/build/nanoapp/app.mk
