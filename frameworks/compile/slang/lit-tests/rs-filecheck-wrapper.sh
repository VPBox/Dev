#!/bin/bash -e

# RS Invocation script to FileCheck
# Usage: rs-filecheck-wrapper.sh <output-directory> <path-to-FileCheck> <source> [<more-args>]

OUTDIR=$1
FILECHECK=$2
SOURCEFILE=$3
shift 3

FILECHECK_INPUTFILE=`basename $SOURCEFILE | sed 's/\.rs\$/.ll/'`

# This runs FileCheck on both the 32 bit and the 64 bit bitcode files.
$FILECHECK -input-file $OUTDIR/bc32/$FILECHECK_INPUTFILE $SOURCEFILE $@
$FILECHECK -input-file $OUTDIR/bc64/$FILECHECK_INPUTFILE $SOURCEFILE $@
