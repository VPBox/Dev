#!/bin/bash

packages=(
    android.hidl.allocator@1.0
    android.hidl.base@1.0
    android.hidl.manager@1.0
    android.hidl.manager@1.1
    android.hidl.manager@1.2
    android.hidl.memory@1.0
    android.hidl.safe_union@1.0
    android.hidl.token@1.0
)

for package in "${packages[@]}"; do
    echo "Updating $package."
    hidl-gen -Landroidbp -r android.hidl:system/libhidl/transport $package
done
