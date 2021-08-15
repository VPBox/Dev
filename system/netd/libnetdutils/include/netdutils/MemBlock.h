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

#ifndef NETUTILS_MEMBLOCK_H
#define NETUTILS_MEMBLOCK_H

#include <memory>
#include "netdutils/Slice.h"

namespace android {
namespace netdutils {

// A class to encapsulate self-deleting byte arrays while preserving access
// to the underlying length (without the length being part of the type, e.g.
// std::array<>). By design, the only interface to the underlying bytes is
// via Slice, to encourage safer memory access usage.
//
// No thread-safety guarantees whatsoever.
class MemBlock {
  public:
    MemBlock() : MemBlock(0U) {}
    explicit MemBlock(size_t len)
            : mData((len > 0U) ? new uint8_t[len]{} : nullptr),
              mLen(len) {}
    // Allocate memory of size src.size() and copy src into this MemBlock.
    explicit MemBlock(Slice src) : MemBlock(src.size()) {
        copy(get(), src);
    }

    // No copy construction or assignment.
    MemBlock(const MemBlock&) = delete;
    MemBlock& operator=(const MemBlock&) = delete;

    // Move construction and assignment are okay.
    MemBlock(MemBlock&&) = default;
    MemBlock& operator=(MemBlock&&) = default;

    // Even though this method is const, the memory wrapped by the
    // returned Slice is mutable.
    Slice get() const noexcept { return Slice(mData.get(), mLen); }

    // Implicit cast to Slice.
    // NOLINTNEXTLINE(google-explicit-constructor)
    operator const Slice() const noexcept { return get(); }

  private:
    std::unique_ptr<uint8_t[]> mData;
    size_t mLen;
};

}  // namespace netdutils
}  // namespace android

#endif /* NETUTILS_MEMBLOCK_H */
