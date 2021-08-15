package test;

/* This file is just a test to make sure all parcelables
 * are correct. It will be removed once they're used in
 * real AIDL files.
 */

import android.bluetooth.AdvertiseData;
import android.bluetooth.AdvertiseSettings;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattDescriptor;
import android.bluetooth.BluetoothGattIncludedService;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.ScanFilter;
import android.bluetooth.ScanResult;
import android.bluetooth.ScanSettings;
import android.bluetooth.UUID;

interface ParcelableTest {
  void OnAdvertiseData(in AdvertiseData advertise_data);
  void OnAdvertiseSettings(in AdvertiseSettings advertise_settings);
  void OnScanFilter(in ScanFilter scan_filter);
  void OnScanResult(in ScanResult scan_result);
  void OnScanSettings(in ScanSettings scan_settings);
  void OnUUID(in UUID uuid);
  void OnDescripor(in BluetoothGattDescriptor descriptor);
  void OnCharacteristic(in BluetoothGattCharacteristic characteristic);
  void OnIncludedService(in BluetoothGattIncludedService service);
  void OnService(in BluetoothGattService service);
}
