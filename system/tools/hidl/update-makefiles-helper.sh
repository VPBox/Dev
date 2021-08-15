#!/bin/bash

#TODO(b/35570956): Do with Soong instead.

#Note: see do_makefiles_update below.

function package_root_to_package() {
  echo $1 | cut -f1 -d:
}

function package_root_to_root() {
  echo $1 | cut -f2 -d:
}

##
# Makes sure the appropriate directories are visible.
# Usage: check_dirs root_or_cwd [package:root ...]
function check_dirs() {
  local root_or_cwd=$1
  shift 1

  for package_root in "$@"; do
    dir=$(package_root_to_root $package_root)
    if [ ! -d $root_or_cwd$dir ] ; then
      echo "Where is $dir?";
      return 1;
    fi
  done
}

##
# Gets all packages in a directory.
# Usage: get_packages package root
function get_packages() {
  local current_dir=$1
  local current_package=$2
  pushd $current_dir > /dev/null;
  find . -type f -name \*.hal -exec dirname {} \; | sort -u | \
    cut -c3- | \
    awk -F'/' \
    '{printf("'$current_package'"); for(i=1;i<NF;i++){printf(".%s", $i);}; printf("@%s\n", $NF);}';
  popd > /dev/null;
}

##
# Package roots to arguments.
# Usage: get_root_arguments [package:root ...]
function get_root_arguments() {
  for package_root in "$@"; do
    printf "%s" "-r$package_root "
  done
}

##
# Returns directory path for a package
# Usage: get_package_dir package_root_dir package_prefix package
function get_package_dir() {
  local package_dir=`echo $3 | cut -f1 -d@ | sed "s/$2\.//" | sed "s/\./\//g"`
  local package_version=`echo $3 | cut -f2 -d@`
  echo $1/$package_dir/$package_version
}

##
# Returns the number of processors to run on, on this machine
function get_num_processors() {
  if command -v nproc >/dev/null 2>&1; then
    PROCS=$(nproc --all 2>/dev/null) && echo $PROCS && return 0
  elif command -v sysctl >/dev/null 2>&1; then
    PROCS=$(sysctl -n hw.logicalcpu 2>/dev/null) && echo $PROCS && return 0
  fi

  echo 1
}

##
# Helps manage the package root of a HAL directory.
# Should be called from the android root directory.
#
# Usage: do_makefiles_update [-O owner-name] [package:root ...]
# Where the first package root is the current one.
#
function do_makefiles_update() {
  if ! command -v hidl-gen 1>/dev/null; then
    echo "Cannot find hidl-gen, try lunching or making it ('m hidl-gen')?"
    exit 1
  fi

  local owner=
  if [[ "$1" = "-O" ]]; then
    owner="$2"
    shift 2
  fi

  local root_or_cwd=${ANDROID_BUILD_TOP%%/}${ANDROID_BUILD_TOP:+/}

  local current_package=$(package_root_to_package $1)
  local current_dir=$root_or_cwd$(package_root_to_root $1)

  echo "Updating makefiles for $current_package in $current_dir."

  check_dirs "$root_or_cwd" $@ || return 1

  local packages=$(get_packages $current_dir $current_package) || return 1
  local root_arguments=$(get_root_arguments $@) || return 1

  function __update_internal() {
    local owner="$1"
    local root_arguments="$2"
    local package="$3"
    echo "Updating $package"
    hidl-gen -O "$owner" -Landroidbp $root_arguments $package || {
      echo "Command failed: hidl-gen -O \"$owner\" -Landroidbp $root_arguments $package";
      return 1;
    }
  }
  export -f __update_internal

  echo "$packages" |\
      xargs -P $(get_num_processors) -I {} \
      bash -c "__update_internal \"$owner\" \"$root_arguments\" \"{}\"" || return 1
}
