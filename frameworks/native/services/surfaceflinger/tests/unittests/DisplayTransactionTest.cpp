/*
 * Copyright (C) 2018 The Android Open Source Project
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

#undef LOG_TAG
#define LOG_TAG "LibSurfaceFlingerUnittests"

#include <type_traits>

#include <compositionengine/Display.h>
#include <compositionengine/DisplayColorProfile.h>
#include <compositionengine/mock/DisplaySurface.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <log/log.h>
#include <renderengine/mock/RenderEngine.h>
#include <ui/DebugUtils.h>

#include "DisplayIdentificationTest.h"
#include "TestableScheduler.h"
#include "TestableSurfaceFlinger.h"
#include "mock/DisplayHardware/MockComposer.h"
#include "mock/MockDispSync.h"
#include "mock/MockEventControlThread.h"
#include "mock/MockEventThread.h"
#include "mock/MockMessageQueue.h"
#include "mock/MockNativeWindowSurface.h"
#include "mock/MockSurfaceInterceptor.h"
#include "mock/gui/MockGraphicBufferConsumer.h"
#include "mock/gui/MockGraphicBufferProducer.h"
#include "mock/system/window/MockNativeWindow.h"

namespace android {
namespace {

using testing::_;
using testing::DoAll;
using testing::Mock;
using testing::ResultOf;
using testing::Return;
using testing::SetArgPointee;

using android::Hwc2::ColorMode;
using android::Hwc2::Error;
using android::Hwc2::Hdr;
using android::Hwc2::IComposer;
using android::Hwc2::IComposerClient;
using android::Hwc2::PerFrameMetadataKey;
using android::Hwc2::RenderIntent;

using FakeDisplayDeviceInjector = TestableSurfaceFlinger::FakeDisplayDeviceInjector;
using FakeHwcDisplayInjector = TestableSurfaceFlinger::FakeHwcDisplayInjector;
using HotplugEvent = TestableSurfaceFlinger::HotplugEvent;
using HWC2Display = TestableSurfaceFlinger::HWC2Display;

constexpr int32_t DEFAULT_REFRESH_RATE = 16'666'666;
constexpr int32_t DEFAULT_DPI = 320;
constexpr int DEFAULT_VIRTUAL_DISPLAY_SURFACE_FORMAT = HAL_PIXEL_FORMAT_RGB_565;

constexpr int HWC_POWER_MODE_LEET = 1337; // An out of range power mode value

/* ------------------------------------------------------------------------
 * Boolean avoidance
 *
 * To make calls and template instantiations more readable, we define some
 * local enums along with an implicit bool conversion.
 */

#define BOOL_SUBSTITUTE(TYPENAME) enum class TYPENAME : bool { FALSE = false, TRUE = true };

BOOL_SUBSTITUTE(Async);
BOOL_SUBSTITUTE(Critical);
BOOL_SUBSTITUTE(Primary);
BOOL_SUBSTITUTE(Secure);
BOOL_SUBSTITUTE(Virtual);

/* ------------------------------------------------------------------------
 *
 */

class DisplayTransactionTest : public testing::Test {
public:
    DisplayTransactionTest();
    ~DisplayTransactionTest() override;

    void setupScheduler();

    // --------------------------------------------------------------------
    // Mock/Fake injection

    void injectMockComposer(int virtualDisplayCount);
    void injectFakeBufferQueueFactory();
    void injectFakeNativeWindowSurfaceFactory();

    // --------------------------------------------------------------------
    // Postcondition helpers

    bool hasPhysicalHwcDisplay(hwc2_display_t hwcDisplayId);
    bool hasTransactionFlagSet(int flag);
    bool hasDisplayDevice(sp<IBinder> displayToken);
    sp<DisplayDevice> getDisplayDevice(sp<IBinder> displayToken);
    bool hasCurrentDisplayState(sp<IBinder> displayToken);
    const DisplayDeviceState& getCurrentDisplayState(sp<IBinder> displayToken);
    bool hasDrawingDisplayState(sp<IBinder> displayToken);
    const DisplayDeviceState& getDrawingDisplayState(sp<IBinder> displayToken);

    // --------------------------------------------------------------------
    // Test instances

    TestableScheduler* mScheduler;
    TestableSurfaceFlinger mFlinger;
    mock::EventThread* mEventThread = new mock::EventThread();
    mock::EventThread* mSFEventThread = new mock::EventThread();
    mock::EventControlThread* mEventControlThread = new mock::EventControlThread();
    sp<mock::NativeWindow> mNativeWindow = new mock::NativeWindow();
    sp<GraphicBuffer> mBuffer = new GraphicBuffer();

    // These mocks are created by the test, but are destroyed by SurfaceFlinger
    // by virtue of being stored into a std::unique_ptr. However we still need
    // to keep a reference to them for use in setting up call expectations.
    renderengine::mock::RenderEngine* mRenderEngine = new renderengine::mock::RenderEngine();
    Hwc2::mock::Composer* mComposer = nullptr;
    mock::MessageQueue* mMessageQueue = new mock::MessageQueue();
    mock::SurfaceInterceptor* mSurfaceInterceptor = new mock::SurfaceInterceptor();
    mock::DispSync* mPrimaryDispSync = new mock::DispSync();

    // These mocks are created only when expected to be created via a factory.
    sp<mock::GraphicBufferConsumer> mConsumer;
    sp<mock::GraphicBufferProducer> mProducer;
    surfaceflinger::mock::NativeWindowSurface* mNativeWindowSurface = nullptr;
};

DisplayTransactionTest::DisplayTransactionTest() {
    const ::testing::TestInfo* const test_info =
            ::testing::UnitTest::GetInstance()->current_test_info();
    ALOGD("**** Setting up for %s.%s\n", test_info->test_case_name(), test_info->name());

    // Default to no wide color display support configured
    mFlinger.mutableHasWideColorDisplay() = false;
    mFlinger.mutableUseColorManagement() = false;
    mFlinger.mutableDisplayColorSetting() = DisplayColorSetting::UNMANAGED;

    // Default to using HWC virtual displays
    mFlinger.mutableUseHwcVirtualDisplays() = true;

    mFlinger.setCreateBufferQueueFunction([](auto, auto, auto) {
        ADD_FAILURE() << "Unexpected request to create a buffer queue.";
    });

    mFlinger.setCreateNativeWindowSurface([](auto) {
        ADD_FAILURE() << "Unexpected request to create a native window surface.";
        return nullptr;
    });

    setupScheduler();
    mFlinger.mutableEventQueue().reset(mMessageQueue);
    mFlinger.setupRenderEngine(std::unique_ptr<renderengine::RenderEngine>(mRenderEngine));
    mFlinger.mutableInterceptor().reset(mSurfaceInterceptor);

    injectMockComposer(0);
}

DisplayTransactionTest::~DisplayTransactionTest() {
    const ::testing::TestInfo* const test_info =
            ::testing::UnitTest::GetInstance()->current_test_info();
    ALOGD("**** Tearing down after %s.%s\n", test_info->test_case_name(), test_info->name());
}

void DisplayTransactionTest::setupScheduler() {
    mScheduler = new TestableScheduler(mFlinger.mutableRefreshRateConfigs());
    mScheduler->mutableEventControlThread().reset(mEventControlThread);
    mScheduler->mutablePrimaryDispSync().reset(mPrimaryDispSync);
    EXPECT_CALL(*mEventThread, registerDisplayEventConnection(_));
    EXPECT_CALL(*mSFEventThread, registerDisplayEventConnection(_));

    sp<Scheduler::ConnectionHandle> sfConnectionHandle =
            mScheduler->addConnection(std::unique_ptr<EventThread>(mSFEventThread));
    mFlinger.mutableSfConnectionHandle() = std::move(sfConnectionHandle);
    sp<Scheduler::ConnectionHandle> appConnectionHandle =
            mScheduler->addConnection(std::unique_ptr<EventThread>(mEventThread));
    mFlinger.mutableAppConnectionHandle() = std::move(appConnectionHandle);
    mFlinger.mutableScheduler().reset(mScheduler);
}

void DisplayTransactionTest::injectMockComposer(int virtualDisplayCount) {
    mComposer = new Hwc2::mock::Composer();
    EXPECT_CALL(*mComposer, getCapabilities())
            .WillOnce(Return(std::vector<IComposer::Capability>()));
    EXPECT_CALL(*mComposer, getMaxVirtualDisplayCount()).WillOnce(Return(virtualDisplayCount));
    mFlinger.setupComposer(std::unique_ptr<Hwc2::Composer>(mComposer));

    Mock::VerifyAndClear(mComposer);
}

void DisplayTransactionTest::injectFakeBufferQueueFactory() {
    // This setup is only expected once per test.
    ASSERT_TRUE(mConsumer == nullptr && mProducer == nullptr);

    mConsumer = new mock::GraphicBufferConsumer();
    mProducer = new mock::GraphicBufferProducer();

    mFlinger.setCreateBufferQueueFunction([this](auto outProducer, auto outConsumer, bool) {
        *outProducer = mProducer;
        *outConsumer = mConsumer;
    });
}

void DisplayTransactionTest::injectFakeNativeWindowSurfaceFactory() {
    // This setup is only expected once per test.
    ASSERT_TRUE(mNativeWindowSurface == nullptr);

    mNativeWindowSurface = new surfaceflinger::mock::NativeWindowSurface();

    mFlinger.setCreateNativeWindowSurface([this](auto) {
        return std::unique_ptr<surfaceflinger::NativeWindowSurface>(mNativeWindowSurface);
    });
}

bool DisplayTransactionTest::hasPhysicalHwcDisplay(hwc2_display_t hwcDisplayId) {
    return mFlinger.mutableHwcPhysicalDisplayIdMap().count(hwcDisplayId) == 1;
}

bool DisplayTransactionTest::hasTransactionFlagSet(int flag) {
    return mFlinger.mutableTransactionFlags() & flag;
}

bool DisplayTransactionTest::hasDisplayDevice(sp<IBinder> displayToken) {
    return mFlinger.mutableDisplays().count(displayToken) == 1;
}

sp<DisplayDevice> DisplayTransactionTest::getDisplayDevice(sp<IBinder> displayToken) {
    return mFlinger.mutableDisplays()[displayToken];
}

bool DisplayTransactionTest::hasCurrentDisplayState(sp<IBinder> displayToken) {
    return mFlinger.mutableCurrentState().displays.indexOfKey(displayToken) >= 0;
}

const DisplayDeviceState& DisplayTransactionTest::getCurrentDisplayState(sp<IBinder> displayToken) {
    return mFlinger.mutableCurrentState().displays.valueFor(displayToken);
}

bool DisplayTransactionTest::hasDrawingDisplayState(sp<IBinder> displayToken) {
    return mFlinger.mutableDrawingState().displays.indexOfKey(displayToken) >= 0;
}

const DisplayDeviceState& DisplayTransactionTest::getDrawingDisplayState(sp<IBinder> displayToken) {
    return mFlinger.mutableDrawingState().displays.valueFor(displayToken);
}

/* ------------------------------------------------------------------------
 *
 */

template <typename PhysicalDisplay>
struct PhysicalDisplayId {};

template <DisplayId::Type displayId>
using VirtualDisplayId = std::integral_constant<DisplayId::Type, displayId>;

struct NoDisplayId {};

template <typename>
struct IsPhysicalDisplayId : std::bool_constant<false> {};

template <typename PhysicalDisplay>
struct IsPhysicalDisplayId<PhysicalDisplayId<PhysicalDisplay>> : std::bool_constant<true> {};

template <typename>
struct DisplayIdGetter;

template <typename PhysicalDisplay>
struct DisplayIdGetter<PhysicalDisplayId<PhysicalDisplay>> {
    static std::optional<DisplayId> get() {
        if (!PhysicalDisplay::HAS_IDENTIFICATION_DATA) {
            return getFallbackDisplayId(static_cast<bool>(PhysicalDisplay::PRIMARY)
                                                ? HWC_DISPLAY_PRIMARY
                                                : HWC_DISPLAY_EXTERNAL);
        }

        const auto info =
                parseDisplayIdentificationData(PhysicalDisplay::PORT,
                                               PhysicalDisplay::GET_IDENTIFICATION_DATA());
        return info ? std::make_optional(info->id) : std::nullopt;
    }
};

template <DisplayId::Type displayId>
struct DisplayIdGetter<VirtualDisplayId<displayId>> {
    static std::optional<DisplayId> get() { return DisplayId{displayId}; }
};

template <>
struct DisplayIdGetter<NoDisplayId> {
    static std::optional<DisplayId> get() { return {}; }
};

// DisplayIdType can be:
//     1) PhysicalDisplayId<...> for generated ID of physical display backed by HWC.
//     2) VirtualDisplayId<...> for hard-coded ID of virtual display backed by HWC.
//     3) NoDisplayId for virtual display without HWC backing.
template <typename DisplayIdType, int width, int height, Critical critical, Async async,
          Secure secure, Primary primary, int grallocUsage>
struct DisplayVariant {
    using DISPLAY_ID = DisplayIdGetter<DisplayIdType>;

    // The display width and height
    static constexpr int WIDTH = width;
    static constexpr int HEIGHT = height;

    static constexpr int GRALLOC_USAGE = grallocUsage;

    // Whether the display is virtual or physical
    static constexpr Virtual VIRTUAL =
            IsPhysicalDisplayId<DisplayIdType>{} ? Virtual::FALSE : Virtual::TRUE;

    // When creating native window surfaces for the framebuffer, whether those should be critical
    static constexpr Critical CRITICAL = critical;

    // When creating native window surfaces for the framebuffer, whether those should be async
    static constexpr Async ASYNC = async;

    // Whether the display should be treated as secure
    static constexpr Secure SECURE = secure;

    // Whether the display is primary
    static constexpr Primary PRIMARY = primary;

    static auto makeFakeExistingDisplayInjector(DisplayTransactionTest* test) {
        auto injector =
                FakeDisplayDeviceInjector(test->mFlinger, DISPLAY_ID::get(),
                                          static_cast<bool>(VIRTUAL), static_cast<bool>(PRIMARY));

        injector.setSecure(static_cast<bool>(SECURE));
        injector.setNativeWindow(test->mNativeWindow);

        // Creating a DisplayDevice requires getting default dimensions from the
        // native window along with some other initial setup.
        EXPECT_CALL(*test->mNativeWindow, query(NATIVE_WINDOW_WIDTH, _))
                .WillRepeatedly(DoAll(SetArgPointee<1>(WIDTH), Return(0)));
        EXPECT_CALL(*test->mNativeWindow, query(NATIVE_WINDOW_HEIGHT, _))
                .WillRepeatedly(DoAll(SetArgPointee<1>(HEIGHT), Return(0)));
        EXPECT_CALL(*test->mNativeWindow, perform(NATIVE_WINDOW_SET_BUFFERS_FORMAT))
                .WillRepeatedly(Return(0));
        EXPECT_CALL(*test->mNativeWindow, perform(NATIVE_WINDOW_API_CONNECT))
                .WillRepeatedly(Return(0));
        EXPECT_CALL(*test->mNativeWindow, perform(NATIVE_WINDOW_SET_USAGE64))
                .WillRepeatedly(Return(0));
        EXPECT_CALL(*test->mNativeWindow, perform(NATIVE_WINDOW_API_DISCONNECT))
                .WillRepeatedly(Return(0));

        return injector;
    }

    // Called by tests to set up any native window creation call expectations.
    static void setupNativeWindowSurfaceCreationCallExpectations(DisplayTransactionTest* test) {
        EXPECT_CALL(*test->mNativeWindowSurface, getNativeWindow())
                .WillOnce(Return(test->mNativeWindow));

        EXPECT_CALL(*test->mNativeWindow, query(NATIVE_WINDOW_WIDTH, _))
                .WillRepeatedly(DoAll(SetArgPointee<1>(WIDTH), Return(0)));
        EXPECT_CALL(*test->mNativeWindow, query(NATIVE_WINDOW_HEIGHT, _))
                .WillRepeatedly(DoAll(SetArgPointee<1>(HEIGHT), Return(0)));
        EXPECT_CALL(*test->mNativeWindow, perform(NATIVE_WINDOW_SET_BUFFERS_FORMAT))
                .WillRepeatedly(Return(0));
        EXPECT_CALL(*test->mNativeWindow, perform(NATIVE_WINDOW_API_CONNECT))
                .WillRepeatedly(Return(0));
        EXPECT_CALL(*test->mNativeWindow, perform(NATIVE_WINDOW_SET_USAGE64))
                .WillRepeatedly(Return(0));
        EXPECT_CALL(*test->mNativeWindow, perform(NATIVE_WINDOW_API_DISCONNECT))
                .WillRepeatedly(Return(0));
    }

    static void setupFramebufferConsumerBufferQueueCallExpectations(DisplayTransactionTest* test) {
        EXPECT_CALL(*test->mConsumer, consumerConnect(_, false)).WillOnce(Return(NO_ERROR));
        EXPECT_CALL(*test->mConsumer, setConsumerName(_)).WillRepeatedly(Return(NO_ERROR));
        EXPECT_CALL(*test->mConsumer, setConsumerUsageBits(GRALLOC_USAGE))
                .WillRepeatedly(Return(NO_ERROR));
        EXPECT_CALL(*test->mConsumer, setDefaultBufferSize(WIDTH, HEIGHT))
                .WillRepeatedly(Return(NO_ERROR));
        EXPECT_CALL(*test->mConsumer, setMaxAcquiredBufferCount(_))
                .WillRepeatedly(Return(NO_ERROR));
    }

