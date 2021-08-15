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
#define LOG_TAG "AData_test"

#include <gtest/gtest.h>
#include <utils/RefBase.h>
//#include <utils/StrongPointer.h>

#include <media/stagefright/foundation/AData.h>
#include <media/stagefright/foundation/ABuffer.h>

namespace android {

class ADataTest : public ::testing::Test {
};

// ============ AUnion

struct Events {
    int dtor;
    int ctor_empty;
    int ctor_copy;
};

struct EventCounter : public RefBase {
    EventCounter(int *counter, int magic=1234) : mCounter(counter), mMagic(magic) { }
    virtual ~EventCounter() { ++*mCounter; mMagic = 0; }
    int magic() const { return mMagic; }
private:
    int *mCounter;
    int mMagic;
};

struct DerivedCounter : public EventCounter {
    DerivedCounter(int *counter, int magic=1234) : EventCounter(counter, magic) { }
};

TEST_F(ADataTest, AUnion_Test) {
    AUnion<int, const char *, char> u;
    u.emplace<int>(4);
    u.del<int>();
    EXPECT_EQ(4, u.get<int>()); // verify that del<> is a no-op for trivial types, such as int.
                                // specifically, verify that it does not clear the objet memory

    u.emplace<const char *>("hello");
    EXPECT_STREQ("hello", u.get<const char *>());
    u.del<const char *>();

    // u.del<char *>();
    // u.emplace<const int>(4);
    u.emplace<void>();
    u.del<void>();

    u.emplace<int>(~0);
    u.del<int>();
    EXPECT_EQ(~0, u.get<int>());
    u.emplace<char>(0x15);
    // verify that rest of memory after char is cleared upon construction
    EXPECT_EQ(0, memcmp((char *)(&u) + sizeof(char), "\0\0\0", 3));
    EXPECT_EQ(0x15, u.get<char>());
    u.del<char>();

    AUnion<EventCounter, EventCounter *> d;
    int destructions = 0;

    d.emplace<EventCounter>(&destructions);
    d.del<EventCounter>();
    EXPECT_EQ(1, destructions);

    EventCounter *ctr = new EventCounter(&destructions);
    d.emplace<EventCounter *>(ctr);
    d.del<EventCounter *>();
    EXPECT_EQ(1, destructions);

    delete ctr;
    EXPECT_EQ(2, destructions);

    AUnion<std::shared_ptr<EventCounter>, std::unique_ptr<EventCounter>> md;
    md.emplace<std::shared_ptr<EventCounter>>(new EventCounter(&destructions));
    std::shared_ptr<EventCounter> copy(md.get<std::shared_ptr<EventCounter>>());
    std::weak_ptr<EventCounter> weak(copy);
    EXPECT_EQ(2, destructions);

    copy.reset();
    EXPECT_EQ(2, destructions);
    md.del<std::shared_ptr<EventCounter>>();
    EXPECT_EQ(3, destructions);
    EXPECT_TRUE(weak.expired());

    md.emplace<std::unique_ptr<EventCounter>>(new EventCounter(&destructions));
    EXPECT_EQ(3, destructions);

    std::unique_ptr<EventCounter> unique = std::move(md.get<std::unique_ptr<EventCounter>>());
    EXPECT_EQ(3, destructions);
    EXPECT_FALSE((bool)md.get<std::unique_ptr<EventCounter>>());

    md.del<std::unique_ptr<EventCounter>>();
    EXPECT_EQ(3, destructions);
    md.emplace<std::unique_ptr<EventCounter>>(std::move(unique));
    EXPECT_TRUE((bool)md.get<std::unique_ptr<EventCounter>>());
    EXPECT_EQ(3, destructions);

    md.del<std::unique_ptr<EventCounter>>();
    EXPECT_EQ(4, destructions);
}

TEST_F(ADataTest, AData_StaticTest) {
    using namespace std;

    static_assert(is_copy_assignable<shared_ptr<EventCounter>>::value, "");
    static_assert(is_copy_constructible<shared_ptr<EventCounter>>::value, "");
    static_assert(is_default_constructible<shared_ptr<EventCounter>>::value, "");

    static_assert(is_copy_assignable<weak_ptr<DerivedCounter>>::value, "");
    static_assert(is_copy_constructible<weak_ptr<DerivedCounter>>::value, "");
    static_assert(is_default_constructible<weak_ptr<DerivedCounter>>::value, "");

    static_assert(!is_copy_assignable<unique_ptr<DerivedCounter>>::value, "");
    static_assert(!is_copy_constructible<unique_ptr<DerivedCounter>>::value, "");
    static_assert(is_default_constructible<unique_ptr<DerivedCounter>>::value, "");

    static_assert(is_copy_assignable<sp<EventCounter>>::value, "");
    static_assert(is_copy_constructible<sp<EventCounter>>::value, "");
    static_assert(is_default_constructible<sp<EventCounter>>::value, "");

    static_assert(is_copy_assignable<wp<EventCounter>>::value, "");
    static_assert(is_copy_constructible<wp<EventCounter>>::value, "");
    static_assert(is_default_constructible<wp<EventCounter>>::value, "");

    static_assert(is_convertible<shared_ptr<DerivedCounter>, shared_ptr<EventCounter>>::value, "");
    static_assert(!is_convertible<shared_ptr<EventCounter>, shared_ptr<DerivedCounter>>::value, "");

    static_assert(is_convertible<unique_ptr<DerivedCounter>, unique_ptr<EventCounter>>::value, "");
    static_assert(!is_convertible<unique_ptr<EventCounter>, unique_ptr<DerivedCounter>>::value, "");

    static_assert(is_convertible<unique_ptr<DerivedCounter>, shared_ptr<EventCounter>>::value, "");
    static_assert(!is_convertible<shared_ptr<DerivedCounter>, unique_ptr<EventCounter>>::value, "");

    static_assert(is_convertible<weak_ptr<DerivedCounter>, weak_ptr<EventCounter>>::value, "");
    static_assert(!is_convertible<weak_ptr<EventCounter>, weak_ptr<DerivedCounter>>::value, "");

    static_assert(is_convertible<shared_ptr<DerivedCounter>, weak_ptr<EventCounter>>::value, "");
    static_assert(!is_convertible<weak_ptr<DerivedCounter>, shared_ptr<EventCounter>>::value, "");

    static_assert(is_convertible<sp<EventCounter>, sp<RefBase>>::value, "");
    static_assert(is_convertible<sp<RefBase>, sp<EventCounter>>::value, "YES");

    static_assert(is_convertible<wp<EventCounter>, wp<RefBase>>::value, "");
    static_assert(is_convertible<wp<RefBase>, wp<EventCounter>>::value, "YES");

    static_assert(is_convertible<sp<EventCounter>, wp<RefBase>>::value, "");
    static_assert(!is_convertible<wp<EventCounter>, sp<RefBase>>::value, "");
}

TEST_F(ADataTest, AData_SampleTest) {
    AData<int, float>::Basic data;
    int i = 1;
    float f = 7.0f;

    data.set(5);
    EXPECT_TRUE(data.find(&i));
    EXPECT_FALSE(data.find(&f));
    EXPECT_EQ(i, 5);

    data.set(6.0f);
    EXPECT_FALSE(data.find(&i));
    EXPECT_TRUE(data.find(&f));
    EXPECT_EQ(f, 6.0f);

    AData<int, sp<RefBase>>::RelaxedBasic objdata; // relaxed type support
    sp<ABuffer> buf = new ABuffer(16), buf2;
    sp<RefBase> obj;

    objdata.set(buf);
    EXPECT_TRUE(objdata.find(&buf2));
    EXPECT_EQ(buf, buf2);
    EXPECT_FALSE(objdata.find(&i));
    EXPECT_TRUE(objdata.find(&obj));
    EXPECT_TRUE(obj == buf);

    obj = buf;
    objdata.set(obj); // storing as sp<RefBase>
    EXPECT_FALSE(objdata.find(&buf2));  // not stored as ABuffer(!)
    EXPECT_TRUE(objdata.find(&obj));
}

struct SampleTypeFlagger {
    typedef unsigned type;
    enum Flags : type {
        kEmpty = 100,
        kInt,
        kConstCharPtr,
        kEventCounter,
        kEventCounterPointer,
        kEventCounterSharedPointer,
        kEventCounterUniquePointer,
        kEventCounterWeakPointer,
        kEventCounterSP,
        kEventCounterWP,
    };
    constexpr static type mask = ~Flags(0);
    constexpr static type flagFor(void*) { return kEmpty; }
    constexpr static type flagFor(int*) { return kInt; }
    constexpr static type flagFor(const char**) { return kConstCharPtr; }
    constexpr static type flagFor(EventCounter*) { return kEventCounter; }
    constexpr static type flagFor(EventCounter**) { return kEventCounterPointer; }
    constexpr static
    type flagFor(std::shared_ptr<EventCounter>*) { return kEventCounterSharedPointer; }
    constexpr static
    type flagFor(std::unique_ptr<EventCounter>*) { return kEventCounterUniquePointer; }
    constexpr static type flagFor(std::weak_ptr<EventCounter>*) { return kEventCounterWeakPointer; }
    constexpr static type flagFor(sp<EventCounter>*) { return kEventCounterSP; }
    constexpr static type flagFor(wp<EventCounter>*) { return kEventCounterWP; }
    constexpr static bool canDeleteAs(type object, type del) { return del == object; }
    template <typename T> struct store { typedef T as_type; };
};

TEST_F(ADataTest, AData_SimpleTest) {
    int _int = 0;
    const char *_constCharPtr = NULL;
    AData<int, const char *>::Custom<SampleTypeFlagger> u;
    EXPECT_FALSE(u.used());
    EXPECT_FALSE(u.find<int>(&_int));
    EXPECT_FALSE(u.find<const char *>(&_constCharPtr));

    EXPECT_TRUE(u.set<int>(4));
    EXPECT_TRUE(u.used());
    EXPECT_TRUE(u.find<int>(&_int));
    EXPECT_EQ(4, _int);
    EXPECT_FALSE(u.find<const char *>(&_constCharPtr));
    EXPECT_EQ(NULL, _constCharPtr);

    EXPECT_TRUE(u.clear());
    EXPECT_FALSE(u.used());
    EXPECT_FALSE(u.find<int>(&_int));
    EXPECT_FALSE(u.find<const char *>(&_constCharPtr));

    EXPECT_TRUE(u.set<int>(5));
    EXPECT_TRUE(u.set<int>(6));
    EXPECT_TRUE(u.find<int>(&_int));
    EXPECT_EQ(6, _int);

    EXPECT_TRUE(u.set<const char *>("hello"));
    EXPECT_TRUE(u.used());
    EXPECT_FALSE(u.find<int>(&_int));
    EXPECT_TRUE(u.find<const char *>(&_constCharPtr));
    EXPECT_STREQ("hello", _constCharPtr);

    EXPECT_TRUE(u.clear());
    EXPECT_FALSE(u.used());
    EXPECT_FALSE(u.find<int>(&_int));
    EXPECT_FALSE(u.find<const char *>(&_constCharPtr));

    EXPECT_TRUE(u.set<const char *>("world"));
    EXPECT_TRUE(u.set<const char *>("!!"));
    EXPECT_TRUE(u.used());
    EXPECT_FALSE(u.find<int>(&_int));
    EXPECT_TRUE(u.find<const char *>(&_constCharPtr));
    EXPECT_STREQ("!!", _constCharPtr);

    EXPECT_FALSE(u.find(&_int));
    EXPECT_TRUE(u.find(&_constCharPtr));
}

void set(std::unique_ptr<int> &dst, std::unique_ptr<int> &&src) {
    dst = std::move(src);
}

void set(std::unique_ptr<int> &dst, std::unique_ptr<int> &src) {
    dst = std::move(src);
}

TEST_F(ADataTest, AData_CopyMoveTest) {
    int destructions = 0;
    int _int = 0;
    std::shared_ptr<EventCounter> _shared;
    std::unique_ptr<EventCounter> _unique;
    std::weak_ptr<EventCounter> _weak;
    const std::shared_ptr<EventCounter> _constShared(new EventCounter(&destructions));
    const std::unique_ptr<EventCounter> _constUnique = nullptr;

    AData<int, std::weak_ptr<EventCounter>, std::shared_ptr<EventCounter>,
            std::unique_ptr<EventCounter>>::Basic u;

    // test that data is empty
    EXPECT_FALSE(u.used());
    EXPECT_FALSE(u.find(&_int));
    EXPECT_FALSE(u.find(&_shared));
    EXPECT_FALSE(u.remove(&_unique));
    EXPECT_FALSE(u.find(&_weak));

    // test that integer can be stored and read
    EXPECT_TRUE(u.set<int>(1));
    EXPECT_TRUE(u.used());
    EXPECT_TRUE(u.find(&_int));
    EXPECT_EQ(1, _int);
    EXPECT_FALSE(u.find(&_shared));
    EXPECT_FALSE(u.remove(&_unique));
    EXPECT_FALSE(u.find(&_weak));

    // test that movable type (unique_ptr) can be moved in and read out, and it moves
    _unique = std::unique_ptr<EventCounter>(new EventCounter(&destructions, 123));
    EXPECT_TRUE(u.set(std::move(_unique)));
    EXPECT_FALSE((bool)_unique);
    EXPECT_TRUE(u.used());
    EXPECT_FALSE(u.find(&_int));
    EXPECT_FALSE(u.find(&_shared));
    EXPECT_FALSE(u.find(&_weak));
    EXPECT_TRUE(u.remove(&_unique));
    EXPECT_TRUE((bool)_unique);
    if (_unique) {
        EXPECT_EQ(123, _unique->magic());
    }

    // the unique value should have been removed but still accessible as nullptr
    EXPECT_TRUE(u.remove(&_unique));
    EXPECT_FALSE((bool)_unique);
    EXPECT_EQ(1, destructions);

    // test that movable-only type (unique_ptr) can be stored without moving (and is still
    // moved)
    _unique = std::unique_ptr<EventCounter>(new EventCounter(&destructions, 321));
    EXPECT_TRUE(u.set(std::move(_unique)));
    EXPECT_FALSE((bool)_unique);
    EXPECT_TRUE(u.set(std::unique_ptr<EventCounter>(new EventCounter(&destructions, 1234))));
    EXPECT_EQ(2, destructions);
    EXPECT_TRUE(u.remove(&_unique));
    EXPECT_TRUE((bool)_unique);
    if (_unique) {
        EXPECT_EQ(1234, _unique->magic());
    }
    EXPECT_TRUE(u.set(std::move(_unique)));
    EXPECT_EQ(2, destructions);
    EXPECT_TRUE(u.clear());
    EXPECT_EQ(3, destructions);
    EXPECT_FALSE(u.find(&_int));
    EXPECT_FALSE(u.find(&_shared));
    EXPECT_FALSE(u.remove(&_unique));
    EXPECT_FALSE(u.find(&_weak));

    // u.set(_constUnique);

    // test that copiable & movable type (shared_ptr) is copied unless explicitly moved.
    _shared = std::make_shared<EventCounter>(&destructions, 234);
    EXPECT_EQ(1L, _shared.use_count());
    EXPECT_TRUE(u.set(_shared));
    EXPECT_TRUE((bool)_shared);
    if (_shared) {
        EXPECT_EQ(234, _shared->magic());
    }

    EXPECT_EQ(2L, _shared.use_count());
    EXPECT_FALSE(u.find(&_int));
    EXPECT_FALSE(u.remove(&_unique));
    EXPECT_FALSE(u.find(&_weak));
    EXPECT_TRUE(u.find(&_shared));
    EXPECT_EQ(2L, _shared.use_count());
    EXPECT_TRUE((bool)_shared);
    if (_shared) {
        EXPECT_EQ(234, _shared->magic());
    }

    // explicitly move in shared_ptr
    EXPECT_TRUE(u.set(std::move(_shared)));
    EXPECT_EQ(0, _shared.use_count()); // shared should be nullptr
    EXPECT_FALSE((bool)_shared);
    EXPECT_TRUE(u.find(&_shared));
    EXPECT_EQ(2L, _shared.use_count()); // now both u and _shared contains the object
    EXPECT_TRUE((bool)_shared);
    if (_shared) {
        EXPECT_EQ(234, _shared->magic());
    }
    EXPECT_FALSE(u.find(&_int));
    EXPECT_FALSE(u.remove(&_unique));
    EXPECT_FALSE(u.find(&_weak));
    EXPECT_TRUE(u.find(&_shared));
    EXPECT_EQ(2L, _shared.use_count()); // still both u and _shared contains the object

    EXPECT_TRUE(u.clear());
    EXPECT_TRUE(_shared.unique()); // now only _shared contains the object

    EXPECT_TRUE(u.set(_constShared));
    EXPECT_EQ(2L, _constShared.use_count()); // even though it is const, we can add a use count
    EXPECT_TRUE(u.find(&_shared));
    EXPECT_EQ(3L, _shared.use_count()); // now u, _shared and _constShared contains the const object
    EXPECT_TRUE((bool)_shared);
    if (_shared) {
        EXPECT_EQ(1234, _shared->magic());
    }

    // test that weak pointer can be copied in (support for moving is from C++14 only)
    _weak = _shared;
    EXPECT_EQ(_weak.use_count(), _shared.use_count());
    EXPECT_TRUE(u.set(_weak));

    _weak.reset();
    EXPECT_EQ(_weak.use_count(), 0);

    EXPECT_FALSE(u.find(&_int));
    EXPECT_FALSE(u.remove(&_unique));
    EXPECT_FALSE(u.find(&_shared));
    EXPECT_TRUE(u.find(&_weak));
    EXPECT_EQ(_weak.use_count(), _shared.use_count());
    EXPECT_EQ(_weak.lock(), _shared);

    // we can remove a weak pointer multiple times
    _weak.reset();
    EXPECT_TRUE(u.find(&_weak));
    EXPECT_EQ(_weak.use_count(), _shared.use_count());
    EXPECT_EQ(_weak.lock(), _shared);
    EXPECT_TRUE(u.clear());
    EXPECT_FALSE(u.find(&_int));
    EXPECT_FALSE(u.remove(&_unique));
    EXPECT_FALSE(u.find(&_shared));
    EXPECT_FALSE(u.find(&_weak));
};

TEST_F(ADataTest, AData_RelaxedCopyMoveTest) {
    int destructions = 0;
    int _int = 0;
    std::shared_ptr<DerivedCounter> _shared;
    std::unique_ptr<DerivedCounter> _unique, _unique2;
    std::weak_ptr<DerivedCounter> _weak;
    std::shared_ptr<EventCounter> _shared_base;
    std::unique_ptr<EventCounter> _unique_base;
    std::weak_ptr<EventCounter> _weak_base;
    const std::shared_ptr<DerivedCounter> _constShared(new DerivedCounter(&destructions));
    const std::unique_ptr<DerivedCounter> _constUnique = nullptr;

    AData<int, std::unique_ptr<EventCounter>, std::shared_ptr<EventCounter>,
            std::weak_ptr<EventCounter>>::RelaxedBasic u;

    // test that data is empty
    EXPECT_FALSE(u.used());
    EXPECT_FALSE(u.find(&_int));
    EXPECT_FALSE(u.find(&_shared));
    EXPECT_FALSE(u.remove(&_unique));
    EXPECT_FALSE(u.find(&_weak));
    EXPECT_FALSE(u.find(&_shared_base));
    EXPECT_FALSE(u.remove(&_unique_base));
    EXPECT_FALSE(u.find(&_weak_base));

    // test that integer can be stored and read
    EXPECT_TRUE(u.set<int>(1));
    EXPECT_TRUE(u.used());
    EXPECT_TRUE(u.find(&_int));
    EXPECT_EQ(1, _int);
    EXPECT_FALSE(u.find(&_shared));
    EXPECT_FALSE(u.remove(&_unique));
    EXPECT_FALSE(u.find(&_weak));
    EXPECT_FALSE(u.find(&_shared_base));
    EXPECT_FALSE(u.remove(&_unique_base));
    EXPECT_FALSE(u.find(&_weak_base));

    // test that movable type (unique_ptr) can be moved in and read out, and it moves
    _unique = std::unique_ptr<DerivedCounter>(new DerivedCounter(&destructions, 123));
    EXPECT_TRUE(u.set(std::move(_unique)));
    EXPECT_FALSE((bool)_unique);
    EXPECT_TRUE(u.used());
    EXPECT_FALSE(u.find(&_int));
    EXPECT_FALSE(u.find(&_shared));
    EXPECT_FALSE(u.find(&_weak));
    EXPECT_TRUE(u.remove(&_unique));
    EXPECT_TRUE((bool)_unique);
    if (_unique) {
        EXPECT_EQ(123, _unique->magic());
    }

    // the unique value should have been removed but still accessible as nullptr
    EXPECT_TRUE(u.remove(&_unique));
    EXPECT_FALSE((bool)_unique);
    EXPECT_EQ(1, destructions);

    EXPECT_FALSE(u.find(&_shared_base));
    EXPECT_TRUE(u.remove(&_unique_base));
    EXPECT_FALSE((bool)_unique_base);
    EXPECT_FALSE(u.find(&_weak_base));

    // test that movable-only type (unique_ptr) can be stored without moving (and is still
    // moved)
    _unique = std::unique_ptr<DerivedCounter>(new DerivedCounter(&destructions, 321));
    EXPECT_TRUE(u.set(std::move(_unique)));
    EXPECT_FALSE((bool)_unique);
    EXPECT_TRUE(u.set(std::unique_ptr<DerivedCounter>(new DerivedCounter(&destructions, 1234))));
    EXPECT_EQ(2, destructions);
    EXPECT_TRUE(u.remove(&_unique));
    EXPECT_TRUE((bool)_unique);
    if (_unique) {
        EXPECT_EQ(1234, _unique->magic());
    }
    EXPECT_TRUE(u.set(std::move(_unique)));
    EXPECT_EQ(2, destructions);
    EXPECT_TRUE(u.clear());
    EXPECT_EQ(3, destructions);
    EXPECT_FALSE(u.find(&_int));
    EXPECT_FALSE(u.find(&_shared));
    EXPECT_FALSE(u.remove(&_unique));
    EXPECT_FALSE(u.find(&_weak));
    EXPECT_FALSE(u.find(&_shared_base));
    EXPECT_FALSE(u.remove(&_unique_base));
    EXPECT_FALSE(u.find(&_weak_base));

    // test that unique pointer can be set and removed as base type (but removed as derived only
    // if it was set as derived type)
    _unique = std::unique_ptr<DerivedCounter>(new DerivedCounter(&destructions, 321));
    EXPECT_TRUE(u.set(std::move(_unique)));
    EXPECT_FALSE((bool)_unique);
    EXPECT_TRUE(u.remove(&_unique_base));
    EXPECT_TRUE((bool)_unique_base);
    if (_unique_base) {
        EXPECT_EQ(321, _unique_base->magic());
    }
    EXPECT_TRUE(u.remove(&_unique));
    EXPECT_FALSE((bool)_unique);

    EXPECT_TRUE(u.set(std::move(_unique_base)));
    EXPECT_FALSE((bool)_unique_base);
    EXPECT_FALSE(u.remove(&_unique));
    EXPECT_FALSE((bool)_unique);
    EXPECT_TRUE(u.remove(&_unique_base));
    EXPECT_TRUE((bool)_unique_base);
    if (_unique_base) {
        EXPECT_EQ(321, _unique_base->magic());
    }

    EXPECT_EQ(3, destructions);
    EXPECT_TRUE(u.remove(&_unique_base));
    EXPECT_EQ(4, destructions);
    EXPECT_FALSE((bool)_unique_base);
    EXPECT_FALSE(u.find(&_int));
    EXPECT_FALSE(u.find(&_shared));
    EXPECT_FALSE(u.find(&_shared_base));
    EXPECT_FALSE(u.find(&_weak));
    EXPECT_FALSE(u.find(&_weak_base));

    // u.set(_constUnique);

    // test that copiable & movable type (shared_ptr) is copied unless explicitly moved.
    _shared = std::make_shared<DerivedCounter>(&destructions, 234);
    EXPECT_EQ(1L, _shared.use_count());
    EXPECT_TRUE(u.set(_shared));
    EXPECT_TRUE((bool)_shared);
    if (_shared) {
        EXPECT_EQ(234, _shared->magic());
    }

    EXPECT_EQ(2L, _shared.use_count());
    EXPECT_FALSE(u.find(&_int));
    EXPECT_FALSE(u.remove(&_unique));
    EXPECT_FALSE(u.find(&_weak));
    EXPECT_TRUE(u.find(&_shared));
    EXPECT_FALSE(u.remove(&_unique_base));
    EXPECT_FALSE(u.find(&_weak_base));
    EXPECT_EQ(2L, _shared.use_count());
    EXPECT_TRUE((bool)_shared);
    if (_shared) {
        EXPECT_EQ(234, _shared->magic());
    }

    // explicitly move in shared_ptr
    EXPECT_TRUE(u.set(std::move(_shared)));
    EXPECT_EQ(0, _shared.use_count()); // shared should be nullptr
    EXPECT_FALSE((bool)_shared);
    EXPECT_TRUE(u.find(&_shared));
    EXPECT_EQ(2L, _shared.use_count()); // now both u and _shared contains the object
    EXPECT_TRUE((bool)_shared);
    if (_shared) {
        EXPECT_EQ(234, _shared->magic());
    }
    EXPECT_FALSE(u.find(&_int));
    EXPECT_FALSE(u.remove(&_unique));
    EXPECT_FALSE(u.find(&_weak));
    EXPECT_FALSE(u.remove(&_unique_base));
    EXPECT_FALSE(u.find(&_weak_base));
    EXPECT_TRUE(u.find(&_shared));
    EXPECT_EQ(2L, _shared.use_count()); // still both u and _shared contains the object

    EXPECT_TRUE(u.clear());
    EXPECT_TRUE(_shared.unique()); // now only _shared contains the object

    EXPECT_TRUE(u.set(_constShared));
    EXPECT_EQ(2L, _constShared.use_count()); // even though it is const, we can add a use count
    EXPECT_TRUE(u.find(&_shared));
    EXPECT_EQ(3L, _shared.use_count()); // now u, _shared and _constShared contains the const object
    EXPECT_TRUE((bool)_shared);
    if (_shared) {
        EXPECT_EQ(1234, _shared->magic());
    }

    // test that shared pointer can be set and removed as base type (but removed as derived only
    // if it was set as derived type)
    EXPECT_TRUE(u.find(&_shared_base));
    EXPECT_TRUE((bool)_shared_base);
    if (_shared_base) {
        EXPECT_EQ(1234, _shared_base->magic());
    }
    EXPECT_EQ(4L, _shared.use_count()); // now u, _shared, _constShared and _shared_base contains
                                        // the const object
    _shared.reset();
    EXPECT_EQ(3L, _shared_base.use_count()); // now u, _constShared and _shared_base contains it
    EXPECT_TRUE(u.clear());
    EXPECT_EQ(2L, _shared_base.use_count()); // now _constShared and _shared_base contains it

    EXPECT_TRUE(u.set(_shared_base));        // now u_ also contains it as base class
    EXPECT_EQ(3L, _shared_base.use_count());
    EXPECT_FALSE(u.find(&_shared)); // cannot get it as derived type
    EXPECT_FALSE((bool)_shared);
    _shared_base.reset();
    EXPECT_TRUE(u.find(&_shared_base)); // can still get it as base type
    EXPECT_TRUE((bool)_shared_base);
    if (_shared_base) {
        EXPECT_EQ(1234, _shared_base->magic());
    }
    _shared = std::static_pointer_cast<DerivedCounter>(_shared_base);
    EXPECT_FALSE(u.find(&_int));
    EXPECT_FALSE(u.remove(&_unique));
    EXPECT_FALSE(u.remove(&_unique_base));
    EXPECT_FALSE(u.find(&_weak));
    EXPECT_FALSE(u.find(&_weak_base));

    // test that weak pointer can be copied in (support for moving is from C++14 only)
    _weak = _shared;
    EXPECT_EQ(_weak.use_count(), _shared.use_count());
    EXPECT_TRUE(u.set(_weak));

    _weak.reset();
    EXPECT_EQ(_weak.use_count(), 0);

    EXPECT_FALSE(u.find(&_int));
    EXPECT_FALSE(u.remove(&_unique));
    EXPECT_FALSE(u.find(&_shared));
    EXPECT_FALSE(u.remove(&_unique_base));
    EXPECT_FALSE(u.find(&_shared_base));
    EXPECT_TRUE(u.find(&_weak));
    EXPECT_EQ(_weak.use_count(), _shared.use_count());
    EXPECT_EQ(_weak.lock(), _shared);

    // we can remove a weak pointer multiple times
    _weak.reset();
    EXPECT_TRUE(u.find(&_weak));
    EXPECT_EQ(_weak.use_count(), _shared.use_count());
    EXPECT_EQ(_weak.lock(), _shared);
    EXPECT_TRUE(u.clear());
    EXPECT_FALSE(u.find(&_int));
    EXPECT_FALSE(u.remove(&_unique));
    EXPECT_FALSE(u.find(&_shared));
    EXPECT_FALSE(u.find(&_weak));
    EXPECT_FALSE(u.remove(&_unique_base));
    EXPECT_FALSE(u.find(&_shared_base));
    EXPECT_FALSE(u.find(&_weak_base));

    // test that weak pointer can be set and removed as base type (but removed as derived only
    // if it was set as derived type)
    _weak = _shared;
    EXPECT_TRUE(u.set(_weak));
    EXPECT_TRUE(u.find(&_weak_base));
    EXPECT_FALSE(_weak_base.expired());
    if (!_weak_base.expired()) {
        EXPECT_EQ(1234, _weak_base.lock()->magic());
    }
    // now _shared, _constShared and _shared_base contains the const object
    EXPECT_EQ(3L, _weak.use_count());
    _weak.reset();
    EXPECT_EQ(3L, _weak_base.use_count()); // _weak did not hold a reference
    _shared.reset();
    EXPECT_EQ(2L, _weak_base.use_count()); // now u, _constShared and _shared_base contains it
    EXPECT_TRUE(u.clear());
    EXPECT_FALSE(u.find(&_int));
    EXPECT_FALSE(u.remove(&_unique));
    EXPECT_FALSE(u.find(&_shared));
    EXPECT_FALSE(u.find(&_weak));
    EXPECT_FALSE(u.remove(&_unique_base));
    EXPECT_FALSE(u.find(&_shared_base));
    EXPECT_FALSE(u.find(&_weak_base));

    EXPECT_TRUE(u.set(_weak_base)); // now u_ also contains it as base class
    EXPECT_FALSE(u.find(&_weak));   // cannot get it as derived type
    EXPECT_TRUE(_weak.expired());
    _weak_base.reset();
    EXPECT_TRUE(u.find(&_weak_base)); // can still get it as base type
    EXPECT_FALSE(_weak_base.expired());
    if (!_weak_base.expired()) {
        EXPECT_EQ(1234, _weak_base.lock()->magic());
    }
};

TEST_F(ADataTest, AData_AndroidSpTest) {
    int destructions = 0;
    int _int = 0;
    sp<EventCounter> _shared;
    wp<EventCounter> _weak;
    const sp<EventCounter> _constShared(new EventCounter(&destructions));

    AData<int, sp<EventCounter>, wp<EventCounter>>::Strict<uint8_t> u;

    // test that data is empty
    EXPECT_FALSE(u.used());
    EXPECT_FALSE(u.find(&_int));
    EXPECT_FALSE(u.find(&_shared));
    EXPECT_FALSE(u.find(&_weak));

    // test that integer can be stored and read
    EXPECT_TRUE(u.set<int>(1));
    EXPECT_TRUE(u.used());
    EXPECT_TRUE(u.find(&_int));
    EXPECT_EQ(1, _int);
    EXPECT_FALSE(u.find(&_shared));
    EXPECT_FALSE(u.find(&_weak));

    // test that copiable & movable type (shared_ptr) is copied unless explicitly moved.
    _shared = new EventCounter(&destructions, 234);
    _weak = _shared; // used for tracking #234

    EXPECT_TRUE(u.set(_shared));
    EXPECT_TRUE((bool)_shared.get());
    if (_shared.get()) {
        EXPECT_EQ(234, _shared->magic());
    }

    _shared.clear();
    EXPECT_EQ(NULL, _shared.get());
    EXPECT_NE(nullptr, _weak.promote().get()); // u still holds object

    EXPECT_FALSE(u.find(&_int));
    EXPECT_FALSE(u.find(&_weak));
    EXPECT_TRUE(u.find(&_shared)); // now u and _shared both hold object
    EXPECT_TRUE((bool)_shared.get());
    if (_shared.get()) {
        EXPECT_EQ(234, _shared->magic());
    }
    // verify the find did not move out object
    _shared.clear();
    EXPECT_EQ(NULL, _shared.get());
    EXPECT_NE(nullptr, _weak.promote().get()); // u still holds object
    EXPECT_TRUE(u.find(&_shared)); // now u and _shared both hold object
    if (_shared.get()) {
        EXPECT_EQ(234, _shared->magic());
    }

    // verify that we can set object multiple times
    EXPECT_TRUE(u.set(_shared));

    // explicitly move in sp
    EXPECT_TRUE(u.set(std::move(_shared)));
    EXPECT_FALSE((bool)_shared.get()); // android also clears sp<> on move...
    EXPECT_TRUE(u.find(&_shared)); // still can get it back
    EXPECT_TRUE((bool)_shared.get());
    if (_shared.get()) {
        EXPECT_EQ(234, _shared->magic());
    }
    EXPECT_FALSE(u.find(&_int));
    EXPECT_FALSE(u.find(&_weak));

    EXPECT_TRUE(u.used());
    EXPECT_TRUE(u.clear()); // now only _shared contains the object
    EXPECT_FALSE(u.used());

    // we still hold a copy
    EXPECT_TRUE((bool)_shared.get());
    EXPECT_FALSE(u.find(&_int));
    EXPECT_FALSE(u.find(&_shared)); // _shared still contains the object

    EXPECT_TRUE(u.set(_constShared));
    EXPECT_TRUE(u.find(&_shared)); // now _shared contains _constShared
    EXPECT_EQ(NULL, _weak.promote().get()); // original _shared is now lost

    EXPECT_TRUE((bool)_shared.get());
    if (_shared.get()) {
        EXPECT_EQ(1234, _shared->magic());
    }
    EXPECT_TRUE(u.clear());

    // test that wp can be copied in
    _weak = _shared;
    EXPECT_TRUE(u.set(_weak));

    _weak.clear();

    EXPECT_FALSE(u.find(&_int));
    EXPECT_FALSE(u.find(&_shared));
    EXPECT_TRUE(u.find(&_weak));
    EXPECT_EQ(_weak.promote(), _shared);

    // we can remove a weak pointer multiple times
    _weak.clear();
    EXPECT_TRUE(u.find(&_weak));
    EXPECT_EQ(_weak.promote(), _shared);
    EXPECT_TRUE(u.clear());
    EXPECT_FALSE(u.find(&_int));
    EXPECT_FALSE(u.find(&_shared));
    EXPECT_FALSE(u.find(&_weak));
};

TEST_F(ADataTest, AData_RelaxedAndroidSpTest) {
    int destructions = 0;
    int _int = 0;
    sp<EventCounter> _shared;
    wp<EventCounter> _weak;
    sp<RefBase> _shared_base;
    wp<RefBase> _weak_base;
    const sp<EventCounter> _constShared(new EventCounter(&destructions));

    AData<int, sp<RefBase>, wp<RefBase>>::Relaxed<uint16_t> u;

    // test that data is empty
    EXPECT_FALSE(u.used());
    EXPECT_FALSE(u.find(&_int));
    EXPECT_FALSE(u.find(&_shared));
    EXPECT_FALSE(u.find(&_weak));
    EXPECT_FALSE(u.find(&_shared_base));
    EXPECT_FALSE(u.find(&_weak_base));

    // test that integer can be stored and read
    EXPECT_TRUE(u.set<int>(1));
    EXPECT_TRUE(u.used());
    EXPECT_TRUE(u.find(&_int));
    EXPECT_EQ(1, _int);
    EXPECT_FALSE(u.find(&_shared));
    EXPECT_FALSE(u.find(&_weak));
    EXPECT_FALSE(u.find(&_shared_base));
    EXPECT_FALSE(u.find(&_weak_base));

    // test that copiable & movable type (shared_ptr) is copied unless explicitly moved.
    _shared = new EventCounter(&destructions, 234);
    _weak = _shared; // used for tracking #234

    EXPECT_TRUE(u.set(_shared));
    EXPECT_TRUE((bool)_shared.get());
    if (_shared.get()) {
        EXPECT_EQ(234, _shared->magic());
    }

    _shared.clear();
    EXPECT_EQ(NULL, _shared.get());
    EXPECT_NE(nullptr, _weak.promote().get()); // u still holds object

    EXPECT_FALSE(u.find(&_int));
    EXPECT_TRUE(u.find(&_shared)); // now u and _shared both hold object
    EXPECT_TRUE((bool)_shared.get());
    if (_shared.get()) {
        EXPECT_EQ(234, _shared->magic());
    }
    // verify the find did not move out object
    _shared.clear();
    EXPECT_EQ(NULL, _shared.get());
    EXPECT_NE(nullptr, _weak.promote().get()); // u still holds object
    EXPECT_TRUE(u.find(&_shared)); // now u and _shared both hold object
    if (_shared.get()) {
        EXPECT_EQ(234, _shared->magic());
    }

    // verify that we can set object multiple times
    EXPECT_TRUE(u.set(_shared));

    // explicitly move in sp
    EXPECT_TRUE(u.set(std::move(_shared)));
    EXPECT_FALSE((bool)_shared.get()); // android also clears sp<> on move...
    EXPECT_TRUE(u.find(&_shared)); // still can get it back
    EXPECT_TRUE((bool)_shared.get());
    if (_shared.get()) {
        EXPECT_EQ(234, _shared->magic());
    }
    EXPECT_FALSE(u.find(&_int));
    EXPECT_FALSE(u.find(&_weak));
    EXPECT_FALSE(u.find(&_weak_base));

    EXPECT_TRUE(u.used());
    EXPECT_TRUE(u.clear()); // now only _shared contains the object
    EXPECT_FALSE(u.used());

    // we still hold a copy
    EXPECT_TRUE((bool)_shared.get());
    EXPECT_FALSE(u.find(&_int));
    EXPECT_FALSE(u.find(&_shared)); // _shared still contains the object

    EXPECT_TRUE(u.set(_constShared));
    EXPECT_TRUE(u.find(&_shared)); // now _shared contains _constShared
    EXPECT_EQ(NULL, _weak.promote().get()); // original _shared is now lost

    EXPECT_TRUE((bool)_shared.get());
    if (_shared.get()) {
        EXPECT_EQ(1234, _shared->magic());
    }
    EXPECT_TRUE(u.clear());

    // test that shared pointer can be set and removed as base type (but removed as derived only
    // if it was set as derived type)
    EXPECT_TRUE(u.set(_constShared));
    EXPECT_TRUE(u.find(&_shared_base));
    EXPECT_TRUE((bool)_shared_base.get());
    if (_shared_base.get()) {
        EXPECT_EQ(1234, static_cast<EventCounter*>(_shared_base.get())->magic());
    }
    _shared.clear();
    EXPECT_TRUE(u.clear());
    EXPECT_TRUE((bool)_shared_base.get());
    if (_shared_base.get()) {
        EXPECT_EQ(1234, static_cast<EventCounter*>(_shared_base.get())->magic());
    }

    EXPECT_TRUE(u.set(_shared_base)); // now u contains it as base class
    EXPECT_TRUE((bool)_shared_base.get());
    EXPECT_FALSE(u.find(&_shared)); // cannot get it as derived type
    EXPECT_FALSE((bool)_shared.get());
    _shared_base.clear();
    EXPECT_TRUE(u.find(&_shared_base)); // can still get it as base type
    EXPECT_TRUE((bool)_shared_base.get());
    if (_shared_base.get()) {
        EXPECT_EQ(1234, static_cast<EventCounter*>(_shared_base.get())->magic());
    }
    _shared = static_cast<DerivedCounter*>(_shared_base.get());
    EXPECT_FALSE(u.find(&_int));
    EXPECT_FALSE(u.find(&_weak));
    EXPECT_FALSE(u.find(&_weak_base));

    // test that wp can be copied in
    _weak = _shared;
    EXPECT_TRUE(u.set(_weak));

    _weak.clear();

    EXPECT_FALSE(u.find(&_int));
    EXPECT_FALSE(u.find(&_shared));
    EXPECT_FALSE(u.find(&_shared_base));
    EXPECT_TRUE(u.find(&_weak));
    EXPECT_EQ(_weak.promote(), _shared);

    // we can remove a weak pointer multiple times
    _weak.clear();
    EXPECT_TRUE(u.find(&_weak));
    EXPECT_EQ(_weak.promote(), _shared);
    EXPECT_TRUE(u.clear());
    EXPECT_FALSE(u.find(&_int));
    EXPECT_FALSE(u.find(&_shared));
    EXPECT_FALSE(u.find(&_weak));
    EXPECT_FALSE(u.find(&_shared_base));
    EXPECT_FALSE(u.find(&_weak_base));

    // test that weak pointer can be set and removed as base type (but removed as derived only
    // if it was set as derived type)
    _weak = _shared;
    EXPECT_TRUE(u.set(_weak));
    EXPECT_TRUE(u.find(&_weak_base));
    EXPECT_TRUE(_weak_base.promote().get() == _shared.get());

    _weak.clear();
    _shared.clear();
    EXPECT_TRUE(u.clear());
    EXPECT_FALSE(u.find(&_int));
    EXPECT_FALSE(u.find(&_shared));
    EXPECT_FALSE(u.find(&_weak));
    EXPECT_FALSE(u.find(&_shared_base));
    EXPECT_FALSE(u.find(&_weak_base));

    EXPECT_TRUE(u.set(_weak_base)); // now u_ also contains it as base class
    EXPECT_FALSE(u.find(&_weak));   // cannot get it as derived type
    EXPECT_FALSE(_weak.promote().get());
    _weak_base.clear();
    EXPECT_TRUE(u.find(&_weak_base)); // can still get it as base type
    EXPECT_TRUE(_weak_base.promote().get());
    if (_weak_base.promote().get()) {
        EXPECT_EQ(1234, static_cast<EventCounter*>(_weak_base.promote().get())->magic());
    }
};

TEST_F(ADataTest, AData_AssignmentTest) {
    typedef AData<sp<ABuffer>, int32_t>::Basic Data;

    sp<ABuffer> buf1 = new ABuffer((void *)"hello", 6);
    wp<ABuffer> buf1w = buf1;

    Data obj1;
    obj1.set(buf1);
    EXPECT_NE(buf1w.promote(), nullptr);
    buf1.clear();
    EXPECT_NE(buf1w.promote(), nullptr);
    obj1.clear();
    EXPECT_EQ(buf1w.promote(), nullptr);

    buf1 = new ABuffer((void *)"again", 6);
    buf1w = buf1;

    obj1.set(buf1);
    EXPECT_TRUE(obj1.used());
    Data obj2 = obj1;

    sp<ABuffer> buf2;
    EXPECT_TRUE(obj2.find(&buf2));
    EXPECT_EQ(buf2, buf1);
    buf1.clear();
    buf2.clear();
    EXPECT_NE(buf1w.promote(), nullptr);
    obj1.clear();
    EXPECT_NE(buf1w.promote(), nullptr);
    obj2.clear();
    EXPECT_EQ(buf1w.promote(), nullptr);

    buf1 = new ABuffer((void *)"still", 6);
    buf1w = buf1;

    obj1.set(buf1);
    EXPECT_TRUE(obj1.used());
    obj2 = std::move(obj1);
    EXPECT_FALSE(obj1.used());

    EXPECT_TRUE(obj2.find(&buf2));
    EXPECT_EQ(buf2, buf1);
    buf1.clear();
    buf2.clear();
    EXPECT_NE(buf1w.promote(), nullptr);
    obj2.clear();
    EXPECT_EQ(buf1w.promote(), nullptr);

    typedef AData<sp<ABuffer>, std::unique_ptr<int32_t>>::Basic Data2;
    Data2 obj3, obj4;

    buf1 = new ABuffer((void *)"hence", 6);
    obj3.set(buf1);
    obj4 = std::move(obj3);
    EXPECT_FALSE(obj3.used());
    EXPECT_TRUE(obj4.find(&buf2));
    EXPECT_EQ(buf2, buf1);
}

} // namespace android
