# Context Hub Runtime Environment (CHRE)

## Build Instructions

Build targets are arranged in the form of a variant triple consisting of:

``vendor_arch_variant``

The vendor is the provider of the CHRE implementation (ex: google, qcom). The
arch is the CPU architecture (ie: hexagonv60, x86, cm4). The variant is the
target platform (ie: slpi, nanohub, linux, googletest).

A debug build can be obtained by appending ``_debug`` to the variant triple. As
an example:

``make google_hexagonv62_slpi``
``make google_hexagonv62_slpi_debug``

### Linux

CHRE is compatible with Linux as a simulator.

#### Linux Build/Run

The simulator has system dependencies:

- TCLAP
    - Command-line argument parsing.
- libsndfile
    - WAV file parsing for audio support.

These are the commands to install these dependencies for Ubuntu:

    sudo apt-get install libtclap-dev
    sudo apt-get install libsndfile1-dev

The build target for x86 linux is ``google_x86_linux``. You can build/run the
simulator with the following command:

    ./run_sim.sh

#### Linux Unit Tests

You can run all unit tests with the following command. Pass arguments to this
script and they are passed to the gtest framework. (example:
``--gtest_filter=DynamicVector.*``)

    ./run_tests.sh

### CHRE Simulator for Android

CHRE is also compatible with Android as a simulator.

This is not intended to be a production implementation but is suitable for
testing CHRE nanoapps on the applications processor where Android runs. It uses
Android NDK APIs to interact with the system.

### SLPI Hexagon

First, setup paths to the Hexagon Tools (v8.x.x), SDK (v3.0), and SLPI source
tree, for example:

    export HEXAGON_TOOLS_PREFIX=~/Qualcomm/HEXAGON_Tools/8.0
    export HEXAGON_SDK_PREFIX=~/Qualcomm/Hexagon_SDK/3.0
    export SLPI_PREFIX=~/Qualcomm/msm8998/slpi_proc

Then use the provided Makefiles to build:

    make google_hexagonv62_slpi -j

## Directory Structure

The CHRE project is organized as follows:

- ``chre_api``
    - The stable API exposed to nanoapps
- ``core``
    - Common code that applies to all CHRE platforms, most notably event
      management.
- ``pal``
    - An abstraction layer that implementers must supply to access
      device-specific functionality (such as GPS and Wi-Fi). The PAL is a C API
      which allows it to be implemented using a vendor-supplied library.
- ``platform``
    - Contains the system interface that all plaforms must implement, along with
      implementations for individual platforms. This includes the implementation
      of the CHRE API.
    - ``platform/shared``
        - Contains code that will apply to multiple platforms, but not
          necessarily all.
    - ``platform/linux``
        - This directory contains the canonical example for running CHRE on
          desktop machines, primarily for simulation and testing.
- ``apps``
    - A small number of sample applications are provided. These are intended to
      guide developers of new applications and help implementers test basic
      functionality quickly.
    - This is reference code and is not required for the CHRE to function.
- ``util``
    - Contains data structures used throughout CHRE and common utility code.
- ``variant/simulator``
    - Contains the CHRE variant for the simulator. This is a good example to
      start from when porting to new devices. Variants are explained in more
      detail below.

Within each of these directories, you may find a ``tests`` subdirectory
containing tests written against the googletest framework.

### Platform Directory Structure

The platform directory contains an interface that common code under ``core``
leverages to implement the runtime. All platforms are required to implement the
interface provided in ``platform/include``.

The following gives a more detailed explanation of the directory structure.

- ``platform`` - The top-level directory for platform-specific code.
    - ``include`` - The interface that platforms are required to implement.
    - ``shared`` - Code that may be shared by more than one platform but not
                   necessarily required for all.
    - ``slpi`` - The implementation of the common interface for the SLPI and any
                 SLPI-specific code.
    - ``linux`` - The implementation of the common interface for the simulator
                  running on Linux and any simulator-specific code.

Common CHRE code that is expected to run across all platforms is located in
``core``. This code must have a stable way to access the platform-specific
implementation of the common platform API. This is handled by providing a stable
include path and changing the search path for the platform implementation. Here
is an example directory layout:

- ``platform``
    - ``<platform_name>``
        - ``include``
            - ``chre``
                - ``target_platform``

The build system will add ``platform/<platform_name>/include`` to the include
search path allowing common code to find the implementation of the platform
interface. Here is an example of core code including a platform-specific header
in this way:

``#include "chre/target_platform/log.h"``

When building for the linux platform, the file is included from:

``platform/linux/include/chre/target_platform/log.h``

## Supplied Nanoapps

This project includes a number of nanoapps that serve as both examples of how to
use CHRE, debugging tools and can perform some useful function.

All nanoapps in the ``apps`` directory are placed in a namespace when built
statically with this CHRE implementation. When compiled as standalone nanoapps,
there is no outer namespace on their entry points. This allows testing various
CHRE subsystems without requiring dynamic loading and allows these nanoapps to
coexist within a CHRE binary. Refer to ``apps/hello_world/hello_world.cc`` for
a minimal example.

### FeatureWorld

Any of the nanoapps that end with the term World are intended to test some
feature of the system. The HelloWorld nanoapp simply exercises logging
functionality, TimerWorld exercises timers and WifiWorld uses wifi, for example.
These nanoapps log all results via chreLog which makes them effective tools when
bringing up a new CHRE implementation.

### ImuCal

This nanoapp implements IMU calibration.

## Porting CHRE

This codebase is intended to be ported to a variety of operating systems. If you
wish to port CHRE to a new OS, refer to the ``platform`` directory. An example of
the Linux port is provided under ``platform/linux``.

There are notes regarding initialization under
``platform/include/chre/platform/init.h`` that will also be helpful.

### Important Considerations

Platforms are required to implement support for invoking the constructors and
destructors of global, non-POD types at load and unload time, respectively. This
is required for both the runtime and nanoapps.

## Coding conventions

There are many well-established coding standards within Google. The official
C++ style guide is used with the exception of Android naming conventions for
methods and variables. This means 2 space indents, camelCase method names, an
mPrefix on class members and so on. Style rules that are not specified in the
Android style guide are inherited from Google.

## CHRE Variants

A CHRE variant allows injecting additional source files into the build on a
per-device basis. This can be used to inject:

* A version string
    * Set to ``undefined`` if not specified
* A static nanoapp list
    * Empty if left undefined
* Additional static nanoapp includes
    * Vendor-specific nanoapps could be specified in the variant

Export the ``CHRE_VARIANT_MK_INCLUDES`` containing the mk files that you wish to
be included the CHRE variant build. Refer to ``run_sim.sh`` and the
``variant/simulator`` subdirectory for an example as used by the simulator.

* [Google C++ Style][1]

[1]: https://google.github.io/styleguide/cppguide.html

### Use of C++

This project uses C++11, but with two main caveats:

 1. General considerations for using C++ in an embedded environment apply. This
    means avoiding language features that can impose runtime overhead should
    be avoided, due to the relative scarcity of memory and CPU resources, and
    power considerations. Examples include RTTI, exceptions, overuse of dynamic
    memory allocation, etc. Refer to existing literature on this topic
    including this [Technical Report on C++ Performance][2] and so on.
 2. Support of C++ standard libraries are not generally expected to be
    extensive or widespread in the embedded environments where this code will
    run. That means that things like <thread> and <mutex> should not be used,
    in favor of simple platform abstractions that can be implemented directly
    with less effort (potentially using those libraries if they are known to be
    available).

[2]: http://www.open-std.org/jtc1/sc22/wg21/docs/TR18015.pdf
