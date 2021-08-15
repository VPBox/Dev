/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef NVRAM_HAL_NVRAM_DEVICE_ADAPTER_H_
#define NVRAM_HAL_NVRAM_DEVICE_ADAPTER_H_

#include <memory>

#include <hardware/nvram.h>
#include <nvram/messages/nvram_messages.h>

namespace nvram {

// |NvramImplementation| subclasses provide an implementation of the NVRAM HAL
// logic.
class NvramImplementation {
 public:
  virtual ~NvramImplementation() = default;

  // This function services all operations defined for the NVRAM HAL. The input
  // parameters are passed in |request| and |response| will be filled in with
  // the result and output parameters of the operation.
  virtual void Execute(const nvram::Request& request,
                       nvram::Response* response) = 0;
};

// |NvramDeviceAdapater| provides glue to turn an |NvramImplementation| object
// into an |nvram_device_t| as defined by the NVRAM HAL C API. This is intended
// to be used in the HAL module's |open()| operation. To obtain the desired
// |hw_device_t|, just create an |NvramDeviceAdapter| with a suitable
// |NvramImplementation| and call |as_device()| to get the HAL device pointer.
struct NvramDeviceAdapter {
 public:
  // Takes ownership of |implementation|.
  NvramDeviceAdapter(const hw_module_t* module,
                     NvramImplementation* implementation);

  hw_device_t* as_device() { return &device_.common; }
  NvramImplementation* nvram_implementation() { return implementation_.get(); }

 private:
  nvram_device_t device_;
  std::unique_ptr<NvramImplementation> implementation_;
};

// Make sure |NvramDeviceAdapter| is a standard layout type. This guarantees
// that casting from/to the type of the first non-static member (i.e. |device_|)
// works as expected.
static_assert(std::is_standard_layout<NvramDeviceAdapter>::value,
              "NvramDeviceAdapater must be a standard layout type.");

}  // namespace nvram

#endif  // NVRAM_HAL_NVRAM_DEVICE_ADAPTER_H_
