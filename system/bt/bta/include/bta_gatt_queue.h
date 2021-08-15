/*
 * Copyright 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <vector>

#include <list>
#include <unordered_map>
#include <unordered_set>
#include "bta_gatt_api.h"

/* BTA GATTC implementation does not allow for multiple commands queuing. So one
 * client making calls to BTA_GATTC_ReadCharacteristic, BTA_GATTC_ReadCharDescr,
 * BTA_GATTC_WriteCharValue, BTA_GATTC_WriteCharDescr must wait for the callacks
 * before scheduling next operation.
 *
 * Methods below can be used as replacement to BTA_GATTC_* in BTA app. They do
 * queue the commands if another command is currently being executed.
 *
 * If you decide to use those methods in your app, make sure to not mix it with
 * existing BTA_GATTC_* API.
 */
class BtaGattQueue {
 public:
  static void Clean(uint16_t conn_id);
  static void ReadCharacteristic(uint16_t conn_id, uint16_t handle,
                                 GATT_READ_OP_CB cb, void* cb_data);
  static void ReadDescriptor(uint16_t conn_id, uint16_t handle,
                             GATT_READ_OP_CB cb, void* cb_data);
  static void WriteCharacteristic(uint16_t conn_id, uint16_t handle,
                                  std::vector<uint8_t> value,
                                  tGATT_WRITE_TYPE write_type,
                                  GATT_WRITE_OP_CB cb, void* cb_data);
  static void WriteDescriptor(uint16_t conn_id, uint16_t handle,
                              std::vector<uint8_t> value,
                              tGATT_WRITE_TYPE write_type, GATT_WRITE_OP_CB cb,
                              void* cb_data);

  /* Holds pending GATT operations */
  struct gatt_operation {
    uint8_t type;
    uint16_t handle;
    GATT_READ_OP_CB read_cb;
    void* read_cb_data;
    GATT_WRITE_OP_CB write_cb;
    void* write_cb_data;

    /* write-specific fields */
    tGATT_WRITE_TYPE write_type;
    std::vector<uint8_t> value;
  };

 private:
  static void mark_as_not_executing(uint16_t conn_id);
  static void gatt_execute_next_op(uint16_t conn_id);
  static void gatt_read_op_finished(uint16_t conn_id, tGATT_STATUS status,
                                    uint16_t handle, uint16_t len,
                                    uint8_t* value, void* data);
  static void gatt_write_op_finished(uint16_t conn_id, tGATT_STATUS status,
                                     uint16_t handle, void* data);

  // maps connection id to operations waiting for execution
  static std::unordered_map<uint16_t, std::list<gatt_operation>> gatt_op_queue;
  // contain connection ids that currently execute operations
  static std::unordered_set<uint16_t> gatt_op_queue_executing;
};