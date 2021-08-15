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

//#define LOG_NDEBUG 0
#define LOG_TAG "hidl_test_java_native"

#include <android-base/file.h>
#include <android-base/logging.h>

#include <android/hardware/tests/baz/1.0/IBaz.h>
#include <android/hardware/tests/safeunion/1.0/IOtherInterface.h>
#include <android/hardware/tests/safeunion/1.0/ISafeUnion.h>

#include <hidl/LegacySupport.h>
#include <hidl/ServiceManagement.h>
#include <gtest/gtest.h>

#include <hidl/HidlTransportSupport.h>
#include <hidl/Status.h>

#include <numeric>
#include <sys/stat.h>

using ::android::sp;
using ::android::hardware::tests::baz::V1_0::IBase;
using ::android::hardware::tests::baz::V1_0::IBaz;
using ::android::hardware::tests::baz::V1_0::IBazCallback;
using ::android::hardware::tests::safeunion::V1_0::IOtherInterface;
using ::android::hardware::tests::safeunion::V1_0::ISafeUnion;

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_vec;
using ::android::hardware::hidl_handle;
using ::android::hardware::hidl_string;
using ::android::hardware::defaultPassthroughServiceImplementation;
using ::android::hardware::Return;
using ::android::hardware::Void;

using HandleTypeSafeUnion = ISafeUnion::HandleTypeSafeUnion;
using InterfaceTypeSafeUnion = ISafeUnion::InterfaceTypeSafeUnion;
using LargeSafeUnion = ISafeUnion::LargeSafeUnion;
using SmallSafeUnion = ISafeUnion::SmallSafeUnion;

struct BazCallback : public IBazCallback {
    Return<void> heyItsMe(const sp<IBazCallback> &cb) override;
    Return<void> hey() override;
};

Return<void> BazCallback::heyItsMe(
        const sp<IBazCallback> &cb) {
    LOG(INFO) << "SERVER: heyItsMe cb = " << cb.get();

    return Void();
}

Return<void> BazCallback::hey() {
    LOG(INFO) << "SERVER: hey";

    return Void();
}

struct OtherInterface : public IOtherInterface {
    Return<void> concatTwoStrings(const hidl_string& a, const hidl_string& b,
                                  concatTwoStrings_cb _hidl_cb) override {
        hidl_string result = std::string(a) + std::string(b);
        _hidl_cb(result);

        return Void();
    }
};

using std::to_string;

static void usage(const char *me) {
    fprintf(stderr, "%s [-c]lient | [-s]erver\n", me);
}

struct HidlEnvironment : public ::testing::Environment {
    void SetUp() override {
    }

    void TearDown() override {
    }
};

struct HidlTest : public ::testing::Test {
    sp<IBaz> baz;
    sp<ISafeUnion> safeunionInterface;
    sp<IOtherInterface> otherInterface;

    void SetUp() override {
        using namespace ::android::hardware;

        ::android::hardware::details::waitForHwService(IBaz::descriptor, "default");
        baz = IBaz::getService();
        CHECK(baz != nullptr);
        CHECK(baz->isRemote());

        ::android::hardware::details::waitForHwService(ISafeUnion::descriptor, "default");
        safeunionInterface = ISafeUnion::getService();
        CHECK(safeunionInterface != nullptr);
        CHECK(safeunionInterface->isRemote());

        ::android::hardware::details::waitForHwService(IOtherInterface::descriptor, "default");
        otherInterface = IOtherInterface::getService();
        CHECK(otherInterface != nullptr);
        CHECK(otherInterface->isRemote());
    }

    void TearDown() override {
    }
};

template <typename T>
static void EXPECT_OK(const ::android::hardware::Return<T> &ret) {
    EXPECT_TRUE(ret.isOk());
}

template<typename T, typename S>
static inline bool isArrayEqual(const T arr1, const S arr2, size_t size) {
    for(size_t i = 0; i < size; i++)
        if(arr1[i] != arr2[i])
            return false;
    return true;
}

TEST_F(HidlTest, GetDescriptorTest) {
    EXPECT_OK(baz->interfaceDescriptor([&] (const auto &desc) {
        EXPECT_EQ(desc, IBaz::descriptor);
    }));
}

TEST_F(HidlTest, BazSomeBaseMethodTest) {
    EXPECT_OK(baz->someBaseMethod());
}

TEST_F(HidlTest, BazSomeOtherBaseMethodTest) {
    IBase::Foo foo;
    foo.x = 1;
    foo.y.z = 2.5;
    // A valid UTF-8 string
    foo.y.s = "Hello, world, \x46\x6F\x6F\x20\xC2\xA9\x20\x62\x61\x72\x20\xF0\x9D\x8C\x86\x20\x54\x72\x65\x62\x6C\x65\x20\xE2\x98\x83\x20\x72\x6F\x63\x6B\x73";

    foo.aaa.resize(5);
    for (size_t i = 0; i < foo.aaa.size(); ++i) {
        foo.aaa[i].z = 1.0f + (float)i * 0.01f;
        foo.aaa[i].s = ("Hello, world " + std::to_string(i)).c_str();
    }

    EXPECT_OK(
            baz->someOtherBaseMethod(
                foo,
                [&](const auto &result) {
                    // Strings should have the same size as they did before
                    // marshaling. b/35038064
                    EXPECT_EQ(result.y.s.size(), foo.y.s.size());
                    EXPECT_EQ(foo, result);
               }));
}

