#define LOG_TAG "hidl_test_client"

#include "FooCallback.h"
#include "hidl_test.h"

#include <android-base/file.h>
#include <android-base/logging.h>

#include <android/hidl/manager/1.0/IServiceNotification.h>
#include <android/hidl/manager/1.2/IServiceManager.h>

#include <android/hidl/allocator/1.0/IAllocator.h>
#include <android/hidl/memory/1.0/IMemory.h>
#include <android/hidl/memory/token/1.0/IMemoryToken.h>
#include <android/hidl/token/1.0/ITokenManager.h>

#include <android/hardware/tests/bar/1.0/BnHwBar.h>
#include <android/hardware/tests/bar/1.0/BpHwBar.h>
#include <android/hardware/tests/bar/1.0/IBar.h>
#include <android/hardware/tests/bar/1.0/IComplicated.h>
#include <android/hardware/tests/bar/1.0/IImportRules.h>
#include <android/hardware/tests/baz/1.0/IBaz.h>
#include <android/hardware/tests/expression/1.0/IExpression.h>
#include <android/hardware/tests/foo/1.0/BnHwSimple.h>
#include <android/hardware/tests/foo/1.0/BpHwSimple.h>
#include <android/hardware/tests/foo/1.0/BsSimple.h>
#include <android/hardware/tests/foo/1.0/IFoo.h>
#include <android/hardware/tests/hash/1.0/IHash.h>
#include <android/hardware/tests/inheritance/1.0/IChild.h>
#include <android/hardware/tests/inheritance/1.0/IFetcher.h>
#include <android/hardware/tests/inheritance/1.0/IGrandparent.h>
#include <android/hardware/tests/inheritance/1.0/IParent.h>
#include <android/hardware/tests/memory/1.0/IMemoryTest.h>
#include <android/hardware/tests/multithread/1.0/IMultithread.h>
#include <android/hardware/tests/pointer/1.0/IGraph.h>
#include <android/hardware/tests/pointer/1.0/IPointer.h>
#include <android/hardware/tests/safeunion/1.0/IOtherInterface.h>
#include <android/hardware/tests/safeunion/1.0/ISafeUnion.h>
#include <android/hardware/tests/safeunion/cpp/1.0/ICppSafeUnion.h>
#include <android/hardware/tests/trie/1.0/ITrie.h>

#include <gtest/gtest.h>
#if GTEST_IS_THREADSAFE
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <pthread.h>
#else
#error "GTest did not detect pthread library."
#endif

#include <getopt.h>
#include <inttypes.h>
#include <algorithm>
#include <condition_variable>
#include <fstream>
#include <future>
#include <limits>
#include <mutex>
#include <random>
#include <set>
#include <sstream>
#include <sys/stat.h>
#include <thread>
#include <type_traits>
#include <unordered_set>
#include <utility>
#include <vector>

#include <hidl-test/FooHelper.h>
#include <hidl-test/PointerHelper.h>
#include <hidl-util/FQName.h>

#include <hidl/ServiceManagement.h>
#include <hidl/Status.h>
#include <hidlmemory/HidlMemoryToken.h>
#include <hidlmemory/mapping.h>

#include <utils/Condition.h>
#include <utils/Timers.h>

#define EXPECT_OK(__ret__) EXPECT_TRUE(isOk(__ret__))
#define EXPECT_FAIL(__ret__) EXPECT_FALSE(isOk(__ret__))
#define EXPECT_ARRAYEQ(__a1__, __a2__, __size__) EXPECT_TRUE(isArrayEqual(__a1__, __a2__, __size__))

// TODO uncomment this when kernel is patched with pointer changes.
//#define HIDL_RUN_POINTER_TESTS 1

// forward declarations.
class HidlEnvironment;

// static storage
enum TestMode {
    BINDERIZED,
    PASSTHROUGH
};

static HidlEnvironment *gHidlEnvironment = nullptr;

using ::android::Condition;
using ::android::DELAY_NS;
using ::android::DELAY_S;
using ::android::FQName;
using ::android::MultiDimensionalToString;
using ::android::Mutex;
using ::android::ONEWAY_TOLERANCE_NS;
using ::android::sp;
using ::android::to_string;
using ::android::TOLERANCE_NS;
using ::android::wp;
using ::android::hardware::GrantorDescriptor;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_death_recipient;
using ::android::hardware::hidl_handle;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::HidlMemory;
using ::android::hardware::MQDescriptor;
using ::android::hardware::MQFlavor;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hardware::tests::bar::V1_0::IBar;
using ::android::hardware::tests::bar::V1_0::IComplicated;
using ::android::hardware::tests::baz::V1_0::IBaz;
using ::android::hardware::tests::expression::V1_0::IExpression;
using ::android::hardware::tests::foo::V1_0::Abc;
using ::android::hardware::tests::foo::V1_0::IFoo;
using ::android::hardware::tests::foo::V1_0::IFooCallback;
using ::android::hardware::tests::foo::V1_0::ISimple;
using ::android::hardware::tests::foo::V1_0::implementation::FooCallback;
using ::android::hardware::tests::hash::V1_0::IHash;
using ::android::hardware::tests::inheritance::V1_0::IChild;
using ::android::hardware::tests::inheritance::V1_0::IFetcher;
using ::android::hardware::tests::inheritance::V1_0::IGrandparent;
using ::android::hardware::tests::inheritance::V1_0::IParent;
using ::android::hardware::tests::memory::V1_0::IMemoryTest;
using ::android::hardware::tests::multithread::V1_0::IMultithread;
using ::android::hardware::tests::pointer::V1_0::IGraph;
using ::android::hardware::tests::pointer::V1_0::IPointer;
using ::android::hardware::tests::safeunion::cpp::V1_0::ICppSafeUnion;
using ::android::hardware::tests::safeunion::V1_0::IOtherInterface;
using ::android::hardware::tests::safeunion::V1_0::ISafeUnion;
using ::android::hardware::tests::trie::V1_0::ITrie;
using ::android::hardware::tests::trie::V1_0::TrieNode;
using ::android::hidl::allocator::V1_0::IAllocator;
using ::android::hidl::base::V1_0::IBase;
using ::android::hidl::manager::V1_0::IServiceNotification;
using ::android::hidl::manager::V1_2::IServiceManager;
using ::android::hidl::memory::block::V1_0::MemoryBlock;
using ::android::hidl::memory::token::V1_0::IMemoryToken;
using ::android::hidl::memory::V1_0::IMemory;
using ::android::hidl::token::V1_0::ITokenManager;
using std::to_string;

using HandleTypeSafeUnion = ISafeUnion::HandleTypeSafeUnion;
using InterfaceTypeSafeUnion = ISafeUnion::InterfaceTypeSafeUnion;
using LargeSafeUnion = ISafeUnion::LargeSafeUnion;
using SmallSafeUnion = ISafeUnion::SmallSafeUnion;

template <typename T>
using hidl_enum_range = ::android::hardware::hidl_enum_range<T>;

template <typename T>
static inline ::testing::AssertionResult isOk(const ::android::hardware::Return<T> &ret) {
    return ret.isOk()
        ? (::testing::AssertionSuccess() << ret.description())
        : (::testing::AssertionFailure() << ret.description());
}

template<typename T, typename S>
static inline bool isArrayEqual(const T arr1, const S arr2, size_t size) {
    for(size_t i = 0; i < size; i++)
        if(arr1[i] != arr2[i])
            return false;
    return true;
}

template<typename T>
std::string to_string(std::set<T> set) {
    std::stringstream ss;
    ss << "{";

    bool first = true;
    for (const T &item : set) {
        if (first) {
            first = false;
        } else {
            ss << ", ";
        }

        ss << to_string(item);
    }

    ss << "}";

    return ss.str();
}

// does not check for fd equality
static void checkNativeHandlesDataEquality(const native_handle_t* reference,
                                           const native_handle_t* result) {
    if (reference == nullptr || result == nullptr) {
        EXPECT_EQ(reference, result);
        return;
    }

    ASSERT_EQ(reference->version, result->version);
    EXPECT_EQ(reference->numFds, result->numFds);
    EXPECT_EQ(reference->numInts, result->numInts);

    int offset = reference->numFds;
    int numInts = reference->numInts;
    EXPECT_ARRAYEQ(&(reference->data[offset]), &(result->data[offset]), numInts);
}

template <typename T, MQFlavor flavor>
static void checkMQDescriptorEquality(const MQDescriptor<T, flavor>& expected,
                                      const MQDescriptor<T, flavor>& actual) {
    checkNativeHandlesDataEquality(expected.handle(), actual.handle());
    EXPECT_EQ(expected.grantors().size(), actual.grantors().size());
    EXPECT_EQ(expected.getQuantum(), actual.getQuantum());
    EXPECT_EQ(expected.getFlags(), actual.getFlags());
}

struct Simple : public ISimple {
    Simple(int32_t cookie)
        : mCookie(cookie) {
    }

    Return<int32_t> getCookie() override {
        return mCookie;
    }

    Return<void> customVecInt(customVecInt_cb _cb) override {
        _cb(hidl_vec<int32_t>());
        return Void();
    }

    Return<void> customVecStr(customVecStr_cb _cb) override {
        hidl_vec<hidl_string> vec;
        vec.resize(2);
        _cb(vec);
        return Void();
    }

    Return<void> mystr(mystr_cb _cb) override {
        _cb(hidl_string());
        return Void();
    }

    Return<void> myhandle(myhandle_cb _cb) override {
        auto h = native_handle_create(0, 1);
        _cb(h);
        native_handle_delete(h);
        return Void();
    }

private:
    int32_t mCookie;
};

struct SimpleParent : public IParent {
    Return<void> doGrandparent() override {
        return Void();
    }
    Return<void> doParent() override {
        return Void();
    }
};

struct SimpleChild : public IChild {
    Return<void> doGrandparent() override {
        return Void();
    }
    Return <void> doParent() override {
        return Void();
    }
    Return <void> doChild() override {
        return Void();
    }
};

struct Complicated : public IComplicated {
    Complicated(int32_t cookie)
        : mCookie(cookie) {
    }

    Return<int32_t> getCookie() override {
        return mCookie;
    }

    Return<void> customVecInt(customVecInt_cb _cb) override {
        _cb(hidl_vec<int32_t>());
        return Void();
    }
    Return<void> customVecStr(customVecStr_cb _cb) override {
        hidl_vec<hidl_string> vec;
        vec.resize(2);
        _cb(vec);
        return Void();
    }

    Return<void> mystr(mystr_cb _cb) override {
        _cb(hidl_string());
        return Void();
    }

    Return<void> myhandle(myhandle_cb _cb) override {
        auto h = native_handle_create(0, 1);
        _cb(h);
        native_handle_delete(h);
        return Void();
    }

private:
    int32_t mCookie;
};

struct OtherInterface : public IOtherInterface {
    Return<void> concatTwoStrings(const hidl_string& a, const hidl_string& b,
                                  concatTwoStrings_cb _hidl_cb) override {
        hidl_string result = std::string(a) + std::string(b);
        _hidl_cb(result);

        return Void();
    }
};

struct ServiceNotification : public IServiceNotification {
    std::mutex mutex;
    std::condition_variable condition;

    Return<void> onRegistration(const hidl_string &fqName,
                                const hidl_string &name,
                                bool preexisting) override {
        if (preexisting) {
            // not interested in things registered from previous runs of hidl_test
            return Void();
        }

        std::unique_lock<std::mutex> lock(mutex);

        mRegistered.push_back(std::string(fqName.c_str()) + "/" + name.c_str());

        lock.unlock();
        condition.notify_one();

        return Void();
    }

    const std::vector<std::string> &getRegistrations() const {
        return mRegistered;
    }

private:
    std::vector<std::string> mRegistered{};
};

class HidlEnvironment : public ::testing::Environment {
public:
    sp<IServiceManager> manager;
    sp<ITokenManager> tokenManager;
    sp<IAllocator> ashmemAllocator;
    sp<IMemoryTest> memoryTest;
    sp<IFetcher> fetcher;
    sp<IFoo> foo;
    sp<IBaz> baz;
    sp<IBaz> dyingBaz;
    sp<IBar> bar;
    sp<IGraph> graphInterface;
    sp<IPointer> pointerInterface;
    sp<IPointer> validationPointerInterface;
    sp<IMultithread> multithreadInterface;
    sp<ITrie> trieInterface;
    sp<ICppSafeUnion> cppSafeunionInterface;
    sp<ISafeUnion> safeunionInterface;
    TestMode mode;
    bool enableDelayMeasurementTests;
    HidlEnvironment(TestMode mode, bool enableDelayMeasurementTests) :
        mode(mode), enableDelayMeasurementTests(enableDelayMeasurementTests) {};

