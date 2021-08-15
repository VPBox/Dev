# Script to gather perf and perf/watt data for several workloads
#
# Setup:
#
# - device connected to monsoon with USB passthrough enabled
# - network enabled (baseline will be measured and subtracted
#   from results) (network needed for chrome, youtube tests)
# - the device is rebooted after each test (can be inhibited
#   with "-r 0")
#
# Default behavior is to run each of the known workloads for
# 30 minutes gathering both performance and power data.
#
# The default time can be overridden with the -t option. To
# change individual test times, a config file can be specifed
# via -f with times for individual tests. Example file contents:
#
#	idleTime=60
#	recentflingTime=60
#	chromeTime=60
#	youtubeTime=0
#	sysappsTime=60
#	suntempleTime=5
#
# Output goes to the current directory.
#
# Examples:
#
# - Run all tests for 15 minutes (default is 30): ./pwrtest.sh -t 15 -R MDA20
#
# - Use a config file for test times: ./pwrtest.sh -f ./myconfig -R MDA20
#
# - Use a init file to setup device tuneables after each restart (this is
#   a bash script which should include adb commands to set up device):
#     ./pwrtest.sh -F devtunables
#

defaultTime=30
garbageminutes=8

function Usage {
	echo "Usage: $0 [OPTIONS]"
	echo "-d device : device type (shamu, bullhead, ...)"
	echo "-f configFile : config file to override individual test times"
	echo "-g garbageMinutes : time to skip power measurement at beginning of test"
	echo "                    default=$garbagetime minutes"
	echo "-r restart : 0=no reboot between tests, 1=reboot (default)"
	echo "-t defaultTimeMin : default time to run each test"
	echo "                    default=$defaultTime minutes"
	echo "-D cmddir : directory to find defs.sh"
	echo "-F restartHookFile : file of commands to set device tunables after restart (optional)"
	echo "-R release : release running on device (MDA20, 2054728, etc)"
}

restart=1
hz=5
shadowgrid2TimeMax=25

CMDDIR=$(dirname $0 2>/dev/null)
CMDDIR=${CMDDIR:=.}

MONSOON=monsoon.par

