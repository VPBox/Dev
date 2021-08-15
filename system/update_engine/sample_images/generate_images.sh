#!/bin/bash

#
# Copyright (C) 2015 The Android Open Source Project
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

# This script generates some sample images used in unittests and packages them
# in the sample_images.tar.bz2 file. The list of generated images and their
# options are described in the main() function. You need to manually run this
# script to update the generated images whenever you modify this script.

set -e

# cleanup <path>
# Unmount and remove the mountpoint <path>
cleanup() {
  local path="$1"
  if ! sudo umount "${path}" 2>/dev/null; then
    if mountpoint -q "${path}"; then
      sync && sudo umount "${path}"
    fi
  fi
  if [ -n "${path}" ]; then
    sudo rm -rf "${path}"
  fi
}

# add_files_default <mntdir> <block_size>
# Add several test files to the image mounted in <mntdir>.
add_files_default() {
  local mntdir="$1"
  local block_size="$2"

  ### Generate the files used in unittest with descriptive names.
  sudo touch "${mntdir}"/empty-file

  # regular: Regular files.
  echo "small file" | sudo dd of="${mntdir}"/regular-small status=none
  dd if=/dev/zero bs=1024 count=16 status=none | tr '\0' '\141' |
    sudo dd of="${mntdir}"/regular-16k status=none
  sudo dd if=/dev/zero of="${mntdir}"/regular-32k-zeros bs=1024 count=16 \
    status=none

  echo "with net_cap" | sudo dd of="${mntdir}"/regular-with_net_cap status=none
  sudo setcap cap_net_raw=ep "${mntdir}"/regular-with_net_cap

  # sparse_empty: Files with no data blocks at all (only sparse holes).
  sudo truncate --size=10240 "${mntdir}"/sparse_empty-10k
  sudo truncate --size=$(( block_size * 2 )) "${mntdir}"/sparse_empty-2blocks

  # sparse: Files with some data blocks but also sparse holes.
  echo -n "foo" |
    sudo dd of="${mntdir}"/sparse-16k-last_block bs=1 \
      seek=$(( 16 * 1024 - 3)) status=none

  # ext2 inodes have 12 direct blocks, one indirect, one double indirect and
  # one triple indirect. 10000 should be enough to have an indirect and double
  # indirect block.
  echo -n "foo" |
    sudo dd of="${mntdir}"/sparse-10000blocks bs=1 \
      seek=$(( block_size * 10000 )) status=none

  sudo truncate --size=16384 "${mntdir}"/sparse-16k-first_block
  echo "first block" | sudo dd of="${mntdir}"/sparse-16k-first_block status=none

  sudo truncate --size=16384 "${mntdir}"/sparse-16k-holes
  echo "a" | sudo dd of="${mntdir}"/sparse-16k-holes bs=1 seek=100 status=none
  echo "b" | sudo dd of="${mntdir}"/sparse-16k-holes bs=1 seek=10000 status=none

  # link: symlinks and hardlinks.
  sudo ln -s "broken-link" "${mntdir}"/link-short_symlink
  sudo ln -s $(dd if=/dev/zero bs=256 count=1 status=none | tr '\0' '\141') \
    "${mntdir}"/link-long_symlink
  sudo ln "${mntdir}"/regular-16k "${mntdir}"/link-hard-regular-16k

  # Directories.
  sudo mkdir -p "${mntdir}"/dir1/dir2/dir1
  echo "foo" | sudo tee "${mntdir}"/dir1/dir2/file >/dev/null
  echo "bar" | sudo tee "${mntdir}"/dir1/file >/dev/null

  # FIFO
  sudo mkfifo "${mntdir}"/fifo

  # character special file
  sudo mknod "${mntdir}"/cdev c 2 3

  # removed: removed files that should not be listed.
  echo "We will remove this file so it's contents will be somewhere in the " \
    "empty space data but it won't be all zeros." |
    sudo dd of="${mntdir}"/removed conv=fsync status=none
  sudo rm "${mntdir}"/removed
}

# add_files_ue_settings <mntdir> <block_size>
# Add the update_engine.conf settings file. This file contains the
add_files_ue_settings() {
  local mntdir="$1"

  sudo mkdir -p "${mntdir}"/etc >/dev/null
  sudo tee "${mntdir}"/etc/update_engine.conf >/dev/null <<EOF
PAYLOAD_MINOR_VERSION=1234
EOF
  # Example of a real lsb-release file released on link stable.
  sudo tee "${mntdir}"/etc/lsb-release >/dev/null <<EOF
CHROMEOS_AUSERVER=https://tools.google.com/service/update2
CHROMEOS_BOARD_APPID={F26D159B-52A3-491A-AE25-B23670A66B32}
CHROMEOS_CANARY_APPID={90F229CE-83E2-4FAF-8479-E368A34938B1}
CHROMEOS_DEVSERVER=
CHROMEOS_RELEASE_APPID={F26D159B-52A3-491A-AE25-B23670A66B32}
CHROMEOS_RELEASE_BOARD=link-signed-mp-v4keys
CHROMEOS_RELEASE_BRANCH_NUMBER=63
CHROMEOS_RELEASE_BUILD_NUMBER=6946
CHROMEOS_RELEASE_BUILD_TYPE=Official Build
CHROMEOS_RELEASE_CHROME_MILESTONE=43
CHROMEOS_RELEASE_DESCRIPTION=6946.63.0 (Official Build) stable-channel link
CHROMEOS_RELEASE_NAME=Chrome OS
CHROMEOS_RELEASE_PATCH_NUMBER=0
CHROMEOS_RELEASE_TRACK=stable-channel
CHROMEOS_RELEASE_VERSION=6946.63.0
GOOGLE_RELEASE=6946.63.0
EOF
}

