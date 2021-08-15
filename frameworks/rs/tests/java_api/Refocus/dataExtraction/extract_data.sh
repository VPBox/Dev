#!/bin/bash

filename=$1
cleanFilename=$2

regexN='^N[0-9]'
regexData='(D/RefocusFilter)'
regexNum='^[1-7]\.'

sum=0
InitializeSum=0
UnpackInputImageSum=0
MarkLayerMaskSum=0
ComputeLayerMatteBehindFocalDepthSum=0
ComputeIntegralImageForLayerBehindFocalDepthSum=0
FilterLayerBehindFocalDepthSum=0
updateSharpImageUsingFuzzyImageSum=0
ComputeLayerMatteInFrontOfFocalDepthSum=0
ComputeIntegralImageForLayerInFrontOfFocalDepthSum=0
FilterLayerInFrontOfFocalDepthSum=0
finalizeFuzzyImageUsingSharpImageSum=0
PackOutputImageSum=0

while read line;do
  if [[ $line =~ $regexN ]];
  then
	  echo "Initialize: $InitializeSum" >> $cleanFilename
		InitializeSum=0
	  echo "UnpackInputImage: $UnpackInputImageSum" >> $cleanFilename
		UnpackInputImageSum=0
	  echo "MarkLayerMask: $MarkLayerMaskSum" >> $cleanFilename
		MarkLayerMaskSum=0
	  echo "ComputeLayerMatteBehindFocalDepth: $ComputeLayerMatteBehindFocalDepthSum" >> $cleanFilename
		ComputeLayerMatteBehindFocalDepthSum=0
	  echo "ComputeIntegralImageForLayerBehindFocalDepth: $ComputeIntegralImageForLayerBehindFocalDepthSum" >> $cleanFilename
		ComputeIntegralImageForLayerBehindFocalDepthSum=0
	  echo "FilterLayerBehindFocalDepth: $FilterLayerBehindFocalDepthSum" >> $cleanFilename
		FilterLayerBehindFocalDepthSum=0
	  echo "updateSharpImageUsingFuzzyImage: $updateSharpImageUsingFuzzyImageSum" >> $cleanFilename
		updateSharpImageUsingFuzzyImageSum=0
	  echo "ComputeLayerMatteInFrontOfFocalDepth: $ComputeLayerMatteInFrontOfFocalDepthSum" >> $cleanFilename
		ComputeLayerMatteInFrontOfFocalDepthSum=0
	  echo "ComputeIntegralImageForLayerInFrontOfFocalDepth: $ComputeIntegralImageForLayerInFrontOfFocalDepthSum" >> $cleanFilename
		ComputeIntegralImageForLayerInFrontOfFocalDepthSum=0
	  echo "FilterLayerInFrontOfFocalDepth: $FilterLayerInFrontOfFocalDepthSum" >> $cleanFilename
		FilterLayerInFrontOfFocalDepthSum=0
	  echo "finalizeFuzzyImageUsingSharpImage: $finalizeFuzzyImageUsingSharpImageSum" >> $cleanFilename
		finalizeFuzzyImageUsingSharpImageSum=0
	  echo "PackOutputImage: $PackOutputImageSum" >> $cleanFilename
		PackOutputImageSum=0
    echo $line >> $cleanFilename

	elif [[ $line =~ $regexData ]];
	then
		#collect corresponding data
		newLine=`echo $line|grep -oE '[0-9]+ ns'|awk '{print $1}'`
		sum=`echo $((sum+newLine))`
		if [[ $line =~ 'Initialize' ]];
		then
			InitializeSum=`echo $((InitializeSum+newLine))`;
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
		if ! [[ $line =~ '1.' ]];
		then
	    echo "Initialize: $InitializeSum" >> $cleanFilename
			InitializeSum=0
	    echo "UnpackInputImage: $UnpackInputImageSum" >> $cleanFilename
			UnpackInputImageSum=0
	    echo "MarkLayerMask: $MarkLayerMaskSum" >> $cleanFilename
			MarkLayerMaskSum=0
	    echo "ComputeLayerMatteBehindFocalDepth: $ComputeLayerMatteBehindFocalDepthSum" >> $cleanFilename
			ComputeLayerMatteBehindFocalDepthSum=0
	    echo "ComputeIntegralImageForLayerBehindFocalDepth: $ComputeIntegralImageForLayerBehindFocalDepthSum" >> $cleanFilename
			ComputeIntegralImageForLayerBehindFocalDepthSum=0
	    echo "FilterLayerBehindFocalDepth: $FilterLayerBehindFocalDepthSum" >> $cleanFilename
			FilterLayerBehindFocalDepthSum=0
	    echo "updateSharpImageUsingFuzzyImage: $updateSharpImageUsingFuzzyImageSum" >> $cleanFilename
			updateSharpImageUsingFuzzyImageSum=0
	    echo "ComputeLayerMatteInFrontOfFocalDepth: $ComputeLayerMatteInFrontOfFocalDepthSum" >> $cleanFilename
			ComputeLayerMatteInFrontOfFocalDepthSum=0
	    echo "ComputeIntegralImageForLayerInFrontOfFocalDepth: $ComputeIntegralImageForLayerInFrontOfFocalDepthSum" >> $cleanFilename
			ComputeIntegralImageForLayerInFrontOfFocalDepthSum=0
	    echo "FilterLayerInFrontOfFocalDepth: $FilterLayerInFrontOfFocalDepthSum" >> $cleanFilename
			FilterLayerInFrontOfFocalDepthSum=0
	    echo "finalizeFuzzyImageUsingSharpImage: $finalizeFuzzyImageUsingSharpImageSum" >> $cleanFilename
			finalizeFuzzyImageUsingSharpImageSum=0
	    echo "PackOutputImage: $PackOutputImageSum" >> $cleanFilename
			PackOutputImageSum=0
		fi
		echo $line >> $cleanFilename
	fi
done < $filename

echo $filename
