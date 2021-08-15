# Fluoride Bluetooth Tests

This document refers to the tests in the system/bt/test directory.

The tests are designed to be run when the Android runtime is not running.  From a terminal, run:

## Before you run tests
```sh
adb shell stop
```

## After you're done
```sh
adb shell start
```

## Running tests
Then see what options the test script provides:

```sh
./run_unit_tests.sh --help
```

But for the impatient, run specific groups of tests like this:

```sh
./run_unit_tests.sh net_test_bluetooth
```

a single test:

```sh
./run_unit_tests.sh net_test_bluetooth.BluetoothTest.AdapterRepeatedEnableDisable
```

## Sample Output

system/bt/test$ ./run_unit_tests.sh net_test_bluetooth  
--- net_test_bluetooth ---  
pushing...  
/tbd/aosp-master/out/target/product/bullhead/data/nativetest/n...st_bluetooth: 1 file pushed. 9.2 MB/s (211832 bytes in 0.022s)  
running...  
  
Running main() from gtest_main.cc  
[==========] Running 11 tests from 2 test cases.  
[----------] Global test environment set-up.  
[----------] 6 tests from BluetoothTest  
[ RUN      ] BluetoothTest.AdapterEnableDisable  
[       OK ] BluetoothTest.AdapterEnableDisable (2538 ms)  
[ RUN      ] BluetoothTest.AdapterRepeatedEnableDisable  
[       OK ] BluetoothTest.AdapterRepeatedEnableDisable (11384 ms)  
[ RUN      ] BluetoothTest.AdapterSetGetName  
[       OK ] BluetoothTest.AdapterSetGetName (2378 ms)  
[ RUN      ] BluetoothTest.AdapterStartDiscovery  
[       OK ] BluetoothTest.AdapterStartDiscovery (2397 ms)  
[ RUN      ] BluetoothTest.AdapterCancelDiscovery  
[       OK ] BluetoothTest.AdapterCancelDiscovery (2401 ms)  
[ RUN      ] BluetoothTest.AdapterDisableDuringBonding  
[       OK ] BluetoothTest.AdapterDisableDuringBonding (11689 ms)  
[----------] 6 tests from BluetoothTest (32789 ms total)  

[----------] 5 tests from GattTest  
[ RUN      ] GattTest.GattClientRegister  
[       OK ] GattTest.GattClientRegister (2370 ms)  
[ RUN      ] GattTest.GattClientScanRemoteDevice  
[       OK ] GattTest.GattClientScanRemoteDevice (2273 ms)  
[ RUN      ] GattTest.GattClientAdvertise  
[       OK ] GattTest.GattClientAdvertise (2236 ms)  
[ RUN      ] GattTest.GattServerRegister  
[       OK ] GattTest.GattServerRegister (2391 ms)  
[ RUN      ] GattTest.GattServerBuild  
[       OK ] GattTest.GattServerBuild (2435 ms)  
[----------] 5 tests from GattTest (11706 ms total)  
  
[----------] Global test environment tear-down  
[==========] 11 tests from 2 test cases ran. (44495 ms total)  
[  PASSED  ] 11 tests.  

## Troubleshooting: Your phone is bricked!
Probably not.  See [After you're done](#After-you're-done)

