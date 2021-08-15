/*
 * Copyright 2017 The Android Open Source Project
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

#include <gtest/gtest.h>

#include <C2AllocatorIon.h>
#include <C2AllocatorGralloc.h>
#include <C2Buffer.h>
#include <C2BufferPriv.h>
#include <C2ParamDef.h>

#include <system/graphics.h>

namespace android {

class C2BufferUtilsTest : public ::testing::Test {
    static void StaticSegmentTest() {
        // constructor
        static_assert(C2Segment(123u, 456u).offset == 123, "");
        static_assert(C2Segment(123u, 456u).size == 456, "");

        // empty
        static_assert(!C2Segment(123u, 456u).isEmpty(), "");
        static_assert(C2Segment(123u, 0u).isEmpty(), "");

        // valid
        static_assert(C2Segment(123u, 456u).isValid(), "");
        static_assert(C2Segment(123u, ~123u).isValid(), "");
        static_assert(!C2Segment(123u, 1 + ~123u).isValid(), "");

        // bool()
        static_assert(C2Segment(123u, 456u), "");
        static_assert((bool)C2Segment(123u, ~123u), "");
        static_assert(!bool(C2Segment(123u, 1 + ~123u)), "");
        static_assert(!bool(C2Segment(123u, 0)), "");

        // !
        static_assert(!!C2Segment(123u, 456u), "");
        static_assert(!!C2Segment(123u, ~123u), "");
        static_assert(!C2Segment(123u, 1 + ~123u), "");
        static_assert(!C2Segment(123u, 0), "");

        // contains
        static_assert(C2Segment(123u, ~123u).contains(C2Segment(123u, 0)), "");
        static_assert(!C2Segment(123u, 1 + ~123u).contains(C2Segment(123u, 0)), "");
        static_assert(C2Segment(123u, ~123u).contains(C2Segment(123u, ~123u)), "");
        static_assert(!C2Segment(123u, ~123u).contains(C2Segment(123u, 1 + ~123u)), "");
        static_assert(!C2Segment(123u, 1 + ~123u).contains(C2Segment(123u, 1 + ~123u)), "");
        static_assert(!C2Segment(123u, ~123u).contains(C2Segment(122u, 2u)), "");
        static_assert(C2Segment(123u, ~123u).contains(C2Segment(123u, 2u)), "");
        static_assert(C2Segment(123u, 3u).contains(C2Segment(124u, 2u)), "");
        static_assert(!C2Segment(123u, 3u).contains(C2Segment(125u, 2u)), "");

        // ==
        static_assert(C2Segment(123u, 456u) == C2Segment(123u, 456u), "");
        static_assert(!(C2Segment(123u, 456u) == C2Segment(123u, 457u)), "");
        static_assert(!(C2Segment(123u, 456u) == C2Segment(123u, 455u)), "");
        static_assert(!(C2Segment(123u, 456u) == C2Segment(122u, 456u)), "");
        static_assert(!(C2Segment(123u, 456u) == C2Segment(124u, 456u)), "");
        static_assert(!(C2Segment(123u, 0u) == C2Segment(124u, 0u)), "");
        static_assert(C2Segment(123u, 0u) == C2Segment(123u, 0u), "");
        static_assert(C2Segment(123u, 1 + ~123u) == C2Segment(124u, 1 + ~124u), "");

        // !=
        static_assert(!(C2Segment(123u, 456u) != C2Segment(123u, 456u)), "");
        static_assert(C2Segment(123u, 456u) != C2Segment(123u, 457u), "");
        static_assert(C2Segment(123u, 456u) != C2Segment(123u, 455u), "");
        static_assert(C2Segment(123u, 456u) != C2Segment(122u, 456u), "");
        static_assert(C2Segment(123u, 456u) != C2Segment(124u, 456u), "");
        static_assert(C2Segment(123u, 0u) != C2Segment(124u, 0u), "");
        static_assert(!(C2Segment(123u, 0u) != C2Segment(123u, 0u)), "");
        static_assert(!(C2Segment(123u, 1 + ~123u) != C2Segment(124u, 1 + ~124u)), "");

        // <=
        static_assert(C2Segment(123u, 456u) <= C2Segment(123u, 456u), "");
        static_assert(C2Segment(123u, 456u) <= C2Segment(123u, 457u), "");
        static_assert(C2Segment(123u, 456u) <= C2Segment(122u, 457u), "");
        static_assert(!(C2Segment(123u, 457u) <= C2Segment(123u, 456u)), "");
        static_assert(!(C2Segment(122u, 457u) <= C2Segment(123u, 456u)), "");
        static_assert(!(C2Segment(123u, 457u) <= C2Segment(124u, 457u)), "");
        static_assert(!(C2Segment(122u, 457u) <= C2Segment(123u, 457u)), "");
        static_assert(!(C2Segment(122u, 0u) <= C2Segment(123u, 0u)), "");
        static_assert(C2Segment(123u, 0u) <= C2Segment(122u, 1u), "");
        static_assert(C2Segment(122u, 0u) <= C2Segment(122u, 1u), "");
        static_assert(!(C2Segment(122u, ~122u) <= C2Segment(122u, 1 + ~122u)), "");
        static_assert(!(C2Segment(122u, 1 + ~122u) <= C2Segment(122u, ~122u)), "");
        static_assert(!(C2Segment(122u, 1 + ~122u) <= C2Segment(122u, 1 + ~122u)), "");

        // <
        static_assert(!(C2Segment(123u, 456u) < C2Segment(123u, 456u)), "");
        static_assert(C2Segment(123u, 456u) < C2Segment(123u, 457u), "");
        static_assert(C2Segment(123u, 456u) < C2Segment(122u, 457u), "");
        static_assert(!(C2Segment(123u, 457u) < C2Segment(123u, 456u)), "");
        static_assert(!(C2Segment(122u, 457u) < C2Segment(123u, 456u)), "");
        static_assert(!(C2Segment(123u, 457u) < C2Segment(124u, 457u)), "");
        static_assert(!(C2Segment(122u, 457u) < C2Segment(123u, 457u)), "");
        static_assert(!(C2Segment(122u, 0u) < C2Segment(123u, 0u)), "");
        static_assert(C2Segment(123u, 0u) < C2Segment(122u, 1u), "");
        static_assert(C2Segment(122u, 0u) < C2Segment(122u, 1u), "");
        static_assert(!(C2Segment(122u, ~122u) < C2Segment(122u, 1 + ~122u)), "");
        static_assert(!(C2Segment(122u, 1 + ~122u) < C2Segment(122u, ~122u)), "");
        static_assert(!(C2Segment(122u, 1 + ~122u) < C2Segment(122u, 1 + ~122u)), "");

        // <=
        static_assert(C2Segment(123u, 456u) >= C2Segment(123u, 456u), "");
        static_assert(C2Segment(123u, 457u) >= C2Segment(123u, 456u), "");
        static_assert(C2Segment(122u, 457u) >= C2Segment(123u, 456u), "");
        static_assert(!(C2Segment(123u, 456u) >= C2Segment(123u, 457u)), "");
        static_assert(!(C2Segment(123u, 456u) >= C2Segment(122u, 457u)), "");
        static_assert(!(C2Segment(124u, 457u) >= C2Segment(123u, 457u)), "");
        static_assert(!(C2Segment(123u, 457u) >= C2Segment(122u, 457u)), "");
        static_assert(!(C2Segment(123u, 0u) >= C2Segment(122u, 0u)), "");
        static_assert(C2Segment(122u, 1u) >= C2Segment(123u, 0u), "");
        static_assert(C2Segment(122u, 1u) >= C2Segment(122u, 0u), "");
        static_assert(!(C2Segment(122u, 1 + ~122u) >= C2Segment(122u, ~122u)), "");
        static_assert(!(C2Segment(122u, ~122u) >= C2Segment(122u, 1 + ~122u)), "");
        static_assert(!(C2Segment(122u, 1 + ~122u) >= C2Segment(122u, 1 + ~122u)), "");

        // <
        static_assert(!(C2Segment(123u, 456u) > C2Segment(123u, 456u)), "");
        static_assert(C2Segment(123u, 457u) > C2Segment(123u, 456u), "");
        static_assert(C2Segment(122u, 457u) > C2Segment(123u, 456u), "");
        static_assert(!(C2Segment(123u, 456u) > C2Segment(123u, 457u)), "");
        static_assert(!(C2Segment(123u, 456u) > C2Segment(122u, 457u)), "");
        static_assert(!(C2Segment(124u, 457u) > C2Segment(123u, 457u)), "");
        static_assert(!(C2Segment(123u, 457u) > C2Segment(122u, 457u)), "");
        static_assert(!(C2Segment(123u, 0u) > C2Segment(122u, 0u)), "");
        static_assert(C2Segment(122u, 1u) > C2Segment(123u, 0u), "");
        static_assert(C2Segment(122u, 1u) > C2Segment(122u, 0u), "");
        static_assert(!(C2Segment(122u, 1 + ~122u) > C2Segment(122u, ~122u)), "");
        static_assert(!(C2Segment(122u, ~122u) > C2Segment(122u, 1 + ~122u)), "");
        static_assert(!(C2Segment(122u, 1 + ~122u) > C2Segment(122u, 1 + ~122u)), "");

        // end
        static_assert(C2Segment(123u, 456u).end() == 579u, "");
        static_assert(C2Segment(123u, 0u).end() == 123u, "");
        static_assert(C2Segment(123u, ~123u).end() == 0xffffffffu, "");
        static_assert(C2Segment(123u, 1 + ~123u).end() == 0u, "");

        // intersect
        static_assert(C2Segment(123u, 456u).intersect(C2Segment(123u, 456u)) == C2Segment(123u, 456u), "");
        static_assert(C2Segment(123u, 456u).intersect(C2Segment(123u, 460u)) == C2Segment(123u, 456u), "");
        static_assert(C2Segment(123u, 456u).intersect(C2Segment(124u, 460u)) == C2Segment(124u, 455u), "");
        static_assert(C2Segment(123u, 456u).intersect(C2Segment(579u, 460u)) == C2Segment(579u, 0u), "");
        static_assert(C2Segment(123u, 456u).intersect(C2Segment(589u, 460u)) == C2Segment(589u, ~9u /* -10 */), "");
        static_assert(C2Segment(123u, 456u).intersect(C2Segment(123u, 455u)) == C2Segment(123u, 455u), "");
        static_assert(C2Segment(123u, 456u).intersect(C2Segment(122u, 456u)) == C2Segment(123u, 455u), "");
        static_assert(C2Segment(123u, 456u).intersect(C2Segment(0u, 123u)) == C2Segment(123u, 0u), "");
        static_assert(C2Segment(123u, 456u).intersect(C2Segment(0u, 0u)) == C2Segment(123u, ~122u /* -123 */), "");

        // normalize (change invalid segments to empty segments)
        static_assert(C2Segment(123u, 456u).normalize() == C2Segment(123u, 456u), "");
        static_assert(C2Segment(123u, ~123u).normalize() == C2Segment(123u, ~123u), "");
        static_assert(C2Segment(123u, 1 + ~123u).normalize() == C2Segment(123u, 0u), "");

        // note: normalize cannot be used to make this work
        static_assert(C2Segment(123u, 456u).intersect(C2Segment(150u, ~150u)).normalize() == C2Segment(150u, 429u), "");
        static_assert(C2Segment(123u, 456u).intersect(C2Segment(150u, 1 + ~150u)).normalize() != C2Segment(150u, 429u), "");
        static_assert(C2Segment(123u, 456u).intersect(C2Segment(150u, 1 + ~150u)).normalize() == C2Segment(150u, 0u), "");

        // saturate (change invalid segments to full segments up to max)
        static_assert(C2Segment(123u, 456u).saturate() == C2Segment(123u, 456u), "");
        static_assert(C2Segment(123u, ~123u).saturate() == C2Segment(123u, ~123u), "");
        static_assert(C2Segment(123u, 1 + ~123u).saturate() == C2Segment(123u, ~123u), "");

        // note: saturate can be used to make this work but only partially
        static_assert(C2Segment(123u, 456u).intersect(C2Segment(150u, 1 + ~150u).saturate()).normalize() == C2Segment(150u, 429u), "");
        static_assert(C2Segment(123u, 456u).intersect(C2Segment(0u, 100u).saturate()).normalize() == C2Segment(123u, 0u), "");
        static_assert(C2Segment(123u, 456u).intersect(C2Segment(1000u, 100u).saturate()).normalize() != C2Segment(579u, 0u), "");
        static_assert(C2Segment(123u, 456u).intersect(C2Segment(1000u, 100u).saturate()).normalize() == C2Segment(1000u, 0u), "");

    }

    static void StaticLinearRangeAndCapacityTest() {
        class TestCapacity : public _C2LinearCapacityAspect {
            using _C2LinearCapacityAspect::_C2LinearCapacityAspect;
            friend class C2BufferUtilsTest;
        };

        class TestRange : public _C2LinearRangeAspect {
            using _C2LinearRangeAspect::_C2LinearRangeAspect;
            friend class C2BufferUtilsTest;
        };

        // _C2LinearCapacityAspect
        static_assert(TestCapacity(0u).capacity() == 0u, "");
        constexpr TestCapacity cap(123u);
        static_assert(TestCapacity(&cap).capacity() == 123u, "");
        static_assert(TestCapacity(nullptr).capacity() == 0u, "");

        // _C2LinearCapacityRange
        static_assert(TestRange(&cap).capacity() == 123u, "");
        static_assert(TestRange(&cap).offset() == 0u, "");
        static_assert(TestRange(&cap).size() == 123u, "");
        static_assert(TestRange(&cap).endOffset() == 123u, "");

        constexpr TestRange range(&cap, 50u, 100u);

        static_assert(range.capacity() == 123u, "");
        static_assert(range.offset() == 50u, "");
        static_assert(range.size() == 73u, "");
        static_assert(range.endOffset() == 123u, "");

        static_assert(TestRange(&cap, 20u, 30u).capacity() == 123u, "");
        static_assert(TestRange(&cap, 20u, 30u).offset() == 20u, "");
        static_assert(TestRange(&cap, 20u, 30u).size() == 30u, "");
        static_assert(TestRange(&cap, 20u, 30u).endOffset() == 50u, "");

        static_assert(TestRange(&cap, 200u, 30u).capacity() == 123u, "");
        static_assert(TestRange(&cap, 200u, 30u).offset() == 123u, "");
        static_assert(TestRange(&cap, 200u, 30u).size() == 0u, "");
        static_assert(TestRange(&cap, 200u, 30u).endOffset() == 123u, "");

    }

};