    static void setupFramebufferProducerBufferQueueCallExpectations(DisplayTransactionTest* test) {
        EXPECT_CALL(*test->mProducer, allocateBuffers(0, 0, 0, 0)).WillRepeatedly(Return());
    }
};

template <hwc2_display_t hwcDisplayId, HWC2::DisplayType hwcDisplayType, typename DisplayVariant,
          typename PhysicalDisplay = void>
struct HwcDisplayVariant {
    // The display id supplied by the HWC
    static constexpr hwc2_display_t HWC_DISPLAY_ID = hwcDisplayId;

    // The HWC display type
    static constexpr HWC2::DisplayType HWC_DISPLAY_TYPE = hwcDisplayType;

    // The HWC active configuration id
    static constexpr int HWC_ACTIVE_CONFIG_ID = 2001;
    static constexpr int INIT_POWER_MODE = HWC_POWER_MODE_NORMAL;

    static void injectPendingHotplugEvent(DisplayTransactionTest* test,
                                          HWC2::Connection connection) {
        test->mFlinger.mutablePendingHotplugEvents().emplace_back(
                HotplugEvent{HWC_DISPLAY_ID, connection});
    }

    // Called by tests to inject a HWC display setup
    static void injectHwcDisplayWithNoDefaultCapabilities(DisplayTransactionTest* test) {
        const auto displayId = DisplayVariant::DISPLAY_ID::get();
        ASSERT_TRUE(displayId);
        FakeHwcDisplayInjector(*displayId, HWC_DISPLAY_TYPE,
                               static_cast<bool>(DisplayVariant::PRIMARY))
                .setHwcDisplayId(HWC_DISPLAY_ID)
                .setWidth(DisplayVariant::WIDTH)
                .setHeight(DisplayVariant::HEIGHT)
                .setActiveConfig(HWC_ACTIVE_CONFIG_ID)
                .setPowerMode(INIT_POWER_MODE)
                .inject(&test->mFlinger, test->mComposer);
    }

    // Called by tests to inject a HWC display setup
    static void injectHwcDisplay(DisplayTransactionTest* test) {
        EXPECT_CALL(*test->mComposer, getDisplayCapabilities(HWC_DISPLAY_ID, _))
                .WillOnce(DoAll(SetArgPointee<1>(std::vector<Hwc2::DisplayCapability>({})),
                                Return(Error::NONE)));
        EXPECT_CALL(*test->mComposer,
                    setPowerMode(HWC_DISPLAY_ID,
                                 static_cast<Hwc2::IComposerClient::PowerMode>(INIT_POWER_MODE)))
                .WillOnce(Return(Error::NONE));
        injectHwcDisplayWithNoDefaultCapabilities(test);
    }

    static void setupHwcHotplugCallExpectations(DisplayTransactionTest* test) {
        EXPECT_CALL(*test->mComposer, getDisplayType(HWC_DISPLAY_ID, _))
                .WillOnce(DoAll(SetArgPointee<1>(static_cast<IComposerClient::DisplayType>(
                                        HWC_DISPLAY_TYPE)),
                                Return(Error::NONE)));
        EXPECT_CALL(*test->mComposer, setClientTargetSlotCount(_)).WillOnce(Return(Error::NONE));
        EXPECT_CALL(*test->mComposer, getDisplayConfigs(HWC_DISPLAY_ID, _))
                .WillOnce(DoAll(SetArgPointee<1>(std::vector<unsigned>{HWC_ACTIVE_CONFIG_ID}),
                                Return(Error::NONE)));
        EXPECT_CALL(*test->mComposer,
                    getDisplayAttribute(HWC_DISPLAY_ID, HWC_ACTIVE_CONFIG_ID,
                                        IComposerClient::Attribute::WIDTH, _))
                .WillOnce(DoAll(SetArgPointee<3>(DisplayVariant::WIDTH), Return(Error::NONE)));
        EXPECT_CALL(*test->mComposer,
                    getDisplayAttribute(HWC_DISPLAY_ID, HWC_ACTIVE_CONFIG_ID,
                                        IComposerClient::Attribute::HEIGHT, _))
                .WillOnce(DoAll(SetArgPointee<3>(DisplayVariant::HEIGHT), Return(Error::NONE)));
        EXPECT_CALL(*test->mComposer,
                    getDisplayAttribute(HWC_DISPLAY_ID, HWC_ACTIVE_CONFIG_ID,
                                        IComposerClient::Attribute::VSYNC_PERIOD, _))
                .WillOnce(DoAll(SetArgPointee<3>(DEFAULT_REFRESH_RATE), Return(Error::NONE)));
        EXPECT_CALL(*test->mComposer,
                    getDisplayAttribute(HWC_DISPLAY_ID, HWC_ACTIVE_CONFIG_ID,
                                        IComposerClient::Attribute::DPI_X, _))
                .WillOnce(DoAll(SetArgPointee<3>(DEFAULT_DPI), Return(Error::NONE)));
        EXPECT_CALL(*test->mComposer,
                    getDisplayAttribute(HWC_DISPLAY_ID, HWC_ACTIVE_CONFIG_ID,
                                        IComposerClient::Attribute::DPI_Y, _))
                .WillOnce(DoAll(SetArgPointee<3>(DEFAULT_DPI), Return(Error::NONE)));

        if (PhysicalDisplay::HAS_IDENTIFICATION_DATA) {
            EXPECT_CALL(*test->mComposer, getDisplayIdentificationData(HWC_DISPLAY_ID, _, _))
                    .WillOnce(DoAll(SetArgPointee<1>(PhysicalDisplay::PORT),
                                    SetArgPointee<2>(PhysicalDisplay::GET_IDENTIFICATION_DATA()),
                                    Return(Error::NONE)));
        } else {
            EXPECT_CALL(*test->mComposer, getDisplayIdentificationData(HWC_DISPLAY_ID, _, _))
                    .WillOnce(Return(Error::UNSUPPORTED));
        }
    }

    // Called by tests to set up HWC call expectations
    static void setupHwcGetActiveConfigCallExpectations(DisplayTransactionTest* test) {
        EXPECT_CALL(*test->mComposer, getActiveConfig(HWC_DISPLAY_ID, _))
                .WillRepeatedly(DoAll(SetArgPointee<1>(HWC_ACTIVE_CONFIG_ID), Return(Error::NONE)));
    }
};

// Physical displays are expected to be synchronous, secure, and have a HWC display for output.
constexpr uint32_t GRALLOC_USAGE_PHYSICAL_DISPLAY =
        GRALLOC_USAGE_HW_RENDER | GRALLOC_USAGE_HW_COMPOSER | GRALLOC_USAGE_HW_FB;

template <hwc2_display_t hwcDisplayId, typename PhysicalDisplay, int width, int height,
          Critical critical>
struct PhysicalDisplayVariant
      : DisplayVariant<PhysicalDisplayId<PhysicalDisplay>, width, height, critical, Async::FALSE,
                       Secure::TRUE, PhysicalDisplay::PRIMARY, GRALLOC_USAGE_PHYSICAL_DISPLAY>,
        HwcDisplayVariant<hwcDisplayId, HWC2::DisplayType::Physical,
                          DisplayVariant<PhysicalDisplayId<PhysicalDisplay>, width, height,
                                         critical, Async::FALSE, Secure::TRUE,
                                         PhysicalDisplay::PRIMARY, GRALLOC_USAGE_PHYSICAL_DISPLAY>,
                          PhysicalDisplay> {};

template <bool hasIdentificationData>
struct PrimaryDisplay {
    static constexpr Primary PRIMARY = Primary::TRUE;
    static constexpr uint8_t PORT = 255;
    static constexpr bool HAS_IDENTIFICATION_DATA = hasIdentificationData;
    static constexpr auto GET_IDENTIFICATION_DATA = getInternalEdid;
};

template <bool hasIdentificationData>
struct ExternalDisplay {
    static constexpr Primary PRIMARY = Primary::FALSE;
    static constexpr uint8_t PORT = 254;
    static constexpr bool HAS_IDENTIFICATION_DATA = hasIdentificationData;
    static constexpr auto GET_IDENTIFICATION_DATA = getExternalEdid;
};

struct TertiaryDisplay {
    static constexpr Primary PRIMARY = Primary::FALSE;
    static constexpr uint8_t PORT = 253;
    static constexpr auto GET_IDENTIFICATION_DATA = getExternalEdid;
};

// A primary display is a physical display that is critical
using PrimaryDisplayVariant =
        PhysicalDisplayVariant<1001, PrimaryDisplay<false>, 3840, 2160, Critical::TRUE>;

// An external display is physical display that is not critical.
using ExternalDisplayVariant =
        PhysicalDisplayVariant<1002, ExternalDisplay<false>, 1920, 1280, Critical::FALSE>;

using TertiaryDisplayVariant =
        PhysicalDisplayVariant<1003, TertiaryDisplay, 1600, 1200, Critical::FALSE>;

// A virtual display not supported by the HWC.
constexpr uint32_t GRALLOC_USAGE_NONHWC_VIRTUAL_DISPLAY = 0;

template <int width, int height, Secure secure>
struct NonHwcVirtualDisplayVariant
      : DisplayVariant<NoDisplayId, width, height, Critical::FALSE, Async::TRUE, secure,
                       Primary::FALSE, GRALLOC_USAGE_NONHWC_VIRTUAL_DISPLAY> {
    using Base = DisplayVariant<NoDisplayId, width, height, Critical::FALSE, Async::TRUE, secure,
                                Primary::FALSE, GRALLOC_USAGE_NONHWC_VIRTUAL_DISPLAY>;

    static void injectHwcDisplay(DisplayTransactionTest*) {}

    static void setupHwcGetActiveConfigCallExpectations(DisplayTransactionTest* test) {
        EXPECT_CALL(*test->mComposer, getActiveConfig(_, _)).Times(0);
    }

    static void setupNativeWindowSurfaceCreationCallExpectations(DisplayTransactionTest* test) {
        Base::setupNativeWindowSurfaceCreationCallExpectations(test);
        EXPECT_CALL(*test->mNativeWindow, setSwapInterval(0)).Times(1);
    }
};

// A virtual display supported by the HWC.
constexpr uint32_t GRALLOC_USAGE_HWC_VIRTUAL_DISPLAY = GRALLOC_USAGE_HW_COMPOSER;

template <int width, int height, Secure secure>
struct HwcVirtualDisplayVariant
      : DisplayVariant<VirtualDisplayId<42>, width, height, Critical::FALSE, Async::TRUE, secure,
                       Primary::FALSE, GRALLOC_USAGE_HWC_VIRTUAL_DISPLAY>,
        HwcDisplayVariant<
                1010, HWC2::DisplayType::Virtual,
                DisplayVariant<VirtualDisplayId<42>, width, height, Critical::FALSE, Async::TRUE,
                               secure, Primary::FALSE, GRALLOC_USAGE_HWC_VIRTUAL_DISPLAY>> {
    using Base = DisplayVariant<VirtualDisplayId<42>, width, height, Critical::FALSE, Async::TRUE,
                                secure, Primary::FALSE, GRALLOC_USAGE_HW_COMPOSER>;
    using Self = HwcVirtualDisplayVariant<width, height, secure>;

    static void setupNativeWindowSurfaceCreationCallExpectations(DisplayTransactionTest* test) {
        Base::setupNativeWindowSurfaceCreationCallExpectations(test);
        EXPECT_CALL(*test->mNativeWindow, setSwapInterval(0)).Times(1);
    }

    static void setupHwcVirtualDisplayCreationCallExpectations(DisplayTransactionTest* test) {
        EXPECT_CALL(*test->mComposer, createVirtualDisplay(Base::WIDTH, Base::HEIGHT, _, _))
                .WillOnce(DoAll(SetArgPointee<3>(Self::HWC_DISPLAY_ID), Return(Error::NONE)));
        EXPECT_CALL(*test->mComposer, setClientTargetSlotCount(_)).WillOnce(Return(Error::NONE));
    }
};

// For this variant, SurfaceFlinger should not configure itself with wide
// display support, so the display should not be configured for wide-color
// support.
struct WideColorSupportNotConfiguredVariant {
    static constexpr bool WIDE_COLOR_SUPPORTED = false;

    static void injectConfigChange(DisplayTransactionTest* test) {
        test->mFlinger.mutableHasWideColorDisplay() = false;
        test->mFlinger.mutableUseColorManagement() = false;
        test->mFlinger.mutableDisplayColorSetting() = DisplayColorSetting::UNMANAGED;
    }

    static void setupComposerCallExpectations(DisplayTransactionTest* test) {
        EXPECT_CALL(*test->mComposer, getColorModes(_, _)).Times(0);
        EXPECT_CALL(*test->mComposer, getRenderIntents(_, _, _)).Times(0);
        EXPECT_CALL(*test->mComposer, setColorMode(_, _, _)).Times(0);
    }
};

// For this variant, SurfaceFlinger should configure itself with wide display
// support, and the display should respond with an non-empty list of supported
// color modes. Wide-color support should be configured.
template <typename Display>
struct WideColorP3ColorimetricSupportedVariant {
    static constexpr bool WIDE_COLOR_SUPPORTED = true;

    static void injectConfigChange(DisplayTransactionTest* test) {
        test->mFlinger.mutableUseColorManagement() = true;
        test->mFlinger.mutableHasWideColorDisplay() = true;
        test->mFlinger.mutableDisplayColorSetting() = DisplayColorSetting::UNMANAGED;
    }

    static void setupComposerCallExpectations(DisplayTransactionTest* test) {
        EXPECT_CALL(*test->mNativeWindow, perform(NATIVE_WINDOW_SET_BUFFERS_DATASPACE)).Times(1);

        EXPECT_CALL(*test->mComposer, getColorModes(Display::HWC_DISPLAY_ID, _))
                .WillOnce(DoAll(SetArgPointee<1>(std::vector<ColorMode>({ColorMode::DISPLAY_P3})),
                                Return(Error::NONE)));
        EXPECT_CALL(*test->mComposer,
                    getRenderIntents(Display::HWC_DISPLAY_ID, ColorMode::DISPLAY_P3, _))
                .WillOnce(DoAll(SetArgPointee<2>(
                                        std::vector<RenderIntent>({RenderIntent::COLORIMETRIC})),
                                Return(Error::NONE)));
        EXPECT_CALL(*test->mComposer,
                    setColorMode(Display::HWC_DISPLAY_ID, ColorMode::SRGB,
                                 RenderIntent::COLORIMETRIC))
                .WillOnce(Return(Error::NONE));
    }
};

// For this variant, SurfaceFlinger should configure itself with wide display
// support, but the display should respond with an empty list of supported color
// modes. Wide-color support for the display should not be configured.
template <typename Display>
struct WideColorNotSupportedVariant {
    static constexpr bool WIDE_COLOR_SUPPORTED = false;

    static void injectConfigChange(DisplayTransactionTest* test) {
        test->mFlinger.mutableUseColorManagement() = true;
        test->mFlinger.mutableHasWideColorDisplay() = true;
    }

    static void setupComposerCallExpectations(DisplayTransactionTest* test) {
        EXPECT_CALL(*test->mComposer, getColorModes(Display::HWC_DISPLAY_ID, _))
                .WillOnce(DoAll(SetArgPointee<1>(std::vector<ColorMode>()), Return(Error::NONE)));
        EXPECT_CALL(*test->mComposer, setColorMode(_, _, _)).Times(0);
    }
};

// For this variant, the display is not a HWC display, so no HDR support should
// be configured.
struct NonHwcDisplayHdrSupportVariant {
    static constexpr bool HDR10_PLUS_SUPPORTED = false;
    static constexpr bool HDR10_SUPPORTED = false;
    static constexpr bool HDR_HLG_SUPPORTED = false;
    static constexpr bool HDR_DOLBY_VISION_SUPPORTED = false;
    static void setupComposerCallExpectations(DisplayTransactionTest* test) {
        EXPECT_CALL(*test->mComposer, getHdrCapabilities(_, _, _, _, _)).Times(0);
    }
};

template <typename Display>
struct Hdr10PlusSupportedVariant {
    static constexpr bool HDR10_PLUS_SUPPORTED = true;
    static constexpr bool HDR10_SUPPORTED = true;
    static constexpr bool HDR_HLG_SUPPORTED = false;
    static constexpr bool HDR_DOLBY_VISION_SUPPORTED = false;
    static void setupComposerCallExpectations(DisplayTransactionTest* test) {
        EXPECT_CALL(*test->mComposer, getHdrCapabilities(_, _, _, _, _))
                .WillOnce(DoAll(SetArgPointee<1>(std::vector<Hdr>({
                                        Hdr::HDR10_PLUS,
                                        Hdr::HDR10,
                                })),
                                Return(Error::NONE)));
    }
};

// For this variant, the composer should respond with a non-empty list of HDR
// modes containing HDR10, so HDR10 support should be configured.
template <typename Display>
struct Hdr10SupportedVariant {
    static constexpr bool HDR10_PLUS_SUPPORTED = false;
    static constexpr bool HDR10_SUPPORTED = true;
    static constexpr bool HDR_HLG_SUPPORTED = false;
    static constexpr bool HDR_DOLBY_VISION_SUPPORTED = false;
    static void setupComposerCallExpectations(DisplayTransactionTest* test) {
        EXPECT_CALL(*test->mComposer, getHdrCapabilities(Display::HWC_DISPLAY_ID, _, _, _, _))
                .WillOnce(DoAll(SetArgPointee<1>(std::vector<Hdr>({Hdr::HDR10})),
                                Return(Error::NONE)));
    }
};

