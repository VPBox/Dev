#
# Build targets for an x86 processor
#

# x86 Environment Checks #######################################################

ifeq ($(ANDROID_BUILD_TOP),)
$(error "You should supply an ANDROID_BUILD_TOP environment variable \
         containing a path to the Android source tree. This is typically \
         provided by initializing the Android build environment.")
endif
export X86_TOOLS_PREFIX=$(ANDROID_BUILD_TOP)/prebuilts/clang/host/linux-x86/clang-r346389c/bin/

# x86 Tools ####################################################################

TARGET_AR  = $(X86_TOOLS_PREFIX)llvm-ar
TARGET_CC  = $(X86_TOOLS_PREFIX)clang++
TARGET_LD  = $(X86_TOOLS_PREFIX)clang++

# x86 Compiler Flags ###########################################################

# Add x86 compiler flags.
TARGET_CFLAGS += $(X86_CFLAGS)

# x86 is purely used for testing, so always include debugging symbols
TARGET_CFLAGS += -g

# Enable position independence.
TARGET_CFLAGS += -fpic

# x86 Shared Object Linker Flags ###############################################

TARGET_SO_LDFLAGS += -shared
TARGET_SO_LDFLAGS += -Wl,-gc-sections

# Optimization Level ###########################################################

TARGET_CFLAGS += -O$(OPT_LEVEL)

# Variant Specific Sources #####################################################

TARGET_VARIANT_SRCS += $(X86_SRCS)
