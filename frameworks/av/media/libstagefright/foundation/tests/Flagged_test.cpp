/*
 * Copyright 2016 The Android Open Source Project
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

//#define LOG_NDEBUG 0
#define LOG_TAG "Flagged_test"

#include <gtest/gtest.h>

#include <media/stagefright/foundation/Flagged.h>

namespace android {

/**
 * Helper template that can be used to print values in static_assert error messages.
 *
 * Use integers here.
 */
template<bool, int ...N>
struct _print_as_warning { };

template<int ...N>
struct _print_as_warning<true, N...> : std::true_type { };

#define static_assert_equals(a, b, msg) \
static_assert(_print_as_warning<(a) == (b), a, b>::value, msg)

class FlaggedTest : public ::testing::Test {
protected:
    // empty structs
    struct A0 { };
    struct A1 { };
    struct A_A0 : public A0 { };

    // simple struct
    struct BB {
        int32_t i;
        uint32_t u;
    };

    // struct inheriting from A0
    struct BB_A0 : public A0 {
        int32_t i;
        uint32_t u;
    };

    // struct inheriting from struct inheriting A0
    struct BB_AA0 : public A_A0 {
        int32_t i;
        uint32_t u;
    };

    // struct that wraps
    struct WBBA0 {
        BB_A0 b;
    };

    struct WBBA0_A1 : public A1 {
        BB_A0 b;
    };

    struct WBBA0_A0 : public A0 {
        BB_A0 b;
    };

    struct WBB_A0 : public A0 {
        BB b;
    };

    struct WBBA0_AA0 : public A_A0 {
        BB_A0 b;
    };

    struct WBBAA0_A0 : public A0 {
        BB_AA0 b;
    };

    struct WWBBA0_A0 : public A0 {
        WBBA0 b;
    };
};

/**
 * This test is here to confirm the handling of wrapping classes that inherit from an interface
 * while also inheriting from that same interface. While we no longer use this construct, we want
 * to track if this defect is ever fixed.
 */
TEST_F(FlaggedTest, StaticSanityTests) {
    static_assert(sizeof(A0) == 1, "");
    static_assert(sizeof(A1) == 1, "");
    static_assert(sizeof(A_A0) == 1, "");

    static constexpr size_t size = sizeof(BB); // original [pair]

    // inheriting from A0 does not increase size
    static_assert(sizeof(BB_A0) == size, ""); // [pair]:A0
    static_assert(sizeof(BB_AA0) == size, ""); // [pair]:[:A0]

     // wrapping a class that inherits from A0 does not increase size
    static_assert(sizeof(WBBA0) == size, ""); // [ [pair]:[:A0] ]

    // wrapping a class that inherits from A0 while also inheriting from A1 does not increase size
    static_assert(sizeof(WBBA0_A1) == size, ""); // [ [pair]:A0 ]:A1

    // wrapping a class that inherits from A0 while also inheriting from A0 DOES increase size
    EXPECT_GT(sizeof(WBBA0_A0), size); // [ [pair]:A0 ]:A0

    // wrapping a class that does not inherit from A0 while inheriting from A0 does not increase
    // size
    static_assert(sizeof(WBB_A0) == size, ""); // [[pair]]:A0

    // wrapping a class that inherits from A0 while also inheriting from a class that inherits
    // from A0 does increase size
    EXPECT_GT(sizeof(WBBA0_AA0), size); // [ [pair]:A0 ]:[:A0]

    // wrapping a class that indirectly inherits from A0 while also inheriting from A0 does
    // increase size
    EXPECT_GT(sizeof(WBBAA0_A0), size); // [ [pair]:[:A0] ]:A0

    // wrapping a class that inherits from A0 while also inheriting A0 does increase size
    EXPECT_GT(sizeof(WWBBA0_A0), size); //  [ [pair]:A0 ]:A0
}

enum FLAG : int32_t {
    kMask0 = 0x0FF,
    kFlag0_A = 0x0AA,
    kFlag0_B = 0x0BB,
    kFlag0_C = 0x0CC,
    kMask1 = 0xFF0,
    kFlag1_A = 0xAA0,
    kFlag1_B = 0xBB0,
    kFlag1_C = 0xCC0,
    kMaskCommon = 0x0F0,
};