    void getServices() {
        manager = IServiceManager::getService();

        // alternatively:
        // manager = defaultServiceManager()

        ASSERT_NE(manager, nullptr);
        ASSERT_TRUE(manager->isRemote()); // manager is always remote

        tokenManager = ITokenManager::getService();
        ASSERT_NE(tokenManager, nullptr);
        ASSERT_TRUE(tokenManager->isRemote()); // tokenManager is always remote

        ashmemAllocator = IAllocator::getService("ashmem");
        ASSERT_NE(ashmemAllocator, nullptr);
        ASSERT_TRUE(ashmemAllocator->isRemote()); // allocator is always remote

        // getStub is true if we are in passthrough mode to skip checking
        // binderized server, false for binderized mode.

        memoryTest = IMemoryTest::getService("memory", mode == PASSTHROUGH /* getStub */);
        ASSERT_NE(memoryTest, nullptr);
        ASSERT_EQ(memoryTest->isRemote(), mode == BINDERIZED);

        fetcher = IFetcher::getService("fetcher", mode == PASSTHROUGH /* getStub */);
        ASSERT_NE(fetcher, nullptr);
        ASSERT_EQ(fetcher->isRemote(), mode == BINDERIZED);

        foo = IFoo::getService("foo", mode == PASSTHROUGH /* getStub */);
        ASSERT_NE(foo, nullptr);
        ASSERT_EQ(foo->isRemote(), mode == BINDERIZED);

        baz = IBaz::getService("baz", mode == PASSTHROUGH /* getStub */);
        ASSERT_NE(baz, nullptr);
        ASSERT_EQ(baz->isRemote(), mode == BINDERIZED);

        dyingBaz = IBaz::getService("dyingBaz", mode == PASSTHROUGH /* getStub */);
        ASSERT_NE(dyingBaz, nullptr);
        ASSERT_EQ(dyingBaz->isRemote(), mode == BINDERIZED);

        bar = IBar::getService("foo", mode == PASSTHROUGH /* getStub */);
        ASSERT_NE(bar, nullptr);
        ASSERT_EQ(bar->isRemote(), mode == BINDERIZED);

        graphInterface = IGraph::getService("graph", mode == PASSTHROUGH /* getStub */);
        ASSERT_NE(graphInterface, nullptr);
        ASSERT_EQ(graphInterface->isRemote(), mode == BINDERIZED);

        pointerInterface = IPointer::getService("pointer", mode == PASSTHROUGH /* getStub */);
        ASSERT_NE(pointerInterface, nullptr);
        ASSERT_EQ(pointerInterface->isRemote(), mode == BINDERIZED);

        // use passthrough mode as the validation object.
        validationPointerInterface = IPointer::getService("pointer", true /* getStub */);
        ASSERT_NE(validationPointerInterface, nullptr);

        multithreadInterface =
            IMultithread::getService("multithread", mode == PASSTHROUGH /* getStub */);
        ASSERT_NE(multithreadInterface, nullptr);
        ASSERT_EQ(multithreadInterface->isRemote(), mode == BINDERIZED);

        trieInterface = ITrie::getService("trie", mode == PASSTHROUGH /* getStub */);
        ASSERT_NE(trieInterface, nullptr);
        ASSERT_EQ(trieInterface->isRemote(), mode == BINDERIZED);

        cppSafeunionInterface =
            ICppSafeUnion::getService("default", mode == PASSTHROUGH /* getStub */);
        ASSERT_NE(cppSafeunionInterface, nullptr);
        ASSERT_EQ(cppSafeunionInterface->isRemote(), mode == BINDERIZED);

        safeunionInterface = ISafeUnion::getService("safeunion", mode == PASSTHROUGH /* getStub */);
        ASSERT_NE(safeunionInterface, nullptr);
        ASSERT_EQ(safeunionInterface->isRemote(), mode == BINDERIZED);
    }

    void SetUp() override {
        ALOGI("Environment setup beginning...");
        getServices();
        ALOGI("Environment setup complete.");
    }
};

class HidlTest : public ::testing::Test {
public:
    sp<IServiceManager> manager;
    sp<ITokenManager> tokenManager;
    sp<IAllocator> ashmemAllocator;
    sp<IMemoryTest> memoryTest;
    sp<IFetcher> fetcher;
    sp<IFoo> foo;
    sp<IBaz> baz;
    sp<IBaz> dyingBaz;
    sp<IBar> bar;
    sp<IGraph> graphInterface;
    sp<IPointer> pointerInterface;
    sp<IPointer> validationPointerInterface;
    sp<ITrie> trieInterface;
    sp<ICppSafeUnion> cppSafeunionInterface;
    sp<ISafeUnion> safeunionInterface;
    TestMode mode = TestMode::PASSTHROUGH;

    void SetUp() override {
        ALOGI("Test setup beginning...");
        manager = gHidlEnvironment->manager;
        tokenManager = gHidlEnvironment->tokenManager;
        ashmemAllocator = gHidlEnvironment->ashmemAllocator;
        memoryTest = gHidlEnvironment->memoryTest;
        fetcher = gHidlEnvironment->fetcher;
        foo = gHidlEnvironment->foo;
        baz = gHidlEnvironment->baz;
        dyingBaz = gHidlEnvironment->dyingBaz;
        bar = gHidlEnvironment->bar;
        graphInterface = gHidlEnvironment->graphInterface;
        pointerInterface = gHidlEnvironment->pointerInterface;
        validationPointerInterface = gHidlEnvironment->validationPointerInterface;
        trieInterface = gHidlEnvironment->trieInterface;
        cppSafeunionInterface = gHidlEnvironment->cppSafeunionInterface;
        safeunionInterface = gHidlEnvironment->safeunionInterface;
        mode = gHidlEnvironment->mode;
        ALOGI("Test setup complete");
    }
};

TEST_F(HidlTest, ToStringTest) {
    using namespace android::hardware;

    LOG(INFO) << toString(IFoo::Everything{});

    // Note that handles don't need to be deleted because MQDescriptor takes ownership
    // and deletes them when destructed.
    auto handle = native_handle_create(0, 1);
    auto handle2 = native_handle_create(0, 1);
    handle->data[0] = 5;
    handle2->data[0] = 6;
    IFoo::Everything e{
        .u = {.number = 3},
        .number = 10,
        .h = handle,
        .descSync = {std::vector<GrantorDescriptor>(), handle, 5},
        .descUnsync = {std::vector<GrantorDescriptor>(), handle2, 6},
        .mem = hidl_memory("mymem", handle, 5),
        .p = reinterpret_cast<void*>(0x6),
        .vs = {"hello", "world"},
        .multidimArray = hidl_vec<hidl_string>{"hello", "great", "awesome", "nice"}.data(),
        .sArray = hidl_vec<hidl_string>{"awesome", "thanks", "you're welcome"}.data(),
        .anotherStruct = {.first = "first", .last = "last"},
        .bf = IFoo::BitField::V0 | IFoo::BitField::V2};
    LOG(INFO) << toString(e);
    LOG(INFO) << toString(foo);
    // toString is for debugging purposes only; no good EXPECT
    // statement can be written here.
}

TEST_F(HidlTest, ConstantExpressionTest) {
    // these tests are written so that these always evaluate to one

    for (const auto value : hidl_enum_range<IExpression::OperatorSanityCheck>()) {
        EXPECT_EQ(1, static_cast<int32_t>(value));
    }
    for (const auto value : hidl_enum_range<IExpression::EnumTagTest>()) {
        EXPECT_EQ(1, static_cast<int32_t>(value));
    }
}

TEST_F(HidlTest, PassthroughLookupTest) {
    // IFoo is special because it returns an interface no matter
    //   what instance name is requested. In general, this is BAD!
    EXPECT_NE(nullptr, IFoo::getService("", true /* getStub */).get());
    EXPECT_NE(nullptr, IFoo::getService("a", true /* getStub */).get());
    EXPECT_NE(nullptr, IFoo::getService("asdf", true /* getStub */).get());
    EXPECT_NE(nullptr, IFoo::getService("::::::::", true /* getStub */).get());
    EXPECT_NE(nullptr, IFoo::getService("/////", true /* getStub */).get());
    EXPECT_NE(nullptr, IFoo::getService("\n", true /* getStub */).get());
}

TEST_F(HidlTest, EnumIteratorTest) {
    using Empty = ::android::hardware::tests::foo::V1_0::EnumIterators::Empty;
    using Grandchild = ::android::hardware::tests::foo::V1_0::EnumIterators::Grandchild;
    using SkipsValues = ::android::hardware::tests::foo::V1_0::EnumIterators::SkipsValues;
    using MultipleValues = ::android::hardware::tests::foo::V1_0::EnumIterators::MultipleValues;

    for (const auto value : hidl_enum_range<Empty>()) {
        (void)value;
        ADD_FAILURE() << "Empty range should not iterate";
    }

    EXPECT_EQ(hidl_enum_range<Grandchild>().begin(), hidl_enum_range<Grandchild>().cbegin());
    EXPECT_EQ(hidl_enum_range<Grandchild>().end(), hidl_enum_range<Grandchild>().cend());
    EXPECT_EQ(hidl_enum_range<Grandchild>().rbegin(), hidl_enum_range<Grandchild>().crbegin());
    EXPECT_EQ(hidl_enum_range<Grandchild>().rend(), hidl_enum_range<Grandchild>().crend());

    auto it1 = hidl_enum_range<Grandchild>().begin();
    EXPECT_EQ(Grandchild::A, *it1++);
    EXPECT_EQ(Grandchild::B, *it1++);
    EXPECT_EQ(hidl_enum_range<Grandchild>().end(), it1);
    auto it1r = hidl_enum_range<Grandchild>().rbegin();
    EXPECT_EQ(Grandchild::B, *it1r++);
    EXPECT_EQ(Grandchild::A, *it1r++);
    EXPECT_EQ(hidl_enum_range<Grandchild>().rend(), it1r);

    auto it2 = hidl_enum_range<SkipsValues>().begin();
    EXPECT_EQ(SkipsValues::A, *it2++);
    EXPECT_EQ(SkipsValues::B, *it2++);
    EXPECT_EQ(SkipsValues::C, *it2++);
    EXPECT_EQ(SkipsValues::D, *it2++);
    EXPECT_EQ(SkipsValues::E, *it2++);
    EXPECT_EQ(hidl_enum_range<SkipsValues>().end(), it2);
    auto it2r = hidl_enum_range<SkipsValues>().rbegin();
    EXPECT_EQ(SkipsValues::E, *it2r++);
    EXPECT_EQ(SkipsValues::D, *it2r++);
    EXPECT_EQ(SkipsValues::C, *it2r++);
    EXPECT_EQ(SkipsValues::B, *it2r++);
    EXPECT_EQ(SkipsValues::A, *it2r++);
    EXPECT_EQ(hidl_enum_range<SkipsValues>().rend(), it2r);

    auto it3 = hidl_enum_range<MultipleValues>().begin();
    EXPECT_EQ(MultipleValues::A, *it3++);
    EXPECT_EQ(MultipleValues::B, *it3++);
    EXPECT_EQ(MultipleValues::C, *it3++);
    EXPECT_EQ(MultipleValues::D, *it3++);
    EXPECT_EQ(hidl_enum_range<MultipleValues>().end(), it3);
    auto it3r = hidl_enum_range<MultipleValues>().rbegin();
    EXPECT_EQ(MultipleValues::D, *it3r++);
    EXPECT_EQ(MultipleValues::C, *it3r++);
    EXPECT_EQ(MultipleValues::B, *it3r++);
    EXPECT_EQ(MultipleValues::A, *it3r++);
    EXPECT_EQ(hidl_enum_range<MultipleValues>().rend(), it3r);
}

TEST_F(HidlTest, EnumToStringTest) {
    using namespace std::string_literals;
    using ::android::hardware::tests::foo::V1_0::toString;
    // toString for enum
    EXPECT_EQ(toString(IFoo::BitField::V0), "V0"s);
    EXPECT_EQ(toString(static_cast<IFoo::BitField>(0)), "0"s)
            << "Invalid enum isn't stringified correctly.";
    EXPECT_EQ(toString(static_cast<IFoo::BitField>(IFoo::BitField::V0 | IFoo::BitField::V2)), "0x5"s)
            << "Invalid enum isn't stringified correctly.";
    // dump bitfields
    EXPECT_EQ(toString<IFoo::BitField>((uint8_t)0 | IFoo::BitField::V0), "V0 (0x1)"s);
    EXPECT_EQ(toString<IFoo::BitField>((uint8_t)0 | IFoo::BitField::V0 | IFoo::BitField::V2),
              "V0 | V2 (0x5)"s);
    EXPECT_EQ(toString<IFoo::BitField>((uint8_t)0xF), "V0 | V1 | V2 | V3 | VALL (0xf)"s);
    EXPECT_EQ(toString<IFoo::BitField>((uint8_t)0xFF), "V0 | V1 | V2 | V3 | VALL | 0xf0 (0xff)"s);

    // inheritance
    using Parent = ::android::hardware::tests::foo::V1_0::EnumIterators::Parent;
    using EmptyChild = ::android::hardware::tests::foo::V1_0::EnumIterators::EmptyChild;
    using Grandchild = ::android::hardware::tests::foo::V1_0::EnumIterators::Grandchild;
    EXPECT_EQ(toString(Parent::A), "A"s);
    EXPECT_EQ(toString(EmptyChild::A), "A"s);
    EXPECT_EQ(toString(Grandchild::A), "A"s);
    EXPECT_EQ(toString(Grandchild::B), "B"s);
}

TEST_F(HidlTest, PingTest) {
    EXPECT_OK(manager->ping());
}

TEST_F(HidlTest, TryGetServiceTest) {
    sp<IServiceManager> dne = IServiceManager::tryGetService("boss");
    ASSERT_EQ(dne, nullptr);

    sp<IServiceManager> manager = IServiceManager::tryGetService();
    ASSERT_NE(manager, nullptr);
}

