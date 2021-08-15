/******************************************************************************
 *
 *  Copyright 2015 Google, Inc.
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

#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "gatt/gatt_test.h"

namespace bttest {

TEST_F(GattTest, GattClientRegister) {
  // Registers gatt client.
  bluetooth::Uuid gatt_client_uuid = bluetooth::Uuid::GetRandom();
  gatt_client_interface()->register_client(gatt_client_uuid);
  semaphore_wait(register_client_callback_sem_);
  EXPECT_TRUE(status() == BT_STATUS_SUCCESS)
      << "Error registering GATT client app callback.";

  // Unregisters gatt client. No callback is expected.
  gatt_client_interface()->unregister_client(client_interface_id());
}

TEST_F(GattTest, GattServerRegister) {
  // Registers gatt server.
  bluetooth::Uuid gatt_server_uuid = bluetooth::Uuid::GetRandom();
  gatt_server_interface()->register_server(gatt_server_uuid);
  semaphore_wait(register_server_callback_sem_);
  EXPECT_TRUE(status() == BT_STATUS_SUCCESS)
      << "Error registering GATT server app callback.";

  // Unregisters gatt server. No callback is expected.
  gatt_server_interface()->unregister_server(server_interface_id());
}

TEST_F(GattTest, GattServerBuild) {
  // Registers gatt server.
  bluetooth::Uuid gatt_server_uuid = bluetooth::Uuid::GetRandom();
  gatt_server_interface()->register_server(gatt_server_uuid);
  semaphore_wait(register_server_callback_sem_);
  EXPECT_TRUE(status() == BT_STATUS_SUCCESS)
      << "Error registering GATT server app callback.";

  // Service UUID.
  bluetooth::Uuid srvc_uuid = bluetooth::Uuid::GetRandom();

  // Characteristics UUID.
  bluetooth::Uuid char_uuid = bluetooth::Uuid::GetRandom();

  // Descriptor UUID.
  bluetooth::Uuid desc_uuid = bluetooth::Uuid::GetRandom();

  // Adds service.
  int server_if = server_interface_id();

  std::vector<btgatt_db_element_t> service = {
      {.type = BTGATT_DB_PRIMARY_SERVICE, .uuid = srvc_uuid},
      {.type = BTGATT_DB_CHARACTERISTIC,
       .uuid = char_uuid,
       .properties = 0x10 /* notification */,
       .permissions = 0x01 /* read only */},
      {.type = BTGATT_DB_DESCRIPTOR, .uuid = desc_uuid, .permissions = 0x01}};

  gatt_server_interface()->add_service(server_if, service);
  semaphore_wait(service_added_callback_sem_);
  EXPECT_TRUE(status() == BT_STATUS_SUCCESS) << "Error adding service.";
  EXPECT_TRUE(server_interface_id() == server_if) << "Wrong server_if added.";
  int service_handle_added = service_handle();

  // Stops server.
  gatt_server_interface()->stop_service(server_if, service_handle());
  semaphore_wait(service_stopped_callback_sem_);
  EXPECT_TRUE(status() == BT_STATUS_SUCCESS) << "Error stopping server.";
  EXPECT_TRUE(service_handle() == service_handle_added)
      << "Wrong service handle stopped.";
  EXPECT_TRUE(server_interface_id() == server_if) << "Wrong server_if stopped.";

  // Deletes service.
  gatt_server_interface()->delete_service(server_if, service_handle());
  semaphore_wait(service_deleted_callback_sem_);
  EXPECT_TRUE(status() == BT_STATUS_SUCCESS) << "Error deleting service.";
  EXPECT_TRUE(service_handle() == service_handle_added)
      << "Wrong service handle deleted.";
  EXPECT_TRUE(server_interface_id() == server_if) << "Wrong server_if deleted.";

  // Unregisters gatt server. No callback is expected.
  gatt_server_interface()->unregister_server(server_if);
}

}  // bttest