TEST_F(HidlTest, SomeOtherBaseMethodInvalidString) {
    IBase::Foo foo {
        .y = {
            .s = "\xff",
        }
    };

    auto ret = baz->someOtherBaseMethod(foo, [&](const auto&) {
        ADD_FAILURE() << "Should not accept invalid UTF-8 String";
    });

    EXPECT_FALSE(ret.isOk());

    EXPECT_OK(baz->ping());
}

TEST_F(HidlTest, BazSomeMethodWithFooArraysTest) {
    hidl_array<IBase::Foo, 2> foo;

    foo[0].x = 1;
    foo[0].y.z = 2.5;
    foo[0].y.s = "Hello, world";

    foo[0].aaa.resize(5);
    for (size_t i = 0; i < foo[0].aaa.size(); ++i) {
        foo[0].aaa[i].z = 1.0f + (float)i * 0.01f;
        foo[0].aaa[i].s = ("Hello, world " + std::to_string(i)).c_str();
    }

    foo[1].x = 2;
    foo[1].y.z = -2.5;
    foo[1].y.s = "Morituri te salutant";

    foo[1].aaa.resize(3);
    for (size_t i = 0; i < foo[1].aaa.size(); ++i) {
        foo[1].aaa[i].z = 2.0f - (float)i * 0.01f;
        foo[1].aaa[i].s = ("Alea iacta est: " + std::to_string(i)).c_str();
    }

    hidl_array<IBaz::Foo, 2> fooExpectedOutput;
    fooExpectedOutput[0] = foo[1];
    fooExpectedOutput[1] = foo[0];

    EXPECT_OK(
            baz->someMethodWithFooArrays(
                foo,
                [&](const auto &result) {
                    EXPECT_EQ(result, fooExpectedOutput);
               }));
}

TEST_F(HidlTest, BazSomeMethodWithFooVectorsTest) {
    hidl_vec<IBase::Foo> foo;
    foo.resize(2);

    foo[0].x = 1;
    foo[0].y.z = 2.5;
    foo[0].y.s = "Hello, world";

    foo[0].aaa.resize(5);
    for (size_t i = 0; i < foo[0].aaa.size(); ++i) {
        foo[0].aaa[i].z = 1.0f + (float)i * 0.01f;
        foo[0].aaa[i].s = ("Hello, world " + std::to_string(i)).c_str();
    }

    foo[1].x = 2;
    foo[1].y.z = -2.5;
    foo[1].y.s = "Morituri te salutant";

    foo[1].aaa.resize(3);
    for (size_t i = 0; i < foo[1].aaa.size(); ++i) {
        foo[1].aaa[i].z = 2.0f - (float)i * 0.01f;
        foo[1].aaa[i].s = ("Alea iacta est: " + std::to_string(i)).c_str();
    }

    hidl_vec<IBaz::Foo> fooExpectedOutput;
    fooExpectedOutput.resize(2);
    fooExpectedOutput[0] = foo[1];
    fooExpectedOutput[1] = foo[0];

    EXPECT_OK(
            baz->someMethodWithFooVectors(
                foo,
                [&](const auto &result) {
                    EXPECT_EQ(result, fooExpectedOutput);
                }));
}

TEST_F(HidlTest, BazSomeMethodWithVectorOfArray) {
    IBase::VectorOfArray in, expectedOut;
    in.addresses.resize(3);
    expectedOut.addresses.resize(3);

    size_t k = 0;
    const size_t n = in.addresses.size();

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < 6; ++j, ++k) {
            in.addresses[i][j] = k;
            expectedOut.addresses[n - 1 - i][j] = k;
        }
    }

    EXPECT_OK(
            baz->someMethodWithVectorOfArray(
                in,
                [&](const auto &out) {
                    EXPECT_EQ(expectedOut, out);
                }));
}

TEST_F(HidlTest, BazSomeMethodTakingAVectorOfArray) {
    hidl_vec<hidl_array<uint8_t, 6> > in, expectedOut;
    in.resize(3);
    expectedOut.resize(3);

    size_t k = 0;
    const size_t n = in.size();
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < 6; ++j, ++k) {
            in[i][j] = k;
            expectedOut[n - 1 - i][j] = k;
        }
    }

    EXPECT_OK(
            baz->someMethodTakingAVectorOfArray(
                in,
                [&](const auto &out) {
                    EXPECT_EQ(expectedOut, out);
                }));
}