TEST_F(HidlTest, ServiceListTest) {
    static const std::set<std::string> binderizedSet = {
        "android.hardware.tests.pointer@1.0::IPointer/pointer",
        "android.hardware.tests.bar@1.0::IBar/foo",
        "android.hardware.tests.inheritance@1.0::IFetcher/fetcher",
        "android.hardware.tests.inheritance@1.0::IParent/parent",
        "android.hardware.tests.inheritance@1.0::IParent/child",
        "android.hardware.tests.inheritance@1.0::IChild/child",
        "android.hardware.tests.pointer@1.0::IGraph/graph",
        "android.hardware.tests.inheritance@1.0::IGrandparent/child",
        "android.hardware.tests.foo@1.0::IFoo/foo",
        "android.hidl.manager@1.0::IServiceManager/default",
        "android.hidl.manager@1.1::IServiceManager/default",
    };

    static const std::set<std::string> passthroughSet = {
        "android.hidl.manager@1.0::IServiceManager/default",
        "android.hidl.manager@1.1::IServiceManager/default",
    };

    std::set<std::string> activeSet;

    switch(mode) {
        case BINDERIZED: {
            activeSet = binderizedSet;
        } break;

        case PASSTHROUGH: {
            activeSet = passthroughSet;
        } break;
        default:
            EXPECT_TRUE(false) << "unrecognized mode";
    }

    EXPECT_OK(manager->list([&activeSet](const hidl_vec<hidl_string> &registered){
        std::set<std::string> registeredSet;

        for (size_t i = 0; i < registered.size(); i++) {
            registeredSet.insert(registered[i]);
        }

        std::set<std::string> difference;
        std::set_difference(activeSet.begin(), activeSet.end(),
                            registeredSet.begin(), registeredSet.end(),
                            std::inserter(difference, difference.begin()));

        EXPECT_EQ(difference.size(), 0u) << "service(s) not registered " << to_string(difference);
    }));
}

TEST_F(HidlTest, ServiceListByInterfaceTest) {
    if (mode != BINDERIZED) {
        // passthrough service manager does not know about services
        return;
    }

    EXPECT_OK(
        manager->listByInterface(IParent::descriptor, [](const hidl_vec<hidl_string>& registered) {
            std::set<std::string> registeredSet;

            for (size_t i = 0; i < registered.size(); i++) {
                registeredSet.insert(registered[i]);
            }

            std::set<std::string> activeSet = {"parent", "child"};
            std::set<std::string> difference;
            std::set_difference(activeSet.begin(), activeSet.end(), registeredSet.begin(),
                                registeredSet.end(), std::inserter(difference, difference.begin()));

            EXPECT_EQ(difference.size(), 0u)
                << "service(s) not registered " << to_string(difference);
        }));
}

TEST_F(HidlTest, ServiceListManifestByInterfaceTest) {
    // system service
    EXPECT_OK(manager->listManifestByInterface(IServiceManager::descriptor,
                                               [](const hidl_vec<hidl_string>& registered) {
                                                   ASSERT_EQ(1, registered.size());
                                                   EXPECT_EQ("default", registered[0]);
                                               }));
    // vendor service (this is required on all devices)
    EXPECT_OK(
        manager->listManifestByInterface("android.hardware.configstore@1.0::ISurfaceFlingerConfigs",
                                         [](const hidl_vec<hidl_string>& registered) {
                                             ASSERT_EQ(1, registered.size());
                                             EXPECT_EQ("default", registered[0]);
                                         }));
    // test service that will never be in a manifest
    EXPECT_OK(manager->listManifestByInterface(
        IParent::descriptor,
        [](const hidl_vec<hidl_string>& registered) { ASSERT_EQ(0, registered.size()); }));
    // invalid service
    EXPECT_OK(manager->listManifestByInterface(
        "!(*#&$ASDASLKDJasdlkjfads",
        [](const hidl_vec<hidl_string>& registered) { ASSERT_EQ(0, registered.size()); }));
}

TEST_F(HidlTest, SubInterfaceServiceRegistrationTest) {
    using ::android::hardware::interfacesEqual;

    const std::string kInstanceName = "no-matter-what-it-is";
    const std::string kOtherName = "something-different";

    sp<IChild> child = new SimpleChild();
    sp<IParent> parent = new SimpleParent();

    EXPECT_EQ(::android::OK, child->registerAsService(kInstanceName));
    EXPECT_EQ(::android::OK, child->registerAsService(kOtherName));

    EXPECT_TRUE(interfacesEqual(child, IChild::getService(kInstanceName)));
    EXPECT_TRUE(interfacesEqual(child, IParent::getService(kInstanceName)));

    EXPECT_EQ(::android::OK, parent->registerAsService(kInstanceName));

    // FALSE since passthrough HAL will return an instance
    // since binderized instance is nullptr
    EXPECT_FALSE(interfacesEqual(parent, IChild::getService(kInstanceName)));
    EXPECT_TRUE(interfacesEqual(parent, IParent::getService(kInstanceName)));

    // other instance name is unchanged
    EXPECT_TRUE(interfacesEqual(child, IChild::getService(kOtherName)));
    EXPECT_TRUE(interfacesEqual(child, IParent::getService(kOtherName)));
}

TEST_F(HidlTest, ServiceNotificationTest) {
    if (mode != BINDERIZED) {
        // service notifications aren't supported in passthrough mode
        return;
    }

    ServiceNotification* notification = new ServiceNotification();

    std::string instanceName = "test-instance";
    EXPECT_TRUE(IParent::registerForNotifications(instanceName, notification));

    EXPECT_EQ(::android::OK, (new SimpleChild())->registerAsService(instanceName));
    EXPECT_EQ(::android::OK, (new SimpleParent())->registerAsService(instanceName));

    std::unique_lock<std::mutex> lock(notification->mutex);

    notification->condition.wait_for(lock, std::chrono::milliseconds(500), [&notification]() {
        return notification->getRegistrations().size() >= 2;
    });

    std::vector<std::string> registrations = notification->getRegistrations();

    EXPECT_EQ(registrations.size(), 2u);

    EXPECT_EQ(to_string(registrations.data(), registrations.size()),
              std::string("['") + IParent::descriptor + "/" + instanceName + "', '" +
                  IParent::descriptor + "/" + instanceName + "']");
}

TEST_F(HidlTest, ServiceUnregisterTest) {
    const std::string instance = "some-instance-name";

    sp<ServiceNotification> sNotification = new ServiceNotification();

    // unregister all
    EXPECT_TRUE(IParent::registerForNotifications(instance, sNotification));
    EXPECT_TRUE(manager->unregisterForNotifications("", "", sNotification));

    // unregister all with instance name
    EXPECT_TRUE(IParent::registerForNotifications(instance, sNotification));
    EXPECT_TRUE(manager->unregisterForNotifications(IParent::descriptor, "", sNotification));

    // unregister package listener
    EXPECT_TRUE(IParent::registerForNotifications("", sNotification));
    EXPECT_TRUE(manager->unregisterForNotifications(IParent::descriptor, "", sNotification));

    // unregister listener for specific service and name
    EXPECT_TRUE(IParent::registerForNotifications(instance, sNotification));
    EXPECT_TRUE(manager->unregisterForNotifications(IParent::descriptor, instance, sNotification));

    EXPECT_FALSE(manager->unregisterForNotifications("", "", sNotification));

    // TODO(b/32837397): remote destructor is lazy
    // wp<ServiceNotification> wNotification = sNotification;
    // sNotification = nullptr;
    // EXPECT_EQ(nullptr, wNotification.promote().get());
}

TEST_F(HidlTest, ServiceAllNotificationTest) {
    ServiceNotification* notification = new ServiceNotification();

    std::string instanceOne = "test-instance-one";
    std::string instanceTwo = "test-instance-two";
    EXPECT_TRUE(ISimple::registerForNotifications("", notification));

    Simple* instanceA = new Simple(1);
    EXPECT_EQ(::android::OK, instanceA->registerAsService(instanceOne));
    Simple* instanceB = new Simple(2);
    EXPECT_EQ(::android::OK, instanceB->registerAsService(instanceTwo));

    std::unique_lock<std::mutex> lock(notification->mutex);

    notification->condition.wait_for(lock, std::chrono::milliseconds(500), [&notification]() {
        return notification->getRegistrations().size() >= 2;
    });

    std::vector<std::string> registrations = notification->getRegistrations();
    std::sort(registrations.begin(), registrations.end());

    EXPECT_EQ(registrations.size(), 2u);

    std::string descriptor = ISimple::descriptor;

    EXPECT_EQ(
        to_string(registrations.data(), registrations.size()),
        "['" + descriptor + "/" + instanceOne + "', '" + descriptor + "/" + instanceTwo + "']");
}

TEST_F(HidlTest, DebugDumpTest) {
    EXPECT_OK(manager->debugDump([](const auto& list) {
        for (const auto& debugInfo : list) {
            FQName name;
            EXPECT_TRUE(FQName::parse(debugInfo.interfaceName, &name)) << debugInfo.interfaceName;
            EXPECT_TRUE(debugInfo.instanceName.size() > 0);
        }
    }));
}

TEST_F(HidlTest, InterfacesEqualTest) {
    using android::hardware::interfacesEqual;

    sp<IParent> service1 = IParent::getService("child", mode == PASSTHROUGH /* getStub */);
    sp<IParent> service2 = service1;

    // Passthrough services are reinstantiated whenever getService is called.
    if (mode == BINDERIZED) {
        service2 = IParent::getService("child");
    }

    EXPECT_NE(nullptr, service1.get());
    EXPECT_NE(nullptr, service2.get());
    EXPECT_TRUE(interfacesEqual(service1, service2));

    sp<IChild> child = IChild::castFrom(service1);
    EXPECT_NE(nullptr, child.get());  // it is actually a child

    EXPECT_TRUE(interfacesEqual(service1, child));
    EXPECT_TRUE(interfacesEqual(service2, child));
}

TEST_F(HidlTest, TestToken) {
    using android::hardware::interfacesEqual;

    Return<void> ret = tokenManager->createToken(manager, [&] (const hidl_vec<uint8_t> &token) {
        Return<sp<IBase>> retService = tokenManager->get(token);
        EXPECT_OK(retService);
        if (retService.isOk()) {
            sp<IBase> service = retService;
            EXPECT_NE(nullptr, service.get());
            sp<IServiceManager> retManager = IServiceManager::castFrom(service);

            EXPECT_TRUE(interfacesEqual(manager, retManager));
        }

        Return<bool> unregisterRet = tokenManager->unregister(token);

        EXPECT_OK(unregisterRet);
        if (unregisterRet.isOk()) {
            EXPECT_TRUE(unregisterRet);
        }
    });
    EXPECT_OK(ret);
}

TEST_F(HidlTest, TestSharedMemory) {
    const uint8_t kValue = 0xCA;
    hidl_memory mem_copy;
    EXPECT_OK(ashmemAllocator->allocate(1024, [&](bool success, const hidl_memory& mem) {
        EXPECT_EQ(success, true);

        sp<IMemory> memory = mapMemory(mem);

        EXPECT_NE(memory, nullptr);

        uint8_t* data = static_cast<uint8_t*>(static_cast<void*>(memory->getPointer()));
        EXPECT_NE(data, nullptr);

        EXPECT_EQ(memory->getSize(), mem.size());

        memory->update();
        memset(data, 0, memory->getSize());
        memory->commit();

        mem_copy = mem;
        memoryTest->fillMemory(mem, kValue);

        memory->read();
        for (size_t i = 0; i < mem.size(); i++) {
            EXPECT_EQ(kValue, data[i]);
        }
        memory->commit();
    }));

    // Test the memory persists after the call
    sp<IMemory> memory = mapMemory(mem_copy);

    EXPECT_NE(memory, nullptr);

    uint8_t* data = static_cast<uint8_t*>(static_cast<void*>(memory->getPointer()));
    EXPECT_NE(data, nullptr);

    memory->read();
    for (size_t i = 0; i < mem_copy.size(); i++) {
        EXPECT_EQ(kValue, data[i]);
    }
    memory->commit();

    hidl_memory mem_move(std::move(mem_copy));
    ASSERT_EQ(nullptr, mem_copy.handle());
    ASSERT_EQ(0UL, mem_copy.size());
    ASSERT_EQ("", mem_copy.name());

    memory.clear();
    memory = mapMemory(mem_move);

    EXPECT_NE(memory, nullptr);

    data = static_cast<uint8_t*>(static_cast<void*>(memory->getPointer()));
    EXPECT_NE(data, nullptr);

    memory->read();
    for (size_t i = 0; i < mem_move.size(); i++) {
        EXPECT_EQ(kValue, data[i]);
    }
    memory->commit();
}

TEST_F(HidlTest, BatchSharedMemory) {
    const uint8_t kValue = 0xCA;
    const uint64_t kBatchSize = 2;
    hidl_vec<hidl_memory> batchCopy;

    EXPECT_OK(ashmemAllocator->batchAllocate(1024, kBatchSize,
        [&](bool success, const hidl_vec<hidl_memory>& batch) {
            ASSERT_TRUE(success);
            EXPECT_EQ(kBatchSize, batch.size());

            for (uint64_t i = 0; i < batch.size(); i++) {
                sp<IMemory> memory = mapMemory(batch[i]);

                EXPECT_NE(nullptr, memory.get());

                uint8_t* data = static_cast<uint8_t*>(static_cast<void*>(memory->getPointer()));
                EXPECT_NE(nullptr, data);

                EXPECT_EQ(memory->getSize(), batch[i].size());

                memory->update();
                memset(data, kValue, memory->getSize());
                memory->commit();
            }

            batchCopy = batch;
        }));

    for (uint64_t i = 0; i < batchCopy.size(); i++) {
        // Test the memory persists after the call
        sp<IMemory> memory = mapMemory(batchCopy[i]);

        EXPECT_NE(memory, nullptr);

        uint8_t* data = static_cast<uint8_t*>(static_cast<void*>(memory->getPointer()));
        EXPECT_NE(data, nullptr);

        memory->read();
        for (size_t i = 0; i < batchCopy[i].size(); i++) {
            EXPECT_EQ(kValue, data[i]);
        }
        memory->commit();
    }
}

