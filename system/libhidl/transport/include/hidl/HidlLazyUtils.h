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

#pragma once

#include <android/hidl/base/1.0/IBase.h>
#include <utils/RefBase.h>
#include <utils/StrongPointer.h>

namespace android {
namespace hardware {
namespace details {
class LazyServiceRegistrarImpl;
}  // namespace details

/** Exits when all HALs registered through this object have 0 clients */
class LazyServiceRegistrar {
   public:
    LazyServiceRegistrar();
    status_t registerService(const sp<::android::hidl::base::V1_0::IBase>& service,
                             const std::string& name = "default");

   private:
    std::shared_ptr<details::LazyServiceRegistrarImpl> mImpl;
};

}  // namespace hardware
}  // namespace android
