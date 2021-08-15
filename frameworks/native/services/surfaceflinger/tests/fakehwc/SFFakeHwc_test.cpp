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

// #define LOG_NDEBUG 0
#undef LOG_TAG
#define LOG_TAG "FakeHwcTest"

#include "FakeComposerClient.h"
#include "FakeComposerService.h"
#include "FakeComposerUtils.h"

#include <gui/DisplayEventReceiver.h>
#include <gui/ISurfaceComposer.h>
#include <gui/LayerDebugInfo.h>
#include <gui/LayerState.h>
#include <gui/Surface.h>
#include <gui/SurfaceComposerClient.h>

#include <android/hidl/manager/1.0/IServiceManager.h>
#include <android/looper.h>
#include <android/native_window.h>
#include <binder/ProcessState.h>
#include <hwbinder/ProcessState.h>
#include <log/log.h>
#include <private/gui/ComposerService.h>
#include <ui/DisplayInfo.h>
#include <utils/Looper.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <limits>

using namespace std::chrono_literals;

using namespace android;
using namespace android::hardware;

using namespace sftest;

namespace {

// Mock test helpers
using ::testing::Invoke;
using ::testing::Return;
using ::testing::SetArgPointee;
using ::testing::_;

using Transaction = SurfaceComposerClient::Transaction;

///////////////////////////////////////////////

struct TestColor {
public:
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

constexpr static TestColor RED = {195, 63, 63, 255};
constexpr static TestColor LIGHT_RED = {255, 177, 177, 255};
constexpr static TestColor GREEN = {63, 195, 63, 255};
constexpr static TestColor BLUE = {63, 63, 195, 255};
constexpr static TestColor DARK_GRAY = {63, 63, 63, 255};
constexpr static TestColor LIGHT_GRAY = {200, 200, 200, 255};

// Fill an RGBA_8888 formatted surface with a single color.
static void fillSurfaceRGBA8(const sp<SurfaceControl>& sc, const TestColor& color,
                             bool unlock = true) {
    ANativeWindow_Buffer outBuffer;
    sp<Surface> s = sc->getSurface();
    ASSERT_TRUE(s != nullptr);
    ASSERT_EQ(NO_ERROR, s->lock(&outBuffer, nullptr));
    uint8_t* img = reinterpret_cast<uint8_t*>(outBuffer.bits);
    for (int y = 0; y < outBuffer.height; y++) {
        for (int x = 0; x < outBuffer.width; x++) {
            uint8_t* pixel = img + (4 * (y * outBuffer.stride + x));
            pixel[0] = color.r;
            pixel[1] = color.g;
            pixel[2] = color.b;
            pixel[3] = color.a;
        }
    }
    if (unlock) {
        ASSERT_EQ(NO_ERROR, s->unlockAndPost());
    }
}

inline RenderState makeSimpleRect(int left, int top, int right, int bottom) {
    RenderState res;
    res.mDisplayFrame = hwc_rect_t{left, top, right, bottom};
    res.mPlaneAlpha = 1.0f;
    res.mSwapCount = 0;
    res.mSourceCrop = hwc_frect_t{0.f, 0.f, static_cast<float>(right - left),
                                  static_cast<float>(bottom - top)};
    return res;
}

inline RenderState makeSimpleRect(unsigned int left, unsigned int top, unsigned int right,
                                  unsigned int bottom) {
    EXPECT_LE(left, static_cast<unsigned int>(INT_MAX));
    EXPECT_LE(top, static_cast<unsigned int>(INT_MAX));
    EXPECT_LE(right, static_cast<unsigned int>(INT_MAX));
    EXPECT_LE(bottom, static_cast<unsigned int>(INT_MAX));
    return makeSimpleRect(static_cast<int>(left), static_cast<int>(top), static_cast<int>(right),
                          static_cast<int>(bottom));
}

////////////////////////////////////////////////

class DisplayTest : public ::testing::Test {
public:
    class MockComposerClient : public FakeComposerClient {
    public:
        MOCK_METHOD2(getDisplayType, Error(Display display, ComposerClient::DisplayType* outType));
        MOCK_METHOD4(getDisplayAttribute,
                     Error(Display display, Config config, IComposerClient::Attribute attribute,
                           int32_t* outValue));

        // Re-routing to basic fake implementation
        Error getDisplayAttributeFake(Display display, Config config,
                                      IComposerClient::Attribute attribute, int32_t* outValue) {
            return FakeComposerClient::getDisplayAttribute(display, config, attribute, outValue);
        }
    };

protected:
    static int processDisplayEvents(int fd, int events, void* data);

    void SetUp() override;
    void TearDown() override;

    void waitForDisplayTransaction();
    bool waitForHotplugEvent(PhysicalDisplayId displayId, bool connected);

    sp<IComposer> mFakeService;
    sp<SurfaceComposerClient> mComposerClient;

    MockComposerClient* mMockComposer;