// For this variant, the composer should respond with a non-empty list of HDR
// modes containing HLG, so HLG support should be configured.
template <typename Display>
struct HdrHlgSupportedVariant {
    static constexpr bool HDR10_PLUS_SUPPORTED = false;
    static constexpr bool HDR10_SUPPORTED = false;
    static constexpr bool HDR_HLG_SUPPORTED = true;
    static constexpr bool HDR_DOLBY_VISION_SUPPORTED = false;
    static void setupComposerCallExpectations(DisplayTransactionTest* test) {
        EXPECT_CALL(*test->mComposer, getHdrCapabilities(Display::HWC_DISPLAY_ID, _, _, _, _))
                .WillOnce(
                        DoAll(SetArgPointee<1>(std::vector<Hdr>({Hdr::HLG})), Return(Error::NONE)));
    }
};

// For this variant, the composer should respond with a non-empty list of HDR
// modes containing DOLBY_VISION, so DOLBY_VISION support should be configured.
template <typename Display>
struct HdrDolbyVisionSupportedVariant {
    static constexpr bool HDR10_PLUS_SUPPORTED = false;
    static constexpr bool HDR10_SUPPORTED = false;
    static constexpr bool HDR_HLG_SUPPORTED = false;
    static constexpr bool HDR_DOLBY_VISION_SUPPORTED = true;
    static void setupComposerCallExpectations(DisplayTransactionTest* test) {
        EXPECT_CALL(*test->mComposer, getHdrCapabilities(Display::HWC_DISPLAY_ID, _, _, _, _))
                .WillOnce(DoAll(SetArgPointee<1>(std::vector<Hdr>({Hdr::DOLBY_VISION})),
                                Return(Error::NONE)));
    }
};

// For this variant, the composer should respond with am empty list of HDR
// modes, so no HDR support should be configured.
template <typename Display>
struct HdrNotSupportedVariant {
    static constexpr bool HDR10_PLUS_SUPPORTED = false;
    static constexpr bool HDR10_SUPPORTED = false;
    static constexpr bool HDR_HLG_SUPPORTED = false;
    static constexpr bool HDR_DOLBY_VISION_SUPPORTED = false;
    static void setupComposerCallExpectations(DisplayTransactionTest* test) {
        EXPECT_CALL(*test->mComposer, getHdrCapabilities(Display::HWC_DISPLAY_ID, _, _, _, _))
                .WillOnce(DoAll(SetArgPointee<1>(std::vector<Hdr>()), Return(Error::NONE)));
    }
};

struct NonHwcPerFrameMetadataSupportVariant {
    static constexpr int PER_FRAME_METADATA_KEYS = 0;
    static void setupComposerCallExpectations(DisplayTransactionTest* test) {
        EXPECT_CALL(*test->mComposer, getPerFrameMetadataKeys(_)).Times(0);
    }
};

template <typename Display>
struct NoPerFrameMetadataSupportVariant {
    static constexpr int PER_FRAME_METADATA_KEYS = 0;
    static void setupComposerCallExpectations(DisplayTransactionTest* test) {
        EXPECT_CALL(*test->mComposer, getPerFrameMetadataKeys(Display::HWC_DISPLAY_ID))
                .WillOnce(Return(std::vector<PerFrameMetadataKey>()));
    }
};

template <typename Display>
struct Smpte2086PerFrameMetadataSupportVariant {
    static constexpr int PER_FRAME_METADATA_KEYS = HdrMetadata::Type::SMPTE2086;
    static void setupComposerCallExpectations(DisplayTransactionTest* test) {
        EXPECT_CALL(*test->mComposer, getPerFrameMetadataKeys(Display::HWC_DISPLAY_ID))
                .WillOnce(Return(std::vector<PerFrameMetadataKey>({
                        PerFrameMetadataKey::DISPLAY_RED_PRIMARY_X,
                        PerFrameMetadataKey::DISPLAY_RED_PRIMARY_Y,
                        PerFrameMetadataKey::DISPLAY_GREEN_PRIMARY_X,
                        PerFrameMetadataKey::DISPLAY_GREEN_PRIMARY_Y,
                        PerFrameMetadataKey::DISPLAY_BLUE_PRIMARY_X,
                        PerFrameMetadataKey::DISPLAY_BLUE_PRIMARY_Y,
                        PerFrameMetadataKey::WHITE_POINT_X,
                        PerFrameMetadataKey::WHITE_POINT_Y,
                        PerFrameMetadataKey::MAX_LUMINANCE,
                        PerFrameMetadataKey::MIN_LUMINANCE,
                })));
    }
};

template <typename Display>
struct Cta861_3_PerFrameMetadataSupportVariant {
    static constexpr int PER_FRAME_METADATA_KEYS = HdrMetadata::Type::CTA861_3;
    static void setupComposerCallExpectations(DisplayTransactionTest* test) {
        EXPECT_CALL(*test->mComposer, getPerFrameMetadataKeys(Display::HWC_DISPLAY_ID))
                .WillOnce(Return(std::vector<PerFrameMetadataKey>({
                        PerFrameMetadataKey::MAX_CONTENT_LIGHT_LEVEL,
                        PerFrameMetadataKey::MAX_FRAME_AVERAGE_LIGHT_LEVEL,
                })));
    }
};

template <typename Display>
struct Hdr10_Plus_PerFrameMetadataSupportVariant {
    static constexpr int PER_FRAME_METADATA_KEYS = HdrMetadata::Type::HDR10PLUS;
    static void setupComposerCallExpectations(DisplayTransactionTest* test) {
        EXPECT_CALL(*test->mComposer, getPerFrameMetadataKeys(Display::HWC_DISPLAY_ID))
                .WillOnce(Return(std::vector<PerFrameMetadataKey>({
                        PerFrameMetadataKey::HDR10_PLUS_SEI,
                })));
    }
};
/* ------------------------------------------------------------------------
 * Typical display configurations to test
 */

template <typename DisplayPolicy, typename WideColorSupportPolicy, typename HdrSupportPolicy,
          typename PerFrameMetadataSupportPolicy>
struct Case {
    using Display = DisplayPolicy;
    using WideColorSupport = WideColorSupportPolicy;
    using HdrSupport = HdrSupportPolicy;
    using PerFrameMetadataSupport = PerFrameMetadataSupportPolicy;
};

using SimplePrimaryDisplayCase =
        Case<PrimaryDisplayVariant, WideColorNotSupportedVariant<PrimaryDisplayVariant>,
             HdrNotSupportedVariant<PrimaryDisplayVariant>,
             NoPerFrameMetadataSupportVariant<PrimaryDisplayVariant>>;
using SimpleExternalDisplayCase =
        Case<ExternalDisplayVariant, WideColorNotSupportedVariant<ExternalDisplayVariant>,
             HdrNotSupportedVariant<ExternalDisplayVariant>,
             NoPerFrameMetadataSupportVariant<ExternalDisplayVariant>>;
using SimpleTertiaryDisplayCase =
        Case<TertiaryDisplayVariant, WideColorNotSupportedVariant<TertiaryDisplayVariant>,
             HdrNotSupportedVariant<TertiaryDisplayVariant>,
             NoPerFrameMetadataSupportVariant<TertiaryDisplayVariant>>;
using NonHwcVirtualDisplayCase =
        Case<NonHwcVirtualDisplayVariant<1024, 768, Secure::FALSE>,
             WideColorSupportNotConfiguredVariant, NonHwcDisplayHdrSupportVariant,
             NonHwcPerFrameMetadataSupportVariant>;
using SimpleHwcVirtualDisplayVariant = HwcVirtualDisplayVariant<1024, 768, Secure::TRUE>;
using HwcVirtualDisplayCase =
        Case<SimpleHwcVirtualDisplayVariant, WideColorSupportNotConfiguredVariant,
             HdrNotSupportedVariant<SimpleHwcVirtualDisplayVariant>,
             NoPerFrameMetadataSupportVariant<SimpleHwcVirtualDisplayVariant>>;
using WideColorP3ColorimetricDisplayCase =
        Case<PrimaryDisplayVariant, WideColorP3ColorimetricSupportedVariant<PrimaryDisplayVariant>,
             HdrNotSupportedVariant<PrimaryDisplayVariant>,
             NoPerFrameMetadataSupportVariant<PrimaryDisplayVariant>>;
using Hdr10PlusDisplayCase =
        Case<PrimaryDisplayVariant, WideColorNotSupportedVariant<PrimaryDisplayVariant>,
             Hdr10SupportedVariant<PrimaryDisplayVariant>,
             Hdr10_Plus_PerFrameMetadataSupportVariant<PrimaryDisplayVariant>>;
using Hdr10DisplayCase =
        Case<PrimaryDisplayVariant, WideColorNotSupportedVariant<PrimaryDisplayVariant>,
             Hdr10SupportedVariant<PrimaryDisplayVariant>,
             NoPerFrameMetadataSupportVariant<PrimaryDisplayVariant>>;
using HdrHlgDisplayCase =
        Case<PrimaryDisplayVariant, WideColorNotSupportedVariant<PrimaryDisplayVariant>,
             HdrHlgSupportedVariant<PrimaryDisplayVariant>,
             NoPerFrameMetadataSupportVariant<PrimaryDisplayVariant>>;
using HdrDolbyVisionDisplayCase =
        Case<PrimaryDisplayVariant, WideColorNotSupportedVariant<PrimaryDisplayVariant>,
             HdrDolbyVisionSupportedVariant<PrimaryDisplayVariant>,
             NoPerFrameMetadataSupportVariant<PrimaryDisplayVariant>>;
using HdrSmpte2086DisplayCase =
        Case<PrimaryDisplayVariant, WideColorNotSupportedVariant<PrimaryDisplayVariant>,
             HdrNotSupportedVariant<PrimaryDisplayVariant>,
             Smpte2086PerFrameMetadataSupportVariant<PrimaryDisplayVariant>>;
using HdrCta861_3_DisplayCase =
        Case<PrimaryDisplayVariant, WideColorNotSupportedVariant<PrimaryDisplayVariant>,
             HdrNotSupportedVariant<PrimaryDisplayVariant>,
             Cta861_3_PerFrameMetadataSupportVariant<PrimaryDisplayVariant>>;

/* ------------------------------------------------------------------------
 *
 * SurfaceFlinger::onHotplugReceived
 */

TEST_F(DisplayTransactionTest, hotplugEnqueuesEventsForDisplayTransaction) {
    constexpr int currentSequenceId = 123;
    constexpr hwc2_display_t hwcDisplayId1 = 456;
    constexpr hwc2_display_t hwcDisplayId2 = 654;

    // --------------------------------------------------------------------
    // Preconditions

    // Set the current sequence id for accepted events
    mFlinger.mutableComposerSequenceId() = currentSequenceId;

    // Set the main thread id so that the current thread does not appear to be
    // the main thread.
    mFlinger.mutableMainThreadId() = std::thread::id();

    // --------------------------------------------------------------------
    // Call Expectations

    // We expect invalidate() to be invoked once to trigger display transaction
    // processing.
    EXPECT_CALL(*mMessageQueue, invalidate()).Times(1);

    // --------------------------------------------------------------------
    // Invocation

    // Simulate two hotplug events (a connect and a disconnect)
    mFlinger.onHotplugReceived(currentSequenceId, hwcDisplayId1, HWC2::Connection::Connected);
    mFlinger.onHotplugReceived(currentSequenceId, hwcDisplayId2, HWC2::Connection::Disconnected);

    // --------------------------------------------------------------------
    // Postconditions

    // The display transaction needed flag should be set.
    EXPECT_TRUE(hasTransactionFlagSet(eDisplayTransactionNeeded));

    // All events should be in the pending event queue.
    const auto& pendingEvents = mFlinger.mutablePendingHotplugEvents();
    ASSERT_EQ(2u, pendingEvents.size());
    EXPECT_EQ(hwcDisplayId1, pendingEvents[0].hwcDisplayId);
    EXPECT_EQ(HWC2::Connection::Connected, pendingEvents[0].connection);
    EXPECT_EQ(hwcDisplayId2, pendingEvents[1].hwcDisplayId);
    EXPECT_EQ(HWC2::Connection::Disconnected, pendingEvents[1].connection);
}

TEST_F(DisplayTransactionTest, hotplugDiscardsUnexpectedEvents) {
    constexpr int currentSequenceId = 123;
    constexpr int otherSequenceId = 321;
    constexpr hwc2_display_t displayId = 456;

    // --------------------------------------------------------------------
    // Preconditions

    // Set the current sequence id for accepted events
    mFlinger.mutableComposerSequenceId() = currentSequenceId;

    // Set the main thread id so that the current thread does not appear to be
    // the main thread.
    mFlinger.mutableMainThreadId() = std::thread::id();

    // --------------------------------------------------------------------
    // Call Expectations

    // We do not expect any calls to invalidate().
    EXPECT_CALL(*mMessageQueue, invalidate()).Times(0);

    // --------------------------------------------------------------------
    // Invocation

    // Call with an unexpected sequence id
    mFlinger.onHotplugReceived(otherSequenceId, displayId, HWC2::Connection::Invalid);

    // --------------------------------------------------------------------
    // Postconditions

    // The display transaction needed flag should not be set
    EXPECT_FALSE(hasTransactionFlagSet(eDisplayTransactionNeeded));

    // There should be no pending events
    EXPECT_TRUE(mFlinger.mutablePendingHotplugEvents().empty());
}

TEST_F(DisplayTransactionTest, hotplugProcessesEnqueuedEventsIfCalledOnMainThread) {
    constexpr int currentSequenceId = 123;
    constexpr hwc2_display_t displayId1 = 456;

    // --------------------------------------------------------------------
    // Note:
    // --------------------------------------------------------------------
    // This test case is a bit tricky. We want to verify that
    // onHotplugReceived() calls processDisplayHotplugEventsLocked(), but we
    // don't really want to provide coverage for everything the later function
    // does as there are specific tests for it.
    // --------------------------------------------------------------------

    // --------------------------------------------------------------------
    // Preconditions

    // Set the current sequence id for accepted events
    mFlinger.mutableComposerSequenceId() = currentSequenceId;

    // Set the main thread id so that the current thread does appear to be the
    // main thread.
    mFlinger.mutableMainThreadId() = std::this_thread::get_id();

    // --------------------------------------------------------------------
    // Call Expectations

    // We expect invalidate() to be invoked once to trigger display transaction
    // processing.
    EXPECT_CALL(*mMessageQueue, invalidate()).Times(1);

    // --------------------------------------------------------------------
    // Invocation

    // Simulate a disconnect on a display id that is not connected. This should
    // be enqueued by onHotplugReceived(), and dequeued by
    // processDisplayHotplugEventsLocked(), but then ignored as invalid.
    mFlinger.onHotplugReceived(currentSequenceId, displayId1, HWC2::Connection::Disconnected);

    // --------------------------------------------------------------------
    // Postconditions

    // The display transaction needed flag should be set.
    EXPECT_TRUE(hasTransactionFlagSet(eDisplayTransactionNeeded));

    // There should be no event queued on return, as it should have been
    // processed.
    EXPECT_TRUE(mFlinger.mutablePendingHotplugEvents().empty());
}

/* ------------------------------------------------------------------------
 * SurfaceFlinger::createDisplay
 */

TEST_F(DisplayTransactionTest, createDisplaySetsCurrentStateForNonsecureDisplay) {
    const String8 name("virtual.test");

    // --------------------------------------------------------------------
    // Call Expectations

    // The call should notify the interceptor that a display was created.
    EXPECT_CALL(*mSurfaceInterceptor, saveDisplayCreation(_)).Times(1);

    // --------------------------------------------------------------------
    // Invocation

    sp<IBinder> displayToken = mFlinger.createDisplay(name, false);

    // --------------------------------------------------------------------
    // Postconditions

    // The display should have been added to the current state
    ASSERT_TRUE(hasCurrentDisplayState(displayToken));
    const auto& display = getCurrentDisplayState(displayToken);
    EXPECT_TRUE(display.isVirtual());
    EXPECT_FALSE(display.isSecure);
    EXPECT_EQ(name.string(), display.displayName);

    // --------------------------------------------------------------------
    // Cleanup conditions

    // Destroying the display invalidates the display state.
    EXPECT_CALL(*mMessageQueue, invalidate()).Times(1);
}

TEST_F(DisplayTransactionTest, createDisplaySetsCurrentStateForSecureDisplay) {
    const String8 name("virtual.test");

    // --------------------------------------------------------------------
    // Call Expectations

    // The call should notify the interceptor that a display was created.
    EXPECT_CALL(*mSurfaceInterceptor, saveDisplayCreation(_)).Times(1);

    // --------------------------------------------------------------------
    // Invocation

    sp<IBinder> displayToken = mFlinger.createDisplay(name, true);

    // --------------------------------------------------------------------
    // Postconditions

    // The display should have been added to the current state
    ASSERT_TRUE(hasCurrentDisplayState(displayToken));
    const auto& display = getCurrentDisplayState(displayToken);
    EXPECT_TRUE(display.isVirtual());
    EXPECT_TRUE(display.isSecure);
    EXPECT_EQ(name.string(), display.displayName);

    // --------------------------------------------------------------------
    // Cleanup conditions

    // Destroying the display invalidates the display state.
    EXPECT_CALL(*mMessageQueue, invalidate()).Times(1);
}

/* ------------------------------------------------------------------------
 * SurfaceFlinger::destroyDisplay
 */