static std::string numberToEnglish(int x) {
    static const char *const kDigits[] = {
        "zero",
        "one",
        "two",
        "three",
        "four",
        "five",
        "six",
        "seven",
        "eight",
        "nine",
    };

    if (x < 0) {
        return "negative " + numberToEnglish(-x);
    }

    if (x < 10) {
        return kDigits[x];
    }

    if (x <= 15) {
        static const char *const kSpecialTens[] = {
            "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen",
        };

        return kSpecialTens[x - 10];
    }

    if (x < 20) {
        return std::string(kDigits[x % 10]) + "teen";
    }

    if (x < 100) {
        static const char *const kDecades[] = {
            "twenty", "thirty", "forty", "fifty", "sixty", "seventy",
            "eighty", "ninety",
        };

        return std::string(kDecades[x / 10 - 2]) + kDigits[x % 10];
    }

    return "positively huge!";
}

TEST_F(HidlTest, BazTransposeTest) {
    IBase::StringMatrix5x3 in;
    IBase::StringMatrix3x5 expectedOut;

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 3; ++j) {
            in.s[i][j] = expectedOut.s[j][i] = numberToEnglish(3 * i + j + 1).c_str();
        }
    }

    EXPECT_OK(baz->transpose(
                in,
                [&](const auto &out) {
                    EXPECT_EQ(expectedOut, out);
                }));
}

TEST_F(HidlTest, BazTranspose2Test) {
    hidl_array<hidl_string, 5, 3> in;
    hidl_array<hidl_string, 3, 5> expectedOut;

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 3; ++j) {
            in[i][j] = expectedOut[j][i] = numberToEnglish(3 * i + j + 1).c_str();
        }
    }

    EXPECT_OK(baz->transpose2(
                in,
                [&](const auto &out) {
                    EXPECT_EQ(expectedOut, out);
                }));
}

TEST_F(HidlTest, BazSomeBoolMethodTest) {
    auto result = baz->someBoolMethod(true);
    EXPECT_OK(result);
    EXPECT_EQ(result, false);
}

TEST_F(HidlTest, BazSomeBoolArrayMethodTest) {
    hidl_array<bool, 3> someBoolArray;
    someBoolArray[0] = true;
    someBoolArray[1] = false;
    someBoolArray[2] = true;

    hidl_array<bool, 4> expectedOut;
    expectedOut[0] = false;
    expectedOut[1] = true;
    expectedOut[2] = false;
    expectedOut[3] = true;

    EXPECT_OK(
            baz->someBoolArrayMethod(
                someBoolArray,
                [&](const auto &result) {
                    EXPECT_EQ(expectedOut, result);
                }));
}

TEST_F(HidlTest, BazSomeBoolVectorMethodTest) {
    hidl_vec<bool> someBoolVector, expected;
    someBoolVector.resize(4);
    expected.resize(4);

    for (size_t i = 0; i < someBoolVector.size(); ++i) {
        someBoolVector[i] = ((i & 1) == 0);
        expected[i] = !someBoolVector[i];
    }

    EXPECT_OK(
            baz->someBoolVectorMethod(
                someBoolVector,
                [&](const auto &result) {
                    EXPECT_EQ(expected, result);
                }));
}

TEST_F(HidlTest, BazDoThisMethodTest) {
    EXPECT_OK(baz->doThis(1.0f));
}

TEST_F(HidlTest, BazDoThatAndReturnSomethingMethodTest) {
    auto result = baz->doThatAndReturnSomething(1);
    EXPECT_OK(result);
    EXPECT_EQ(result, 666);
}

TEST_F(HidlTest, BazDoQuiteABitMethodTest) {
    auto result = baz->doQuiteABit(1, 2LL, 3.0f, 4.0);

    EXPECT_OK(result);
    EXPECT_EQ(result, 666.5);
}

TEST_F(HidlTest, BazDoSomethingElseMethodTest) {
    hidl_array<int32_t, 15> param;
    hidl_array<int32_t, 32> expected;

    for (size_t i = 0; i < 15; ++i) {
        param[i] = expected[15 + i] = i;
        expected[i] = 2 * i;
    }

    expected[30] = 1;
    expected[31] = 2;

    EXPECT_OK(
            baz->doSomethingElse(
                param,
                [&](const auto &result) {
                    EXPECT_EQ(expected, result);
                }));
}

TEST_F(HidlTest, BazDoStuffAndReturnAStringMethodTest) {
    std::string expected = "Hello, world!";
    EXPECT_OK(
            baz->doStuffAndReturnAString(
                [&](const auto &result) {
                    EXPECT_EQ(expected, result);
                }));
}

TEST_F(HidlTest, BazMapThisVectorMethodTest) {
    hidl_vec<int32_t> vec_param, expected;
    vec_param.resize(15);
    expected.resize(15);

    for (size_t i = 0; i < 15; ++i) {
        vec_param[i] = i;
        expected[i] = 2 * i;
    }

    EXPECT_OK(
            baz->mapThisVector(
                vec_param,
                [&](const auto &result) {
                    EXPECT_EQ(expected, result);
                }));
}