    std::unique_ptr<DisplayEventReceiver> mReceiver;
    sp<Looper> mLooper;;
    std::deque<DisplayEventReceiver::Event> mReceivedDisplayEvents;
};

void DisplayTest::SetUp() {
    // TODO: The mMockComposer should be a unique_ptr, but it needs to
    // outlive the test class.  Currently ComposerClient only dies
    // when the service is replaced. The Mock deletes itself when
    // removeClient is called on it, which is ugly.  This can be
    // changed if HIDL ServiceManager allows removing services or
    // ComposerClient starts taking the ownership of the contained
    // implementation class. Moving the fake class to the HWC2
    // interface instead of the current Composer interface might also
    // change the situation.
    mMockComposer = new MockComposerClient;
    sp<ComposerClient> client = new ComposerClient(mMockComposer);
    mFakeService = new FakeComposerService(client);
    (void)mFakeService->registerAsService("mock");

    android::hardware::ProcessState::self()->startThreadPool();
    android::ProcessState::self()->startThreadPool();

    EXPECT_CALL(*mMockComposer, getDisplayType(PRIMARY_DISPLAY, _))
            .WillOnce(DoAll(SetArgPointee<1>(IComposerClient::DisplayType::PHYSICAL),
                            Return(Error::NONE)));
    // Primary display will be queried twice for all 5 attributes. One
    // set of queries comes from the SurfaceFlinger proper an the
    // other set from the VR composer.
    // TODO: Is VR composer always present? Change to atLeast(5)?
    EXPECT_CALL(*mMockComposer, getDisplayAttribute(PRIMARY_DISPLAY, 1, _, _))
            .Times(2 * 5)
            .WillRepeatedly(Invoke(mMockComposer, &MockComposerClient::getDisplayAttributeFake));

    startSurfaceFlinger();

    // Fake composer wants to enable VSync injection
    mMockComposer->onSurfaceFlingerStart();

    mComposerClient = new SurfaceComposerClient;
    ASSERT_EQ(NO_ERROR, mComposerClient->initCheck());

    mReceiver.reset(new DisplayEventReceiver());
    mLooper = new Looper(false);
    mLooper->addFd(mReceiver->getFd(), 0, ALOOPER_EVENT_INPUT, processDisplayEvents, this);
}

void DisplayTest::TearDown() {
    mLooper = nullptr;
    mReceiver = nullptr;

    mComposerClient->dispose();
    mComposerClient = nullptr;

    // Fake composer needs to release SurfaceComposerClient before the stop.
    mMockComposer->onSurfaceFlingerStop();
    stopSurfaceFlinger();

    mFakeService = nullptr;
    // TODO: Currently deleted in FakeComposerClient::removeClient(). Devise better lifetime
    // management.
    mMockComposer = nullptr;
}


int DisplayTest::processDisplayEvents(int /*fd*/, int /*events*/, void* data) {
    auto self = static_cast<DisplayTest*>(data);

    ssize_t n;
    DisplayEventReceiver::Event buffer[1];

    while ((n = self->mReceiver->getEvents(buffer, 1)) > 0) {
        for (int i=0 ; i<n ; i++) {
            self->mReceivedDisplayEvents.push_back(buffer[i]);
        }
    }
    ALOGD_IF(n < 0, "Error reading events (%s)\n", strerror(-n));
    return 1;
}

void DisplayTest::waitForDisplayTransaction() {
    // Both a refresh and a vsync event are needed to apply pending display
    // transactions.
    mMockComposer->refreshDisplay(EXTERNAL_DISPLAY);
    mMockComposer->runVSyncAndWait();

    // Extra vsync and wait to avoid a 10% flake due to a race.
    mMockComposer->runVSyncAndWait();
}

bool DisplayTest::waitForHotplugEvent(PhysicalDisplayId displayId, bool connected) {
    int waitCount = 20;
    while (waitCount--) {
        while (!mReceivedDisplayEvents.empty()) {
            auto event = mReceivedDisplayEvents.front();
            mReceivedDisplayEvents.pop_front();

            ALOGV_IF(event.header.type == DisplayEventReceiver::DISPLAY_EVENT_HOTPLUG,
                     "event hotplug: displayId %" ANDROID_PHYSICAL_DISPLAY_ID_FORMAT
                     ", connected %d\t",
                     event.header.displayId, event.hotplug.connected);

            if (event.header.type == DisplayEventReceiver::DISPLAY_EVENT_HOTPLUG &&
                event.header.displayId == displayId && event.hotplug.connected == connected) {
                return true;
            }
        }

        mLooper->pollOnce(1);
    }

    return false;
}

TEST_F(DisplayTest, Hotplug) {
    ALOGD("DisplayTest::Hotplug");

    EXPECT_CALL(*mMockComposer, getDisplayType(EXTERNAL_DISPLAY, _))
            .Times(2)
            .WillRepeatedly(DoAll(SetArgPointee<1>(IComposerClient::DisplayType::PHYSICAL),
                                  Return(Error::NONE)));
    // The attribute queries will get done twice. This is for defaults
    EXPECT_CALL(*mMockComposer, getDisplayAttribute(EXTERNAL_DISPLAY, 1, _, _))
            .Times(2 * 3)
            .WillRepeatedly(Invoke(mMockComposer, &MockComposerClient::getDisplayAttributeFake));
    // ... and then special handling for dimensions. Specifying these
    // rules later means that gmock will try them first, i.e.,
    // ordering of width/height vs. the default implementation for
    // other queries is significant.
    EXPECT_CALL(*mMockComposer,
                getDisplayAttribute(EXTERNAL_DISPLAY, 1, IComposerClient::Attribute::WIDTH, _))
            .Times(2)
            .WillRepeatedly(DoAll(SetArgPointee<3>(400), Return(Error::NONE)));

    EXPECT_CALL(*mMockComposer,
                getDisplayAttribute(EXTERNAL_DISPLAY, 1, IComposerClient::Attribute::HEIGHT, _))
            .Times(2)
            .WillRepeatedly(DoAll(SetArgPointee<3>(200), Return(Error::NONE)));

    mMockComposer->hotplugDisplay(EXTERNAL_DISPLAY, IComposerCallback::Connection::CONNECTED);

    waitForDisplayTransaction();

    EXPECT_TRUE(waitForHotplugEvent(EXTERNAL_DISPLAY, true));

    {
        const auto display = SurfaceComposerClient::getPhysicalDisplayToken(EXTERNAL_DISPLAY);
        ASSERT_FALSE(display == nullptr);

        DisplayInfo info;
        ASSERT_EQ(NO_ERROR, SurfaceComposerClient::getDisplayInfo(display, &info));
        ASSERT_EQ(400u, info.w);
        ASSERT_EQ(200u, info.h);

        auto surfaceControl =
                mComposerClient->createSurface(String8("Display Test Surface Foo"), info.w, info.h,
                                               PIXEL_FORMAT_RGBA_8888, 0);
        ASSERT_TRUE(surfaceControl != nullptr);
        ASSERT_TRUE(surfaceControl->isValid());
        fillSurfaceRGBA8(surfaceControl, BLUE);

        {
            TransactionScope ts(*mMockComposer);
            ts.setDisplayLayerStack(display, 0);

            ts.setLayer(surfaceControl, INT32_MAX - 2)
                .show(surfaceControl);
        }
    }

    mMockComposer->hotplugDisplay(EXTERNAL_DISPLAY, IComposerCallback::Connection::DISCONNECTED);

    mMockComposer->clearFrames();

    mMockComposer->hotplugDisplay(EXTERNAL_DISPLAY, IComposerCallback::Connection::CONNECTED);

    waitForDisplayTransaction();

    EXPECT_TRUE(waitForHotplugEvent(EXTERNAL_DISPLAY, false));
    EXPECT_TRUE(waitForHotplugEvent(EXTERNAL_DISPLAY, true));

    {
        const auto display = SurfaceComposerClient::getPhysicalDisplayToken(EXTERNAL_DISPLAY);
        ASSERT_FALSE(display == nullptr);

        DisplayInfo info;
        ASSERT_EQ(NO_ERROR, SurfaceComposerClient::getDisplayInfo(display, &info));
        ASSERT_EQ(400u, info.w);
        ASSERT_EQ(200u, info.h);

        auto surfaceControl =
                mComposerClient->createSurface(String8("Display Test Surface Bar"), info.w, info.h,
                                               PIXEL_FORMAT_RGBA_8888, 0);
        ASSERT_TRUE(surfaceControl != nullptr);
        ASSERT_TRUE(surfaceControl->isValid());
        fillSurfaceRGBA8(surfaceControl, BLUE);

        {
            TransactionScope ts(*mMockComposer);
            ts.setDisplayLayerStack(display, 0);

            ts.setLayer(surfaceControl, INT32_MAX - 2)
                .show(surfaceControl);
        }
    }
    mMockComposer->hotplugDisplay(EXTERNAL_DISPLAY, IComposerCallback::Connection::DISCONNECTED);
}

TEST_F(DisplayTest, HotplugPrimaryDisplay) {
    ALOGD("DisplayTest::HotplugPrimaryDisplay");

    mMockComposer->hotplugDisplay(PRIMARY_DISPLAY, IComposerCallback::Connection::DISCONNECTED);

    waitForDisplayTransaction();

    EXPECT_TRUE(waitForHotplugEvent(PRIMARY_DISPLAY, false));

    {
        const auto display = SurfaceComposerClient::getPhysicalDisplayToken(PRIMARY_DISPLAY);
        EXPECT_FALSE(display == nullptr);

        DisplayInfo info;
        auto result = SurfaceComposerClient::getDisplayInfo(display, &info);
        EXPECT_NE(NO_ERROR, result);
    }

    mMockComposer->clearFrames();

    EXPECT_CALL(*mMockComposer, getDisplayType(PRIMARY_DISPLAY, _))
            .Times(2)
            .WillRepeatedly(DoAll(SetArgPointee<1>(IComposerClient::DisplayType::PHYSICAL),
                                  Return(Error::NONE)));
    // The attribute queries will get done twice. This is for defaults
    EXPECT_CALL(*mMockComposer, getDisplayAttribute(PRIMARY_DISPLAY, 1, _, _))
            .Times(2 * 3)
            .WillRepeatedly(Invoke(mMockComposer, &MockComposerClient::getDisplayAttributeFake));
    // ... and then special handling for dimensions. Specifying these
    // rules later means that gmock will try them first, i.e.,
    // ordering of width/height vs. the default implementation for
    // other queries is significant.
    EXPECT_CALL(*mMockComposer,
                getDisplayAttribute(PRIMARY_DISPLAY, 1, IComposerClient::Attribute::WIDTH, _))
            .Times(2)
            .WillRepeatedly(DoAll(SetArgPointee<3>(400), Return(Error::NONE)));

    EXPECT_CALL(*mMockComposer,
                getDisplayAttribute(PRIMARY_DISPLAY, 1, IComposerClient::Attribute::HEIGHT, _))
            .Times(2)
            .WillRepeatedly(DoAll(SetArgPointee<3>(200), Return(Error::NONE)));

    mMockComposer->hotplugDisplay(PRIMARY_DISPLAY, IComposerCallback::Connection::CONNECTED);

    waitForDisplayTransaction();

    EXPECT_TRUE(waitForHotplugEvent(PRIMARY_DISPLAY, true));

    {
        const auto display = SurfaceComposerClient::getPhysicalDisplayToken(PRIMARY_DISPLAY);
        EXPECT_FALSE(display == nullptr);

        DisplayInfo info;
        auto result = SurfaceComposerClient::getDisplayInfo(display, &info);
        EXPECT_EQ(NO_ERROR, result);
        ASSERT_EQ(400u, info.w);
        ASSERT_EQ(200u, info.h);
    }
}

////////////////////////////////////////////////

class TransactionTest : public ::testing::Test {
protected:
    // Layer array indexing constants.
    constexpr static int BG_LAYER = 0;
    constexpr static int FG_LAYER = 1;

