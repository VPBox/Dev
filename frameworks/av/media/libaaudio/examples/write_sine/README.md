# cd to this directory
mkdir -p jni/include/aaudio
ln -s $PLATFORM/frameworks/av/media/liboboe/include/aaudio/*.h jni/include/aaudio
ln -s $PLATFORM/out/target/product/$TARGET_PRODUCT/symbols/out/soong/ndk/platforms/android-current/arch-arm64/usr/lib/liboboe.so jni
$NDK/ndk-build
adb push libs/arm64-v8a/write_sine_threaded /data
adb shell /data/write_sine_threaded