TEST_F(HidlTest, BazCallMeMethodTest) {
    EXPECT_OK(baz->callMe(new BazCallback()));
}

TEST_F(HidlTest, BazCallMeLaterMethodTest) {
    EXPECT_OK(baz->callMeLater(new BazCallback()));
    EXPECT_OK(baz->iAmFreeNow());
}

TEST_F(HidlTest, BazUseAnEnumMethodTest) {
    auto result = baz->useAnEnum(IBaz::SomeEnum::bar);

    EXPECT_OK(result);
    EXPECT_TRUE(result == IBaz::SomeEnum::quux);
}

TEST_F(HidlTest, BazHaveSomeStringsMethodTest) {
    hidl_array<hidl_string, 3> string_params;
    string_params[0] = "one";
    string_params[1] = "two";
    string_params[2] = "three";

    hidl_array<hidl_string, 2> expected;
    expected[0] = "Hello";
    expected[1] = "World";

    EXPECT_OK(
            baz->haveSomeStrings(
                string_params,
                [&](const auto &result) {
                    EXPECT_EQ(expected, result);
                }));
}

TEST_F(HidlTest, BazHaveAStringVecMethodTest) {
    hidl_vec<hidl_string> string_vec{ "Uno", "Dos", "Tres", "Cuatro" };
    hidl_vec<hidl_string> expected{"Hello", "World"};

    EXPECT_OK(
            baz->haveAStringVec(
                string_vec,
                [&](const auto &result) {
                    EXPECT_EQ(expected, result);
                }));
}

TEST_F(HidlTest, BazReturnABunchOfStringsMethodTest) {
    std::string expectedA = "Eins";
    std::string expectedB = "Zwei";
    std::string expectedC = "Drei";
    EXPECT_OK(
            baz->returnABunchOfStrings(
                [&](const auto &a, const auto &b, const auto &c) {
                    EXPECT_EQ(a, expectedA);
                    EXPECT_EQ(b, expectedB);
                    EXPECT_EQ(c, expectedC);
                }));
}

TEST_F(HidlTest, BazTestArrays) {
    IBase::LotsOfPrimitiveArrays in;

    for (size_t i = 0; i < 128; ++i) {
        in.byte1[i] = i;
        in.boolean1[i] = (i & 4) != 0;
        in.double1[i] = i;
    }

    size_t k = 0;
    for (size_t i = 0; i < 8; ++i) {
        for (size_t j = 0; j < 128; ++j, ++k) {
            in.byte2[i][j] = k;
            in.boolean2[i][j] = (k & 4) != 0;
            in.double2[i][j] = k;
        }
    }

    size_t m = 0;
    for (size_t i = 0; i < 8; ++i) {
        for (size_t j = 0; j < 16; ++j) {
            for (size_t k = 0; k < 128; ++k, ++m) {
                in.byte3[i][j][k] = m;
                in.boolean3[i][j][k] = (m & 4) != 0;
                in.double3[i][j][k] = m;
            }
        }
    }

    EXPECT_OK(
            baz->testArrays(in,
                [&](const auto &out) {
                    EXPECT_EQ(in, out);
                }));
}

TEST_F(HidlTest, BazTestByteVecs) {
    hidl_vec<IBase::ByteOneDim> in;
    in.resize(8);

    size_t k = 0;
    for (size_t i = 0; i < in.size(); ++i) {
        for (size_t j = 0; j < 128; ++j, ++k) {
            in[i][j] = k;
        }
    }

    EXPECT_OK(baz->testByteVecs(
                in, [&](const auto &out) { EXPECT_EQ(in, out); }));
}

TEST_F(HidlTest, BazTestBooleanVecs) {
    hidl_vec<IBase::BooleanOneDim> in;
    in.resize(8);

    size_t k = 0;
    for (size_t i = 0; i < in.size(); ++i) {
        for (size_t j = 0; j < 128; ++j, ++k) {
            in[i][j] = (k & 4) != 0;
        }
    }

    EXPECT_OK(baz->testBooleanVecs(
                in, [&](const auto &out) { EXPECT_EQ(in, out); }));
}

TEST_F(HidlTest, BazTestDoubleVecs) {
    hidl_vec<IBase::DoubleOneDim> in;
    in.resize(8);

    size_t k = 0;
    for (size_t i = 0; i < in.size(); ++i) {
        for (size_t j = 0; j < 128; ++j, ++k) {
            in[i][j] = k;
        }
    }

    EXPECT_OK(baz->testDoubleVecs(
                in, [&](const auto &out) { EXPECT_EQ(in, out); }));
}

TEST_F(HidlTest, SafeUnionNoInitTest) {
    EXPECT_OK(safeunionInterface->newLargeSafeUnion([&](const LargeSafeUnion& safeUnion) {
        EXPECT_EQ(LargeSafeUnion::hidl_discriminator::noinit, safeUnion.getDiscriminator());
    }));
}

