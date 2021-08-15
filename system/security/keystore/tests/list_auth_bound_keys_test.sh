#!/usr/bin/env bash

#
# Copyright (C) 2018 The Android Open Source Project
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
#
# Simple adb based test for keystore method list_auth_bound_keys
# Depends on keystore_cli_v2 tool and root
#

set -e

ROOT_ID=0
USER1_ID=10901
USER2_ID=10902
SYSTEM_ID=1000

function cli {
	adb shell "su $1 keystore_cli_v2 $2"
}

#start as root
adb root

# generate keys as user
echo "generating keys"
cli $USER1_ID "delete --name=no_auth_key" || true
cli $USER1_ID "generate --name=no_auth_key"
cli $USER2_ID "delete --name=auth_key" || true
if ! cli $USER2_ID "generate --name=auth_key --auth_bound"; then
	echo "Unable to generate auth bound key, make sure device/emulator has a pin/password set."
	echo "$ adb shell locksettings set-pin 1234"
	exit 1
fi

# try to list keys as user
if cli $USER2_ID list-apps-with-keys; then
	echo "Error: list-apps-with-keys succeeded as user, this is not expected!"
	exit 1
fi

# try to list keys as root
if cli $ROOT_ID "list-apps-with-keys"; then
	echo "Error: list-apps-with-keys succeeded as root, this is not expected!"
	exit 1
fi

# try to list keys as system
success=false
while read -r line; do
	echo $line
    if [ "$line" == "$USER2_ID" ]; then
    	success=true
    fi
    if [ "$line" == "$USER1_ID" ]; then
    	echo "Error: User1 id not expected in list"
    	exit 1
    fi
done <<< $(cli $SYSTEM_ID "list-apps-with-keys")
if [ $success = true ]; then
	echo "Success!"
else
	echo "Error: User2 id not in list"
	exit 1
fi