#
# Qualcomm CHRE Implementation for Hexagon v60, based on Nanohub
#

include $(CHRE_PREFIX)/build/clean_build_template_args.mk

TARGET_NAME = qcom_hexagonv60_nanohub-uimg
TARGET_CFLAGS = -DCHRE_MESSAGE_TO_HOST_MAX_SIZE=4080
TARGET_CFLAGS += -mno-pic-data-is-text-relative
TARGET_CFLAGS += $(QCOM_HEXAGONV60_NANOHUB-UIMG_CFLAGS)
TARGET_VARIANT_SRCS = $(QCOM_HEXAGONV60_NANOHUB-UIMG_SRCS)
TARGET_SO_LATE_LIBS = $(QCOM_HEXAGONV60_NANOHUB-UIMG_LATE_LIBS)
TARGET_PLATFORM_ID = 0x2051434F4D000001
HEXAGON_ARCH = v60

# Enable uImage support.
TARGET_VARIANT_SRCS += $(SYS_SUPPORT_PATH)/qcom/uimg_dl_ver.c
TARGET_SO_LDFLAGS += --script=$(SYS_SUPPORT_PATH)/qcom/uimage.lcs.toolv80

ifneq ($(filter $(TARGET_NAME)% all, $(MAKECMDGOALS)),)
ifneq ($(IS_NANOAPP_BUILD),)
TARGET_SO_LATE_LIBS += $(CHRE_PREFIX)/build/app_support/qcom_nanohub/chre.so
TARGET_SO_LATE_LIBS += $(CHRE_PREFIX)/build/app_support/qcom_nanohub/chre_platform.so
include $(CHRE_PREFIX)/build/nanoapp/qcom_nanohub.mk
endif

include $(CHRE_PREFIX)/build/arch/hexagon.mk
include $(CHRE_PREFIX)/build/build_template.mk
endif
