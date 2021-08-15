#!/bin/bash

# Generate the CHRE-side header file
flatc --cpp -o ../include/chre/platform/shared/ --scoped-enums \
  --cpp-ptr-type chre::UniquePtr host_messages.fbs

# Generate the AP-side header file with some extra goodies
flatc --cpp -o ../../../host/common/include/chre_host/ --scoped-enums \
  --gen-mutable --gen-object-api host_messages.fbs