add_files_postinstall() {
  local mntdir="$1"

  sudo mkdir -p "${mntdir}"/bin >/dev/null

  # A postinstall bash program.
  sudo tee "${mntdir}"/bin/postinst_example >/dev/null <<EOF
#!/etc/../bin/sh
echo "I'm a postinstall program and I know how to write to stdout"
echo "My call was $@"
exit 0
EOF

  # A symlink to another program. This should also work.
  sudo ln -s "postinst_example" "${mntdir}"/bin/postinst_link

  sudo tee "${mntdir}"/bin/postinst_fail3 >/dev/null <<EOF
#!/etc/../bin/sh
exit 3
EOF

  sudo tee "${mntdir}"/bin/postinst_fail1 >/dev/null <<EOF
#!/etc/../bin/sh
exit 1
EOF

  # A program that succeeds if it is suspended during the first 5 minutes.
  sudo tee "${mntdir}"/bin/postinst_suspend >/dev/null <<EOF
#!/etc/../bin/sh
trap "{ echo Got a SIGCONT; exit 0; }" CONT
# Signal that we are ready to receive the signal by redirecting our stdin to
# /dev/zero, the test can detect that.
exec </dev/zero
# Allow the signal handler to run every 100 ms.
i=3000
while [ \$i -ge 0 ]; do
  sleep 0.1
  i=\$((i-1))
done
exit 1
EOF

  # A program that reports back progress.
  sudo tee "${mntdir}"/bin/postinst_progress >/dev/null <<EOF
#!/etc/../bin/sh
# These values have exact representation in IEEE 754 so we avoid rounding
# errors.
echo global_progress 0.25 >&3
echo global_progress 0.5 >&3
echo global_progress 1.0 >&3
exit 0
EOF

  # A postinstall bash program.
  sudo tee "${mntdir}"/bin/self_check_context >/dev/null <<EOF
#!/etc/../bin/sh
echo "This is my context:"
ls -lZ "\$0" | grep -F ' u:object_r:postinstall_file:s0 ' || exit 5
exit 0
EOF

  sudo tee "${mntdir}"/postinst >/dev/null <<EOF
#!/etc/../bin/sh
echo "postinst"
exit 0
EOF

  sudo chmod +x "${mntdir}"/postinst "${mntdir}"/bin/*
}

# generate_fs <filename> <kind> <size> [block_size] [block_groups]
generate_fs() {
  local filename="$1"
  local type="$2"
  local kind="$3"
  local size="$4"
  local block_size="${5:-4096}"
  local block_groups="${6:-}"

  local mkfs_opts=( -q -F -b "${block_size}" -L "ROOT-TEST" -t ext2 )
  if [[ -n "${block_groups}" ]]; then
    mkfs_opts+=( -G "${block_groups}" )
  fi

  local mntdir=$(mktemp --tmpdir -d generate_ext2.XXXXXX)
  trap 'cleanup "${mntdir}"; rm -f "${filename}"' INT TERM EXIT
  # Cleanup old image.
  if [[ -e "${filename}" ]]; then
    rm -f "${filename}"
  fi

  if [[ "${type}" == "ext2" ]]; then
    truncate --size="${size}" "${filename}"

    mkfs.ext2 "${mkfs_opts[@]}" "${filename}"
    sudo mount "${filename}" "${mntdir}" -o loop
  fi
  case "${kind}" in
    unittest)
      add_files_ue_settings "${mntdir}" "${block_size}"
      add_files_postinstall "${mntdir}" "${block_size}"
      ;;
    default)
      add_files_default "${mntdir}" "${block_size}"
      ;;
    empty)
      ;;
  esac

  if [[ "${type}" == "sqfs" ]]; then
    mksquashfs "${mntdir}" "${filename}"
  fi

  cleanup "${mntdir}"
  trap - INT TERM EXIT
}

OUTPUT_DIR=$(dirname "$0")
IMAGES=()

# generate_image <image_name> [<image args> ...]
generate_image() {
  echo "Generating image $1.img"
  IMAGES+=( "$1.img" )
  generate_fs "${OUTPUT_DIR}/$1.img" "${@:2}"
}

main() {
  # Add more sample images here.
  generate_image disk_ext2_1k ext2 default $((1024 * 1024)) 1024
  generate_image disk_ext2_4k ext2 default $((1024 * 4096)) 4096
  generate_image disk_ext2_4k_empty ext2 empty $((1024 * 4096)) 4096
  generate_image disk_ext2_unittest ext2 unittest $((1024 * 4096)) 4096

  # Add squashfs sample images.
  generate_image disk_sqfs_empty sqfs empty $((1024 * 4096)) 4096
  generate_image disk_sqfs_default sqfs default $((1024 * 4096)) 4096

  # Generate the tarball and delete temporary images.
  echo "Packing tar file sample_images.tar.bz2"
  tar -jcf "${OUTPUT_DIR}/sample_images.tar.bz2" -C "${OUTPUT_DIR}" \
    --sparse "${IMAGES[@]}"
  cd "${OUTPUT_DIR}"
  rm "${IMAGES[@]}"
}

main
