#
# Google CHRE Simulator for Android on arm64
#

include $(CHRE_PREFIX)/build/clean_build_template_args.mk

TARGET_NAME = google_arm64_android
TARGET_CFLAGS = -DCHRE_MESSAGE_TO_HOST_MAX_SIZE=2048
TARGET_VARIANT_SRCS = $(GOOGLE_ARM64_ANDROID_SRCS)
TARGET_BIN_LDFLAGS = $(GOOGLE_ARM64_ANDROID_BIN_LDFLAGS)
TARGET_SO_EARLY_LIBS = $(GOOGLE_ARM64_ANDROID_EARLY_LIBS)
TARGET_SO_LATE_LIBS = $(GOOGLE_ARM64_ANDROID_LATE_LIBS)
TARGET_CFLAGS += $(GOOGLE_ARM64_ANDROID_CFLAGS)
TARGET_PLATFORM_ID = 0x476f6f676c000001

TARGET_CFLAGS += $(SIM_CFLAGS)
TARGET_VARIANT_SRCS += $(SIM_SRCS)

ifneq ($(filter $(TARGET_NAME)% all, $(MAKECMDGOALS)),)
ifneq ($(IS_NANOAPP_BUILD),)
include $(CHRE_PREFIX)/build/nanoapp/google_android.mk
else
# Instruct the build to link a final executable.
TARGET_BUILD_BIN = true

# Link in libraries for the final executable and export symbols to dynamically
# loaded objects.
TARGET_BIN_LDFLAGS += -pie -laaudio -llog -ldl -Wl,--export-dynamic
endif

include $(CHRE_PREFIX)/build/arch/arm64.mk
include $(CHRE_PREFIX)/build/build_template.mk
endif