TEST_F(HidlTest, SafeUnionSimpleTest) {
    EXPECT_OK(safeunionInterface->newLargeSafeUnion([&](const LargeSafeUnion& safeUnion) {
        EXPECT_OK(safeunionInterface->setA(safeUnion, -5, [&](const LargeSafeUnion& safeUnion) {
            EXPECT_EQ(LargeSafeUnion::hidl_discriminator::a, safeUnion.getDiscriminator());
            EXPECT_EQ(-5, safeUnion.a());

            uint64_t max = std::numeric_limits<uint64_t>::max();
            EXPECT_OK(
                safeunionInterface->setD(safeUnion, max, [&](const LargeSafeUnion& safeUnion) {
                    EXPECT_EQ(LargeSafeUnion::hidl_discriminator::d, safeUnion.getDiscriminator());
                    EXPECT_EQ(max, safeUnion.d());
                }));
        }));
    }));
}

TEST_F(HidlTest, SafeUnionArrayLikeTypesTest) {
    const std::array<int64_t, 5> testArray{1, -2, 3, -4, 5};
    const hidl_vec<uint64_t> testVector{std::numeric_limits<uint64_t>::max()};

    EXPECT_OK(safeunionInterface->newLargeSafeUnion([&](const LargeSafeUnion& safeUnion) {
        EXPECT_OK(
            safeunionInterface->setF(safeUnion, testArray, [&](const LargeSafeUnion& safeUnion) {
                EXPECT_EQ(LargeSafeUnion::hidl_discriminator::f, safeUnion.getDiscriminator());

                for (size_t i = 0; i < testArray.size(); i++) {
                    EXPECT_EQ(testArray[i], safeUnion.f()[i]);
                }
            }));

        EXPECT_OK(
            safeunionInterface->setI(safeUnion, testVector, [&](const LargeSafeUnion& safeUnion) {
                EXPECT_EQ(LargeSafeUnion::hidl_discriminator::i, safeUnion.getDiscriminator());
                EXPECT_EQ(testVector, safeUnion.i());
            }));
    }));
}

TEST_F(HidlTest, SafeUnionStringTypeTest) {
    const std::string testString =
        "This is an inordinately long test string to exercise hidl_string types in safe unions.";

    EXPECT_OK(safeunionInterface->newLargeSafeUnion([&](const LargeSafeUnion& safeUnion) {
        EXPECT_OK(safeunionInterface->setG(
            safeUnion, hidl_string(testString), [&](const LargeSafeUnion& safeUnion) {
                EXPECT_EQ(LargeSafeUnion::hidl_discriminator::g, safeUnion.getDiscriminator());
                EXPECT_EQ(testString, std::string(safeUnion.g()));
            }));
    }));
}

TEST_F(HidlTest, SafeUnionNestedTest) {
    SmallSafeUnion smallSafeUnion;
    smallSafeUnion.a(1);

    EXPECT_OK(safeunionInterface->newLargeSafeUnion([&](const LargeSafeUnion& safeUnion) {
        EXPECT_OK(safeunionInterface->setL(
            safeUnion, smallSafeUnion, [&](const LargeSafeUnion& safeUnion) {
                EXPECT_EQ(LargeSafeUnion::hidl_discriminator::l, safeUnion.getDiscriminator());

                EXPECT_EQ(SmallSafeUnion::hidl_discriminator::a, safeUnion.l().getDiscriminator());
                EXPECT_EQ(1, safeUnion.l().a());
            }));
    }));
}

// does not check for fd equality
static void checkNativeHandlesDataEquality(const native_handle_t* reference,
                                       const native_handle_t* result) {
    if (reference == nullptr || result == nullptr) {
        EXPECT_EQ(reference == nullptr, result == nullptr);
        return;
    }

    ASSERT_NE(reference, result);
    ASSERT_EQ(reference->version, result->version);
    EXPECT_EQ(reference->numFds, result->numFds);
    EXPECT_EQ(reference->numInts, result->numInts);

    int offset = reference->numFds;
    int numInts = reference->numInts;
    EXPECT_TRUE(isArrayEqual(&(reference->data[offset]), &(result->data[offset]), numInts));
}

TEST_F(HidlTest, SafeUnionInterfaceNullHandleTest) {
    InterfaceTypeSafeUnion safeUnion;

    EXPECT_OK(safeunionInterface->setInterfaceF(
        safeUnion, hidl_handle(nullptr), [&](const InterfaceTypeSafeUnion& safeUnion) {
            EXPECT_EQ(InterfaceTypeSafeUnion::hidl_discriminator::f,
                      safeUnion.getDiscriminator());

            checkNativeHandlesDataEquality(nullptr, safeUnion.f().getNativeHandle());
        }));
}

