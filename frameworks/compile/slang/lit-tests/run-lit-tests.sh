#!/bin/bash -e

LIT_PATH=$ANDROID_BUILD_TOP/frameworks/compile/libbcc/tests/debuginfo/llvm-lit
TESTS=$ANDROID_BUILD_TOP/frameworks/compile/slang/lit-tests

$LIT_PATH $TESTS $@