class C2BufferTest : public ::testing::Test {
public:
    C2BufferTest()
        : mBlockPoolId(C2BlockPool::PLATFORM_START),
          mLinearAllocator(std::make_shared<C2AllocatorIon>('i')),
          mSize(0u),
          mAddr(nullptr),
          mGraphicAllocator(std::make_shared<C2AllocatorGralloc>('g')) {
    }

    ~C2BufferTest() = default;

    void allocateLinear(size_t capacity) {
        c2_status_t err = mLinearAllocator->newLinearAllocation(
                capacity,
                { C2MemoryUsage::CPU_READ, C2MemoryUsage::CPU_WRITE },
                &mLinearAllocation);
        if (err != C2_OK) {
            mLinearAllocation.reset();
            FAIL() << "C2Allocator::newLinearAllocation() failed: " << err;
        }
    }

    void mapLinear(size_t offset, size_t size, uint8_t **addr) {
        ASSERT_TRUE(mLinearAllocation);
        c2_status_t err = mLinearAllocation->map(
                offset,
                size,
                { C2MemoryUsage::CPU_READ, C2MemoryUsage::CPU_WRITE },
                // TODO: fence
                nullptr,
                &mAddr);
        if (err != C2_OK) {
            mAddr = nullptr;
            FAIL() << "C2LinearAllocation::map() failed: " << err;
        }
        ASSERT_NE(nullptr, mAddr);
        mSize = size;
        *addr = (uint8_t *)mAddr;
    }

