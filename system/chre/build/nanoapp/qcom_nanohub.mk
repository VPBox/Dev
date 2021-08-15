#
# Qualcomm Nanoapp Build Rules for CHRE based on Nanohub
#

################################################################################
#
# Qualcomm CHRE Nanoapp Build Template
#
# Invoke this to instantiate a set of Nanoapp post processing build targets.
#
# TARGET_NAME_nanoapp - The resulting nanoapp output.
#
# Argument List:
#     $1 - TARGET_NAME         - The name of the target being built.
#
################################################################################

TARGET_CFLAGS += -DNANOAPP_ID=$(NANOAPP_ID)
TARGET_CFLAGS += -DNANOAPP_VERSION=$(NANOAPP_VERSION)

ifndef QCOM_CHRE_NANOHUB_NANOAPP_BUILD_TEMPLATE
define QCOM_CHRE_NANOHUB_NANOAPP_BUILD_TEMPLATE

# TODO: Invoke nanoapp post-processing tools. This simply adds the underlying
# shared object and archive to the nanoapp target.

.PHONY: $(1)_nanoapp
all: $(1)_nanoapp

$(1)_nanoapp: $(1)

endef
endif

# Template Invocation ##########################################################

$(eval $(call QCOM_CHRE_NANOHUB_NANOAPP_BUILD_TEMPLATE, $(TARGET_NAME)))
