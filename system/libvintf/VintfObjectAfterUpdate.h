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

#include <memory>

#include "SchemaType.h"
#include "VintfObject.h"

namespace android {
namespace vintf {
namespace details {
/**
 * Simulate the state of VintfObject after an update.
 *
 * - Old metadata is stored in parent VintfObject.
 * - New (updated) metadata is stored in this VintfObjectAfterUpdate
 * - Dependencies are from the given VintfObject (dep) before construction.
 */
class VintfObjectAfterUpdate : public VintfObject {
   public:
    /* Use dependencies from the object dep. */
    VintfObjectAfterUpdate(VintfObject* dep) : mDependency(dep) {}

    std::shared_ptr<const HalManifest> getDeviceHalManifest(bool skipCache = false) override {
        if (mDeviceManifest != nullptr) return mDeviceManifest;
        return VintfObject::getDeviceHalManifest(skipCache);
    }

    std::shared_ptr<const HalManifest> getFrameworkHalManifest(bool skipCache = false) override {
        if (mFrameworkManifest != nullptr) return mFrameworkManifest;
        return VintfObject::getFrameworkHalManifest(skipCache);
    }

    std::shared_ptr<const CompatibilityMatrix> getDeviceCompatibilityMatrix(
        bool skipCache = false) override {
        if (mDeviceMatrix != nullptr) return mDeviceMatrix;
        return VintfObject::getDeviceCompatibilityMatrix(skipCache);
    }

    std::shared_ptr<const CompatibilityMatrix> getFrameworkCompatibilityMatrix(
        bool skipCache = false) override {
        if (mFrameworkMatrix != nullptr) return mFrameworkMatrix;
        return VintfObject::getFrameworkCompatibilityMatrix(skipCache);
    }

    const std::unique_ptr<FileSystem>& getFileSystem() override {
        return mDependency->getFileSystem();
    }

    const std::unique_ptr<PropertyFetcher>& getPropertyFetcher() override {
        return mDependency->getPropertyFetcher();
    }

    const std::unique_ptr<ObjectFactory<RuntimeInfo>>& getRuntimeInfoFactory() override {
        return mDependency->getRuntimeInfoFactory();
    }

    bool set(const std::shared_ptr<HalManifest>& o) {
        return set(o, &mDeviceManifest, &mFrameworkManifest);
    }

    bool set(const std::shared_ptr<CompatibilityMatrix>& o) {
        return set(o, &mDeviceMatrix, &mFrameworkMatrix);
    }

   private:
    VintfObject* mDependency = nullptr;
    std::shared_ptr<HalManifest> mDeviceManifest;
    std::shared_ptr<HalManifest> mFrameworkManifest;
    std::shared_ptr<CompatibilityMatrix> mDeviceMatrix;
    std::shared_ptr<CompatibilityMatrix> mFrameworkMatrix;

    template <typename T>
    bool set(const std::shared_ptr<T>& o, std::shared_ptr<T>* dev, std::shared_ptr<T>* fwk) {
        if (o->type() == SchemaType::DEVICE) {
            if (*dev != nullptr) return false;
            *dev = o;
            return true;
        } else if (o->type() == SchemaType::FRAMEWORK) {
            if (*fwk != nullptr) return false;
            *fwk = o;
            return true;
        }
        return false;
    }
};

}  // namespace details
}  // namespace vintf
}  // namespace android
