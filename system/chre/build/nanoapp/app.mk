#
# Nanoapp Makefile
#
# Include this file in your nanoapp Makefile to produce binary nanoapps to
# target a variety of architectures.
#

# Nanoapp Build Configuration Checks ###########################################

ifeq ($(NANOAPP_NAME),)
$(error "The NANOAPP_NAME variable must be set to the name of the nanoapp. \
         This should be assigned by the Makefile that includes app.mk.")
endif

ifeq ($(NANOAPP_ID),)
$(error "The NANOAPP_ID variable must be set to the ID of the nanoapp. \
         This should be assigned by the Makefile that includes app.mk.")
endif

ifeq ($(NANOAPP_VERSION),)
$(error "The NANOAPP_VERSION variable must be set to the version of the nanoapp. \
         This should be assigned by the Makefile that includes app.mk.")
endif

NANOAPP_VERSION := $(strip $(NANOAPP_VERSION))
MATCHED_NANOAPP_VERSION := $(shell echo $(NANOAPP_VERSION) \
                                 | grep "^0x[0-9a-fA-F]\{8\}")
ifneq ($(MATCHED_NANOAPP_VERSION), $(NANOAPP_VERSION))
$(error "The NANOAPP_VERSION must be a 4-byte hex-formatted integer. Example: \
         0x00000101")
endif

ifeq ($(NANOAPP_NAME_STRING),)
$(error "The NANOAPP_NAME_STRING variable must be set to the friendly name of \
         the nanoapp. This should be assigned by the Makefile that includes \
         app.mk.")
endif

ifeq ($(NANOAPP_VENDOR_STRING),)
$(info "NANOAPP_VENDOR_STRING not supplied, defaulting to \"Google\".")
NANOAPP_VENDOR_STRING = \"Google\"
endif

ifeq ($(NANOAPP_IS_SYSTEM_NANOAPP),)
$(info "NANOAPP_IS_SYSTEM_NANOAPP not supplied, defaulting to 0.")
NANOAPP_IS_SYSTEM_NANOAPP = 0
endif

ifeq ($(CHRE_PREFIX),)
ifeq ($(ANDROID_BUILD_TOP),)
$(error "You must run lunch, or specify an explicit CHRE_PREFIX environment \
         variable")
else
CHRE_PREFIX = $(ANDROID_BUILD_TOP)/system/chre
endif
endif

# Nanoapp Build ################################################################

# This variable indicates to the variants that some post-processing may be
# required as the target is a nanoapp.
IS_NANOAPP_BUILD = true

# Common App Build Configuration ###############################################

OUTPUT_NAME = $(NANOAPP_NAME)

# Common Compiler Flags ########################################################

# Add the CHRE API to the include search path.
COMMON_CFLAGS += -I$(CHRE_PREFIX)/chre_api/include/chre_api

# Add util and platform/shared to the include search path.
COMMON_CFLAGS += -I$(CHRE_PREFIX)/util/include
COMMON_CFLAGS += -I$(CHRE_PREFIX)/platform/shared/include

# Allows a nanoapp to know that is compiled separately from the CHRE system.
COMMON_CFLAGS += -DCHRE_IS_NANOAPP_BUILD

# Compile FlatBuffers in a portable way.
COMMON_CFLAGS += -DFLATBUFFERS_CHRE

# Nanoapp configuration flags.
COMMON_CFLAGS += -DNANOAPP_ID=$(NANOAPP_ID)
COMMON_CFLAGS += -DNANOAPP_VERSION=$(NANOAPP_VERSION)
COMMON_CFLAGS += -DNANOAPP_VENDOR_STRING=$(NANOAPP_VENDOR_STRING)
COMMON_CFLAGS += -DNANOAPP_NAME_STRING=$(NANOAPP_NAME_STRING)
COMMON_CFLAGS += -DNANOAPP_IS_SYSTEM_NANOAPP=$(NANOAPP_IS_SYSTEM_NANOAPP)

# Version String ###############################################################

COMMIT_HASH_DIRTY_SUFFIX = $(shell git diff --quiet || echo -dirty)
COMMIT_HASH = $(shell git log -1 --pretty="format:%h" .)$(COMMIT_HASH_DIRTY_SUFFIX)
NANOAPP_VERSION_STRING = "nanoapp=$(NANOAPP_NAME)@$(COMMIT_HASH)"
COMMON_CFLAGS += -DNANOAPP_VERSION_STRING="\"$(NANOAPP_VERSION_STRING)\""

