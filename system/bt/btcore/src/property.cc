/******************************************************************************
 *
 *  Copyright 2014 Google, Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#include "btcore/include/property.h"
#include <base/logging.h>
#include <string.h>
#include "btcore/include/device_class.h"
#include "osi/include/allocator.h"
#include "osi/include/compat.h"

using bluetooth::Uuid;

static bt_property_t* property_new_(void* val, size_t len,
                                    bt_property_type_t type);

bt_property_t* property_copy_array(const bt_property_t* properties,
                                   size_t count) {
  CHECK(properties != NULL);
  bt_property_t* clone =
      static_cast<bt_property_t*>(osi_calloc(sizeof(bt_property_t) * count));

  memcpy(&clone[0], &properties[0], sizeof(bt_property_t) * count);
  for (size_t i = 0; i < count; ++i) {
    clone[i].val = osi_calloc(clone[i].len);
    memcpy(clone[i].val, properties[i].val, clone[i].len);
  }

  return clone;
}

bt_property_t* property_copy(bt_property_t* dest, const bt_property_t* src) {
  CHECK(dest != NULL);
  CHECK(src != NULL);
  return (bt_property_t*)memcpy(dest, src, sizeof(bt_property_t));
}

bool property_equals(const bt_property_t* p1, const bt_property_t* p2) {
  // Two null properties are not the same. May need to revisit that
  // decision when we have a test case that exercises that condition.
  if (!p1 || !p2 || p1->type != p2->type) {
    return false;
  }

  // Although the Bluetooth name is a 249-byte array, the implementation
  // treats it like a variable-length array with its size specified in the
  // property's `len` field. We special-case the equivalence of BDNAME
  // types here by truncating the larger, zero-padded name to its string
  // length and comparing against the shorter name.
  //
  // Note: it may be the case that both strings are zero-padded but that
  // hasn't come up yet so this implementation doesn't handle it.
  if (p1->type == BT_PROPERTY_BDNAME && p1->len != p2->len) {
    const bt_property_t *shorter = p1, *longer = p2;
    if (p1->len > p2->len) {
      shorter = p2;
      longer = p1;
    }
    return strlen((const char*)longer->val) == (size_t)shorter->len &&
           !memcmp(longer->val, shorter->val, shorter->len);
  }

  return p1->len == p2->len && !memcmp(p1->val, p2->val, p1->len);
}

bt_property_t* property_new_addr(const RawAddress* addr) {
  CHECK(addr != NULL);
  return property_new_((void*)addr, sizeof(RawAddress), BT_PROPERTY_BDADDR);
}

bt_property_t* property_new_device_class(const bt_device_class_t* dc) {
  CHECK(dc != NULL);
  return property_new_((void*)dc, sizeof(bt_device_class_t),
                       BT_PROPERTY_CLASS_OF_DEVICE);
}

bt_property_t* property_new_device_type(bt_device_type_t type) {
  return property_new_((void*)&type, sizeof(bt_device_type_t),
                       BT_PROPERTY_TYPE_OF_DEVICE);
}

bt_property_t* property_new_discovery_timeout(const uint32_t timeout) {
  return property_new_((void*)&timeout, sizeof(uint32_t),
                       BT_PROPERTY_ADAPTER_DISCOVERY_TIMEOUT);
}

bt_property_t* property_new_name(const char* name) {
  CHECK(name != NULL);
  return property_new_((void*)name, sizeof(bt_bdname_t), BT_PROPERTY_BDNAME);
}

bt_property_t* property_new_rssi(int8_t rssi) {
  return property_new_((void*)&rssi, sizeof(int8_t), BT_PROPERTY_REMOTE_RSSI);
}

bt_property_t* property_new_scan_mode(bt_scan_mode_t scan_mode) {
  return property_new_((void*)&scan_mode, sizeof(bt_scan_mode_t),
                       BT_PROPERTY_ADAPTER_SCAN_MODE);
}

bt_property_t* property_new_uuids(const Uuid* uuid, size_t count) {
  CHECK(uuid != NULL);
  return property_new_((void*)uuid, sizeof(Uuid) * count, BT_PROPERTY_UUIDS);
}

void property_free(bt_property_t* property) {
  property_free_array(property, 1);
}

void property_free_array(bt_property_t* properties, size_t count) {
  if (properties == NULL) return;

  for (size_t i = 0; i < count; ++i) {
    osi_free(properties[i].val);
  }

  osi_free(properties);
}

bool property_is_addr(const bt_property_t* property) {
  CHECK(property != NULL);
  return property->type == BT_PROPERTY_BDADDR;
}

bool property_is_device_class(const bt_property_t* property) {
  CHECK(property != NULL);
  return property->type == BT_PROPERTY_CLASS_OF_DEVICE;
}

bool property_is_device_type(const bt_property_t* property) {
  CHECK(property != NULL);
  return property->type == BT_PROPERTY_TYPE_OF_DEVICE;
}

bool property_is_discovery_timeout(const bt_property_t* property) {
  CHECK(property != NULL);
  return property->type == BT_PROPERTY_ADAPTER_DISCOVERY_TIMEOUT;
}

bool property_is_name(const bt_property_t* property) {
  CHECK(property != NULL);
  return property->type == BT_PROPERTY_BDNAME;
}

bool property_is_rssi(const bt_property_t* property) {
  CHECK(property != NULL);
  return property->type == BT_PROPERTY_REMOTE_RSSI;
}

bool property_is_scan_mode(const bt_property_t* property) {
  CHECK(property != NULL);
  return property->type == BT_PROPERTY_ADAPTER_SCAN_MODE;
}

bool property_is_uuids(const bt_property_t* property) {
  CHECK(property != NULL);
  return property->type == BT_PROPERTY_UUIDS;
}

// Convenience conversion methods to property values
const RawAddress* property_as_addr(const bt_property_t* property) {
  CHECK(property_is_addr(property));
  return (const RawAddress*)property->val;
}

const bt_device_class_t* property_as_device_class(
    const bt_property_t* property) {
  CHECK(property_is_device_class(property));
  return (const bt_device_class_t*)property->val;
}

bt_device_type_t property_as_device_type(const bt_property_t* property) {
  CHECK(property_is_device_type(property));
  return *(const bt_device_type_t*)property->val;
}

uint32_t property_as_discovery_timeout(const bt_property_t* property) {
  CHECK(property_is_discovery_timeout(property));
  return *(const uint32_t*)property->val;
}

const bt_bdname_t* property_as_name(const bt_property_t* property) {
  CHECK(property_is_name(property));
  return (const bt_bdname_t*)property->val;
}

int8_t property_as_rssi(const bt_property_t* property) {
  CHECK(property_is_rssi(property));
  return *(const int8_t*)property->val;
}

bt_scan_mode_t property_as_scan_mode(const bt_property_t* property) {
  CHECK(property_is_scan_mode(property));
  return *(const bt_scan_mode_t*)property->val;
}

const Uuid* property_as_uuids(const bt_property_t* property, size_t* count) {
  CHECK(property_is_uuids(property));
  *count = sizeof(Uuid) / property->len;
  return (const Uuid*)property->val;
}

static bt_property_t* property_new_(void* val, size_t len,
                                    bt_property_type_t type) {
  bt_property_t* property =
      static_cast<bt_property_t*>(osi_calloc(sizeof(bt_property_t)));

  property->val = osi_calloc(len + 1);
  if (type == BT_PROPERTY_BDNAME) {
    strlcpy((char*)property->val, (const char*)val, len);
  } else {
    memcpy(property->val, val, len);
  }

  property->type = type;
  property->len = len;

  return property;
}
