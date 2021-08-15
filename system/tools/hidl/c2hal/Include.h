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

#ifndef INCLUDE_H_
#define INCLUDE_H_

#include "Declaration.h"

#include <android-base/macros.h>
#include <android-base/logging.h>
#include <string>
#include <hidl-util/Formatter.h>

namespace android {

struct Include : Declaration {
    Include(const std::string &name, bool isLibrary);
    ~Include();

    static std::string type() { return "include"; }
    const std::string decType() const override { return type(); }

    void generateSource(Formatter &out) const override;
    void processContents(AST &ast) override;

    bool isLibrary() const;

private:
    const bool mIsLibrary;

    DISALLOW_COPY_AND_ASSIGN(Include);
};

}  // namespace android

#endif  // INCLUDE_H_