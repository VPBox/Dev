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

#include "Note.h"

namespace android {

Note::Note(const std::string &name)
    : Declaration(name)
    {}

Note::Note(Declaration *decl)
    : Declaration(""),
      mDecl(decl)
    {}

Note::~Note() {
    if(mDecl) {
        delete mDecl;
    }
}

void Note::generateSource(Formatter &out) const {
    out.setLinePrefix("//");
    out << "NOTE:\n";

    out.indent();
    if(mDecl) {
        mDecl->generateSource(out);
    } else {
        out << getName();
    }
    out.unindent();

    out.unsetLinePrefix();
    out << "\n";
}

void Note::processContents(AST &ast) {
    if (mDecl) {
        mDecl->processContents(ast);
    }
}

} //namespace android
