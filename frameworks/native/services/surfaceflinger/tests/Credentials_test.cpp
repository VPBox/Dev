#include <algorithm>
#include <functional>
#include <limits>
#include <ostream>

#include <gtest/gtest.h>

#include <gui/ISurfaceComposer.h>
#include <gui/LayerDebugInfo.h>
#include <gui/Surface.h>
#include <gui/SurfaceComposerClient.h>

#include <private/android_filesystem_config.h>
#include <private/gui/ComposerService.h>
#include <ui/DisplayInfo.h>
#include <utils/String8.h>

namespace android {

using Transaction = SurfaceComposerClient::Transaction;
using ui::ColorMode;

namespace {
const String8 DISPLAY_NAME("Credentials Display Test");
const String8 SURFACE_NAME("Test Surface Name");
const uint32_t ROTATION = 0;
const float FRAME_SCALE = 1.0f;
} // namespace

/**
 * This class tests the CheckCredentials method in SurfaceFlinger.
 * Methods like EnableVsyncInjections and InjectVsync are not tested since they do not
 * return anything meaningful.
 */
class CredentialsTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Start the tests as root.
        seteuid(AID_ROOT);

        ASSERT_NO_FATAL_FAILURE(initClient());
    }

    void TearDown() override {
        mComposerClient->dispose();
        mBGSurfaceControl.clear();
        mComposerClient.clear();
        // Finish the tests as root.
        seteuid(AID_ROOT);
    }

    sp<IBinder> mDisplay;
    sp<IBinder> mVirtualDisplay;
    sp<SurfaceComposerClient> mComposerClient;
    sp<SurfaceControl> mBGSurfaceControl;
    sp<SurfaceControl> mVirtualSurfaceControl;

    void initClient() {
        mComposerClient = new SurfaceComposerClient;
        ASSERT_EQ(NO_ERROR, mComposerClient->initCheck());
    }

    void setupBackgroundSurface() {
        mDisplay = SurfaceComposerClient::getInternalDisplayToken();
        ASSERT_FALSE(mDisplay == nullptr);

        DisplayInfo info;
        ASSERT_EQ(NO_ERROR, SurfaceComposerClient::getDisplayInfo(mDisplay, &info));
        const ssize_t displayWidth = info.w;
        const ssize_t displayHeight = info.h;

        // Background surface
        mBGSurfaceControl =
                mComposerClient->createSurface(SURFACE_NAME, displayWidth, displayHeight,
                                               PIXEL_FORMAT_RGBA_8888, 0);
        ASSERT_TRUE(mBGSurfaceControl != nullptr);
        ASSERT_TRUE(mBGSurfaceControl->isValid());

        Transaction t;
        t.setDisplayLayerStack(mDisplay, 0);
        ASSERT_EQ(NO_ERROR,
                  t.setLayer(mBGSurfaceControl, INT_MAX - 3).show(mBGSurfaceControl).apply());
    }

    void setupVirtualDisplay() {
        mVirtualDisplay = SurfaceComposerClient::createDisplay(DISPLAY_NAME, true);
        const ssize_t displayWidth = 100;
        const ssize_t displayHeight = 100;

        // Background surface
        mVirtualSurfaceControl =
                mComposerClient->createSurface(SURFACE_NAME, displayWidth, displayHeight,
                                               PIXEL_FORMAT_RGBA_8888, 0);
        ASSERT_TRUE(mVirtualSurfaceControl != nullptr);
        ASSERT_TRUE(mVirtualSurfaceControl->isValid());

        Transaction t;
        t.setDisplayLayerStack(mVirtualDisplay, 0);
        ASSERT_EQ(NO_ERROR,
                  t.setLayer(mVirtualSurfaceControl, INT_MAX - 3)
                          .show(mVirtualSurfaceControl)
                          .apply());
    }

    /**
     * Sets UID to imitate Graphic's process.
     */
    void setGraphicsUID() {
        seteuid(AID_ROOT);
        seteuid(AID_GRAPHICS);
    }

    /**
     * Sets UID to imitate System's process.
     */
    void setSystemUID() {
        seteuid(AID_ROOT);
        seteuid(AID_SYSTEM);
    }

    /**
     * Sets UID to imitate a process that doesn't have any special privileges in
     * our code.
     */
    void setBinUID() {
        seteuid(AID_ROOT);
        seteuid(AID_BIN);
    }

    /**
     * Template function the check a condition for different types of users: root
     * graphics, system, and non-supported user. Root, graphics, and system should
     * always equal privilegedValue, and non-supported user should equal unprivilegedValue.
     */
    template <typename T>
    void checkWithPrivileges(std::function<T()> condition, T privilegedValue, T unprivilegedValue) {
        // Check with root.
        seteuid(AID_ROOT);
        ASSERT_EQ(privilegedValue, condition());

        // Check as a Graphics user.
        setGraphicsUID();
        ASSERT_EQ(privilegedValue, condition());

        // Check as a system user.
        setSystemUID();
        ASSERT_EQ(privilegedValue, condition());

        // Check as a non-supported user.
        setBinUID();
        ASSERT_EQ(unprivilegedValue, condition());
    }
};

