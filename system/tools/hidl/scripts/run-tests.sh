#!/bin/bash

# See hal_hidl_gtest.py

THREADS=
CHECKER=vts_testability_checker
CHECKER_DEVICE_PATH="/data/local/tmp/${CHECKER}"
PRINT_COMMANDS=

function run() {
    if [ "${PRINT_COMMANDS}" = true ] ; then
        >&2 echo "*** $@"
    fi
    $@
}

function make_modules() {
    if [ "${THREADS}" != "0" ] ; then
        run make -j${THREADS} -C ${ANDROID_BUILD_TOP} -f build/core/main.mk $@
    fi
}

function push_checker() {
    run adb push ${OUT}/system/bin/${CHECKER} ${CHECKER_DEVICE_PATH}
}

function push_test() {
    local module=$1
    for test_dir in nativetest nativetest64 ; do
        local test_file=/data/${test_dir}/${module}/${module}
        run adb push ${OUT}${test_file} ${test_file}
    done
}

function read_checker_output() {
    python -c 'import json,sys;obj=json.load(sys.stdin);sys.stdout.write("%s\n"%obj["Testable"]);map(lambda i:sys.stdout.write("%s\n"%i),obj["instances"])'
}

function run_test() {
    local module=$1
    local status=0

    for test_dir in nativetest nativetest64 ; do
        local test_file=/data/${test_dir}/${module}/${module}
        local interfaces=$(run adb shell ${test_file} --list_registered_services \
            | sed -n 's/^hal_service: \(.*\)$/\1/p')
        if [ -z "$interfaces" ]; then
            run adb shell ${test_file} || status=$?
        else
            for interface in ${interfaces} ; do
                local output=$(run adb shell ${CHECKER_DEVICE_PATH} -c ${interface} | read_checker_output)
                local testable=$(echo "${output}" | head -n1)
                local instances=$(echo "${output}" | tail -n+2)

                if [ "${testable}" == "True" ] ; then
                    for instance in ${instances} ; do
                        run adb shell ${test_file} --hal_service_instance="${interface}/${instance}" || status=$?
                    done
                fi
            done
        fi
    done
    return ${status}
}

function usage() {
    echo "usage: $0 -m <module_name> [-m <module_name>[...]] [-j <jobs>] [-p]"
    echo "          -m <module_name>: name of test (e.g. VtsHalHealthV2_0TargetTest)"
    echo "          -p: print commands"
    echo "          -j <jobs>: # jobs in make. "
    echo "                     -j0 skips making any modules."
    echo "                     If not present, use infinite number of jobs."

    exit 1
}

function main() {
    local modules=

    while getopts "m:j:p" option ; do
        case "${option}" in
            m)
                [ ! -z ${OPTARG} ] || usage
                modules="${modules} ${OPTARG}"
                ;;
            j)
                THREADS=${OPTARG}
                ;;
            p)
                PRINT_COMMANDS=true
                ;;
            *)
                usage
                ;;
        esac
    done

    set -e
    make_modules ${CHECKER} ${modules}
    run adb root
    push_checker
    for module in ${modules} ; do
        push_test ${module}
    done

    set +e
    local status=0
    for module in ${modules} ; do
        run_test ${module} || status=$?
    done
    return ${status}
}

main $@
