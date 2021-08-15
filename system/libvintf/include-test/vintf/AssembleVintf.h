/*
 * Copyright (C) 2017 The Android Open Source Project
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

#ifndef ANDROID_VINTF_ASSEMBLE_VINTF_H
#define ANDROID_VINTF_ASSEMBLE_VINTF_H

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <vintf/Version.h>

namespace android {
namespace vintf {

class AssembleVintf {
   public:
    using Ostream = std::unique_ptr<std::ostream>;
    using Istream = std::unique_ptr<std::istream>;

    static std::unique_ptr<AssembleVintf> newInstance();

    virtual ~AssembleVintf() = default;
    virtual bool setHalsOnly() = 0;
    virtual bool setNoHals() = 0;
    virtual bool setNoKernelRequirements() = 0;
    virtual void setOutputMatrix() = 0;
    virtual bool assemble() = 0;

    bool openOutFile(const std::string& path);
    bool openInFile(const std::string& path);
    bool openCheckFile(const std::string& path);
    bool addKernel(const std::string& kernelArg);

    virtual std::ostream& setOutputStream(Ostream&&) = 0;
    virtual std::istream& addInputStream(const std::string& name, Istream&&) = 0;
    virtual std::istream& setCheckInputStream(Istream&&) = 0;
    virtual std::istream& addKernelConfigInputStream(const KernelVersion& kernelVer,
                                                     const std::string& name, Istream&& in) = 0;
    virtual void setFakeEnv(const std::string& key, const std::string& value) = 0;

   protected:
    virtual bool hasKernelVersion(const KernelVersion&) const = 0;
    virtual std::string getEnv(const std::string& key) const = 0;
};

}  // namespace vintf
}  // namespace android
#endif  // ANDROID_VINTF_ASSEMBLE_VINTF_H
