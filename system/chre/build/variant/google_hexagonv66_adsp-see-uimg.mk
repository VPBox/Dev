#
# Google CHRE Reference Implementation for Hexagon v66 Architecture on ADSP
#

include $(CHRE_PREFIX)/build/clean_build_template_args.mk

TARGET_NAME = google_hexagonv66_adsp-see-uimg
# Sized based on the buffer allocated in the host daemon (4096 bytes), minus
# FlatBuffer overhead (max 80 bytes), minus some extra space to make a nice
# round number and allow for addition of new fields to the FlatBuffer
TARGET_CFLAGS = -DCHRE_MESSAGE_TO_HOST_MAX_SIZE=4000
TARGET_CFLAGS += -DUIMG_DL_VER_MAJOR=2
TARGET_CFLAGS += -DCHRE_SLPI_UIMG_ENABLED
TARGET_CFLAGS += -DCHRE_THREAD_UTIL_ENABLED
TARGET_CFLAGS += $(GOOGLE_HEXAGONV66_ADSP-SEE-UIMG_CFLAGS)
TARGET_VARIANT_SRCS = $(GOOGLE_HEXAGONV66_ADSP-SEE-UIMG_SRCS)
TARGET_SO_LATE_LIBS = $(GOOGLE_HEXAGONV66_ADSP-SEE-UIMG_LATE_LIBS)
TARGET_PLATFORM_ID = 0x476f6f676c000007
HEXAGON_ARCH = v66

TARGET_CFLAGS += $(SLPI_CFLAGS)
TARGET_VARIANT_SRCS += $(SLPI_SRCS)

# Add SLPI/SEE-specific compiler flags and source files
TARGET_CFLAGS += $(SLPI_SEE_CFLAGS)
TARGET_CFLAGS += $(SLPI_SEE_QSK_CFLAGS)
TARGET_VARIANT_SRCS += $(SLPI_SEE_SRCS)
TARGET_VARIANT_SRCS += $(SLPI_SEE_QSK_SRCS)

# Enable uImage support.
TARGET_VARIANT_SRCS += $(SYS_SUPPORT_PATH)/qcom/uimg_dl_ver.c
TARGET_SO_LDFLAGS += --script=$(SYS_SUPPORT_PATH)/qcom/uimage_v2.lcs

ifneq ($(filter $(TARGET_NAME)% all, $(MAKECMDGOALS)),)
ifneq ($(IS_NANOAPP_BUILD),)
TARGET_SO_LATE_LIBS += $(CHRE_PREFIX)/build/app_support/google_slpi/libchre_slpi_skel.so
include $(CHRE_PREFIX)/build/nanoapp/google_slpi.mk
endif

include $(CHRE_PREFIX)/build/arch/hexagon.mk
include $(CHRE_PREFIX)/build/build_template.mk
endif
