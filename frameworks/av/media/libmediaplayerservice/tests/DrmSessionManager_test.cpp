/*
 * Copyright (C) 2015 The Android Open Source Project
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
#define LOG_TAG "DrmSessionManager_test"
#include <utils/Log.h>

#include <gtest/gtest.h>

#include <media/stagefright/foundation/ADebug.h>
#include <media/stagefright/ProcessInfoInterface.h>
#include <mediadrm/DrmHal.h>
#include <mediadrm/DrmSessionClientInterface.h>
#include <mediadrm/DrmSessionManager.h>

namespace android {

struct FakeProcessInfo : public ProcessInfoInterface {
    FakeProcessInfo() {}
    virtual ~FakeProcessInfo() {}

    virtual bool getPriority(int pid, int* priority) {
        // For testing, use pid as priority.
        // Lower the value higher the priority.
        *priority = pid;
        return true;
    }

    virtual bool isValidPid(int /* pid */) {
        return true;
    }

private:
    DISALLOW_EVIL_CONSTRUCTORS(FakeProcessInfo);
};

struct FakeDrm : public DrmSessionClientInterface {
    FakeDrm() {}
    virtual ~FakeDrm() {}

    virtual bool reclaimSession(const Vector<uint8_t>& sessionId) {
        mReclaimedSessions.push_back(sessionId);
        return true;
    }

    const Vector<Vector<uint8_t> >& reclaimedSessions() const {
        return mReclaimedSessions;
    }

private:
    Vector<Vector<uint8_t> > mReclaimedSessions;

    DISALLOW_EVIL_CONSTRUCTORS(FakeDrm);
};

static const int kTestPid1 = 30;
static const int kTestPid2 = 20;
static const uint8_t kTestSessionId1[] = {1, 2, 3};
static const uint8_t kTestSessionId2[] = {4, 5, 6, 7, 8};
static const uint8_t kTestSessionId3[] = {9, 0};

class DrmSessionManagerTest : public ::testing::Test {
public:
    DrmSessionManagerTest()
        : mDrmSessionManager(new DrmSessionManager(new FakeProcessInfo())),
          mTestDrm1(new FakeDrm()),
          mTestDrm2(new FakeDrm()) {
        GetSessionId(kTestSessionId1, ARRAY_SIZE(kTestSessionId1), &mSessionId1);
        GetSessionId(kTestSessionId2, ARRAY_SIZE(kTestSessionId2), &mSessionId2);
        GetSessionId(kTestSessionId3, ARRAY_SIZE(kTestSessionId3), &mSessionId3);
    }

protected:
    static void GetSessionId(const uint8_t* ids, size_t num, Vector<uint8_t>* sessionId) {
        for (size_t i = 0; i < num; ++i) {
            sessionId->push_back(ids[i]);
        }
    }

    static void ExpectEqSessionInfo(const SessionInfo& info, sp<DrmSessionClientInterface> drm,
            const Vector<uint8_t>& sessionId, int64_t timeStamp) {
        EXPECT_EQ(drm, info.drm);
        EXPECT_TRUE(isEqualSessionId(sessionId, info.sessionId));
        EXPECT_EQ(timeStamp, info.timeStamp);
    }

    void addSession() {
        mDrmSessionManager->addSession(kTestPid1, mTestDrm1, mSessionId1);
        mDrmSessionManager->addSession(kTestPid2, mTestDrm2, mSessionId2);
        mDrmSessionManager->addSession(kTestPid2, mTestDrm2, mSessionId3);
        const PidSessionInfosMap& map = sessionMap();
        EXPECT_EQ(2u, map.size());
        ssize_t index1 = map.indexOfKey(kTestPid1);
        ASSERT_GE(index1, 0);
        const SessionInfos& infos1 = map[index1];
        EXPECT_EQ(1u, infos1.size());
        ExpectEqSessionInfo(infos1[0], mTestDrm1, mSessionId1, 0);

        ssize_t index2 = map.indexOfKey(kTestPid2);
        ASSERT_GE(index2, 0);
        const SessionInfos& infos2 = map[index2];
        EXPECT_EQ(2u, infos2.size());
        ExpectEqSessionInfo(infos2[0], mTestDrm2, mSessionId2, 1);
        ExpectEqSessionInfo(infos2[1], mTestDrm2, mSessionId3, 2);
    }

    const PidSessionInfosMap& sessionMap() {
        return mDrmSessionManager->mSessionMap;
    }

    void testGetLowestPriority() {
        int pid;
        int priority;
        EXPECT_FALSE(mDrmSessionManager->getLowestPriority_l(&pid, &priority));

        addSession();
        EXPECT_TRUE(mDrmSessionManager->getLowestPriority_l(&pid, &priority));

        EXPECT_EQ(kTestPid1, pid);
        FakeProcessInfo processInfo;
        int priority1;
        processInfo.getPriority(kTestPid1, &priority1);
        EXPECT_EQ(priority1, priority);
    }

