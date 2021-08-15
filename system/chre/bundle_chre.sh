#!/bin/bash
#
# Generates a bundle of this project to share with partners. It produces a file
# with the name chre-$HEAD_HASH.bundle where $HEAD_HASH is the current ToT
# commit hash.
#
# Usage:
#     ./bundle_chre.sh

BRANCH=`git rev-parse --abbrev-ref --symbolic-full-name @{u}`
HEAD_HASH=`git describe --always --long $BRANCH`
FILENAME=chre-$HEAD_HASH.bundle
git bundle create $FILENAME $BRANCH

echo
echo "Created bundle for $BRANCH as $FILENAME"
