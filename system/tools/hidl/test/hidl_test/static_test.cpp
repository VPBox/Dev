
#include <android/hardware/tests/bar/1.0/IImportRules.h>
#include <android/hardware/tests/expression/1.0/IExpression.h>
#include <android/hardware/tests/foo/1.0/IFoo.h>

using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hardware::hidl_array;
using ::android::hardware::tests::bar::V1_0::IImportRules;
using ::android::hardware::tests::expression::V1_0::IExpression;
using ::android::hardware::tests::foo::V1_0::IFoo;
using ::android::sp;

namespace {

// Check array dimension order
static_assert(std::is_same<IFoo::multidimArrayOne, hidl_array<int32_t, 5, 6, 7>>::value,
              "hidl-gen output array dimention order is incorrect");
static_assert(std::is_same<IFoo::multidimArrayTwo, hidl_array<int32_t, 8, 9, 10, 5, 6, 7>>::value,
              "hidl-gen output array dimention order is incorrect");
static_assert(
    std::is_same<IFoo::multidimArrayThree, hidl_array<int32_t, 2, 3, 4, 8, 9, 10, 5, 6, 7>>::value,
    "hidl-gen output array dimention order is incorrect");

// Check correct type in ambiguous case
static_assert(std::is_same<decltype(IFoo::S1::foo), IFoo::InnerTestStruct>::value,
              "hidl-gen wrong (inner) type in output");
static_assert(!std::is_same<decltype(IFoo::S1::foo), IFoo::S1::InnerTestStruct>::value,
              "hidl-gen wrong (inner) type in output");
static_assert((int32_t) decltype(IFoo::S2::foo)::VALUE == 0,
              "hidl-gen wrong (inner) type in output");

// Ensure (statically) that the types in IImportRules resolves to the correct types by
// overriding the methods with fully namespaced types as arguments.
struct MyImportRules : public IImportRules {
    Return<void> rule0a(
        const ::android::hardware::tests::bar::V1_0::IImportRules::Outer&) override {
        return Void();
    }

    Return<void> rule0a1(
        const ::android::hardware::tests::bar::V1_0::IImportRules::Outer&) override {
        return Void();
    }

    Return<void> rule0b(
        const ::android::hardware::tests::bar::V1_0::IImportRules::Outer&) override {
        return Void();
    }

    Return<void> rule0b1(
        const ::android::hardware::tests::bar::V1_0::IImportRules::Outer&) override {
        return Void();
    }

    Return<void> rule0c(const ::android::hardware::tests::foo::V1_0::Outer&) override {
        return Void();
    }

    Return<void> rule0d(const ::android::hardware::tests::foo::V1_0::Outer&) override {
        return Void();
    }

    Return<void> rule0e(
        const ::android::hardware::tests::bar::V1_0::IImportRules::Outer::Inner&) override {
        return Void();
    }

    Return<void> rule0f(
        const ::android::hardware::tests::bar::V1_0::IImportRules::Outer::Inner&) override {
        return Void();
    }

    Return<void> rule0g(const ::android::hardware::tests::foo::V1_0::Outer::Inner&) override {
        return Void();
    }

    Return<void> rule0h(const ::android::hardware::tests::foo::V1_0::Outer::Inner&) override {
        return Void();
    }

    Return<void> rule1a(const ::android::hardware::tests::bar::V1_0::Def&) override {
        return Void();
    }

    Return<void> rule1b(const ::android::hardware::tests::foo::V1_0::Def&) override {
        return Void();
    }

    Return<void> rule2a(const ::android::hardware::tests::foo::V1_0::Unrelated&) override {
        return Void();
    }

