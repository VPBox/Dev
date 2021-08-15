#
# Build targets for an ARM64 processor
#

# arm64 Environment Checks #####################################################

ifeq ($(ANDROID_NDK_PREFIX),)
$(error "You should supply an ANDROID_NDK_PREFIX environment variable \
         containing a path to a generated Android NDK toolchain. i.e. \
         ./build/tools/make-standalone-toolchain.sh --arch=arm64 \
         --platform=android-26 --install-dir=$$ANDROID_NDK_PREFIX")
endif

export ARM64_TOOLS_PREFIX=$(ANDROID_NDK_PREFIX)/bin

# arm64 Tools ##################################################################

TARGET_AR = $(ARM64_TOOLS_PREFIX)/llvm-ar
TARGET_CC = $(ARM64_TOOLS_PREFIX)/clang++
TARGET_LD = $(ARM64_TOOLS_PREFIX)/clang++

# arm64 Compiler Flags #########################################################

# Add arm64 compiler flags.
TARGET_CFLAGS += $(ARM64_CFLAGS)

# Generate a position independent executable.
TARGET_CFLAGS += -fPIE

# arm64 Shared Object Linker Flags #############################################

TARGET_SO_LDFLAGS += -shared
TARGET_SO_LDFLAGS += -Wl,-gc-sections

# Optimization Level ###########################################################

TARGET_CFLAGS += -O$(OPT_LEVEL)

# Variant Specific Sources #####################################################

TARGET_VARIANT_SRCS += $(ARM64_SRCS)
