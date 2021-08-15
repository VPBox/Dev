#!/bin/bash
#
# This script uses test-mixer to generate WAV files
# for evaluation of the AudioMixer component.
#
# Sine and chirp signals are used for input because they
# show up as clear lines, either horizontal or diagonal,
# on a spectrogram. This means easy verification of multiple
# track mixing.
#
# After execution, look for created subdirectories like
# mixer_i_i
# mixer_i_f
# mixer_f_f
#
# Recommend using a program such as audacity to evaluate
# the output WAV files, e.g.
#
# cd testdir
# audacity *.wav
#
# Using Audacity:
#
# Under "Waveform" view mode you can zoom into the
# start of the WAV file to verify proper ramping.
#
# Select "Spectrogram" to see verify the lines
# (sine = horizontal, chirp = diagonal) which should
# be clear (except for around the start as the volume
# ramping causes spectral distortion).

if [ -z "$ANDROID_BUILD_TOP" ]; then
    echo "Android build environment not set"
    exit -1
fi

# ensure we have mm
. $ANDROID_BUILD_TOP/build/envsetup.sh

pushd $ANDROID_BUILD_TOP/frameworks/av/media/libaudioprocessing

# build
pwd
mm

# send to device
echo "waiting for device"
adb root && adb wait-for-device remount
adb push $OUT/system/lib/libaudioprocessing.so /system/lib
adb push $OUT/system/lib64/libaudioprocessing.so /system/lib64
adb push $OUT/system/bin/test-mixer /system/bin

# createwav creates a series of WAV files testing various
# mixer settings
# $1 = flags
# $2 = directory
function createwav() {
# create directory if it doesn't exist
    if [ ! -d $2 ]; then
        mkdir $2
    fi

# Test:
# process__genericResampling with mixed integer and float track input
# track__Resample / track__genericResample
    adb shell test-mixer $1 -s 48000 \
        -o /sdcard/tm48000grif.wav \
        sine:2,4000,7520 chirp:2,9200 sine:1,3000,18000 \
        sine:f,6,6000,19000  chirp:i,4,30000
    adb pull /sdcard/tm48000grif.wav $2

# Test:
# process__genericResampling
# track__Resample / track__genericResample
    adb shell test-mixer $1 -s 48000 \
        -o /sdcard/tm48000gr.wav \
        sine:2,4000,7520 chirp:2,9200 sine:1,3000,18000 \
        sine:6,6000,19000
    adb pull /sdcard/tm48000gr.wav $2

# Test:
# process__genericResample
# track__Resample / track__genericResample
# track__NoResample / track__16BitsStereo / track__16BitsMono
# Aux buffer
    adb shell test-mixer $1 -c 5 -s 9307 \
        -a /sdcard/aux9307gra.wav -o /sdcard/tm9307gra.wav \
        sine:4,1000,3000 sine:1,2000,9307 chirp:3,9307
    adb pull /sdcard/tm9307gra.wav $2
    adb pull /sdcard/aux9307gra.wav $2

# Test:
# process__genericNoResampling
# track__NoResample / track__16BitsStereo / track__16BitsMono
    adb shell test-mixer $1 -s 32000 \
        -o /sdcard/tm32000gnr.wav \
        sine:2,1000,32000 chirp:2,32000  sine:1,3000,32000
    adb pull /sdcard/tm32000gnr.wav $2

# Test:
# process__genericNoResampling
# track__NoResample / track__16BitsStereo / track__16BitsMono
# Aux buffer
    adb shell test-mixer $1 -s 32000 \
        -a /sdcard/aux32000gnra.wav -o /sdcard/tm32000gnra.wav \
        sine:2,1000,32000 chirp:2,32000  sine:1,3000,32000
    adb pull /sdcard/tm32000gnra.wav $2
    adb pull /sdcard/aux32000gnra.wav $2

# Test:
# process__NoResampleOneTrack / process__OneTrack16BitsStereoNoResampling
# Downmixer
    adb shell test-mixer $1 -s 32000 \
        -o /sdcard/tm32000nrot.wav \
        sine:6,1000,32000
    adb pull /sdcard/tm32000nrot.wav $2

# Test:
# process__NoResampleOneTrack / OneTrack16BitsStereoNoResampling
# Aux buffer
    adb shell test-mixer $1 -s 44100 \
        -a /sdcard/aux44100nrota.wav -o /sdcard/tm44100nrota.wav \
        sine:2,2000,44100
    adb pull /sdcard/tm44100nrota.wav $2
    adb pull /sdcard/aux44100nrota.wav $2
}

#
# Call createwav to generate WAV files in various combinations
#
# i_i = integer input track, integer mixer output
# f_f = float input track,   float mixer output
# i_f = integer input track, float_mixer output
#
# If the mixer output is float, then the output WAV file is pcm float.
#
# TODO: create a "snr" like "diff" to automatically
# compare files in these directories together.
#

createwav "" "tests/mixer_i_i"
createwav "-f -m" "tests/mixer_f_f"
createwav "-m" "tests/mixer_i_f"

popd
