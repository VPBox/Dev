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

#include "AST.h"

#include <android-base/logging.h>
#include <hidl-util/Formatter.h>
#include <string>
#include <vector>

#include "NamedType.h"
#include "Type.h"

namespace android {

void AST::generateDependencies(Formatter& out) const {
    std::unordered_set<const Type*> visited;
    (void)mRootScope.recursivePass(
        Type::ParseStage::COMPLETED,
        [&](const Type* type) {
            if (type != &mRootScope && type->isNamedType()) {
                out << static_cast<const NamedType*>(type)->fqName().string() << "\n";
            }
            return OK;
        },
        &visited);
}

}  // namespace android
