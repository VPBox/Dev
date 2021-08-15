#!/bin/bash

# Quit if any command produces an error.
set -e

# Build and run the CHRE unit test binary.
JOB_COUNT=$((`grep -c ^processor /proc/cpuinfo`))

# Export the variant Makefile.
export CHRE_VARIANT_MK_INCLUDES="$CHRE_VARIANT_MK_INCLUDES \
  variant/googletest/variant.mk"

make google_x86_googletest_debug -j$JOB_COUNT
./out/google_x86_googletest_debug/libchre $1
