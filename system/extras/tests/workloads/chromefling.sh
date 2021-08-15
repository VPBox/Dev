#
# Script to start 3 chrome tabs, fling each of them, repeat
# For each iteration, Total frames and janky frames are reported.
#
# Options are described below.
#
iterations=10
startapps=1
capturesystrace=0
waittime=4
app=chrome

function processLocalOption {
	ret=0
	case "$1" in
	(-N) startapps=0;;
	(-A) unset appList;;
	(-L) appList=$2; shift; ret=1;;
	(-T) capturesystrace=1;;
	(-W) waittime=$2; shift; ret=1;;
	(*)
		echo "$0: unrecognized option: $1"
		echo; echo "Usage: $0 [options]"
		echo "-A : use all known applications"
		echo "-L applist : list of applications"
		echo "   default: $appList"
		echo "-N : no app startups, just fling"
		echo "-g : generate activity strings"
		echo "-i iterations"
		echo "-T : capture systrace on each iteration"
		echo "-d device : device type (shamu, volantis, bullhead,...)"
		exit 1;;
	esac
	return $ret
}

CMDDIR=$(dirname $0 2>/dev/null)
CMDDIR=${CMDDIR:=.}
. $CMDDIR/defs.sh

case $DEVICE in
(hammerhead)
	flingtime=300
	downCount=2
	upCount=6
	UP="70 400 70 100 $flingtime"
	DOWN="70 100 70 400 $flingtime";;
(shamu)
	flingtime=100
	downCount=2 
	upCount=2
	UP="700 1847 700 400 $flingtime"
	DOWN="700 400 700 1847 $flingtime";;
(angler)
	flingtime=150
	downCount=4
	upCount=3
	UP="500 1200 500 550 $flingtime"
	DOWN="500 550 500 1200 $flingtime";;
(bullhead|volantis)
	flingtime=200
	downCount=5
	upCount=5
	UP="500 1400 500 400 $flingtime"
	DOWN="500 400 500 1400 $flingtime";;
(ariel)
	flingtime=200
	downCount=5
	upCount=5
	UP="500 1560 500 530 $flingtime"
	DOWN="500 530 500 1560 $flingtime";;
(*)
	echo "Error: No display information available for $DEVICE"
	exit 1;;
esac

function swipe {
	count=0
	while [ $count -lt $2 ]
	do
		doSwipe $1
		((count=count+1))
	done
	sleep 1
}

cur=1
frameSum=0
jankSum=0
latency90Sum=0
latency95Sum=0
latency99Sum=0

doKeyevent HOME
sleep 0.5
resetJankyFrames $(getPackageName $app)

while [ $cur -le $iterations ]
do
	if [ $capturesystrace -gt 0 ]; then
		${ADB}atrace --async_start -z -c -b 16000 freq gfx view idle sched
	fi
	t=$(startActivity $app)
	sleep $waittime
	swipe "$UP" $upCount

	sleep $waittime
	swipe "$DOWN" $downCount

	doKeyevent BACK
	sleep 0.5

	if [ $capturesystrace -gt 0 ]; then
		${ADB}atrace --async_dump -z -c -b 16000 freq gfx view idle sched > trace.${cur}.out
	fi
	doKeyevent HOME
	sleep 0.5

	set -- $(getJankyFrames $(getPackageName $app))
	totalDiff=$1
	jankyDiff=$2
	latency90=$3
	latency95=$4
	latency99=$5
	if [ ${totalDiff:=0} -eq 0 ]; then
		echo Error: could not read frame info with \"dumpsys gfxinfo\"
		exit 1
	fi

	((frameSum=frameSum+totalDiff))
	((jankSum=jankSum+jankyDiff))
	((latency90Sum=latency90Sum+latency90))
	((latency95Sum=latency95Sum+latency95))
	((latency99Sum=latency99Sum+latency99))
	if [ "$totalDiff" -eq 0 ]; then
		echo Error: no frames detected. Is the display off?
		exit 1
	fi
	((jankPct=jankyDiff*100/totalDiff))
	resetJankyFrames $(getPackageName $app)


	echo Frames: $totalDiff latency: $latency90/$latency95/$latency99 Janks: $jankyDiff\(${jankPct}%\)
	((cur=cur+1))
done
doKeyevent HOME
((aveJankPct=jankSum*100/frameSum))
((aveJanks=jankSum/iterations))
((aveFrames=frameSum/iterations))
((aveLatency90=latency90Sum/iterations))
((aveLatency95=latency95Sum/iterations))
((aveLatency99=latency99Sum/iterations))
echo AVE: Frames: $aveFrames latency: $aveLatency90/$aveLatency95/$aveLatency99 Janks: $aveJanks\(${aveJankPct}%\)
