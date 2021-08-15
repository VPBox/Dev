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

#ifndef NETUTILS_HANDLE_H
#define NETUTILS_HANDLE_H

#include <ostream>

namespace android {
namespace netdutils {

// Opaque, strongly typed wrapper for integer-like handles.
// Explicitly avoids implementing arithmetic operations.
//
// This class is intended to avoid common errors when reordering
// arguments to functions, typos and other cases where plain integer
// types would silently cover up the mistake.
//
// usage:
// DEFINE_HANDLE(ProductId, uint64_t);
// DEFINE_HANDLE(ThumbnailHash, uint64_t);
// void foo(ProductId p, ThumbnailHash th) {...}
//
// void test() {
//     ProductId p(88);
//     ThumbnailHash th1(100), th2(200);
//
//     foo(p, th1);        <- ok!
//     foo(th1, p);        <- disallowed!
//     th1 += 10;          <- disallowed!
//     p = th2;            <- disallowed!
//     assert(th1 != th2); <- ok!
// }
template <typename T, typename TagT>
class Handle {
  public:
    constexpr Handle() = default;
    constexpr Handle(const T& value) : mValue(value) {}

    const T get() const { return mValue; }

    bool operator==(const Handle& that) const { return get() == that.get(); }
    bool operator!=(const Handle& that) const { return get() != that.get(); }

  private:
    T mValue;
};

#define DEFINE_HANDLE(name, type) \
    struct _##name##Tag {};       \
    using name = ::android::netdutils::Handle<type, _##name##Tag>;

template <typename T, typename TagT>
inline std::ostream& operator<<(std::ostream& os, const Handle<T, TagT>& handle) {
    return os << handle.get();
}

}  // namespace netdutils
}  // namespace android

#endif /* NETUTILS_HANDLE_H */