TEST_F(DisplayTransactionTest, destroyDisplayClearsCurrentStateForDisplay) {
    using Case = NonHwcVirtualDisplayCase;

    // --------------------------------------------------------------------
    // Preconditions

    // A virtual display exists
    auto existing = Case::Display::makeFakeExistingDisplayInjector(this);
    existing.inject();

    // --------------------------------------------------------------------
    // Call Expectations

    // The call should notify the interceptor that a display was created.
    EXPECT_CALL(*mSurfaceInterceptor, saveDisplayDeletion(_)).Times(1);

    // Destroying the display invalidates the display state.
    EXPECT_CALL(*mMessageQueue, invalidate()).Times(1);

    // --------------------------------------------------------------------
    // Invocation

    mFlinger.destroyDisplay(existing.token());

    // --------------------------------------------------------------------
    // Postconditions

    // The display should have been removed from the current state
    EXPECT_FALSE(hasCurrentDisplayState(existing.token()));

    // Ths display should still exist in the drawing state
    EXPECT_TRUE(hasDrawingDisplayState(existing.token()));

    // The display transaction needed flasg should be set
    EXPECT_TRUE(hasTransactionFlagSet(eDisplayTransactionNeeded));
}

TEST_F(DisplayTransactionTest, destroyDisplayHandlesUnknownDisplay) {
    // --------------------------------------------------------------------
    // Preconditions

    sp<BBinder> displayToken = new BBinder();

    // --------------------------------------------------------------------
    // Invocation

    mFlinger.destroyDisplay(displayToken);
}

/* ------------------------------------------------------------------------
 * SurfaceFlinger::resetDisplayState
 */

TEST_F(DisplayTransactionTest, resetDisplayStateClearsState) {
    using Case = NonHwcVirtualDisplayCase;

    // --------------------------------------------------------------------
    // Preconditions

    // vsync is enabled and available
    mScheduler->mutablePrimaryHWVsyncEnabled() = true;
    mScheduler->mutableHWVsyncAvailable() = true;

    // A display exists
    auto existing = Case::Display::makeFakeExistingDisplayInjector(this);
    existing.inject();

    // --------------------------------------------------------------------
    // Call Expectations

    // The call disable vsyncs
    EXPECT_CALL(*mEventControlThread, setVsyncEnabled(false)).Times(1);

    // The call ends any display resyncs
    EXPECT_CALL(*mPrimaryDispSync, endResync()).Times(1);

    // --------------------------------------------------------------------
    // Invocation

    mFlinger.resetDisplayState();

    // --------------------------------------------------------------------
    // Postconditions

    // vsyncs should be off and not available.
    EXPECT_FALSE(mScheduler->mutablePrimaryHWVsyncEnabled());
    EXPECT_FALSE(mScheduler->mutableHWVsyncAvailable());

    // The display should have been removed from the display map.
    EXPECT_FALSE(hasDisplayDevice(existing.token()));

    // The display should still exist in the current state
    EXPECT_TRUE(hasCurrentDisplayState(existing.token()));

    // The display should have been removed from the drawing state
    EXPECT_FALSE(hasDrawingDisplayState(existing.token()));
}

/* ------------------------------------------------------------------------
 * DisplayDevice::GetBestColorMode
 */
class GetBestColorModeTest : public DisplayTransactionTest {
public:
    static constexpr DisplayId DEFAULT_DISPLAY_ID = DisplayId{777};

    GetBestColorModeTest()
          : DisplayTransactionTest(),
            mInjector(FakeDisplayDeviceInjector(mFlinger, DEFAULT_DISPLAY_ID, false /* isVirtual */,
                                                true /* isPrimary */)) {}

    void setHasWideColorGamut(bool hasWideColorGamut) { mHasWideColorGamut = hasWideColorGamut; }

    void addHwcColorModesMapping(ui::ColorMode colorMode,
                                 std::vector<ui::RenderIntent> renderIntents) {
        mHwcColorModes[colorMode] = renderIntents;
    }

    void setInputDataspace(ui::Dataspace dataspace) { mInputDataspace = dataspace; }

    void setInputRenderIntent(ui::RenderIntent renderIntent) { mInputRenderIntent = renderIntent; }

    void getBestColorMode() {
        mInjector.setHwcColorModes(mHwcColorModes);
        mInjector.setHasWideColorGamut(mHasWideColorGamut);
        mInjector.setNativeWindow(mNativeWindow);

        // Creating a DisplayDevice requires getting default dimensions from the
        // native window.
        EXPECT_CALL(*mNativeWindow, query(NATIVE_WINDOW_WIDTH, _))
                .WillRepeatedly(DoAll(SetArgPointee<1>(1080 /* arbitrary */), Return(0)));
        EXPECT_CALL(*mNativeWindow, query(NATIVE_WINDOW_HEIGHT, _))
                .WillRepeatedly(DoAll(SetArgPointee<1>(1920 /* arbitrary */), Return(0)));
        EXPECT_CALL(*mNativeWindow, perform(NATIVE_WINDOW_SET_BUFFERS_FORMAT)).Times(1);
        EXPECT_CALL(*mNativeWindow, perform(NATIVE_WINDOW_API_CONNECT)).Times(1);
        EXPECT_CALL(*mNativeWindow, perform(NATIVE_WINDOW_SET_USAGE64)).Times(1);
        EXPECT_CALL(*mNativeWindow, perform(NATIVE_WINDOW_API_DISCONNECT)).Times(1);
        auto displayDevice = mInjector.inject();

        displayDevice->getCompositionDisplay()
                ->getDisplayColorProfile()
                ->getBestColorMode(mInputDataspace, mInputRenderIntent, &mOutDataspace,
                                   &mOutColorMode, &mOutRenderIntent);
    }

    ui::Dataspace mOutDataspace;
    ui::ColorMode mOutColorMode;
    ui::RenderIntent mOutRenderIntent;

private:
    ui::Dataspace mInputDataspace;
    ui::RenderIntent mInputRenderIntent;
    bool mHasWideColorGamut = false;
    std::unordered_map<ui::ColorMode, std::vector<ui::RenderIntent>> mHwcColorModes;
    FakeDisplayDeviceInjector mInjector;
};

TEST_F(GetBestColorModeTest, DataspaceDisplayP3_ColorModeSRGB) {
    addHwcColorModesMapping(ui::ColorMode::SRGB,
                            std::vector<ui::RenderIntent>(1, RenderIntent::COLORIMETRIC));
    setInputDataspace(ui::Dataspace::DISPLAY_P3);
    setInputRenderIntent(ui::RenderIntent::COLORIMETRIC);
    setHasWideColorGamut(true);

    getBestColorMode();

    ASSERT_EQ(ui::Dataspace::V0_SRGB, mOutDataspace);
    ASSERT_EQ(ui::ColorMode::SRGB, mOutColorMode);
    ASSERT_EQ(ui::RenderIntent::COLORIMETRIC, mOutRenderIntent);
}

TEST_F(GetBestColorModeTest, DataspaceDisplayP3_ColorModeDisplayP3) {
    addHwcColorModesMapping(ui::ColorMode::DISPLAY_P3,
                            std::vector<ui::RenderIntent>(1, RenderIntent::COLORIMETRIC));
    addHwcColorModesMapping(ui::ColorMode::SRGB,
                            std::vector<ui::RenderIntent>(1, RenderIntent::COLORIMETRIC));
    addHwcColorModesMapping(ui::ColorMode::DISPLAY_BT2020,
                            std::vector<ui::RenderIntent>(1, RenderIntent::COLORIMETRIC));
    setInputDataspace(ui::Dataspace::DISPLAY_P3);
    setInputRenderIntent(ui::RenderIntent::COLORIMETRIC);
    setHasWideColorGamut(true);

    getBestColorMode();

    ASSERT_EQ(ui::Dataspace::DISPLAY_P3, mOutDataspace);
    ASSERT_EQ(ui::ColorMode::DISPLAY_P3, mOutColorMode);
    ASSERT_EQ(ui::RenderIntent::COLORIMETRIC, mOutRenderIntent);
}

TEST_F(GetBestColorModeTest, DataspaceDisplayP3_ColorModeDISPLAY_BT2020) {
    addHwcColorModesMapping(ui::ColorMode::DISPLAY_BT2020,
                            std::vector<ui::RenderIntent>(1, RenderIntent::COLORIMETRIC));
    setInputDataspace(ui::Dataspace::DISPLAY_P3);
    setInputRenderIntent(ui::RenderIntent::COLORIMETRIC);
    setHasWideColorGamut(true);

    getBestColorMode();

    ASSERT_EQ(ui::Dataspace::DISPLAY_BT2020, mOutDataspace);
    ASSERT_EQ(ui::ColorMode::DISPLAY_BT2020, mOutColorMode);
    ASSERT_EQ(ui::RenderIntent::COLORIMETRIC, mOutRenderIntent);
}

/* ------------------------------------------------------------------------
 * SurfaceFlinger::getDisplayNativePrimaries
 */

class GetDisplayNativePrimaries : public DisplayTransactionTest {
public:
    GetDisplayNativePrimaries();
    void populateDummyDisplayNativePrimaries(ui::DisplayPrimaries& primaries);
    void checkDummyDisplayNativePrimaries(const ui::DisplayPrimaries& primaries);

private:
    static constexpr float mStartingTestValue = 1.0f;
};

GetDisplayNativePrimaries::GetDisplayNativePrimaries() {
    SimplePrimaryDisplayCase::Display::injectHwcDisplay(this);
    injectFakeNativeWindowSurfaceFactory();
}

void GetDisplayNativePrimaries::populateDummyDisplayNativePrimaries(
        ui::DisplayPrimaries& primaries) {
    float startingVal = mStartingTestValue;
    primaries.red.X = startingVal++;
    primaries.red.Y = startingVal++;
    primaries.red.Z = startingVal++;
    primaries.green.X = startingVal++;
    primaries.green.Y = startingVal++;
    primaries.green.Z = startingVal++;
    primaries.blue.X = startingVal++;
    primaries.blue.Y = startingVal++;
    primaries.blue.Z = startingVal++;
    primaries.white.X = startingVal++;
    primaries.white.Y = startingVal++;
    primaries.white.Z = startingVal++;
}

void GetDisplayNativePrimaries::checkDummyDisplayNativePrimaries(
        const ui::DisplayPrimaries& primaries) {
    float startingVal = mStartingTestValue;
    EXPECT_EQ(primaries.red.X, startingVal++);
    EXPECT_EQ(primaries.red.Y, startingVal++);
    EXPECT_EQ(primaries.red.Z, startingVal++);
    EXPECT_EQ(primaries.green.X, startingVal++);
    EXPECT_EQ(primaries.green.Y, startingVal++);
    EXPECT_EQ(primaries.green.Z, startingVal++);
    EXPECT_EQ(primaries.blue.X, startingVal++);
    EXPECT_EQ(primaries.blue.Y, startingVal++);
    EXPECT_EQ(primaries.blue.Z, startingVal++);
    EXPECT_EQ(primaries.white.X, startingVal++);
    EXPECT_EQ(primaries.white.Y, startingVal++);
    EXPECT_EQ(primaries.white.Z, startingVal++);
}

TEST_F(GetDisplayNativePrimaries, nullDisplayToken) {
    ui::DisplayPrimaries primaries;
    EXPECT_EQ(BAD_VALUE, mFlinger.getDisplayNativePrimaries(nullptr, primaries));
}

TEST_F(GetDisplayNativePrimaries, internalDisplayWithPrimariesData) {
    auto injector = SimplePrimaryDisplayCase::Display::makeFakeExistingDisplayInjector(this);
    injector.inject();
    auto internalDisplayToken = injector.token();

    ui::DisplayPrimaries expectedPrimaries;
    populateDummyDisplayNativePrimaries(expectedPrimaries);
    mFlinger.setInternalDisplayPrimaries(expectedPrimaries);

    ui::DisplayPrimaries primaries;
    EXPECT_EQ(NO_ERROR, mFlinger.getDisplayNativePrimaries(internalDisplayToken, primaries));

    checkDummyDisplayNativePrimaries(primaries);
}

TEST_F(GetDisplayNativePrimaries, notInternalDisplayToken) {
    sp<BBinder> notInternalDisplayToken = new BBinder();

    ui::DisplayPrimaries primaries;
    populateDummyDisplayNativePrimaries(primaries);
    EXPECT_EQ(BAD_VALUE, mFlinger.getDisplayNativePrimaries(notInternalDisplayToken, primaries));

    // Check primaries argument wasn't modified in case of failure
    checkDummyDisplayNativePrimaries(primaries);
}

/* ------------------------------------------------------------------------
 * SurfaceFlinger::setupNewDisplayDeviceInternal
 */

class SetupNewDisplayDeviceInternalTest : public DisplayTransactionTest {
public:
    template <typename T>
    void setupNewDisplayDeviceInternalTest();
};

template <typename Case>
void SetupNewDisplayDeviceInternalTest::setupNewDisplayDeviceInternalTest() {
    const sp<BBinder> displayToken = new BBinder();
    const sp<compositionengine::mock::DisplaySurface> displaySurface =
            new compositionengine::mock::DisplaySurface();
    const sp<mock::GraphicBufferProducer> producer = new mock::GraphicBufferProducer();

    // --------------------------------------------------------------------
    // Preconditions

    // Wide color displays support is configured appropriately
    Case::WideColorSupport::injectConfigChange(this);

    // The display is setup with the HWC.
    Case::Display::injectHwcDisplay(this);

    // SurfaceFlinger will use a test-controlled factory for native window
    // surfaces.
    injectFakeNativeWindowSurfaceFactory();

    // --------------------------------------------------------------------
    // Call Expectations

    // Various native window calls will be made.
    Case::Display::setupNativeWindowSurfaceCreationCallExpectations(this);
    Case::Display::setupHwcGetActiveConfigCallExpectations(this);
    Case::WideColorSupport::setupComposerCallExpectations(this);
    Case::HdrSupport::setupComposerCallExpectations(this);
    Case::PerFrameMetadataSupport::setupComposerCallExpectations(this);

    // --------------------------------------------------------------------
    // Invocation

    DisplayDeviceState state;
    state.displayId = static_cast<bool>(Case::Display::VIRTUAL) ? std::nullopt
                                                                : Case::Display::DISPLAY_ID::get();
    state.isSecure = static_cast<bool>(Case::Display::SECURE);

    auto device =
            mFlinger.setupNewDisplayDeviceInternal(displayToken, Case::Display::DISPLAY_ID::get(),
                                                   state, displaySurface, producer);

    // --------------------------------------------------------------------
    // Postconditions

    ASSERT_TRUE(device != nullptr);
    EXPECT_EQ(Case::Display::DISPLAY_ID::get(), device->getId());
    EXPECT_EQ(static_cast<bool>(Case::Display::VIRTUAL), device->isVirtual());
    EXPECT_EQ(static_cast<bool>(Case::Display::SECURE), device->isSecure());
    EXPECT_EQ(static_cast<bool>(Case::Display::PRIMARY), device->isPrimary());
    EXPECT_EQ(Case::Display::WIDTH, device->getWidth());
    EXPECT_EQ(Case::Display::HEIGHT, device->getHeight());
    EXPECT_EQ(Case::WideColorSupport::WIDE_COLOR_SUPPORTED, device->hasWideColorGamut());
    EXPECT_EQ(Case::HdrSupport::HDR10_PLUS_SUPPORTED, device->hasHDR10PlusSupport());
    EXPECT_EQ(Case::HdrSupport::HDR10_SUPPORTED, device->hasHDR10Support());
    EXPECT_EQ(Case::HdrSupport::HDR_HLG_SUPPORTED, device->hasHLGSupport());
    EXPECT_EQ(Case::HdrSupport::HDR_DOLBY_VISION_SUPPORTED, device->hasDolbyVisionSupport());
    // Note: This is not Case::Display::HWC_ACTIVE_CONFIG_ID as the ids are
    // remapped, and the test only ever sets up one config. If there were an error
    // looking up the remapped index, device->getActiveConfig() would be -1 instead.
    EXPECT_EQ(0, device->getActiveConfig());
    EXPECT_EQ(Case::PerFrameMetadataSupport::PER_FRAME_METADATA_KEYS,
              device->getSupportedPerFrameMetadata());
}

TEST_F(SetupNewDisplayDeviceInternalTest, createSimplePrimaryDisplay) {
    setupNewDisplayDeviceInternalTest<SimplePrimaryDisplayCase>();
}

TEST_F(SetupNewDisplayDeviceInternalTest, createSimpleExternalDisplay) {
    setupNewDisplayDeviceInternalTest<SimpleExternalDisplayCase>();
}

TEST_F(SetupNewDisplayDeviceInternalTest, createNonHwcVirtualDisplay) {
    setupNewDisplayDeviceInternalTest<NonHwcVirtualDisplayCase>();
}

TEST_F(SetupNewDisplayDeviceInternalTest, createHwcVirtualDisplay) {
    using Case = HwcVirtualDisplayCase;

    // Insert display data so that the HWC thinks it created the virtual display.
    const auto displayId = Case::Display::DISPLAY_ID::get();
    ASSERT_TRUE(displayId);
    mFlinger.mutableHwcDisplayData().try_emplace(*displayId);

    setupNewDisplayDeviceInternalTest<Case>();
}

TEST_F(SetupNewDisplayDeviceInternalTest, createWideColorP3Display) {
    setupNewDisplayDeviceInternalTest<WideColorP3ColorimetricDisplayCase>();
}

