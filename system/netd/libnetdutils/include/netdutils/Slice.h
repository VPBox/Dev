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

#ifndef NETUTILS_SLICE_H
#define NETUTILS_SLICE_H

#include <algorithm>
#include <array>
#include <cstring>
#include <ostream>
#include <tuple>
#include <vector>

namespace android {
namespace netdutils {

// Immutable wrapper for a linear region of unowned bytes.
// Slice represents memory as a half-closed interval [base, limit).
//
// Note that without manually invoking the Slice() constructor, it is
// impossible to increase the size of a slice. This guarantees that
// applications that properly use the slice API will never access
// memory outside of a slice.
//
// Note that const Slice still wraps mutable memory, however copy
// assignment and move assignment to slice are disabled.
class Slice {
  public:
    Slice() = default;

    // Create a slice beginning at base and continuing to but not including limit
    Slice(void* base, void* limit) : mBase(toUint8(base)), mLimit(toUint8(limit)) {}

    // Create a slice beginning at base and continuing for size bytes
    Slice(void* base, size_t size) : Slice(base, toUint8(base) + size) {}

    // Return the address of the first byte in this slice
    uint8_t* base() const { return mBase; }

    // Return the address of the first byte following this slice
    uint8_t* limit() const { return mLimit; }

    // Return the size of this slice in bytes
    size_t size() const { return limit() - base(); }

    // Return true if size() == 0
    bool empty() const { return base() == limit(); }

  private:
    static uint8_t* toUint8(void* ptr) { return reinterpret_cast<uint8_t*>(ptr); }

    uint8_t* mBase = nullptr;
    uint8_t* mLimit = nullptr;
};

// Return slice representation of ref which must be a POD type
template <typename T>
inline const Slice makeSlice(const T& ref) {
    static_assert(std::is_pod<T>::value, "value must be a POD type");
    static_assert(!std::is_pointer<T>::value, "value must not be a pointer type");
    return {const_cast<T*>(&ref), sizeof(ref)};
}

// Return slice representation of string data()
inline const Slice makeSlice(const std::string& s) {
    using ValueT = std::string::value_type;
    return {const_cast<ValueT*>(s.data()), s.size() * sizeof(ValueT)};
}

// Return slice representation of vector data()
template <typename T>
inline const Slice makeSlice(const std::vector<T>& v) {
    return {const_cast<T*>(v.data()), v.size() * sizeof(T)};
}

// Return slice representation of array data()
template <typename U, size_t V>
inline const Slice makeSlice(const std::array<U, V>& a) {
    return {const_cast<U*>(a.data()), a.size() * sizeof(U)};
}

// Return prefix and suffix of Slice s ending and starting at position cut
inline std::pair<const Slice, const Slice> split(const Slice s, size_t cut) {
    const size_t tmp = std::min(cut, s.size());
    return {{s.base(), s.base() + tmp}, {s.base() + tmp, s.limit()}};
}

// Return prefix of Slice s ending at position cut
inline const Slice take(const Slice s, size_t cut) {
    return std::get<0>(split(s, cut));
}

// Return suffix of Slice s starting at position cut
inline const Slice drop(const Slice s, size_t cut) {
    return std::get<1>(split(s, cut));
}

// Copy from src into dst. Bytes copied is the lesser of dst.size() and src.size()
inline size_t copy(const Slice dst, const Slice src) {
    const auto min = std::min(dst.size(), src.size());
    memcpy(dst.base(), src.base(), min);
    return min;
}

// Base case for variadic extract below
template <typename Head>
inline size_t extract(const Slice src, Head& head) {
    return copy(makeSlice(head), src);
}

// Copy from src into one or more pointers to POD data.  If src.size()
// is less than the sum of all data pointers a suffix of data will be
// left unmodified. Return the number of bytes copied.
template <typename Head, typename... Tail>
inline size_t extract(const Slice src, Head& head, Tail&... tail) {
    const auto extracted = extract(src, head);
    return extracted + extract(drop(src, extracted), tail...);
}

// Return a string containing a copy of the contents of s
std::string toString(const Slice s);

// Return a string containing a hexadecimal representation of the contents of s.
// This function inserts a newline into its output every wrap bytes.
std::string toHex(const Slice s, int wrap = INT_MAX);

inline bool operator==(const Slice& lhs, const Slice& rhs) {
    return (lhs.base() == rhs.base()) && (lhs.limit() == rhs.limit());
}

inline bool operator!=(const Slice& lhs, const Slice& rhs) {
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const Slice& slice);

// Return suffix of Slice s starting at the first match of byte c. If no matched
// byte, return an empty Slice.
inline const Slice findFirstMatching(const Slice s, uint8_t c) {
    uint8_t* match = (uint8_t*)memchr(s.base(), c, s.size());
    if (!match) return Slice();
    return drop(s, match - s.base());
}

}  // namespace netdutils
}  // namespace android

#endif /* NETUTILS_SLICE_H */
