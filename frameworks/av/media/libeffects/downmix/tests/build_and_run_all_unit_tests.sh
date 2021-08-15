#!/bin/bash
#
#Run tests in this directory.
#

if [ -z "$ANDROID_BUILD_TOP" ]; then
    echo "Android build environment not set"
    exit -1
fi
#ensure we have mm
. $ANDROID_BUILD_TOP/build/envsetup.sh

mm -j

echo "waiting for device"

adb root && adb wait-for-device remount

#location of test files
testdir="/data/local/tmp/downmixtest"

fs_arr=(
    8000
    11025
    12000
    16000
    22050
    24000
    32000
    44100
    48000
    88200
    96000
    176400
    192000
)

echo "========================================"
echo "testing Downmix"
adb shell mkdir $testdir

adb push $ANDROID_BUILD_TOP/cts/tests/tests/media/res/raw/sinesweepraw.raw \
$testdir
adb push $OUT/testcases/downmixtest/arm64/downmixtest $testdir

#run the downmix test application for test.
for fs in ${fs_arr[*]}
do
    for f_ch in {1..8}
    do
        for ch_fmt in {0..4}
        do
            adb shell  LD_LIBRARY_PATH=/vendor/lib64/soundfx \
            $testdir/downmixtest $testdir/sinesweepraw.raw \
            $testdir/sinesweep_fmt_$((ch_fmt))_fch_$((f_ch))_$((fs)).raw \
            -ch_fmt:$ch_fmt -fch:$f_ch -fs:$fs

            # Implementation dependent test:
            # check that higher frequencies match 8 kHz result.
            if [ $fs != 8000 ]
            then
                adb shell cmp \
                $testdir/sinesweep_fmt_$((ch_fmt))_fch_$((f_ch))_8000.raw \
                $testdir/sinesweep_fmt_$((ch_fmt))_fch_$((f_ch))_$((fs)).raw
            fi
        done
    done
done
adb shell rm -r $testdir
