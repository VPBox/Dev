#!/bin/bash

filename=$1
cleanFilename=$2

regexN='^N[0-9]'
regexData=':'
regexNum='^[1-7]\.'

sum=0
count=0
InitializeSum=0
UnpackInputImageSum=0
MarkLayerMaskSum=0
ComputeLayerMatteBehindFocalDepthSum=0
ComputeIntegralImageForLayerBehindFocalDepthSum=0
FilterLayerBehindFocalDepthSum=0
updateSharpImageUsingFuzzyImageSum=0
ComputeLayerMatteInFrontOfFocalDepthSum=0
FilterLayerInFrontOfFocalDepthSum=0
finalizeFuzzyImageUsingSharpImageSum=0
PackOutputImageSum=0

while read line;do
  if [[ $line =~ $regexN ]];
  then
		if (( count > 0 ));then
			rNine=$((InitializeSum/count))
	  	echo "Initialize: $rNine" >> $cleanFilename
			InitializeSum=0
			rTen=$((UnpackInputImageSum/count))
	  	echo "UnpackInputImage: $rTen" >> $cleanFilename
			UnpackInputImageSum=0
			rZero=$((MarkLayerMaskSum/count))
	  	echo "MarkLayerMask: $rZero" >> $cleanFilename
			MarkLayerMaskSum=0
			rOne=$((ComputeLayerMatteBehindFocalDepthSum/count))
	  	echo "ComputeLayerMatteBehindFocalDepth: $rOne" >> $cleanFilename
			ComputeLayerMatteBehindFocalDepthSum=0
			rTwo=$((ComputeIntegralImageForLayerBehindFocalDepthSum/count))
	  	echo "ComputeIntegralImageForLayerBehindFocalDepth: $rTwo" >> $cleanFilename
			ComputeIntegralImageForLayerBehindFocalDepthSum=0
			rThree=$((FilterLayerBehindFocalDepthSum/count))
	  	echo "FilterLayerBehindFocalDepth: $rThree" >> $cleanFilename
			FilterLayerBehindFocalDepthSum=0
			rFour=$((updateSharpImageUsingFuzzyImageSum/count))
	  	echo "updateSharpImageUsingFuzzyImage: $rFour" >> $cleanFilename
			updateSharpImageUsingFuzzyImageSum=0
			rFive=$((ComputeLayerMatteInFrontOfFocalDepthSum/count))
	 		echo "ComputeLayerMatteInFrontOfFocalDepth: $rFive" >> $cleanFilename
			ComputeLayerMatteInFrontOfFocalDepthSum=0
			rEight=$((ComputeIntegralImageForLayerInFrontOfFocalDepthSum/count))
	  	echo "ComputeIntegralImageForLayerInFrontOfFocalDepth: $rEight" >> $cleanFilename
			ComputeIntegralImageForLayerInFrontOfFocalDepthSum=0
			rSix=$((FilterLayerInFrontOfFocalDepthSum/count))
	  	echo "FilterLayerInFrontOfFocalDepth: $rSix" >> $cleanFilename
			FilterLayerInFrontOfFocalDepthSum=0
			rSeven=$((finalizeFuzzyImageUsingSharpImageSum/count))
	  	echo "finalizeFuzzyImageUsingSharpImage: $rSeven" >> $cleanFilename
			finalizeFuzzyImageUsingSharpImageSum=0
			rEight=$((PackOutputImageSum/count))
	  	echo "PackOutputImage: $rEight" >> $cleanFilename
			PackOutputImageSum=0
    	count=0
		fi
		echo $line >> $cleanFilename
		
	elif [[ $line =~ $regexData ]];
	then
		#collect corresponding data
		newLine=`echo $line|awk '{print $2}'`
		sum=`echo $((sum+newLine))`
		if [[ $line =~ 'Initialize' ]];
		then
			InitializeSum=`echo $((InitializeSum+newLine))`
		elif [[ $line =~ 'UnpackInputImage' ]];
		then
			UnpackInputImageSum=`echo $((UnpackInputImageSum+newLine))`
		elif [[ $line =~ 'MarkLayerMask' ]];
		then
			MarkLayerMaskSum=`echo $((MarkLayerMaskSum+newLine))`
		elif [[ $line =~ 'ComputeLayerMatteBehindFocalDepth' ]];
		then
			ComputeLayerMatteBehindFocalDepthSum=`echo $((ComputeLayerMatteBehindFocalDepthSum+newLine))`
		elif [[ $line =~ 'ComputeIntegralImageForLayerBehindFocalDepth' ]];
		then
			ComputeIntegralImageForLayerBehindFocalDepthSum=`echo $((ComputeIntegralImageForLayerBehindFocalDepthSum+newLine))`
		elif [[ $line =~ 'FilterLayerBehindFocalDepth' ]];
		then
			FilterLayerBehindFocalDepthSum=`echo $((FilterLayerBehindFocalDepthSum+newLine))`
		elif [[ $line =~ [uU]pdateSharpImageUsingFuzzyImage ]];
		then
			updateSharpImageUsingFuzzyImageSum=`echo $((updateSharpImageUsingFuzzyImageSum+newLine))`
		elif [[ $line =~ 'ComputeLayerMatteInFrontOfFocalDepth' ]];
		then
			ComputeLayerMatteInFrontOfFocalDepthSum=`echo $((ComputeLayerMatteInFrontOfFocalDepthSum+newLine))`
		elif [[ $line =~ 'ComputeIntegralImageForLayerInFrontOfFocalDepth' ]];
		then
			ComputeIntegralImageForLayerInFrontOfFocalDepthSum=`echo $((ComputeIntegralImageForLayerInFrontOfFocalDepthSum+newLine))`
		elif [[ $line =~ 'FilterLayerInFrontOfFocalDepth' ]];
		then
			FilterLayerInFrontOfFocalDepthSum=`echo $((FilterLayerInFrontOfFocalDepthSum+newLine))`
		elif [[ $line =~ [fF]inalizeFuzzyImageUsingSharpImage ]];
		then
			finalizeFuzzyImageUsingSharpImageSum=`echo $((finalizeFuzzyImageUsingSharpImageSum+newLine))`
		elif [[ $line =~ 'PackOutputImage' ]];
		then
			PackOutputImageSum=`echo $((PackOutputImageSum+newLine))`
		fi

	elif [[ $line =~ $regexNum ]];
	then
		count=$((count+1))
	fi
done < <(tr -d '\r' < $filename)

echo $filename
