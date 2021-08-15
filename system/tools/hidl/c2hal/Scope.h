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

#ifndef SCOPE_H_
#define SCOPE_H_

#include <android-base/macros.h>
#include <android-base/logging.h>
#include <string>
#include <vector>
#include <map>

namespace android {

/* This class is used to represent declarations or notes
 * which are otherwise not included in a HIDL HAL
 */
template<typename T>
struct Scope {
    Scope() {}
    ~Scope() {}

    void enter(std::string name, T item);
    void leave(std::string name);

    T lookup(std::string name) const;

private:
    std::map<std::string, T> mScopeContents;

    DISALLOW_COPY_AND_ASSIGN(Scope);
};

template<typename T>
void Scope<T>::enter(std::string name, T item) {
    auto it = mScopeContents.find(name);

    if (it != mScopeContents.end()) {
        LOG(WARNING) << "Redeclaring variable in scope: " << name;
        return;
    }

    mScopeContents[name] = item;
}

template<typename T>
void Scope<T>::leave(std::string name) {
    auto it = mScopeContents.find(name);

    if (it == mScopeContents.end()) {
        LOG(WARNING) << "Tried to undefined already undefined value in scope: " << name;
        return;
    }

    mScopeContents.erase(it);
}

template<typename T>
T Scope<T>::lookup(std::string name) const {
    auto it = mScopeContents.find(name);

    if (it == mScopeContents.end()) {
        return NULL;
    }

    return (*it).second;
}

}  // namespace android

#endif  // SCOPE_H_