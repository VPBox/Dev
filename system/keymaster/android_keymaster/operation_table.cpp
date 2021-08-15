/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include <keymaster/operation_table.h>
#include <keymaster/operation.h>
#include <keymaster/android_keymaster_utils.h>

#include <keymaster/new>

namespace keymaster {

keymaster_error_t OperationTable::Add(OperationPtr&& operation) {
    if (!table_) {
        table_.reset(new (std::nothrow) OperationPtr[table_size_]);
        if (!table_)
            return KM_ERROR_MEMORY_ALLOCATION_FAILED;
    }
    for (size_t i = 0; i < table_size_; ++i) {
        if (!table_[i]) {
            table_[i] = move(operation);
            return KM_ERROR_OK;
        }
    }
    return KM_ERROR_TOO_MANY_OPERATIONS;
}

Operation* OperationTable::Find(keymaster_operation_handle_t op_handle) {
    if (op_handle == 0)
        return nullptr;

    if (!table_.get())
        return nullptr;

    for (size_t i = 0; i < table_size_; ++i) {
        if (table_[i] && table_[i]->operation_handle() == op_handle)
            return table_[i].get();
    }
    return nullptr;
}

bool OperationTable::Delete(keymaster_operation_handle_t op_handle) {
    if (!table_.get())
        return false;

    for (size_t i = 0; i < table_size_; ++i) {
        if (table_[i] && table_[i]->operation_handle() == op_handle) {
            table_[i].reset();
            return true;
        }
    }
    return false;
}

}  // namespace keymaster
