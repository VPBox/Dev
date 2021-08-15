#
# Script to play a john oliver youtube video N times.
# For each iteration, Total frames and janky frames are reported.
#
# Options are described below.
#
iterations=10
app=youtube
searchText="last week tonight with john oliver: online harassment"
vidMinutes=15

function processLocalOption {
	ret=0
	case "$1" in
	(-S) searchText="$2"; shift;;
	(-t) vidMinutes="$2"; shift;;
	(*)
		echo "$0: unrecognized option: $1"
		echo; echo "Usage: $0 [options]"
		echo "-i iterations"
		echo "-S youtube search text"
		echo "-d device"
		echo "-t vidMinutes"
		exit 1;;
	esac
	return $ret
}

CMDDIR=$(dirname $0 2>/dev/null)
CMDDIR=${CMDDIR:=.}
. $CMDDIR/defs.sh

case $DEVICE in
(angler)
	searchButton="860 177"
	selectFirstVideo="225 400"
	enableControls="1000 610"
	fullScreen="1011 632"
	;;
(shamu)
	searchButton="1200 160"
	selectFirstVideo="480 653"
	enableControls="1377 812"
	fullScreen="1377 812"
	;;
(bullhead|hammerhead)
	searchButton="860 177"
	selectFirstVideo="225 400"
	enableControls="1000 610"
	fullScreen="1011 632"
	;;
(volantis)
	searchButton="1356 93"
	selectFirstVideo="378 264"
	enableControls="1464 812"
	fullScreen="1480 835"
	;;
(ariel)
	searchButton="1440 70"
	selectFirstVideo="228 224"
	enableControls="1528 880"
	fullScreen="1528 880"
	;;

(*)
	echo "Error: No display information available for $DEVICE"
	exit 1;;
esac

function swipe {
	count=0
	while [ $count -lt $2 ]
	do
		echo doSwipe...
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
	t=$(startActivity $app)
	sleep 4.0
	doTap $searchButton
	sleep 1.0
	doText "$searchText"
	sleep 1.0
	doKeyevent ENTER
	sleep 5.0
	doTap $selectFirstVideo
	sleep 10.0
	doTap $fullScreen
	sleep 0.5
	doTap $fullScreen
	# 15 minutes
	((vidTime=60*vidMinutes))
	sleep $vidTime
	doKeyevent BACK
	sleep 0.5
	doKeyevent BACK
	sleep 0.5
	doKeyevent BACK
	sleep 0.5

	set -- $(getJankyFrames $(getPackageName $app))
	totalDiff=$1
	jankyDiff=$2
	latency90=$3
	latency95=$4
	latency99=$5
	if [ ${totalDiff:=0} -eq 0 ]; then
		echo Error: could not read frame info with \"dumpsys gfxinfo\"
	fi

	((frameSum=frameSum+totalDiff))
	((jankSum=jankSum+jankyDiff))
	((latency90Sum=latency90Sum+latency90))
	((latency95Sum=latency95Sum+latency95))
	((latency99Sum=latency99Sum+latency99))
	if [ "$totalDiff" -eq 0 ]; then
		echo Error: no frames detected. Is the display off?
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