    static void SetUpTestCase();
    static void TearDownTestCase();

    void SetUp() override;
    void TearDown() override;

    sp<SurfaceComposerClient> mComposerClient;
    sp<SurfaceControl> mBGSurfaceControl;
    sp<SurfaceControl> mFGSurfaceControl;
    std::vector<RenderState> mBaseFrame;
    uint32_t mDisplayWidth;
    uint32_t mDisplayHeight;

    static FakeComposerClient* sFakeComposer;
};

FakeComposerClient* TransactionTest::sFakeComposer;

void TransactionTest::SetUpTestCase() {
    // TODO: See TODO comment at DisplayTest::SetUp for background on
    // the lifetime of the FakeComposerClient.
    sFakeComposer = new FakeComposerClient;
    sp<ComposerClient> client = new ComposerClient(sFakeComposer);
    sp<IComposer> fakeService = new FakeComposerService(client);
    (void)fakeService->registerAsService("mock");

    android::hardware::ProcessState::self()->startThreadPool();
    android::ProcessState::self()->startThreadPool();

    startSurfaceFlinger();

    // Fake composer wants to enable VSync injection
    sFakeComposer->onSurfaceFlingerStart();
}

void TransactionTest::TearDownTestCase() {
    // Fake composer needs to release SurfaceComposerClient before the stop.
    sFakeComposer->onSurfaceFlingerStop();
    stopSurfaceFlinger();
    // TODO: This is deleted when the ComposerClient calls
    // removeClient. Devise better lifetime control.
    sFakeComposer = nullptr;
}

void TransactionTest::SetUp() {
    ALOGI("TransactionTest::SetUp");
    mComposerClient = new SurfaceComposerClient;
    ASSERT_EQ(NO_ERROR, mComposerClient->initCheck());

    ALOGI("TransactionTest::SetUp - display");
    const auto display = SurfaceComposerClient::getPhysicalDisplayToken(PRIMARY_DISPLAY);
    ASSERT_FALSE(display == nullptr);

    DisplayInfo info;
    ASSERT_EQ(NO_ERROR, SurfaceComposerClient::getDisplayInfo(display, &info));

    mDisplayWidth = info.w;
    mDisplayHeight = info.h;

    // Background surface
    mBGSurfaceControl = mComposerClient->createSurface(String8("BG Test Surface"), mDisplayWidth,
                                                       mDisplayHeight, PIXEL_FORMAT_RGBA_8888, 0);
    ASSERT_TRUE(mBGSurfaceControl != nullptr);
    ASSERT_TRUE(mBGSurfaceControl->isValid());
    fillSurfaceRGBA8(mBGSurfaceControl, BLUE);

    // Foreground surface
    mFGSurfaceControl = mComposerClient->createSurface(String8("FG Test Surface"), 64, 64,
                                                       PIXEL_FORMAT_RGBA_8888, 0);
    ASSERT_TRUE(mFGSurfaceControl != nullptr);
    ASSERT_TRUE(mFGSurfaceControl->isValid());

    fillSurfaceRGBA8(mFGSurfaceControl, RED);

    Transaction t;
    t.setDisplayLayerStack(display, 0);

    t.setLayer(mBGSurfaceControl, INT32_MAX - 2);
    t.show(mBGSurfaceControl);

    t.setLayer(mFGSurfaceControl, INT32_MAX - 1);
    t.setPosition(mFGSurfaceControl, 64, 64);
    t.show(mFGSurfaceControl);

    // Synchronous transaction will stop this thread, so we set up a
    // delayed, off-thread vsync request before closing the
    // transaction. In the test code this is usually done with
    // TransactionScope. Leaving here in the 'vanilla' form for
    // reference.
    ASSERT_EQ(0, sFakeComposer->getFrameCount());
    sFakeComposer->runVSyncAfter(1ms);
    t.apply();
    sFakeComposer->waitUntilFrame(1);

    // Reference data. This is what the HWC should see.
    static_assert(BG_LAYER == 0 && FG_LAYER == 1, "Unexpected enum values for array indexing");
    mBaseFrame.push_back(makeSimpleRect(0u, 0u, mDisplayWidth, mDisplayHeight));
    mBaseFrame[BG_LAYER].mSwapCount = 1;
    mBaseFrame.push_back(makeSimpleRect(64, 64, 64 + 64, 64 + 64));
    mBaseFrame[FG_LAYER].mSwapCount = 1;

    auto frame = sFakeComposer->getFrameRects(0);
    ASSERT_TRUE(framesAreSame(mBaseFrame, frame));
}

void TransactionTest::TearDown() {
    ALOGD("TransactionTest::TearDown");

    mComposerClient->dispose();
    mBGSurfaceControl = 0;
    mFGSurfaceControl = 0;
    mComposerClient = 0;

    sFakeComposer->runVSyncAndWait();
    mBaseFrame.clear();
    sFakeComposer->clearFrames();
    ASSERT_EQ(0, sFakeComposer->getFrameCount());

    sp<ISurfaceComposer> sf(ComposerService::getComposerService());
    std::vector<LayerDebugInfo> layers;
    status_t result = sf->getLayerDebugInfo(&layers);
    if (result != NO_ERROR) {
        ALOGE("Failed to get layers %s %d", strerror(-result), result);
    } else {
        // If this fails, the test being torn down leaked layers.
        EXPECT_EQ(0u, layers.size());
        if (layers.size() > 0) {
            for (auto layer = layers.begin(); layer != layers.end(); ++layer) {
                std::cout << to_string(*layer).c_str();
            }
            // To ensure the next test has clean slate, will run the class
            // tear down and setup here.
            TearDownTestCase();
            SetUpTestCase();
        }
    }
    ALOGD("TransactionTest::TearDown - complete");
}

TEST_F(TransactionTest, LayerMove) {
    ALOGD("TransactionTest::LayerMove");

    // The scope opens and closes a global transaction and, at the
    // same time, makes sure the SurfaceFlinger progresses one frame
    // after the transaction closes. The results of the transaction
    // should be available in the latest frame stored by the fake
    // composer.
    {
        TransactionScope ts(*sFakeComposer);
        ts.setPosition(mFGSurfaceControl, 128, 128);
        // NOTE: No changes yet, so vsync will do nothing, HWC does not get any calls.
        // (How to verify that? Throw in vsync and wait a 2x frame time? Separate test?)
        //
        // sFakeComposer->runVSyncAndWait();
    }

    fillSurfaceRGBA8(mFGSurfaceControl, GREEN);
    sFakeComposer->runVSyncAndWait();

    ASSERT_EQ(3, sFakeComposer->getFrameCount()); // Make sure the waits didn't time out and there's
                                                  // no extra frames.

    // NOTE: Frame 0 is produced in the SetUp.
    auto frame1Ref = mBaseFrame;
    frame1Ref[FG_LAYER].mDisplayFrame =
            hwc_rect_t{128, 128, 128 + 64, 128 + 64}; // Top-most layer moves.
    EXPECT_TRUE(framesAreSame(frame1Ref, sFakeComposer->getFrameRects(1)));

    auto frame2Ref = frame1Ref;
    frame2Ref[FG_LAYER].mSwapCount++;
    EXPECT_TRUE(framesAreSame(frame2Ref, sFakeComposer->getFrameRects(2)));
}

TEST_F(TransactionTest, LayerResize) {
    ALOGD("TransactionTest::LayerResize");
    {
        TransactionScope ts(*sFakeComposer);
        ts.setSize(mFGSurfaceControl, 128, 128);
    }

    fillSurfaceRGBA8(mFGSurfaceControl, GREEN);
    sFakeComposer->runVSyncAndWait();

    ASSERT_EQ(3, sFakeComposer->getFrameCount()); // Make sure the waits didn't time out and there's
                                                  // no extra frames.

    auto frame1Ref = mBaseFrame;
    // NOTE: The resize should not be visible for frame 1 as there's no buffer with new size posted.
    EXPECT_TRUE(framesAreSame(frame1Ref, sFakeComposer->getFrameRects(1)));

    auto frame2Ref = frame1Ref;
    frame2Ref[FG_LAYER].mSwapCount++;
    frame2Ref[FG_LAYER].mDisplayFrame = hwc_rect_t{64, 64, 64 + 128, 64 + 128};
    frame2Ref[FG_LAYER].mSourceCrop = hwc_frect_t{0.f, 0.f, 128.f, 128.f};
    EXPECT_TRUE(framesAreSame(frame2Ref, sFakeComposer->getFrameRects(2)));
}

TEST_F(TransactionTest, LayerCrop) {
    // TODO: Add scaling to confirm that crop happens in buffer space?
    {
        TransactionScope ts(*sFakeComposer);
        Rect cropRect(16, 16, 32, 32);
        ts.setCrop_legacy(mFGSurfaceControl, cropRect);
    }
    ASSERT_EQ(2, sFakeComposer->getFrameCount());

    auto referenceFrame = mBaseFrame;
    referenceFrame[FG_LAYER].mSourceCrop = hwc_frect_t{16.f, 16.f, 32.f, 32.f};
    referenceFrame[FG_LAYER].mDisplayFrame = hwc_rect_t{64 + 16, 64 + 16, 64 + 32, 64 + 32};
    EXPECT_TRUE(framesAreSame(referenceFrame, sFakeComposer->getLatestFrame()));
}

TEST_F(TransactionTest, LayerSetLayer) {
    {
        TransactionScope ts(*sFakeComposer);
        ts.setLayer(mFGSurfaceControl, INT_MAX - 3);
    }
    ASSERT_EQ(2, sFakeComposer->getFrameCount());

    // The layers will switch order, but both are rendered because the background layer is
    // transparent (RGBA8888).
    std::vector<RenderState> referenceFrame(2);
    referenceFrame[0] = mBaseFrame[FG_LAYER];
    referenceFrame[1] = mBaseFrame[BG_LAYER];
    EXPECT_TRUE(framesAreSame(referenceFrame, sFakeComposer->getLatestFrame()));
}

TEST_F(TransactionTest, LayerSetLayerOpaque) {
    {
        TransactionScope ts(*sFakeComposer);
        ts.setLayer(mFGSurfaceControl, INT_MAX - 3);
        ts.setFlags(mBGSurfaceControl, layer_state_t::eLayerOpaque,
                layer_state_t::eLayerOpaque);
    }
    ASSERT_EQ(2, sFakeComposer->getFrameCount());

    // The former foreground layer is now covered with opaque layer - it should have disappeared
    std::vector<RenderState> referenceFrame(1);
    referenceFrame[BG_LAYER] = mBaseFrame[BG_LAYER];
    EXPECT_TRUE(framesAreSame(referenceFrame, sFakeComposer->getLatestFrame()));
}

TEST_F(TransactionTest, SetLayerStack) {
    ALOGD("TransactionTest::SetLayerStack");
    {
        TransactionScope ts(*sFakeComposer);
        ts.setLayerStack(mFGSurfaceControl, 1);
    }

    // Foreground layer should have disappeared.
    ASSERT_EQ(2, sFakeComposer->getFrameCount());
    std::vector<RenderState> refFrame(1);
    refFrame[BG_LAYER] = mBaseFrame[BG_LAYER];
    EXPECT_TRUE(framesAreSame(refFrame, sFakeComposer->getLatestFrame()));
}

TEST_F(TransactionTest, LayerShowHide) {
    ALOGD("TransactionTest::LayerShowHide");
    {
        TransactionScope ts(*sFakeComposer);
        ts.hide(mFGSurfaceControl);
    }

    // Foreground layer should have disappeared.
    ASSERT_EQ(2, sFakeComposer->getFrameCount());
    std::vector<RenderState> refFrame(1);
    refFrame[BG_LAYER] = mBaseFrame[BG_LAYER];
    EXPECT_TRUE(framesAreSame(refFrame, sFakeComposer->getLatestFrame()));

    {
        TransactionScope ts(*sFakeComposer);
        ts.show(mFGSurfaceControl);
    }

    // Foreground layer should be back
    ASSERT_EQ(3, sFakeComposer->getFrameCount());
    EXPECT_TRUE(framesAreSame(mBaseFrame, sFakeComposer->getLatestFrame()));
}

TEST_F(TransactionTest, LayerSetAlpha) {
    {
        TransactionScope ts(*sFakeComposer);
        ts.setAlpha(mFGSurfaceControl, 0.75f);
    }

    ASSERT_EQ(2, sFakeComposer->getFrameCount());
    auto referenceFrame = mBaseFrame;
    referenceFrame[FG_LAYER].mPlaneAlpha = 0.75f;
    EXPECT_TRUE(framesAreSame(referenceFrame, sFakeComposer->getLatestFrame()));
}

TEST_F(TransactionTest, LayerSetFlags) {
    {
        TransactionScope ts(*sFakeComposer);
        ts.setFlags(mFGSurfaceControl, layer_state_t::eLayerHidden,
                layer_state_t::eLayerHidden);
    }

    // Foreground layer should have disappeared.
    ASSERT_EQ(2, sFakeComposer->getFrameCount());
    std::vector<RenderState> refFrame(1);
    refFrame[BG_LAYER] = mBaseFrame[BG_LAYER];
    EXPECT_TRUE(framesAreSame(refFrame, sFakeComposer->getLatestFrame()));
}

TEST_F(TransactionTest, LayerSetMatrix) {
    struct matrixTestData {
        float matrix[4];
        hwc_transform_t expectedTransform;
        hwc_rect_t expectedDisplayFrame;
    };

    // The matrix operates on the display frame and is applied before
    // the position is added. So, the foreground layer rect is (0, 0,
    // 64, 64) is first transformed, potentially yielding negative
    // coordinates and then the position (64, 64) is added yielding
    // the final on-screen rectangles given.

    const matrixTestData MATRIX_TESTS[7] = // clang-format off
            {{{-1.f, 0.f, 0.f, 1.f},    HWC_TRANSFORM_FLIP_H,           {0, 64, 64, 128}},
             {{1.f, 0.f, 0.f, -1.f},    HWC_TRANSFORM_FLIP_V,           {64, 0, 128, 64}},
             {{0.f, 1.f, -1.f, 0.f},    HWC_TRANSFORM_ROT_90,           {0, 64, 64, 128}},
             {{-1.f, 0.f, 0.f, -1.f},   HWC_TRANSFORM_ROT_180,          {0, 0, 64, 64}},
             {{0.f, -1.f, 1.f, 0.f},    HWC_TRANSFORM_ROT_270,          {64, 0, 128, 64}},
             {{0.f, 1.f, 1.f, 0.f},     HWC_TRANSFORM_FLIP_H_ROT_90,    {64, 64, 128, 128}},
             {{0.f, 1.f, 1.f, 0.f},     HWC_TRANSFORM_FLIP_V_ROT_90,    {64, 64, 128, 128}}};
    // clang-format on
    constexpr int TEST_COUNT = sizeof(MATRIX_TESTS) / sizeof(matrixTestData);

    for (int i = 0; i < TEST_COUNT; i++) {
        // TODO: How to leverage the HWC2 stringifiers?
        const matrixTestData& xform = MATRIX_TESTS[i];
        SCOPED_TRACE(i);
        {
            TransactionScope ts(*sFakeComposer);
            ts.setMatrix(mFGSurfaceControl, xform.matrix[0], xform.matrix[1],
                    xform.matrix[2], xform.matrix[3]);
        }

        auto referenceFrame = mBaseFrame;
        referenceFrame[FG_LAYER].mTransform = xform.expectedTransform;
        referenceFrame[FG_LAYER].mDisplayFrame = xform.expectedDisplayFrame;

        EXPECT_TRUE(framesAreSame(referenceFrame, sFakeComposer->getLatestFrame()));
    }
}

#if 0
TEST_F(TransactionTest, LayerSetMatrix2) {
    {
        TransactionScope ts(*sFakeComposer);
        // TODO: PLEASE SPEC THE FUNCTION!
        ts.setMatrix(mFGSurfaceControl, 0.11f, 0.123f,
                -2.33f, 0.22f);
    }
    auto referenceFrame = mBaseFrame;
    // TODO: Is this correct for sure?
    //referenceFrame[FG_LAYER].mTransform = HWC_TRANSFORM_FLIP_V & HWC_TRANSFORM_ROT_90;

    EXPECT_TRUE(framesAreSame(referenceFrame, sFakeComposer->getLatestFrame()));
}
#endif

TEST_F(TransactionTest, DeferredTransaction) {
    // Synchronization surface
    constexpr static int SYNC_LAYER = 2;
    auto syncSurfaceControl = mComposerClient->createSurface(String8("Sync Test Surface"), 1, 1,
                                                             PIXEL_FORMAT_RGBA_8888, 0);
    ASSERT_TRUE(syncSurfaceControl != nullptr);
    ASSERT_TRUE(syncSurfaceControl->isValid());

    fillSurfaceRGBA8(syncSurfaceControl, DARK_GRAY);

    {
        TransactionScope ts(*sFakeComposer);
        ts.setLayer(syncSurfaceControl, INT32_MAX - 1);
        ts.setPosition(syncSurfaceControl, mDisplayWidth - 2, mDisplayHeight - 2);
        ts.show(syncSurfaceControl);
    }
    auto referenceFrame = mBaseFrame;
    referenceFrame.push_back(makeSimpleRect(mDisplayWidth - 2, mDisplayHeight - 2,
                                            mDisplayWidth - 1, mDisplayHeight - 1));
    referenceFrame[SYNC_LAYER].mSwapCount = 1;
    EXPECT_EQ(2, sFakeComposer->getFrameCount());
    EXPECT_TRUE(framesAreSame(referenceFrame, sFakeComposer->getLatestFrame()));

    // set up two deferred transactions on different frames - these should not yield composited
    // frames
    {
        TransactionScope ts(*sFakeComposer);
        ts.setAlpha(mFGSurfaceControl, 0.75);
        ts.deferTransactionUntil_legacy(mFGSurfaceControl, syncSurfaceControl->getHandle(),
                                        syncSurfaceControl->getSurface()->getNextFrameNumber());
    }
    EXPECT_TRUE(framesAreSame(referenceFrame, sFakeComposer->getLatestFrame()));

    {
        TransactionScope ts(*sFakeComposer);
        ts.setPosition(mFGSurfaceControl, 128, 128);
        ts.deferTransactionUntil_legacy(mFGSurfaceControl, syncSurfaceControl->getHandle(),
                                        syncSurfaceControl->getSurface()->getNextFrameNumber() + 1);
    }
    EXPECT_EQ(4, sFakeComposer->getFrameCount());
    EXPECT_TRUE(framesAreSame(referenceFrame, sFakeComposer->getLatestFrame()));

    // should trigger the first deferred transaction, but not the second one
    fillSurfaceRGBA8(syncSurfaceControl, DARK_GRAY);
    sFakeComposer->runVSyncAndWait();
    EXPECT_EQ(5, sFakeComposer->getFrameCount());

    referenceFrame[FG_LAYER].mPlaneAlpha = 0.75f;
    referenceFrame[SYNC_LAYER].mSwapCount++;
    EXPECT_TRUE(framesAreSame(referenceFrame, sFakeComposer->getLatestFrame()));

    // should show up immediately since it's not deferred
    {
        TransactionScope ts(*sFakeComposer);
        ts.setAlpha(mFGSurfaceControl, 1.0);
    }
    referenceFrame[FG_LAYER].mPlaneAlpha = 1.f;
    EXPECT_EQ(6, sFakeComposer->getFrameCount());
    EXPECT_TRUE(framesAreSame(referenceFrame, sFakeComposer->getLatestFrame()));

    // trigger the second deferred transaction
    fillSurfaceRGBA8(syncSurfaceControl, DARK_GRAY);
    sFakeComposer->runVSyncAndWait();
    // TODO: Compute from layer size?
    referenceFrame[FG_LAYER].mDisplayFrame = hwc_rect_t{128, 128, 128 + 64, 128 + 64};
    referenceFrame[SYNC_LAYER].mSwapCount++;
    EXPECT_EQ(7, sFakeComposer->getFrameCount());
    EXPECT_TRUE(framesAreSame(referenceFrame, sFakeComposer->getLatestFrame()));
}

TEST_F(TransactionTest, SetRelativeLayer) {
    constexpr int RELATIVE_LAYER = 2;
    auto relativeSurfaceControl = mComposerClient->createSurface(String8("Test Surface"), 64, 64,
                                                                 PIXEL_FORMAT_RGBA_8888, 0);
    fillSurfaceRGBA8(relativeSurfaceControl, LIGHT_RED);

    // Now we stack the surface above the foreground surface and make sure it is visible.
    {
        TransactionScope ts(*sFakeComposer);
        ts.setPosition(relativeSurfaceControl, 64, 64);
        ts.show(relativeSurfaceControl);
        ts.setRelativeLayer(relativeSurfaceControl, mFGSurfaceControl->getHandle(), 1);
    }
    auto referenceFrame = mBaseFrame;
    // NOTE: All three layers will be visible as the surfaces are
    // transparent because of the RGBA format.
    referenceFrame.push_back(makeSimpleRect(64, 64, 64 + 64, 64 + 64));
    referenceFrame[RELATIVE_LAYER].mSwapCount = 1;
    EXPECT_TRUE(framesAreSame(referenceFrame, sFakeComposer->getLatestFrame()));

    // A call to setLayer will override a call to setRelativeLayer
    {
        TransactionScope ts(*sFakeComposer);
        ts.setLayer(relativeSurfaceControl, 0);
    }

    // Previous top layer will now appear at the bottom.
    auto referenceFrame2 = mBaseFrame;
    referenceFrame2.insert(referenceFrame2.begin(), referenceFrame[RELATIVE_LAYER]);
    EXPECT_EQ(3, sFakeComposer->getFrameCount());
    EXPECT_TRUE(framesAreSame(referenceFrame2, sFakeComposer->getLatestFrame()));
}

class ChildLayerTest : public TransactionTest {
protected:
    constexpr static int CHILD_LAYER = 2;

