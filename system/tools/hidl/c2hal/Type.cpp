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

#include "Type.h"
#include <sstream>

#include <hidl-util/StringHelper.h>

namespace android {

Type::Type(std::vector<Qualifier*> *qualifiers)
    : mQualifiers(qualifiers)
    {}

Type::~Type() {
    if(mArrays != nullptr) {
        for(auto* array : *mArrays) {
            delete array;
        }
    }

    if(mQualifiers != nullptr) {
        for(auto* qual : *mQualifiers) {
            delete qual;
        }
    }
    delete mQualifiers;}


void Type::setArrays(std::vector<Expression*> *arrays) {
    mArrays = arrays;
}

const std::string Type::decorateName(const std::string &name) const {
    std::stringstream ss;

    std::string special = getSpecialTypeName();

    if(special.empty()) {
        ss << getHidlType();
    } else {
        ss << special;
    }

    ss << " " << name;

    return ss.str();
}

std::map<std::string, std::string> Type::kSignedToUnsignedMap = {
    { "char", "uint8_t" },
    { "short", "uint16_t" },
    { "int", "uint32_t" },
    { "long", "uint64_t" },
    { "int8_t", "uint8_t" },
    { "int16_t", "uint16_t" },
    { "int32_t", "uint32_t" },
    { "int64_t", "uint64_t" },
};

const std::string Type::signedToUnsigned(const std::string &signedType) {
    auto it = kSignedToUnsignedMap.find(signedType);

    if (it == kCToHidlMap.end()) {
        return "";
    }

    return (*it).second;
}

std::map<std::string, std::string> Type::kCToHidlMap = {
    { "char", "int8_t /* NOTE: char */" },
    { "short", "int16_t" },
    { "int", "int32_t" },
    { "long", "int64_t"},
    { "native_handle_t", "handle" },
    { "size_t", "uint64_t" },
    { "int8_t", "int8_t" },
    { "uint8_t", "uint8_t" },
    { "int16_t", "int16_t" },
    { "uint16_t", "uint16_t" },
    { "int32_t", "int32_t" },
    { "uint32_t", "uint32_t" },
    { "int64_t", "int64_t" },
    { "uint64_t", "uint64_t" },
    { "float", "float" },
    { "double", "double" },
    { "bool", "bool" },
    { "wchar_t", "int32_t /* NOTE: wchar_t */"},
    // { "hidl_string", "string" },
    // { "hidl_vec", "vec"},
};

const std::string Type::cToHidlType(const std::string &cType) {
    auto it = kCToHidlMap.find(cType);

    if (it == kCToHidlMap.end()) {
        return "";
    }

    return (*it).second;
}

const std::string Type::getHidlType() const {
    if (mQualifiers == nullptr) {
        return "";
    }

    std::stringstream ss;

    for (auto it = mQualifiers->begin(); it != mQualifiers->end(); ++it) {
        if (it != mQualifiers->begin()) {
            ss << " ";
        }

        switch((*it)->qualification) {
            case Type::Qualifier::STRUCT:
            case Type::Qualifier::UNION:
            case Type::Qualifier::ENUM:
            case Type::Qualifier::POINTER:
            case Type::Qualifier::CONST: {
                ss << "/* "
                   << Type::qualifierText((*it)->qualification)
                   << " */";
                break;
            }
            case Type::Qualifier::ID: {
                std::string id = (*it)->id;
                std::string conversion = cToHidlType(id);
                if (!conversion.empty()) {
                    ss << conversion;
                } else {
                    std::string baseName = StringHelper::RTrim(id, "_t");
                    ss << StringHelper::ToPascalCase(baseName);
                }
                break;
            }
            case Type::Qualifier::GENERICS: {
                ss << "<"
                   << (*it)->generics->decorateName("")
                   << ">";
                break;
            }
            case Type::Qualifier::UNSIGNED: {
                auto next = it + 1;
                if (next == mQualifiers->end()) {
                    ss << "uint32_t"; // 'unsigned a' -> 'uint32_t a'
                    break;
                }
                std::string unsignedType = signedToUnsigned((*next)->id);
                if(unsignedType.empty()) {
                    ss << Type::qualifierText((*it)->qualification);
                } else {
                    ss << unsignedType;
                    ++it;
                }
                break;
            }
            default: {
                ss << Type::qualifierText((*it)->qualification);
            }
        }
    }

    if (mArrays != nullptr) {
        for (const auto &array : *mArrays) {
            ss << "[" << array->toString() << "]";
        }
    }

    return ss.str();
}

const std::string Type::getRawQualifierList() const {
    if (mQualifiers == nullptr) {
        return "";
    }

    std::stringstream ss;

    for(auto* qualifier : *mQualifiers) {
        ss << Type::qualifierText(qualifier->qualification) << " ";
    }

    return ss.str();
}

const std::string Type::getSpecialTypeName() const {
    // this makes for a relatively expensive comparison, but it is
    // readable until the converstion get nailed down.
    std::string qualifiers = getRawQualifierList();

    if (qualifiers == "const ID * " ||
        qualifiers == "ID * ") {

        std::string id = mQualifiers->at(mQualifiers->size() - 2)->id;

        if (id == "char") {
            return "string";
        } else {
            // can't tell if it's a hidl_vec or a pointer
            // return "vec<" + id + ">";
            return "";
        }
    }

    return "";
}

bool Type::isVoid() const {
    if (mQualifiers->size() == 0) {
        return true;
    }

    return mQualifiers->size() == 1 &&
           (*mQualifiers)[0]->qualification == Type::Qualifier::VOID;
}

bool Type::isHwDevice() const {
    if (mQualifiers->size() < 2) {
        return false;
    }

    return (*mQualifiers)[0]->qualification == Type::Qualifier::STRUCT &&
        (*mQualifiers)[1]->qualification == Type::Qualifier::ID &&
        (*mQualifiers)[1]->id == "hw_device_t";
}

std::string Type::removeLastId() {
    if(mQualifiers == nullptr || mQualifiers->size() == 0) {
        return "";
    }

    Qualifier *last = (*mQualifiers)[mQualifiers->size() - 1];

    if(last == nullptr || last->qualification != Qualifier::ID) {
        return "";
    }

    std::string ret{last->id};

    mQualifiers->erase(mQualifiers->end() - 1);

    return ret;
}

} //namespace android