TEST_F(HidlTest, SafeUnionInterfaceTest) {
    const std::array<int8_t, 7> testArray{-1, -2, -3, 0, 1, 2, 3};
    const hidl_vec<hidl_string> testVector{"So", "Many", "Words"};
    const std::string testStringA = "Hello";
    const std::string testStringB = "World";

    const std::array<int, 6> testHandleData{2, -32, 10, -4329454, 11, 24};
    native_handle_t* h = native_handle_create(0, testHandleData.size());
    CHECK(sizeof(testHandleData) == testHandleData.size() * sizeof(int));
    std::memcpy(h->data, testHandleData.data(), sizeof(testHandleData));

    std::vector<hidl_handle> testHandlesVector(256);
    for (size_t i = 0; i < testHandlesVector.size(); i++) {
        testHandlesVector[i].setTo(native_handle_clone(h), true /* shouldOwn */);
    }

    EXPECT_OK(
        safeunionInterface->newInterfaceTypeSafeUnion([&](const InterfaceTypeSafeUnion& safeUnion) {
            EXPECT_EQ(InterfaceTypeSafeUnion::hidl_discriminator::noinit,
                      safeUnion.getDiscriminator());

            EXPECT_OK(safeunionInterface->setInterfaceB(
                safeUnion, testArray, [&](const InterfaceTypeSafeUnion& safeUnion) {
                    EXPECT_EQ(InterfaceTypeSafeUnion::hidl_discriminator::b,
                              safeUnion.getDiscriminator());

                    for (size_t i = 0; i < testArray.size(); i++) {
                        EXPECT_EQ(testArray[i], safeUnion.b()[i]);
                    }
                }));

            EXPECT_OK(safeunionInterface->setInterfaceD(
                safeUnion, testStringA, [&](const InterfaceTypeSafeUnion& safeUnion) {
                    EXPECT_EQ(InterfaceTypeSafeUnion::hidl_discriminator::d,
                              safeUnion.getDiscriminator());
                    EXPECT_EQ(testStringA, safeUnion.d());
                }));

            EXPECT_OK(safeunionInterface->setInterfaceE(
                safeUnion, testVector, [&](const InterfaceTypeSafeUnion& safeUnion) {
                    EXPECT_EQ(InterfaceTypeSafeUnion::hidl_discriminator::e,
                              safeUnion.getDiscriminator());
                    EXPECT_EQ(testVector, safeUnion.e());
                }));

            EXPECT_OK(safeunionInterface->setInterfaceF(
                safeUnion, hidl_handle(h), [&](const InterfaceTypeSafeUnion& safeUnion) {
                    EXPECT_EQ(InterfaceTypeSafeUnion::hidl_discriminator::f,
                              safeUnion.getDiscriminator());

                    const native_handle_t* result = safeUnion.f().getNativeHandle();
                    checkNativeHandlesDataEquality(h, result);
                }));

            EXPECT_OK(safeunionInterface->setInterfaceG(
                safeUnion, testHandlesVector, [&](const InterfaceTypeSafeUnion& safeUnion) {
                    EXPECT_EQ(InterfaceTypeSafeUnion::hidl_discriminator::g,
                              safeUnion.getDiscriminator());

                    for (size_t i = 0; i < testHandlesVector.size(); i++) {
                        checkNativeHandlesDataEquality(h, safeUnion.g()[i].getNativeHandle());
                    }
                }));
        }));

    // Same-process interface calls are not supported in Java, so we use
    // a safe_union instance bound to this (client) process instead of
    // safeunionInterface to exercise this test-case. Ref: b/110957763.
    InterfaceTypeSafeUnion safeUnion;
    safeUnion.c(otherInterface);

    EXPECT_EQ(InterfaceTypeSafeUnion::hidl_discriminator::c, safeUnion.getDiscriminator());
    EXPECT_OK(safeUnion.c()->concatTwoStrings(
        hidl_string(testStringA), hidl_string(testStringB), [&](const hidl_string& result) {
            EXPECT_EQ(testStringA + testStringB, std::string(result));
        }));

    native_handle_delete(h);
}

TEST_F(HidlTest, SafeUnionNullHandleTest) {
    HandleTypeSafeUnion safeUnion;

    EXPECT_OK(safeunionInterface->setHandleA(
        safeUnion, hidl_handle(nullptr), [&](const HandleTypeSafeUnion& safeUnion) {
            EXPECT_EQ(HandleTypeSafeUnion::hidl_discriminator::a,
                      safeUnion.getDiscriminator());

            checkNativeHandlesDataEquality(nullptr, safeUnion.a().getNativeHandle());
        }));
}