while [ $# -gt 0 ]
do
	case "$1" in
	(-D) CMDDIR=$2; shift;;
	(-r) restart=$2; shift;;
	(-t) defaultTime=$2; shift;;
	(-F) restartfile=$2; shift;;
	(-g) garbageminutes=$2; shift;;
	(-f)
		configFile=$2;
		echo "Reading configs from $configFile..."
		. ./$configFile
		shift;;
	(-R) echo $2 > ./build; shift;;
	(--) ;;
	(--help)
		Usage
		exit 0;;
	(*)
		echo "Unknown option: $1"
		Usage
		exit 1;;
	esac
	shift
done

. $CMDDIR/defs.sh --

devdir="/data/local/tmp"
suntempledir=${CMDDIR}/suntemple

case $DEVICE in
(shamu|hammerhead)
	HWUIMACRO=hwuimacro
	onSwipe="700 1847 700 400 50"
	;;
(*)
	HWUIMACRO=hwuimacro64
	onSwipe="500 1200 500 550 150"
	;;
esac

scripts="defs.sh systemapps.sh recentfling.sh youtube.sh chromefling.sh"

if ! $MONSOON >/dev/null 2>&1; then
	echo $MONSOON must be in your PATH >&2
	exit 1
fi

function usbpassthru {
	if [ "$1" = off ]; then
		state=off
	else
		state=on
	fi
	echo Setting usb pass-thru to $state
	monsoon.par --usbpassthrough=$state
}

function pwrcollect {
	collectmin=$1
	collectmin=${collectmin:=60}
	# samples = hz * 60 * minutes
	((samples=5*60*collectmin))
	monsoon.par --timestamp --samples $samples --hz 5
}

function copy_files {
	adb shell mkdir -p $devdir
	for file in $scripts
	do
		adb push $CMDDIR/$file $devdir
	done
}

function install_suntemple {
	echo Checking for suntemple installation...
	#stdest=/storage/sdcard0/obb/com.BrueComputing.SunTemple
	stdest=/storage/emulated/0/obb/com.BrueComputing.SunTemple
	dircontents=$(adb ls $stdest 2>/dev/null)
	if [ "$dircontents" = "" ]; then
		echo Installing suntemple...
		adb install $suntempledir/*.apk
		adb shell mkdir -p $stdest
		adb push $suntempledir/main*obb $stdest
	else
		echo dircontents=$dircontents
		echo Suntemple already installed.
	fi
}

function run_test {
	testName=$1
	collectMinutes=$2
	collectOutput=${testName}-power-raw.out
	powerOutput=${testName}-power.out
	echo -----------------------------------------------------
	echo TEST: $testName
	echo enabled Cores $(adb shell "cat /sys/devices/system/cpu/online")
	date
	echo -----------------------------------------------------
	usbpassthru off
	pwrcollect $collectMinutes > $collectOutput 2>/dev/null
	# take off the first 2min of samples
	totalSamples=$(cat $collectOutput | wc -l)
	# we throw away the first "garbageminutes" of the data
	# since it is volatile
	((garbage=hz*60*garbageminutes))
	((remaining=totalSamples-garbage))
	if [ $remaining -gt 0 ]; then
		tail -$remaining $collectOutput > $powerOutput
	else
		cp $collectOutput $powerOutput
	fi
	echo power data for $testName copied to $collectOutput
	usbpassthru on
	sleep 10
	adb devices
	sleep 10
}

function start_job {
	cmdline="$1"
	echo Running $cmdline
	(adb shell "cd $devdir && nohup $cmdline > test.out") &
	sleep 5
	kill %1 2>/dev/null
}

function cleanup_job {
	testName=$1
	processName=$2
	processName=${processName:=" sh "}
	set -- $(adb shell ps | tr "\r" " " | grep "$processName")
	echo killing PID=$2...
	adb shell kill $2
	sleep 1
	echo copying test output to $testName...
	adb pull $devdir/test.out
	mv test.out ${testName}.out
	if [ $restart -gt 0 ]; then
		restart_device
	else
		doKeyevent HOME
	fi
}

function airplane_mode {
	if [ "$1" = "on" ]; then
		mode=true
		setting=1
	else
		mode=false
		setting=0
	fi
	adb shell settings put global airplane_mode_on $setting
	adb shell am broadcast -a android.intent.action.AIRPLANE_MODE --ez state $mode
	echo Set airplane mode to $mode
}

function restart_device {
	adb reboot
	echo Wait 60s for device to restart...
	sleep 60
	while ! adb root
	do
		echo Waiting for device to come up...
		sleep 10
	done
	echo Wait 30s to complete boot activities...
	sleep 30
	echo Restart complete.
	doTap 897 1075
	sleep 2
	doSwipe $onSwipe
	restartfile=${restartfile:="./restarthook"}
	if [ -f $restartfile ]; then
		# hook to change tunables after a restart
		. $restartfile
	fi
}

usbpassthru on
adb devices 2>/dev/null

airplane_mode off
if [ $restart -gt 0 ]; then
	restart_device
fi

echo Copying $scripts to device $devdir...
copy_files
adb shell ln -s /data/benchmarktest/hwuimacro/$HWUIMACRO $devdir/$HWUIMACRO
tests=""

# measure background power
idleTime=${idleTime:=$defaultTime}
if [ $idleTime -gt 0 ]; then
	echo Test 1 : measure idle power for $idleTime minutes
	run_test idle $idleTime
	airplane_mode on
	echo Restarting for power baseline in airplane mode...
	restart_device
	run_test idle-airplane $idleTime
	airplane_mode off
	# the screen blanks after 30 minutes. The first 2 minutes of the test
	# have already been filtered off. For our power baseline, keep the first
	# 20 minutes of the results
	((twentyminutes=hz*20*60))
	powerOutput="idle-power.out"
	displayPowerOutput="idle-display-power.out"
	airplanePowerOutput="idle-airplane-power.out"
	airplaneDisplayPowerOutput="idle-airplane-display-power.out"
	totalSamples=$(cat $powerOutput | wc -l)
	if [ $twentyminutes -lt $totalSamples ]; then
		head -$twentyminutes $powerOutput > $displayPowerOutput
		head -$twentyminutes $airplanePowerOutput > $airplaneDisplayPowerOutput
	else
		cp $powerOutput $displayPowerOutput
		cp $airplanePowerOutput $airplaneDisplayPowerOutput
	fi
	tests="$tests idle"
fi

recentflingTime=${recentflingTime:=$defaultTime}
if [ $recentflingTime -gt 0 ]; then
	echo $(date) Test 2 : recents fling for $recentflingTime minutes
	airplane_mode on
	adb shell "cd $devdir && ./systemapps.sh -A -T -i 1"
	start_job "./recentfling.sh -N -i 1000 -d $DEVICE"
	run_test recentfling $recentflingTime
	cleanup_job recentfling
	airplane_mode off
	date
	tests="$tests recentfling"
fi

suntempleTime=${suntempleTime:=$defaultTime}
if [ $suntempleTime -gt 0 ]; then
	echo $(date) Test 2 : run Sun Temple $suntempleTime minutes
	airplane_mode on
	install_suntemple
	adb shell "am start $suntempleActivity"
	run_test suntemple $suntempleTime
	adb pull /sdcard/SunTemple/SunTemple/Saved/Logs/SunTemple.log
	cleanup_job suntemple BrueComp
	airplane_mode off
	mv SunTemple.log suntemple.out
	# grab the suntemple log
	date
	tests="$tests suntemple"
fi

chromeTime=${chromeTime:=$defaultTime}
if [ $chromeTime -gt 0 ]; then
	echo $(date) Test 3 : chrome fling for $chromeTime minutes
	start_job "./chromefling.sh -i 1000 -d $DEVICE"
	run_test chrome $chromeTime
	cleanup_job chrome
	date
	tests="$tests chrome"
fi

shadowgrid2Time=${shadowgrid2Time:=$defaultTime}
if [ $shadowgrid2Time -gt $shadowgrid2TimeMax ]; then
	# we cap shadowgrid2 time since the display goes
	# off after 30 minutes
	$shadowgrid2Time=$shadowgrid2TimeMax
fi
if [ $shadowgrid2Time -gt 0 ]; then
	airplane_mode on
	echo $(date) Test 4 : shadowgrid2 for $shadowgrid2Time minutes
	start_job "./$HWUIMACRO --onscreen shadowgrid2 100000"
	run_test shadowgrid2 $shadowgrid2Time
	cleanup_job shadowgrid2 $HWUIMACRO
	airplane_mode off
	date
	tests="$tests shadowgrid2"
fi

youtubeTime=${youtubeTime:=$defaultTime}
if [ $youtubeTime -gt 0 ]; then
	echo $(date) Test 5 : youtube for $youtubeTime minutes
	start_job "./youtube.sh -i 1000 -d $DEVICE"
	run_test youtube $youtubeTime
	cleanup_job youtube
	date
	tests="$tests youtube"
fi

sysappsTime=${sysappsTime:=$defaultTime}
if [ $sysappsTime -gt 0 ]; then
	echo $(date) Test 6 : app switching for $sysappsTime minutes
	start_job "./systemapps.sh -T -i 1000 -d $DEVICE"
	run_test sysapps $sysappsTime
	cleanup_job sysapps
	date
	tests="$tests sysapps"
fi

echo Ran tests: $tests
echo $tests > tests

