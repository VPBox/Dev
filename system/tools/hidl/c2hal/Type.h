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

#ifndef TYPE_H_
#define TYPE_H_

#include "Expression.h"

#include <android-base/macros.h>
#include <android-base/logging.h>
#include <string>
#include <vector>
#include <map>

namespace android {

struct Type {

    struct Qualifier {

        enum Qualification {
            NONE = 0,
            STRUCT,
            UNION,
            SIGNED,
            UNSIGNED,
            VOID,
            POINTER,
            CONST,
            GENERICS,
            ID,
            ENUM
        } qualification;

        union {
            std::string id;
            Type *generics;
        };

        Qualifier(Qualification qualification)
        : qualification(qualification) {}
        Qualifier(Qualification qualification, std::string id)
        : qualification(qualification), id(id) {}
        Qualifier(Qualification qualification, Type* generics)
        : qualification(qualification), generics(generics) {}

        ~Qualifier() {
            if (qualification == GENERICS) {
                delete generics;
            }
        }
    };

    Type(std::vector<Qualifier*> *qualifiers);
    ~Type();

    static std::string qualifierText(Qualifier::Qualification qual) {
        switch(qual) {
            case Qualifier::STRUCT: return "struct";
            case Qualifier::UNION: return "union";
            case Qualifier::ENUM: return "enum";
            case Qualifier::SIGNED: return "signed";
            case Qualifier::UNSIGNED: return "unsigned";
            case Qualifier::VOID: return "void";
            case Qualifier::POINTER: return "*";
            case Qualifier::CONST: return "const";
            case Qualifier::ID: return "ID";
            case Qualifier::NONE: return "";
            default: return "/* UNKNOWN TYPE QUALIFIER */";
        }
    }

    void setArrays(std::vector<Expression *> *arrays);

    const std::string decorateName(const std::string &name) const;

    bool isVoid() const;
    bool isHwDevice() const;
    std::string removeLastId();

private:

    static std::map<std::string, std::string> kSignedToUnsignedMap;
    static const std::string signedToUnsigned(const std::string &signedType);

    static std::map<std::string, std::string> kCToHidlMap;
    static const std::string cToHidlType(const std::string &cType);

    const std::string getHidlType() const;

    const std::string getRawQualifierList() const;
    const std::string getSpecialTypeName() const;

    std::vector<Qualifier*> *mQualifiers = nullptr;

    /* [ expression ] [ expression ] ... [ expression ] */
    std::vector<Expression*> *mArrays = nullptr;

    DISALLOW_COPY_AND_ASSIGN(Type);
};

}  // namespace android

#endif  // TYPE_H_