TEST_F(CredentialsTest, ClientInitTest) {
    // Root can init can init the client.
    ASSERT_NO_FATAL_FAILURE(initClient());

    // Graphics can init the client.
    setGraphicsUID();
    ASSERT_NO_FATAL_FAILURE(initClient());

    // System can init the client.
    setSystemUID();
    ASSERT_NO_FATAL_FAILURE(initClient());

    // Anyone else can init the client.
    setBinUID();
    mComposerClient = new SurfaceComposerClient;
    ASSERT_NO_FATAL_FAILURE(initClient());
}

TEST_F(CredentialsTest, GetBuiltInDisplayAccessTest) {
    std::function<bool()> condition = [] {
        return SurfaceComposerClient::getInternalDisplayToken() != nullptr;
    };
    // Anyone can access display information.
    ASSERT_NO_FATAL_FAILURE(checkWithPrivileges(condition, true, true));
}

TEST_F(CredentialsTest, AllowedGetterMethodsTest) {
    // The following methods are tested with a UID that is not root, graphics,
    // or system, to show that anyone can access them.
    setBinUID();
    const auto display = SurfaceComposerClient::getInternalDisplayToken();
    ASSERT_TRUE(display != nullptr);

    DisplayInfo info;
    ASSERT_EQ(NO_ERROR, SurfaceComposerClient::getDisplayInfo(display, &info));

    Vector<DisplayInfo> configs;
    ASSERT_EQ(NO_ERROR, SurfaceComposerClient::getDisplayConfigs(display, &configs));

    ASSERT_EQ(NO_ERROR, SurfaceComposerClient::getActiveConfig(display));

    ASSERT_NE(static_cast<ui::ColorMode>(BAD_VALUE),
              SurfaceComposerClient::getActiveColorMode(display));
}

TEST_F(CredentialsTest, GetDisplayColorModesTest) {
    const auto display = SurfaceComposerClient::getInternalDisplayToken();
    std::function<status_t()> condition = [=]() {
        Vector<ui::ColorMode> outColorModes;
        return SurfaceComposerClient::getDisplayColorModes(display, &outColorModes);
    };
    ASSERT_NO_FATAL_FAILURE(checkWithPrivileges<status_t>(condition, NO_ERROR, NO_ERROR));
}

TEST_F(CredentialsTest, GetDisplayNativePrimariesTest) {
    const auto display = SurfaceComposerClient::getInternalDisplayToken();
    std::function<status_t()> condition = [=]() {
        ui::DisplayPrimaries primaries;
        return SurfaceComposerClient::getDisplayNativePrimaries(display, primaries);
    };
    ASSERT_NO_FATAL_FAILURE(checkWithPrivileges<status_t>(condition, NO_ERROR, NO_ERROR));
}

TEST_F(CredentialsTest, SetActiveConfigTest) {
    const auto display = SurfaceComposerClient::getInternalDisplayToken();
    std::function<status_t()> condition = [=]() {
        return SurfaceComposerClient::setActiveConfig(display, 0);
    };
    ASSERT_NO_FATAL_FAILURE(checkWithPrivileges<status_t>(condition, NO_ERROR, PERMISSION_DENIED));
}

TEST_F(CredentialsTest, SetActiveColorModeTest) {
    const auto display = SurfaceComposerClient::getInternalDisplayToken();
    std::function<status_t()> condition = [=]() {
        return SurfaceComposerClient::setActiveColorMode(display, ui::ColorMode::NATIVE);
    };
    ASSERT_NO_FATAL_FAILURE(checkWithPrivileges<status_t>(condition, NO_ERROR, PERMISSION_DENIED));
}

TEST_F(CredentialsTest, CreateDisplayTest) {
    std::function<bool()> condition = [=]() {
        sp<IBinder> testDisplay = SurfaceComposerClient::createDisplay(DISPLAY_NAME, true);
        return testDisplay.get() != nullptr;
    };
    ASSERT_NO_FATAL_FAILURE(checkWithPrivileges(condition, true, false));

    condition = [=]() {
        sp<IBinder> testDisplay = SurfaceComposerClient::createDisplay(DISPLAY_NAME, false);
        return testDisplay.get() != nullptr;
    };
    ASSERT_NO_FATAL_FAILURE(checkWithPrivileges(condition, true, false));
}

TEST_F(CredentialsTest, DISABLED_DestroyDisplayTest) {
    setupVirtualDisplay();

    DisplayInfo info;
    ASSERT_EQ(NO_ERROR, SurfaceComposerClient::getDisplayInfo(mVirtualDisplay, &info));
    SurfaceComposerClient::destroyDisplay(mVirtualDisplay);
    // This test currently fails. TODO(b/112002626): Find a way to properly create
    // a display in the test environment, so that destroy display can remove it.
    ASSERT_EQ(NAME_NOT_FOUND, SurfaceComposerClient::getDisplayInfo(mVirtualDisplay, &info));
}

