#!/bin/bash
# Top-level driver for slicing a given model.
# Usage: slicing.sh <number of operations> <model>
# The sliced model would be <model>_sliced_<number of operations>.mod.py
#
# Note this tool has the following side effects:
# * updates ../generated/all_generated_tests.cpp by running
#   ./generate_test.sh
# * runs adb sync for producing reference outputs
# * runs adb remount so reference outputs could be saved by the test harness

if [[ $# -ne 2 || "$1" -eq "-h" || !(-e $2) || $1 -lt 1 ]]; then
  echo "Usage: $0 <number of operations> <model>"
  echo "The sliced model would be <model>_sliced_<number of operations>.mod.py"
  echo "<number of operations> has to be >= 1"
  echo
  echo "Note this tool has the following side effects:"
  echo "* runs adb remount and adb push/pull to /data/local/tmp"
  echo "* alters ../generated/all_generated_tests.cpp"
  echo
  exit 1
fi

set -eu

NO_OF_OPERATIONS=$1
INPUT=$2
SLICE=$ANDROID_BUILD_TOP/frameworks/ml/nn/tools/test_generator/slicing.py
DIR_AND_NAME=${INPUT/.mod.py/}
BASENAME=$(basename $DIR_AND_NAME)
MODEL_ONLY=${DIR_AND_NAME}_sliced.model_only.py
INPUT_ONLY=${DIR_AND_NAME}_sliced.input_only.py
REFERENCE=${DIR_AND_NAME}_sliced.ref.py
FINAL=${DIR_AND_NAME}_sliced_$1.mod.py
SAVED_OUTPUT_FILE=/data/local/tmp/current_nnapi_example.example.py

set +eu
source $ANDROID_BUILD_TOP/build/envsetup.sh > /dev/null
lunch $TARGET_PRODUCT
set -eu
source $ANDROID_BUILD_TOP/frameworks/ml/nn/runtime/test/specs/generate_test.sh

$SLICE $INPUT -n $NO_OF_OPERATIONS -m $MODEL_ONLY -e $INPUT_ONLY

# create a temporary spec from the model and the input-only example
FORCE = "-f"
echo "collecting_data = True" > ${DIR_AND_NAME}_tmp.mod.py
cat $MODEL_ONLY $INPUT_ONLY >> ${DIR_AND_NAME}_tmp.mod.py
generate_wrapper "log" $SAVED_OUTPUT_FILE ${DIR_AND_NAME}_tmp.mod.py

# execute the sliced testcase and collect reference outputs
TMP_EXEC=$(adb shell mktemp --tmpdir /data/local/tmp)
HOST_EXEC_DIR=$ANDROID_PRODUCT_OUT/data/nativetest64/NeuralNetworksTest_static/
if [[ ! -d $HOST_EXEC_DIR ]]; then
  HOST_EXEC_DIR=$ANDROID_PRODUCT_OUT/data/nativetest/NeuralNetworksTest_static/
fi
[[ -d $HOST_EXEC_DIR ]] || (echo "cannot find $HOST_EXEC_DIR"; exit 1)

set +u
adb remount
mm -j40
adb push ${HOST_EXEC_DIR}/NeuralNetworksTest_static $TMP_EXEC
adb shell $TMP_EXEC --gtest_filter="*.${BASENAME}_tmp"
adb pull $SAVED_OUTPUT_FILE $REFERENCE
set -u
GENERATED=$ANDROID_BUILD_TOP/frameworks/ml/nn/runtime/test/generated/

# remove temporary spec and corresponding generated files
rm ${DIR_AND_NAME}_tmp.mod.py
rm ${GENERATED}/models/${BASENAME}_tmp.model.cpp
rm ${GENERATED}/examples/${BASENAME}_tmp.example.cpp

if [ $? -ne 0 ]; then
  echo Error: Failed building intermediate model for $2
  exit $?
fi

echo "collecting_data = False" > ${FINAL}
cat $MODEL_ONLY $INPUT_ONLY $REFERENCE |sed s/Ignored// \
  >> ${FINAL}
echo "Example((input0, output0))" >> ${FINAL}
rm -f $MODEL_ONLY $INPUT_ONLY $REFERENCE
adb shell rm $TMP_EXEC
adb shell rm -f $SAVED_OUTPUT_FILE
# Regnerate the tests
#./generate_test.sh
echo
echo Sliced model is at $FINAL
