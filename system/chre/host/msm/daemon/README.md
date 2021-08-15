This folder contains source code for the daemon used to host CHRE on SLPI for
MSM-based devices.

This daemon cannot be built out of this folder, however, because it depends on
proprietary header files distributed as part of the Hexagon SDK. Therefore, it
is compiled from a device-specific directory in vendor/.
