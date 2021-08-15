#
# Nanoapp Build Rules for Google CHRE on Android
#

################################################################################
#
# Google Generic CHRE on Android Nanoapp Build Template
#
# Invoke this to instantiate a set of Nanoapp post processing build targets.
#
# TARGET_NAME_nanoapp - The resulting nanoapp output.
#
# Argument List:
#     $1 - TARGET_NAME         - The name of the target being built.
#
################################################################################

ifndef GOOGLE_ANDROID_NANOAPP_BUILD_TEMPLATE
define GOOGLE_ANDROID_NANOAPP_BUILD_TEMPLATE

.PHONY: $(1)_nanoapp
all: $(1)_nanoapp

$(1)_nanoapp: $(1)

endef
endif

# Template Invocation ##########################################################

$(eval $(call GOOGLE_ANDROID_NANOAPP_BUILD_TEMPLATE, $(TARGET_NAME)))
