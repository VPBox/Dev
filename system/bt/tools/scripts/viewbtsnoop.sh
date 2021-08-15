#!/bin/bash

# This is a script that will allow you to open a btsnooz log
# in wireshark directly from a zip file. This script will handle
# the unzipping if it is a zip file, or will convert the btsnooz
# directly in the case of a plain text file for use with wireshark.
# After wireshark closes, it will clean up all the temporary files
# used.

WIRESHARK="${WIRESHARK:-wireshark}"
BTSNOOZ="${BTSNOOZ:-btsnooz.py}"

if ! hash "${WIRESHARK}" 2>/dev/null;
then
    echo "Please make sure wireshark is in your path before running."
    exit 1;
fi

if ! hash btsnooz.py 2>/dev/null;
then
    echo "Please make sure btsnooz.py is in your path before running."
    exit 2;
fi

if [ $# -eq 0 ];
then
    echo "Usage: $0 bugreport(.txt|.zip)"
    exit 3;
fi

BUGREPORT="$1"
FILENAME="$(basename ${BUGREPORT})"
TMPDIR=$(mktemp --tmpdir -d "viewbtsnooz_XXXXX")
LOGFILE="${TMPDIR}/${FILENAME%.*}.btsnooz"

trap ctrl_c INT
function ctrl_c() {
    rm -rf "${TMPDIR}"
}

if [ ! -f "${BUGREPORT}" ];
then
    echo "File ${BUGREPORT} does not exist."
    exit 4;
fi

if [ ! -d "${TMPDIR}" ];
then
    echo "Unable to create temp. dir (${TMPDIR}) :("
    exit 5;
fi

if [ "${BUGREPORT: -4}" == ".zip" ];
then
    unzip "${BUGREPORT}" -d "${TMPDIR}"
    BUGREPORT="${TMPDIR}/${FILENAME%.*}.txt"
fi

if [ -f "${BUGREPORT}" ];
then
    ${BTSNOOZ} "${BUGREPORT}" > "${LOGFILE}"
    if [ ! $? -eq 0 ];
    then
        echo "Could not extract btsnooz data from ${BUGREPORT}."
        rm -rf "${TMPDIR}"
        exit 6;
    fi

    ${WIRESHARK} "${LOGFILE}"
else
    echo "Looks like there is no plain text bugreport (${BUGREPORT})?"
fi

rm -rf "${TMPDIR}"
