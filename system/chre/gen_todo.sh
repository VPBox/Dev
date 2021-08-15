#!/bin/bash
#
# Generate a TODO with a unique hash and priority level to allow tracking.
#
# Usage: ./gen_todo.sh 2 "Implement this."
#
# Output: TODO(P2-a07e5416): Implement this.

# Quit if any command produces an error.
set -e

# Check the positional arguments, assign defaults or prompt the user.
if [ $# -lt 2 ];
then
  read -p "Priority (ex: 0, 1, 2 or 3):"
  if [ -z $REPLY ]
  then
    PRIORITY="?"
  else
    PRIORITY=$REPLY
  fi

  read -p "Description (ex: 'Implement this.'):"
  TODO_TEXT=$REPLY
else
  PRIORITY=$1
  TODO_TEXT=$2
fi

# Build the TODO string.
TIME=`date +%s.%N`
SHASUM=`echo $TIME | shasum`
TODO_ID=${SHASUM:0:6}
TODO_STR="TODO(P$PRIORITY-$TODO_ID): $TODO_TEXT"
echo $TODO_STR
