#
# CHRE GoogleTest Build Variant
#

include $(CHRE_PREFIX)/build/clean_build_template_args.mk

TARGET_NAME = google_x86_googletest
TARGET_CFLAGS = -DCHRE_MESSAGE_TO_HOST_MAX_SIZE=2048
TARGET_VARIANT_SRCS = $(GOOGLE_X86_GOOGLETEST_SRCS)
TARGET_VARIANT_SRCS += $(GOOGLETEST_SRCS)
TARGET_PLATFORM_ID = 0x476f6f676c000001

TARGET_CFLAGS += $(SIM_CFLAGS)
TARGET_VARIANT_SRCS += $(SIM_SRCS)

# Add a symbol to determine when building for a test.
TARGET_CFLAGS += -DGTEST

# Ignore sign comparison warnings triggered by EXPECT/ASSERT macros in tests
# (typically, unsigned value vs. implicitly signed literal)
TARGET_CFLAGS += -Wno-sign-compare

# Ignore missing field initializers as googletest will not compile with this
# warning enabled.
TARGET_CFLAGS += -Wno-missing-field-initializers

TARGET_SO_LATE_LIBS = $(GOOGLE_X86_GOOGLETEST_LATE_LIBS)

ifneq ($(filter $(TARGET_NAME)% all, $(MAKECMDGOALS)),)

ifeq ($(ANDROID_BUILD_TOP),)
$(error "You should supply an ANDROID_BUILD_TOP environment variable \
         containing a path to the Android source tree. This is typically \
         provided by initializing the Android build environment.")
endif
export GOOGLETEST_PREFIX=$(ANDROID_BUILD_TOP)/external/googletest
include $(CHRE_PREFIX)/build/arch/x86.mk

TARGET_CFLAGS += $(GOOGLETEST_CFLAGS)
TARGET_CFLAGS += $(GOOGLE_X86_GOOGLETEST_CFLAGS)

# Instruct the build to link a final executable.
TARGET_BUILD_BIN = true

# Link in libraries for the final executable.
TARGET_BIN_LDFLAGS += -lrt -ldl
TARGET_BIN_LDFLAGS += -lpthread
TARGET_BIN_LDFLAGS += $(shell pkg-config --libs sndfile)

include $(CHRE_PREFIX)/build/build_template.mk
endif