    void testGetLeastUsedSession() {
        sp<DrmSessionClientInterface> drm;
        Vector<uint8_t> sessionId;
        EXPECT_FALSE(mDrmSessionManager->getLeastUsedSession_l(kTestPid1, &drm, &sessionId));

        addSession();

        EXPECT_TRUE(mDrmSessionManager->getLeastUsedSession_l(kTestPid1, &drm, &sessionId));
        EXPECT_EQ(mTestDrm1, drm);
        EXPECT_TRUE(isEqualSessionId(mSessionId1, sessionId));

        EXPECT_TRUE(mDrmSessionManager->getLeastUsedSession_l(kTestPid2, &drm, &sessionId));
        EXPECT_EQ(mTestDrm2, drm);
        EXPECT_TRUE(isEqualSessionId(mSessionId2, sessionId));

        // mSessionId2 is no longer the least used session.
        mDrmSessionManager->useSession(mSessionId2);
        EXPECT_TRUE(mDrmSessionManager->getLeastUsedSession_l(kTestPid2, &drm, &sessionId));
        EXPECT_EQ(mTestDrm2, drm);
        EXPECT_TRUE(isEqualSessionId(mSessionId3, sessionId));
    }

    sp<DrmSessionManager> mDrmSessionManager;
    sp<FakeDrm> mTestDrm1;
    sp<FakeDrm> mTestDrm2;
    Vector<uint8_t> mSessionId1;
    Vector<uint8_t> mSessionId2;
    Vector<uint8_t> mSessionId3;
};

TEST_F(DrmSessionManagerTest, addSession) {
    addSession();
}

TEST_F(DrmSessionManagerTest, useSession) {
    addSession();

    mDrmSessionManager->useSession(mSessionId1);
    mDrmSessionManager->useSession(mSessionId3);

    const PidSessionInfosMap& map = sessionMap();
    const SessionInfos& infos1 = map.valueFor(kTestPid1);
    const SessionInfos& infos2 = map.valueFor(kTestPid2);
    ExpectEqSessionInfo(infos1[0], mTestDrm1, mSessionId1, 3);
    ExpectEqSessionInfo(infos2[1], mTestDrm2, mSessionId3, 4);
}

TEST_F(DrmSessionManagerTest, removeSession) {
    addSession();

    mDrmSessionManager->removeSession(mSessionId2);

    const PidSessionInfosMap& map = sessionMap();
    EXPECT_EQ(2u, map.size());
    const SessionInfos& infos1 = map.valueFor(kTestPid1);
    const SessionInfos& infos2 = map.valueFor(kTestPid2);
    EXPECT_EQ(1u, infos1.size());
    EXPECT_EQ(1u, infos2.size());
    // mSessionId2 has been removed.
    ExpectEqSessionInfo(infos2[0], mTestDrm2, mSessionId3, 2);
}

TEST_F(DrmSessionManagerTest, removeDrm) {
    addSession();

    sp<FakeDrm> drm = new FakeDrm;
    const uint8_t ids[] = {123};
    Vector<uint8_t> sessionId;
    GetSessionId(ids, ARRAY_SIZE(ids), &sessionId);
    mDrmSessionManager->addSession(kTestPid2, drm, sessionId);

    mDrmSessionManager->removeDrm(mTestDrm2);

    const PidSessionInfosMap& map = sessionMap();
    const SessionInfos& infos2 = map.valueFor(kTestPid2);
    EXPECT_EQ(1u, infos2.size());
    // mTestDrm2 has been removed.
    ExpectEqSessionInfo(infos2[0], drm, sessionId, 3);
}

TEST_F(DrmSessionManagerTest, reclaimSession) {
    EXPECT_FALSE(mDrmSessionManager->reclaimSession(kTestPid1));
    addSession();

    // calling pid priority is too low
    EXPECT_FALSE(mDrmSessionManager->reclaimSession(50));

    EXPECT_TRUE(mDrmSessionManager->reclaimSession(10));
    EXPECT_EQ(1u, mTestDrm1->reclaimedSessions().size());
    EXPECT_TRUE(isEqualSessionId(mSessionId1, mTestDrm1->reclaimedSessions()[0]));

    mDrmSessionManager->removeSession(mSessionId1);

    // add a session from a higher priority process.
    sp<FakeDrm> drm = new FakeDrm;
    const uint8_t ids[] = {1, 3, 5};
    Vector<uint8_t> sessionId;
    GetSessionId(ids, ARRAY_SIZE(ids), &sessionId);
    mDrmSessionManager->addSession(15, drm, sessionId);

    EXPECT_TRUE(mDrmSessionManager->reclaimSession(18));
    EXPECT_EQ(1u, mTestDrm2->reclaimedSessions().size());
    // mSessionId2 is reclaimed.
    EXPECT_TRUE(isEqualSessionId(mSessionId2, mTestDrm2->reclaimedSessions()[0]));
}

TEST_F(DrmSessionManagerTest, getLowestPriority) {
    testGetLowestPriority();
}

TEST_F(DrmSessionManagerTest, getLeastUsedSession_l) {
    testGetLeastUsedSession();
}

} // namespace android