TEST_F(HidlTest, SafeUnionSimpleHandleTest) {
    const std::array<int, 6> testData{2, -32, 10, -4329454, 11, 24};
    native_handle_t* h = native_handle_create(0, testData.size());
    ASSERT_EQ(sizeof(testData), testData.size() * sizeof(int));
    std::memcpy(h->data, testData.data(), sizeof(testData));

    std::array<hidl_handle, 5> testArray;
    for (size_t i = 0; i < testArray.size(); i++) {
        testArray[i].setTo(native_handle_clone(h), true /* shouldOwn */);
    }

    std::vector<hidl_handle> testVector(256);
    for (size_t i = 0; i < testVector.size(); i++) {
        testVector[i].setTo(native_handle_clone(h), true /* shouldOwn */);
    }

    EXPECT_OK(
        safeunionInterface->newHandleTypeSafeUnion([&](const HandleTypeSafeUnion& safeUnion) {
            EXPECT_OK(safeunionInterface->setHandleA(
                safeUnion, hidl_handle(h), [&](const HandleTypeSafeUnion& safeUnion) {
                    EXPECT_EQ(HandleTypeSafeUnion::hidl_discriminator::a,
                              safeUnion.getDiscriminator());

                    checkNativeHandlesDataEquality(h, safeUnion.a().getNativeHandle());
                }));

            EXPECT_OK(safeunionInterface->setHandleB(
                safeUnion, testArray, [&](const HandleTypeSafeUnion& safeUnion) {
                    EXPECT_EQ(HandleTypeSafeUnion::hidl_discriminator::b,
                              safeUnion.getDiscriminator());

                    for (size_t i = 0; i < testArray.size(); i++) {
                        checkNativeHandlesDataEquality(h, safeUnion.b()[i].getNativeHandle());
                    }
                }));

            EXPECT_OK(safeunionInterface->setHandleC(
                safeUnion, testVector, [&](const HandleTypeSafeUnion& safeUnion) {
                    EXPECT_EQ(HandleTypeSafeUnion::hidl_discriminator::c,
                              safeUnion.getDiscriminator());

                    for (size_t i = 0; i < testVector.size(); i++) {
                        checkNativeHandlesDataEquality(h, safeUnion.c()[i].getNativeHandle());
                    }
                }));
        }));

    native_handle_delete(h);
}

TEST_F(HidlTest, SafeUnionVecOfHandlesWithOneFdTest) {
    const std::vector<std::string> testStrings{"This ", "is ", "so ", "much ", "data!\n"};
    const std::string testFileName = "/data/local/tmp/SafeUnionVecOfHandlesWithOneFdTest";
    const std::array<int, 6> testData{2, -32, 10, -4329454, 11, 24};
    ASSERT_EQ(sizeof(testData), testData.size() * sizeof(int));

    const std::string goldenResult = std::accumulate(testStrings.begin(),
                                                     testStrings.end(),
                                                     std::string());

    int fd = open(testFileName.c_str(), (O_RDWR | O_TRUNC | O_CREAT), (S_IRUSR | S_IWUSR));
    ASSERT_TRUE(fd >= 0);

    native_handle* h = native_handle_create(1 /* numFds */, testData.size() /* numInts */);
    std::memcpy(&(h->data[1]), testData.data(), sizeof(testData));
    h->data[0] = fd;

    hidl_vec<hidl_handle> testHandles(testStrings.size());
    for (size_t i = 0; i < testHandles.size(); i++) {
        testHandles[i].setTo(native_handle_clone(h), true /* shouldOwn */);
    }

    EXPECT_OK(
        safeunionInterface->newHandleTypeSafeUnion([&](const HandleTypeSafeUnion& safeUnion) {
            EXPECT_OK(safeunionInterface->setHandleC(
                safeUnion, testHandles, [&](const HandleTypeSafeUnion& safeUnion) {
                    EXPECT_EQ(HandleTypeSafeUnion::hidl_discriminator::c,
                              safeUnion.getDiscriminator());

                    for (size_t i = 0; i < safeUnion.c().size(); i++) {
                        const native_handle_t* reference = testHandles[i].getNativeHandle();
                        const native_handle_t* result = safeUnion.c()[i].getNativeHandle();
                        checkNativeHandlesDataEquality(reference, result);

                        // Original FDs should be dup'd
                        int resultFd = result->data[0];
                        EXPECT_NE(reference->data[0], resultFd);

                        EXPECT_TRUE(android::base::WriteStringToFd(testStrings[i], resultFd));
                        EXPECT_EQ(0, fsync(resultFd));
                    }
                }));
        }));

    std::string result;
    lseek(fd, 0, SEEK_SET);

    EXPECT_TRUE(android::base::ReadFdToString(fd, &result));
    EXPECT_EQ(goldenResult, result);

    native_handle_delete(h);
    EXPECT_EQ(0, close(fd));
    EXPECT_EQ(0, remove(testFileName.c_str()));
}

