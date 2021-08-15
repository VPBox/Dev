# hidl-gen

Full documentation can be found here:
<https://source.android.com/devices/architecture/hidl/>

hidl-gen is a compiler for the HIDL (HAL Interface Design Language) which generates
C++ and Java endpoints for RPC mechanisms. The main userspace libraries which this
compiler uses can be found at system/libhidl.

## 1. Build

```
m hidl-gen
```

## 2. Run

Note that options for hidl-gen expected to be invoked by the build system
are marked with 'internal' in the help menu.

```
hidl-gen -h

hidl-gen -o output -L c++-impl -r android.hardware:hardware/interfaces -r android.hidl:system/libhidl/transport android.hardware.nfc@1.0
```

Some defaults for package roots are also provided

```
hidl-gen -o output -L c++-impl android.hardware.nfc@1.0
hidl-gen -o output -L vts android.hardware.nfc@1.0
hidl-gen -L hash android.hardware.nfc@1.0
```

Example command for vendor project

```
hidl-gen -L c++-impl -r vendor.foo:vendor/foo/interfaces vendor.foo.nfc@1.0
```

See update-makefiles-helper.sh and update-all-google-makefiles.sh for examples
of how to generate HIDL makefiles (using the -Landroidbp option).

# c2hal

This is a helper tool to convert C headers to valid .hal files.

```
m c2hal && c2hal -h
```

# docs

This tool generates html documentation for hal interfaces.

```
m hidl-doc && hidl-doc -h
```