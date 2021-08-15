#!/bin/bash -e

# Path to flatc compiler
FLATC_PATH="$ANDROID_BUILD_TOP/external/flatbuffers/flatc"

# Flat buffer schema files
FLATC_SRCS="flatbuffers_types.fbs"

# Flatc arguments
FLATC_ARGS="--cpp --no-includes"

# Generate c++ code
$FLATC_PATH $FLATC_ARGS $FLATC_SRCS
