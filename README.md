# Paper

This software is the outcome of our accademic research.

If you use this code, please cite our accademic paper as:

@inproceedings{VPBox,
 author = {Wenna Song and Jiang Ming and Lin Jiang and Yi Xiang and Xuanchen Pan and Jianming Fu and Guojun Peng},
 title = {{Towards Transparent and Stealthy Android OS Sandboxing via Customizable Container-Based Virtualization}},
 booktitle = {Proceedings of the 28th ACM Conference on Computer and Communications Security (CCS'21)},
 year = {2021}
} 


# VPBox (Android 10)

We develop a transparent and stealthy Android OS Sandboxing named VPBox. With VPBox, security analysts can bypass a set of virtual environment detection heuristics. Besides, we demonstrate VPBox’s flexibility in testing environment-sensitive malware that try to evade sandboxes. VPBox’s design is capable of meeting the following two progressive requirements: 

- RQ1: Transparency. This requirement involves the virtualized device exhibits the same hardware effects as the underlying physical device; and complete virtualization support for all devices and services listed in this paper. 
- RQ2: Stealthiness. On top of the transparency, this requirement ensures a dedicated adversary in the VP is difficult to fingerprint the presence of the container, including the presence of virtualization components and the particular mobile device that runs the container. 

VPBox reveals a strong resilience against virtual-machine detection heuristics and device-consistency checks, as well as native performance. VPBox’s demo video is available at https://youtu.be/TpGD_jjxSqc.  

## Overview of VPBox's Virtualization Architecture

<img src="https://github.com/VPBox/Dev/blob/master/VBOX-Architecture.png" width="398">

The figure provides an overview of VPBox’s device virtualization. Each isolated VP runs a stock Android userspace environment. The names in red in figure represent Cells’s modules reused by VPBox. VPBox updates the other modules. For updated code implementation, please see the section of the code introduction below. 

## Code Introduction



### cells
  - cells/: VP manager daemons
    
  - cellsservice/: VP manager

  - cellsapp/: switch applications

  - busybox/: Linux tools

### system
  - core/adb/: adb mutex virtualization

  - core/init/: VP startup

  - core/rootdir/init.cells.rc: VP's init.rc

  - core/rootdir/cells/: additional configuration files for the VP

  - core/sdcard/: sdcard virtualization

### kernel
  - drivers/base/core.c: drv namespace initialization

  - kernel/drv_namespace.c: drv namespace api

  - kernel/nsproxy.c: namespaces proxy

  - drivers/android/binder.c: binder virtualization

  - drivers/input/evdev.c: input virtualization

  - kernel/power/wakelock.c: wakelock virtualization

### frameworks
  - av/: camera audio video media virtualization

  - native/libs/binder/: binder virtualization

  - native/services/surfaceflinger/: display virtualization

  - base/services/core/java/com/android/server/CellsService.java: net virtualization

  - base/core/java/android/app/CellsPrivateServiceManager.java: cellsservice java service

### android-binder
  - binder virtualization  of the android operating system

### configuration file

  - share-services: compile system configuration file

  - kernel-modify-config： kernel config modify file

  - build.VPDroid.prop build.VPDroid.cell2.prop build.VPDroid.cell3.prop:  custom configuration file for update customized environment

### other modify

  - aufs and ksm modify see commit log

### basic code 

- The Android source code is:  android-10.0.0_r33

- The ways to download source code:  https://source.android.com/setup/develop/repo

## System Prerequisites

- Operating System: Ubuntu 20.04 LTS

- JDK version：openJDK version 9

## Compile Command

`source build/envsetup.sh`

`lunch 4`

`make -j4`


## Benchmarks and Samples

### Benchmarks 

- Linpack (v1.1) for CPU; 

- Quadrant advanced edition (v2.1.1) for 2D graphics and file I/O; 

- 3DMark (v2.0.4646) for 3D graphics;

- BusyBox wget (v1.21.1) for networking.

- GPS Test app (v1.6.3) for GPS.

- Bluetooth Setting for Bluetooth. 

### Samples  

We have used nine families of malware to evaluate virtualization environment detection.

Rotexy，Ashas, HeHe, Ztorg, Andr RuSms-AT, OBAD, Android BankBot, GhostClicker, G-Ware.






