#
# Google CHRE Reference Implementation for Hexagon v60 Architecture on SLPI
#

include $(CHRE_PREFIX)/build/clean_build_template_args.mk

TARGET_NAME = google_hexagonv60_slpi
TARGET_CFLAGS = -DCHRE_MESSAGE_TO_HOST_MAX_SIZE=4000
TARGET_CFLAGS += $(GOOGLE_HEXAGONV60_SLPI_CFLAGS)
TARGET_VARIANT_SRCS = $(GOOGLE_HEXAGONV60_SLPI_SRCS)
TARGET_SO_LATE_LIBS = $(GOOGLE_HEXAGONV60_SLPI_LATE_LIBS)
TARGET_PLATFORM_ID = 0x476f6f676c000002
HEXAGON_ARCH = v60

TARGET_CFLAGS += $(SLPI_CFLAGS)
TARGET_VARIANT_SRCS += $(SLPI_SRCS)

# Add SLPI/SMGR-specific compiler flags and source files
TARGET_CFLAGS += $(SLPI_SMGR_CFLAGS)
TARGET_VARIANT_SRCS += $(SLPI_SMGR_SRCS)

ifneq ($(filter $(TARGET_NAME)% all, $(MAKECMDGOALS)),)
ifneq ($(IS_NANOAPP_BUILD),)
TARGET_SO_LATE_LIBS += $(CHRE_PREFIX)/build/app_support/google_slpi/libchre_slpi_skel.so
include $(CHRE_PREFIX)/build/nanoapp/google_slpi.mk
endif

include $(CHRE_PREFIX)/build/arch/hexagon.mk
include $(CHRE_PREFIX)/build/build_template.mk
endif
