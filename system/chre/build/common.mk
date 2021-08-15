#
# Common Makefile Rules
#

# Environment Checks ###########################################################

ifeq ($(OPT_LEVEL),)
$(warning The OPT_LEVEL variable is unset. Defaulting to 0.)
OPT_LEVEL = 0
endif

ifeq ($(OUTPUT_NAME),)
$(error "The OUTPUT_NAME variable must be set to the name of the desired \
         binary. Example: OUTPUT_NAME = my_nanoapp")
endif

# Define all ###################################################################

# All is defined here as the first target which causes make to build all by
# default when no targets are supplied.
.PHONY: all
all:

# If no make command goals are specified, default to all. At least one target
# is required for environment checks. Building all will require toolchains for
# all supported architectures which may not be desirable.
ifeq ($(MAKECMDGOALS),)
MAKECMDGOALS = all
endif

# Variant-specific Support Source Files ########################################

SYS_SUPPORT_PATH = $(CHRE_PREFIX)/build/sys_support

# Host Toolchain ###############################################################

# The host toolchain is used to compile any programs for the compilation host
# in order to complete the build.

ifeq ($(CHRE_HOST_CC),)
CHRE_HOST_CC = g++
endif

# Makefile Includes ############################################################

# Common Includes
include $(CHRE_PREFIX)/build/clean.mk
include $(CHRE_PREFIX)/build/tools_config.mk

# NanoPB Source Generation
include $(CHRE_PREFIX)/build/nanopb.mk