TEST_F(HidlTest, MemoryBlock) {
    const uint8_t kValue = 0xCA;
    using ::android::hardware::IBinder;
    using ::android::hardware::interfacesEqual;
    using ::android::hardware::toBinder;

    sp<HidlMemory> mem;
    EXPECT_OK(ashmemAllocator->allocate(1024, [&](bool success, const hidl_memory& _mem) {
        ASSERT_TRUE(success);
        mem = HidlMemory::getInstance(_mem);
    }));
    memoryTest->set(*mem);
    Return<sp<IMemoryToken>> tokenRet = memoryTest->get();
    EXPECT_OK(tokenRet);
    sp<IMemoryToken> token = tokenRet;
    EXPECT_NE(nullptr, token.get());
    EXPECT_OK(token->get([&](const hidl_memory& mem) {
        sp<IMemory> memory = mapMemory(mem);

        EXPECT_NE(nullptr, memory.get());

        uint8_t* data = static_cast<uint8_t*>(static_cast<void*>(memory->getPointer()));
        EXPECT_NE(data, nullptr);

        EXPECT_EQ(memory->getSize(), mem.size());

        memory->update();
        memset(data, 0, memory->getSize());
        memory->commit();

        memoryTest->fillMemory(mem, kValue);
        memory->commit();
    }));
    MemoryBlock blk = {token, 0x200 /* size */, 0x100 /* offset */};
    EXPECT_OK(memoryTest->haveSomeMemoryBlock(blk, [&](const MemoryBlock& blkBack) {
        sp<IMemoryToken> tokenBack = blkBack.token;
        EXPECT_TRUE(interfacesEqual(token, tokenBack));
        EXPECT_EQ(blkBack.size, 0x200ULL);
        EXPECT_EQ(blkBack.offset, 0x100ULL);
        blk = blkBack;
    }));

    sp<IMemoryToken> mtoken = blk.token;
    mtoken->get([&](const hidl_memory& mem) {
        sp<IMemory> memory = mapMemory(mem);
        uint8_t* data = static_cast<uint8_t*>(static_cast<void*>(memory->getPointer()));
        EXPECT_NE(data, nullptr);
        for (size_t i = 0; i < mem.size(); i++) {
            EXPECT_EQ(kValue, data[i]);
        }
    });
}

TEST_F(HidlTest, NullSharedMemory) {
    hidl_memory memory{};

    EXPECT_EQ(nullptr, memory.handle());

    EXPECT_OK(memoryTest->haveSomeMemory(memory, [&](const hidl_memory &mem) {
        EXPECT_EQ(nullptr, mem.handle());
    }));
}

TEST_F(HidlTest, FooGetDescriptorTest) {
    EXPECT_OK(foo->interfaceDescriptor([&] (const auto &desc) {
        EXPECT_EQ(desc, mode == BINDERIZED
                ? IBar::descriptor // service is actually IBar in binderized mode
                : IFoo::descriptor); // dlopened, so service is IFoo
    }));
}

TEST_F(HidlTest, FooConvertToBoolIfSmallTest) {
    hidl_vec<IFoo::Union> u = {
        {.intValue = 7}, {.intValue = 0}, {.intValue = 1}, {.intValue = 8},
    };
    EXPECT_OK(foo->convertToBoolIfSmall(IFoo::Discriminator::INT, u, [&](const auto& res) {
        ASSERT_EQ(4u, res.size());
        EXPECT_EQ(IFoo::Discriminator::INT, res[0].discriminator);
        EXPECT_EQ(u[0].intValue, res[0].value.intValue);
        EXPECT_EQ(IFoo::Discriminator::BOOL, res[1].discriminator);
        EXPECT_EQ(static_cast<bool>(u[1].intValue), res[1].value.boolValue);
        EXPECT_EQ(IFoo::Discriminator::BOOL, res[2].discriminator);
        EXPECT_EQ(static_cast<bool>(u[2].intValue), res[2].value.boolValue);
        EXPECT_EQ(IFoo::Discriminator::INT, res[3].discriminator);
        EXPECT_EQ(u[3].intValue, res[3].value.intValue);
    }));
}

TEST_F(HidlTest, FooDoThisTest) {
    ALOGI("CLIENT call doThis.");
    EXPECT_OK(foo->doThis(1.0f));
    ALOGI("CLIENT doThis returned.");
}

TEST_F(HidlTest, FooDoThatAndReturnSomethingTest) {
    ALOGI("CLIENT call doThatAndReturnSomething.");
    int32_t result = foo->doThatAndReturnSomething(2.0f);
    ALOGI("CLIENT doThatAndReturnSomething returned %d.", result);
    EXPECT_EQ(result, 666);
}

TEST_F(HidlTest, FooDoQuiteABitTest) {
    ALOGI("CLIENT call doQuiteABit");
    double something = foo->doQuiteABit(1, 2, 3.0f, 4.0);
    ALOGI("CLIENT doQuiteABit returned %f.", something);
    EXPECT_DOUBLE_EQ(something, 666.5);
}

TEST_F(HidlTest, FooDoSomethingElseTest) {

    ALOGI("CLIENT call doSomethingElse");
    hidl_array<int32_t, 15> param;
    for (size_t i = 0; i < sizeof(param) / sizeof(param[0]); ++i) {
        param[i] = i;
    }
    EXPECT_OK(foo->doSomethingElse(param, [&](const auto &something) {
            ALOGI("CLIENT doSomethingElse returned %s.",
                  to_string(something).c_str());
            int32_t expect[] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24,
                26, 28, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 1, 2};
            EXPECT_TRUE(isArrayEqual(something, expect, 32));
        }));
}

TEST_F(HidlTest, FooDoStuffAndReturnAStringTest) {
    ALOGI("CLIENT call doStuffAndReturnAString");
    EXPECT_OK(foo->doStuffAndReturnAString([&](const auto &something) {
            ALOGI("CLIENT doStuffAndReturnAString returned '%s'.",
                  something.c_str());
            EXPECT_STREQ(something.c_str(), "Hello, world");
            EXPECT_EQ(strlen("Hello, world"), something.size());
        }));
}

TEST_F(HidlTest, FooMapThisVectorTest) {
    hidl_vec<int32_t> vecParam;
    vecParam.resize(10);
    for (size_t i = 0; i < 10; ++i) {
        vecParam[i] = i;
    }
    EXPECT_OK(foo->mapThisVector(vecParam, [&](const auto &something) {
            ALOGI("CLIENT mapThisVector returned %s.",
                  to_string(something).c_str());
            int32_t expect[] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18};
            EXPECT_TRUE(isArrayEqual(something, expect, something.size()));
        }));
}

TEST_F(HidlTest, WrapTest) {
    if (!gHidlEnvironment->enableDelayMeasurementTests) {
        return;
    }

    using ::android::hardware::tests::foo::V1_0::BnHwSimple;
    using ::android::hardware::tests::foo::V1_0::BsSimple;
    using ::android::hardware::tests::foo::V1_0::BpHwSimple;
    using ::android::hardware::details::HidlInstrumentor;
    nsecs_t now;
    int i = 0;

    now = systemTime();
    new BnHwSimple(new Simple(1));
    EXPECT_LT(systemTime() - now, 2000000) << "    for BnHwSimple(nonnull)";

    now = systemTime();
    new BnHwSimple(nullptr);
    EXPECT_LT(systemTime() - now, 2000000) << "    for BnHwSimple(null)";

    now = systemTime();
    new BsSimple(new Simple(1));
    EXPECT_LT(systemTime() - now, 2000000) << "    for BsSimple(nonnull)";

    now = systemTime();
    new BsSimple(nullptr);
    EXPECT_LT(systemTime() - now, 2000000) << "    for BsSimple(null)";

    now = systemTime();
    new BpHwSimple(nullptr);
    EXPECT_LT(systemTime() - now, 2000000) << "    for BpHwSimple(null)";

    now = systemTime();
    new ::android::hardware::details::HidlInstrumentor("", "");
    EXPECT_LT(systemTime() - now, 2000000) << "    for HidlInstrumentor";

    now = systemTime();
    i++;
    EXPECT_LT(systemTime() - now,    1000) << "    for nothing";
}

TEST_F(HidlTest, FooCallMeTest) {
    if (!gHidlEnvironment->enableDelayMeasurementTests) {
        return;
    }
    sp<IFooCallback> fooCb = new FooCallback();
    ALOGI("CLIENT call callMe.");
    // callMe is oneway, should return instantly.
    nsecs_t now;
    now = systemTime();
    EXPECT_OK(foo->callMe(fooCb));
    EXPECT_LT(systemTime() - now, ONEWAY_TOLERANCE_NS);
    ALOGI("CLIENT callMe returned.");

    // Bar::callMe will invoke three methods on FooCallback; one will return
    // right away (even though it is a two-way method); the second one will
    // block Bar for DELAY_S seconds, and the third one will return
    // to Bar right away (is oneway) but will itself block for DELAY_S seconds.
    // We need a way to make sure that these three things have happened within
    // 2*DELAY_S seconds plus some small tolerance.
    //
    // Method FooCallback::reportResults() takes a timeout parameter.  It blocks for
    // that length of time, while waiting for the three methods above to
    // complete.  It returns the information of whether each method was invoked,
    // as well as how long the body of the method took to execute.  We verify
    // the information returned by reportResults() against the timeout we pass (which
    // is long enough for the method bodies to execute, plus tolerance), and
    // verify that eachof them executed, as expected, and took the length of
    // time to execute that we also expect.

    const nsecs_t waitNs =
        3 * DELAY_NS + TOLERANCE_NS;
    const nsecs_t reportResultsNs =
        2 * DELAY_NS + TOLERANCE_NS;

    ALOGI("CLIENT: Waiting for up to %" PRId64 " seconds.",
          nanoseconds_to_seconds(waitNs));

    fooCb->reportResults(waitNs,
                [&](int64_t timeLeftNs,
                    const hidl_array<IFooCallback::InvokeInfo, 3> &invokeResults) {
        ALOGI("CLIENT: FooCallback::reportResults() is returning data.");
        ALOGI("CLIENT: Waited for %" PRId64 " milliseconds.",
              nanoseconds_to_milliseconds(waitNs - timeLeftNs));

        EXPECT_LE(waitNs - timeLeftNs, reportResultsNs)
                << "waited for "
                << (timeLeftNs >= 0 ? "" : "more than ")
                << (timeLeftNs >= 0 ? (waitNs - timeLeftNs) : waitNs)
                << "ns, expect to finish in "
                << reportResultsNs << " ns";

        // two-way method, was supposed to return right away
        EXPECT_TRUE(invokeResults[0].invoked);
        EXPECT_LE(invokeResults[0].timeNs, invokeResults[0].callerBlockedNs);
        EXPECT_LE(invokeResults[0].callerBlockedNs, TOLERANCE_NS);
        // two-way method, was supposed to block caller for DELAY_NS
        EXPECT_TRUE(invokeResults[1].invoked);
        EXPECT_LE(invokeResults[1].timeNs, invokeResults[1].callerBlockedNs);
        EXPECT_LE(invokeResults[1].callerBlockedNs,
                    DELAY_NS + TOLERANCE_NS);
        // one-way method, do not block caller, but body was supposed to block for DELAY_NS
        EXPECT_TRUE(invokeResults[2].invoked);
        EXPECT_LE(invokeResults[2].callerBlockedNs, ONEWAY_TOLERANCE_NS);
        EXPECT_LE(invokeResults[2].timeNs, DELAY_NS + TOLERANCE_NS);
    });
}



TEST_F(HidlTest, FooUseAnEnumTest) {
    ALOGI("CLIENT call useAnEnum.");
    IFoo::SomeEnum sleepy = foo->useAnEnum(IFoo::SomeEnum::quux);
    ALOGI("CLIENT useAnEnum returned %u", (unsigned)sleepy);
    EXPECT_EQ(sleepy, IFoo::SomeEnum::goober);
}

TEST_F(HidlTest, FooHaveAGooberTest) {
    hidl_vec<IFoo::Goober> gooberVecParam;
    gooberVecParam.resize(2);
    gooberVecParam[0].name = "Hello";
    gooberVecParam[1].name = "World";

    ALOGI("CLIENT call haveAGooberVec.");
    EXPECT_OK(foo->haveAGooberVec(gooberVecParam));
    ALOGI("CLIENT haveAGooberVec returned.");

    ALOGI("CLIENT call haveaGoober.");
    EXPECT_OK(foo->haveAGoober(gooberVecParam[0]));
    ALOGI("CLIENT haveaGoober returned.");

    ALOGI("CLIENT call haveAGooberArray.");
    hidl_array<IFoo::Goober, 20> gooberArrayParam;
    EXPECT_OK(foo->haveAGooberArray(gooberArrayParam));
    ALOGI("CLIENT haveAGooberArray returned.");
}

TEST_F(HidlTest, FooHaveATypeFromAnotherFileTest) {
    ALOGI("CLIENT call haveATypeFromAnotherFile.");
    Abc abcParam{};
    abcParam.x = "alphabet";
    abcParam.y = 3.14f;
    native_handle_t *handle = native_handle_create(0, 0);
    abcParam.z = handle;
    EXPECT_OK(foo->haveATypeFromAnotherFile(abcParam));
    ALOGI("CLIENT haveATypeFromAnotherFile returned.");
    native_handle_delete(handle);
    abcParam.z = nullptr;
}

