#!/bin/bash

# Copyright 2017, The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# A script which runs the unittest tests in slang_test.py
# and outputs the results to $1/slang_tests/slang_tests_out.txt
# $1 is expected to be DIST_DIR
# We use python2.7 because we need unittest features added in 2.7

mkdir -p "$1"/slang_tests/
/usr/bin/python2.7 frameworks/compile/slang/tests/slang_test.py -z 2> "$1"/slang_tests/slang_tests_out.txt
# if python could not run script, return code will be non-zero
# if any tests fail, return code will be zero due to '-z' flag passed to slang_test.py
exit $?
