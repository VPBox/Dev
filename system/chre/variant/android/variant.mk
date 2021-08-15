#
# Android Simulator-Specific CHRE Makefile
#

# Version String ###############################################################

COMMIT_HASH_COMMAND = git describe --always --long --dirty

VERSION_STRING = chre=$(shell $(COMMIT_HASH_COMMAND))

COMMON_CFLAGS += -DCHRE_VERSION_STRING='"$(VERSION_STRING)"'

# Common Compiler Flags ########################################################

# Supply a symbol to indicate that the build variant supplies the static
# nanoapp list.
COMMON_CFLAGS += -DCHRE_VARIANT_SUPPLIES_STATIC_NANOAPP_LIST

# Optional Features ############################################################

CHRE_AUDIO_SUPPORT_ENABLED = true
CHRE_GNSS_SUPPORT_ENABLED = true
CHRE_WIFI_SUPPORT_ENABLED = true
CHRE_WWAN_SUPPORT_ENABLED = true

# Common Source Files ##########################################################

COMMON_SRCS += variant/android/static_nanoapps.cc
