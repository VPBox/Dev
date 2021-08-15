# c2hal user-guide

## 1. Build

```
croot
make c2hal -j64
```

## 2. Run

```
c2hal [-g] [-o dir] -p package (-r interface-root)+ (header-filepath)+
```

-o output path: If missing, the second half of a relevant interface-root will be used.

-p package: For example android.hardware.baz@1.0.
This will be used as the package in .hal files and will also be used to construct the correct directory structure.

-g: Enabling this flag changes the behavior of c2hal to parse opengl files.

-r package:path root: For example 'android.hardware:hardware/interfaces'.

Examples:

```
# Build the test.h header:
c2hal -r android.hardware:hardware/interfaces -p android.hardware.baz@1.0 system/tools/hidl/c2hal/test/test.h

# Build the simple.h header:
c2hal -r android.hardware:hardware/interfaces -p android.hardware.simple@1.0 system/tools/hidl/c2hal/test/simple.h

# Build a particular libhardware header:
c2hal -r android.hardware:hardware/interfaces -p android.hardware.nfc@1.0 hardware/libhardware/include/hardware/nfc.h

# Build all headers from libhardware:
python3 system/tools/hidl/c2hal/test/build_all.py ~/android/master/hardware/libhardware/include/hardware/

# Build various OpenGl versions:
python3 system/tools/hidl/c2hal/test/build_all.py -g ~/android/master/frameworks/native/opengl/include/EGL/
python3 system/tools/hidl/c2hal/test/build_all.py -g ~/android/master/frameworks/native/opengl/include/ETC1/
python3 system/tools/hidl/c2hal/test/build_all.py -g ~/android/master/frameworks/native/opengl/include/GLES/
python3 system/tools/hidl/c2hal/test/build_all.py -g ~/android/master/frameworks/native/opengl/include/GLES2/
python3 system/tools/hidl/c2hal/test/build_all.py -g ~/android/master/frameworks/native/opengl/include/GLES3/
python3 system/tools/hidl/c2hal/test/build_all.py -g ~/android/master/frameworks/native/opengl/include/KHR/
```

