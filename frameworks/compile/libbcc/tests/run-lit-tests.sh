#!/bin/bash -e

LIT_PATH=$ANDROID_BUILD_TOP/frameworks/compile/libbcc/tests/debuginfo/llvm-lit
LIBBCC_TESTS=$ANDROID_BUILD_TOP/frameworks/compile/libbcc/tests/libbcc

$LIT_PATH $LIBBCC_TESTS $@
