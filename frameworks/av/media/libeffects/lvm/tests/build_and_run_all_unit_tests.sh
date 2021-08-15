#!/bin/bash
#
# Run tests in this directory.
#

if [ -z "$ANDROID_BUILD_TOP" ]; then
    echo "Android build environment not set"
    exit -1
fi

# ensure we have mm
. $ANDROID_BUILD_TOP/build/envsetup.sh

mm -j

echo "waiting for device"

adb root && adb wait-for-device remount

# location of test files
testdir="/data/local/tmp/lvmTest"

echo "========================================"
echo "testing lvm"
adb shell mkdir -p $testdir
adb push $ANDROID_BUILD_TOP/cts/tests/tests/media/res/raw/sinesweepraw.raw $testdir
adb push $OUT/testcases/snr/arm64/snr $testdir

E_VAL=1
if [ -z "$1" ]
then
    cmds=("adb push $OUT/testcases/lvmtest/arm64/lvmtest $testdir"
          "adb push $OUT/testcases/lvmtest/arm/lvmtest $testdir"
         )
elif [ "$1" == "32" ]
then
    cmds="adb push $OUT/testcases/lvmtest/arm/lvmtest $testdir"
elif [ "$1" == "64" ]
then
    cmds="adb push $OUT/testcases/lvmtest/arm64/lvmtest $testdir"
else
    echo ""
    echo "Invalid \"val\""
    echo "Usage:"
    echo "      "$0" [val]"
    echo "      where, val can be either 32 or 64."
    echo ""
    echo "      If val is not specified then both 32 bit and 64 bit binaries"
    echo "      are tested."
    exit $E_VAL
fi

flags_arr=(
    "-csE"
    "-eqE"
    "-tE"
    "-csE -tE -eqE"
    "-bE -M"
    "-csE -tE"
    "-csE -eqE" "-tE -eqE"
    "-csE -tE -bE -M -eqE"
    "-tE -eqE -vcBal:96 -M"
    "-tE -eqE -vcBal:-96 -M"
    "-tE -eqE -vcBal:0 -M"
    "-tE -eqE -bE -vcBal:30 -M"
)

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

# run multichannel effects at different configs, saving only the stereo channel
# pair.
error_count=0
for cmd in "${cmds[@]}"
do
    $cmd
    for flags in "${flags_arr[@]}"
    do
        for fs in ${fs_arr[*]}
        do
            for chMask in {0..22}
            do
                adb shell $testdir/lvmtest -i:$testdir/sinesweepraw.raw \
                    -o:$testdir/sinesweep_$((chMask))_$((fs)).raw -chMask:$chMask -fs:$fs $flags

                shell_ret=$?
                if [ $shell_ret -ne 0 ]; then
                    echo "error: $shell_ret"
                    ((++error_count))
                fi


                # two channel files should be identical to higher channel
                # computation (first 2 channels).
                # Do not compare cases where -bE is in flags (due to mono computation)
                if [[ $flags != *"-bE"* ]] && [[ "$chMask" -gt 1 ]]
                then
                    adb shell cmp $testdir/sinesweep_1_$((fs)).raw \
                        $testdir/sinesweep_$((chMask))_$((fs)).raw
                elif [[ $flags == *"-bE"* ]] && [[ "$chMask" -gt 1 ]]
                then
                    adb shell $testdir/snr $testdir/sinesweep_1_$((fs)).raw \
                        $testdir/sinesweep_$((chMask))_$((fs)).raw -thr:90.308998
                fi

                # both cmp and snr return EXIT_FAILURE on mismatch.
                shell_ret=$?
                if [ $shell_ret -ne 0 ]; then
                    echo "error: $shell_ret"
                    ((++error_count))
                fi
            done
        done
    done
done

adb shell rm -r $testdir
echo "$error_count errors"
exit $error_count