TEST_F(HidlTest, SafeUnionHandleWithMultipleFdsTest) {
    const std::vector<std::string> testStrings{"This ", "is ", "so ", "much ", "data!\n"};
    const std::string testFileName = "/data/local/tmp/SafeUnionHandleWithMultipleFdsTest";
    const std::array<int, 6> testData{2, -32, 10, -4329454, 11, 24};
    ASSERT_EQ(sizeof(testData), testData.size() * sizeof(int));

    const std::string goldenResult = std::accumulate(testStrings.begin(),
                                                     testStrings.end(),
                                                     std::string());

    int fd = open(testFileName.c_str(), (O_RDWR | O_TRUNC | O_CREAT), (S_IRUSR | S_IWUSR));
    ASSERT_TRUE(fd >= 0);

    const int numFds = testStrings.size();
    native_handle* h = native_handle_create(numFds, testData.size() /* numInts */);
    std::memcpy(&(h->data[numFds]), testData.data(), sizeof(testData));
    for (size_t i = 0; i < numFds; i++) {
        h->data[i] = fd;
    }

    hidl_handle testHandle;
    testHandle.setTo(h, false /* shouldOwn */);

    EXPECT_OK(
        safeunionInterface->newHandleTypeSafeUnion([&](const HandleTypeSafeUnion& safeUnion) {
            EXPECT_OK(safeunionInterface->setHandleA(
                safeUnion, testHandle, [&](const HandleTypeSafeUnion& safeUnion) {
                    EXPECT_EQ(HandleTypeSafeUnion::hidl_discriminator::a,
                              safeUnion.getDiscriminator());

                    const native_handle_t* result = safeUnion.a().getNativeHandle();
                    checkNativeHandlesDataEquality(h, result);

                    for (size_t i = 0; i < result->numFds; i++) {
                        // Original FDs should be dup'd
                        int resultFd = result->data[i];
                        EXPECT_NE(h->data[i], resultFd);

                        EXPECT_TRUE(android::base::WriteStringToFd(testStrings[i], resultFd));
                        EXPECT_EQ(0, fsync(resultFd));
                    }
                }));
        }));

    std::string result;
    lseek(fd, 0, SEEK_SET);

    EXPECT_TRUE(android::base::ReadFdToString(fd, &result));
    EXPECT_EQ(goldenResult, result);

    native_handle_delete(h);
    EXPECT_EQ(0, close(fd));
    EXPECT_EQ(0, remove(testFileName.c_str()));
}

TEST_F(HidlTest, SafeUnionEqualityTest) {
    EXPECT_OK(safeunionInterface->newLargeSafeUnion([&](const LargeSafeUnion& one) {
        EXPECT_OK(safeunionInterface->newLargeSafeUnion([&](const LargeSafeUnion& two) {
            EXPECT_TRUE(one == two);
            EXPECT_FALSE(one != two);
        }));

        EXPECT_OK(safeunionInterface->setA(one, 1, [&](const LargeSafeUnion& one) {
            EXPECT_OK(safeunionInterface->newLargeSafeUnion([&](const LargeSafeUnion& two) {
                EXPECT_FALSE(one == two);
                EXPECT_TRUE(one != two);
            }));

            EXPECT_OK(safeunionInterface->newLargeSafeUnion([&](const LargeSafeUnion& two) {
                EXPECT_OK(safeunionInterface->setB(two, 1, [&](const LargeSafeUnion& two) {
                    EXPECT_FALSE(one == two);
                    EXPECT_TRUE(one != two);
                }));
            }));

            EXPECT_OK(safeunionInterface->newLargeSafeUnion([&](const LargeSafeUnion& two) {
                EXPECT_OK(safeunionInterface->setA(two, 2, [&](const LargeSafeUnion& two) {
                    EXPECT_FALSE(one == two);
                    EXPECT_TRUE(one != two);
                }));
            }));

            EXPECT_OK(safeunionInterface->newLargeSafeUnion([&](const LargeSafeUnion& two) {
                EXPECT_OK(safeunionInterface->setA(two, 1, [&](const LargeSafeUnion& two) {
                    EXPECT_TRUE(one == two);
                    EXPECT_FALSE(one != two);
                }));
            }));
        }));
    }));
}

int main(int argc, char **argv) {
    setenv("TREBLE_TESTING_OVERRIDE", "true", true);

    using namespace android::hardware;

    const char *me = argv[0];

    bool wantClient = false;
    bool wantServer = false;

    int res;
    while ((res = getopt(argc, argv, "chs")) >= 0) {
        switch (res) {
            case 'c':
            {
                wantClient = true;
                break;
            }

            case 's':
            {
                wantServer = true;
                break;
            }

            case '?':
            case 'h':
            default:
            {
                usage(me);
                exit(1);
                break;
            }
        }
    }

    if ((!wantClient && !wantServer) || (wantClient && wantServer)) {
        usage(me);
        exit(1);
    }

    if (wantClient) {
        ::testing::AddGlobalTestEnvironment(new HidlEnvironment);
        ::testing::InitGoogleTest(&argc, argv);
        int status = RUN_ALL_TESTS();
        return status;
    }

    ::android::status_t status;
    configureRpcThreadpool(1, true);

    status = registerPassthroughServiceImplementation<IBaz>();
    CHECK(status == ::android::OK) << "IBaz didn't register";

    status = registerPassthroughServiceImplementation<ISafeUnion>();
    CHECK(status == ::android::OK) << "ISafeUnion didn't register";

    sp<IOtherInterface> otherInterface = new OtherInterface();
    status = otherInterface->registerAsService();
    CHECK(status == ::android::OK) << "IOtherInterface didn't register";

    joinRpcThreadpool();
    return 0;
}
