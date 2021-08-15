#!/bin/bash
# This script invokes QAIC to generate the source files associated with the
# FastRPC IDL

if [ -z "$HEXAGON_SDK_ROOT" ]; then
    HEXAGON_SDK_ROOT="$HOME/Qualcomm/Hexagon_SDK/3.0"
fi

QAIC_DIR="$HEXAGON_SDK_ROOT/tools/qaic/Linux"
OUT_DIR="../generated"

mkdir -p $OUT_DIR
"$QAIC_DIR"/qaic -mdll -o $OUT_DIR -I. chre_slpi.idl