    void unmapLinear() {
        ASSERT_TRUE(mLinearAllocation);
        ASSERT_NE(nullptr, mAddr);
        ASSERT_NE(0u, mSize);

        // TODO: fence
        ASSERT_EQ(C2_OK, mLinearAllocation->unmap(mAddr, mSize, nullptr));
        mSize = 0u;
        mAddr = nullptr;
    }

    std::shared_ptr<C2BlockPool> makeLinearBlockPool() {
        return std::make_shared<C2PooledBlockPool>(mLinearAllocator, mBlockPoolId++);
    }

    void allocateGraphic(uint32_t width, uint32_t height) {
        c2_status_t err = mGraphicAllocator->newGraphicAllocation(
                width,
                height,
                HAL_PIXEL_FORMAT_YCBCR_420_888,
                { C2MemoryUsage::CPU_READ, C2MemoryUsage::CPU_WRITE },
                &mGraphicAllocation);
        if (err != C2_OK) {
            mGraphicAllocation.reset();
            FAIL() << "C2Allocator::newGraphicAllocation() failed: " << err;
        }
    }

    void mapGraphic(C2Rect rect, C2PlanarLayout *layout, uint8_t **addr) {
        ASSERT_TRUE(mGraphicAllocation);
        c2_status_t err = mGraphicAllocation->map(
                rect,
                { C2MemoryUsage::CPU_READ, C2MemoryUsage::CPU_WRITE },
                // TODO: fence
                nullptr,
                layout,
                addr);
        if (err != C2_OK) {
            addr[C2PlanarLayout::PLANE_Y] = nullptr;
            addr[C2PlanarLayout::PLANE_U] = nullptr;
            addr[C2PlanarLayout::PLANE_V] = nullptr;
            FAIL() << "C2GraphicAllocation::map() failed: " << err;
        }
        mMappedRect = rect;
        memcpy(mAddrGraphic, addr, sizeof(uint8_t*) * C2PlanarLayout::MAX_NUM_PLANES);
    }