# Variant-specific Nanoapp Support Source Files ################################

APP_SUPPORT_PATH = $(CHRE_PREFIX)/build/app_support
DSO_SUPPORT_LIB_PATH = $(CHRE_PREFIX)/platform/shared/nanoapp
DSO_SUPPORT_LIB_SRCS = $(DSO_SUPPORT_LIB_PATH)/nanoapp_support_lib_dso.cc

GOOGLE_HEXAGONV60_SLPI_SRCS += $(DSO_SUPPORT_LIB_SRCS)
GOOGLE_HEXAGONV62_SLPI_SRCS += $(DSO_SUPPORT_LIB_SRCS)
GOOGLE_HEXAGONV62_SLPI-UIMG_SRCS += $(DSO_SUPPORT_LIB_SRCS)
GOOGLE_HEXAGONV65_ADSP-SEE_SRCS += $(DSO_SUPPORT_LIB_SRCS)
GOOGLE_HEXAGONV65_ADSP-SEE-UIMG_SRCS += $(DSO_SUPPORT_LIB_SRCS)
GOOGLE_HEXAGONV65_SLPI-SEE_SRCS += $(DSO_SUPPORT_LIB_SRCS)
GOOGLE_HEXAGONV65_SLPI-SEE-UIMG_SRCS += $(DSO_SUPPORT_LIB_SRCS)
GOOGLE_HEXAGONV66_ADSP-SEE_SRCS += $(DSO_SUPPORT_LIB_SRCS)
GOOGLE_HEXAGONV66_ADSP-SEE-UIMG_SRCS += $(DSO_SUPPORT_LIB_SRCS)
GOOGLE_HEXAGONV66_SLPI-SEE_SRCS += $(DSO_SUPPORT_LIB_SRCS)
GOOGLE_HEXAGONV66_SLPI-SEE-UIMG_SRCS += $(DSO_SUPPORT_LIB_SRCS)
GOOGLE_ARM64_ANDROID_SRCS += $(DSO_SUPPORT_LIB_SRCS)
GOOGLE_X86_LINUX_SRCS += $(DSO_SUPPORT_LIB_SRCS)
QCOM_HEXAGONV60_NANOHUB_SRCS += $(APP_SUPPORT_PATH)/qcom_nanohub/app_support.cc
QCOM_HEXAGONV60_NANOHUB-UIMG_SRCS += $(APP_SUPPORT_PATH)/qcom_nanohub/app_support_uimg.cc

# Makefile Includes ############################################################

# Common includes
include $(CHRE_PREFIX)/build/defs.mk
include $(CHRE_PREFIX)/build/common.mk

# CHRE API version.
include $(CHRE_PREFIX)/chre_api/chre_api_version.mk

# Supported variants includes
include $(CHRE_PREFIX)/build/variant/google_arm64_android.mk
include $(CHRE_PREFIX)/build/variant/google_cm4_nanohub.mk
include $(CHRE_PREFIX)/build/variant/google_hexagonv55_slpi-see.mk
include $(CHRE_PREFIX)/build/variant/google_hexagonv60_slpi.mk
include $(CHRE_PREFIX)/build/variant/google_hexagonv62_slpi.mk
include $(CHRE_PREFIX)/build/variant/google_hexagonv62_slpi-uimg.mk
include $(CHRE_PREFIX)/build/variant/google_hexagonv65_adsp-see.mk
include $(CHRE_PREFIX)/build/variant/google_hexagonv65_adsp-see-uimg.mk
include $(CHRE_PREFIX)/build/variant/google_hexagonv65_slpi-see.mk
include $(CHRE_PREFIX)/build/variant/google_hexagonv65_slpi-see-uimg.mk
include $(CHRE_PREFIX)/build/variant/google_hexagonv66_adsp-see.mk
include $(CHRE_PREFIX)/build/variant/google_hexagonv66_adsp-see-uimg.mk
include $(CHRE_PREFIX)/build/variant/google_hexagonv66_slpi-see.mk
include $(CHRE_PREFIX)/build/variant/google_hexagonv66_slpi-see-uimg.mk
include $(CHRE_PREFIX)/build/variant/google_x86_linux.mk
include $(CHRE_PREFIX)/build/variant/qcom_hexagonv60_nanohub.mk
include $(CHRE_PREFIX)/build/variant/qcom_hexagonv60_nanohub-uimg.mk
