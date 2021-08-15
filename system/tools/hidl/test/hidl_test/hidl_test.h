#ifndef HIDL_TEST_H_
#define HIDL_TEST_H_

#include <android/hardware/tests/bar/1.0/IBar.h>
#include <android/hardware/tests/baz/1.0/IBaz.h>
#include <android/hardware/tests/hash/1.0/IHash.h>
#include <android/hardware/tests/inheritance/1.0/IChild.h>
#include <android/hardware/tests/inheritance/1.0/IFetcher.h>
#include <android/hardware/tests/inheritance/1.0/IParent.h>
#include <android/hardware/tests/memory/1.0/IMemoryTest.h>
#include <android/hardware/tests/multithread/1.0/IMultithread.h>
#include <android/hardware/tests/pointer/1.0/IGraph.h>
#include <android/hardware/tests/pointer/1.0/IPointer.h>
#include <android/hardware/tests/safeunion/1.0/ISafeUnion.h>
#include <android/hardware/tests/safeunion/cpp/1.0/ICppSafeUnion.h>
#include <android/hardware/tests/trie/1.0/ITrie.h>

template <template <typename Type> class Service>
void runOnEachServer(void) {
    using ::android::hardware::tests::bar::V1_0::IBar;
    using ::android::hardware::tests::baz::V1_0::IBaz;
    using ::android::hardware::tests::hash::V1_0::IHash;
    using ::android::hardware::tests::inheritance::V1_0::IChild;
    using ::android::hardware::tests::inheritance::V1_0::IFetcher;
    using ::android::hardware::tests::inheritance::V1_0::IParent;
    using ::android::hardware::tests::memory::V1_0::IMemoryTest;
    using ::android::hardware::tests::multithread::V1_0::IMultithread;
    using ::android::hardware::tests::pointer::V1_0::IGraph;
    using ::android::hardware::tests::pointer::V1_0::IPointer;
    using ::android::hardware::tests::safeunion::cpp::V1_0::ICppSafeUnion;
    using ::android::hardware::tests::safeunion::V1_0::ISafeUnion;
    using ::android::hardware::tests::trie::V1_0::ITrie;

    Service<IMemoryTest>::run("memory");
    Service<IChild>::run("child");
    Service<IParent>::run("parent");
    Service<IFetcher>::run("fetcher");
    Service<IBaz>::run("baz");
    Service<IBar>::run("foo");
    Service<IHash>::run("default");
    Service<IGraph>::run("graph");
    Service<IPointer>::run("pointer");
    Service<IMultithread>::run("multithread");
    Service<ITrie>::run("trie");
    Service<ICppSafeUnion>::run("default");
    Service<ISafeUnion>::run("safeunion");
}

#endif  // HIDL_TEST_H_