    Return<void> rule2b(const sp<::android::hardware::tests::foo::V1_0::IFooCallback>&) override {
        return Void();
    }
};

template <typename E>
constexpr bool operator==(E actual, typename std::underlying_type<E>::type expected) {
    return expected == static_cast<typename std::underlying_type<E>::type>(actual);
}

// See IExpression.hal
// Use the C++ compiler to compute these expressions and make sure hidl-gen emits the same result.
static_assert(IExpression::UInt64LiteralTypeGuessing::noSuffixDec1 == 0, "");
static_assert(IExpression::UInt64LiteralTypeGuessing::noSuffixDec2 == 1, "");
static_assert(IExpression::UInt64LiteralTypeGuessing::noSuffixDec3 == -1, "");
static_assert(IExpression::UInt64LiteralTypeGuessing::noSuffixDec4 == ~0, "");
static_assert(IExpression::UInt64LiteralTypeGuessing::noSuffixDec5 == 2147483647, "");
static_assert(IExpression::UInt64LiteralTypeGuessing::noSuffixDec6 == -2147483648, "");
static_assert(IExpression::UInt64LiteralTypeGuessing::noSuffixDec7 == 2147483648, "");
static_assert(IExpression::UInt64LiteralTypeGuessing::noSuffixDec8 == -2147483649, "");
static_assert(IExpression::UInt64LiteralTypeGuessing::noSuffixDec9 == ~(-1), "");
static_assert(IExpression::UInt64LiteralTypeGuessing::noSuffixHex1 == 0x7fffffff, "");
static_assert(IExpression::UInt64LiteralTypeGuessing::noSuffixHex2 == 0x80000000, "");
static_assert(IExpression::UInt64LiteralTypeGuessing::noSuffixHex3 == 0xffffffff, "");
static_assert(IExpression::UInt64LiteralTypeGuessing::longHex1 == 0xffffffffL, "");
static_assert(IExpression::UInt64LiteralTypeGuessing::longHex2 == 0Xfffffffff, "");
static_assert(IExpression::UInt64LiteralTypeGuessing::longHex3 == 0x7fffffffffffffff, "");
static_assert(IExpression::UInt64LiteralTypeGuessing::longHex4 == 0x8000000000000000, "");
static_assert(IExpression::UInt64LiteralTypeGuessing::longHex5 == 0xFFFFFFFFFFFFFFFF, "");

static_assert(IExpression::SuffixedLiteralTypeGuessing::decInt32_1 == 1, "");
static_assert(IExpression::SuffixedLiteralTypeGuessing::decInt32_2 == 1, "");
static_assert(IExpression::SuffixedLiteralTypeGuessing::decInt64_1 == 1, "");
static_assert(IExpression::SuffixedLiteralTypeGuessing::decInt64_2 == 1, "");
static_assert(IExpression::SuffixedLiteralTypeGuessing::decInt64_3 == 1, "");
static_assert(IExpression::SuffixedLiteralTypeGuessing::decInt64_4 == 1, "");
static_assert(IExpression::SuffixedLiteralTypeGuessing::hexInt32_1 == 1, "");
static_assert(IExpression::SuffixedLiteralTypeGuessing::hexUInt32_1 == 1, "");
static_assert(IExpression::SuffixedLiteralTypeGuessing::hexUInt32_2 == 1, "");
static_assert(IExpression::SuffixedLiteralTypeGuessing::hexInt64_1 == 1, "");
static_assert(IExpression::SuffixedLiteralTypeGuessing::hexUInt64_1 == 1, "");

static_assert(IExpression::Int64LiteralTypeGuessing::noSuffixDec11 == -2147483648,
              "1 + 0x7fffffff");
static_assert(IExpression::Int64LiteralTypeGuessing::noSuffixDec12 == 2147483647, "0x80000000 - 1");

static_assert(IExpression::Int32BitShifting::int32BitShift1 == 1 << 31, "");
static_assert(IExpression::UInt32BitShifting::uint32BitShift1 == 1 << 31, "");
static_assert(IExpression::Int64BitShifting::int64BitShift1 == 1LL << 63, "1l << 63");
static_assert(IExpression::UInt64BitShifting::uint64BitShift1 == 1LL << 63, "1l << 63");

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconstant-logical-operand"
#pragma clang diagnostic ignored "-Wlogical-op-parentheses"
#pragma clang diagnostic ignored "-Wbitwise-op-parentheses"

static_assert(IExpression::Precedence::literal == (4), "");
static_assert(IExpression::Precedence::neg == (-4), "");
static_assert(IExpression::Precedence::literalL == (-4L), "");
static_assert(IExpression::Precedence::hex == (0xffffffff), "");
static_assert(IExpression::Precedence::hexLong == (0xffffffffL), "");
static_assert(IExpression::Precedence::hexLong2 == (0xfffffffff), "");
static_assert(IExpression::Precedence::simpleArithmetic == (4 + 1), "");
static_assert(IExpression::Precedence::simpleArithmetic2 == (2 + 3 - 4), "");
static_assert(IExpression::Precedence::simpleArithmetic3 == (2 - 3 + 4), "");
static_assert(IExpression::Precedence::simpleBoolExpr == (1 == 4), "");
static_assert(IExpression::Precedence::simpleLogical == (1 && 1), "");
static_assert(IExpression::Precedence::simpleLogical2 == (1 || 1 && 0),
              "&& higher than || in HIDL");
static_assert(IExpression::Precedence::simpleComp == (1 < 2), "");
static_assert(IExpression::Precedence::boolExpr1 == (!((3 != 4 || (2 < 3 <= 3 > 4)) >= 0)), "");
static_assert(IExpression::Precedence::boolExpr == (1 == 7 && !((3 != 4 || (2 < 3 <= 3 > 4)) >= 0)),
              "");
static_assert(IExpression::Precedence::simpleBitShift == (1 << 2), "");
static_assert(IExpression::Precedence::simpleBitShift2 == (4 >> 1), "");
// Shifting negative number of bits is UB in HIDL, so the value is not tested.
// static_assert(IExpression::Precedence::simpleBitShiftNeg == (4 << -1), "");
static_assert(IExpression::Precedence::simpleArithmeticRightShift == (1 << 31 >> 31), "");
static_assert(IExpression::Precedence::simpleBitExpr == (1 | 16 >> 2), "");
static_assert(IExpression::Precedence::simpleBitExpr2 == (0x0f ^ 0x33 & 0x99),
              "& higher than ^ in HIDL");
static_assert(IExpression::Precedence::bitExpr == (~42 & (1 << 3 | 16 >> 2) ^ 7),
              "& higher than ^ in HIDL");
static_assert(IExpression::Precedence::arithmeticExpr == (2 + 3 - 4 * -7 / (10 % 3)), "");
static_assert(IExpression::Precedence::messyExpr == (2 + (-3 & 4 / 7)), "");
static_assert(IExpression::Precedence::paranExpr == ((((((1 + 1)))))), "");
static_assert(IExpression::Precedence::ternary == (1 ? 2 : 3), "");
static_assert(IExpression::Precedence::ternary2 == (1 && 2 ? 3 : 4), "");
static_assert(IExpression::Precedence::complicatedTernary2 == (1 - 1 && 2 + 3 || 5 ? 7 * 8 : -3),
              "&& higher than || in HIDL");

#pragma clang diagnostic pop

static_assert(IExpression::OperatorSanityCheck::plus == 1, "");
static_assert(IExpression::OperatorSanityCheck::minus == 1, "");
static_assert(IExpression::OperatorSanityCheck::product == 1, "");
static_assert(IExpression::OperatorSanityCheck::division == 1, "");
static_assert(IExpression::OperatorSanityCheck::mod == 1, "");
static_assert(IExpression::OperatorSanityCheck::bit_or == 1, "");
static_assert(IExpression::OperatorSanityCheck::bit_or2 == 1, "");
static_assert(IExpression::OperatorSanityCheck::bit_and == 1, "");
static_assert(IExpression::OperatorSanityCheck::bit_xor == 1, "");
static_assert(IExpression::OperatorSanityCheck::lt1 == 1, "");
static_assert(IExpression::OperatorSanityCheck::lt2 == 1, "");
static_assert(IExpression::OperatorSanityCheck::gt1 == 1, "");
static_assert(IExpression::OperatorSanityCheck::gt2 == 1, "");
static_assert(IExpression::OperatorSanityCheck::gte1 == 1, "");
static_assert(IExpression::OperatorSanityCheck::gte2 == 1, "");
static_assert(IExpression::OperatorSanityCheck::lte1 == 1, "");
static_assert(IExpression::OperatorSanityCheck::lte2 == 1, "");
static_assert(IExpression::OperatorSanityCheck::ne1 == 1, "");
static_assert(IExpression::OperatorSanityCheck::ne2 == 1, "");
static_assert(IExpression::OperatorSanityCheck::lshift == 1, "");
static_assert(IExpression::OperatorSanityCheck::rshift == 1, "");
static_assert(IExpression::OperatorSanityCheck::logor1 == 1, "");
static_assert(IExpression::OperatorSanityCheck::logor2 == 1, "");
static_assert(IExpression::OperatorSanityCheck::logor3 == 1, "");
static_assert(IExpression::OperatorSanityCheck::logor4 == 1, "");
static_assert(IExpression::OperatorSanityCheck::logand1 == 1, "");
static_assert(IExpression::OperatorSanityCheck::logand2 == 1, "");
static_assert(IExpression::OperatorSanityCheck::logand3 == 1, "");
static_assert(IExpression::OperatorSanityCheck::logand4 == 1, "");

static_assert(IExpression::Grayscale::WHITE == 126, "");
static_assert(IExpression::Grayscale::GRAY == 127, "");
static_assert(IExpression::Grayscale::DARK_GRAY == -128, "");
static_assert(IExpression::Grayscale::BLACK == -127, "");

static_assert(IExpression::Color::RED == -126, "");
static_assert(IExpression::Color::RUBY == 0, "");
static_assert(IExpression::Color::GREEN == 1, "");
static_assert(IExpression::Color::BLUE == 5, "");
static_assert(IExpression::Color::CYAN == 6, "");
static_assert(IExpression::Color::ORANGE == 7, "");
static_assert(IExpression::Color::ROSE == 126, "");

static_assert(std::is_same<std::underlying_type<IExpression::Foo1>::type,
                           std::underlying_type<IExpression::Foo2>::type>::value,
              "");
static_assert(std::is_same<std::underlying_type<IExpression::Foo1>::type,
                           std::underlying_type<IExpression::Foo3>::type>::value,
              "");
static_assert(std::is_same<std::underlying_type<IExpression::Foo1>::type,
                           std::underlying_type<IExpression::Foo4>::type>::value,
              "");
static_assert(IExpression::Foo3::BAR1 == 0, "");
static_assert(IExpression::Foo3::BAR2 == 10, "");
static_assert(IExpression::Foo4::BAR3 == 11, "");
static_assert(IExpression::Foo4::BAR4 == 21, "");

static_assert(IExpression::Number::MAX == 255, "");
static_assert(IExpression::Number::MAX_PLUS_1 == 0, "");
static_assert(IExpression::Number::MAX_PLUS_2 == 1, "");

static_assert(IExpression::Constants::CONST_FOO == 0, "");
static_assert(IExpression::Constants::CONST_BAR == 70, "");
static_assert(IExpression::Constants::MAX_ARRAY_SIZE == 20, "");
static_assert(IExpression::Constants::MAX_ARRAY_SIZE2 == 21, "");
static_assert(IExpression::Constants::MAX_ARRAY_SIZE3 == 40, "");
static_assert(IExpression::Constants::MY_INT32_MAX_MINUS_1 == 0x7FFFFFFE, "");
static_assert(IExpression::Constants::MY_INT32_MAX == 0x7FFFFFFF, "");
static_assert(IExpression::Constants::MY_INT32_MIN == 0x80000000, "");
static_assert(IExpression::Constants::MY_INT32_MIN_PLUS_1 == 0x80000001, "");

// Ensure (statically) that the array sizes in function arguments are correct.
struct Expression : public IExpression {
    Return<void> foo1(const hidl_array<int32_t, 1>&) override { return Void(); }
    Return<void> foo2(const hidl_array<int32_t, 13>&) override { return Void(); }
    Return<void> foo3(const hidl_array<int32_t, 20>&) override { return Void(); }
};

}  // anonymous namespace
