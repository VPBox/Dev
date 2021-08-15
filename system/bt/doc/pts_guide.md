# Fluoride Bluetooth Profile Tuning Suite (PTS) Test Mode

This document provides commands to enable PTS test mode for Fluoride stack. We
need special handling for some test cases as they are not applicable for the
Fluoride stack.

## PTS Test Mode system property

Profile services in packages/apps/Bluetooth uses system property
`persist.bluetooth.pts` to check if the PTS test mode is enabled. To enable it:

```sh
adb shell setprop persist.bluetooth.pts true
```

To disable it:

```sh
adb shell setprop persist.bluetooth.pts false
```

### Current use case

- In `newavrcp`, we send active player update to remote device only in PTS test
  mode (AVRCP/TG/MPS/BV-05-C AVRCP/TG/MPS/BV-07-C).

## PTS Helpers in stack config

Native stack also requires some special handling, and the config is stored in
`conf/bt_stack.conf`. To enable a flag, uncomment the corresponding line and
push the config file to `/etc/bluetooth/` in IUT.

### Current use case

- `PTS_SecurePairOnly` enables secure connections only mode.
- `PTS_DisableConnUpdates` disables LE Connection updates.
- `PTS_DisableSDPOnLEPair` disables BR/EDR discovery after LE pairing to avoid
  cross key derivation errors.
- `PTS_SmpOptions` sets SMP Pair options (formatted as hex bytes) `auth, io,
  ikey, rkey, ksize`.
- `PTS_AvrcpTest` enables AVRCP test mode. The UID is set to 0xffffffffffffffff
  in `TrackChangedNotificationResponse` (AVRCP/TG/NFY/BV-04-C).
- `PTS_SmpFailureCase` enables handling for various SMP failure cases.
