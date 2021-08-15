/*
 * Copyright 2014 The Android Open Source Project
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

#ifndef SYSTEM_KEYMASTER_ANDROID_KEYMASTER_UTILS_H_
#define SYSTEM_KEYMASTER_ANDROID_KEYMASTER_UTILS_H_

#include <stdint.h>
#ifndef  __clang__
// We need to diable foritfy level for memset in gcc because we want to use
// memset unoptimized. This would falsely trigger __warn_memset_zero_len in
// /usr/include/bits/string3.h. The inline checking function is only supposed to
// work when the optimization level is at least 1.
#pragma push_macro("__USE_FORTIFY_LEVEL")
#undef __USE_FORTIFY_LEVEL
#endif
#include <string.h>
#ifndef  __clang__
#pragma pop_macro("__USE_FORTIFY_LEVEL")
#endif
#include <time.h>  // for time_t.

#include <keymaster/UniquePtr.h>

#include <hardware/keymaster_defs.h>
#include <keymaster/serializable.h>

#ifndef __has_cpp_attribute
#define __has_cpp_attribute(x) 0
#endif

// Mark intentional fallthroughts in switch statements to silence
// -Wimplicit-fallthrough.
#if __has_cpp_attribute(clang::fallthrough)
#define FALLTHROUGH [[clang::fallthrough]]
#else
#define FALLTHROUGH
#endif

namespace keymaster {

/**
 * Convert the specified time value into "Java time", which is a signed 64-bit integer representing
 * elapsed milliseconds since Jan 1, 1970.
 */
inline int64_t java_time(time_t time) {
    // The exact meaning of a time_t value is implementation-dependent.  If this code is ported to a
    // platform that doesn't define it as "seconds since Jan 1, 1970 UTC", this function will have
    // to be revised.
    return static_cast<int64_t>(time) * 1000;
}

/*
 * Array Manipulation functions.  This set of templated inline functions provides some nice tools
 * for operating on c-style arrays.  C-style arrays actually do have a defined size associated with
 * them, as long as they are not allowed to decay to a pointer.  These template methods exploit this
 * to allow size-based array operations without explicitly specifying the size.  If passed a pointer
 * rather than an array, they'll fail to compile.
 */

/**
 * Return the size in bytes of the array \p a.
 */
template <typename T, size_t N> inline size_t array_size(const T (&a)[N]) {
    return sizeof(a);
}

/**
 * Return the number of elements in array \p a.
 */
template <typename T, size_t N> inline size_t array_length(const T (&)[N]) {
    return N;
}

/**
 * Duplicate the array \p a.  The memory for the new array is allocated and the caller takes
 * responsibility.
 */
template <typename T> inline T* dup_array(const T* a, size_t n) {
    T* dup = new (std::nothrow) T[n];
    if (dup)
        for (size_t i = 0; i < n; ++i)
            dup[i] = a[i];
    return dup;
}

/**
 * Duplicate the array \p a.  The memory for the new array is allocated and the caller takes
 * responsibility.  Note that the dup is necessarily returned as a pointer, so size is lost.  Call
 * array_length() on the original array to discover the size.
 */
template <typename T, size_t N> inline T* dup_array(const T (&a)[N]) {
    return dup_array(a, N);
}

/**
 * Duplicate the buffer \p buf.  The memory for the new buffer is allocated and the caller takes
 * responsibility.
 */
uint8_t* dup_buffer(const void* buf, size_t size);

/**
 * Copy the contents of array \p arr to \p dest.
 */
template <typename T, size_t N> inline void copy_array(const T (&arr)[N], T* dest) {
    for (size_t i = 0; i < N; ++i)
        dest[i] = arr[i];
}

/**
 * Search array \p a for value \p val, returning true if found.  Note that this function is
 * early-exit, meaning that it should not be used in contexts where timing analysis attacks could be
 * a concern.
 */