TEST_F(HidlTest, FooHaveSomeStringsTest) {
    ALOGI("CLIENT call haveSomeStrings.");
    hidl_array<hidl_string, 3> stringArrayParam;
    stringArrayParam[0] = "What";
    stringArrayParam[1] = "a";
    stringArrayParam[2] = "disaster";
    EXPECT_OK(foo->haveSomeStrings(
                stringArrayParam,
                [&](const auto &out) {
                    ALOGI("CLIENT haveSomeStrings returned %s.",
                          to_string(out).c_str());

                    EXPECT_EQ(to_string(out), "['Hello', 'World']");
                }));
    ALOGI("CLIENT haveSomeStrings returned.");
}

TEST_F(HidlTest, FooHaveAStringVecTest) {
    ALOGI("CLIENT call haveAStringVec.");
    hidl_vec<hidl_string> stringVecParam;
    stringVecParam.resize(3);
    stringVecParam[0] = "What";
    stringVecParam[1] = "a";
    stringVecParam[2] = "disaster";
    EXPECT_OK(foo->haveAStringVec(
                stringVecParam,
                [&](const auto &out) {
                    ALOGI("CLIENT haveAStringVec returned %s.",
                          to_string(out).c_str());

                    EXPECT_EQ(to_string(out), "['Hello', 'World']");
                }));
    ALOGI("CLIENT haveAStringVec returned.");
}

TEST_F(HidlTest, FooTransposeMeTest) {
    hidl_array<float, 3, 5> in;
    float k = 1.0f;
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 5; ++j, ++k) {
            in[i][j] = k;
        }
    }

    ALOGI("CLIENT call transposeMe(%s).", to_string(in).c_str());

    EXPECT_OK(foo->transposeMe(
                in,
                [&](const auto &out) {
                    ALOGI("CLIENT transposeMe returned %s.",
                          to_string(out).c_str());

                    for (size_t i = 0; i < 3; ++i) {
                        for (size_t j = 0; j < 5; ++j) {
                            EXPECT_EQ(out[j][i], in[i][j]);
                        }
                    }
                }));
}

TEST_F(HidlTest, FooCallingDrWhoTest) {
    IFoo::MultiDimensional in;

    size_t k = 0;
    for (size_t i = 0; i < 5; ++i) {
        for (size_t j = 0; j < 3; ++j, ++k) {
            in.quuxMatrix[i][j].first = ("First " + std::to_string(k)).c_str();
            in.quuxMatrix[i][j].last = ("Last " + std::to_string(15-k)).c_str();
        }
    }

    ALOGI("CLIENT call callingDrWho(%s).",
          MultiDimensionalToString(in).c_str());

    EXPECT_OK(foo->callingDrWho(
                in,
                [&](const auto &out) {
                    ALOGI("CLIENT callingDrWho returned %s.",
                          MultiDimensionalToString(out).c_str());

                    size_t k = 0;
                    for (size_t i = 0; i < 5; ++i) {
                        for (size_t j = 0; j < 3; ++j, ++k) {
                            EXPECT_STREQ(
                                out.quuxMatrix[i][j].first.c_str(),
                                in.quuxMatrix[4 - i][2 - j].last.c_str());

                            EXPECT_STREQ(
                                out.quuxMatrix[i][j].last.c_str(),
                                in.quuxMatrix[4 - i][2 - j].first.c_str());
                        }
                    }
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

TEST_F(HidlTest, FooTransposeTest) {
    IFoo::StringMatrix5x3 in;

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 3; ++j) {
            in.s[i][j] = numberToEnglish(3 * i + j + 1).c_str();
        }
    }

    EXPECT_OK(foo->transpose(
                in,
                [&](const auto &out) {
                    EXPECT_EQ(
                        to_string(out),
                        "[['one', 'four', 'seven', 'ten', 'thirteen'], "
                         "['two', 'five', 'eight', 'eleven', 'fourteen'], "
                         "['three', 'six', 'nine', 'twelve', 'fifteen']]");
                }));
}

TEST_F(HidlTest, FooTranspose2Test) {
    hidl_array<hidl_string, 5, 3> in;

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 3; ++j) {
            in[i][j] = numberToEnglish(3 * i + j + 1).c_str();
        }
    }

    EXPECT_OK(foo->transpose2(
                in,
                [&](const auto &out) {
                    EXPECT_EQ(
                        to_string(out),
                        "[['one', 'four', 'seven', 'ten', 'thirteen'], "
                         "['two', 'five', 'eight', 'eleven', 'fourteen'], "
                         "['three', 'six', 'nine', 'twelve', 'fifteen']]");
                }));
}

TEST_F(HidlTest, FooNullNativeHandleTest) {
    Abc xyz;
    xyz.z = nullptr;
    EXPECT_OK(bar->expectNullHandle(nullptr, xyz, [](bool hIsNull, bool xyzHasNull) {
        EXPECT_TRUE(hIsNull);
        EXPECT_TRUE(xyzHasNull);
    }));
}

TEST_F(HidlTest, FooNullSynchronousCallbackTest) {
    Return<void> ret = foo->echoNullInterface(nullptr, nullptr /* synchronous callback */);

    EXPECT_FAIL(ret);
    EXPECT_TRUE(ret.description().find("Null synchronous callback passed") != std::string::npos);
}

TEST_F(HidlTest, FooNullCallbackTest) {
    EXPECT_OK(foo->echoNullInterface(nullptr,
                [](const auto receivedNull, const auto &intf) {
                   EXPECT_TRUE(receivedNull);
                   EXPECT_EQ(intf, nullptr);
                }));
}

TEST_F(HidlTest, StructWithFmq) {
    IFoo::WithFmq w = {
        .scatterGathered =
            {
                .descSync = {std::vector<GrantorDescriptor>(), native_handle_create(0, 1), 5},
            },
        .containsPointer =
            {
                .descSync = {std::vector<GrantorDescriptor>(), native_handle_create(0, 1), 5},
                .foo = nullptr,
            },
    };
    EXPECT_OK(foo->repeatWithFmq(w, [&](const IFoo::WithFmq& returned) {
        checkMQDescriptorEquality(w.scatterGathered.descSync, returned.scatterGathered.descSync);
        checkMQDescriptorEquality(w.containsPointer.descSync, returned.containsPointer.descSync);

        EXPECT_EQ(w.containsPointer.foo, returned.containsPointer.foo);
    }));
}

TEST_F(HidlTest, FooNonNullCallbackTest) {
    hidl_array<hidl_string, 5, 3> in;

    EXPECT_FAIL(foo->transpose2(in, nullptr /* _hidl_cb */));
}

TEST_F(HidlTest, FooSendVecTest) {
    hidl_vec<uint8_t> in;
    in.resize(16);
    for (size_t i = 0; i < in.size(); ++i) {
        in[i] = i;
    }

    EXPECT_OK(foo->sendVec(
                in,
                [&](const auto &out) {
                    EXPECT_EQ(to_string(in), to_string(out));
                }));
}

TEST_F(HidlTest, FooSendEmptyVecTest) {
    hidl_vec<uint8_t> in;
    EXPECT_OK(foo->sendVec(
                in,
                [&](const auto &out) {
                    EXPECT_EQ(out.size(), 0u);
                    EXPECT_EQ(to_string(in), to_string(out));
                }));
}

TEST_F(HidlTest, FooHaveAVectorOfInterfacesTest) {
    hidl_vec<sp<ISimple> > in;
    in.resize(16);
    for (size_t i = 0; i < in.size(); ++i) {
        in[i] = new Simple(i);
    }

    EXPECT_OK(foo->haveAVectorOfInterfaces(
                in,
                [&](const auto &out) {
                    EXPECT_EQ(in.size(), out.size());
                    for (size_t i = 0; i < in.size(); ++i) {
                        int32_t inCookie = in[i]->getCookie();
                        int32_t outCookie = out[i]->getCookie();
                        EXPECT_EQ(inCookie, outCookie);
                    }
                }));
}

TEST_F(HidlTest, FooHaveAVectorOfGenericInterfacesTest) {

    hidl_vec<sp<::android::hidl::base::V1_0::IBase> > in;
    in.resize(16);
    for (size_t i = 0; i < in.size(); ++i) {
        sp<ISimple> s = new Simple(i);
        in[i] = s;
    }

    EXPECT_OK(foo->haveAVectorOfGenericInterfaces(
                in,
                [&](const auto &out) {
                    EXPECT_EQ(in.size(), out.size());

                    EXPECT_OK(out[0]->interfaceDescriptor([](const auto &name) {
                        ASSERT_STREQ(name.c_str(), ISimple::descriptor);
                    }));
                    for (size_t i = 0; i < in.size(); ++i) {
                        sp<ISimple> inSimple = ISimple::castFrom(in[i]);
                        sp<ISimple> outSimple = ISimple::castFrom(out[i]);

                        ASSERT_NE(inSimple.get(), nullptr);
                        ASSERT_NE(outSimple.get(), nullptr);
                        EXPECT_EQ(in[i], inSimple.get()); // pointers must be equal!
                        int32_t inCookie = inSimple->getCookie();
                        int32_t outCookie = outSimple->getCookie();
                        EXPECT_EQ(inCookie, outCookie);
                    }
                }));
}

TEST_F(HidlTest, FooStructEmbeddedHandleTest) {
    EXPECT_OK(foo->createMyHandle([&](const auto &myHandle) {
        EXPECT_EQ(myHandle.guard, 666);
        const native_handle_t* handle = myHandle.h.getNativeHandle();
        EXPECT_EQ(handle->numInts, 10);
        EXPECT_EQ(handle->numFds, 0);
        int data[] = {2,3,5,7,11,13,17,19,21,23};
        EXPECT_ARRAYEQ(handle->data, data, 10);
    }));

    EXPECT_OK(foo->closeHandles());
}

TEST_F(HidlTest, FooHandleVecTest) {
    EXPECT_OK(foo->createHandles(3, [&](const auto &handles) {
        EXPECT_EQ(handles.size(), 3ull);
        int data[] = {2,3,5,7,11,13,17,19,21,23};
        for (size_t i = 0; i < 3; i++) {
            const native_handle_t *h = handles[i];
            EXPECT_EQ(h->numInts, 10) << " for element " << i;
            EXPECT_EQ(h->numFds, 0) << " for element " << i;
            EXPECT_ARRAYEQ(h->data, data, 10);
        }
    }));

    EXPECT_OK(foo->closeHandles());
}

TEST_F(HidlTest, BazStructWithInterfaceTest) {
    using ::android::hardware::interfacesEqual;

    const std::string testString = "Hello, World!";
    const std::array<int8_t, 7> testArray{-1, -2, -3, 0, 1, 2, 3};
    const hidl_vec<hidl_string> testStrings{"So", "Many", "Words"};
    const hidl_vec<bool> testVector{false, true, false, true, true, true};

    hidl_vec<bool> goldenResult(testVector.size());
    for (size_t i = 0; i < testVector.size(); i++) {
        goldenResult[i] = !testVector[i];
    }

    IBaz::StructWithInterface swi;
    swi.number = 42;
    swi.array = testArray;
    swi.oneString = testString;
    swi.vectorOfStrings = testStrings;
    swi.dummy = baz;

    EXPECT_OK(baz->haveSomeStructWithInterface(swi, [&](const IBaz::StructWithInterface& swiBack) {
        EXPECT_EQ(42, swiBack.number);
        for (size_t i = 0; i < testArray.size(); i++) {
            EXPECT_EQ(testArray[i], swiBack.array[i]);
        }

        EXPECT_EQ(testString, std::string(swiBack.oneString));
        EXPECT_EQ(testStrings, swiBack.vectorOfStrings);

        EXPECT_TRUE(interfacesEqual(swi.dummy, swiBack.dummy));
        EXPECT_OK(swiBack.dummy->someBoolVectorMethod(
            testVector, [&](const hidl_vec<bool>& result) { EXPECT_EQ(goldenResult, result); }));
    }));
}

struct HidlDeathRecipient : hidl_death_recipient {
    std::mutex mutex;
    std::condition_variable condition;
    wp<IBase> who;
    bool fired = false;
    uint64_t cookie = 0;

    void serviceDied(uint64_t cookie, const wp<IBase>& who) override {
        std::unique_lock<std::mutex> lock(mutex);
        fired = true;
        this->cookie = cookie;
        this->who = who;
        condition.notify_one();
    };
};

