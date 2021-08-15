#
# CHRE API Version Makefile
#
# Defines the default version of CHRE to target.

# Default CHRE API Version #####################################################

PRINT_CURRENT_CHRE_API_VERSION_SRCS = $(CHRE_PREFIX)/build/print_current_chre_api_version.c
PRINT_CURRENT_CHRE_API_VERSION_BIN = $(OUT)/print_current_chre_api_version
CURRENT_CHRE_API_VERSION_MK = $(OUT)/current_chre_api_version.mk

$(PRINT_CURRENT_CHRE_API_VERSION_BIN): $(PRINT_CURRENT_CHRE_API_VERSION_SRCS)
	mkdir -p $(OUT)
	$(CHRE_HOST_CC) -I$(CHRE_PREFIX)/chre_api/include/chre_api $^ -o $@

$(CURRENT_CHRE_API_VERSION_MK): $(PRINT_CURRENT_CHRE_API_VERSION_BIN)
	./$< > $@

# Only include default version if this is not a clean operation.
ifeq ($(filter clean, $(MAKECMDGOALS)),)
include $(CURRENT_CHRE_API_VERSION_MK)
endif

# Target CHRE API Version ######################################################

ifeq ($(CHRE_API_VERSION_MAJOR),)
TARGET_CHRE_API_VERSION_MAJOR = $(DEFAULT_CHRE_API_VERSION_MAJOR)
endif

ifeq ($(CHRE_API_VERSION_MINOR),)
TARGET_CHRE_API_VERSION_MINOR = $(DEFAULT_CHRE_API_VERSION_MINOR)
endif
