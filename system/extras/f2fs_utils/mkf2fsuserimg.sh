#!/bin/bash
#
# To call this script, make sure make_f2fs is somewhere in PATH

function usage() {
cat<<EOT
Usage:
${0##*/} OUTPUT_FILE SIZE
         [-C FS_CONFIG] [-f SRC_DIR] [-D PRODUCT_OUT]
         [-s FILE_CONTEXTS] [-t MOUNT_POINT] [-T TIMESTAMP]
         [-L LABEL]
EOT
}

echo "in mkf2fsuserimg.sh PATH=$PATH"

MKFS_OPTS=""
SLOAD_OPTS=""

if [ $# -lt 2 ]; then
  usage
  exit 1
fi

OUTPUT_FILE=$1
SIZE=$2
shift; shift

if [[ "$1" == "-C" ]]; then
  SLOAD_OPTS+=" -C $2"
  shift; shift
fi
if [[ "$1" == "-f" ]]; then
  SLOAD_OPTS+=" -f $2"
  shift; shift
fi
if [[ "$1" == "-D" ]]; then
  SLOAD_OPTS+=" -p $2"
  shift; shift
fi
if [[ "$1" == "-s" ]]; then
  SLOAD_OPTS+=" -s $2"
  shift; shift
fi
if [[ "$1" == "-t" ]]; then
  MOUNT_POINT=$2
  shift; shift
fi

if [ -z $MOUNT_POINT ]; then
  echo "Mount point is required"
  exit 2
fi

if [[ ${MOUNT_POINT:0:1} != "/" ]]; then
  MOUNT_POINT="/"$MOUNT_POINT
fi

SLOAD_OPTS+=" -t $MOUNT_POINT"

if [[ "$1" == "-T" ]]; then
  SLOAD_OPTS+=" -T $2"
  shift; shift
fi

if [[ "$1" == "-L" ]]; then
  MKFS_OPTS+=" -l $2"
  shift; shift
fi

if [ -z $SIZE ]; then
  echo "Need size of filesystem"
  exit 2
fi

MAKE_F2FS_CMD="make_f2fs -S $SIZE -g android $MKFS_OPTS $OUTPUT_FILE"
echo $MAKE_F2FS_CMD
$MAKE_F2FS_CMD
if [ $? -ne 0 ]; then
  exit 4
fi

SLOAD_F2FS_CMD="sload_f2fs -S $SLOAD_OPTS $OUTPUT_FILE"
echo $SLOAD_F2FS_CMD
$SLOAD_F2FS_CMD
if [ $? -ne 0 ]; then
  rm -f $OUTPUT_FILE
  exit 4
fi
