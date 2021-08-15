#
# Google CHRE Reference Implementation for Hexagon v62 Architecture on SLPI
#

include $(CHRE_PREFIX)/build/clean_build_template_args.mk

TARGET_NAME = google_hexagonv62_slpi-uimg
# Sized based on the buffer allocated in the host daemon (4096 bytes), minus
# FlatBuffer overhead (max 80 bytes), minus some extra space to make a nice
# round number and allow for addition of new fields to the FlatBuffer
TARGET_CFLAGS = -DCHRE_MESSAGE_TO_HOST_MAX_SIZE=4000
TARGET_CFLAGS += -mno-pic-data-is-text-relative
TARGET_CFLAGS += -DCHRE_SLPI_UIMG_ENABLED
TARGET_CFLAGS += $(GOOGLE_HEXAGONV62_SLPI-UIMG_CFLAGS)
TARGET_VARIANT_SRCS = $(GOOGLE_HEXAGONV62_SLPI-UIMG_SRCS)
TARGET_SO_LATE_LIBS = $(GOOGLE_HEXAGONV62_SLPI-UIMG_LATE_LIBS)
TARGET_PLATFORM_ID = 0x476f6f676c000002
HEXAGON_ARCH = v62

TARGET_CFLAGS += $(SLPI_CFLAGS)
TARGET_VARIANT_SRCS += $(SLPI_SRCS)

# Add SLPI/SMGR-specific compiler flags and source files
TARGET_CFLAGS += $(SLPI_SMGR_CFLAGS)
TARGET_VARIANT_SRCS += $(SLPI_SMGR_SRCS)

# Enable uImage support.
TARGET_VARIANT_SRCS += $(SYS_SUPPORT_PATH)/qcom/uimg_dl_ver.c
TARGET_SO_LDFLAGS += --script=$(SYS_SUPPORT_PATH)/qcom/uimage.lcs.toolv80

ifneq ($(filter $(TARGET_NAME)% all, $(MAKECMDGOALS)),)
ifneq ($(IS_NANOAPP_BUILD),)
TARGET_SO_LATE_LIBS += $(CHRE_PREFIX)/build/app_support/google_slpi/libchre_slpi_skel.so
include $(CHRE_PREFIX)/build/nanoapp/google_slpi.mk
endif

include $(CHRE_PREFIX)/build/arch/hexagon.mk
include $(CHRE_PREFIX)/build/build_template.mk
endif
