#!/usr/bin/env bash

set -e
set -u

readonly DEFAULT_TESTS="
    netdutils_test
    netd_unit_test
    netd_integration_test
    resolv_integration_test
    resolv_unit_test
"

readonly EXTENDED_TESTS="
    netd_benchmark
"

readonly TEST_DEVICE_PATH="/data/local/tmp"

REPO_TOP=""
SOONG_BIN=""
DEBUG=""

function logToStdErr() {
    echo "$1" >&2
}

function testAndSetRepoTop() {
    if [[ -n "$1" && -d "$1/.repo" ]]; then
        REPO_TOP="$1"
        SOONG_BIN="$REPO_TOP/build/soong/soong_ui.bash"
        return 0
    fi
    return 1
}

function gotoRepoTop() {
    if testAndSetRepoTop "$ANDROID_BUILD_TOP"; then
        return
    fi

    while ! testAndSetRepoTop "$PWD"; do
        if [[ "$PWD" == "/" ]]; then
            break
        fi
        cd ..
    done
}

function runCmd() {
    local cmd="$@"
    echo "#"
    echo "# $cmd"
    $DEBUG $cmd
    return $?
}

function runOneTest() {
    local test="$1"
    echo
    echo "###"
    echo "# $test"
    echo "###"

    local prefix="$ANDROID_TARGET_OUT_TESTCASES/$test/$TARGET_ARCH/$test"
    for bits in '' 32 64; do
            local testbin="${prefix}${bits}"
            if [ -f "$testbin" ]; then
                runCmd adb push "$testbin" "$TEST_DEVICE_PATH/$test" &&
                    runCmd adb shell "$TEST_DEVICE_PATH/$test" &&
                    runCmd adb shell "rm $TEST_DEVICE_PATH/$test"
                return $?
            fi
    done

    logToStdErr "Couldn't find test binary '$prefix'"
    return 1
}

function main() {
    if [ ! -v ANDROID_BUILD_TOP ]; then
        logToStdErr "You need to source and lunch before you can use this script"
        return 1
    fi
    gotoRepoTop
    if ! testAndSetRepoTop "$REPO_TOP"; then
        logToStdErr "Could not find useful top of repo directory"
        return 1
    fi
    logToStdErr "Using REPO_TOP=$REPO_TOP"

    TARGET_ARCH=$("$SOONG_BIN" --dumpvar-mode TARGET_ARCH)
    if [ -z "$TARGET_ARCH" ]; then
        logToStdErr "Could not determine TARGET_ARCH"
        return 1
    fi

    local test_regex=""
    while [[ $# -gt 0 ]]; do
        case "$1" in
            "-n")
                DEBUG=echo
                shift
                ;;
            *)
                # Allow us to do things like "runtests.sh integration", etc.
                test_regex="$1"
                shift
                ;;
        esac
    done

    # Try becoming root, in case the tests will be run on a device
    # with a userdebug build. Failure to become root is not fatal
    # for all (in fact, not even most) tests so don't exit on error.
    runCmd adb root || logToStdErr "WARNING: unable to 'adb root'"

    local failures=0
    local skipped=0

    # Find out which tests to run.
    local tests=""
    for testName in $DEFAULT_TESTS; do
        if [[ -z "$test_regex" || "$testName" =~ "$test_regex" ]]; then
            tests="$tests $testName"
        else
            logToStdErr "Skipping $testName"
            skipped=$((skipped + 1))
        fi
    done

    # If something has been specified, also check the extended tests for
    # a possible match (i.e. in order to run benchmarks, etc).
    if [[ -n "$test_regex" ]]; then
        for testName in $EXTENDED_TESTS; do
            if [[ "$testName" =~ "$test_regex" ]]; then
                tests="$tests $testName"
            fi
        done
    fi

    if [[ -z "$tests" ]]; then
        logToStdErr "No tests matched"
        return 1
    fi

    # Build all the specified tests.
    runCmd "$SOONG_BIN" --make-mode "$tests" || return $?

    # Run all the specified tests.
    for testName in $tests; do
        runOneTest "$testName" || failures=$((failures + 1))
    done

    echo "Number of tests failing: $failures"
    [[ $skipped -gt 0 ]] && echo "Number of tests skipped: $skipped"
    return $failures
}


main "$@"
exit $?