TEST_F(HidlTest, DeathRecipientTest) {
    sp<HidlDeathRecipient> recipient = new HidlDeathRecipient();
    sp<HidlDeathRecipient> recipient2 = new HidlDeathRecipient();

    EXPECT_TRUE(dyingBaz->linkToDeath(recipient, 0x1481));

    EXPECT_TRUE(dyingBaz->linkToDeath(recipient, 0x1482));
    EXPECT_TRUE(dyingBaz->unlinkToDeath(recipient));

    EXPECT_TRUE(dyingBaz->linkToDeath(recipient2, 0x2592));
    EXPECT_TRUE(dyingBaz->unlinkToDeath(recipient2));

    if (mode != BINDERIZED) {
        // Passthrough doesn't fire, nor does it keep state of
        // registered death recipients (so it won't fail unlinking
        // the same recipient twice).
        return;
    }

    EXPECT_FALSE(dyingBaz->unlinkToDeath(recipient2));
    auto ret = dyingBaz->dieNow();
    if (!ret.isOk()) {
        //do nothing, this is expected
    }

    // further calls fail
    EXPECT_FAIL(dyingBaz->ping());

    std::unique_lock<std::mutex> lock(recipient->mutex);
    recipient->condition.wait_for(lock, std::chrono::milliseconds(100), [&recipient]() {
            return recipient->fired;
    });
    EXPECT_TRUE(recipient->fired);
    EXPECT_EQ(recipient->cookie, 0x1481u);
    EXPECT_EQ(recipient->who, dyingBaz);
    std::unique_lock<std::mutex> lock2(recipient2->mutex);
    recipient2->condition.wait_for(lock2, std::chrono::milliseconds(100), [&recipient2]() {
            return recipient2->fired;
    });
    EXPECT_FALSE(recipient2->fired);

    // Verify servicemanager dropped its reference too
    sp<IBaz> deadBaz = IBaz::getService("dyingBaz", false);
    if (deadBaz != nullptr) {
        // Got a passthrough
        EXPECT_FALSE(deadBaz->isRemote());
    }
}

TEST_F(HidlTest, BarThisIsNewTest) {
    // Now the tricky part, get access to the derived interface.
    ALOGI("CLIENT call thisIsNew.");
    EXPECT_OK(bar->thisIsNew());
    ALOGI("CLIENT thisIsNew returned.");
}

static void expectGoodChild(sp<IChild> child) {
    ASSERT_NE(child.get(), nullptr);
    child = IChild::castFrom(child);
    ASSERT_NE(child.get(), nullptr);
    EXPECT_OK(child->doGrandparent());
    EXPECT_OK(child->doParent());
    EXPECT_OK(child->doChild());
}

static void expectGoodParent(sp<IParent> parent) {
    ASSERT_NE(parent.get(), nullptr);
    parent = IParent::castFrom(parent);
    ASSERT_NE(parent.get(), nullptr);
    EXPECT_OK(parent->doGrandparent());
    EXPECT_OK(parent->doParent());
    sp<IChild> child = IChild::castFrom(parent);
    expectGoodChild(child);
}

static void expectGoodGrandparent(sp<IGrandparent> grandparent) {
    ASSERT_NE(grandparent.get(), nullptr);
    grandparent = IGrandparent::castFrom(grandparent);
    ASSERT_NE(grandparent.get(), nullptr);
    EXPECT_OK(grandparent->doGrandparent());
    sp<IParent> parent = IParent::castFrom(grandparent);
    expectGoodParent(parent);
}

TEST_F(HidlTest, FooHaveAnInterfaceTest) {
    sp<ISimple> in = new Complicated(42);
    Return<sp<ISimple>> ret = bar->haveAInterface(in);
    EXPECT_OK(ret);
    sp<ISimple> out = ret;
    ASSERT_NE(out.get(), nullptr);
    EXPECT_EQ(out->getCookie(), 42);
    EXPECT_OK(out->customVecInt([](const auto &) { }));
    EXPECT_OK(out->customVecStr([](const auto &) { }));
    EXPECT_OK(out->ping());
    EXPECT_OK(out->mystr([](const auto &) { }));
    EXPECT_OK(out->myhandle([](const auto &) { }));
}

TEST_F(HidlTest, InheritRemoteGrandparentTest) {
    Return<sp<IGrandparent>> ret = fetcher->getGrandparent(true);
    EXPECT_OK(ret);
    expectGoodGrandparent(ret);
}

TEST_F(HidlTest, InheritLocalGrandparentTest) {
    Return<sp<IGrandparent>> ret = fetcher->getGrandparent(false);
    EXPECT_OK(ret);
    expectGoodGrandparent(ret);
}

TEST_F(HidlTest, InheritRemoteParentTest) {
    Return<sp<IParent>> ret = fetcher->getParent(true);
    EXPECT_OK(ret);
    expectGoodParent(ret);
}

TEST_F(HidlTest, InheritLocalParentTest) {
    Return<sp<IParent>> ret = fetcher->getParent(false);
    EXPECT_OK(ret);
    expectGoodParent(ret);
}

TEST_F(HidlTest, InheritRemoteChildTest) {
    Return<sp<IChild>> ret = fetcher->getChild(true);
    EXPECT_OK(ret);
    expectGoodChild(ret);
}

TEST_F(HidlTest, InheritLocalChildTest) {
    Return<sp<IChild>> ret = fetcher->getChild(false);
    EXPECT_OK(ret);
    expectGoodChild(ret);
}

TEST_F(HidlTest, TestArrayDimensionality) {
    hidl_array<int, 2> oneDim;
    hidl_array<int, 2, 3> twoDim;
    hidl_array<int, 2, 3, 4> threeDim;

    EXPECT_EQ(oneDim.size(), 2u);
    EXPECT_EQ(twoDim.size(), std::make_tuple(2u, 3u));
    EXPECT_EQ(threeDim.size(), std::make_tuple(2u, 3u, 4u));
}

TEST_F(HidlTest, StructEqualTest) {
    using G = IFoo::Goober;
    using F = IFoo::Fumble;
    G g1{
        .q = 42,
        .name = "The Ultimate Question of Life, the Universe, and Everything",
        .address = "North Pole",
        .numbers = std::array<double, 10>{ {1, 2, 3, 4, 5, 6, 7, 8, 9, 10} },
        .fumble = F{.data = {.data = 50}},
        .gumble = F{.data = {.data = 60}}
    };
    G g2{
        .q = 42,
        .name = "The Ultimate Question of Life, the Universe, and Everything",
        .address = "North Pole",
        .numbers = std::array<double, 10>{ {1, 2, 3, 4, 5, 6, 7, 8, 9, 10} },
        .fumble = F{.data = {.data = 50}},
        .gumble = F{.data = {.data = 60}}
    };
    G g3{
        .q = 42,
        .name = "The Ultimate Question of Life, the Universe, and Everything",
        .address = "North Pole",
        .numbers = std::array<double, 10>{ {1, 2, 3, 4, 5, 6, 7, 8, 9, 10} },
        .fumble = F{.data = {.data = 50}},
        .gumble = F{.data = {.data = 61}}
    };
    // explicitly invoke operator== here.
    EXPECT_TRUE(g1 == g2);
    EXPECT_TRUE(g1 != g3);
}

TEST_F(HidlTest, EnumEqualTest) {
    using E = IFoo::SomeEnum;
    E e1 = E::quux;
    E e2 = E::quux;
    E e3 = E::goober;
    // explicitly invoke operator== here.
    EXPECT_TRUE(e1 == e2);
    EXPECT_TRUE(e1 != e3);
}

TEST_F(HidlTest, InvalidTransactionTest) {
    using ::android::hardware::tests::bar::V1_0::BnHwBar;
    using ::android::hardware::IBinder;
    using ::android::hardware::Parcel;

    sp<IBinder> binder = ::android::hardware::toBinder(bar);

    Parcel request, reply;
    EXPECT_EQ(::android::OK, request.writeInterfaceToken(IBar::descriptor));
    EXPECT_EQ(::android::UNKNOWN_TRANSACTION, binder->transact(1234, request, &reply));

    EXPECT_OK(bar->ping());  // still works
}

TEST_F(HidlTest, EmptyTransactionTest) {
    using ::android::hardware::IBinder;
    using ::android::hardware::Parcel;
    using ::android::hardware::tests::bar::V1_0::BnHwBar;

    sp<IBinder> binder = ::android::hardware::toBinder(bar);

    Parcel request, reply;
    EXPECT_EQ(::android::BAD_TYPE, binder->transact(2 /*someBoolMethod*/, request, &reply));

    EXPECT_OK(bar->ping());  // still works
}

TEST_F(HidlTest, WrongDescriptorTest) {
    using ::android::hardware::IBinder;
    using ::android::hardware::Parcel;
    using ::android::hardware::tests::bar::V1_0::BnHwBar;

    sp<IBinder> binder = ::android::hardware::toBinder(bar);

    Parcel request, reply;
    // wrong descriptor
    EXPECT_EQ(::android::OK, request.writeInterfaceToken("not a real descriptor"));
    EXPECT_EQ(::android::BAD_TYPE, binder->transact(2 /*someBoolMethod*/, request, &reply));

    EXPECT_OK(bar->ping());  // still works
}

TEST_F(HidlTest, TrieSimpleTest) {
    trieInterface->newTrie([&](const TrieNode& trie) {
        trieInterface->addStrings(trie, {"a", "ba"}, [&](const TrieNode& trie) {
            trieInterface->containsStrings(
                trie, {"", "a", "b", "ab", "ba", "c"}, [](const hidl_vec<bool>& response) {
                    EXPECT_EQ(response,
                              std::vector<bool>({false, true, false, false, true, false}));
                });

            trieInterface->addStrings(trie, {"", "ab", "bab"}, [&](const TrieNode& trie) {
                trieInterface->containsStrings(
                    trie, {"", "a", "b", "ab", "ba", "c"}, [](const hidl_vec<bool>& response) {
                        EXPECT_EQ(response,
                                  std::vector<bool>({true, true, false, true, true, false}));
                    });
            });
        });
    });
}

struct RandomString {
    std::string next() {
        std::string ret(lengthDist(rng), 0);
        std::generate(ret.begin(), ret.end(), [&]() { return charDist(rng); });
        return ret;
    }

    RandomString() : rng(std::random_device{}()), lengthDist(5, 10), charDist('a', 'a' + 10) {}

   private:
    std::default_random_engine rng;
    std::uniform_int_distribution<> lengthDist;
    std::uniform_int_distribution<> charDist;
};

