#!/usr/bin/env bash

if [[ "$1" == "--help" ]]; then
  cat <<END
Usage for $0

	<no-args>			run all tests
	-r				print raw results
	-e class <class-name>		run all the tests in <class-name>
	-e class <class-name>#<method>	run just the specified <method>

Example:
$ $0 -r -e class \\
  com.android.server.wifi.WifiDiagnosticsTest#startLoggingRegistersLogEventHandler
Run just the specified test, and show the raw output.

For more options, see https://goo.gl/JxYjIw
END
  exit 0
fi

if [ -z $ANDROID_BUILD_TOP ]; then
  echo "You need to source and lunch before you can use this script"
  exit 1
fi

echo "Running tests"

set -e # fail early

echo "+ mmma -j32 $ANDROID_BUILD_TOP/frameworks/opt/net/wifi/tests"
# NOTE Don't actually run the command above since this shell doesn't inherit functions from the
#      caller.
$ANDROID_BUILD_TOP/build/soong/soong_ui.bash --make-mode MODULES-IN-frameworks-opt-net-wifi-tests
APK_NAME="$(ls -t $(find $OUT -name FrameworksWifiTests.apk) | head -n 1)"

set -x # print commands

adb root
adb wait-for-device

adb install -r -g "$APK_NAME"

adb shell am instrument --no-hidden-api-checks -w "$@" \
  'com.android.server.wifi.test/com.android.server.wifi.CustomTestRunner'