template <typename T, size_t N> inline bool array_contains(const T (&a)[N], T val) {
    for (size_t i = 0; i < N; ++i) {
        if (a[i] == val) {
            return true;
        }
    }
    return false;
}

/**
 * Variant of memset() that uses GCC-specific pragmas to disable optimizations, so effect is not
 * optimized away.  This is important because we often need to wipe blocks of sensitive data from
 * memory.  As an additional convenience, this implementation avoids writing to NULL pointers.
 */
#ifdef __clang__
#define OPTNONE __attribute__((optnone))
#else  // not __clang__
#define OPTNONE __attribute__((optimize("O0")))
#endif  // not __clang__
inline OPTNONE void* memset_s(void* s, int c, size_t n) {
    if (!s)
        return s;
    return memset(s, c, n);
}
#undef OPTNONE

/**
 * Variant of memcmp that has the same runtime regardless of whether the data matches (i.e. doesn't
 * short-circuit).  Not an exact equivalent to memcmp because it doesn't return <0 if p1 < p2, just
 * 0 for match and non-zero for non-match.
 */
int memcmp_s(const void* p1, const void* p2, size_t length);

/**
 * Eraser clears buffers.  Construct it with a buffer or object and the destructor will ensure that
 * it is zeroed.
 */
class Eraser {
  public:
    /* Not implemented.  If this gets used, we want a link error. */
    template <typename T> explicit Eraser(T* t);

    template <typename T>
    explicit Eraser(T& t) : buf_(reinterpret_cast<uint8_t*>(&t)), size_(sizeof(t)) {}

    template <size_t N> explicit Eraser(uint8_t (&arr)[N]) : buf_(arr), size_(N) {}

    Eraser(void* buf, size_t size) : buf_(static_cast<uint8_t*>(buf)), size_(size) {}
    ~Eraser() { memset_s(buf_, 0, size_); }

  private:
    Eraser(const Eraser&);
    void operator=(const Eraser&);

    uint8_t* buf_;
    size_t size_;
};

/**
 * ArrayWrapper is a trivial wrapper around a C-style array that provides begin() and end()
 * methods. This is primarily to facilitate range-based iteration on arrays.  It does not copy, nor
 * does it take ownership; it just holds pointers.
 */
template <typename T> class ArrayWrapper {
  public:
    ArrayWrapper(T* array, size_t size) : begin_(array), end_(array + size) {}

    T* begin() { return begin_; }
    T* end() { return end_; }

  private:
    T* begin_;
    T* end_;
};

template <typename T> ArrayWrapper<T> array_range(T* begin, size_t length) {
    return ArrayWrapper<T>(begin, length);
}

template <typename T, size_t n> ArrayWrapper<T> array_range(T (&a)[n]) {
    return ArrayWrapper<T>(a, n);
}

/**
 * Convert any unsigned integer from network to host order.  We implement this here rather than
 * using the functions from arpa/inet.h because the TEE doesn't have inet.h.  This isn't the most
 * efficient implementation, but the compiler should unroll the loop and tighten it up.
 */
template <typename T> T ntoh(T t) {
    const uint8_t* byte_ptr = reinterpret_cast<const uint8_t*>(&t);
    T retval = 0;
    for (size_t i = 0; i < sizeof(t); ++i) {
        retval <<= 8;
        retval |= byte_ptr[i];
    }
    return retval;
}

/**
 * Convert any unsigned integer from host to network order.  We implement this here rather than
 * using the functions from arpa/inet.h because the TEE doesn't have inet.h.  This isn't the most
 * efficient implementation, but the compiler should unroll the loop and tighten it up.
 */
template <typename T> T hton(T t) {
    T retval;
    uint8_t* byte_ptr = reinterpret_cast<uint8_t*>(&retval);
    for (size_t i = sizeof(t); i > 0; --i) {
        byte_ptr[i - 1] = t & 0xFF;
        t >>= 8;
    }
    return retval;
}

