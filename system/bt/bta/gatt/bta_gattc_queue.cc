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

#include "bta_gatt_queue.h"

#include <list>
#include <unordered_map>
#include <unordered_set>

using gatt_operation = BtaGattQueue::gatt_operation;

constexpr uint8_t GATT_READ_CHAR = 1;
constexpr uint8_t GATT_READ_DESC = 2;
constexpr uint8_t GATT_WRITE_CHAR = 3;
constexpr uint8_t GATT_WRITE_DESC = 4;

struct gatt_read_op_data {
  GATT_READ_OP_CB cb;
  void* cb_data;
};

std::unordered_map<uint16_t, std::list<gatt_operation>>
    BtaGattQueue::gatt_op_queue;
std::unordered_set<uint16_t> BtaGattQueue::gatt_op_queue_executing;

void BtaGattQueue::mark_as_not_executing(uint16_t conn_id) {
  gatt_op_queue_executing.erase(conn_id);
}

void BtaGattQueue::gatt_read_op_finished(uint16_t conn_id, tGATT_STATUS status,
                                         uint16_t handle, uint16_t len,
                                         uint8_t* value, void* data) {
  gatt_read_op_data* tmp = (gatt_read_op_data*)data;
  GATT_READ_OP_CB tmp_cb = tmp->cb;
  void* tmp_cb_data = tmp->cb_data;

  osi_free(data);

  mark_as_not_executing(conn_id);
  gatt_execute_next_op(conn_id);

  if (tmp_cb) {
    tmp_cb(conn_id, status, handle, len, value, tmp_cb_data);
    return;
  }
}

struct gatt_write_op_data {
  GATT_WRITE_OP_CB cb;
  void* cb_data;
};

void BtaGattQueue::gatt_write_op_finished(uint16_t conn_id, tGATT_STATUS status,
                                          uint16_t handle, void* data) {
  gatt_write_op_data* tmp = (gatt_write_op_data*)data;
  GATT_WRITE_OP_CB tmp_cb = tmp->cb;
  void* tmp_cb_data = tmp->cb_data;

  osi_free(data);

  mark_as_not_executing(conn_id);
  gatt_execute_next_op(conn_id);

  if (tmp_cb) {
    tmp_cb(conn_id, status, handle, tmp_cb_data);
    return;
  }
}

void BtaGattQueue::gatt_execute_next_op(uint16_t conn_id) {
  APPL_TRACE_DEBUG("%s: conn_id=0x%x", __func__, conn_id);
  if (gatt_op_queue.empty()) {
    APPL_TRACE_DEBUG("%s: op queue is empty", __func__);
    return;
  }

  auto map_ptr = gatt_op_queue.find(conn_id);
  if (map_ptr == gatt_op_queue.end() || map_ptr->second.empty()) {
    APPL_TRACE_DEBUG("%s: no more operations queued for conn_id %d", __func__,
                     conn_id);
    return;
  }

  if (gatt_op_queue_executing.count(conn_id)) {
    APPL_TRACE_DEBUG("%s: can't enqueue next op, already executing", __func__);
    return;
  }

  gatt_op_queue_executing.insert(conn_id);

  std::list<gatt_operation>& gatt_ops = map_ptr->second;

  gatt_operation& op = gatt_ops.front();

  if (op.type == GATT_READ_CHAR) {
    gatt_read_op_data* data =
        (gatt_read_op_data*)osi_malloc(sizeof(gatt_read_op_data));
    data->cb = op.read_cb;
    data->cb_data = op.read_cb_data;
    BTA_GATTC_ReadCharacteristic(conn_id, op.handle, GATT_AUTH_REQ_NONE,
                                 gatt_read_op_finished, data);

  } else if (op.type == GATT_READ_DESC) {
    gatt_read_op_data* data =
        (gatt_read_op_data*)osi_malloc(sizeof(gatt_read_op_data));
    data->cb = op.read_cb;
    data->cb_data = op.read_cb_data;
    BTA_GATTC_ReadCharDescr(conn_id, op.handle, GATT_AUTH_REQ_NONE,
                            gatt_read_op_finished, data);

  } else if (op.type == GATT_WRITE_CHAR) {
    gatt_write_op_data* data =
        (gatt_write_op_data*)osi_malloc(sizeof(gatt_write_op_data));
    data->cb = op.write_cb;
    data->cb_data = op.write_cb_data;
    BTA_GATTC_WriteCharValue(conn_id, op.handle, op.write_type,
                             std::move(op.value), GATT_AUTH_REQ_NONE,
                             gatt_write_op_finished, data);

  } else if (op.type == GATT_WRITE_DESC) {
    gatt_write_op_data* data =
        (gatt_write_op_data*)osi_malloc(sizeof(gatt_write_op_data));
    data->cb = op.write_cb;
    data->cb_data = op.write_cb_data;
    BTA_GATTC_WriteCharDescr(conn_id, op.handle, std::move(op.value),
                             GATT_AUTH_REQ_NONE, gatt_write_op_finished, data);
  }

  gatt_ops.pop_front();
}

void BtaGattQueue::Clean(uint16_t conn_id) {
  gatt_op_queue.erase(conn_id);
  gatt_op_queue_executing.erase(conn_id);
}

void BtaGattQueue::ReadCharacteristic(uint16_t conn_id, uint16_t handle,
                                      GATT_READ_OP_CB cb, void* cb_data) {
  gatt_op_queue[conn_id].push_back({.type = GATT_READ_CHAR,
                                    .handle = handle,
                                    .read_cb = cb,
                                    .read_cb_data = cb_data});
  gatt_execute_next_op(conn_id);
}

void BtaGattQueue::ReadDescriptor(uint16_t conn_id, uint16_t handle,
                                  GATT_READ_OP_CB cb, void* cb_data) {
  gatt_op_queue[conn_id].push_back({.type = GATT_READ_DESC,
                                    .handle = handle,
                                    .read_cb = cb,
                                    .read_cb_data = cb_data});
  gatt_execute_next_op(conn_id);
}

void BtaGattQueue::WriteCharacteristic(uint16_t conn_id, uint16_t handle,
                                       std::vector<uint8_t> value,
                                       tGATT_WRITE_TYPE write_type,
                                       GATT_WRITE_OP_CB cb, void* cb_data) {
  gatt_op_queue[conn_id].push_back({.type = GATT_WRITE_CHAR,
                                    .handle = handle,
                                    .write_type = write_type,
                                    .write_cb = cb,
                                    .write_cb_data = cb_data,
                                    .value = std::move(value)});
  gatt_execute_next_op(conn_id);
}

void BtaGattQueue::WriteDescriptor(uint16_t conn_id, uint16_t handle,
                                   std::vector<uint8_t> value,
                                   tGATT_WRITE_TYPE write_type,
                                   GATT_WRITE_OP_CB cb, void* cb_data) {
  gatt_op_queue[conn_id].push_back({.type = GATT_WRITE_DESC,
                                    .handle = handle,
                                    .write_type = write_type,
                                    .write_cb = cb,
                                    .write_cb_data = cb_data,
                                    .value = std::move(value)});
  gatt_execute_next_op(conn_id);
}