TEST_F(CredentialsTest, CaptureTest) {
    const auto display = SurfaceComposerClient::getInternalDisplayToken();
    std::function<status_t()> condition = [=]() {
        sp<GraphicBuffer> outBuffer;
        return ScreenshotClient::capture(display, ui::Dataspace::V0_SRGB,
                                         ui::PixelFormat::RGBA_8888, Rect(), 0 /*reqWidth*/,
                                         0 /*reqHeight*/, false, ROTATION, &outBuffer);
    };
    ASSERT_NO_FATAL_FAILURE(checkWithPrivileges<status_t>(condition, NO_ERROR, PERMISSION_DENIED));
}

TEST_F(CredentialsTest, CaptureLayersTest) {
    setupBackgroundSurface();
    sp<GraphicBuffer> outBuffer;
    std::function<status_t()> condition = [=]() {
        sp<GraphicBuffer> outBuffer;
        return ScreenshotClient::captureLayers(mBGSurfaceControl->getHandle(),
                                               ui::Dataspace::V0_SRGB, ui::PixelFormat::RGBA_8888,
                                               Rect(), FRAME_SCALE, &outBuffer);
    };
    ASSERT_NO_FATAL_FAILURE(checkWithPrivileges<status_t>(condition, NO_ERROR, PERMISSION_DENIED));
}

/**
 * The following tests are for methods accessible directly through SurfaceFlinger.
 */

/**
 * An app can pass a buffer queue to the media server and ask the media server to decode a DRM video
 * to that buffer queue. The media server is the buffer producer in this case. Because the app may create
 * its own buffer queue and act as the buffer consumer, the media server wants to be careful to avoid
 * sending decoded video frames to the app. This is where authenticateSurfaceTexture call comes in, to check
 * the consumer of a buffer queue is SurfaceFlinger.
 */
TEST_F(CredentialsTest, AuthenticateSurfaceTextureTest) {
    setupBackgroundSurface();
    sp<IGraphicBufferProducer> producer =
            mBGSurfaceControl->getSurface()->getIGraphicBufferProducer();
    sp<ISurfaceComposer> sf(ComposerService::getComposerService());

    std::function<bool()> condition = [=]() { return sf->authenticateSurfaceTexture(producer); };
    // Anyone should be able to check if the consumer of the buffer queue is SF.
    ASSERT_NO_FATAL_FAILURE(checkWithPrivileges(condition, true, true));
}

TEST_F(CredentialsTest, GetLayerDebugInfo) {
    setupBackgroundSurface();
    sp<ISurfaceComposer> sf(ComposerService::getComposerService());

    // Historically, only root and shell can access the getLayerDebugInfo which
    // is called when we call dumpsys. I don't see a reason why we should change this.
    std::vector<LayerDebugInfo> outLayers;
    // Check with root.
    seteuid(AID_ROOT);
    ASSERT_EQ(NO_ERROR, sf->getLayerDebugInfo(&outLayers));

    // Check as a shell.
    seteuid(AID_SHELL);
    ASSERT_EQ(NO_ERROR, sf->getLayerDebugInfo(&outLayers));

    // Check as anyone else.
    seteuid(AID_ROOT);
    seteuid(AID_BIN);
    ASSERT_EQ(PERMISSION_DENIED, sf->getLayerDebugInfo(&outLayers));
}

TEST_F(CredentialsTest, IsWideColorDisplayBasicCorrectness) {
    const auto display = SurfaceComposerClient::getInternalDisplayToken();
    ASSERT_FALSE(display == nullptr);
    bool result = false;
    status_t error = SurfaceComposerClient::isWideColorDisplay(display, &result);
    ASSERT_EQ(NO_ERROR, error);
    bool hasWideColorMode = false;
    Vector<ColorMode> colorModes;
    SurfaceComposerClient::getDisplayColorModes(display, &colorModes);
    for (ColorMode colorMode : colorModes) {
        switch (colorMode) {
            case ColorMode::DISPLAY_P3:
            case ColorMode::ADOBE_RGB:
            case ColorMode::DCI_P3:
                hasWideColorMode = true;
                break;
            default:
                break;
        }
    }
    ASSERT_EQ(hasWideColorMode, result);
}

TEST_F(CredentialsTest, IsWideColorDisplayWithPrivileges) {
    const auto display = SurfaceComposerClient::getInternalDisplayToken();
    ASSERT_FALSE(display == nullptr);
    std::function<status_t()> condition = [=]() {
        bool result = false;
        return SurfaceComposerClient::isWideColorDisplay(display, &result);
    };
    ASSERT_NO_FATAL_FAILURE(checkWithPrivileges<status_t>(condition, NO_ERROR, NO_ERROR));
}

TEST_F(CredentialsTest, GetActiveColorModeBasicCorrectness) {
    const auto display = SurfaceComposerClient::getInternalDisplayToken();
    ASSERT_FALSE(display == nullptr);
    ColorMode colorMode = SurfaceComposerClient::getActiveColorMode(display);
    ASSERT_NE(static_cast<ColorMode>(BAD_VALUE), colorMode);
}

} // namespace android
