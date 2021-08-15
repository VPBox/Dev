#!/bin/bash

# dir with prebuilts in it
# contains subdirs named 19.1.0, 20.0.0, etc.
PREBUILT_DIR=~/android/ndk-master/prebuilts/renderscript/host/linux-x86/
# aosp dir
AOSP_DIR=~/android/aosp
# throwaway dir
TMP=/tmp/bad

mkdir -p $TMP

# for each dir with pattern #.#.#
for version in $PREBUILT_DIR/*.*.*; do
  echo $(basename $version)
  for unittest in $AOSP_DIR/frameworks/rs/tests/java_api/RSUnitTests/src/com/android/rs/unittest/*.rs ; do
    echo $(basename $unittest)
    txtfile=$(basename $version | tr '.' '_').txt
    ($version/bin/llvm-rs-cc $unittest -o $TMP -p $TMP -I $version/include/ -I $version/clang-include/ 1> /dev/null 2> /dev/null) && (echo $(basename $unittest) | sed 's/...$//') >> $txtfile
  done
done
