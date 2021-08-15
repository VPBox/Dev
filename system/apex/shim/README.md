# Cts Shim Apex

## Overview

A shim apex is an apex that doesn't provide any functionality as it's sole
purpose is to enable CTS testing of APEX-related APIs.

A cts shim apex has following restrictions:

*   Its name is equal to `com.android.apex.cts.shim`
*   It is signed with a throw-away key.
*   First version (i.e. `com.android.apex.cts.shim.apex`) should be
    pre-installed on the /system/partition.
*   `apex_payload.img` of the pre-installed shim apexcontains a single text
    file called `hash.txt` with a whitelist of SHA512 hashes of versions shim
    apex can be upgraded to.
*   `apex_payload.img` of all other versions contains SHA512 hash of
    `/dev/null`.


## Building shim apexes

Modules to build shim apexes are defined in the
system/apex/shim/build/Android.bp blueprint file.

To build shim apexes simply run

```
m com.android.apex.cts.shim.v3 \
  com.android.apex.cts.shim.v2 \
  com.android.apex.cts.shim.v2_additional_file \
  com.android.apex.cts.shim.v2_additional_folder \
  com.android.apex.cts.shim.v2_with_pre_install_hook \
  com.android.apex.cts.shim.v2_with_post_install_hook \
  com.android.apex.cts.shim.v1 \
  com.android.apex.cts.shim.v2_wrong_sha \
  com.android.apex.cts.shim_not_pre_installed
```

Generated apexes will be located in the following folders.

*   out/target/product/${DEVICE}/obj/ETC/com.android.apex.cts.shim.v1_intermediates/com.android.apex.cts.shim.v1.apex
*   out/target/product/${DEVICE}/obj/ETC/com.android.apex.cts.shim.v2_intermediates/com.android.apex.cts.shim.v2.apex
*   out/target/product/${DEVICE}/obj/ETC/com.android.apex.cts.shim.v2_wrong_sha_intermediates/com.android.apex.cts.shim.v2_wrong_sha.apex
*   etc.

## Updating shim apexes

Shim apex files should be built on a build server and then downloaded from it
into following directories:

*   system/apex/shim
*   cts/hostsidetests/stagedinstall/testdata/apex