    void unmapGraphic() {
        ASSERT_TRUE(mGraphicAllocation);

        // TODO: fence
        ASSERT_EQ(C2_OK, mGraphicAllocation->unmap(mAddrGraphic, mMappedRect, nullptr));
    }

    std::shared_ptr<C2BlockPool> makeGraphicBlockPool() {
        return std::make_shared<C2BasicGraphicBlockPool>(mGraphicAllocator);
    }

private:
    C2BlockPool::local_id_t mBlockPoolId;
    std::shared_ptr<C2Allocator> mLinearAllocator;
    std::shared_ptr<C2LinearAllocation> mLinearAllocation;
    size_t mSize;
    void *mAddr;
    C2Rect mMappedRect;
    uint8_t* mAddrGraphic[C2PlanarLayout::MAX_NUM_PLANES];

    std::shared_ptr<C2Allocator> mGraphicAllocator;
    std::shared_ptr<C2GraphicAllocation> mGraphicAllocation;
};

TEST_F(C2BufferTest, LinearAllocationTest) {
    constexpr size_t kCapacity = 1024u * 1024u;

    allocateLinear(kCapacity);

    uint8_t *addr = nullptr;
    mapLinear(0u, kCapacity, &addr);
    ASSERT_NE(nullptr, addr);

    for (size_t i = 0; i < kCapacity; ++i) {
        addr[i] = i % 100u;
    }

    unmapLinear();
    addr = nullptr;

    mapLinear(kCapacity / 3, kCapacity / 3, &addr);
    ASSERT_NE(nullptr, addr);
    for (size_t i = 0; i < kCapacity / 3; ++i) {
        ASSERT_EQ((i + kCapacity / 3) % 100, addr[i]) << " at i = " << i;
    }
}

