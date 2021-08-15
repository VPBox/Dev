#!/bin/bash

RESULT=0

for i in "$@"; do
  a="`tail -c 1 "$i"`"
  if [ "$a" != "" ]; then
    echo "$i does not have a trailing newline" 1>&2
    RESULT=-1
  fi
done

exit $RESULT