TEST_F(SetupNewDisplayDeviceInternalTest, createHdr10PlusDisplay) {
    setupNewDisplayDeviceInternalTest<Hdr10PlusDisplayCase>();
}

TEST_F(SetupNewDisplayDeviceInternalTest, createHdr10Display) {
    setupNewDisplayDeviceInternalTest<Hdr10DisplayCase>();
}

TEST_F(SetupNewDisplayDeviceInternalTest, createHdrHlgDisplay) {
    setupNewDisplayDeviceInternalTest<HdrHlgDisplayCase>();
}

TEST_F(SetupNewDisplayDeviceInternalTest, createHdrDolbyVisionDisplay) {
    setupNewDisplayDeviceInternalTest<HdrDolbyVisionDisplayCase>();
}

TEST_F(SetupNewDisplayDeviceInternalTest, createHdrSmpte2086DisplayCase) {
    setupNewDisplayDeviceInternalTest<HdrSmpte2086DisplayCase>();
}

TEST_F(SetupNewDisplayDeviceInternalTest, createHdrCta816_3_DisplayCase) {
    setupNewDisplayDeviceInternalTest<HdrCta861_3_DisplayCase>();
}

/* ------------------------------------------------------------------------
 * SurfaceFlinger::handleTransactionLocked(eDisplayTransactionNeeded)
 */

class HandleTransactionLockedTest : public DisplayTransactionTest {
public:
    template <typename Case>
    void setupCommonPreconditions();

    template <typename Case, bool connected>
    static void expectHotplugReceived(mock::EventThread*);

    template <typename Case>
    void setupCommonCallExpectationsForConnectProcessing();

    template <typename Case>
    void setupCommonCallExpectationsForDisconnectProcessing();

    template <typename Case>
    void processesHotplugConnectCommon();

    template <typename Case>
    void ignoresHotplugConnectCommon();

    template <typename Case>
    void processesHotplugDisconnectCommon();

    template <typename Case>
    void verifyDisplayIsConnected(const sp<IBinder>& displayToken);

    template <typename Case>
    void verifyPhysicalDisplayIsConnected();

    void verifyDisplayIsNotConnected(const sp<IBinder>& displayToken);
};

template <typename Case>
void HandleTransactionLockedTest::setupCommonPreconditions() {
    // Wide color displays support is configured appropriately
    Case::WideColorSupport::injectConfigChange(this);

    // SurfaceFlinger will use a test-controlled factory for BufferQueues
    injectFakeBufferQueueFactory();

    // SurfaceFlinger will use a test-controlled factory for native window
    // surfaces.
    injectFakeNativeWindowSurfaceFactory();
}

template <typename Case, bool connected>
void HandleTransactionLockedTest::expectHotplugReceived(mock::EventThread* eventThread) {
    const auto convert = [](auto physicalDisplayId) {
        return std::make_optional(DisplayId{physicalDisplayId});
    };

    EXPECT_CALL(*eventThread,
                onHotplugReceived(ResultOf(convert, Case::Display::DISPLAY_ID::get()), connected))
            .Times(1);
}

template <typename Case>
void HandleTransactionLockedTest::setupCommonCallExpectationsForConnectProcessing() {
    Case::Display::setupHwcHotplugCallExpectations(this);

    Case::Display::setupFramebufferConsumerBufferQueueCallExpectations(this);
    Case::Display::setupFramebufferProducerBufferQueueCallExpectations(this);
    Case::Display::setupNativeWindowSurfaceCreationCallExpectations(this);
    Case::Display::setupHwcGetActiveConfigCallExpectations(this);

    Case::WideColorSupport::setupComposerCallExpectations(this);
    Case::HdrSupport::setupComposerCallExpectations(this);
    Case::PerFrameMetadataSupport::setupComposerCallExpectations(this);

    EXPECT_CALL(*mSurfaceInterceptor, saveDisplayCreation(_)).Times(1);
    expectHotplugReceived<Case, true>(mEventThread);
    expectHotplugReceived<Case, true>(mSFEventThread);
}

template <typename Case>
void HandleTransactionLockedTest::setupCommonCallExpectationsForDisconnectProcessing() {
    EXPECT_CALL(*mSurfaceInterceptor, saveDisplayDeletion(_)).Times(1);

    expectHotplugReceived<Case, false>(mEventThread);
    expectHotplugReceived<Case, false>(mSFEventThread);
}

template <typename Case>
void HandleTransactionLockedTest::verifyDisplayIsConnected(const sp<IBinder>& displayToken) {
    // The display device should have been set up in the list of displays.
    ASSERT_TRUE(hasDisplayDevice(displayToken));
    const auto& device = getDisplayDevice(displayToken);
    EXPECT_EQ(static_cast<bool>(Case::Display::SECURE), device->isSecure());
    EXPECT_EQ(static_cast<bool>(Case::Display::PRIMARY), device->isPrimary());

    // The display should have been set up in the current display state
    ASSERT_TRUE(hasCurrentDisplayState(displayToken));
    const auto& current = getCurrentDisplayState(displayToken);
    EXPECT_EQ(static_cast<bool>(Case::Display::VIRTUAL), current.isVirtual());
    EXPECT_EQ(static_cast<bool>(Case::Display::VIRTUAL) ? std::nullopt
                                                        : Case::Display::DISPLAY_ID::get(),
              current.displayId);

    // The display should have been set up in the drawing display state
    ASSERT_TRUE(hasDrawingDisplayState(displayToken));
    const auto& draw = getDrawingDisplayState(displayToken);
    EXPECT_EQ(static_cast<bool>(Case::Display::VIRTUAL), draw.isVirtual());
    EXPECT_EQ(static_cast<bool>(Case::Display::VIRTUAL) ? std::nullopt
                                                        : Case::Display::DISPLAY_ID::get(),
              draw.displayId);
}

template <typename Case>
void HandleTransactionLockedTest::verifyPhysicalDisplayIsConnected() {
    // HWComposer should have an entry for the display
    EXPECT_TRUE(hasPhysicalHwcDisplay(Case::Display::HWC_DISPLAY_ID));

    // SF should have a display token.
    const auto displayId = Case::Display::DISPLAY_ID::get();
    ASSERT_TRUE(displayId);
    ASSERT_TRUE(mFlinger.mutablePhysicalDisplayTokens().count(*displayId) == 1);
    auto& displayToken = mFlinger.mutablePhysicalDisplayTokens()[*displayId];

    verifyDisplayIsConnected<Case>(displayToken);
}

void HandleTransactionLockedTest::verifyDisplayIsNotConnected(const sp<IBinder>& displayToken) {
    EXPECT_FALSE(hasDisplayDevice(displayToken));
    EXPECT_FALSE(hasCurrentDisplayState(displayToken));
    EXPECT_FALSE(hasDrawingDisplayState(displayToken));
}

template <typename Case>
void HandleTransactionLockedTest::processesHotplugConnectCommon() {
    // --------------------------------------------------------------------
    // Preconditions

    setupCommonPreconditions<Case>();

    // A hotplug connect event is enqueued for a display
    Case::Display::injectPendingHotplugEvent(this, HWC2::Connection::Connected);

    // --------------------------------------------------------------------
    // Call Expectations

    EXPECT_CALL(*mComposer, isUsingVrComposer()).WillOnce(Return(false));

    setupCommonCallExpectationsForConnectProcessing<Case>();

    // --------------------------------------------------------------------
    // Invocation

    mFlinger.handleTransactionLocked(eDisplayTransactionNeeded);

    // --------------------------------------------------------------------
    // Postconditions

    verifyPhysicalDisplayIsConnected<Case>();

    // --------------------------------------------------------------------
    // Cleanup conditions

    EXPECT_CALL(*mComposer,
                setVsyncEnabled(Case::Display::HWC_DISPLAY_ID, IComposerClient::Vsync::DISABLE))
            .WillOnce(Return(Error::NONE));
    EXPECT_CALL(*mConsumer, consumerDisconnect()).WillOnce(Return(NO_ERROR));
}

template <typename Case>
void HandleTransactionLockedTest::ignoresHotplugConnectCommon() {
    // --------------------------------------------------------------------
    // Preconditions

    setupCommonPreconditions<Case>();

    // A hotplug connect event is enqueued for a display
    Case::Display::injectPendingHotplugEvent(this, HWC2::Connection::Connected);

    // --------------------------------------------------------------------
    // Invocation

    mFlinger.handleTransactionLocked(eDisplayTransactionNeeded);

    // --------------------------------------------------------------------
    // Postconditions

    // HWComposer should not have an entry for the display
    EXPECT_FALSE(hasPhysicalHwcDisplay(Case::Display::HWC_DISPLAY_ID));
}

template <typename Case>
void HandleTransactionLockedTest::processesHotplugDisconnectCommon() {
    // --------------------------------------------------------------------
    // Preconditions

    setupCommonPreconditions<Case>();

    // A hotplug disconnect event is enqueued for a display
    Case::Display::injectPendingHotplugEvent(this, HWC2::Connection::Disconnected);

    // The display is already completely set up.
    Case::Display::injectHwcDisplay(this);
    auto existing = Case::Display::makeFakeExistingDisplayInjector(this);
    existing.inject();

    // --------------------------------------------------------------------
    // Call Expectations

    EXPECT_CALL(*mComposer, isUsingVrComposer()).WillRepeatedly(Return(false));
    EXPECT_CALL(*mComposer, getDisplayIdentificationData(Case::Display::HWC_DISPLAY_ID, _, _))
            .Times(0);

    setupCommonCallExpectationsForDisconnectProcessing<Case>();

    // --------------------------------------------------------------------
    // Invocation

    mFlinger.handleTransactionLocked(eDisplayTransactionNeeded);

    // --------------------------------------------------------------------
    // Postconditions

    // HWComposer should not have an entry for the display
    EXPECT_FALSE(hasPhysicalHwcDisplay(Case::Display::HWC_DISPLAY_ID));

    // SF should not have a display token.
    const auto displayId = Case::Display::DISPLAY_ID::get();
    ASSERT_TRUE(displayId);
    ASSERT_TRUE(mFlinger.mutablePhysicalDisplayTokens().count(*displayId) == 0);

    // The existing token should have been removed
    verifyDisplayIsNotConnected(existing.token());
}

TEST_F(HandleTransactionLockedTest, processesHotplugConnectPrimaryDisplay) {
    processesHotplugConnectCommon<SimplePrimaryDisplayCase>();
}

TEST_F(HandleTransactionLockedTest,
       processesHotplugConnectPrimaryDisplayWithExternalAlreadyConnected) {
    // Inject an external display.
    ExternalDisplayVariant::injectHwcDisplay(this);

    processesHotplugConnectCommon<SimplePrimaryDisplayCase>();
}

TEST_F(HandleTransactionLockedTest, processesHotplugConnectExternalDisplay) {
    // Inject a primary display.
    PrimaryDisplayVariant::injectHwcDisplay(this);

    processesHotplugConnectCommon<SimpleExternalDisplayCase>();
}

TEST_F(HandleTransactionLockedTest, ignoresHotplugConnectIfPrimaryAndExternalAlreadyConnected) {
    // Inject both a primary and external display.
    PrimaryDisplayVariant::injectHwcDisplay(this);
    ExternalDisplayVariant::injectHwcDisplay(this);

    // TODO: This is an unnecessary call.
    EXPECT_CALL(*mComposer,
                getDisplayIdentificationData(TertiaryDisplayVariant::HWC_DISPLAY_ID, _, _))
            .WillOnce(DoAll(SetArgPointee<1>(TertiaryDisplay::PORT),
                            SetArgPointee<2>(TertiaryDisplay::GET_IDENTIFICATION_DATA()),
                            Return(Error::NONE)));

    EXPECT_CALL(*mComposer, isUsingVrComposer()).WillRepeatedly(Return(false));

    ignoresHotplugConnectCommon<SimpleTertiaryDisplayCase>();
}

TEST_F(HandleTransactionLockedTest, ignoresHotplugConnectIfExternalForVrComposer) {
    // Inject a primary display.
    PrimaryDisplayVariant::injectHwcDisplay(this);

    EXPECT_CALL(*mComposer, isUsingVrComposer()).WillRepeatedly(Return(true));

    ignoresHotplugConnectCommon<SimpleExternalDisplayCase>();
}

TEST_F(HandleTransactionLockedTest, processHotplugDisconnectPrimaryDisplay) {
    processesHotplugDisconnectCommon<SimplePrimaryDisplayCase>();
}

TEST_F(HandleTransactionLockedTest, processHotplugDisconnectExternalDisplay) {
    processesHotplugDisconnectCommon<SimpleExternalDisplayCase>();
}

TEST_F(HandleTransactionLockedTest, processesHotplugConnectThenDisconnectPrimary) {
    using Case = SimplePrimaryDisplayCase;

    // --------------------------------------------------------------------
    // Preconditions

    setupCommonPreconditions<Case>();

    // A hotplug connect event is enqueued for a display
    Case::Display::injectPendingHotplugEvent(this, HWC2::Connection::Connected);
    // A hotplug disconnect event is also enqueued for the same display
    Case::Display::injectPendingHotplugEvent(this, HWC2::Connection::Disconnected);

    // --------------------------------------------------------------------
    // Call Expectations

    EXPECT_CALL(*mComposer, isUsingVrComposer()).WillRepeatedly(Return(false));

    setupCommonCallExpectationsForConnectProcessing<Case>();
    setupCommonCallExpectationsForDisconnectProcessing<Case>();

    EXPECT_CALL(*mComposer,
                setVsyncEnabled(Case::Display::HWC_DISPLAY_ID, IComposerClient::Vsync::DISABLE))
            .WillOnce(Return(Error::NONE));
    EXPECT_CALL(*mConsumer, consumerDisconnect()).WillOnce(Return(NO_ERROR));

    // --------------------------------------------------------------------
    // Invocation

    mFlinger.handleTransactionLocked(eDisplayTransactionNeeded);

    // --------------------------------------------------------------------
    // Postconditions

    // HWComposer should not have an entry for the display
    EXPECT_FALSE(hasPhysicalHwcDisplay(Case::Display::HWC_DISPLAY_ID));

    // SF should not have a display token.
    const auto displayId = Case::Display::DISPLAY_ID::get();
    ASSERT_TRUE(displayId);
    ASSERT_TRUE(mFlinger.mutablePhysicalDisplayTokens().count(*displayId) == 0);
}

TEST_F(HandleTransactionLockedTest, processesHotplugDisconnectThenConnectPrimary) {
    using Case = SimplePrimaryDisplayCase;

    // --------------------------------------------------------------------
    // Preconditions

    setupCommonPreconditions<Case>();

    // The display is already completely set up.
    Case::Display::injectHwcDisplay(this);
    auto existing = Case::Display::makeFakeExistingDisplayInjector(this);
    existing.inject();

    // A hotplug disconnect event is enqueued for a display
    Case::Display::injectPendingHotplugEvent(this, HWC2::Connection::Disconnected);
    // A hotplug connect event is also enqueued for the same display
    Case::Display::injectPendingHotplugEvent(this, HWC2::Connection::Connected);

    // --------------------------------------------------------------------
    // Call Expectations

    EXPECT_CALL(*mComposer, isUsingVrComposer()).WillRepeatedly(Return(false));

    setupCommonCallExpectationsForConnectProcessing<Case>();
    setupCommonCallExpectationsForDisconnectProcessing<Case>();

    // --------------------------------------------------------------------
    // Invocation

    mFlinger.handleTransactionLocked(eDisplayTransactionNeeded);

    // --------------------------------------------------------------------
    // Postconditions

    // The existing token should have been removed
    verifyDisplayIsNotConnected(existing.token());
    const auto displayId = Case::Display::DISPLAY_ID::get();
    ASSERT_TRUE(displayId);
    ASSERT_TRUE(mFlinger.mutablePhysicalDisplayTokens().count(*displayId) == 1);
    EXPECT_NE(existing.token(), mFlinger.mutablePhysicalDisplayTokens()[*displayId]);

    // A new display should be connected in its place

    verifyPhysicalDisplayIsConnected<Case>();

    // --------------------------------------------------------------------
    // Cleanup conditions

    EXPECT_CALL(*mComposer,
                setVsyncEnabled(Case::Display::HWC_DISPLAY_ID, IComposerClient::Vsync::DISABLE))
            .WillOnce(Return(Error::NONE));
    EXPECT_CALL(*mConsumer, consumerDisconnect()).WillOnce(Return(NO_ERROR));
}

