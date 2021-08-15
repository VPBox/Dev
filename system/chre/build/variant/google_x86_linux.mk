#
# Google Reference CHRE Implementation for Linux on x86
#

include $(CHRE_PREFIX)/build/clean_build_template_args.mk

TARGET_NAME = google_x86_linux
TARGET_CFLAGS = -DCHRE_MESSAGE_TO_HOST_MAX_SIZE=2048
TARGET_VARIANT_SRCS = $(GOOGLE_X86_LINUX_SRCS)
TARGET_BIN_LDFLAGS = $(GOOGLE_X86_LINUX_BIN_LDFLAGS)
TARGET_SO_EARLY_LIBS = $(GOOGLE_X86_LINUX_EARLY_LIBS)
TARGET_SO_LATE_LIBS = $(GOOGLE_X86_LINUX_LATE_LIBS)
TARGET_PLATFORM_ID = 0x476f6f676c000001

TARGET_CFLAGS += $(SIM_CFLAGS)
TARGET_VARIANT_SRCS += $(SIM_SRCS)

# Enable conversion warnings for the simulator. Since this is a platform 100%
# within our control we expect that there will be no conversion issues. It would
# be nice to enable this globally in the tools_config.mk but some vendor header
# files do not compile cleanly with it.
TARGET_CFLAGS += -Wconversion

# Add the target CFLAGS after the -Wconversion warning to allow targets to
# disable it.
TARGET_CFLAGS += $(GOOGLE_X86_LINUX_CFLAGS)

ifneq ($(filter $(TARGET_NAME)% all, $(MAKECMDGOALS)),)
ifneq ($(IS_NANOAPP_BUILD),)
include $(CHRE_PREFIX)/build/nanoapp/google_linux.mk
else
# Instruct the build to link a final executable.
TARGET_BUILD_BIN = true

# Add CFLAGS for the simulator.
TARGET_CFLAGS += $(shell pkg-config --cflags tclap)
TARGET_CFLAGS += $(shell pkg-config --cflags sndfile)

# Link in libraries for the final executable and export symbols to dynamically
# loaded objects.
TARGET_BIN_LDFLAGS += -lpthread -lrt -ldl -Wl,--export-dynamic
TARGET_BIN_LDFLAGS += $(shell pkg-config --libs sndfile)
endif

include $(CHRE_PREFIX)/build/arch/x86.mk
include $(CHRE_PREFIX)/build/build_template.mk
endif
