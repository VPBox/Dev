#!/bin/bash
#
# Copyright (C) 2013 The Android Open Source Project
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

# A test script for paycheck.py and the update_payload.py library.
#
# This script requires three payload files, along with a metadata signature for
# each, and a public key for verifying signatures. Payload include:
#
# - A full payload for release X (old_full_payload)
#
# - A full payload for release Y (new_full_payload), where Y > X
#
# - A delta payload from X to Y (delta_payload)
#
# The test performs the following:
#
# - It verifies each payload against its metadata signature, also asserting the
#   payload type. Another artifact is a human-readable payload report, which
#   is output to stdout to be inspected by the user.
#
# - It applies old_full_payload to yield old kernel (old_kern.part) and rootfs
#   (old_root.part) partitions.
#
# - It applies delta_payload to old_{kern,root}.part to yield new kernel
#   (new_delta_kern.part) and rootfs (new_delta_root.part) partitions.
#
# - It applies new_full_payload to yield reference new kernel
#   (new_full_kern.part) and rootfs (new_full_root.part) partitions.
#
# - It compares new_{delta,full}_kern.part and new_{delta,full}_root.part to
#   ensure that they are binary identical.
#
# If all steps have completed successfully we know with high certainty that
# paycheck.py (and hence update_payload.py) correctly parses both full and delta
# payloads, and applies them to yield the expected result. Finally, each
# paycheck.py execution is timed.


# Stop on errors, unset variables.
set -e
set -u

# Temporary image files.
OLD_KERN_PART=old_kern.part
OLD_ROOT_PART=old_root.part
NEW_DELTA_KERN_PART=new_delta_kern.part
NEW_DELTA_ROOT_PART=new_delta_root.part
NEW_FULL_KERN_PART=new_full_kern.part
NEW_FULL_ROOT_PART=new_full_root.part
CROS_PARTS="kernel root"


log() {
  echo "$@" >&2
}

die() {
  log "$@"
  exit 1
}

usage_and_exit() {
  cat >&2 <<EOF
Usage: ${0##*/} old_full_payload delta_payload new_full_payload
EOF
  exit
}

check_payload() {
  payload_file=$1
  payload_type=$2

  time ${paycheck} -t ${payload_type} ${payload_file}
}

apply_full_payload() {
  payload_file=$1
  out_dst_kern_part="$2/$3"
  out_dst_root_part="$2/$4"

  time ${paycheck} ${payload_file} \
    --part_names ${CROS_PARTS} \
    --out_dst_part_paths ${out_dst_kern_part} ${out_dst_root_part}
}

apply_delta_payload() {
  payload_file=$1
  out_dst_kern_part="$2/$3"
  out_dst_root_part="$2/$4"
  dst_kern_part="$2/$5"
  dst_root_part="$2/$6"
  src_kern_part="$2/$7"
  src_root_part="$2/$8"

  time ${paycheck} ${payload_file} \
    --part_names ${CROS_PARTS} \
    --out_dst_part_paths ${out_dst_kern_part} ${out_dst_root_part} \
    --dst_part_paths ${dst_kern_part} ${dst_root_part} \
    --src_part_paths ${src_kern_part} ${src_root_part}
}

main() {
  # Read command-line arguments.
  if [ $# == 1 ] && [ "$1" == "-h" ]; then
    usage_and_exit
  elif [ $# != 3 ]; then
    die "Error: unexpected number of arguments"
  fi
  old_full_payload="$1"
  delta_payload="$2"
  new_full_payload="$3"

  # Find paycheck.py
  paycheck=${0%/*}/paycheck.py
  if [ -z "${paycheck}" ] || [ ! -x ${paycheck} ]; then
    die "cannot find ${paycheck} or file is not executable"
  fi

  # Check the payloads statically.
  log "Checking payloads..."
  check_payload "${old_full_payload}" full
  check_payload "${new_full_payload}" full
  check_payload "${delta_payload}" delta
  log "Done"

  # Apply full/delta payloads and verify results are identical.
  tmpdir="$(mktemp -d --tmpdir test_paycheck.XXXXXXXX)"
  log "Initiating application of payloads at $tmpdir"

  log "Applying old full payload..."
  apply_full_payload "${old_full_payload}" "${tmpdir}" "${OLD_KERN_PART}" \
    "${OLD_ROOT_PART}"
  log "Done"

  log "Applying new full payload..."
  apply_full_payload "${new_full_payload}" "${tmpdir}" "${NEW_FULL_KERN_PART}" \
    "${NEW_FULL_ROOT_PART}"
  log "Done"

  log "Applying delta payload to old partitions..."
  apply_delta_payload "${delta_payload}" "${tmpdir}" "${NEW_DELTA_KERN_PART}" \
    "${NEW_DELTA_ROOT_PART}" "${NEW_FULL_KERN_PART}" \
    "${NEW_FULL_ROOT_PART}" "${OLD_KERN_PART}" "${OLD_ROOT_PART}"
  log "Done"

  log "Comparing results of delta and new full updates..."
  diff "${tmpdir}/${NEW_FULL_KERN_PART}" "${tmpdir}/${NEW_DELTA_KERN_PART}"
  diff "${tmpdir}/${NEW_FULL_ROOT_PART}" "${tmpdir}/${NEW_DELTA_ROOT_PART}"
  log "Done"

  log "Cleaning up"
  rm -fr "${tmpdir}"
}

main "$@"
