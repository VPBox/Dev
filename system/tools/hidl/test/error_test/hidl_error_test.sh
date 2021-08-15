#!/bin/bash

if [ $# -gt 1 ]; then
    echo "usage: hidl_error_test.sh hidl-gen_path"
    exit 1
fi

readonly HIDL_GEN_PATH=${1:-hidl-gen}
readonly HIDL_ERROR_TEST_DIR="${ANDROID_BUILD_TOP:-.}/system/tools/hidl/test/error_test"

if [ ! -d $HIDL_ERROR_TEST_DIR ]; then
    echo "cannot find test directory: $HIDL_ERROR_TEST_DIR"
    exit 1
fi

for dir in $(ls -d $HIDL_ERROR_TEST_DIR/*/); do
  package=$(basename $dir)
  output=$($HIDL_GEN_PATH -L check -r test:$HIDL_ERROR_TEST_DIR test.$package@1.0 2>&1)
  command_fails=$?
  error=$(cat $HIDL_ERROR_TEST_DIR/$package/1.0/required_error)

  if [[ $error == "" ]]; then
    echo "error: No required error message specified for $package."
    echo "$output" | while read line; do echo "test output: $line"; done
    exit 1
  fi

  if [ $command_fails -ne 1 ]; then
    echo "error: $package test did not fail"
    exit 1
  fi

  if [[ $output != *$error* ]]; then
    echo "error: error output for $package does not contain '$error':"
    echo "$output" | while read line; do echo "test output: $line"; done
    exit 1
  fi

done