TEST_F(HandleTransactionLockedTest, processesVirtualDisplayAdded) {
    using Case = HwcVirtualDisplayCase;

    // --------------------------------------------------------------------
    // Preconditions

    // The HWC supports at least one virtual display
    injectMockComposer(1);

    setupCommonPreconditions<Case>();

    // A virtual display was added to the current state, and it has a
    // surface(producer)
    sp<BBinder> displayToken = new BBinder();

    DisplayDeviceState state;
    state.isSecure = static_cast<bool>(Case::Display::SECURE);

    sp<mock::GraphicBufferProducer> surface{new mock::GraphicBufferProducer()};
    state.surface = surface;
    mFlinger.mutableCurrentState().displays.add(displayToken, state);

    // --------------------------------------------------------------------
    // Call Expectations

    Case::Display::setupFramebufferConsumerBufferQueueCallExpectations(this);
    Case::Display::setupNativeWindowSurfaceCreationCallExpectations(this);

    EXPECT_CALL(*surface, query(NATIVE_WINDOW_WIDTH, _))
            .WillRepeatedly(DoAll(SetArgPointee<1>(Case::Display::WIDTH), Return(NO_ERROR)));
    EXPECT_CALL(*surface, query(NATIVE_WINDOW_HEIGHT, _))
            .WillRepeatedly(DoAll(SetArgPointee<1>(Case::Display::HEIGHT), Return(NO_ERROR)));
    EXPECT_CALL(*surface, query(NATIVE_WINDOW_FORMAT, _))
            .WillRepeatedly(DoAll(SetArgPointee<1>(DEFAULT_VIRTUAL_DISPLAY_SURFACE_FORMAT),
                                  Return(NO_ERROR)));
    EXPECT_CALL(*surface, query(NATIVE_WINDOW_CONSUMER_USAGE_BITS, _))
            .WillRepeatedly(DoAll(SetArgPointee<1>(0), Return(NO_ERROR)));

    EXPECT_CALL(*surface, setAsyncMode(true)).Times(1);

    EXPECT_CALL(*mProducer, connect(_, NATIVE_WINDOW_API_EGL, false, _)).Times(1);
    EXPECT_CALL(*mProducer, disconnect(_, _)).Times(1);

    Case::Display::setupHwcVirtualDisplayCreationCallExpectations(this);
    Case::WideColorSupport::setupComposerCallExpectations(this);
    Case::HdrSupport::setupComposerCallExpectations(this);
    Case::PerFrameMetadataSupport::setupComposerCallExpectations(this);

    // --------------------------------------------------------------------
    // Invocation

    mFlinger.handleTransactionLocked(eDisplayTransactionNeeded);

    // --------------------------------------------------------------------
    // Postconditions

    // The display device should have been set up in the list of displays.
    verifyDisplayIsConnected<Case>(displayToken);

    // --------------------------------------------------------------------
    // Cleanup conditions

    EXPECT_CALL(*mComposer, destroyVirtualDisplay(Case::Display::HWC_DISPLAY_ID))
            .WillOnce(Return(Error::NONE));
    EXPECT_CALL(*mConsumer, consumerDisconnect()).WillOnce(Return(NO_ERROR));

    // Cleanup
    mFlinger.mutableCurrentState().displays.removeItem(displayToken);
    mFlinger.mutableDrawingState().displays.removeItem(displayToken);
}

TEST_F(HandleTransactionLockedTest, processesVirtualDisplayAddedWithNoSurface) {
    using Case = HwcVirtualDisplayCase;

    // --------------------------------------------------------------------
    // Preconditions

    // The HWC supports at least one virtual display
    injectMockComposer(1);

    setupCommonPreconditions<Case>();

    // A virtual display was added to the current state, but it does not have a
    // surface.
    sp<BBinder> displayToken = new BBinder();

    DisplayDeviceState state;
    state.isSecure = static_cast<bool>(Case::Display::SECURE);

    mFlinger.mutableCurrentState().displays.add(displayToken, state);

    // --------------------------------------------------------------------
    // Call Expectations

    // --------------------------------------------------------------------
    // Invocation

    mFlinger.handleTransactionLocked(eDisplayTransactionNeeded);

    // --------------------------------------------------------------------
    // Postconditions

    // There will not be a display device set up.
    EXPECT_FALSE(hasDisplayDevice(displayToken));

    // The drawing display state will be set from the current display state.
    ASSERT_TRUE(hasDrawingDisplayState(displayToken));
    const auto& draw = getDrawingDisplayState(displayToken);
    EXPECT_EQ(static_cast<bool>(Case::Display::VIRTUAL), draw.isVirtual());
}

TEST_F(HandleTransactionLockedTest, processesVirtualDisplayRemoval) {
    using Case = HwcVirtualDisplayCase;

    // --------------------------------------------------------------------
    // Preconditions

    // A virtual display is set up but is removed from the current state.
    const auto displayId = Case::Display::DISPLAY_ID::get();
    ASSERT_TRUE(displayId);
    mFlinger.mutableHwcDisplayData().try_emplace(*displayId);
    Case::Display::injectHwcDisplay(this);
    auto existing = Case::Display::makeFakeExistingDisplayInjector(this);
    existing.inject();
    mFlinger.mutableCurrentState().displays.removeItem(existing.token());

    // --------------------------------------------------------------------
    // Call Expectations

    EXPECT_CALL(*mComposer, isUsingVrComposer()).WillRepeatedly(Return(false));

    // --------------------------------------------------------------------
    // Invocation

    mFlinger.handleTransactionLocked(eDisplayTransactionNeeded);

    // --------------------------------------------------------------------
    // Postconditions

    // The existing token should have been removed
    verifyDisplayIsNotConnected(existing.token());
}

TEST_F(HandleTransactionLockedTest, processesDisplayLayerStackChanges) {
    using Case = NonHwcVirtualDisplayCase;

    constexpr uint32_t oldLayerStack = 0u;
    constexpr uint32_t newLayerStack = 123u;

    // --------------------------------------------------------------------
    // Preconditions

    // A display is set up
    auto display = Case::Display::makeFakeExistingDisplayInjector(this);
    display.inject();

    // There is a change to the layerStack state
    display.mutableDrawingDisplayState().layerStack = oldLayerStack;
    display.mutableCurrentDisplayState().layerStack = newLayerStack;

    // --------------------------------------------------------------------
    // Invocation

    mFlinger.handleTransactionLocked(eDisplayTransactionNeeded);

    // --------------------------------------------------------------------
    // Postconditions

    EXPECT_EQ(newLayerStack, display.mutableDisplayDevice()->getLayerStack());
}

TEST_F(HandleTransactionLockedTest, processesDisplayTransformChanges) {
    using Case = NonHwcVirtualDisplayCase;

    constexpr int oldTransform = 0;
    constexpr int newTransform = 2;

    // --------------------------------------------------------------------
    // Preconditions

    // A display is set up
    auto display = Case::Display::makeFakeExistingDisplayInjector(this);
    display.inject();

    // There is a change to the orientation state
    display.mutableDrawingDisplayState().orientation = oldTransform;
    display.mutableCurrentDisplayState().orientation = newTransform;

    // --------------------------------------------------------------------
    // Invocation

    mFlinger.handleTransactionLocked(eDisplayTransactionNeeded);

    // --------------------------------------------------------------------
    // Postconditions

    EXPECT_EQ(newTransform, display.mutableDisplayDevice()->getOrientation());
}

TEST_F(HandleTransactionLockedTest, processesDisplayViewportChanges) {
    using Case = NonHwcVirtualDisplayCase;

    const Rect oldViewport(0, 0, 0, 0);
    const Rect newViewport(0, 0, 123, 456);

    // --------------------------------------------------------------------
    // Preconditions

    // A display is set up
    auto display = Case::Display::makeFakeExistingDisplayInjector(this);
    display.inject();

    // There is a change to the viewport state
    display.mutableDrawingDisplayState().viewport = oldViewport;
    display.mutableCurrentDisplayState().viewport = newViewport;

    // --------------------------------------------------------------------
    // Invocation

    mFlinger.handleTransactionLocked(eDisplayTransactionNeeded);

    // --------------------------------------------------------------------
    // Postconditions

    EXPECT_EQ(newViewport, display.mutableDisplayDevice()->getViewport());
}

TEST_F(HandleTransactionLockedTest, processesDisplayFrameChanges) {
    using Case = NonHwcVirtualDisplayCase;

    const Rect oldFrame(0, 0, 0, 0);
    const Rect newFrame(0, 0, 123, 456);

    // --------------------------------------------------------------------
    // Preconditions

    // A display is set up
    auto display = Case::Display::makeFakeExistingDisplayInjector(this);
    display.inject();

    // There is a change to the viewport state
    display.mutableDrawingDisplayState().frame = oldFrame;
    display.mutableCurrentDisplayState().frame = newFrame;

    // --------------------------------------------------------------------
    // Invocation

    mFlinger.handleTransactionLocked(eDisplayTransactionNeeded);

    // --------------------------------------------------------------------
    // Postconditions

    EXPECT_EQ(newFrame, display.mutableDisplayDevice()->getFrame());
}

TEST_F(HandleTransactionLockedTest, processesDisplayWidthChanges) {
    using Case = NonHwcVirtualDisplayCase;

    constexpr int oldWidth = 0;
    constexpr int oldHeight = 10;
    constexpr int newWidth = 123;

    // --------------------------------------------------------------------
    // Preconditions

    // A display is set up
    auto nativeWindow = new mock::NativeWindow();
    auto displaySurface = new compositionengine::mock::DisplaySurface();
    sp<GraphicBuffer> buf = new GraphicBuffer();
    auto display = Case::Display::makeFakeExistingDisplayInjector(this);
    display.setNativeWindow(nativeWindow);
    display.setDisplaySurface(displaySurface);
    // Setup injection expections
    EXPECT_CALL(*nativeWindow, query(NATIVE_WINDOW_WIDTH, _))
            .WillOnce(DoAll(SetArgPointee<1>(oldWidth), Return(0)));
    EXPECT_CALL(*nativeWindow, query(NATIVE_WINDOW_HEIGHT, _))
            .WillOnce(DoAll(SetArgPointee<1>(oldHeight), Return(0)));
    EXPECT_CALL(*nativeWindow, perform(NATIVE_WINDOW_SET_BUFFERS_FORMAT)).Times(1);
    EXPECT_CALL(*nativeWindow, perform(NATIVE_WINDOW_API_CONNECT)).Times(1);
    EXPECT_CALL(*nativeWindow, perform(NATIVE_WINDOW_SET_USAGE64)).Times(1);
    EXPECT_CALL(*nativeWindow, perform(NATIVE_WINDOW_API_DISCONNECT)).Times(1);
    display.inject();

    // There is a change to the viewport state
    display.mutableDrawingDisplayState().width = oldWidth;
    display.mutableDrawingDisplayState().height = oldHeight;
    display.mutableCurrentDisplayState().width = newWidth;
    display.mutableCurrentDisplayState().height = oldHeight;

    // --------------------------------------------------------------------
    // Call Expectations

    EXPECT_CALL(*displaySurface, resizeBuffers(newWidth, oldHeight)).Times(1);

    // --------------------------------------------------------------------
    // Invocation

    mFlinger.handleTransactionLocked(eDisplayTransactionNeeded);
}

TEST_F(HandleTransactionLockedTest, processesDisplayHeightChanges) {
    using Case = NonHwcVirtualDisplayCase;

    constexpr int oldWidth = 0;
    constexpr int oldHeight = 10;
    constexpr int newHeight = 123;

    // --------------------------------------------------------------------
    // Preconditions

    // A display is set up
    auto nativeWindow = new mock::NativeWindow();
    auto displaySurface = new compositionengine::mock::DisplaySurface();
    sp<GraphicBuffer> buf = new GraphicBuffer();
    auto display = Case::Display::makeFakeExistingDisplayInjector(this);
    display.setNativeWindow(nativeWindow);
    display.setDisplaySurface(displaySurface);
    // Setup injection expections
    EXPECT_CALL(*nativeWindow, query(NATIVE_WINDOW_WIDTH, _))
            .WillOnce(DoAll(SetArgPointee<1>(oldWidth), Return(0)));
    EXPECT_CALL(*nativeWindow, query(NATIVE_WINDOW_HEIGHT, _))
            .WillOnce(DoAll(SetArgPointee<1>(oldHeight), Return(0)));
    EXPECT_CALL(*nativeWindow, perform(NATIVE_WINDOW_SET_BUFFERS_FORMAT)).Times(1);
    EXPECT_CALL(*nativeWindow, perform(NATIVE_WINDOW_API_CONNECT)).Times(1);
    EXPECT_CALL(*nativeWindow, perform(NATIVE_WINDOW_SET_USAGE64)).Times(1);
    EXPECT_CALL(*nativeWindow, perform(NATIVE_WINDOW_API_DISCONNECT)).Times(1);
    display.inject();

    // There is a change to the viewport state
    display.mutableDrawingDisplayState().width = oldWidth;
    display.mutableDrawingDisplayState().height = oldHeight;
    display.mutableCurrentDisplayState().width = oldWidth;
    display.mutableCurrentDisplayState().height = newHeight;

    // --------------------------------------------------------------------
    // Call Expectations

    EXPECT_CALL(*displaySurface, resizeBuffers(oldWidth, newHeight)).Times(1);

    // --------------------------------------------------------------------
    // Invocation

    mFlinger.handleTransactionLocked(eDisplayTransactionNeeded);
}

/* ------------------------------------------------------------------------
 * SurfaceFlinger::setDisplayStateLocked
 */

TEST_F(DisplayTransactionTest, setDisplayStateLockedDoesNothingWithUnknownDisplay) {
    // --------------------------------------------------------------------
    // Preconditions

    // We have an unknown display token not associated with a known display
    sp<BBinder> displayToken = new BBinder();

    // The requested display state references the unknown display.
    DisplayState state;
    state.what = DisplayState::eLayerStackChanged;
    state.token = displayToken;
    state.layerStack = 456;

    // --------------------------------------------------------------------
    // Invocation

    uint32_t flags = mFlinger.setDisplayStateLocked(state);

    // --------------------------------------------------------------------
    // Postconditions

    // The returned flags are empty
    EXPECT_EQ(0u, flags);

    // The display token still doesn't match anything known.
    EXPECT_FALSE(hasCurrentDisplayState(displayToken));
}

TEST_F(DisplayTransactionTest, setDisplayStateLockedDoesNothingWhenNoChanges) {
    using Case = SimplePrimaryDisplayCase;

    // --------------------------------------------------------------------
    // Preconditions

    // A display is already set up
    auto display = Case::Display::makeFakeExistingDisplayInjector(this);
    display.inject();

    // No changes are made to the display
    DisplayState state;
    state.what = 0;
    state.token = display.token();

    // --------------------------------------------------------------------
    // Invocation

    uint32_t flags = mFlinger.setDisplayStateLocked(state);

    // --------------------------------------------------------------------
    // Postconditions

    // The returned flags are empty
    EXPECT_EQ(0u, flags);
}

TEST_F(DisplayTransactionTest, setDisplayStateLockedDoesNothingIfSurfaceDidNotChange) {
    using Case = SimplePrimaryDisplayCase;

    // --------------------------------------------------------------------
    // Preconditions

    // A display is already set up
    auto display = Case::Display::makeFakeExistingDisplayInjector(this);
    display.inject();

    // There is a surface that can be set.
    sp<mock::GraphicBufferProducer> surface = new mock::GraphicBufferProducer();

    // The current display state has the surface set
    display.mutableCurrentDisplayState().surface = surface;

    // The incoming request sets the same surface
    DisplayState state;
    state.what = DisplayState::eSurfaceChanged;
    state.token = display.token();
    state.surface = surface;

    // --------------------------------------------------------------------
    // Invocation

    uint32_t flags = mFlinger.setDisplayStateLocked(state);

    // --------------------------------------------------------------------
    // Postconditions

    // The returned flags are empty
    EXPECT_EQ(0u, flags);

    // The current display state is unchanged.
    EXPECT_EQ(surface.get(), display.getCurrentDisplayState().surface.get());
}

TEST_F(DisplayTransactionTest, setDisplayStateLockedRequestsUpdateIfSurfaceChanged) {
    using Case = SimplePrimaryDisplayCase;

    // --------------------------------------------------------------------
    // Preconditions

    // A display is already set up
    auto display = Case::Display::makeFakeExistingDisplayInjector(this);
    display.inject();

    // There is a surface that can be set.
    sp<mock::GraphicBufferProducer> surface = new mock::GraphicBufferProducer();

    // The current display state does not have a surface
    display.mutableCurrentDisplayState().surface = nullptr;

    // The incoming request sets a surface
    DisplayState state;
    state.what = DisplayState::eSurfaceChanged;
    state.token = display.token();
    state.surface = surface;

    // --------------------------------------------------------------------
    // Invocation

    uint32_t flags = mFlinger.setDisplayStateLocked(state);

    // --------------------------------------------------------------------
    // Postconditions

    // The returned flags indicate a transaction is needed
    EXPECT_EQ(eDisplayTransactionNeeded, flags);

    // The current display layer stack state is set to the new value
    EXPECT_EQ(surface.get(), display.getCurrentDisplayState().surface.get());
}

TEST_F(DisplayTransactionTest, setDisplayStateLockedDoesNothingIfLayerStackDidNotChange) {
    using Case = SimplePrimaryDisplayCase;

    // --------------------------------------------------------------------
    // Preconditions

    // A display is already set up
    auto display = Case::Display::makeFakeExistingDisplayInjector(this);
    display.inject();

    // The display has a layer stack set
    display.mutableCurrentDisplayState().layerStack = 456u;

    // The incoming request sets the same layer stack
    DisplayState state;
    state.what = DisplayState::eLayerStackChanged;
    state.token = display.token();
    state.layerStack = 456u;

    // --------------------------------------------------------------------
    // Invocation

    uint32_t flags = mFlinger.setDisplayStateLocked(state);

    // --------------------------------------------------------------------
    // Postconditions

    // The returned flags are empty
    EXPECT_EQ(0u, flags);

    // The current display state is unchanged
    EXPECT_EQ(456u, display.getCurrentDisplayState().layerStack);
}

