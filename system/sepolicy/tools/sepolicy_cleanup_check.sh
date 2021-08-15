#!/bin/bash

# This script uses some heuristics to suggest potential ways to clean up SELinux policy.
# As these are heuristics, not everything it outputs is an error.
# It is better to run this on device-specific policy rather than core policy.
# It requires a device connected to adb.
# Usage:
#   ./sepolicy_cleanup_check.sh <sepolicy source path> [serial]

if [[ $# -lt 1 ]]; then
  echo "Usage: $0 <sepolicy source path> [serial]"
  exit
fi

sedir=$1
shift

adb_cmd="adb"
if [[ $# -eq 1 ]]; then
  adb_cmd="$adb_cmd -s $1"
  shift
fi

$adb_cmd shell id &>/dev/null
if [[ $? -ne 0 ]]; then
  echo "Please plug in a device and/or specify a serial"
  adb devices
  exit
fi

echo "Warning: this file uses heuristics, so all of its outputs are not necessarily errors."
echo "For example, when run on core policy, it will likely find many things that do not exist on a given device but might exist on others."

echo
echo "Scanning for labels that are not assigned to any files."
# Find all types.
grep -r "^type " --exclude=\*.go $sedir --exclude=\*_macros | sed 's/^.*:.*type \([^,]*\)*.*$/\1/' | sort | uniq | while read -r type; do
  # Find types that are not referenced in *_contexts.
  if [[ `find $sedir -name "*_contexts" -not -path "*prebuilts*" -exec grep $type '{}' \; |wc -l` -eq 0 ]]; then
    echo "None for $type"
    grep -r $type --exclude-dir=prebuilts --exclude=\*.cil $sedir
  fi
done

echo
echo "Scanning for executables that don't exist."
# Find executable types.
grep -r "^type .*exec_type" --exclude=\*.go $sedir | sed 's/^.*:.*type \([^,]*\)*.*$/\1/' | sort | uniq | while read -r type; do
  path_line=`grep -r $type --include=\*_contexts $sedir`
  # Note that this only examines one entry, even if multiple executables have the same label.
  # But the file_contexts scan below covers that case.
  path=`echo $path_line | sed 's/^.*:[^\/]*\([^ ]*\) .*$/\1/'`
  # Replace character classes and + with *.
  path=`echo $path | sed 's/\[[^]]*\]/*/' | sed 's/+/*/'`
  # Check whether the file exists.
  if [ -n "`$adb_cmd shell ls -lZ $path < /dev/null |& grep "No such file or directory"`" ]; then
    echo "$path does not exist"
  fi
done

echo
echo "Scanning genfs_contexts for files that don't exist."
# Find files in genfs_contexts.
find $sedir -name genfs_contexts -exec grep "^genfscon " '{}' \; | cut -d' ' -f2,3 | sort | uniq | while read -r file_line; do
  # Extract the full path.
  path=`echo $file_line | sed 's/rootfs //' | sed 's/sysfs /\/sys/' | sed 's/proc /\/proc/' | sed 's/debugfs /\/sys\/kernel\/debug/' | sed 's/tracefs /\/sys\/kernel\/debug\/tracing/'`
  # Skip things whose prefix we don't recognize.
  if [[ $path = *" "* ]]; then
    continue
  fi
  # Check whether the file exists.
  if [ -n "`$adb_cmd shell ls -lZ $path < /dev/null |& grep "No such file or directory"`" ]; then
    echo "$path does not exist"
  fi
done

echo
echo "Scanning file_contexts for files that don't exist."
# Find files in file_contexts.
find $sedir -name file_contexts -not -path "*prebuilts*" -exec grep "^/" '{}' \; | cut -d' ' -f1 | cut -f1 | sort | uniq | while read -r path; do
  # Replace (/.*)? with *
  # Replace (64)? with ??
  # Replace (vendor|system/vendor) with /vendor
  # Replace character classes and + with *.
  # Replace captures.
  # Replace \. with .
  # Replace .* with *
  # Replace ** with *
  path=`echo "$path" | sed 's/(\/\.\*)?$//' | sed 's/(64)?/??/' | sed 's/\(vendor|system\/vendor\)/vendor/' | sed 's/\[[^]]*\]/*/' | sed 's/+/*/' | sed 's/(\([^)]*\))/\1/' | sed 's/\\\././g' | sed 's/\.\*/\*/g' | sed 's/\*\*/\*/g'`
  # Check whether the file exists.
  if [ -n "`$adb_cmd shell ls -lZ "$path" < /dev/null |& grep "No such file or directory"`" ]; then
    echo "$path does not exist"
  fi
done

echo
echo "Scanning for rules that are defined in the wrong file."
echo "That is, rules that do not contain the name of the file."
# Find .te files.
find $sedir -name "*.te" -not -path "*prebuilts*" | while read -r file; do
  filename=`basename $file`
  filename="${filename%.*}"
  # Look for lines that don't have the filename in them.
  lines=$(grep "^[^# }']" $file | grep -v $filename | grep -v "^userdebug_or_eng(\`$" | grep -v "^type " | grep "[,)]" | grep -v "^define(")
  if [[ -n "$lines" ]]; then
    echo "$file:"
    echo "$lines"
  fi
done

echo
echo "Scanning for labels in file_contexts that do not escape '.' properly."
find $sedir -name file_contexts -not -path "*prebuilts*" -exec grep -H "^[^#].*[^\\]\.[^*]" '{}' \;

echo
echo "Scanning for rules that use the wrong file/dir macros."
grep -r ":file.*_dir_perms" --exclude=\*_macros $sedir
grep -r ":dir.*_file_perms" --exclude=\*_macros $sedir
