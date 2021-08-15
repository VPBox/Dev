#!/system/bin/sh
# Run a loopback test in the background after a delay.
# To run the script, enter these commands once:
#    adb disable-verity
#    adb reboot
#    adb remount
#    adb sync
#    adb push loopback.sh /data/
# For each test run:
#    adb shell "nohup sh /data/loopback.sh &"
# Quickly connect USB audio if needed, either manually or via Tigertail switch.
# Wait until the test completes, restore USB to host if needed, and then:
#    adb pull /data/loopreport.txt
#    adb pull /data/loopback_all.wav
#    adb pull /data/loopback_echos.wav

SLEEP_TIME=10
TEST_COMMAND="/data/nativetest/aaudio_loopback/aaudio_loopback -pl -Pl -C1 -n2 -m2 -te -d5"
# Partial list of options:
#   -pl (output) performance mode: low latency
#   -Pl input performance mode: low latency
#   -C1 input channel count: 1
#   -n2 number of bursts: 2
#   -m2 mmap policy: 2
#   -t? test mode: -tm for sine magnitude, -te for echo latency, -tf for file latency
#   -d5 device ID
# For full list of available options, see AAudioArgsParser.h and loopback.cpp

echo "Plug in USB Mir and Fun Plug."
echo "Test will start in ${SLEEP_TIME} seconds: ${TEST_COMMAND}"
sleep ${SLEEP_TIME}
date > /data/loopreport.txt
${TEST_COMMAND} >> /data/loopreport.txt
date >> /data/loopreport.txt
