#
# Google CHRE Reference Implementation for Hexagon v55 Architecture on SLPI
#

include $(CHRE_PREFIX)/build/clean_build_template_args.mk

TARGET_NAME = google_hexagonv55_slpi-see
# Sized based on the buffer allocated in the host daemon (4096 bytes), minus
# FlatBuffer overhead (max 80 bytes), minus some extra space to make a nice
# round number and allow for addition of new fields to the FlatBuffer
TARGET_CFLAGS = -DCHRE_MESSAGE_TO_HOST_MAX_SIZE=4000
TARGET_CFLAGS += $(GOOGLE_HEXAGONV55_SLPI-SEE_CFLAGS)
TARGET_VARIANT_SRCS = $(GOOGLE_HEXAGONV55_SLPI-SEE_SRCS)
TARGET_SO_LATE_LIBS = $(GOOGLE_HEXAGONV55_SLPI-SEE_LATE_LIBS)
TARGET_PLATFORM_ID = 0x476f6f676c000004
HEXAGON_ARCH = v55

TARGET_CFLAGS += $(SLPI_CFLAGS)
TARGET_VARIANT_SRCS += $(SLPI_SRCS)

# Add SLPI/SEE-specific compiler flags and source files
TARGET_CFLAGS += $(SLPI_SEE_CFLAGS)
TARGET_CFLAGS += $(SLPI_SEE_QMI_CFLAGS)
TARGET_VARIANT_SRCS += $(SLPI_SEE_SRCS)
TARGET_VARIANT_SRCS += $(SLPI_SEE_QMI_SRCS)

ifneq ($(filter $(TARGET_NAME)% all, $(MAKECMDGOALS)),)
ifneq ($(IS_NANOAPP_BUILD),)
TARGET_SO_LATE_LIBS += $(CHRE_PREFIX)/build/app_support/google_slpi/libchre_slpi_skel.so
include $(CHRE_PREFIX)/build/nanoapp/google_slpi.mk
endif

include $(CHRE_PREFIX)/build/arch/hexagon.mk

# remove any CUST_H definitions by common code for this variant
TARGET_CFLAGS := $(filter-out -DCUST_H%, $(TARGET_CFLAGS))
# add our private definition, only if requested by variant config
ifdef TARGET_V55_CUST_H
TARGET_CFLAGS += -DCUST_H=\"$(TARGET_V55_CUST_H)\"
endif

include $(CHRE_PREFIX)/build/build_template.mk
endif
