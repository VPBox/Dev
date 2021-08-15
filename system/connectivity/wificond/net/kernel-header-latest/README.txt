Readme
===========
This folder contains any wifi kernel header updates to the kernels running on
the latest devices, but not present in external/kernel-headers.
external/kernel-headers has the headers from the stable kernel tree which is generally
a few releases behind the kernels shipped on newer devices.

Copy the nl80211.h header from the following location:
cp <kernel-dir>/private/msm-google/include/uapi/linux/nl80211.h
<android-source-dir>/system/connectivity/wificond/net/kernel-header-latest/nl80211.h

Last update from kernel branch: `p-dev-msm-bluecross-4.9`
