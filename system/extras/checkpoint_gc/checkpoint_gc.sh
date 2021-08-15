#!/system/bin/sh

#
# Copyright (C) 2019 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# This script will run as an pre-checkpointing cleanup for mounting f2fs
# with checkpoint=disable, so that the first mount after the reboot will
# be faster. It is unnecessary to run if the device does not use userdata
# checkpointing on F2FS.

# TARGET_SLOT="${1}"
STATUS_FD="${2}"
DIRTY_SEGMENTS_THRESHOLD=100

SLEEP=5
TIME=0
MAX_TIME=3600

NAME=`while read dev dir type opt; do
  if [ /data = ${dir} -a f2fs = ${type} ]; then
    echo ${dev##*/}
    break
  fi
done < /proc/mounts`
if [ -z "${NAME}" ]; then
  exit 0
fi
log -pi -t checkpoint_gc Turning on GC for ${NAME}
read OLD_SLEEP < /sys/fs/f2fs/${NAME}/gc_urgent_sleep_time || exit 1
echo 50 > /sys/fs/f2fs/${NAME}/gc_urgent_sleep_time || exit 1
echo 1 > /sys/fs/f2fs/${NAME}/gc_urgent || exit 1

read DIRTY_SEGMENTS_START < /sys/fs/f2fs/${NAME}/dirty_segments
DIRTY_SEGMENTS=${DIRTY_SEGMENTS_START}
TODO_SEGMENTS=$((${DIRTY_SEGMENTS_START}-${DIRTY_SEGMENTS_THRESHOLD}))
while [ ${DIRTY_SEGMENTS} -gt ${DIRTY_SEGMENTS_THRESHOLD} ]; do
  log -pi -t checkpoint_gc dirty segments:${DIRTY_SEGMENTS} \(threshold:${DIRTY_SEGMENTS_THRESHOLD}\)
  PROGRESS=`echo "(${DIRTY_SEGMENTS_START}-${DIRTY_SEGMENTS})/${TODO_SEGMENTS}"|bc -l`
  if [[ $PROGRESS == -* ]]; then
      PROGRESS=0
  fi
  print -u${STATUS_FD} "global_progress ${PROGRESS}"
  read DIRTY_SEGMENTS < /sys/fs/f2fs/${NAME}/dirty_segments
  sleep ${SLEEP}
  TIME=$((${TIME}+${SLEEP}))
  if [ ${TIME} -gt ${MAX_TIME} ]; then
    break
  fi
  # In case someone turns it off behind our back
  echo 1 > /sys/fs/f2fs/${NAME}/gc_urgent
done

log -pi -t checkpoint_gc Turning off GC for ${NAME}
echo 0 > /sys/fs/f2fs/${NAME}/gc_urgent
echo ${OLD_SLEEP} > /sys/fs/f2fs/${NAME}/gc_urgent_sleep_time
sync

print -u${STATUS_FD} "global_progress 1.0"
exit 0