TEST_F(C2BufferTest, BlockPoolTest) {
    constexpr size_t kCapacity = 1024u * 1024u;

    std::shared_ptr<C2BlockPool> blockPool(makeLinearBlockPool());

    std::shared_ptr<C2LinearBlock> block;
    ASSERT_EQ(C2_OK, blockPool->fetchLinearBlock(
            kCapacity,
            { C2MemoryUsage::CPU_READ, C2MemoryUsage::CPU_WRITE },
            &block));
    ASSERT_TRUE(block);

    C2Acquirable<C2WriteView> writeViewHolder = block->map();
    C2WriteView writeView = writeViewHolder.get();
    ASSERT_EQ(C2_OK, writeView.error());
    ASSERT_EQ(kCapacity, writeView.capacity());
    ASSERT_EQ(0u, writeView.offset());
    ASSERT_EQ(kCapacity, writeView.size());

    uint8_t *data = writeView.data();
    ASSERT_NE(nullptr, data);
    for (size_t i = 0; i < writeView.size(); ++i) {
        data[i] = i % 100u;
    }

    writeView.setOffset(kCapacity / 3);
    data = writeView.data();
    ASSERT_NE(nullptr, data);
    for (size_t i = 0; i < writeView.size(); ++i) {
        ASSERT_EQ((i + kCapacity / 3) % 100u, data[i]) << " at i = " << i
            << "; data = " << static_cast<void *>(data);
    }

    C2Fence fence;
    C2ConstLinearBlock constBlock = block->share(
            kCapacity / 3, kCapacity / 3, fence);

    C2Acquirable<C2ReadView> readViewHolder = constBlock.map();
    C2ReadView readView = readViewHolder.get();
    ASSERT_EQ(C2_OK, readView.error());
    ASSERT_EQ(kCapacity / 3, readView.capacity());

    // TODO: fence
    const uint8_t *constData = readView.data();
    ASSERT_NE(nullptr, constData);
    for (size_t i = 0; i < readView.capacity(); ++i) {
        ASSERT_EQ((i + kCapacity / 3) % 100u, constData[i]) << " at i = " << i
                << "; data = " << static_cast<void *>(data)
                << "; constData = " << static_cast<const void *>(constData);
    }

    readView = readView.subView(333u, 100u);
    ASSERT_EQ(C2_OK, readView.error());
    ASSERT_EQ(100u, readView.capacity());

    constData = readView.data();
    ASSERT_NE(nullptr, constData);
    for (size_t i = 0; i < readView.capacity(); ++i) {
        ASSERT_EQ((i + 333u + kCapacity / 3) % 100u, constData[i]) << " at i = " << i;
    }
}

void fillPlane(const C2Rect rect, const C2PlaneInfo info, uint8_t *addr, uint8_t value) {
    for (uint32_t row = 0; row < rect.height / info.rowSampling; ++row) {
        int32_t rowOffset = (row + rect.top / info.rowSampling) * info.rowInc;
        for (uint32_t col = 0; col < rect.width / info.colSampling; ++col) {
            int32_t colOffset = (col + rect.left / info.colSampling) * info.colInc;
            addr[rowOffset + colOffset] = value;
        }
    }
}

bool verifyPlane(const C2Rect rect, const C2PlaneInfo info, const uint8_t *addr, uint8_t value) {
    for (uint32_t row = 0; row < rect.height / info.rowSampling; ++row) {
        int32_t rowOffset = (row + rect.top / info.rowSampling) * info.rowInc;
        for (uint32_t col = 0; col < rect.width / info.colSampling; ++col) {
            int32_t colOffset = (col + rect.left / info.colSampling) * info.colInc;
            if (addr[rowOffset + colOffset] != value) {
                return false;
            }
        }
    }
    return true;
}

TEST_F(C2BufferTest, GraphicAllocationTest) {
    constexpr uint32_t kWidth = 320;
    constexpr uint32_t kHeight = 240;

    allocateGraphic(kWidth, kHeight);

    uint8_t *addr[C2PlanarLayout::MAX_NUM_PLANES];
    C2Rect rect(kWidth, kHeight);
    C2PlanarLayout layout;
    mapGraphic(rect, &layout, addr);
    ASSERT_NE(nullptr, addr[C2PlanarLayout::PLANE_Y]);
    ASSERT_NE(nullptr, addr[C2PlanarLayout::PLANE_U]);
    ASSERT_NE(nullptr, addr[C2PlanarLayout::PLANE_V]);

    uint8_t *y = addr[C2PlanarLayout::PLANE_Y];
    C2PlaneInfo yInfo = layout.planes[C2PlanarLayout::PLANE_Y];
    uint8_t *u = addr[C2PlanarLayout::PLANE_U];
    C2PlaneInfo uInfo = layout.planes[C2PlanarLayout::PLANE_U];
    uint8_t *v = addr[C2PlanarLayout::PLANE_V];
    C2PlaneInfo vInfo = layout.planes[C2PlanarLayout::PLANE_V];

    fillPlane(rect, yInfo, y, 0);
    fillPlane(rect, uInfo, u, 0);
    fillPlane(rect, vInfo, v, 0);
    fillPlane(C2Rect(kWidth / 2, kHeight / 2).at(kWidth / 4, kHeight / 4), yInfo, y, 0x12);
    fillPlane(C2Rect(kWidth / 2, kHeight / 2).at(kWidth / 4, kHeight / 4), uInfo, u, 0x34);
    fillPlane(C2Rect(kWidth / 2, kHeight / 2).at(kWidth / 4, kHeight / 4), vInfo, v, 0x56);

    unmapGraphic();

    mapGraphic(rect, &layout, addr);
    ASSERT_NE(nullptr, addr[C2PlanarLayout::PLANE_Y]);
    ASSERT_NE(nullptr, addr[C2PlanarLayout::PLANE_U]);
    ASSERT_NE(nullptr, addr[C2PlanarLayout::PLANE_V]);

    y = addr[C2PlanarLayout::PLANE_Y];
    yInfo = layout.planes[C2PlanarLayout::PLANE_Y];
    u = addr[C2PlanarLayout::PLANE_U];
    uInfo = layout.planes[C2PlanarLayout::PLANE_U];
    v = addr[C2PlanarLayout::PLANE_V];
    vInfo = layout.planes[C2PlanarLayout::PLANE_V];

    ASSERT_TRUE(verifyPlane(C2Rect(kWidth / 2, kHeight / 2).at(kWidth / 4, kHeight / 4), yInfo, y, 0x12));
    ASSERT_TRUE(verifyPlane(C2Rect(kWidth / 2, kHeight / 2).at(kWidth / 4, kHeight / 4), uInfo, u, 0x34));
    ASSERT_TRUE(verifyPlane(C2Rect(kWidth / 2, kHeight / 2).at(kWidth / 4, kHeight / 4), vInfo, v, 0x56));
    ASSERT_TRUE(verifyPlane({ kWidth, kHeight / 4 }, yInfo, y, 0));
    ASSERT_TRUE(verifyPlane({ kWidth, kHeight / 4 }, uInfo, u, 0));
    ASSERT_TRUE(verifyPlane({ kWidth, kHeight / 4 }, vInfo, v, 0));
    ASSERT_TRUE(verifyPlane({ kWidth / 4, kHeight }, yInfo, y, 0));
    ASSERT_TRUE(verifyPlane({ kWidth / 4, kHeight }, uInfo, u, 0));
    ASSERT_TRUE(verifyPlane({ kWidth / 4, kHeight }, vInfo, v, 0));
}

