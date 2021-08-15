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

#ifndef NOTE_DECLARATION_H_
#define NOTE_DECLARATION_H_

#include <android-base/macros.h>
#include <android-base/logging.h>
#include <string>
#include <vector>
#include "Declaration.h"

namespace android {

/* This class is used to represent declarations or notes
 * which are otherwise not included in a HIDL HAL
 */
struct Note : Declaration {
    Note(const std::string &name);
    // assume ownership on decl
    Note(Declaration *decl);
    ~Note();

    static std::string type() { return "note"; }
    const std::string decType() const override { return type(); }

    void generateSource(Formatter &out) const override;
    void processContents(AST &ast) override;

private:
    Declaration *mDecl = nullptr;

    DISALLOW_COPY_AND_ASSIGN(Note);
};

}  // namespace android

#endif  // NOTE_DECLARATION_H_