    void SetUp() override {
        TransactionTest::SetUp();
        mChild = mComposerClient->createSurface(String8("Child surface"), 10, 10,
                                                PIXEL_FORMAT_RGBA_8888, 0, mFGSurfaceControl.get());
        fillSurfaceRGBA8(mChild, LIGHT_GRAY);

        sFakeComposer->runVSyncAndWait();
        mBaseFrame.push_back(makeSimpleRect(64, 64, 64 + 10, 64 + 10));
        mBaseFrame[CHILD_LAYER].mSwapCount = 1;
        ASSERT_EQ(2, sFakeComposer->getFrameCount());
        ASSERT_TRUE(framesAreSame(mBaseFrame, sFakeComposer->getLatestFrame()));
    }
    void TearDown() override {
        mChild = 0;
        TransactionTest::TearDown();
    }

    sp<SurfaceControl> mChild;
};

TEST_F(ChildLayerTest, Positioning) {
    {
        TransactionScope ts(*sFakeComposer);
        ts.show(mChild);
        ts.setPosition(mChild, 10, 10);
        // Move to the same position as in the original setup.
        ts.setPosition(mFGSurfaceControl, 64, 64);
    }

    auto referenceFrame = mBaseFrame;
    referenceFrame[FG_LAYER].mDisplayFrame = hwc_rect_t{64, 64, 64 + 64, 64 + 64};
    referenceFrame[CHILD_LAYER].mDisplayFrame =
            hwc_rect_t{64 + 10, 64 + 10, 64 + 10 + 10, 64 + 10 + 10};
    EXPECT_TRUE(framesAreSame(referenceFrame, sFakeComposer->getLatestFrame()));

    {
        TransactionScope ts(*sFakeComposer);
        ts.setPosition(mFGSurfaceControl, 0, 0);
    }

    auto referenceFrame2 = mBaseFrame;
    referenceFrame2[FG_LAYER].mDisplayFrame = hwc_rect_t{0, 0, 0 + 64, 0 + 64};
    referenceFrame2[CHILD_LAYER].mDisplayFrame =
            hwc_rect_t{0 + 10, 0 + 10, 0 + 10 + 10, 0 + 10 + 10};
    EXPECT_TRUE(framesAreSame(referenceFrame2, sFakeComposer->getLatestFrame()));
}

TEST_F(ChildLayerTest, Cropping) {
    {
        TransactionScope ts(*sFakeComposer);
        ts.show(mChild);
        ts.setPosition(mChild, 0, 0);
        ts.setPosition(mFGSurfaceControl, 0, 0);
        ts.setCrop_legacy(mFGSurfaceControl, Rect(0, 0, 5, 5));
    }
    // NOTE: The foreground surface would be occluded by the child
    // now, but is included in the stack because the child is
    // transparent.
    auto referenceFrame = mBaseFrame;
    referenceFrame[FG_LAYER].mDisplayFrame = hwc_rect_t{0, 0, 0 + 5, 0 + 5};
    referenceFrame[FG_LAYER].mSourceCrop = hwc_frect_t{0.f, 0.f, 5.f, 5.f};
    referenceFrame[CHILD_LAYER].mDisplayFrame = hwc_rect_t{0, 0, 0 + 5, 0 + 5};
    referenceFrame[CHILD_LAYER].mSourceCrop = hwc_frect_t{0.f, 0.f, 5.f, 5.f};
    EXPECT_TRUE(framesAreSame(referenceFrame, sFakeComposer->getLatestFrame()));
}

TEST_F(ChildLayerTest, Constraints) {
    {
        TransactionScope ts(*sFakeComposer);
        ts.show(mChild);
        ts.setPosition(mFGSurfaceControl, 0, 0);
        ts.setPosition(mChild, 63, 63);
    }
    auto referenceFrame = mBaseFrame;
    referenceFrame[FG_LAYER].mDisplayFrame = hwc_rect_t{0, 0, 64, 64};
    referenceFrame[CHILD_LAYER].mDisplayFrame = hwc_rect_t{63, 63, 64, 64};
    referenceFrame[CHILD_LAYER].mSourceCrop = hwc_frect_t{0.f, 0.f, 1.f, 1.f};
    EXPECT_TRUE(framesAreSame(referenceFrame, sFakeComposer->getLatestFrame()));
}

TEST_F(ChildLayerTest, Scaling) {
    {
        TransactionScope ts(*sFakeComposer);
        ts.setPosition(mFGSurfaceControl, 0, 0);
    }
    auto referenceFrame = mBaseFrame;
    referenceFrame[FG_LAYER].mDisplayFrame = hwc_rect_t{0, 0, 64, 64};
    referenceFrame[CHILD_LAYER].mDisplayFrame = hwc_rect_t{0, 0, 10, 10};
    EXPECT_TRUE(framesAreSame(referenceFrame, sFakeComposer->getLatestFrame()));

    {
        TransactionScope ts(*sFakeComposer);
        ts.setMatrix(mFGSurfaceControl, 2.0, 0, 0, 2.0);
    }

    auto referenceFrame2 = mBaseFrame;
    referenceFrame2[FG_LAYER].mDisplayFrame = hwc_rect_t{0, 0, 128, 128};
    referenceFrame2[CHILD_LAYER].mDisplayFrame = hwc_rect_t{0, 0, 20, 20};
    EXPECT_TRUE(framesAreSame(referenceFrame2, sFakeComposer->getLatestFrame()));
}

TEST_F(ChildLayerTest, LayerAlpha) {
    {
        TransactionScope ts(*sFakeComposer);
        ts.show(mChild);
        ts.setPosition(mChild, 0, 0);
        ts.setPosition(mFGSurfaceControl, 0, 0);
        ts.setAlpha(mChild, 0.5);
    }

    auto referenceFrame = mBaseFrame;
    referenceFrame[FG_LAYER].mDisplayFrame = hwc_rect_t{0, 0, 64, 64};
    referenceFrame[CHILD_LAYER].mDisplayFrame = hwc_rect_t{0, 0, 10, 10};
    referenceFrame[CHILD_LAYER].mPlaneAlpha = 0.5f;
    EXPECT_TRUE(framesAreSame(referenceFrame, sFakeComposer->getLatestFrame()));

    {
        TransactionScope ts(*sFakeComposer);
        ts.setAlpha(mFGSurfaceControl, 0.5);
    }

    auto referenceFrame2 = referenceFrame;
    referenceFrame2[FG_LAYER].mPlaneAlpha = 0.5f;
    referenceFrame2[CHILD_LAYER].mPlaneAlpha = 0.25f;
    EXPECT_TRUE(framesAreSame(referenceFrame2, sFakeComposer->getLatestFrame()));
}

TEST_F(ChildLayerTest, ReparentChildren) {
    {
        TransactionScope ts(*sFakeComposer);
        ts.show(mChild);
        ts.setPosition(mChild, 10, 10);
        ts.setPosition(mFGSurfaceControl, 64, 64);
    }
    auto referenceFrame = mBaseFrame;
    referenceFrame[FG_LAYER].mDisplayFrame = hwc_rect_t{64, 64, 64 + 64, 64 + 64};
    referenceFrame[CHILD_LAYER].mDisplayFrame =
            hwc_rect_t{64 + 10, 64 + 10, 64 + 10 + 10, 64 + 10 + 10};
    EXPECT_TRUE(framesAreSame(referenceFrame, sFakeComposer->getLatestFrame()));

    {
        TransactionScope ts(*sFakeComposer);
        ts.reparentChildren(mFGSurfaceControl, mBGSurfaceControl->getHandle());
    }

    auto referenceFrame2 = referenceFrame;
    referenceFrame2[FG_LAYER].mDisplayFrame = hwc_rect_t{64, 64, 64 + 64, 64 + 64};
    referenceFrame2[CHILD_LAYER].mDisplayFrame = hwc_rect_t{10, 10, 10 + 10, 10 + 10};
    EXPECT_TRUE(framesAreSame(referenceFrame2, sFakeComposer->getLatestFrame()));
}

TEST_F(ChildLayerTest, DetachChildrenSameClient) {
    {
        TransactionScope ts(*sFakeComposer);
        ts.show(mChild);
        ts.setPosition(mChild, 10, 10);
        ts.setPosition(mFGSurfaceControl, 64, 64);
    }

    auto referenceFrame = mBaseFrame;
    referenceFrame[FG_LAYER].mDisplayFrame = hwc_rect_t{64, 64, 64 + 64, 64 + 64};
    referenceFrame[CHILD_LAYER].mDisplayFrame =
            hwc_rect_t{64 + 10, 64 + 10, 64 + 10 + 10, 64 + 10 + 10};
    EXPECT_TRUE(framesAreSame(referenceFrame, sFakeComposer->getLatestFrame()));

    {
        TransactionScope ts(*sFakeComposer);
        ts.setPosition(mFGSurfaceControl, 0, 0);
        ts.detachChildren(mFGSurfaceControl);
    }

    {
        TransactionScope ts(*sFakeComposer);
        ts.setPosition(mFGSurfaceControl, 64, 64);
        ts.hide(mChild);
    }

    std::vector<RenderState> refFrame(2);
    refFrame[BG_LAYER] = mBaseFrame[BG_LAYER];
    refFrame[FG_LAYER] = mBaseFrame[FG_LAYER];

    EXPECT_TRUE(framesAreSame(refFrame, sFakeComposer->getLatestFrame()));
}

TEST_F(ChildLayerTest, DetachChildrenDifferentClient) {
    sp<SurfaceComposerClient> newComposerClient = new SurfaceComposerClient;
    sp<SurfaceControl> childNewClient =
            newComposerClient->createSurface(String8("New Child Test Surface"), 10, 10,
                                             PIXEL_FORMAT_RGBA_8888, 0, mFGSurfaceControl.get());
    ASSERT_TRUE(childNewClient != nullptr);
    ASSERT_TRUE(childNewClient->isValid());
    fillSurfaceRGBA8(childNewClient, LIGHT_GRAY);

    {
        TransactionScope ts(*sFakeComposer);
        ts.hide(mChild);
        ts.show(childNewClient);
        ts.setPosition(childNewClient, 10, 10);
        ts.setPosition(mFGSurfaceControl, 64, 64);
    }

    auto referenceFrame = mBaseFrame;
    referenceFrame[FG_LAYER].mDisplayFrame = hwc_rect_t{64, 64, 64 + 64, 64 + 64};
    referenceFrame[CHILD_LAYER].mDisplayFrame =
            hwc_rect_t{64 + 10, 64 + 10, 64 + 10 + 10, 64 + 10 + 10};
    EXPECT_TRUE(framesAreSame(referenceFrame, sFakeComposer->getLatestFrame()));

    {
        TransactionScope ts(*sFakeComposer);
        ts.detachChildren(mFGSurfaceControl);
        ts.setPosition(mFGSurfaceControl, 0, 0);
    }

    {
        TransactionScope ts(*sFakeComposer);
        ts.setPosition(mFGSurfaceControl, 64, 64);
        ts.setPosition(childNewClient, 0, 0);
        ts.hide(childNewClient);
    }

    // Nothing should have changed. The child control becomes a no-op
    // zombie on detach. See comments for detachChildren in the
    // SurfaceControl.h file.
    EXPECT_TRUE(framesAreSame(referenceFrame, sFakeComposer->getLatestFrame()));
}

TEST_F(ChildLayerTest, InheritNonTransformScalingFromParent) {
    {
        TransactionScope ts(*sFakeComposer);
        ts.show(mChild);
        ts.setPosition(mChild, 0, 0);
        ts.setPosition(mFGSurfaceControl, 0, 0);
    }

    {
        TransactionScope ts(*sFakeComposer);
        ts.setOverrideScalingMode(mFGSurfaceControl, NATIVE_WINDOW_SCALING_MODE_SCALE_TO_WINDOW);
        // We cause scaling by 2.
        ts.setSize(mFGSurfaceControl, 128, 128);
    }

    auto referenceFrame = mBaseFrame;
    referenceFrame[FG_LAYER].mDisplayFrame = hwc_rect_t{0, 0, 128, 128};
    referenceFrame[FG_LAYER].mSourceCrop = hwc_frect_t{0.f, 0.f, 64.f, 64.f};
    referenceFrame[CHILD_LAYER].mDisplayFrame = hwc_rect_t{0, 0, 20, 20};
    referenceFrame[CHILD_LAYER].mSourceCrop = hwc_frect_t{0.f, 0.f, 10.f, 10.f};
    EXPECT_TRUE(framesAreSame(referenceFrame, sFakeComposer->getLatestFrame()));
}

// Regression test for b/37673612
TEST_F(ChildLayerTest, ChildrenWithParentBufferTransform) {
    {
        TransactionScope ts(*sFakeComposer);
        ts.show(mChild);
        ts.setPosition(mChild, 0, 0);
        ts.setPosition(mFGSurfaceControl, 0, 0);
    }

    // We set things up as in b/37673612 so that there is a mismatch between the buffer size and
    // the WM specified state size.
    {
        TransactionScope ts(*sFakeComposer);
        ts.setSize(mFGSurfaceControl, 128, 64);
    }

    sp<Surface> s = mFGSurfaceControl->getSurface();
    auto anw = static_cast<ANativeWindow*>(s.get());
    native_window_set_buffers_transform(anw, NATIVE_WINDOW_TRANSFORM_ROT_90);
    native_window_set_buffers_dimensions(anw, 64, 128);
    fillSurfaceRGBA8(mFGSurfaceControl, RED);
    sFakeComposer->runVSyncAndWait();

    // The child should still be in the same place and not have any strange scaling as in
    // b/37673612.
    auto referenceFrame = mBaseFrame;
    referenceFrame[FG_LAYER].mDisplayFrame = hwc_rect_t{0, 0, 128, 64};
    referenceFrame[FG_LAYER].mSourceCrop = hwc_frect_t{0.f, 0.f, 64.f, 128.f};
    referenceFrame[FG_LAYER].mSwapCount++;
    referenceFrame[CHILD_LAYER].mDisplayFrame = hwc_rect_t{0, 0, 10, 10};
    EXPECT_TRUE(framesAreSame(referenceFrame, sFakeComposer->getLatestFrame()));
}

TEST_F(ChildLayerTest, Bug36858924) {
    // Destroy the child layer
    mChild.clear();

    // Now recreate it as hidden
    mChild = mComposerClient->createSurface(String8("Child surface"), 10, 10,
                                            PIXEL_FORMAT_RGBA_8888, ISurfaceComposerClient::eHidden,
                                            mFGSurfaceControl.get());

    // Show the child layer in a deferred transaction
    {
        TransactionScope ts(*sFakeComposer);
        ts.deferTransactionUntil_legacy(mChild, mFGSurfaceControl->getHandle(),
                                        mFGSurfaceControl->getSurface()->getNextFrameNumber());
        ts.show(mChild);
    }

    // Render the foreground surface a few times
    //
    // Prior to the bugfix for b/36858924, this would usually hang while trying to fill the third
    // frame because SurfaceFlinger would never process the deferred transaction and would therefore
    // never acquire/release the first buffer
    ALOGI("Filling 1");
    fillSurfaceRGBA8(mFGSurfaceControl, GREEN);
    sFakeComposer->runVSyncAndWait();
    ALOGI("Filling 2");
    fillSurfaceRGBA8(mFGSurfaceControl, BLUE);
    sFakeComposer->runVSyncAndWait();
    ALOGI("Filling 3");
    fillSurfaceRGBA8(mFGSurfaceControl, RED);
    sFakeComposer->runVSyncAndWait();
    ALOGI("Filling 4");
    fillSurfaceRGBA8(mFGSurfaceControl, GREEN);
    sFakeComposer->runVSyncAndWait();
}

class ChildColorLayerTest : public ChildLayerTest {
protected:
    void SetUp() override {
        TransactionTest::SetUp();
        mChild = mComposerClient->createSurface(String8("Child surface"), 0, 0,
                                                PIXEL_FORMAT_RGBA_8888,
                                                ISurfaceComposerClient::eFXSurfaceColor,
                                                mFGSurfaceControl.get());
        {
            TransactionScope ts(*sFakeComposer);
            ts.setColor(mChild,
                        {LIGHT_GRAY.r / 255.0f, LIGHT_GRAY.g / 255.0f, LIGHT_GRAY.b / 255.0f});
            ts.setCrop_legacy(mChild, Rect(0, 0, 10, 10));
        }

        sFakeComposer->runVSyncAndWait();
        mBaseFrame.push_back(makeSimpleRect(64, 64, 64 + 10, 64 + 10));
        mBaseFrame[CHILD_LAYER].mSourceCrop = hwc_frect_t{0.0f, 0.0f, 0.0f, 0.0f};
        mBaseFrame[CHILD_LAYER].mSwapCount = 0;
        ASSERT_EQ(2, sFakeComposer->getFrameCount());
        ASSERT_TRUE(framesAreSame(mBaseFrame, sFakeComposer->getLatestFrame()));
    }
};

TEST_F(ChildColorLayerTest, LayerAlpha) {
    {
        TransactionScope ts(*sFakeComposer);
        ts.show(mChild);
        ts.setPosition(mChild, 0, 0);
        ts.setPosition(mFGSurfaceControl, 0, 0);
        ts.setAlpha(mChild, 0.5);
    }

    auto referenceFrame = mBaseFrame;
    referenceFrame[FG_LAYER].mDisplayFrame = hwc_rect_t{0, 0, 64, 64};
    referenceFrame[CHILD_LAYER].mDisplayFrame = hwc_rect_t{0, 0, 10, 10};
    referenceFrame[CHILD_LAYER].mPlaneAlpha = 0.5f;
    EXPECT_TRUE(framesAreSame(referenceFrame, sFakeComposer->getLatestFrame()));

    {
        TransactionScope ts(*sFakeComposer);
        ts.setAlpha(mFGSurfaceControl, 0.5);
    }

    auto referenceFrame2 = referenceFrame;
    referenceFrame2[FG_LAYER].mPlaneAlpha = 0.5f;
    referenceFrame2[CHILD_LAYER].mPlaneAlpha = 0.25f;
    EXPECT_TRUE(framesAreSame(referenceFrame2, sFakeComposer->getLatestFrame()));
}

TEST_F(ChildColorLayerTest, LayerZeroAlpha) {
    {
        TransactionScope ts(*sFakeComposer);
        ts.show(mChild);
        ts.setPosition(mChild, 0, 0);
        ts.setPosition(mFGSurfaceControl, 0, 0);
        ts.setAlpha(mChild, 0.5);
    }

    auto referenceFrame = mBaseFrame;
    referenceFrame[FG_LAYER].mDisplayFrame = hwc_rect_t{0, 0, 64, 64};
    referenceFrame[CHILD_LAYER].mDisplayFrame = hwc_rect_t{0, 0, 10, 10};
    referenceFrame[CHILD_LAYER].mPlaneAlpha = 0.5f;
    EXPECT_TRUE(framesAreSame(referenceFrame, sFakeComposer->getLatestFrame()));

    {
        TransactionScope ts(*sFakeComposer);
        ts.setAlpha(mFGSurfaceControl, 0.0f);
    }

    std::vector<RenderState> refFrame(1);
    refFrame[BG_LAYER] = mBaseFrame[BG_LAYER];

    EXPECT_TRUE(framesAreSame(refFrame, sFakeComposer->getLatestFrame()));
}

class LatchingTest : public TransactionTest {
protected:
    void lockAndFillFGBuffer() { fillSurfaceRGBA8(mFGSurfaceControl, RED, false); }