TEST_F(C2BufferTest, GraphicBlockPoolTest) {
    constexpr uint32_t kWidth = 320;
    constexpr uint32_t kHeight = 240;

    std::shared_ptr<C2BlockPool> blockPool(makeGraphicBlockPool());

    std::shared_ptr<C2GraphicBlock> block;
    ASSERT_EQ(C2_OK, blockPool->fetchGraphicBlock(
            kWidth,
            kHeight,
            HAL_PIXEL_FORMAT_YCBCR_420_888,
            { C2MemoryUsage::CPU_READ, C2MemoryUsage::CPU_WRITE },
            &block));
    ASSERT_TRUE(block);

    C2Acquirable<C2GraphicView> graphicViewHolder = block->map();
    C2GraphicView graphicView = graphicViewHolder.get();
    ASSERT_EQ(C2_OK, graphicView.error());
    ASSERT_EQ(kWidth, graphicView.width());
    ASSERT_EQ(kHeight, graphicView.height());

    uint8_t *const *data = graphicView.data();
    C2PlanarLayout layout = graphicView.layout();
    ASSERT_NE(nullptr, data);

    uint8_t *y = data[C2PlanarLayout::PLANE_Y];
    C2PlaneInfo yInfo = layout.planes[C2PlanarLayout::PLANE_Y];
    uint8_t *u = data[C2PlanarLayout::PLANE_U];
    C2PlaneInfo uInfo = layout.planes[C2PlanarLayout::PLANE_U];
    uint8_t *v = data[C2PlanarLayout::PLANE_V];
    C2PlaneInfo vInfo = layout.planes[C2PlanarLayout::PLANE_V];

    fillPlane({ kWidth, kHeight }, yInfo, y, 0);
    fillPlane({ kWidth, kHeight }, uInfo, u, 0);
    fillPlane({ kWidth, kHeight }, vInfo, v, 0);
    fillPlane(C2Rect(kWidth / 2, kHeight / 2).at(kWidth / 4, kHeight / 4), yInfo, y, 0x12);
    fillPlane(C2Rect(kWidth / 2, kHeight / 2).at(kWidth / 4, kHeight / 4), uInfo, u, 0x34);
    fillPlane(C2Rect(kWidth / 2, kHeight / 2).at(kWidth / 4, kHeight / 4), vInfo, v, 0x56);

    C2Fence fence;
    C2ConstGraphicBlock constBlock = block->share(C2Rect(kWidth, kHeight), fence);
    block.reset();

    C2Acquirable<const C2GraphicView> constViewHolder = constBlock.map();
    const C2GraphicView constGraphicView = constViewHolder.get();
    ASSERT_EQ(C2_OK, constGraphicView.error());
    ASSERT_EQ(kWidth, constGraphicView.width());
    ASSERT_EQ(kHeight, constGraphicView.height());

    const uint8_t *const *constData = constGraphicView.data();
    layout = graphicView.layout();
    ASSERT_NE(nullptr, constData);

    const uint8_t *cy = constData[C2PlanarLayout::PLANE_Y];
    yInfo = layout.planes[C2PlanarLayout::PLANE_Y];
    const uint8_t *cu = constData[C2PlanarLayout::PLANE_U];
    uInfo = layout.planes[C2PlanarLayout::PLANE_U];
    const uint8_t *cv = constData[C2PlanarLayout::PLANE_V];
    vInfo = layout.planes[C2PlanarLayout::PLANE_V];

    ASSERT_TRUE(verifyPlane(C2Rect(kWidth / 2, kHeight / 2).at(kWidth / 4, kHeight / 4), yInfo, cy, 0x12));
    ASSERT_TRUE(verifyPlane(C2Rect(kWidth / 2, kHeight / 2).at(kWidth / 4, kHeight / 4), uInfo, cu, 0x34));
    ASSERT_TRUE(verifyPlane(C2Rect(kWidth / 2, kHeight / 2).at(kWidth / 4, kHeight / 4), vInfo, cv, 0x56));
    ASSERT_TRUE(verifyPlane({ kWidth, kHeight / 4 }, yInfo, cy, 0));
    ASSERT_TRUE(verifyPlane({ kWidth, kHeight / 4 }, uInfo, cu, 0));
    ASSERT_TRUE(verifyPlane({ kWidth, kHeight / 4 }, vInfo, cv, 0));
    ASSERT_TRUE(verifyPlane({ kWidth / 4, kHeight }, yInfo, cy, 0));
    ASSERT_TRUE(verifyPlane({ kWidth / 4, kHeight }, uInfo, cu, 0));
    ASSERT_TRUE(verifyPlane({ kWidth / 4, kHeight }, vInfo, cv, 0));
}

