#!/usr/bin/env bash
#
# Copyright (C) 2017 The Android Open Source Project
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
#
# Creates fake test data files by taking the files in $1 and turning them into ones
# that appear to be ones from IANA release $2. The resulting files are placed beneath $3.

REFERENCE_FILES_DIR=$1
OUTPUT_IANA_RULES_VERSION=$2
TEST_DATA_OUTPUT_DIR=$3

# Fail on error
set -e

TZDATA_FILE=iana/tzdata
TZDATA_LICENSE_FILE=iana/NOTICE
ICU_FILE=icu_overlay/icu_tzdata.dat
ICU_LICENSE_FILE=icu_overlay/LICENSE
TZLOOKUP_FILE=android/tzlookup.xml

INPUT_TZDATA_FILE=${REFERENCE_FILES_DIR}/${TZDATA_FILE}
INPUT_ICU_FILE=${REFERENCE_FILES_DIR}/${ICU_FILE}
INPUT_TZLOOKUP_FILE=${REFERENCE_FILES_DIR}/${TZLOOKUP_FILE}

TZHEADER=$(head -n1 ${INPUT_TZDATA_FILE} | cut -c1-11)
INPUT_IANA_RULES_VERSION=${TZHEADER:6}

OUTPUT_TZDATA_FILE=${TEST_DATA_OUTPUT_DIR}/${TZDATA_FILE}
OUTPUT_ICU_FILE=${TEST_DATA_OUTPUT_DIR}/${ICU_FILE}
OUTPUT_TZLOOKUP_FILE=${TEST_DATA_OUTPUT_DIR}/${TZLOOKUP_FILE}

mkdir -p ${TEST_DATA_OUTPUT_DIR}
mkdir -p ${TEST_DATA_OUTPUT_DIR}/iana
mkdir -p ${TEST_DATA_OUTPUT_DIR}/icu_overlay
mkdir -p ${TEST_DATA_OUTPUT_DIR}/android

# Create a new tzdata file.
sed "1s/^tzdata${INPUT_IANA_RULES_VERSION}/tzdata${OUTPUT_IANA_RULES_VERSION}/" ${INPUT_TZDATA_FILE} > ${OUTPUT_TZDATA_FILE}

# Create new ICU file.
SEARCH=$(echo ${INPUT_IANA_RULES_VERSION} | sed "s/\(.\)/\1\\\x00/g")
REPLACE=$(echo ${OUTPUT_IANA_RULES_VERSION} | sed "s/\(.\)/\1\\\x00/g")
sed "s/$SEARCH/$REPLACE/" ${INPUT_ICU_FILE} > ${OUTPUT_ICU_FILE}

# Copy the tzlookup.xml but update the ianaversion= attribute.
sed "s/${INPUT_IANA_RULES_VERSION}/${OUTPUT_IANA_RULES_VERSION}/" ${INPUT_TZLOOKUP_FILE} > ${OUTPUT_TZLOOKUP_FILE}

# Copy license files
cp ${REFERENCE_FILES_DIR}/${TZDATA_LICENSE_FILE} ${TEST_DATA_OUTPUT_DIR}/${TZDATA_LICENSE_FILE}
cp ${REFERENCE_FILES_DIR}/${ICU_LICENSE_FILE} ${TEST_DATA_OUTPUT_DIR}/${ICU_LICENSE_FILE}

echo Transformed input files with version ${INPUT_IANA_RULES_VERSION} to ${OUTPUT_IANA_RULES_VERSION} in ${TEST_DATA_OUTPUT_DIR}

