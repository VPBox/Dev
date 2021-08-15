#
# Build targets for a Hexagon-based processor
#

# Hexagon Environment Checks ###################################################

# Ensure that the user has specified a path to the Hexagon toolchain that they
# wish to use.
ifeq ($(HEXAGON_TOOLS_PREFIX),)
$(error "You must supply a HEXAGON_TOOLS_PREFIX environment variable \
         containing a path to the hexagon toolchain. Example: \
         export HEXAGON_TOOLS_PREFIX=$$HOME/Qualcomm/HEXAGON_Tools/8.0.07")
endif

ifeq ($(IS_NANOAPP_BUILD),)
ifeq ($(SLPI_PREFIX),)
$(error "You must supply an SLPI_PREFIX environment variable \
         containing a path to the SLPI source tree. Example: \
         export SLPI_PREFIX=$$HOME/slpi_proc")
endif
endif

# Hexagon Tools ################################################################

TARGET_AR = $(HEXAGON_TOOLS_PREFIX)/Tools/bin/hexagon-ar
TARGET_CC = $(HEXAGON_TOOLS_PREFIX)/Tools/bin/hexagon-clang
TARGET_LD = $(HEXAGON_TOOLS_PREFIX)/Tools/bin/hexagon-link

# Hexagon Compiler Flags #######################################################

# Define CUST_H to allow including the customer header file.
TARGET_CFLAGS += -DCUST_H='"custaaaaaaaaq.h"'

# Add Hexagon compiler flags
TARGET_CFLAGS += $(HEXAGON_CFLAGS)

# Enable position independence.
TARGET_CFLAGS += -fpic

# Disable splitting double registers.
TARGET_CFLAGS += -mllvm -disable-hsdr

# Don't use small data section.
TARGET_CFLAGS += -G0

# Enable default visibility for FastRPC entry points.
TARGET_CFLAGS += -D'__QAIC_SKEL_EXPORT=__attribute__((visibility("default")))'

# This code is loaded into a dynamic module. Define this symbol in the event
# that any Qualcomm code needs it.
TARGET_CFLAGS += -D__V_DYNAMIC__

# This flag is used by some QC-supplied code to differentiate things intended to
# run on Hexagon vs. other architectures
TARGET_CFLAGS += -DQDSP6

# Hexagon Shared Object Linker Flags ###########################################

TARGET_SO_LDFLAGS += --gc-sections
TARGET_SO_LDFLAGS += -shared
TARGET_SO_LDFLAGS += -call_shared
TARGET_SO_LDFLAGS += -Bsymbolic
TARGET_SO_LDFLAGS += --wrap=malloc
TARGET_SO_LDFLAGS += --wrap=calloc
TARGET_SO_LDFLAGS += --wrap=free
TARGET_SO_LDFLAGS += --wrap=realloc
TARGET_SO_LDFLAGS += --wrap=memalign
TARGET_SO_LDFLAGS += --wrap=__stack_chk_fail
TARGET_SO_LDFLAGS += --no-threads

HEXAGON_LIB_PATH = $(HEXAGON_TOOLS_PREFIX)/Tools/target/hexagon/lib
TARGET_SO_EARLY_LIBS += $(HEXAGON_LIB_PATH)/$(HEXAGON_ARCH)/G0/pic/initS.o
TARGET_SO_LATE_LIBS += $(HEXAGON_LIB_PATH)/$(HEXAGON_ARCH)/G0/pic/finiS.o

# Supported Hexagon Architectures ##############################################

HEXAGON_SUPPORTED_ARCHS = v55 v60 v62 v65 v66

# Environment Checks ###########################################################

# Ensure that an architecture is chosen.
ifeq ($(filter $(HEXAGON_ARCH), $(HEXAGON_SUPPORTED_ARCHS)),)
$(error "The HEXAGON_ARCH variable must be set to a supported architecture \
         ($(HEXAGON_SUPPORTED_ARCHS))")
endif

# Target Architecture ##########################################################

# Set the Hexagon architecture.
TARGET_CFLAGS += -m$(strip $(HEXAGON_ARCH))

# Optimization Level ###########################################################

TARGET_CFLAGS += -O$(OPT_LEVEL)

# TODO: Consider disabling this when compiling for >-O0.
TARGET_CFLAGS += -D_DEBUG

# Variant Specific Sources #####################################################

TARGET_VARIANT_SRCS += $(HEXAGON_SRCS)
