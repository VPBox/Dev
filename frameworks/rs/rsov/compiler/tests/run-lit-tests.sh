#!/bin/bash -e

current_dir=$(pwd)
export PATH=$current_dir:$PATH
LIT_PATH=$current_dir/llvm-lit
LIBSPIRV_TESTS=$current_dir

for arg in $@ ; do
  if [[ -a "$arg" ]] ; then
    # Looks like the user specified one or more files,
    # so don't implicitly specify the current directory.
    LIBSPIRV_TESTS=
    break
  fi
done

$LIT_PATH $LIBSPIRV_TESTS $@
