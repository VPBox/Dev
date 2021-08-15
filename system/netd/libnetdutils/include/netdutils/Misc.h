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

#ifndef NETUTILS_MISC_H
#define NETUTILS_MISC_H

#include <map>

namespace android {
namespace netdutils {

// Lookup key in map, returing a default value if key is not found
template <typename U, typename V>
inline const V& findWithDefault(const std::map<U, V>& map, const U& key, const V& dflt) {
    auto it = map.find(key);
    return (it == map.end()) ? dflt : it->second;
}

// Movable, copiable, scoped lambda (or std::function) runner. Useful
// for running arbitrary cleanup or logging code when exiting a scope.
//
// Compare to defer in golang.
template <typename FnT>
class Cleanup {
  public:
    Cleanup() = delete;
    explicit Cleanup(FnT fn) : mFn(fn) {}
    ~Cleanup() { if (!mReleased) mFn(); }

    void release() { mReleased = true; }

  private:
    bool mReleased{false};
    FnT mFn;
};

// Helper to make a new Cleanup. Avoids complex or impossible syntax
// when wrapping lambdas.
//
// Usage:
// auto cleanup = makeCleanup([](){ your_code_here; });
template <typename FnT>
Cleanup<FnT> makeCleanup(FnT fn) {
    return Cleanup<FnT>(fn);
}

}  // namespace netdutils
}  // namespace android

#endif /* NETUTILS_MISC_H */