TEST_F(FlaggedTest, BasicExample) {
    enum SafeFlags : uint32_t {
      kUnsafe,
      kSafe,
      kSafeMask = _Flagged_helper::minMask(kSafe),
    };
    typedef Flagged<int32_t, SafeFlags, kSafeMask> safeInt32;

    safeInt32 a(kUnsafe);
    a.setFlags(kSafe);
    a.get() = 15;
    EXPECT_EQ(a.flags(), kSafe);
    EXPECT_EQ(a.get(), 15);

    enum OriginFlags : uint32_t {
      kUnknown,
      kConst,
      kCalculated,
      kComponent,
      kApplication,
      kFile,
      kBinder,
      kOriginMask = _Flagged_helper::minMask(kBinder),
    };
    typedef Flagged<safeInt32, OriginFlags, kOriginMask>
             trackedSafeInt32;

    static_assert(sizeof(trackedSafeInt32) == sizeof(safeInt32), "");

    trackedSafeInt32 b(kConst, kSafe, 1);
    EXPECT_EQ(b.flags(), kConst);
    EXPECT_EQ(b.get().flags(), kSafe);
    EXPECT_EQ(b.get().get(), 1);
    b.setFlags(kCalculated);
    volatile bool overflow = true;
    b.get().setFlags(overflow ? kUnsafe : kSafe);

    enum ValidatedFlags : uint32_t {
      kUnsafeV = kUnsafe,
      kSafeV = kSafe,
      kValidated = kSafe | 2,
      kSharedMaskV = kSafeMask,
      kValidatedMask = _Flagged_helper::minMask(kValidated),
    };
    typedef Flagged<safeInt32, ValidatedFlags, kValidatedMask, kSharedMaskV> validatedInt32;

    validatedInt32 v(kUnsafeV, kSafe, 10);
    EXPECT_EQ(v.flags(), kUnsafeV);
    EXPECT_EQ(v.get().flags(), kUnsafe);  // !kUnsafeV overrides kSafe
    EXPECT_EQ(v.get().get(), 10);
    v.setFlags(kValidated);
    EXPECT_EQ(v.flags(), kValidated);
    EXPECT_EQ(v.get().flags(), kSafe);
    v.get().setFlags(kUnsafe);
    EXPECT_EQ(v.flags(), 2);  // NOTE: sharing masks with enums allows strange situations to occur
}