class BufferData : public C2BufferData {
public:
    explicit BufferData(const std::vector<C2ConstLinearBlock> &blocks) : C2BufferData(blocks) {}
    explicit BufferData(const std::vector<C2ConstGraphicBlock> &blocks) : C2BufferData(blocks) {}
};

class Buffer : public C2Buffer {
public:
    explicit Buffer(const std::vector<C2ConstLinearBlock> &blocks) : C2Buffer(blocks) {}
    explicit Buffer(const std::vector<C2ConstGraphicBlock> &blocks) : C2Buffer(blocks) {}
};

TEST_F(C2BufferTest, BufferDataTest) {
    std::shared_ptr<C2BlockPool> linearBlockPool(makeLinearBlockPool());
    std::shared_ptr<C2BlockPool> graphicBlockPool(makeGraphicBlockPool());

    constexpr uint32_t kWidth1 = 320;
    constexpr uint32_t kHeight1 = 240;
    constexpr C2Rect kCrop1(kWidth1, kHeight1);
    constexpr uint32_t kWidth2 = 176;
    constexpr uint32_t kHeight2 = 144;
    constexpr C2Rect kCrop2(kWidth2, kHeight2);
    constexpr size_t kCapacity1 = 1024u;
    constexpr size_t kCapacity2 = 2048u;

    std::shared_ptr<C2LinearBlock> linearBlock1;
    std::shared_ptr<C2LinearBlock> linearBlock2;
    ASSERT_EQ(C2_OK, linearBlockPool->fetchLinearBlock(
            kCapacity1,
            { C2MemoryUsage::CPU_READ, C2MemoryUsage::CPU_WRITE },
            &linearBlock1));
    ASSERT_EQ(C2_OK, linearBlockPool->fetchLinearBlock(
            kCapacity2,
            { C2MemoryUsage::CPU_READ, C2MemoryUsage::CPU_WRITE },
            &linearBlock2));
    std::shared_ptr<C2GraphicBlock> graphicBlock1;
    std::shared_ptr<C2GraphicBlock> graphicBlock2;
    ASSERT_EQ(C2_OK, graphicBlockPool->fetchGraphicBlock(
            kWidth1,
            kHeight1,
            HAL_PIXEL_FORMAT_YCBCR_420_888,
            { C2MemoryUsage::CPU_READ, C2MemoryUsage::CPU_WRITE },
            &graphicBlock1));
    ASSERT_EQ(C2_OK, graphicBlockPool->fetchGraphicBlock(
            kWidth2,
            kHeight2,
            HAL_PIXEL_FORMAT_YCBCR_420_888,
            { C2MemoryUsage::CPU_READ, C2MemoryUsage::CPU_WRITE },
            &graphicBlock2));

    std::shared_ptr<C2BufferData> data(new BufferData({ linearBlock1->share(0, kCapacity1, C2Fence()) }));
    EXPECT_EQ(C2BufferData::LINEAR, data->type());
    ASSERT_EQ(1u, data->linearBlocks().size());
    EXPECT_EQ(linearBlock1->handle(), data->linearBlocks().front().handle());
    EXPECT_TRUE(data->graphicBlocks().empty());

    data.reset(new BufferData({
        linearBlock1->share(0, kCapacity1, C2Fence()),
        linearBlock2->share(0, kCapacity2, C2Fence()),
    }));
    EXPECT_EQ(C2BufferData::LINEAR_CHUNKS, data->type());
    ASSERT_EQ(2u, data->linearBlocks().size());
    EXPECT_EQ(linearBlock1->handle(), data->linearBlocks().front().handle());
    EXPECT_EQ(linearBlock2->handle(), data->linearBlocks().back().handle());
    EXPECT_TRUE(data->graphicBlocks().empty());

    data.reset(new BufferData({ graphicBlock1->share(kCrop1, C2Fence()) }));
    EXPECT_EQ(C2BufferData::GRAPHIC, data->type());
    ASSERT_EQ(1u, data->graphicBlocks().size());
    EXPECT_EQ(graphicBlock1->handle(), data->graphicBlocks().front().handle());
    EXPECT_TRUE(data->linearBlocks().empty());

    data.reset(new BufferData({
        graphicBlock1->share(kCrop1, C2Fence()),
        graphicBlock2->share(kCrop2, C2Fence()),
    }));
    EXPECT_EQ(C2BufferData::GRAPHIC_CHUNKS, data->type());
    ASSERT_EQ(2u, data->graphicBlocks().size());
    EXPECT_EQ(graphicBlock1->handle(), data->graphicBlocks().front().handle());
    EXPECT_EQ(graphicBlock2->handle(), data->graphicBlocks().back().handle());
    EXPECT_TRUE(data->linearBlocks().empty());
}

void DestroyCallback(const C2Buffer * /* buf */, void *arg) {
    std::function<void(void)> *cb = (std::function<void(void)> *)arg;
    (*cb)();
}

enum : uint32_t {
    kParamIndexNumber1,
    kParamIndexNumber2,
};