    void unlockFGBuffer() {
        sp<Surface> s = mFGSurfaceControl->getSurface();
        ASSERT_EQ(NO_ERROR, s->unlockAndPost());
        sFakeComposer->runVSyncAndWait();
    }

    void completeFGResize() {
        fillSurfaceRGBA8(mFGSurfaceControl, RED);
        sFakeComposer->runVSyncAndWait();
    }
    void restoreInitialState() {
        TransactionScope ts(*sFakeComposer);
        ts.setSize(mFGSurfaceControl, 64, 64);
        ts.setPosition(mFGSurfaceControl, 64, 64);
        ts.setCrop_legacy(mFGSurfaceControl, Rect(0, 0, 64, 64));
    }
};

TEST_F(LatchingTest, SurfacePositionLatching) {
    // By default position can be updated even while
    // a resize is pending.
    {
        TransactionScope ts(*sFakeComposer);
        ts.setSize(mFGSurfaceControl, 32, 32);
        ts.setPosition(mFGSurfaceControl, 100, 100);
    }

    // The size should not have updated as we have not provided a new buffer.
    auto referenceFrame1 = mBaseFrame;
    referenceFrame1[FG_LAYER].mDisplayFrame = hwc_rect_t{100, 100, 100 + 64, 100 + 64};
    EXPECT_TRUE(framesAreSame(referenceFrame1, sFakeComposer->getLatestFrame()));

    restoreInitialState();

    // Now we repeat with setGeometryAppliesWithResize
    // and verify the position DOESN'T latch.
    {
        TransactionScope ts(*sFakeComposer);
        ts.setGeometryAppliesWithResize(mFGSurfaceControl);
        ts.setSize(mFGSurfaceControl, 32, 32);
        ts.setPosition(mFGSurfaceControl, 100, 100);
    }
    EXPECT_TRUE(framesAreSame(mBaseFrame, sFakeComposer->getLatestFrame()));

    completeFGResize();

    auto referenceFrame2 = mBaseFrame;
    referenceFrame2[FG_LAYER].mDisplayFrame = hwc_rect_t{100, 100, 100 + 32, 100 + 32};
    referenceFrame2[FG_LAYER].mSourceCrop = hwc_frect_t{0.f, 0.f, 32.f, 32.f};
    referenceFrame2[FG_LAYER].mSwapCount++;
    EXPECT_TRUE(framesAreSame(referenceFrame2, sFakeComposer->getLatestFrame()));
}

TEST_F(LatchingTest, CropLatching) {
    // Normally the crop applies immediately even while a resize is pending.
    {
        TransactionScope ts(*sFakeComposer);
        ts.setSize(mFGSurfaceControl, 128, 128);
        ts.setCrop_legacy(mFGSurfaceControl, Rect(0, 0, 63, 63));
    }

    auto referenceFrame1 = mBaseFrame;
    referenceFrame1[FG_LAYER].mDisplayFrame = hwc_rect_t{64, 64, 64 + 63, 64 + 63};
    referenceFrame1[FG_LAYER].mSourceCrop = hwc_frect_t{0.f, 0.f, 63.f, 63.f};
    EXPECT_TRUE(framesAreSame(referenceFrame1, sFakeComposer->getLatestFrame()));

    restoreInitialState();

    {
        TransactionScope ts(*sFakeComposer);
        ts.setSize(mFGSurfaceControl, 128, 128);
        ts.setGeometryAppliesWithResize(mFGSurfaceControl);
        ts.setCrop_legacy(mFGSurfaceControl, Rect(0, 0, 63, 63));
    }
    EXPECT_TRUE(framesAreSame(mBaseFrame, sFakeComposer->getLatestFrame()));

    completeFGResize();

    auto referenceFrame2 = mBaseFrame;
    referenceFrame2[FG_LAYER].mDisplayFrame = hwc_rect_t{64, 64, 64 + 63, 64 + 63};
    referenceFrame2[FG_LAYER].mSourceCrop = hwc_frect_t{0.f, 0.f, 63.f, 63.f};
    referenceFrame2[FG_LAYER].mSwapCount++;
    EXPECT_TRUE(framesAreSame(referenceFrame2, sFakeComposer->getLatestFrame()));
}

} // namespace

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    sftest::FakeHwcEnvironment* fakeEnvironment = new sftest::FakeHwcEnvironment;
    ::testing::AddGlobalTestEnvironment(fakeEnvironment);
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
