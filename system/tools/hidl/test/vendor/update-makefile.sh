#!/bin/bash

options="-O some-owner-name
         -r hidl.tests:system/tools/hidl/test/
         -r android.hidl:system/libhidl/transport
         -r android.hardware:hardware/interfaces"

hidl-gen -Landroidbp $options hidl.tests.vendor@1.0;
hidl-gen -Landroidbp $options hidl.tests.vendor@1.1;