inline
const uint8_t* const & accessBlobData(const keymaster_key_blob_t* blob) {
    return blob->key_material;
}
inline
const uint8_t*& accessBlobData(keymaster_key_blob_t* blob) {
    return blob->key_material;
}
inline
const size_t& accessBlobSize(const keymaster_key_blob_t* blob) {
    return blob->key_material_size;
}
inline
size_t& accessBlobSize(keymaster_key_blob_t* blob) {
    return blob->key_material_size;
}

inline
const uint8_t* const & accessBlobData(const keymaster_blob_t* blob) {
    return blob->data;
}
inline
const uint8_t*& accessBlobData(keymaster_blob_t* blob) {
    return blob->data;
}
inline
const size_t & accessBlobSize(const keymaster_blob_t* blob) {
    return blob->data_length;
}
inline
size_t& accessBlobSize(keymaster_blob_t* blob) {
    return blob->data_length;
}

/**
 * TKeymasterBlob is a very simple extension of the C structs keymaster_blob_t and
 * keymaster_key_blob_t.  It manages its own memory, which makes avoiding memory leaks
 * much easier.
 */
template <typename BlobType>
struct TKeymasterBlob : public BlobType {
    TKeymasterBlob() {
        accessBlobData(this) = nullptr;
        accessBlobSize(this) = 0;
    }

    TKeymasterBlob(const uint8_t* data, size_t size) {
        accessBlobSize(this) = 0;
        accessBlobData(this) = dup_buffer(data, size);
        if (accessBlobData(this))
            accessBlobSize(this) = size;
    }

    explicit TKeymasterBlob(size_t size) {
        accessBlobSize(this) = 0;
        accessBlobData(this) = new (std::nothrow) uint8_t[size];
        if (accessBlobData(this))
            accessBlobSize(this) = size;
    }

    explicit TKeymasterBlob(const BlobType& blob) {
        accessBlobSize(this) = 0;
        accessBlobData(this) = dup_buffer(accessBlobData(&blob), accessBlobSize(&blob));
        if (accessBlobData(this))
            accessBlobSize(this) = accessBlobSize(&blob);
    }

    template<size_t N>
    explicit TKeymasterBlob(const uint8_t (&data)[N]) {
        accessBlobSize(this) = 0;
        accessBlobData(this) = dup_buffer(data, N);
        if (accessBlobData(this))
            accessBlobSize(this) = N;
    }

    TKeymasterBlob(const TKeymasterBlob& blob) {
        accessBlobSize(this) = 0;
        accessBlobData(this) = dup_buffer(accessBlobData(&blob), accessBlobSize(&blob));
        if (accessBlobData(this))
            accessBlobSize(this) = accessBlobSize(&blob);
    }

    TKeymasterBlob(TKeymasterBlob&& rhs) {
        accessBlobSize(this) = accessBlobSize(&rhs);
        accessBlobData(this) = accessBlobData(&rhs);
        accessBlobSize(&rhs) = 0;
        accessBlobData(&rhs) = nullptr;
    }

    TKeymasterBlob& operator=(const TKeymasterBlob& blob) {
        if (this != &blob) {
            Clear();
            accessBlobData(this) = dup_buffer(accessBlobData(&blob), accessBlobSize(&blob));
            accessBlobSize(this) = accessBlobSize(&blob);
        }
        return *this;
    }

    TKeymasterBlob& operator=(TKeymasterBlob&& rhs) {
        if (this != &rhs) {
            Clear();
            accessBlobSize(this) = accessBlobSize(&rhs);
            accessBlobData(this) = accessBlobData(&rhs);
            accessBlobSize(&rhs) = 0;
            accessBlobData(&rhs) = nullptr;
        }
        return *this;
    }

    ~TKeymasterBlob() { Clear(); }

    const uint8_t* begin() const { return accessBlobData(this); }
    const uint8_t* end() const { return accessBlobData(this) + accessBlobSize(this); }