TEST_F(FlaggedTest, _Flagged_helper_Test) {
    using helper = _Flagged_helper;

    using i32 = int32_t;
    using u32 = uint32_t;
    using u8 = uint8_t;

    // base2
    static_assert(Flagged<i32, u32, 0u, 0u, 0>::sFlagMask == 0u, "");
    static_assert(Flagged<i32, u32, 0u, 0u, 0>::sFlagShift == 0, "");
    static_assert(Flagged<i32, u32, 0u, 0u, 0>::sEffectiveMask == 0u, "");

    static_assert(Flagged<i32, u32, 0u, 0u, 10>::sFlagMask == 0u, "");
    static_assert(Flagged<i32, u32, 0u, 0u, 10>::sFlagShift == 10, "");
    static_assert(Flagged<i32, u32, 0u, 0u, 10>::sEffectiveMask == 0u, "");

    static_assert(Flagged<i32, u32, 0u, 0u, -1>::sFlagMask == 0u, "");
    static_assert(Flagged<i32, u32, 0u, 0u, -1>::sFlagShift == 0, "");
    static_assert(Flagged<i32, u32, 0u, 0u, -1>::sEffectiveMask == 0u, "");

    static_assert(Flagged<i32, u32, 99u, 0u, 0>::sFlagMask == 99u, "");
    static_assert(Flagged<i32, u32, 99u, 0u, 0>::sFlagShift == 0, "");
    static_assert(Flagged<i32, u32, 99u, 0u, 0>::sEffectiveMask == 99u, "");

    static_assert(Flagged<i32, u32, 0x99u, 0u, 12>::sFlagMask == 0x99u, "");
    static_assert(Flagged<i32, u32, 0x99u, 0u, 12>::sFlagShift == 12, "");
    static_assert(Flagged<i32, u32, 0x99u, 0u, 12>::sEffectiveMask == 0x99000u, "");

    static_assert(Flagged<i32, u32, 99u, 0u, -1>::sFlagMask == 99u, "");
    static_assert(Flagged<i32, u32, 99u, 0u, -1>::sFlagShift == 0, "");
    static_assert(Flagged<i32, u32, 99u, 0u, -1>::sEffectiveMask == 99u, "");

    // mask_of<T, Flag>
    // also Flagged<> no default
    typedef Flagged<i32, u32, 0x800F /* mask */, 0 /* shared mask */, 0 /* shift */> i32_800f_0;
    typedef Flagged<i32, u32, 0x800F /* mask */, 0 /* shared mask */, 4 /* shift */> i32_800f_4;
    // this also tests that these types can be instantiated
    static_assert(sizeof(i32_800f_0) >= sizeof(i32) + sizeof(u32),
                  "should be at least size of component types");
    static_assert(sizeof(i32_800f_4) == sizeof(i32_800f_0), "regardless of shift");
    static_assert(!i32_800f_0::sFlagCombined, "");
    static_assert(!i32_800f_4::sFlagCombined, "");

    static_assert(helper::mask_of<i32_800f_0, u32>::value == 0x800F, "incorrect mask");
    static_assert(helper::mask_of<i32_800f_0, i32>::value == 0,
                  "mask should be 0 when types mismatch");
    static_assert(helper::mask_of<i32_800f_0, u32>::effective_value == 0x800F, "incorrect mask");
    static_assert(helper::mask_of<i32_800f_0, i32>::effective_value == 0,
                  "mask should be 0 when types mismatch");
    static_assert(helper::mask_of<i32_800f_0, u32>::shift == 0, "incorrect shift");
    static_assert(helper::mask_of<i32_800f_0, i32>::shift == 0,
                  "shift should be 0 when types mismatch");

    static_assert(helper::mask_of<i32_800f_4, u32>::value == 0x800F, "incorrect mask");
    static_assert(helper::mask_of<i32_800f_4, i32>::value == 0,
                  "mask should be 0 when types mismatch");
    static_assert(helper::mask_of<i32_800f_4, u32>::effective_value == 0x800F0, "incorrect mask");
    static_assert(helper::mask_of<i32_800f_4, i32>::effective_value == 0,
                  "mask should be 0 when types mismatch");
    static_assert(helper::mask_of<i32_800f_4, u32>::shift == 4, "incorrect shift");
    static_assert(helper::mask_of<i32_800f_4, i32>::shift == 0,
                  "shift should be 0 when types mismatch");
    static_assert(helper::mask_of<i32, u32>::value == 0, "mask should be 0 if not masked");
    static_assert(helper::mask_of<i32, i32>::value == 0, "mask should be 0 if not masked");

    // lshift(value, n)
    static_assert(helper::lshift(0U, 0) == 0U, "");
    static_assert(helper::lshift(0U, 30) == 0U, "");
    static_assert(helper::lshift(1U, 0) == 1U, "");
    static_assert(helper::lshift(1U, 10) == 1024U, "");
    static_assert(helper::lshift(10U, 10) == 10240U, "");
    static_assert(helper::lshift(10, 10) == 10240, "");
    static_assert(helper::lshift(-10, 0) == -10, "");
    // static_assert(helper::lshift(-10, 10) == -10240, ""); // error: left shift of negative value

    // minMask(maxValue)
    static_assert(helper::minMask(0U) == 0U, "lowest 0 bits");
    static_assert(helper::minMask(1U) == 1U, "lowest 1 bit");
    static_assert(helper::minMask(2U) == 3U, "lowest 2 bits");
    static_assert(helper::minMask(3U) == 3U, "lowest 2 bits");
    static_assert(helper::minMask(4U) == 7U, "lowest 3 bits");
    static_assert(helper::minMask(~0U) == ~0U, "all bits");
    // static_assert(helper::minMask(10) == 0xF, "all bits"); // error: must be unsigned

    // topBits(n)
    static_assert(helper::topBits<u32>(0) == 0U, "top 0 bit");
    static_assert(helper::topBits<u32>(1) == 0x80000000U, "top 1 bit");
    static_assert(helper::topBits<u32>(2) == 0xC0000000U, "top 2 bits");
    static_assert(helper::topBits<u32>(12) == 0xFFF00000U, "top 12 bits");
    static_assert(helper::topBits<u32>(32) == 0xFFFFFFFFU, "all bits");
    // static_assert(helper::topBits<u32>(33) == 0xFFFFFFFFU, ""); // should OVERFLOW

    static_assert(helper::topBits<u8>(0) == 0U, "top 0 bit");
    static_assert(helper::topBits<u8>(1) == 0x80U, "top 1 bit");
    static_assert(helper::topBits<u8>(2) == 0xC0U, "top 2 bit");
    static_assert(helper::topBits<u8>(8) == 0xFFU, "all bits");
    // static_assert(helper::topBits<u8>(9) == 0xFFU, ""); // should OVERFLOW

    // getShift(mask, base, shared, base-shift, base-effective)
    static_assert(helper::getShift(0u, 0u, 0u, 0, 0u) == 0, "no flag require no shift");
    static_assert(helper::getShift(0u, 0u, 1u, 0, 0u) == -1,
                  "shared must be within mask and base mask");
    static_assert(helper::getShift(0u, 1u, 1u, 0, 1u) == -1, "shared must be within mask");
    static_assert(helper::getShift(0u, 1u, 0u, 0, 1u) == 0,
                  "no flags require no shift even with base mask");
    static_assert(helper::getShift(0u, 1u, 0u, 1, 2u) == 0,
                  "no flags require no shift even with shifted base mask");
    static_assert(helper::getShift(1u, 0u, 0u, 0, 0u) == 0, "no base mask requires no shift");
    static_assert(helper::getShift(1u, 1u, 0u, 0, 1u) == 1,
                  "overlapping mask and basemask requires shift");
    static_assert(helper::getShift(1u, 1u, 0u, 0, 1u) == 1,
                  "overlapping mask and basemask requires shift");
    static_assert(helper::getShift(1u, 1u, 1u, 0, 1u) == 0,
                  "shared mask requires using base shift");
    static_assert(helper::getShift(1u, 1u, 1u, 1, 2u) == 1,
                  "shared mask requires using base shift");
    static_assert(helper::getShift(3u, 5u, 1u, 0, 5u) == 0,
                  "mask and basemask that overlap only in shared region requires no shift");
    static_assert(helper::getShift(3u, 7u, 1u, 0, 7u) == -1,
                  "mask and basemask must not overlap in more than shared region");
    static_assert(helper::getShift(1u, 0u, 1u, 0, 0u) == -1, "shared must be within base mask");

    static_assert(helper::getShift(0u, 1u, 0u, 1, 1u) == -2, "effective mask must cover base mask");
    static_assert(helper::getShift(0u, 5u, 0u, 1, 2u) == -2, "effective mask must cover base mask");
    static_assert(helper::getShift(0u, 5u, 0u, 1, 10u) == 0, "");
    static_assert(helper::getShift(0u, 5u, 0u, 1, 31u) == 0,
                  "effective mask can be larger than base mask");

    static_assert(helper::getShift(0x800Fu, 0x800Fu, 0x800Fu, 0, 0x800Fu) == 0,
                  "(0x800F << 0) & 0x800F == 0x800F");
    static_assert(helper::getShift(0x800Fu, 0x800Fu, 0x800Fu, 16, 0x800F0000u) == 16,
                  "(0x800F << 0) & 0x800F == 0x800F");
    static_assert(helper::getShift(0x1800Fu, 0x800Fu, 0x800Fu, 0, 0x800Fu) == 0,
                  "(0x1800F << 0) & 0x800F == 0x800F");
    static_assert(helper::getShift(0x1800Fu, 0x800Fu, 0x800Fu, 16, 0x800F0000u) == -1,
                  "(0x1800F << 16) overflows");

    // verify that when not sharing masks, effective mask makes the difference
    static_assert(helper::getShift(0x800Fu, 0u, 0u, 0, 0x800Fu) == 4,
                  "(0x800F << 4) & 0x800F == 0");
    static_assert(helper::getShift(0x800Fu, 0x2u, 0u, 0, 0x8002u) == 2,
                  "(0x800F << 2) & 0x8002 == 0");
    static_assert(helper::getShift(0x800Fu, 0x1u, 0u, 15, 0x8001u) == 1,
                  "(0x800F << 1) & 0x8001 == 0");
    static_assert(helper::getShift(0x800Fu, 0x800Fu, 0u, 16, 0x800F0000u) == 0,
                  "0x800F & 0x800F0000 == 0");
    static_assert(helper::getShift(0x800Fu, 0x800F8000u, 0u, 0, 0x800F8000u) == 5,
                  "(0x800F << 5) & 0x800F8000 == 0");
    static_assert(helper::getShift(0x800Fu, 0xF0000u, 0u, 0, 0x800F8000u) == 5,
                  "(0x800F << 5) & 0x800F8000 == 0");
    static_assert(helper::getShift(0x800Fu, 0x1Fu, 0u, 15, 0x800F8000u) == 5,
                  "(0x800F << 5) & 0x800F8000 == 0");
    static_assert(helper::getShift(0xFFu, 0x80808080u, 0u, 0, 0x80808080u) == -1,
                  "0xFF always overlaps with 0x80808080");
    static_assert(helper::getShift(0xFFu, 0x10001000u, 0u, 3, 0x80808080u) == -1,
                  "0xFF always overlaps with 0x80808080");
    static_assert(helper::getShift(0xFFu, 0x80808040u, 0u, 0, 0x80808040u) == 7,
                  "(0xFF << 7) & 0x 80808040 == 0");

    // verify min_shift (mask must be positive or no shift can be required)
    static_assert(helper::getShift(0xFF, 0x40808040, 0, 0, 0x40808040) == 7, "");
    static_assert(helper::getShift((i32)0x800000FF, 0x40808040, 0, 0, 0x40808040) == -1, "");
    static_assert(helper::getShift(0x100000FF, 0x40808040, 0, 0, 0x40808040) == -1, "");
    static_assert(helper::getShift(0xFF, (i32)0x80808040, 0, 0, (i32)0x80808040) == 7, "");
    static_assert(helper::getShift((i32)0x80007F80, 0x40808040, 0, 0, 0x40808040) == 0, "");

    // shared mask can also be negative (but not shift can be required)
    static_assert(helper::getShift((i32)0x80007F80, (i32)0xC0808040, (i32)0x80000000,
                                   0, (i32)0xC0808040) == 0, "");
    static_assert(helper::getShift((i32)0x80007F80, (i32)0xC0808040, (i32)0xC0000000,
                                   0, (i32)0xC0808040) == -1, "");
    static_assert(helper::getShift((i32)0x80007F80, (i32)0x60404020, (i32)0x60000000,
                                   1, (i32)0xC0808040) == -1, "");

    // min_shift
    typedef Flagged<i32, u32, 0u> i32_0_0;
    typedef Flagged<i32, u32, 1u> i32_1_0;
    typedef Flagged<i32, u32, 1u, 0u, 1> i32_1_1;

    // this is a wrapper over getShift, so same test cases apply when T is flagged
    static_assert(helper::min_shift<i32_0_0, u32, 0u, 0u>::value == 0, "");
    static_assert(helper::min_shift<i32_0_0, u32, 0u, 1u>::value == -1, "");
    static_assert(helper::min_shift<i32_1_0, u32, 0u, 1u>::value == -1, "");
    static_assert(helper::min_shift<i32_1_0, u32, 0u, 0u>::value == 0, "");
    static_assert(helper::min_shift<i32_0_0, u32, 1u, 0u>::value == 0, "");
    static_assert(helper::min_shift<i32_1_0, u32, 1u, 0u>::value == 1, "");
    static_assert(helper::min_shift<i32_1_0, u32, 1u, 1u>::value == 0, "");
    static_assert(helper::min_shift<i32_1_1, u32, 1u, 1u>::value == 1, "");
    static_assert(helper::min_shift<i32_1_1, u32, 3u, 0u>::value == 2, "");
    static_assert(helper::min_shift<Flagged<i32, u32, 5u>, u32, 3u, 1u>::value == 0, "");
    static_assert(helper::min_shift<Flagged<i32, u32, 7u>, u32, 3u, 1u>::value == -1, "");
    static_assert(helper::min_shift<i32_0_0, u32, 1u, 1u>::value == -1, "");

    static_assert(helper::min_shift<i32_800f_0, u32, 0x800Fu, 0u>::value == 4, "");
    static_assert(helper::min_shift<i32_800f_4, u32, 0x1800Fu, 0x800Fu>::value == 4, "");
    static_assert(helper::min_shift<i32_800f_4, u32, 0x800Fu, 0u>::value == 0, "");
    static_assert(helper::min_shift<Flagged<i32, u32, 0x8002u>, u32, 0x800Fu, 0u>::value == 2, "");
    static_assert(helper::min_shift<Flagged<i32, u32, 0x8001u>, u32, 0x800Fu, 0u>::value == 1, "");
    static_assert(
            helper::min_shift<Flagged<i32, u32, 0x800Fu, 0u, 16>, u32, 0x800Fu, 0u>::value == 0, "");
    static_assert(
            helper::min_shift<Flagged<i32, u32, 0x800F8000u>, u32, 0x800Fu, 0u>::value == 5, "");
    static_assert(
            helper::min_shift<Flagged<i32, u32, 0x80808080u>, u32, 0xFFu, 0u>::value == -1, "");
    static_assert(
            helper::min_shift<Flagged<i32, u32, 0x80808040u>, u32, 0xFFu, 0u>::value == 7, "");

    // for min_shift, non-tagged type behaves as if having base mask of 0
    static_assert(helper::min_shift<i32, u32, 0u, 0u>::value == 0, "");
    static_assert(helper::min_shift<u32, u32, 0u, 0u>::value == 0, "");
    static_assert(helper::min_shift<i32, u32, 0u, 0u>::value == 0, "");
    static_assert(helper::min_shift<i32, u32, 0u, 1u>::value == -1, "");
    static_assert(helper::min_shift<i32, u32, 1u, 0u>::value == 0, "");
    static_assert(helper::min_shift<i32, u32, 1u, 1u>::value == -1, "");

    // verify min_shift (mask must be positive or no shift can be required)
    static_assert(helper::min_shift<Flagged<i32, i32, 0x40808040>, i32, 0xFF, 0>::value == 7, "");
    static_assert(helper::min_shift<Flagged<i32, i32, 0x40808040>,
                                    i32, (i32)0x800000FF, 0>::value == -1, "");
    static_assert(helper::min_shift<Flagged<i32, i32, 0x40808040>,
                                    i32, 0x100000FF, 0>::value == -1, "");
    static_assert(helper::min_shift<Flagged<i32, i32, (i32)0x80808040>,
                                    i32, 0xFF, 0>::value == 7, "");
    static_assert(helper::min_shift<Flagged<i32, i32, 0x40808040>,
                                    i32, (i32)0x80007F80, 0>::value == 0, "");

    static_assert(helper::min_shift<Flagged<i32, i32, (i32)0x80808040>,
                                    i32, (i32)0x80007F80, (i32)0x80000000>::value == 0, "");
    static_assert(helper::min_shift<Flagged<i32, i32, (i32)0xC0808040>,
                                    i32, (i32)0x80007F80, (i32)0xC0000000>::value == -1, "");
    // note: cannot create a flagged type with signed flag and shift
    // static_assert(helper::min_shift<Flagged<i32, i32, (i32)0x60404020, 0, 1>,
    //                                i32, (i32)0x40003FC0, (i32)0x40000000>::value == -1, "");

    typedef Flagged<i32, u32, 0x800F /* mask */, 0 /* shared mask */, 16 /* shift */> i32_800f_16;
    static_assert_equals(sizeof(i32_800f_16), sizeof(i32_800f_0), "");
    // shifted mask overflows!
    // typedef Flagged<i32, u32, 0x800F /* mask */, 0 /* shared mask */, 17 /* shift */> i32_800f_17;
    // static_assert(sizeof(i32_800f_17) == sizeof(i32_800f_0), "");
    typedef Flagged<i32, i32, 0x800F /* mask */, 0 /* shared mask */, 15 /* shift */> i32_800f_15i;
    static_assert_equals(sizeof(i32_800f_15i), sizeof(i32_800f_0), "");
    // shifted mask overflows!
    // typedef Flagged<i32, i32, 0x800F /* mask */, 0 /* shared mask */, 16 /* shift */> i32_800f_16i;
    // static_assert(sizeof(i32_800f_16i) == sizeof(i32_800f_0), "");

    // canCombine(mask, base, shared, shift, base-shift, base-effective)
    static_assert(helper::canCombine(0u, 0u, 0u, 0, 0, 0u), "using no mask is valid");
    static_assert(helper::canCombine(0u, 0u, 0u, 0, 0, 0u), "");
    static_assert(helper::canCombine(0u, 0u, 0u, 4, 0, 0u), "");
    static_assert(!helper::canCombine(0u, 0u, 1u, 0, 0, 0u),
                  "shared mask must be the overlap of masks");
    static_assert(helper::canCombine(1u, 0u, 0u, 0, 0, 0u), "");
    static_assert(helper::canCombine(1u, 0u, 0u, 4, 0, 0u), "");
    static_assert(helper::canCombine(3u, 5u, 1u, 0, 0, 5u), "");
    static_assert(!helper::canCombine(3u, 3u, 3u, 1, 0, 3u), "shift must match when sharing mask");
    static_assert(helper::canCombine(3u, 3u, 3u, 1, 1, 6u), "");
    static_assert(!helper::canCombine(3u, 3u, 3u, 1, 2, 12u), "shift must match when sharing mask");
    static_assert(!helper::canCombine(3u, 7u, 1u, 0, 0, 7u), "");
    static_assert(!helper::canCombine(1u, 0u, 1u, 0, 0, 0u), "");

    static_assert(!helper::canCombine(0u, 1u, 1u, 0, 0, 1u),
                  "shared mask must be the overlap of masks");
    static_assert(helper::canCombine(0u, 1u, 0u, 0, 0, 1u), "");
    static_assert(helper::canCombine(0u, 1u, 0u, 4, 0, 1u), "");
    static_assert(helper::canCombine(1u, 1u, 0u, 1, 0, 1u), "");
    static_assert(!helper::canCombine(1u, 1u, 0u, 0, 0, 1u), "");
    static_assert(helper::canCombine(1u, 1u, 0u, 1, 0, 1u), "");
    static_assert(helper::canCombine(1u, 1u, 1u, 0, 0, 1u), "");
    static_assert(!helper::canCombine(1u, 1u, 1u, 1, 0, 1u), "shift must match when sharing mask");

    static_assert(helper::canCombine(0x800Fu, 0x800Fu, 0u, 4, 0, 0x800Fu), "");
    static_assert(!helper::canCombine(0x800Fu, 0x800Fu, 0u, 1, 0, 0x800Fu), "");
    static_assert(helper::canCombine(0x800Fu, 0x8002u, 0u, 2, 0, 0x8002u), "");
    static_assert(helper::canCombine(0x800Fu, 0x8001u, 0u, 1, 0, 0x8001u), "");
    static_assert(helper::canCombine(0x800Fu, 0x800Fu, 0u, 0, 16, 0x800F0000u), "");
    static_assert(helper::canCombine(0x800Fu, 0x800Fu, 0x800Fu, 16, 16, 0x800F0000u), "");
    static_assert(!helper::canCombine(0x1800Fu, 0x800Fu, 0u, 0, 16, 0x800F0000u), "");
    static_assert(!helper::canCombine(0x1800Fu, 0x800Fu, 0x800Fu, 16, 16, 0x800F0000u), "");
    static_assert(helper::canCombine(0x800Fu, 0x800F8000u, 0u, 8, 0, 0x800F8000u), "");
    static_assert(!helper::canCombine(0xFFu, 0x80808080u, 0u, -1, 0, 0x80808080u), "");
    static_assert(helper::canCombine(0xFFu, 0x80808040u, 0u, 7, 0, 0x80808040u), "");
    static_assert(helper::canCombine(0xFFu, 0x8000u, 0u, 7, 0, 0x80808040u), "");
    static_assert(helper::canCombine(0xFFu, 0x101u, 0u, 7, 15, 0x80808040u), "");

    // can combine signed-flagged types only if mask is positive or no shift is required
    static_assert(!helper::canCombine(0xFF, 0x40808040, 0, 0, 0, 0x40808040), "");
    static_assert(helper::canCombine(0xFF, 0x40808040, 0, 7, 0, 0x40808040), "");
    static_assert(!helper::canCombine((i32)0x800000FF, 0x40808040, 0, 0, 0, 0x40808040), "");
    static_assert(!helper::canCombine((i32)0x800000FF, 0x40808040, 0, 7, 0, 0x40808040), "");
    static_assert(!helper::canCombine(0x100000FF, 0x40808040, 0, 0, 0, 0x40808040), "");
    static_assert(!helper::canCombine(0x100000FF, 0x40808040, 0, 7, 0, 0x40808040), "");
    static_assert(!helper::canCombine(0xFF, (i32)0x80808040, 0, 0, 0, (i32)0x80808040), "");
    static_assert(helper::canCombine(0xFF, (i32)0x80808040, 0, 7, 0, (i32)0x80808040), "");
    static_assert(helper::canCombine((i32)0x80007F80, 0x40808040, 0, 0, 0, 0x40808040), "");

    static_assert(helper::canCombine((i32)0x80007F80, (i32)0x80808040, (i32)0x80000000, 0, 0, (i32)0x80808040), "");
    static_assert(!helper::canCombine((i32)0xC0007F80, (i32)0x80808040, (i32)0xC0000000, 0, 0, (i32)0x80808040), "");
    static_assert(!helper::canCombine((i32)0x80007F80, (i32)0x80808040, (i32)0x80000000, 1, 0, (i32)0x80808040), "");
    static_assert(!helper::canCombine((i32)0xC0007F80, (i32)0x80808040, (i32)0xC0000000, 1, 0, (i32)0x80808040), "");

    // can_combine<T, Flag, MASK, [SHARED_MASK], [SHIFT]
    static_assert(helper::can_combine<i32_0_0, u32, 0u>::value, "");
    static_assert(helper::can_combine<i32_0_0, u32, 0u, 0u>::value, "");
    static_assert(helper::can_combine<i32_0_0, u32, 0u, 0u, 4>::value, "");
    static_assert(!helper::can_combine<i32_0_0, u32, 0u, 1u>::value, "");
    static_assert(helper::can_combine<i32_0_0, u32, 1u, 0u>::value, "");
    static_assert(helper::can_combine<i32_0_0, u32, 1u, 0u, 4>::value, "");
    static_assert(!helper::can_combine<i32_0_0, u32, 1u, 1u>::value, "");

    static_assert(!helper::can_combine<i32_1_0, u32, 0u, 1u>::value, "");
    static_assert(helper::can_combine<i32_1_0, u32, 0u, 0u>::value, "");
    static_assert(helper::can_combine<i32_1_0, u32, 0u, 0u, 4>::value, "");
    static_assert(helper::can_combine<i32_1_0, u32, 1u, 0u>::value, "");
    static_assert(!helper::can_combine<i32_1_0, u32, 1u, 0u, 0>::value, "");
    static_assert(helper::can_combine<i32_1_0, u32, 1u, 0u, 1>::value, "");
    static_assert(helper::can_combine<i32_1_0, u32, 1u, 1u>::value, "");
    static_assert(helper::can_combine<i32_1_0, u32, 1u, 1u, 0>::value, "");
    static_assert(!helper::can_combine<i32_1_0, u32, 1u, 1u, 1>::value,
                  "shouldn't be able to use SHIFT with SHARED_MASK");

    static_assert(helper::can_combine<i32_800f_0, u32, 0x800Fu, 0u, 4>::value, "");
    static_assert(!helper::can_combine<i32_800f_0, u32, 0x800Fu, 0u, 1>::value, "");
    static_assert(helper::can_combine<i32_800f_0, u32, 0x800Fu, 0u>::value, "");
    static_assert(helper::can_combine<Flagged<i32, u32, 0x8002u>, u32, 0x800Fu, 0u>::value, "");
    static_assert(helper::can_combine<Flagged<i32, u32, 0x8001u>, u32, 0x800Fu, 0u>::value, "");
    static_assert(helper::can_combine<Flagged<i32, u32, 0x800F0000u>, u32, 0x800Fu, 0u>::value, "");
    static_assert(helper::can_combine<Flagged<i32, u32, 0x800F8000u>, u32, 0x800Fu, 0u>::value, "");
    static_assert(!helper::can_combine<Flagged<i32, u32, 0x80808080u>, u32, 0xFFu, 0u>::value, "");
    static_assert(helper::can_combine<Flagged<i32, u32, 0x80808040u>, u32, 0xFFu, 0u>::value, "");

    // can combine signed-flagged types only if mask is positive or no shift is required
    static_assert(helper::can_combine<Flagged<i32, i32, 0x40808040>, i32, 0xFF, 0>::value, "");
    static_assert(!helper::can_combine<Flagged<i32, i32, 0x40808040>,
                                       i32, (i32)0x800000FF, 0>::value, "");
    static_assert(!helper::can_combine<Flagged<i32, i32, 0x40808040>,
                                       i32, 0x100000FF, 0>::value, "");
    static_assert(helper::can_combine<Flagged<i32, i32, (i32)0x80808040>, i32, 0xFF, 0>::value, "");
    static_assert(helper::can_combine<Flagged<i32, i32, 0x40808040>,
                                      i32, (i32)0x80007F80, 0>::value, "");

    static_assert(helper::can_combine<Flagged<i32, i32, (i32)0x80808040>,
                                      i32, (i32)0x80007F80, (i32)0x80000000>::value, "");
    static_assert(!helper::can_combine<Flagged<i32, i32, (i32)0xC0808040>,
                                       i32, (i32)0x80007F80, (i32)0xC0000000>::value, "");

    static_assert(helper::min_shift<Flagged<i32, FLAG, (FLAG)0x80808040>,
                                    FLAG, (FLAG)0x80007F80, (FLAG)0x80000000>::value == 0, "");
    static_assert(helper::can_combine<Flagged<i32, FLAG, (FLAG)0x80808040>,
                                      FLAG, (FLAG)0x80007F80, (FLAG)0x80000000>::value, "");

    // cannot combine non-tagged types
    static_assert(!helper::can_combine<i32, u32, 0u, 0u>::value, "");
    static_assert(!helper::can_combine<u32, u32, 0u, 0u>::value, "");
    static_assert(!helper::can_combine<i32, u32, 0u, 0u>::value, "");
    static_assert(!helper::can_combine<i32, u32, 0u, 1u>::value, "");
    static_assert(!helper::can_combine<i32, u32, 1u, 0u>::value, "");
    static_assert(!helper::can_combine<i32, u32, 1u, 1u>::value, "");

    typedef Flagged<i32_800f_0, u32, 0x800F /* mask */, 0 /* shared mask */> i32_800f_800f;
    static_assert(i32_800f_800f::sFlagMask == 0x800F, "");
    static_assert(i32_800f_800f::sFlagShift == 4, "");
    static_assert(i32_800f_800f::sEffectiveMask == 0x880FF, "");
    static_assert(!i32_800f_0::sFlagCombined, "");
    static_assert(!i32_800f_4::sFlagCombined, "");

    static_assert(i32_800f_800f::sFlagCombined, "");
    static_assert_equals(sizeof(i32_800f_800f), sizeof(i32_800f_0), "");

    typedef Flagged<i32_800f_0, u32, 0x1FFFF /* mask */> i32_800f_1ffff;
    static_assert(i32_800f_1ffff::sFlagMask == 0x1FFFF, "");
    static_assert(i32_800f_1ffff::sFlagShift == 0, "");
    static_assert(i32_800f_1ffff::sEffectiveMask == 0x1FFFF, "");
    static_assert(!i32_800f_1ffff::sFlagCombined, "");

    // operational tests
    i32_800f_800f val(0x8000, 0x1234, 56);
    EXPECT_EQ(val.get().get(), 56);
    EXPECT_EQ(val.flags(), 0x8000u);
    EXPECT_EQ(val.get().flags(), 0x1234u & 0x800F);
    val.setFlags(0x12345);
    EXPECT_EQ(val.flags(), 0x12345u & 0x800F);
    EXPECT_EQ(val.get().flags(), 0x1234u & 0x800F);
    val.get().setFlags(0x54321);
    EXPECT_EQ(val.flags(), 0x12345u & 0x800F);
    EXPECT_EQ(val.get().flags(), 0x54321u & 0x800F);
    EXPECT_EQ(val.get().get(), 56);

    typedef Flagged<i32_800f_4, u32, 0x800F /* mask */, 0 /* shared mask */> i32_800f_800f_B;
    static_assert(i32_800f_800f_B::sFlagMask == 0x800F, "");
    static_assert(i32_800f_800f_B::sFlagShift == 0, "");
    static_assert(i32_800f_800f_B::sEffectiveMask == 0x880FF, "");

    i32_800f_800f_B valB(0x8000, 0x1234, -987);
    EXPECT_EQ(valB.get().get(), -987);
    EXPECT_EQ(valB.flags(), 0x8000u);
    EXPECT_EQ(valB.get().flags(), 0x1234u & 0x800F);
    valB.setFlags(0x12345);
    EXPECT_EQ(valB.flags(), 0x12345u & 0x800F);
    EXPECT_EQ(valB.get().flags(), 0x1234u & 0x800F);
    valB.get().setFlags(0x5C321);
    EXPECT_EQ(valB.flags(), 0x12345u & 0x800F);
    EXPECT_EQ(valB.get().flags(), 0x5C321u & 0x800F);
    EXPECT_EQ(valB.get().get(), -987);

    typedef Flagged<Flagged<i32, u32, 0xFF>, u32, 0xFF0, 0xF0> i32_ff_ff0;
    i32_ff_ff0 valC(0xABCD, 0x1234, 101);
    EXPECT_EQ(valC.get().get(), 101);
    EXPECT_EQ(valC.flags(), 0xBC0u);
    EXPECT_EQ(valC.get().flags(), 0xC4u);
    valC.setFlags(0x12345);
    EXPECT_EQ(valC.flags(), 0x340u);
    EXPECT_EQ(valC.get().flags(), 0x44u);
    valC.get().setFlags(0x54321);
    EXPECT_EQ(valC.flags(), 0x320u);
    EXPECT_EQ(valC.get().flags(), 0x21u);
    EXPECT_EQ(valC.get().get(), 101);

    // when combining flags (with no shift), it should work with signed flags
    typedef Flagged<Flagged<i32, FLAG, kMask0>, FLAG, kMask1, kMaskCommon> i32_F_ff_ff0;
    static_assert(i32_F_ff_ff0::sFlagCombined, "flags should be combined");

    i32_F_ff_ff0 valD(kFlag1_A, kFlag0_A, 1023);
    EXPECT_EQ(valD.get().get(), 1023);
    EXPECT_EQ(valD.flags(), kFlag1_A);
    EXPECT_EQ(valD.get().flags(), kFlag0_A);
    valD.setFlags(kFlag1_B);
    EXPECT_EQ(valD.flags(), kFlag1_B);
    EXPECT_EQ(valD.get().flags(), FLAG(0x0BA));
    valD.get().setFlags(kFlag0_C);
    EXPECT_EQ(valD.flags(), FLAG(0xBC0));
    EXPECT_EQ(valD.get().flags(), kFlag0_C);
    EXPECT_EQ(valD.get().get(), 1023);
}

} // namespace android
