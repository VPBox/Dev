#!/bin/bash

#
# Copyright (C) 2019 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set -Eeuo pipefail

TEST_NAME=$1
SPEC_NAME=${TEST_NAME//[\/.]/_}

GENERATOR_DIR=$ANDROID_BUILD_TOP/frameworks/ml/nn/tools/test_generator
NNTEST_DIR=$ANDROID_PRODUCT_OUT/data/nativetest

# Create tmp dir for spec dump.
LOG_DIR=$(mktemp -d)/nnapi-fuzzing-logs
mkdir -p $LOG_DIR
echo Creating logs in $LOG_DIR

# Save the fuzzer settings.
DUMPSPEC=$(adb shell getprop debug.nn.fuzzer.dumpspec)
LOGLEVEL=$(adb shell getprop debug.nn.fuzzer.log)

if [[ "$DUMPSPEC" == "" ]]; then
    DUMPSPEC="0"
fi

if [[ "$LOGLEVEL" == "" ]]; then
    LOGLEVEL="SILENCE"
fi

# Set to dump the spec file after the test.
adb shell setprop debug.nn.fuzzer.dumpspec 1
adb shell setprop debug.nn.fuzzer.log silence

# Run test.
adb push $NNTEST_DIR/NeuralNetworksTest_static_fuzzing/NeuralNetworksTest_static_fuzzing /data/local/tmp
adb shell /data/local/tmp/NeuralNetworksTest_static_fuzzing --gtest_filter=$TEST_NAME

# Apply the previous settings.
adb shell setprop debug.nn.fuzzer.dumpspec $DUMPSPEC
adb shell setprop debug.nn.fuzzer.log $LOGLEVEL

# Pull spec file, and generate HTML for visualization.
adb pull /data/local/tmp/${SPEC_NAME}.mod.py $LOG_DIR
$GENERATOR_DIR/spec_visualizer.py $LOG_DIR/${SPEC_NAME}.mod.py -o $LOG_DIR/${SPEC_NAME}.html

google-chrome $LOG_DIR/${SPEC_NAME}.html