typedef C2GlobalParam<C2Info, C2Int32Value, kParamIndexNumber1> C2Number1Info;
typedef C2GlobalParam<C2Info, C2Int32Value, kParamIndexNumber2> C2Number2Info;

TEST_F(C2BufferTest, BufferTest) {
    std::shared_ptr<C2BlockPool> alloc(makeLinearBlockPool());
    constexpr size_t kCapacity = 1024u;
    std::shared_ptr<C2LinearBlock> block;

    ASSERT_EQ(C2_OK, alloc->fetchLinearBlock(
            kCapacity,
            { C2MemoryUsage::CPU_READ, C2MemoryUsage::CPU_WRITE },
            &block));

    std::atomic_bool destroyed(false);
    std::function<void(void)> arg = [&destroyed](){ destroyed = true; };

    std::shared_ptr<C2Buffer> buffer(new Buffer( { block->share(0, kCapacity, C2Fence()) }));
    ASSERT_EQ(C2_OK, buffer->registerOnDestroyNotify(&DestroyCallback, &arg));
    EXPECT_FALSE(destroyed);
    ASSERT_EQ(C2_DUPLICATE, buffer->registerOnDestroyNotify(&DestroyCallback, &arg));
    buffer.reset();
    EXPECT_TRUE(destroyed);

    buffer.reset(new Buffer( { block->share(0, kCapacity, C2Fence()) }));
    destroyed = false;
    ASSERT_EQ(C2_OK, buffer->registerOnDestroyNotify(&DestroyCallback, &arg));
    EXPECT_FALSE(destroyed);
    ASSERT_EQ(C2_NOT_FOUND, buffer->unregisterOnDestroyNotify(&DestroyCallback, nullptr));
    ASSERT_EQ(C2_OK, buffer->unregisterOnDestroyNotify(&DestroyCallback, &arg));
    EXPECT_FALSE(destroyed);
    ASSERT_EQ(C2_NOT_FOUND, buffer->unregisterOnDestroyNotify(&DestroyCallback, &arg));
    buffer.reset();
    EXPECT_FALSE(destroyed);

    std::shared_ptr<C2Info> info1(new C2Number1Info(1));
    std::shared_ptr<C2Info> info2(new C2Number2Info(2));
    buffer.reset(new Buffer( { block->share(0, kCapacity, C2Fence()) }));
    EXPECT_TRUE(buffer->info().empty());
    EXPECT_FALSE(buffer->hasInfo(info1->type()));
    EXPECT_FALSE(buffer->hasInfo(info2->type()));

    ASSERT_EQ(C2_OK, buffer->setInfo(info1));
    EXPECT_EQ(1u, buffer->info().size());
    EXPECT_EQ(*info1, *buffer->info().front());
    EXPECT_TRUE(buffer->hasInfo(info1->type()));
    EXPECT_FALSE(buffer->hasInfo(info2->type()));

    ASSERT_EQ(C2_OK, buffer->setInfo(info2));
    EXPECT_EQ(2u, buffer->info().size());
    EXPECT_TRUE(buffer->hasInfo(info1->type()));
    EXPECT_TRUE(buffer->hasInfo(info2->type()));

    std::shared_ptr<C2Info> removed = buffer->removeInfo(info1->type());
    ASSERT_TRUE(removed);
    EXPECT_EQ(*removed, *info1);
    EXPECT_EQ(1u, buffer->info().size());
    EXPECT_EQ(*info2, *buffer->info().front());
    EXPECT_FALSE(buffer->hasInfo(info1->type()));
    EXPECT_TRUE(buffer->hasInfo(info2->type()));

    removed = buffer->removeInfo(info1->type());
    ASSERT_FALSE(removed);
    EXPECT_EQ(1u, buffer->info().size());
    EXPECT_FALSE(buffer->hasInfo(info1->type()));
    EXPECT_TRUE(buffer->hasInfo(info2->type()));

    std::shared_ptr<C2Info> info3(new C2Number2Info(3));
    ASSERT_EQ(C2_OK, buffer->setInfo(info3));
    EXPECT_EQ(1u, buffer->info().size());
    EXPECT_FALSE(buffer->hasInfo(info1->type()));
    EXPECT_TRUE(buffer->hasInfo(info2->type()));

    removed = buffer->removeInfo(info2->type());
    ASSERT_TRUE(removed);
    EXPECT_EQ(*info3, *removed);
    EXPECT_TRUE(buffer->info().empty());
    EXPECT_FALSE(buffer->hasInfo(info1->type()));
    EXPECT_FALSE(buffer->hasInfo(info2->type()));
}

TEST_F(C2BufferTest, MultipleLinearMapTest) {
    std::shared_ptr<C2BlockPool> pool(makeLinearBlockPool());
    constexpr size_t kCapacity = 524288u;
    for (int i = 0; i < 100; ++i) {
        std::vector<C2WriteView> wViews;
        std::vector<C2ReadView> rViews;
        for (int j = 0; j < 16; ++j) {
            std::shared_ptr<C2LinearBlock> block;
            ASSERT_EQ(C2_OK, pool->fetchLinearBlock(
                    kCapacity,
                    { C2MemoryUsage::CPU_READ, C2MemoryUsage::CPU_WRITE },
                    &block));
            wViews.push_back(block->map().get());
            C2ConstLinearBlock cBlock = block->share(0, kCapacity / 2, C2Fence());
            rViews.push_back(cBlock.map().get());
        }
    }
}

} // namespace android