TEST_F(DisplayTransactionTest, setDisplayStateLockedRequestsUpdateIfLayerStackChanged) {
    using Case = SimplePrimaryDisplayCase;

    // --------------------------------------------------------------------
    // Preconditions

    // A display is set up
    auto display = Case::Display::makeFakeExistingDisplayInjector(this);
    display.inject();

    // The display has a layer stack set
    display.mutableCurrentDisplayState().layerStack = 654u;

    // The incoming request sets a different layer stack
    DisplayState state;
    state.what = DisplayState::eLayerStackChanged;
    state.token = display.token();
    state.layerStack = 456u;

    // --------------------------------------------------------------------
    // Invocation

    uint32_t flags = mFlinger.setDisplayStateLocked(state);

    // --------------------------------------------------------------------
    // Postconditions

    // The returned flags indicate a transaction is needed
    EXPECT_EQ(eDisplayTransactionNeeded, flags);

    // The desired display state has been set to the new value.
    EXPECT_EQ(456u, display.getCurrentDisplayState().layerStack);
}

TEST_F(DisplayTransactionTest, setDisplayStateLockedDoesNothingIfProjectionDidNotChange) {
    using Case = SimplePrimaryDisplayCase;
    constexpr int initialOrientation = 180;
    const Rect initialFrame = {1, 2, 3, 4};
    const Rect initialViewport = {5, 6, 7, 8};

    // --------------------------------------------------------------------
    // Preconditions

    // A display is set up
    auto display = Case::Display::makeFakeExistingDisplayInjector(this);
    display.inject();

    // The current display state projection state is all set
    display.mutableCurrentDisplayState().orientation = initialOrientation;
    display.mutableCurrentDisplayState().frame = initialFrame;
    display.mutableCurrentDisplayState().viewport = initialViewport;

    // The incoming request sets the same projection state
    DisplayState state;
    state.what = DisplayState::eDisplayProjectionChanged;
    state.token = display.token();
    state.orientation = initialOrientation;
    state.frame = initialFrame;
    state.viewport = initialViewport;

    // --------------------------------------------------------------------
    // Invocation

    uint32_t flags = mFlinger.setDisplayStateLocked(state);

    // --------------------------------------------------------------------
    // Postconditions

    // The returned flags are empty
    EXPECT_EQ(0u, flags);

    // The current display state is unchanged
    EXPECT_EQ(initialOrientation, display.getCurrentDisplayState().orientation);

    EXPECT_EQ(initialFrame, display.getCurrentDisplayState().frame);
    EXPECT_EQ(initialViewport, display.getCurrentDisplayState().viewport);
}

TEST_F(DisplayTransactionTest, setDisplayStateLockedRequestsUpdateIfOrientationChanged) {
    using Case = SimplePrimaryDisplayCase;
    constexpr int initialOrientation = 90;
    constexpr int desiredOrientation = 180;

    // --------------------------------------------------------------------
    // Preconditions

    // A display is set up
    auto display = Case::Display::makeFakeExistingDisplayInjector(this);
    display.inject();

    // The current display state has an orientation set
    display.mutableCurrentDisplayState().orientation = initialOrientation;

    // The incoming request sets a different orientation
    DisplayState state;
    state.what = DisplayState::eDisplayProjectionChanged;
    state.token = display.token();
    state.orientation = desiredOrientation;

    // --------------------------------------------------------------------
    // Invocation

    uint32_t flags = mFlinger.setDisplayStateLocked(state);

    // --------------------------------------------------------------------
    // Postconditions

    // The returned flags indicate a transaction is needed
    EXPECT_EQ(eDisplayTransactionNeeded, flags);

    // The current display state has the new value.
    EXPECT_EQ(desiredOrientation, display.getCurrentDisplayState().orientation);
}

TEST_F(DisplayTransactionTest, setDisplayStateLockedRequestsUpdateIfFrameChanged) {
    using Case = SimplePrimaryDisplayCase;
    const Rect initialFrame = {0, 0, 0, 0};
    const Rect desiredFrame = {5, 6, 7, 8};

    // --------------------------------------------------------------------
    // Preconditions

    // A display is set up
    auto display = Case::Display::makeFakeExistingDisplayInjector(this);
    display.inject();

    // The current display state does not have a frame
    display.mutableCurrentDisplayState().frame = initialFrame;

    // The incoming request sets a frame
    DisplayState state;
    state.what = DisplayState::eDisplayProjectionChanged;
    state.token = display.token();
    state.frame = desiredFrame;

    // --------------------------------------------------------------------
    // Invocation

    uint32_t flags = mFlinger.setDisplayStateLocked(state);

    // --------------------------------------------------------------------
    // Postconditions

    // The returned flags indicate a transaction is needed
    EXPECT_EQ(eDisplayTransactionNeeded, flags);

    // The current display state has the new value.
    EXPECT_EQ(desiredFrame, display.getCurrentDisplayState().frame);
}

TEST_F(DisplayTransactionTest, setDisplayStateLockedRequestsUpdateIfViewportChanged) {
    using Case = SimplePrimaryDisplayCase;
    const Rect initialViewport = {0, 0, 0, 0};
    const Rect desiredViewport = {5, 6, 7, 8};

    // --------------------------------------------------------------------
    // Preconditions

    // A display is set up
    auto display = Case::Display::makeFakeExistingDisplayInjector(this);
    display.inject();

    // The current display state does not have a viewport
    display.mutableCurrentDisplayState().viewport = initialViewport;

    // The incoming request sets a viewport
    DisplayState state;
    state.what = DisplayState::eDisplayProjectionChanged;
    state.token = display.token();
    state.viewport = desiredViewport;

    // --------------------------------------------------------------------
    // Invocation

    uint32_t flags = mFlinger.setDisplayStateLocked(state);

    // --------------------------------------------------------------------
    // Postconditions

    // The returned flags indicate a transaction is needed
    EXPECT_EQ(eDisplayTransactionNeeded, flags);

    // The current display state has the new value.
    EXPECT_EQ(desiredViewport, display.getCurrentDisplayState().viewport);
}

TEST_F(DisplayTransactionTest, setDisplayStateLockedDoesNothingIfSizeDidNotChange) {
    using Case = SimplePrimaryDisplayCase;
    constexpr uint32_t initialWidth = 1024;
    constexpr uint32_t initialHeight = 768;

    // --------------------------------------------------------------------
    // Preconditions

    // A display is set up
    auto display = Case::Display::makeFakeExistingDisplayInjector(this);
    display.inject();

    // The current display state has a size set
    display.mutableCurrentDisplayState().width = initialWidth;
    display.mutableCurrentDisplayState().height = initialHeight;

    // The incoming request sets the same display size
    DisplayState state;
    state.what = DisplayState::eDisplaySizeChanged;
    state.token = display.token();
    state.width = initialWidth;
    state.height = initialHeight;

    // --------------------------------------------------------------------
    // Invocation

    uint32_t flags = mFlinger.setDisplayStateLocked(state);

    // --------------------------------------------------------------------
    // Postconditions

    // The returned flags are empty
    EXPECT_EQ(0u, flags);

    // The current display state is unchanged
    EXPECT_EQ(initialWidth, display.getCurrentDisplayState().width);
    EXPECT_EQ(initialHeight, display.getCurrentDisplayState().height);
}

TEST_F(DisplayTransactionTest, setDisplayStateLockedRequestsUpdateIfWidthChanged) {
    using Case = SimplePrimaryDisplayCase;
    constexpr uint32_t initialWidth = 0;
    constexpr uint32_t desiredWidth = 1024;

    // --------------------------------------------------------------------
    // Preconditions

    // A display is set up
    auto display = Case::Display::makeFakeExistingDisplayInjector(this);
    display.inject();

    // The display does not yet have a width
    display.mutableCurrentDisplayState().width = initialWidth;

    // The incoming request sets a display width
    DisplayState state;
    state.what = DisplayState::eDisplaySizeChanged;
    state.token = display.token();
    state.width = desiredWidth;

    // --------------------------------------------------------------------
    // Invocation

    uint32_t flags = mFlinger.setDisplayStateLocked(state);

    // --------------------------------------------------------------------
    // Postconditions

    // The returned flags indicate a transaction is needed
    EXPECT_EQ(eDisplayTransactionNeeded, flags);

    // The current display state has the new value.
    EXPECT_EQ(desiredWidth, display.getCurrentDisplayState().width);
}

TEST_F(DisplayTransactionTest, setDisplayStateLockedRequestsUpdateIfHeightChanged) {
    using Case = SimplePrimaryDisplayCase;
    constexpr uint32_t initialHeight = 0;
    constexpr uint32_t desiredHeight = 768;

    // --------------------------------------------------------------------
    // Preconditions

    // A display is set up
    auto display = Case::Display::makeFakeExistingDisplayInjector(this);
    display.inject();

    // The display does not yet have a height
    display.mutableCurrentDisplayState().height = initialHeight;

    // The incoming request sets a display height
    DisplayState state;
    state.what = DisplayState::eDisplaySizeChanged;
    state.token = display.token();
    state.height = desiredHeight;

    // --------------------------------------------------------------------
    // Invocation

    uint32_t flags = mFlinger.setDisplayStateLocked(state);

    // --------------------------------------------------------------------
    // Postconditions

    // The returned flags indicate a transaction is needed
    EXPECT_EQ(eDisplayTransactionNeeded, flags);

    // The current display state has the new value.
    EXPECT_EQ(desiredHeight, display.getCurrentDisplayState().height);
}

/* ------------------------------------------------------------------------
 * SurfaceFlinger::onInitializeDisplays
 */

TEST_F(DisplayTransactionTest, onInitializeDisplaysSetsUpPrimaryDisplay) {
    using Case = SimplePrimaryDisplayCase;

    // --------------------------------------------------------------------
    // Preconditions

    // A primary display is set up
    Case::Display::injectHwcDisplay(this);
    auto primaryDisplay = Case::Display::makeFakeExistingDisplayInjector(this);
    primaryDisplay.inject();

    // --------------------------------------------------------------------
    // Call Expectations

    // We expect the surface interceptor to possibly be used, but we treat it as
    // disabled since it is called as a side effect rather than directly by this
    // function.
    EXPECT_CALL(*mSurfaceInterceptor, isEnabled()).WillOnce(Return(false));

    // We expect a call to get the active display config.
    Case::Display::setupHwcGetActiveConfigCallExpectations(this);

    // We expect invalidate() to be invoked once to trigger display transaction
    // processing.
    EXPECT_CALL(*mMessageQueue, invalidate()).Times(1);

    EXPECT_CALL(*mPrimaryDispSync, expectedPresentTime()).WillRepeatedly(Return(0));

    // --------------------------------------------------------------------
    // Invocation

    mFlinger.onInitializeDisplays();

    // --------------------------------------------------------------------
    // Postconditions

    // The primary display should have a current state
    ASSERT_TRUE(hasCurrentDisplayState(primaryDisplay.token()));
    const auto& primaryDisplayState = getCurrentDisplayState(primaryDisplay.token());
    // The layer stack state should be set to zero
    EXPECT_EQ(0u, primaryDisplayState.layerStack);
    // The orientation state should be set to zero
    EXPECT_EQ(0, primaryDisplayState.orientation);

    // The frame state should be set to INVALID
    EXPECT_EQ(Rect::INVALID_RECT, primaryDisplayState.frame);

    // The viewport state should be set to INVALID
    EXPECT_EQ(Rect::INVALID_RECT, primaryDisplayState.viewport);

    // The width and height should both be zero
    EXPECT_EQ(0u, primaryDisplayState.width);
    EXPECT_EQ(0u, primaryDisplayState.height);

    // The display should be set to HWC_POWER_MODE_NORMAL
    ASSERT_TRUE(hasDisplayDevice(primaryDisplay.token()));
    auto displayDevice = primaryDisplay.mutableDisplayDevice();
    EXPECT_EQ(HWC_POWER_MODE_NORMAL, displayDevice->getPowerMode());

    // The display refresh period should be set in the frame tracker.
    FrameStats stats;
    mFlinger.getAnimFrameTracker().getStats(&stats);
    EXPECT_EQ(DEFAULT_REFRESH_RATE, stats.refreshPeriodNano);

    // The display transaction needed flag should be set.
    EXPECT_TRUE(hasTransactionFlagSet(eDisplayTransactionNeeded));

    // The compositor timing should be set to default values
    const auto& compositorTiming = mFlinger.getCompositorTiming();
    EXPECT_EQ(-DEFAULT_REFRESH_RATE, compositorTiming.deadline);
    EXPECT_EQ(DEFAULT_REFRESH_RATE, compositorTiming.interval);
    EXPECT_EQ(DEFAULT_REFRESH_RATE, compositorTiming.presentLatency);
}

/* ------------------------------------------------------------------------
 * SurfaceFlinger::setPowerModeInternal
 */

// Used when we simulate a display that supports doze.
template <typename Display>
struct DozeIsSupportedVariant {
    static constexpr bool DOZE_SUPPORTED = true;
    static constexpr IComposerClient::PowerMode ACTUAL_POWER_MODE_FOR_DOZE =
            IComposerClient::PowerMode::DOZE;
    static constexpr IComposerClient::PowerMode ACTUAL_POWER_MODE_FOR_DOZE_SUSPEND =
            IComposerClient::PowerMode::DOZE_SUSPEND;

    static void setupComposerCallExpectations(DisplayTransactionTest* test) {
        EXPECT_CALL(*test->mComposer, getDisplayCapabilities(Display::HWC_DISPLAY_ID, _))
                .WillOnce(DoAll(SetArgPointee<1>(std::vector<Hwc2::DisplayCapability>(
                                        {Hwc2::DisplayCapability::DOZE})),
                                Return(Error::NONE)));
    }
};

template <typename Display>
// Used when we simulate a display that does not support doze.
struct DozeNotSupportedVariant {
    static constexpr bool DOZE_SUPPORTED = false;
    static constexpr IComposerClient::PowerMode ACTUAL_POWER_MODE_FOR_DOZE =
            IComposerClient::PowerMode::ON;
    static constexpr IComposerClient::PowerMode ACTUAL_POWER_MODE_FOR_DOZE_SUSPEND =
            IComposerClient::PowerMode::ON;

    static void setupComposerCallExpectations(DisplayTransactionTest* test) {
        EXPECT_CALL(*test->mComposer, getDisplayCapabilities(Display::HWC_DISPLAY_ID, _))
                .WillOnce(DoAll(SetArgPointee<1>(std::vector<Hwc2::DisplayCapability>({})),
                                Return(Error::NONE)));
    }
};

struct EventThreadBaseSupportedVariant {
    static void setupEventAndEventControlThreadNoCallExpectations(DisplayTransactionTest* test) {
        // The event control thread should not be notified.
        EXPECT_CALL(*test->mEventControlThread, setVsyncEnabled(_)).Times(0);

        // The event thread should not be notified.
        EXPECT_CALL(*test->mEventThread, onScreenReleased()).Times(0);
        EXPECT_CALL(*test->mEventThread, onScreenAcquired()).Times(0);
    }
};

struct EventThreadNotSupportedVariant : public EventThreadBaseSupportedVariant {
    static void setupAcquireAndEnableVsyncCallExpectations(DisplayTransactionTest* test) {
        // These calls are only expected for the primary display.

        // Instead expect no calls.
        setupEventAndEventControlThreadNoCallExpectations(test);
    }

    static void setupReleaseAndDisableVsyncCallExpectations(DisplayTransactionTest* test) {
        // These calls are only expected for the primary display.

        // Instead expect no calls.
        setupEventAndEventControlThreadNoCallExpectations(test);
    }
};

struct EventThreadIsSupportedVariant : public EventThreadBaseSupportedVariant {
    static void setupAcquireAndEnableVsyncCallExpectations(DisplayTransactionTest* test) {
        // The event control thread should be notified to enable vsyncs
        EXPECT_CALL(*test->mEventControlThread, setVsyncEnabled(true)).Times(1);

        // The event thread should be notified that the screen was acquired.
        EXPECT_CALL(*test->mEventThread, onScreenAcquired()).Times(1);
    }

    static void setupReleaseAndDisableVsyncCallExpectations(DisplayTransactionTest* test) {
        // There should be a call to setVsyncEnabled(false)
        EXPECT_CALL(*test->mEventControlThread, setVsyncEnabled(false)).Times(1);

        // The event thread should not be notified that the screen was released.
        EXPECT_CALL(*test->mEventThread, onScreenReleased()).Times(1);
    }
};

struct DispSyncIsSupportedVariant {
    static void setupBeginResyncCallExpectations(DisplayTransactionTest* test) {
        EXPECT_CALL(*test->mPrimaryDispSync, setPeriod(DEFAULT_REFRESH_RATE)).Times(1);
        EXPECT_CALL(*test->mPrimaryDispSync, beginResync()).Times(1);
    }

    static void setupEndResyncCallExpectations(DisplayTransactionTest* test) {
        EXPECT_CALL(*test->mPrimaryDispSync, endResync()).Times(1);
    }
};

struct DispSyncNotSupportedVariant {
    static void setupBeginResyncCallExpectations(DisplayTransactionTest* /* test */) {}

    static void setupEndResyncCallExpectations(DisplayTransactionTest* /* test */) {}
};

// --------------------------------------------------------------------
// Note:
//
// There are a large number of transitions we could test, however we only test a
// selected subset which provides complete test coverage of the implementation.
// --------------------------------------------------------------------

template <int initialPowerMode, int targetPowerMode>
struct TransitionVariantCommon {
    static constexpr auto INITIAL_POWER_MODE = initialPowerMode;
    static constexpr auto TARGET_POWER_MODE = targetPowerMode;

    static void verifyPostconditions(DisplayTransactionTest*) {}
};

