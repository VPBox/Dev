#!/bin/bash

SCRIPT_DIR="$(dirname $0)"
DIST_DIR="$1"

STUDIO_JDK=${SCRIPT_DIR}"/../../../../prebuilts/studio/jdk/linux"
MISC_COMMON=${SCRIPT_DIR}"/../../../../prebuilts/misc/common"
M2_REPO=${SCRIPT_DIR}"/../../../../prebuilts/tools/common/m2/repository"
JAVA_LIBRARIES=${SCRIPT_DIR}"/../../../../out/host/common/obj/JAVA_LIBRARIES"

${STUDIO_JDK}/bin/java -ea \
    -Dtest_res.dir=${SCRIPT_DIR}/res \
    -Dtest_failure.dir=${DIST_DIR}/layoutlib_failures \
    -cp ${M2_REPO}/junit/junit/4.12/junit-4.12.jar:${M2_REPO}/org/hamcrest/hamcrest-core/1.3/hamcrest-core-1.3.jar:${MISC_COMMON}/tools-common/tools-common-prebuilt.jar:${MISC_COMMON}/sdk-common/sdk-common.jar:${MISC_COMMON}/layoutlib_api/layoutlib_api-prebuilt.jar:${MISC_COMMON}/kxml2/kxml2-2.3.0.jar:${M2_REPO}/com/google/guava/guava/22.0/guava-22.0.jar:${JAVA_LIBRARIES}/layoutlib-tests_intermediates/javalib.jar:${JAVA_LIBRARIES}/layoutlib_intermediates/javalib.jar:${JAVA_LIBRARIES}/mockito-host_intermediates/javalib.jar:${JAVA_LIBRARIES}/objenesis-host_intermediates/javalib.jar \
    org.junit.runner.JUnitCore \
    com.android.layoutlib.bridge.intensive.Main