    void Clear() {
        if (accessBlobSize(this)) {
            memset_s(const_cast<uint8_t*>(accessBlobData(this)), 0, accessBlobSize(this));
        }
        delete[] accessBlobData(this);
        accessBlobData(this) = nullptr;
        accessBlobSize(this) = 0;
    }

    const uint8_t* Reset(size_t new_size) {
        Clear();
        accessBlobData(this) = new (std::nothrow) uint8_t[new_size];
        if (accessBlobData(this))
            accessBlobSize(this) = new_size;
        return accessBlobData(this);
    }

    // The key_material in keymaster_key_blob_t is const, which is the right thing in most
    // circumstances, but occasionally we do need to write into it.  This method exposes a non-const
    // version of the pointer.  Use sparingly.
    uint8_t* writable_data() { return const_cast<uint8_t*>(accessBlobData(this)); }

    BlobType release() {
        BlobType tmp = {accessBlobData(this), accessBlobSize(this)};
        accessBlobData(this) = nullptr;
        accessBlobSize(this) = 0;
        return tmp;
    }

    size_t SerializedSize() const { return sizeof(uint32_t) + accessBlobSize(this); }
    uint8_t* Serialize(uint8_t* buf, const uint8_t* end) const {
        return append_size_and_data_to_buf(buf, end, accessBlobData(this), accessBlobSize(this));
    }

    bool Deserialize(const uint8_t** buf_ptr, const uint8_t* end) {
        Clear();
        UniquePtr<uint8_t[]> tmp;
        if (!copy_size_and_data_from_buf(buf_ptr, end, &accessBlobSize(this), &tmp)) {
            accessBlobData(this) = nullptr;
            accessBlobSize(this) = 0;
            return false;
        }
        accessBlobData(this) = tmp.release();
        return true;
    }
};

typedef TKeymasterBlob<keymaster_blob_t> KeymasterBlob;
typedef TKeymasterBlob<keymaster_key_blob_t> KeymasterKeyBlob;

struct Characteristics_Delete {
    void operator()(keymaster_key_characteristics_t* p) {
        keymaster_free_characteristics(p);
        free(p);
    }
};

struct Malloc_Delete {
    void operator()(void* p) { free(p); }
};

struct CertificateChainDelete {
    void operator()(keymaster_cert_chain_t* p) {
        if (!p)
            return;
        for (size_t i = 0; i < p->entry_count; ++i)
            delete[] p->entries[i].data;
        delete[] p->entries;
        delete p;
    }
};

typedef UniquePtr<keymaster_cert_chain_t, CertificateChainDelete> CertChainPtr;

keymaster_error_t EcKeySizeToCurve(uint32_t key_size_bits, keymaster_ec_curve_t* curve);
keymaster_error_t EcCurveToKeySize(keymaster_ec_curve_t curve, uint32_t* key_size_bits);

template<typename T> struct remove_reference      {typedef T type;};
template<typename T> struct remove_reference<T&>  {typedef T type;};
template<typename T> struct remove_reference<T&&> {typedef T type;};
template<typename T>
using remove_reference_t = typename remove_reference<T>::type;
template<typename T>
remove_reference_t<T>&& move(T&& x) {
    return static_cast<remove_reference_t<T>&&>(x);
}

template<typename T>
constexpr T&& forward(remove_reference_t<T>& x) {
    return static_cast<T&&>(x);
}
template<typename T>
constexpr T&& forward(remove_reference_t<T>&& x) {
    return static_cast<T&&>(x);
}

template <class F> class final_action {
  public:
    explicit final_action(F f) : f_(move(f)) {}
    ~final_action() { f_(); }

  private:
    F f_;
};

template <class F> inline final_action<F> finally(const F& f) {
    return final_action<F>(f);
}

}  // namespace keymaster

#endif  // SYSTEM_KEYMASTER_ANDROID_KEYMASTER_UTILS_H_
