
#define LOG_TAG "hidl_test"

#include "FooCallback.h"
#include <hidl-test/FooHelper.h>
#include <inttypes.h>
#include <log/log.h>
#include <utils/Timers.h>

namespace android {
namespace hardware {
namespace tests {
namespace foo {
namespace V1_0 {
namespace implementation {

enum {
    NOT_REPORTED = -1LL
};

FooCallback::FooCallback()
        : mLock{}, mCond{} {
    for (size_t i = 0; i < invokeInfo.size(); i++) {
        invokeInfo[i].invoked = false;
        invokeInfo[i].timeNs = NOT_REPORTED;
        invokeInfo[i].callerBlockedNs = NOT_REPORTED;
    }
}

Return<void> FooCallback::heyItsYou(
        const sp<IFooCallback> &_cb) {
    nsecs_t start = systemTime();
    ALOGI("SERVER(FooCallback) 1: heyItsYou cb = %p", _cb.get());
    nsecs_t end = systemTime();
    {
        Mutex::Autolock lock(mLock);
        invokeInfo[0].invoked = true;
        invokeInfo[0].timeNs = end - start;
        mCond.signal();
    }
    ALOGI("SERVER(FooCallback) 2: heyItsYou returned");
    return Void();
}

Return<bool> FooCallback::heyItsYouIsntIt(const sp<IFooCallback> &_cb) {
    nsecs_t start = systemTime();
    ALOGI("SERVER(FooCallback) 3: heyItsYouIsntIt cb = %p sleeping for %" PRId64 " seconds", _cb.get(), DELAY_S);
    sleep(DELAY_S);
    ALOGI("SERVER(FooCallback) 4: heyItsYouIsntIt cb = %p responding", _cb.get());
    nsecs_t end = systemTime();
    {
        Mutex::Autolock lock(mLock);
        invokeInfo[1].invoked = true;
        invokeInfo[1].timeNs = end - start;
        mCond.signal();
    }
    ALOGI("SERVER(FooCallback) 5: heyItsYouIsntIt cb = %p responding", _cb.get());
    return true;
}

Return<void> FooCallback::heyItsTheMeaningOfLife(uint8_t tmol) {
    nsecs_t start = systemTime();
    ALOGI("SERVER(FooCallback) 6.1: heyItsTheMeaningOfLife = %d sleeping for %" PRId64 " seconds", tmol, DELAY_S);
    sleep(DELAY_S);
    ALOGI("SERVER(FooCallback) 6.2: heyItsTheMeaningOfLife = %d done sleeping", tmol);
    nsecs_t end = systemTime();
    {
        Mutex::Autolock lock(mLock);
        invokeInfo[2].invoked = true;
        invokeInfo[2].timeNs = end - start;
        mCond.signal();
    }
    ALOGI("SERVER(FooCallback) 6.3: heyItsTheMeaningOfLife returned");
    return Void();
}

Return<void> FooCallback::reportResults(int64_t ns, reportResults_cb cb) {
    ALOGI("SERVER(FooCallback) 8.1: reportResults(%" PRId64 " seconds)", nanoseconds_to_seconds(ns));
    nsecs_t leftToWaitNs = ns;
    bool cond;
    {
        Mutex::Autolock lock(mLock);
        while ((cond = ((!invokeInfo[0].invoked ||
                !invokeInfo[1].invoked ||
                !invokeInfo[2].invoked ||
                invokeInfo[0].callerBlockedNs == NOT_REPORTED ||
                invokeInfo[1].callerBlockedNs == NOT_REPORTED ||
                invokeInfo[2].callerBlockedNs == NOT_REPORTED)   &&
               leftToWaitNs > 0))) {
            nsecs_t start = systemTime();
            ::android::status_t rc = mCond.waitRelative(mLock, leftToWaitNs);
            if (rc != ::android::OK) {
                ALOGW("SERVER(FooCallback)::reportResults(%" PRId64 " ns) Condition::waitRelative(%" PRId64 ") returned error (%d)", ns, leftToWaitNs, rc);
                if (rc == -ETIMEDOUT) {
                    // time's up
                    leftToWaitNs = -INT32_MAX;
                }
                break;
            }
            ALOGI("SERVER(FooCallback)::reportResults(%" PRId64 " ns) Condition::waitRelative was signalled", ns);
            leftToWaitNs -= systemTime() - start;
        }
    }
    ALOGI("SERVER(FooCallback) 8.2: reportResults returned;"
            "invoked? %d, %d, %d; leftToWaitNs = %" PRId64 "; cond = %d",
            invokeInfo[0].invoked, invokeInfo[1].invoked, invokeInfo[2].invoked,
            leftToWaitNs, cond);
    cb(leftToWaitNs, invokeInfo);
    return Void();
}

Return<void> FooCallback::youBlockedMeFor(const hidl_array<int64_t, 3> &ns) {
    ALOGI("SERVER(FooCallback) 7.1: youBlockedMeFor");
    {
        Mutex::Autolock lock(mLock);
        for (size_t i = 0; i < 3; i++) {
            invokeInfo[i].callerBlockedNs = ns[i];
        }
        mCond.signal();
    }
    ALOGI("SERVER(FooCallback) 7.2: returned");
    return Void();
}

IFooCallback* HIDL_FETCH_IFooCallback(const char* /* name */) {
    return new FooCallback();
}

} // namespace implementation
}  // namespace V1_0
}  // namespace foo
}  // namespace tests
}  // namespace hardware
}  // namespace android
