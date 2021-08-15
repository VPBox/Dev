#
# Wifi World Nanoapp Makefile
#

# Environment Checks ###########################################################

ifeq ($(CHRE_PREFIX),)
$(error "The CHRE_PREFIX environment variable must be set to a path to the \
         CHRE project root. Example: export CHRE_PREFIX=$$HOME/chre")
endif

# Nanoapp Configuration ########################################################

NANOAPP_NAME = wifi_world
NANOAPP_ID = 0x0123456789000006
NANOAPP_VERSION = 0x00000000

NANOAPP_NAME_STRING = \"Wi-Fi\ World\"

# Common Compiler Flags ########################################################

COMMON_CFLAGS += -I.

COMMON_CFLAGS += -DNANOAPP_MINIMUM_LOG_LEVEL=CHRE_LOG_LEVEL_DEBUG

# Common Source Files ##########################################################

COMMON_SRCS += wifi_world.cc
COMMON_SRCS += $(CHRE_PREFIX)/util/nanoapp/wifi.cc

# Makefile Includes ############################################################

include $(CHRE_PREFIX)/build/nanoapp/app.mk