struct TransitionOffToOnVariant
      : public TransitionVariantCommon<HWC_POWER_MODE_OFF, HWC_POWER_MODE_NORMAL> {
    template <typename Case>
    static void setupCallExpectations(DisplayTransactionTest* test) {
        Case::setupComposerCallExpectations(test, IComposerClient::PowerMode::ON);
        Case::EventThread::setupAcquireAndEnableVsyncCallExpectations(test);
        Case::DispSync::setupBeginResyncCallExpectations(test);
        Case::setupRepaintEverythingCallExpectations(test);
    }

    static void verifyPostconditions(DisplayTransactionTest* test) {
        EXPECT_TRUE(test->mFlinger.getVisibleRegionsDirty());
        EXPECT_TRUE(test->mFlinger.getHasPoweredOff());
    }
};

struct TransitionOffToDozeSuspendVariant
      : public TransitionVariantCommon<HWC_POWER_MODE_OFF, HWC_POWER_MODE_DOZE_SUSPEND> {
    template <typename Case>
    static void setupCallExpectations(DisplayTransactionTest* test) {
        Case::setupComposerCallExpectations(test, Case::Doze::ACTUAL_POWER_MODE_FOR_DOZE_SUSPEND);
        Case::EventThread::setupEventAndEventControlThreadNoCallExpectations(test);
        Case::setupRepaintEverythingCallExpectations(test);
    }

    static void verifyPostconditions(DisplayTransactionTest* test) {
        EXPECT_TRUE(test->mFlinger.getVisibleRegionsDirty());
        EXPECT_TRUE(test->mFlinger.getHasPoweredOff());
    }
};

struct TransitionOnToOffVariant
      : public TransitionVariantCommon<HWC_POWER_MODE_NORMAL, HWC_POWER_MODE_OFF> {
    template <typename Case>
    static void setupCallExpectations(DisplayTransactionTest* test) {
        Case::EventThread::setupReleaseAndDisableVsyncCallExpectations(test);
        Case::DispSync::setupEndResyncCallExpectations(test);
        Case::setupComposerCallExpectations(test, IComposerClient::PowerMode::OFF);
    }

    static void verifyPostconditions(DisplayTransactionTest* test) {
        EXPECT_TRUE(test->mFlinger.getVisibleRegionsDirty());
    }
};

struct TransitionDozeSuspendToOffVariant
      : public TransitionVariantCommon<HWC_POWER_MODE_DOZE_SUSPEND, HWC_POWER_MODE_OFF> {
    template <typename Case>
    static void setupCallExpectations(DisplayTransactionTest* test) {
        Case::EventThread::setupEventAndEventControlThreadNoCallExpectations(test);
        Case::setupComposerCallExpectations(test, IComposerClient::PowerMode::OFF);
    }

    static void verifyPostconditions(DisplayTransactionTest* test) {
        EXPECT_TRUE(test->mFlinger.getVisibleRegionsDirty());
    }
};

struct TransitionOnToDozeVariant
      : public TransitionVariantCommon<HWC_POWER_MODE_NORMAL, HWC_POWER_MODE_DOZE> {
    template <typename Case>
    static void setupCallExpectations(DisplayTransactionTest* test) {
        Case::EventThread::setupEventAndEventControlThreadNoCallExpectations(test);
        Case::setupComposerCallExpectations(test, Case::Doze::ACTUAL_POWER_MODE_FOR_DOZE);
    }
};

struct TransitionDozeSuspendToDozeVariant
      : public TransitionVariantCommon<HWC_POWER_MODE_DOZE_SUSPEND, HWC_POWER_MODE_DOZE> {
    template <typename Case>
    static void setupCallExpectations(DisplayTransactionTest* test) {
        Case::EventThread::setupAcquireAndEnableVsyncCallExpectations(test);
        Case::DispSync::setupBeginResyncCallExpectations(test);
        Case::setupComposerCallExpectations(test, Case::Doze::ACTUAL_POWER_MODE_FOR_DOZE);
    }
};

struct TransitionDozeToOnVariant
      : public TransitionVariantCommon<HWC_POWER_MODE_DOZE, HWC_POWER_MODE_NORMAL> {
    template <typename Case>
    static void setupCallExpectations(DisplayTransactionTest* test) {
        Case::EventThread::setupEventAndEventControlThreadNoCallExpectations(test);
        Case::setupComposerCallExpectations(test, IComposerClient::PowerMode::ON);
    }
};

struct TransitionDozeSuspendToOnVariant
      : public TransitionVariantCommon<HWC_POWER_MODE_DOZE_SUSPEND, HWC_POWER_MODE_NORMAL> {
    template <typename Case>
    static void setupCallExpectations(DisplayTransactionTest* test) {
        Case::EventThread::setupAcquireAndEnableVsyncCallExpectations(test);
        Case::DispSync::setupBeginResyncCallExpectations(test);
        Case::setupComposerCallExpectations(test, IComposerClient::PowerMode::ON);
    }
};

struct TransitionOnToDozeSuspendVariant
      : public TransitionVariantCommon<HWC_POWER_MODE_NORMAL, HWC_POWER_MODE_DOZE_SUSPEND> {
    template <typename Case>
    static void setupCallExpectations(DisplayTransactionTest* test) {
        Case::EventThread::setupReleaseAndDisableVsyncCallExpectations(test);
        Case::DispSync::setupEndResyncCallExpectations(test);
        Case::setupComposerCallExpectations(test, Case::Doze::ACTUAL_POWER_MODE_FOR_DOZE_SUSPEND);
    }
};

struct TransitionOnToUnknownVariant
      : public TransitionVariantCommon<HWC_POWER_MODE_NORMAL, HWC_POWER_MODE_LEET> {
    template <typename Case>
    static void setupCallExpectations(DisplayTransactionTest* test) {
        Case::EventThread::setupEventAndEventControlThreadNoCallExpectations(test);
        Case::setupNoComposerPowerModeCallExpectations(test);
    }
};

// --------------------------------------------------------------------
// Note:
//
// Rather than testing the cartesian product of of
// DozeIsSupported/DozeNotSupported with all other options, we use one for one
// display type, and the other for another display type.
// --------------------------------------------------------------------

template <typename DisplayVariant, typename DozeVariant, typename EventThreadVariant,
          typename DispSyncVariant, typename TransitionVariant>
struct DisplayPowerCase {
    using Display = DisplayVariant;
    using Doze = DozeVariant;
    using EventThread = EventThreadVariant;
    using DispSync = DispSyncVariant;
    using Transition = TransitionVariant;

    static auto injectDisplayWithInitialPowerMode(DisplayTransactionTest* test, int mode) {
        Display::injectHwcDisplayWithNoDefaultCapabilities(test);
        auto display = Display::makeFakeExistingDisplayInjector(test);
        display.inject();
        display.mutableDisplayDevice()->setPowerMode(mode);
        return display;
    }

    static void setInitialPrimaryHWVsyncEnabled(DisplayTransactionTest* test, bool enabled) {
        test->mScheduler->mutablePrimaryHWVsyncEnabled() = enabled;
    }

    static void setupRepaintEverythingCallExpectations(DisplayTransactionTest* test) {
        EXPECT_CALL(*test->mMessageQueue, invalidate()).Times(1);
    }

    static void setupSurfaceInterceptorCallExpectations(DisplayTransactionTest* test, int mode) {
        EXPECT_CALL(*test->mSurfaceInterceptor, isEnabled()).WillOnce(Return(true));
        EXPECT_CALL(*test->mSurfaceInterceptor, savePowerModeUpdate(_, mode)).Times(1);
    }

    static void setupComposerCallExpectations(DisplayTransactionTest* test,
                                              IComposerClient::PowerMode mode) {
        // Any calls to get the active config will return a default value.
        EXPECT_CALL(*test->mComposer, getActiveConfig(Display::HWC_DISPLAY_ID, _))
                .WillRepeatedly(DoAll(SetArgPointee<1>(Display::HWC_ACTIVE_CONFIG_ID),
                                      Return(Error::NONE)));

        // Any calls to get whether the display supports dozing will return the value set by the
        // policy variant.
        EXPECT_CALL(*test->mComposer, getDozeSupport(Display::HWC_DISPLAY_ID, _))
                .WillRepeatedly(DoAll(SetArgPointee<1>(Doze::DOZE_SUPPORTED), Return(Error::NONE)));

        EXPECT_CALL(*test->mComposer, setPowerMode(Display::HWC_DISPLAY_ID, mode)).Times(1);
    }

    static void setupNoComposerPowerModeCallExpectations(DisplayTransactionTest* test) {
        EXPECT_CALL(*test->mComposer, setPowerMode(Display::HWC_DISPLAY_ID, _)).Times(0);
    }
};

// A sample configuration for the primary display.
// In addition to having event thread support, we emulate doze support.
template <typename TransitionVariant>
using PrimaryDisplayPowerCase =
        DisplayPowerCase<PrimaryDisplayVariant, DozeIsSupportedVariant<PrimaryDisplayVariant>,
                         EventThreadIsSupportedVariant, DispSyncIsSupportedVariant,
                         TransitionVariant>;

// A sample configuration for the external display.
// In addition to not having event thread support, we emulate not having doze
// support.
template <typename TransitionVariant>
using ExternalDisplayPowerCase =
        DisplayPowerCase<ExternalDisplayVariant, DozeNotSupportedVariant<ExternalDisplayVariant>,
                         EventThreadNotSupportedVariant, DispSyncNotSupportedVariant,
                         TransitionVariant>;

class SetPowerModeInternalTest : public DisplayTransactionTest {
public:
    template <typename Case>
    void transitionDisplayCommon();
};

template <int PowerMode>
struct PowerModeInitialVSyncEnabled : public std::false_type {};

template <>
struct PowerModeInitialVSyncEnabled<HWC_POWER_MODE_NORMAL> : public std::true_type {};

template <>
struct PowerModeInitialVSyncEnabled<HWC_POWER_MODE_DOZE> : public std::true_type {};

template <typename Case>
void SetPowerModeInternalTest::transitionDisplayCommon() {
    // --------------------------------------------------------------------
    // Preconditions

    Case::Doze::setupComposerCallExpectations(this);
    auto display =
            Case::injectDisplayWithInitialPowerMode(this, Case::Transition::INITIAL_POWER_MODE);
    Case::setInitialPrimaryHWVsyncEnabled(this,
                                          PowerModeInitialVSyncEnabled<
                                                  Case::Transition::INITIAL_POWER_MODE>::value);

    // --------------------------------------------------------------------
    // Call Expectations

    Case::setupSurfaceInterceptorCallExpectations(this, Case::Transition::TARGET_POWER_MODE);
    Case::Transition::template setupCallExpectations<Case>(this);

    // --------------------------------------------------------------------
    // Invocation

    mFlinger.setPowerModeInternal(display.mutableDisplayDevice(),
                                  Case::Transition::TARGET_POWER_MODE);

    // --------------------------------------------------------------------
    // Postconditions

    Case::Transition::verifyPostconditions(this);
}

TEST_F(SetPowerModeInternalTest, setPowerModeInternalDoesNothingIfNoChange) {
    using Case = SimplePrimaryDisplayCase;

    // --------------------------------------------------------------------
    // Preconditions

    // A primary display device is set up
    Case::Display::injectHwcDisplay(this);
    auto display = Case::Display::makeFakeExistingDisplayInjector(this);
    display.inject();

    // The display is already set to HWC_POWER_MODE_NORMAL
    display.mutableDisplayDevice()->setPowerMode(HWC_POWER_MODE_NORMAL);

    // --------------------------------------------------------------------
    // Invocation

    mFlinger.setPowerModeInternal(display.mutableDisplayDevice(), HWC_POWER_MODE_NORMAL);

    // --------------------------------------------------------------------
    // Postconditions

    EXPECT_EQ(HWC_POWER_MODE_NORMAL, display.mutableDisplayDevice()->getPowerMode());
}

TEST_F(SetPowerModeInternalTest, setPowerModeInternalDoesNothingIfVirtualDisplay) {
    using Case = HwcVirtualDisplayCase;

    // --------------------------------------------------------------------
    // Preconditions

    // Insert display data so that the HWC thinks it created the virtual display.
    const auto displayId = Case::Display::DISPLAY_ID::get();
    ASSERT_TRUE(displayId);
    mFlinger.mutableHwcDisplayData().try_emplace(*displayId);

    // A virtual display device is set up
    Case::Display::injectHwcDisplay(this);
    auto display = Case::Display::makeFakeExistingDisplayInjector(this);
    display.inject();

    // The display is set to HWC_POWER_MODE_NORMAL
    getDisplayDevice(display.token())->setPowerMode(HWC_POWER_MODE_NORMAL);

    // --------------------------------------------------------------------
    // Invocation

    mFlinger.setPowerModeInternal(display.mutableDisplayDevice(), HWC_POWER_MODE_OFF);

    // --------------------------------------------------------------------
    // Postconditions

    EXPECT_EQ(HWC_POWER_MODE_NORMAL, display.mutableDisplayDevice()->getPowerMode());
}

TEST_F(SetPowerModeInternalTest, transitionsDisplayFromOffToOnPrimaryDisplay) {
    transitionDisplayCommon<PrimaryDisplayPowerCase<TransitionOffToOnVariant>>();
}

TEST_F(SetPowerModeInternalTest, transitionsDisplayFromOffToDozeSuspendPrimaryDisplay) {
    transitionDisplayCommon<PrimaryDisplayPowerCase<TransitionOffToDozeSuspendVariant>>();
}

TEST_F(SetPowerModeInternalTest, transitionsDisplayFromOnToOffPrimaryDisplay) {
    transitionDisplayCommon<PrimaryDisplayPowerCase<TransitionOnToOffVariant>>();
}

TEST_F(SetPowerModeInternalTest, transitionsDisplayFromDozeSuspendToOffPrimaryDisplay) {
    transitionDisplayCommon<PrimaryDisplayPowerCase<TransitionDozeSuspendToOffVariant>>();
}

TEST_F(SetPowerModeInternalTest, transitionsDisplayFromOnToDozePrimaryDisplay) {
    transitionDisplayCommon<PrimaryDisplayPowerCase<TransitionOnToDozeVariant>>();
}

TEST_F(SetPowerModeInternalTest, transitionsDisplayFromDozeSuspendToDozePrimaryDisplay) {
    transitionDisplayCommon<PrimaryDisplayPowerCase<TransitionDozeSuspendToDozeVariant>>();
}

TEST_F(SetPowerModeInternalTest, transitionsDisplayFromDozeToOnPrimaryDisplay) {
    transitionDisplayCommon<PrimaryDisplayPowerCase<TransitionDozeToOnVariant>>();
}

TEST_F(SetPowerModeInternalTest, transitionsDisplayFromDozeSuspendToOnPrimaryDisplay) {
    transitionDisplayCommon<PrimaryDisplayPowerCase<TransitionDozeSuspendToOnVariant>>();
}

TEST_F(SetPowerModeInternalTest, transitionsDisplayFromOnToDozeSuspendPrimaryDisplay) {
    transitionDisplayCommon<PrimaryDisplayPowerCase<TransitionOnToDozeSuspendVariant>>();
}

TEST_F(SetPowerModeInternalTest, transitionsDisplayFromOnToUnknownPrimaryDisplay) {
    transitionDisplayCommon<PrimaryDisplayPowerCase<TransitionOnToUnknownVariant>>();
}

TEST_F(SetPowerModeInternalTest, transitionsDisplayFromOffToOnExternalDisplay) {
    transitionDisplayCommon<ExternalDisplayPowerCase<TransitionOffToOnVariant>>();
}

TEST_F(SetPowerModeInternalTest, transitionsDisplayFromOffToDozeSuspendExternalDisplay) {
    transitionDisplayCommon<ExternalDisplayPowerCase<TransitionOffToDozeSuspendVariant>>();
}

TEST_F(SetPowerModeInternalTest, transitionsDisplayFromOnToOffExternalDisplay) {
    transitionDisplayCommon<ExternalDisplayPowerCase<TransitionOnToOffVariant>>();
}

TEST_F(SetPowerModeInternalTest, transitionsDisplayFromDozeSuspendToOffExternalDisplay) {
    transitionDisplayCommon<ExternalDisplayPowerCase<TransitionDozeSuspendToOffVariant>>();
}

TEST_F(SetPowerModeInternalTest, transitionsDisplayFromOnToDozeExternalDisplay) {
    transitionDisplayCommon<ExternalDisplayPowerCase<TransitionOnToDozeVariant>>();
}

TEST_F(SetPowerModeInternalTest, transitionsDisplayFromDozeSuspendToDozeExternalDisplay) {
    transitionDisplayCommon<ExternalDisplayPowerCase<TransitionDozeSuspendToDozeVariant>>();
}

TEST_F(SetPowerModeInternalTest, transitionsDisplayFromDozeToOnExternalDisplay) {
    transitionDisplayCommon<ExternalDisplayPowerCase<TransitionDozeToOnVariant>>();
}

TEST_F(SetPowerModeInternalTest, transitionsDisplayFromDozeSuspendToOnExternalDisplay) {
    transitionDisplayCommon<ExternalDisplayPowerCase<TransitionDozeSuspendToOnVariant>>();
}

TEST_F(SetPowerModeInternalTest, transitionsDisplayFromOnToDozeSuspendExternalDisplay) {
    transitionDisplayCommon<ExternalDisplayPowerCase<TransitionOnToDozeSuspendVariant>>();
}

TEST_F(SetPowerModeInternalTest, transitionsDisplayFromOnToUnknownExternalDisplay) {
    transitionDisplayCommon<ExternalDisplayPowerCase<TransitionOnToUnknownVariant>>();
}

} // namespace
} // namespace android
