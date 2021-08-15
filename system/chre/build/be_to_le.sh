#!/bin/bash
#
# Converts a big-endian hex string to a little-endian hex string.
#
# Examples:
#
# ./be_to_le.sh 0x12345678
# 0x78563412
#
# ./be_to_le.sh 12345678
# 0x78563412

BE_VALUE=$1

# If the input starts with 0x, strip it off.
if [[ $BE_VALUE =~ ^0x.* ]];
then
BE_VALUE=${BE_VALUE:2}
fi

echo 0x`echo -n $BE_VALUE | tac -rs ..`
