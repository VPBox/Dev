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

#ifndef ANDROID_VINTF_KERNEL_CONFIG_PARSER_H_
#define ANDROID_VINTF_KERNEL_CONFIG_PARSER_H_

#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include <utils/Errors.h>

namespace android {
namespace vintf {

class KernelConfigParser {
   public:
    KernelConfigParser(bool processComments = false, bool relaxedFormat = false);

    status_t process(const char* buf, size_t len);
    status_t finish();
    status_t processAndFinish(const char* buf, size_t len);
    status_t processAndFinish(const std::string& content);
    std::stringbuf* error() const;
    std::map<std::string, std::string>& configs();
    const std::map<std::string, std::string>& configs() const;

   private:
    status_t processRemaining();
    std::map<std::string, std::string> mConfigs;
    std::stringstream mError;
    std::string mRemaining;
    bool mProcessComments;
    bool mRelaxedFormat;
};

}  // namespace vintf
}  // namespace android

#endif  // ANDROID_VINTF_KERNEL_CONFIG_PARSER_H_