TEST_F(HidlTest, TrieStressTest) {
    const size_t REQUEST_NUM = 1000;
    RandomString stringGenerator;

    trieInterface->newTrie([&](const TrieNode& trie) {
        std::vector<std::string> strings(REQUEST_NUM);
        for (auto& str : strings) {
            str = stringGenerator.next();
        }

        trieInterface->addStrings(
            trie, hidl_vec<hidl_string>(strings.begin(), strings.end()), [&](const TrieNode& trie) {
                std::unordered_set<std::string> addedStrings(strings.begin(), strings.end());

                for (size_t i = 0; i != REQUEST_NUM; ++i) {
                    strings.push_back(stringGenerator.next());
                }

                std::vector<bool> trueResponse(strings.size());
                std::transform(strings.begin(), strings.end(), trueResponse.begin(),
                               [&](const std::string& str) {
                                   return addedStrings.find(str) != addedStrings.end();
                               });

                trieInterface->containsStrings(
                    trie, hidl_vec<hidl_string>(strings.begin(), strings.end()),
                    [&](const hidl_vec<bool>& response) { EXPECT_EQ(response, trueResponse); });
            });
    });
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

TEST_F(HidlTest, SafeUnionCopyConstructorTest) {
    const hidl_vec<bool> testVector{true, false, true, false, false, false, true,  false,
                                    true, true,  true, false, false, true,  false, true};

    EXPECT_OK(safeunionInterface->newLargeSafeUnion([&](const LargeSafeUnion& safeUnion) {
        EXPECT_OK(
            safeunionInterface->setH(safeUnion, testVector, [&](const LargeSafeUnion& safeUnion) {
                LargeSafeUnion safeUnionCopy(safeUnion);

                EXPECT_EQ(LargeSafeUnion::hidl_discriminator::h, safeUnionCopy.getDiscriminator());
                EXPECT_EQ(testVector, safeUnionCopy.h());
            }));
    }));
}

template <typename T>
void testZeroInit(const std::string& header) {
    uint8_t buf[sizeof(T)];
    memset(buf, 0xFF, sizeof(buf));

    T* t = new (buf) T;

    for (size_t i = 0; i < sizeof(T); i++) {
        EXPECT_EQ(0, buf[i]) << header << " at offset: " << i;
    }

    t->~T();
    t = nullptr;

    memset(buf, 0xFF, sizeof(buf));
    t = new (buf) T(T());  // copy constructor

    for (size_t i = 0; i < sizeof(T); i++) {
        EXPECT_EQ(0, buf[i]) << header << " at offset: " << i;
    }

    t->~T();
    t = nullptr;

    memset(buf, 0xFF, sizeof(buf));
    const T aT = T();
    t = new (buf) T(std::move(aT));  // move constructor

    for (size_t i = 0; i < sizeof(T); i++) {
        EXPECT_EQ(0, buf[i]) << header << " at offset: " << i;
    }

    t->~T();
    t = nullptr;
}

TEST_F(HidlTest, SafeUnionUninit) {
    testZeroInit<SmallSafeUnion>("SmallSafeUnion");
    testZeroInit<LargeSafeUnion>("LargeSafeUnion");
    testZeroInit<InterfaceTypeSafeUnion>("InterfaceTypeSafeUnion");
    testZeroInit<HandleTypeSafeUnion>("HandleTypeSafeUnion");
}

TEST_F(HidlTest, SafeUnionMoveConstructorTest) {
    sp<IOtherInterface> otherInterface = new OtherInterface();
    ASSERT_EQ(1, otherInterface->getStrongCount());

    InterfaceTypeSafeUnion safeUnion;
    safeUnion.c(otherInterface);
    EXPECT_EQ(2, otherInterface->getStrongCount());

    InterfaceTypeSafeUnion anotherSafeUnion(std::move(safeUnion));
    EXPECT_EQ(InterfaceTypeSafeUnion::hidl_discriminator::c,
              anotherSafeUnion.getDiscriminator());
    EXPECT_EQ(2, otherInterface->getStrongCount());
}

TEST_F(HidlTest, SafeUnionCopyAssignmentTest) {
    const hidl_vec<hidl_string> testVector{"So", "Many", "Words"};
    InterfaceTypeSafeUnion safeUnion;
    safeUnion.e(testVector);

    InterfaceTypeSafeUnion anotherSafeUnion;
    anotherSafeUnion = safeUnion;

    EXPECT_EQ(InterfaceTypeSafeUnion::hidl_discriminator::e, anotherSafeUnion.getDiscriminator());
    EXPECT_EQ(InterfaceTypeSafeUnion::hidl_discriminator::e, safeUnion.getDiscriminator());
    EXPECT_NE(&(safeUnion.e()), &(anotherSafeUnion.e()));
    EXPECT_EQ(testVector, anotherSafeUnion.e());
    EXPECT_EQ(testVector, safeUnion.e());
}

TEST_F(HidlTest, SafeUnionMoveAssignmentTest) {
    sp<IOtherInterface> otherInterface = new OtherInterface();
    ASSERT_EQ(1, otherInterface->getStrongCount());

    InterfaceTypeSafeUnion safeUnion;
    safeUnion.c(otherInterface);
    EXPECT_EQ(2, otherInterface->getStrongCount());

    InterfaceTypeSafeUnion anotherSafeUnion;
    anotherSafeUnion.a(255);
    anotherSafeUnion = std::move(safeUnion);

    EXPECT_EQ(InterfaceTypeSafeUnion::hidl_discriminator::c,
              anotherSafeUnion.getDiscriminator());
    EXPECT_EQ(2, otherInterface->getStrongCount());
}

TEST_F(HidlTest, SafeUnionMutateTest) {
    const std::array<int64_t, 5> testArray{-1, -2, -3, -4, -5};
    const std::string testString = "Test string";
    LargeSafeUnion safeUnion;

    safeUnion.f(testArray);
    safeUnion.f()[0] += 10;
    EXPECT_EQ(testArray[0] + 10, safeUnion.f()[0]);

    safeUnion.j(ISafeUnion::J());
    safeUnion.j().j3 = testString;
    EXPECT_EQ(testString, std::string(safeUnion.j().j3));
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

TEST_F(HidlTest, SafeUnionEnumTest) {
    EXPECT_OK(safeunionInterface->newLargeSafeUnion([&](const LargeSafeUnion& safeUnion) {
        EXPECT_OK(safeunionInterface->setM(
            safeUnion, ISafeUnion::BitField::V1, [&](const LargeSafeUnion& safeUnion) {
                EXPECT_EQ(LargeSafeUnion::hidl_discriminator::m, safeUnion.getDiscriminator());
                EXPECT_EQ(ISafeUnion::BitField::V1, safeUnion.m());
            }));
    }));
}

TEST_F(HidlTest, SafeUnionBitFieldTest) {
    EXPECT_OK(safeunionInterface->newLargeSafeUnion([&](const LargeSafeUnion& safeUnion) {
        EXPECT_OK(safeunionInterface->setN(
            safeUnion, 0 | ISafeUnion::BitField::V1, [&](const LargeSafeUnion& safeUnion) {
                EXPECT_EQ(LargeSafeUnion::hidl_discriminator::n, safeUnion.getDiscriminator());
                EXPECT_EQ(0 | ISafeUnion::BitField::V1, safeUnion.n());
            }));
    }));
}

TEST_F(HidlTest, SafeUnionInterfaceTest) {
    const std::array<int8_t, 7> testArray{-1, -2, -3, 0, 1, 2, 3};
    const hidl_vec<hidl_string> testVector{"So", "Many", "Words"};
    const std::string testStringA = "Hello";
    const std::string testStringB = "World";

    const std::string serviceName = "otherinterface";
    sp<IOtherInterface> otherInterface = new OtherInterface();
    EXPECT_EQ(::android::OK, otherInterface->registerAsService(serviceName));

    EXPECT_OK(
        safeunionInterface->newInterfaceTypeSafeUnion([&](const InterfaceTypeSafeUnion& safeUnion) {
            EXPECT_EQ(InterfaceTypeSafeUnion::hidl_discriminator::noinit,
                      safeUnion.getDiscriminator());

            isOk(safeunionInterface->setInterfaceB(
                safeUnion, testArray, [&](const InterfaceTypeSafeUnion& safeUnion) {
                    EXPECT_EQ(InterfaceTypeSafeUnion::hidl_discriminator::b,
                              safeUnion.getDiscriminator());

                    for (size_t i = 0; i < testArray.size(); i++) {
                        EXPECT_EQ(testArray[i], safeUnion.b()[i]);
                    }

                    EXPECT_OK(safeunionInterface->setInterfaceC(
                        safeUnion, otherInterface, [&](const InterfaceTypeSafeUnion& safeUnion) {
                            EXPECT_EQ(InterfaceTypeSafeUnion::hidl_discriminator::c,
                                      safeUnion.getDiscriminator());

                            EXPECT_OK(safeUnion.c()->concatTwoStrings(
                                testStringA, testStringB, [&](const hidl_string& result) {
                                    EXPECT_EQ(testStringA + testStringB, std::string(result));
                                }));
                        }));
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
        }));
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

TEST_F(HidlTest, SafeUnionSimpleDestructorTest) {
    sp<IOtherInterface> otherInterface = new OtherInterface();
    ASSERT_EQ(1, otherInterface->getStrongCount());

    {
        InterfaceTypeSafeUnion safeUnion;
        safeUnion.c(otherInterface);
        EXPECT_EQ(2, otherInterface->getStrongCount());
    }

    EXPECT_EQ(1, otherInterface->getStrongCount());
}

TEST_F(HidlTest, SafeUnionSwitchActiveComponentsDestructorTest) {
    sp<IOtherInterface> otherInterface = new OtherInterface();
    ASSERT_EQ(1, otherInterface->getStrongCount());

    InterfaceTypeSafeUnion safeUnion;
    safeUnion.c(otherInterface);
    EXPECT_EQ(2, otherInterface->getStrongCount());

    safeUnion.a(1);
    EXPECT_EQ(1, otherInterface->getStrongCount());
}

TEST_F(HidlTest, SafeUnionCppSpecificTest) {
    ICppSafeUnion::PointerFmqSafeUnion pointerFmqSafeUnion;
    pointerFmqSafeUnion.fmqSync({std::vector<GrantorDescriptor>(), native_handle_create(0, 1), 5});

    EXPECT_OK(cppSafeunionInterface->repeatPointerFmqSafeUnion(
        pointerFmqSafeUnion, [&](const ICppSafeUnion::PointerFmqSafeUnion& fmq) {
            ASSERT_EQ(pointerFmqSafeUnion.getDiscriminator(), fmq.getDiscriminator());
            checkMQDescriptorEquality(pointerFmqSafeUnion.fmqSync(), fmq.fmqSync());
        }));

    ICppSafeUnion::FmqSafeUnion fmqSafeUnion;
    fmqSafeUnion.fmqUnsync({std::vector<GrantorDescriptor>(), native_handle_create(0, 1), 5});

    EXPECT_OK(cppSafeunionInterface->repeatFmqSafeUnion(
        fmqSafeUnion, [&](const ICppSafeUnion::FmqSafeUnion& fmq) {
            ASSERT_EQ(fmqSafeUnion.getDiscriminator(), fmq.getDiscriminator());
            checkMQDescriptorEquality(fmqSafeUnion.fmqUnsync(), fmq.fmqUnsync());
        }));
}

class HidlMultithreadTest : public ::testing::Test {
   public:
    sp<IMultithread> multithreadInterface;
    TestMode mode = TestMode::PASSTHROUGH;

    void SetUp() override {
        ALOGI("Test setup beginning...");
        multithreadInterface = gHidlEnvironment->multithreadInterface;
        mode = gHidlEnvironment->mode;
        ALOGI("Test setup complete");
    }

    void test_multithread(int maxThreads, int numThreads) {
        LOG(INFO) << "CLIENT call setNumThreads("
                  << maxThreads << ", " << numThreads << ")";
        EXPECT_OK(multithreadInterface->setNumThreads(maxThreads, numThreads));

        std::vector<std::future<bool>> threads;

        for (int i = 0; i != numThreads; ++i) {
            LOG(INFO) << "CLIENT call runNewThread";
            threads.emplace_back(std::async(
                std::launch::async, [&]() { return (bool)multithreadInterface->runNewThread(); }));
        }

        bool noTimeout = std::all_of(threads.begin(), threads.end(),
                                     [](std::future<bool>& thread) { return thread.get(); });
        EXPECT_EQ(noTimeout, maxThreads >= numThreads || mode == PASSTHROUGH);
    }
};

// If it fails first try to increment timeout duration at
// hardware/interfaces/tests/multithread/1.0/default
TEST_F(HidlMultithreadTest, MultithreadTest) {
    // configureRpcThreadpool doesn't stop threads,
    // so maxThreads should not decrease
    test_multithread(1, 1);
    test_multithread(2, 1);
    test_multithread(2, 2);
    test_multithread(2, 3);
    test_multithread(10, 5);
    test_multithread(10, 10);
    test_multithread(10, 15);
    test_multithread(20, 30);
    test_multithread(20, 20);
    test_multithread(20, 10);
}

#if HIDL_RUN_POINTER_TESTS

TEST_F(HidlTest, PassAGraphTest) {
    IGraph::Graph g;
    ::android::simpleGraph(g);
    ::android::logSimpleGraph("CLIENT", g);
    ALOGI("CLIENT call passAGraph");
    EXPECT_OK(graphInterface->passAGraph(g));
}

TEST_F(HidlTest, GiveAGraphTest) {
    EXPECT_OK(graphInterface->giveAGraph([&](const auto &newGraph) {
        ::android::logSimpleGraph("CLIENT", newGraph);
        EXPECT_TRUE(::android::isSimpleGraph(newGraph));
    }));
}
TEST_F(HidlTest, PassANodeTest) {
    IGraph::Node node; node.data = 10;
    EXPECT_OK(graphInterface->passANode(node));
}
TEST_F(HidlTest, PassTwoGraphsTest) {
    IGraph::Graph g;
    ::android::simpleGraph(g);
    EXPECT_OK(graphInterface->passTwoGraphs(&g, &g));
}
TEST_F(HidlTest, PassAGammaTest) {
    IGraph::Theta s; s.data = 500;
    IGraph::Alpha a; a.s_ptr = &s;
    IGraph::Beta  b; b.s_ptr = &s;
    IGraph::Gamma c; c.a_ptr = &a; c.b_ptr = &b;
    ALOGI("CLIENT calling passAGamma: c.a = %p, c.b = %p, c.a->s = %p, c.b->s = %p",
        c.a_ptr, c.b_ptr, c.a_ptr->s_ptr, c.b_ptr->s_ptr);
    EXPECT_OK(graphInterface->passAGamma(c));
}
TEST_F(HidlTest, PassNullTest) {
    IGraph::Gamma c;
    c.a_ptr = nullptr;
    c.b_ptr = nullptr;
    EXPECT_OK(graphInterface->passAGamma(c));
}
TEST_F(HidlTest, PassASimpleRefTest) {
    IGraph::Theta s;
    s.data = 500;
    IGraph::Alpha a;
    a.s_ptr = &s;
    EXPECT_OK(graphInterface->passASimpleRef(&a));
}
TEST_F(HidlTest, PassASimpleRefSTest) {
    IGraph::Theta s;
    s.data = 500;
    ALOGI("CLIENT call passASimpleRefS with %p", &s);
    EXPECT_OK(graphInterface->passASimpleRefS(&s));
}
TEST_F(HidlTest, GiveASimpleRefTest) {
    EXPECT_OK(graphInterface->giveASimpleRef([&](const auto & a_ptr) {
        EXPECT_EQ(a_ptr->s_ptr->data, 500);
    }));
}
TEST_F(HidlTest, GraphReportErrorsTest) {
    Return<int32_t> ret = graphInterface->getErrors();
    EXPECT_OK(ret);
    EXPECT_EQ(int32_t(ret), 0);
}

TEST_F(HidlTest, PointerPassOldBufferTest) {
    EXPECT_OK(validationPointerInterface->bar1([&](const auto& sptr, const auto& s) {
        EXPECT_OK(pointerInterface->foo1(sptr, s));
    }));
}
TEST_F(HidlTest, PointerPassOldBufferTest2) {
    EXPECT_OK(validationPointerInterface->bar2([&](const auto& s, const auto& a) {
        EXPECT_OK(pointerInterface->foo2(s, a));
    }));
}
TEST_F(HidlTest, PointerPassSameOldBufferPointerTest) {
    EXPECT_OK(validationPointerInterface->bar3([&](const auto& s, const auto& a, const auto& b) {
        EXPECT_OK(pointerInterface->foo3(s, a, b));
    }));
}
TEST_F(HidlTest, PointerPassOnlyTest) {
    EXPECT_OK(validationPointerInterface->bar4([&](const auto& s) {
        EXPECT_OK(pointerInterface->foo4(s));
    }));
}
TEST_F(HidlTest, PointerPassTwoEmbeddedTest) {
    EXPECT_OK(validationPointerInterface->bar5([&](const auto& a, const auto& b) {
        EXPECT_OK(pointerInterface->foo5(a, b));
    }));
}
TEST_F(HidlTest, PointerPassIndirectBufferHasDataTest) {
    EXPECT_OK(validationPointerInterface->bar6([&](const auto& a) {
        EXPECT_OK(pointerInterface->foo6(a));
    }));
}
TEST_F(HidlTest, PointerPassTwoIndirectBufferTest) {
    EXPECT_OK(validationPointerInterface->bar7([&](const auto& a, const auto& b) {
        EXPECT_OK(pointerInterface->foo7(a, b));
    }));
}
TEST_F(HidlTest, PointerPassDeeplyIndirectTest) {
    EXPECT_OK(validationPointerInterface->bar8([&](const auto& d) {
        EXPECT_OK(pointerInterface->foo8(d));
    }));
}
TEST_F(HidlTest, PointerPassStringRefTest) {
    EXPECT_OK(validationPointerInterface->bar9([&](const auto& str) {
        EXPECT_OK(pointerInterface->foo9(str));
    }));
}
TEST_F(HidlTest, PointerPassRefVecTest) {
    EXPECT_OK(validationPointerInterface->bar10([&](const auto& v) {
        EXPECT_OK(pointerInterface->foo10(v));
    }));
}
TEST_F(HidlTest, PointerPassVecRefTest) {
    EXPECT_OK(validationPointerInterface->bar11([&](const auto& v) {
        EXPECT_OK(pointerInterface->foo11(v));
    }));
}
TEST_F(HidlTest, PointerPassArrayRefTest) {
    EXPECT_OK(validationPointerInterface->bar12([&](const auto& array) {
        EXPECT_OK(pointerInterface->foo12(array));
    }));
}
TEST_F(HidlTest, PointerPassRefArrayTest) {
    EXPECT_OK(validationPointerInterface->bar13([&](const auto& array) {
        EXPECT_OK(pointerInterface->foo13(array));
    }));
}
TEST_F(HidlTest, PointerPass3RefTest) {
    EXPECT_OK(validationPointerInterface->bar14([&](const auto& p3) {
        EXPECT_OK(pointerInterface->foo14(p3));
    }));
}
TEST_F(HidlTest, PointerPassInt3RefTest) {
    EXPECT_OK(validationPointerInterface->bar15([&](const auto& p3) {
        EXPECT_OK(pointerInterface->foo15(p3));
    }));
}
TEST_F(HidlTest, PointerPassEmbeddedPointersTest) {
    EXPECT_OK(validationPointerInterface->bar16([&](const auto& p) {
        EXPECT_OK(pointerInterface->foo16(p));
    }));
}
TEST_F(HidlTest, PointerPassEmbeddedPointers2Test) {
    EXPECT_OK(validationPointerInterface->bar17([&](const auto& p) {
        EXPECT_OK(pointerInterface->foo17(p));
    }));
}
TEST_F(HidlTest, PointerPassCopiedStringTest) {
    EXPECT_OK(validationPointerInterface->bar18([&](const auto& str_ref, const auto& str_ref2, const auto& str) {
        EXPECT_OK(pointerInterface->foo18(str_ref, str_ref2, str));
    }));
}
TEST_F(HidlTest, PointerPassCopiedVecTest) {
    EXPECT_OK(validationPointerInterface->bar19([&](const auto& a_vec_ref, const auto& a_vec, const auto& a_vec_ref2) {
        EXPECT_OK(pointerInterface->foo19(a_vec_ref, a_vec, a_vec_ref2));
    }));
}
TEST_F(HidlTest, PointerPassBigRefVecTest) {
    EXPECT_OK(validationPointerInterface->bar20([&](const auto& v) {
        EXPECT_FAIL(pointerInterface->foo20(v));
    }));
}
TEST_F(HidlTest, PointerPassMultidimArrayRefTest) {
    EXPECT_OK(validationPointerInterface->bar21([&](const auto& v) {
        EXPECT_OK(pointerInterface->foo21(v));
    }));
}
TEST_F(HidlTest, PointerPassRefMultidimArrayTest) {
    EXPECT_OK(validationPointerInterface->bar22([&](const auto& v) {
        EXPECT_OK(pointerInterface->foo22(v));
    }));
}
TEST_F(HidlTest, PointerGiveOldBufferTest) {
    EXPECT_OK(pointerInterface->bar1([&](const auto& sptr, const auto& s) {
        EXPECT_OK(validationPointerInterface->foo1(sptr, s));
    }));
}
TEST_F(HidlTest, PointerGiveOldBufferTest2) {
    EXPECT_OK(pointerInterface->bar2([&](const auto& s, const auto& a) {
        EXPECT_OK(validationPointerInterface->foo2(s, a));
    }));
}
TEST_F(HidlTest, PointerGiveSameOldBufferPointerTest) {
    EXPECT_OK(pointerInterface->bar3([&](const auto& s, const auto& a, const auto& b) {
        EXPECT_OK(validationPointerInterface->foo3(s, a, b));
    }));
}
TEST_F(HidlTest, PointerGiveOnlyTest) {
    EXPECT_OK(pointerInterface->bar4([&](const auto& s) {
        EXPECT_OK(validationPointerInterface->foo4(s));
    }));
}
TEST_F(HidlTest, PointerGiveTwoEmbeddedTest) {
    EXPECT_OK(pointerInterface->bar5([&](const auto& a, const auto& b) {
        EXPECT_OK(validationPointerInterface->foo5(a, b));
    }));
}
TEST_F(HidlTest, PointerGiveIndirectBufferHasDataTest) {
    EXPECT_OK(pointerInterface->bar6([&](const auto& a) {
        EXPECT_OK(validationPointerInterface->foo6(a));
    }));
}
TEST_F(HidlTest, PointerGiveTwoIndirectBufferTest) {
    EXPECT_OK(pointerInterface->bar7([&](const auto& a, const auto& b) {
        EXPECT_OK(validationPointerInterface->foo7(a, b));
    }));
}
TEST_F(HidlTest, PointerGiveDeeplyIndirectTest) {
    EXPECT_OK(pointerInterface->bar8([&](const auto& d) {
        EXPECT_OK(validationPointerInterface->foo8(d));
    }));
}
TEST_F(HidlTest, PointerGiveStringRefTest) {
    EXPECT_OK(pointerInterface->bar9([&](const auto& str) {
        EXPECT_OK(validationPointerInterface->foo9(str));
    }));
}
TEST_F(HidlTest, PointerGiveRefVecTest) {
    EXPECT_OK(pointerInterface->bar10([&](const auto& v) {
        EXPECT_OK(validationPointerInterface->foo10(v));
    }));
}
TEST_F(HidlTest, PointerGiveVecRefTest) {
    EXPECT_OK(pointerInterface->bar11([&](const auto& v) {
        EXPECT_OK(validationPointerInterface->foo11(v));
    }));
}
TEST_F(HidlTest, PointerGiveArrayRefTest) {
    EXPECT_OK(pointerInterface->bar12([&](const auto& array) {
        EXPECT_OK(validationPointerInterface->foo12(array));
    }));
}
TEST_F(HidlTest, PointerGiveRefArrayTest) {
    EXPECT_OK(pointerInterface->bar13([&](const auto& array) {
        EXPECT_OK(validationPointerInterface->foo13(array));
    }));
}
TEST_F(HidlTest, PointerGive3RefTest) {
    EXPECT_OK(pointerInterface->bar14([&](const auto& p3) {
        EXPECT_OK(validationPointerInterface->foo14(p3));
    }));
}
TEST_F(HidlTest, PointerGiveInt3RefTest) {
    EXPECT_OK(pointerInterface->bar15([&](const auto& p3) {
        EXPECT_OK(validationPointerInterface->foo15(p3));
    }));
}
TEST_F(HidlTest, PointerGiveEmbeddedPointersTest) {
    EXPECT_OK(pointerInterface->bar16([&](const auto& p) {
        EXPECT_OK(validationPointerInterface->foo16(p));
    }));
}
TEST_F(HidlTest, PointerGiveEmbeddedPointers2Test) {
    EXPECT_OK(pointerInterface->bar17([&](const auto& p) {
        EXPECT_OK(validationPointerInterface->foo17(p));
    }));
}
TEST_F(HidlTest, PointerGiveCopiedStringTest) {
    EXPECT_OK(pointerInterface->bar18([&](const auto& str_ref, const auto& str_ref2, const auto& str) {
        EXPECT_OK(validationPointerInterface->foo18(str_ref, str_ref2, str));
    }));
}
TEST_F(HidlTest, PointerGiveCopiedVecTest) {
    EXPECT_OK(pointerInterface->bar19([&](const auto& a_vec_ref, const auto& a_vec, const auto& a_vec_ref2) {
        EXPECT_OK(validationPointerInterface->foo19(a_vec_ref, a_vec, a_vec_ref2));
    }));
}
// This cannot be enabled until _hidl_error is not ignored when
// the remote writeEmbeddedReferencesToParcel.
// TEST_F(HidlTest, PointerGiveBigRefVecTest) {
//     EXPECT_FAIL(pointerInterface->bar20([&](const auto& v) {
//     }));
// }
TEST_F(HidlTest, PointerGiveMultidimArrayRefTest) {
    EXPECT_OK(pointerInterface->bar21([&](const auto& v) {
        EXPECT_OK(validationPointerInterface->foo21(v));
    }));
}
TEST_F(HidlTest, PointerGiveRefMultidimArrayTest) {
    EXPECT_OK(pointerInterface->bar22([&](const auto& v) {
        EXPECT_OK(validationPointerInterface->foo22(v));
    }));
}
TEST_F(HidlTest, PointerReportErrorsTest) {
    Return<int32_t> ret = pointerInterface->getErrors();
    EXPECT_OK(ret);
    EXPECT_EQ(int32_t(ret), 0);
}
#endif

template <class T>
struct WaitForServer {
    static void run(const std::string& serviceName) {
        ::android::hardware::details::waitForHwService(T::descriptor, serviceName);
    }
};

int forkAndRunTests(TestMode mode, bool enableDelayMeasurementTests) {
    pid_t child;
    int status;

    const char* modeText = (mode == BINDERIZED) ? "BINDERIZED" : "PASSTHROUGH";
    ALOGI("Start running tests in %s mode...", modeText);
    fprintf(stdout, "Start running tests in %s mode...\n", modeText);
    fflush(stdout);

    if ((child = fork()) == 0) {
        gHidlEnvironment = static_cast<HidlEnvironment *>(
                ::testing::AddGlobalTestEnvironment(new HidlEnvironment(
                        mode, enableDelayMeasurementTests)));
        int testStatus = RUN_ALL_TESTS();
        if(testStatus == 0) {
            exit(0);
        }
        int failed = ::testing::UnitTest::GetInstance()->failed_test_count();
        if (failed == 0) {
            exit(-testStatus);
        }
        exit(failed);
    }
    waitpid(child, &status, 0 /* options */);
    ALOGI("All tests finished in %s mode.", modeText);
    fprintf(stdout, "All tests finished in %s mode.\n", modeText);
    fflush(stdout);
    return status;
}

void handleStatus(int status, const char *mode) {
    if (status != 0) {
        if (WIFEXITED(status)) {
            status = WEXITSTATUS(status);
            if (status < 0) {
                fprintf(stdout, "    RUN_ALL_TESTS returns %d for %s mode.\n", -status, mode);
            } else {
                fprintf(stdout, "    %d test(s) failed for %s mode.\n", status, mode);
            }
        } else {
            fprintf(stdout, "    ERROR: %s child process exited abnormally with %d\n", mode, status);
        }
    }
}

static void usage(const char *me) {
    fprintf(stderr,
            "usage: %s [-b] [-p] [-d] [GTEST_OPTIONS]\n",
            me);

    fprintf(stderr, "         -b binderized mode only\n");
    fprintf(stderr, "         -p passthrough mode only\n");
    fprintf(stderr, "            (if -b and -p are both missing or both present, "
                                 "both modes are tested.)\n");
    fprintf(stderr, "         -d Enable delay measurement tests\n");
}

int main(int argc, char **argv) {
    setenv("TREBLE_TESTING_OVERRIDE", "true", true);

    const char *me = argv[0];
    bool b = false;
    bool p = false;
    bool d = false;
    struct option longopts[] = {{nullptr,0,nullptr,0}};
    int res;
    while ((res = getopt_long(argc, argv, "hbpd", longopts, nullptr)) >= 0) {
        switch (res) {
            case 'h': {
                usage(me);
                exit(1);
            } break;

            case 'b': {
                b = true;
            } break;

            case 'p': {
                p = true;
            } break;

            case 'd': {
                d = true;
            } break;

            case '?':
            default: {
                // ignore. pass to gTest.
            } break;
        }
    }
    if (!b && !p) {
        b = p = true;
    }

    ::testing::InitGoogleTest(&argc, argv);
    // put test in child process because RUN_ALL_TESTS
    // should not be run twice.
    int pStatus = p ? forkAndRunTests(PASSTHROUGH, d) : 0;
    int bStatus = b ? forkAndRunTests(BINDERIZED, d)  : 0;

    fprintf(stdout, "\n=========================================================\n\n"
                    "    Summary:\n\n");
    if (p) {
        ALOGI("PASSTHROUGH Test result = %d", pStatus);
        handleStatus(pStatus, "PASSTHROUGH");
    }
    if (b) {
        runOnEachServer<WaitForServer>();
        ALOGI("BINDERIZED Test result = %d", bStatus);
        handleStatus(bStatus, "BINDERIZED ");
    }

    if (pStatus == 0 && bStatus == 0) {
        fprintf(stdout, "    Hooray! All tests passed.\n");
    }
    fprintf(stdout, "\n=========================================================\n\n");

    return pStatus + bStatus != 0;
}
