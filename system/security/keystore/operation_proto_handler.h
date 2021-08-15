/*
 * Copyright (C) 2018 The Android Open Source Project
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

#ifndef KEYSTORE_OPERATION_PROTO_HANDLER_H_
#define KEYSTORE_OPERATION_PROTO_HANDLER_H_

#include "operation_config.pb.h"
#include "operation_struct.h"
#include <chrono>
#include <mutex>
#include <unordered_map>
#include <vector>

namespace keystore {

using ::android::IBinder;
using keymaster::support::Keymaster;

class OperationProtoHandler {
  public:
    void uploadOpAsProto(Operation& op, bool wasOpSuccessful);

  private:
    std::unordered_map<std::string, int> protoMap;
    std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
    std::mutex op_upload_mutex;
};

}  // namespace keystore

#endif  // KEYSTORE_OPERATION_PROTO_HANDLER_H_
