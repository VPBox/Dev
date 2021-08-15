/*
 * Copyright (C) 2011 The Android Open Source Project
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

#include <algorithm>
#include <chrono>
#include <cinttypes>
#include <functional>
#include <limits>
#include <ostream>
#include <thread>

#include <gtest/gtest.h>

#include <android/native_window.h>

#include <binder/ProcessState.h>
#include <gui/BufferItemConsumer.h>
#include <gui/IProducerListener.h>
#include <gui/ISurfaceComposer.h>
#include <gui/LayerState.h>
#include <gui/Surface.h>
#include <gui/SurfaceComposerClient.h>
#include <hardware/hwcomposer_defs.h>
#include <private/android_filesystem_config.h>
#include <private/gui/ComposerService.h>

#include <ui/ColorSpace.h>
#include <ui/DisplayInfo.h>
#include <ui/Rect.h>
#include <utils/String8.h>

#include <math.h>
#include <math/vec3.h>
#include <sys/types.h>
#include <unistd.h>

#include "BufferGenerator.h"

namespace android {

namespace {

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    static const Color RED;
    static const Color GREEN;
    static const Color BLUE;
    static const Color WHITE;
    static const Color BLACK;
    static const Color TRANSPARENT;
};

const Color Color::RED{255, 0, 0, 255};
const Color Color::GREEN{0, 255, 0, 255};
const Color Color::BLUE{0, 0, 255, 255};
const Color Color::WHITE{255, 255, 255, 255};
const Color Color::BLACK{0, 0, 0, 255};
const Color Color::TRANSPARENT{0, 0, 0, 0};

using android::hardware::graphics::common::V1_1::BufferUsage;
using namespace std::chrono_literals;

std::ostream& operator<<(std::ostream& os, const Color& color) {
    os << int(color.r) << ", " << int(color.g) << ", " << int(color.b) << ", " << int(color.a);
    return os;
}

// Fill a region with the specified color.
void fillANativeWindowBufferColor(const ANativeWindow_Buffer& buffer, const Rect& rect,
                                  const Color& color) {
    Rect r(0, 0, buffer.width, buffer.height);
    if (!r.intersect(rect, &r)) {
        return;
    }

    int32_t width = r.right - r.left;
    int32_t height = r.bottom - r.top;

    for (int32_t row = 0; row < height; row++) {
        uint8_t* dst =
                static_cast<uint8_t*>(buffer.bits) + (buffer.stride * (r.top + row) + r.left) * 4;
        for (int32_t column = 0; column < width; column++) {
            dst[0] = color.r;
            dst[1] = color.g;
            dst[2] = color.b;
            dst[3] = color.a;
            dst += 4;
        }
    }
}

// Fill a region with the specified color.
void fillGraphicBufferColor(const sp<GraphicBuffer>& buffer, const Rect& rect, const Color& color) {
    Rect r(0, 0, buffer->width, buffer->height);
    if (!r.intersect(rect, &r)) {
        return;
    }

    int32_t width = r.right - r.left;
    int32_t height = r.bottom - r.top;

    uint8_t* pixels;
    buffer->lock(GRALLOC_USAGE_SW_READ_OFTEN | GRALLOC_USAGE_SW_WRITE_OFTEN,
                 reinterpret_cast<void**>(&pixels));

    for (int32_t row = 0; row < height; row++) {
        uint8_t* dst = pixels + (buffer->getStride() * (r.top + row) + r.left) * 4;
        for (int32_t column = 0; column < width; column++) {
            dst[0] = color.r;
            dst[1] = color.g;
            dst[2] = color.b;
            dst[3] = color.a;
            dst += 4;
        }
    }
    buffer->unlock();
}

// Check if a region has the specified color.
void expectBufferColor(const sp<GraphicBuffer>& outBuffer, uint8_t* pixels, const Rect& rect,
                       const Color& color, uint8_t tolerance) {
    int32_t x = rect.left;
    int32_t y = rect.top;
    int32_t width = rect.right - rect.left;
    int32_t height = rect.bottom - rect.top;

    int32_t bufferWidth = int32_t(outBuffer->getWidth());
    int32_t bufferHeight = int32_t(outBuffer->getHeight());
    if (x + width > bufferWidth) {
        x = std::min(x, bufferWidth);
        width = bufferWidth - x;
    }
    if (y + height > bufferHeight) {
        y = std::min(y, bufferHeight);
        height = bufferHeight - y;
    }

    auto colorCompare = [tolerance](uint8_t a, uint8_t b) {
        uint8_t tmp = a >= b ? a - b : b - a;
        return tmp <= tolerance;
    };
    for (int32_t j = 0; j < height; j++) {
        const uint8_t* src = pixels + (outBuffer->getStride() * (y + j) + x) * 4;
        for (int32_t i = 0; i < width; i++) {
            const uint8_t expected[4] = {color.r, color.g, color.b, color.a};
            EXPECT_TRUE(std::equal(src, src + 4, expected, colorCompare))
                    << "pixel @ (" << x + i << ", " << y + j << "): "
                    << "expected (" << color << "), "
                    << "got (" << Color{src[0], src[1], src[2], src[3]} << ")";
            src += 4;
        }
    }
}

} // anonymous namespace

using Transaction = SurfaceComposerClient::Transaction;

// Fill an RGBA_8888 formatted surface with a single color.
static void fillSurfaceRGBA8(const sp<SurfaceControl>& sc, uint8_t r, uint8_t g, uint8_t b,
                             bool unlock = true) {
    ANativeWindow_Buffer outBuffer;
    sp<Surface> s = sc->getSurface();
    ASSERT_TRUE(s != nullptr);
    ASSERT_EQ(NO_ERROR, s->lock(&outBuffer, nullptr));
    uint8_t* img = reinterpret_cast<uint8_t*>(outBuffer.bits);
    for (int y = 0; y < outBuffer.height; y++) {
        for (int x = 0; x < outBuffer.width; x++) {
            uint8_t* pixel = img + (4 * (y * outBuffer.stride + x));
            pixel[0] = r;
            pixel[1] = g;
            pixel[2] = b;
            pixel[3] = 255;
        }
    }
    if (unlock) {
        ASSERT_EQ(NO_ERROR, s->unlockAndPost());
    }
}

// A ScreenCapture is a screenshot from SurfaceFlinger that can be used to check
// individual pixel values for testing purposes.
class ScreenCapture : public RefBase {
public:
    static void captureScreen(std::unique_ptr<ScreenCapture>* sc) {
        captureScreen(sc, SurfaceComposerClient::getInternalDisplayToken());
    }

    static void captureScreen(std::unique_ptr<ScreenCapture>* sc, sp<IBinder> displayToken) {
        const auto sf = ComposerService::getComposerService();
        SurfaceComposerClient::Transaction().apply(true);

        sp<GraphicBuffer> outBuffer;
        ASSERT_EQ(NO_ERROR, sf->captureScreen(displayToken, &outBuffer, Rect(), 0, 0, false));
        *sc = std::make_unique<ScreenCapture>(outBuffer);
    }

    static void captureLayers(std::unique_ptr<ScreenCapture>* sc, sp<IBinder>& parentHandle,
                              Rect crop = Rect::EMPTY_RECT, float frameScale = 1.0) {
        sp<ISurfaceComposer> sf(ComposerService::getComposerService());
        SurfaceComposerClient::Transaction().apply(true);

        sp<GraphicBuffer> outBuffer;
        ASSERT_EQ(NO_ERROR, sf->captureLayers(parentHandle, &outBuffer, crop, frameScale));
        *sc = std::make_unique<ScreenCapture>(outBuffer);
    }

    static void captureChildLayers(std::unique_ptr<ScreenCapture>* sc, sp<IBinder>& parentHandle,
                                   Rect crop = Rect::EMPTY_RECT, float frameScale = 1.0) {
        sp<ISurfaceComposer> sf(ComposerService::getComposerService());
        SurfaceComposerClient::Transaction().apply(true);

        sp<GraphicBuffer> outBuffer;
        ASSERT_EQ(NO_ERROR, sf->captureLayers(parentHandle, &outBuffer, crop, frameScale, true));
        *sc = std::make_unique<ScreenCapture>(outBuffer);
    }

    static void captureChildLayersExcluding(
            std::unique_ptr<ScreenCapture>* sc, sp<IBinder>& parentHandle,
            std::unordered_set<sp<IBinder>, ISurfaceComposer::SpHash<IBinder>> excludeLayers) {
        sp<ISurfaceComposer> sf(ComposerService::getComposerService());
        SurfaceComposerClient::Transaction().apply(true);

        sp<GraphicBuffer> outBuffer;
        ASSERT_EQ(NO_ERROR,
                  sf->captureLayers(parentHandle, &outBuffer, ui::Dataspace::V0_SRGB,
                                    ui::PixelFormat::RGBA_8888, Rect::EMPTY_RECT, excludeLayers,
                                    1.0f, true));
        *sc = std::make_unique<ScreenCapture>(outBuffer);
    }

    void expectColor(const Rect& rect, const Color& color, uint8_t tolerance = 0) {
        ASSERT_EQ(HAL_PIXEL_FORMAT_RGBA_8888, mOutBuffer->getPixelFormat());
        expectBufferColor(mOutBuffer, mPixels, rect, color, tolerance);
    }

    void expectBorder(const Rect& rect, const Color& color, uint8_t tolerance = 0) {
        ASSERT_EQ(HAL_PIXEL_FORMAT_RGBA_8888, mOutBuffer->getPixelFormat());
        const bool leftBorder = rect.left > 0;
        const bool topBorder = rect.top > 0;
        const bool rightBorder = rect.right < int32_t(mOutBuffer->getWidth());
        const bool bottomBorder = rect.bottom < int32_t(mOutBuffer->getHeight());

        if (topBorder) {
            Rect top(rect.left, rect.top - 1, rect.right, rect.top);
            if (leftBorder) {
                top.left -= 1;
            }
            if (rightBorder) {
                top.right += 1;
            }
            expectColor(top, color, tolerance);
        }
        if (leftBorder) {
            Rect left(rect.left - 1, rect.top, rect.left, rect.bottom);
            expectColor(left, color, tolerance);
        }
        if (rightBorder) {
            Rect right(rect.right, rect.top, rect.right + 1, rect.bottom);
            expectColor(right, color, tolerance);
        }
        if (bottomBorder) {
            Rect bottom(rect.left, rect.bottom, rect.right, rect.bottom + 1);
            if (leftBorder) {
                bottom.left -= 1;
            }
            if (rightBorder) {
                bottom.right += 1;
            }
            expectColor(bottom, color, tolerance);
        }
    }

    void expectQuadrant(const Rect& rect, const Color& topLeft, const Color& topRight,
                        const Color& bottomLeft, const Color& bottomRight, bool filtered = false,
                        uint8_t tolerance = 0) {
        ASSERT_TRUE((rect.right - rect.left) % 2 == 0 && (rect.bottom - rect.top) % 2 == 0);

        const int32_t centerX = rect.left + (rect.right - rect.left) / 2;
        const int32_t centerY = rect.top + (rect.bottom - rect.top) / 2;
        // avoid checking borders due to unspecified filtering behavior
        const int32_t offsetX = filtered ? 2 : 0;
        const int32_t offsetY = filtered ? 2 : 0;
        expectColor(Rect(rect.left, rect.top, centerX - offsetX, centerY - offsetY), topLeft,
                    tolerance);
        expectColor(Rect(centerX + offsetX, rect.top, rect.right, centerY - offsetY), topRight,
                    tolerance);
        expectColor(Rect(rect.left, centerY + offsetY, centerX - offsetX, rect.bottom), bottomLeft,
                    tolerance);
        expectColor(Rect(centerX + offsetX, centerY + offsetY, rect.right, rect.bottom),
                    bottomRight, tolerance);
    }

    void checkPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b) {
        ASSERT_EQ(HAL_PIXEL_FORMAT_RGBA_8888, mOutBuffer->getPixelFormat());
        const uint8_t* pixel = mPixels + (4 * (y * mOutBuffer->getStride() + x));
        if (r != pixel[0] || g != pixel[1] || b != pixel[2]) {
            String8 err(String8::format("pixel @ (%3d, %3d): "
                                        "expected [%3d, %3d, %3d], got [%3d, %3d, %3d]",
                                        x, y, r, g, b, pixel[0], pixel[1], pixel[2]));
            EXPECT_EQ(String8(), err) << err.string();
        }
    }

    void expectFGColor(uint32_t x, uint32_t y) { checkPixel(x, y, 195, 63, 63); }

    void expectBGColor(uint32_t x, uint32_t y) { checkPixel(x, y, 63, 63, 195); }

    void expectChildColor(uint32_t x, uint32_t y) { checkPixel(x, y, 200, 200, 200); }

    explicit ScreenCapture(const sp<GraphicBuffer>& outBuffer) : mOutBuffer(outBuffer) {
        mOutBuffer->lock(GRALLOC_USAGE_SW_READ_OFTEN, reinterpret_cast<void**>(&mPixels));
    }

    ~ScreenCapture() { mOutBuffer->unlock(); }

private:
    sp<GraphicBuffer> mOutBuffer;
    uint8_t* mPixels = nullptr;
};

class LayerTransactionTest : public ::testing::Test {
protected:
    void SetUp() override {
        mClient = new SurfaceComposerClient;
        ASSERT_EQ(NO_ERROR, mClient->initCheck()) << "failed to create SurfaceComposerClient";

        ASSERT_NO_FATAL_FAILURE(SetUpDisplay());

        sp<ISurfaceComposer> sf(ComposerService::getComposerService());
        ASSERT_NO_FATAL_FAILURE(sf->getColorManagement(&mColorManagementUsed));
    }

    virtual void TearDown() {
        mBlackBgSurface = 0;
        mClient->dispose();
        mClient = 0;
    }

    virtual sp<SurfaceControl> createLayer(const sp<SurfaceComposerClient>& client,
                                           const char* name, uint32_t width, uint32_t height,
                                           uint32_t flags = 0, SurfaceControl* parent = nullptr) {
        auto layer =
                createSurface(client, name, width, height, PIXEL_FORMAT_RGBA_8888, flags, parent);

        Transaction t;
        t.setLayerStack(layer, mDisplayLayerStack).setLayer(layer, mLayerZBase);

        status_t error = t.apply();
        if (error != NO_ERROR) {
            ADD_FAILURE() << "failed to initialize SurfaceControl";
            layer.clear();
        }

        return layer;
    }

    virtual sp<SurfaceControl> createSurface(const sp<SurfaceComposerClient>& client,
                                             const char* name, uint32_t width, uint32_t height,
                                             PixelFormat format, uint32_t flags,
                                             SurfaceControl* parent = nullptr) {
        auto layer = client->createSurface(String8(name), width, height, format, flags, parent);
        EXPECT_NE(nullptr, layer.get()) << "failed to create SurfaceControl";
        return layer;
    }

    virtual sp<SurfaceControl> createLayer(const char* name, uint32_t width, uint32_t height,
                                           uint32_t flags = 0, SurfaceControl* parent = nullptr) {
        return createLayer(mClient, name, width, height, flags, parent);
    }

    sp<SurfaceControl> createColorLayer(const char* name, const Color& color,
                                        SurfaceControl* parent = nullptr) {
        auto colorLayer = createSurface(mClient, name, 0 /* buffer width */, 0 /* buffer height */,
                                        PIXEL_FORMAT_RGBA_8888,
                                        ISurfaceComposerClient::eFXSurfaceColor, parent);
        asTransaction([&](Transaction& t) {
            t.setColor(colorLayer, half3{color.r / 255.0f, color.g / 255.0f, color.b / 255.0f});
            t.setAlpha(colorLayer, color.a / 255.0f);
        });
        return colorLayer;
    }

    ANativeWindow_Buffer getBufferQueueLayerBuffer(const sp<SurfaceControl>& layer) {
        // wait for previous transactions (such as setSize) to complete
        Transaction().apply(true);

        ANativeWindow_Buffer buffer = {};
        EXPECT_EQ(NO_ERROR, layer->getSurface()->lock(&buffer, nullptr));

        return buffer;
    }

    void postBufferQueueLayerBuffer(const sp<SurfaceControl>& layer) {
        ASSERT_EQ(NO_ERROR, layer->getSurface()->unlockAndPost());

        // wait for the newly posted buffer to be latched
        waitForLayerBuffers();
    }

    virtual void fillBufferQueueLayerColor(const sp<SurfaceControl>& layer, const Color& color,
                                           int32_t bufferWidth, int32_t bufferHeight) {
        ANativeWindow_Buffer buffer;
        ASSERT_NO_FATAL_FAILURE(buffer = getBufferQueueLayerBuffer(layer));
        fillANativeWindowBufferColor(buffer, Rect(0, 0, bufferWidth, bufferHeight), color);
        postBufferQueueLayerBuffer(layer);
    }

    virtual void fillBufferStateLayerColor(const sp<SurfaceControl>& layer, const Color& color,
                                           int32_t bufferWidth, int32_t bufferHeight) {
        sp<GraphicBuffer> buffer =
                new GraphicBuffer(bufferWidth, bufferHeight, PIXEL_FORMAT_RGBA_8888, 1,
                                  BufferUsage::CPU_READ_OFTEN | BufferUsage::CPU_WRITE_OFTEN |
                                          BufferUsage::COMPOSER_OVERLAY,
                                  "test");
        fillGraphicBufferColor(buffer, Rect(0, 0, bufferWidth, bufferHeight), color);
        Transaction().setBuffer(layer, buffer).apply();
    }

    void fillLayerColor(uint32_t mLayerType, const sp<SurfaceControl>& layer, const Color& color,
                        int32_t bufferWidth, int32_t bufferHeight) {
        switch (mLayerType) {
            case ISurfaceComposerClient::eFXSurfaceBufferQueue:
                fillBufferQueueLayerColor(layer, color, bufferWidth, bufferHeight);
                break;
            case ISurfaceComposerClient::eFXSurfaceBufferState:
                fillBufferStateLayerColor(layer, color, bufferWidth, bufferHeight);
                break;
            default:
                ASSERT_TRUE(false) << "unsupported layer type: " << mLayerType;
        }
    }

    void fillLayerQuadrant(uint32_t mLayerType, const sp<SurfaceControl>& layer,
                           int32_t bufferWidth, int32_t bufferHeight, const Color& topLeft,
                           const Color& topRight, const Color& bottomLeft,
                           const Color& bottomRight) {
        switch (mLayerType) {
            case ISurfaceComposerClient::eFXSurfaceBufferQueue:
                fillBufferQueueLayerQuadrant(layer, bufferWidth, bufferHeight, topLeft, topRight,
                                             bottomLeft, bottomRight);
                break;
            case ISurfaceComposerClient::eFXSurfaceBufferState:
                fillBufferStateLayerQuadrant(layer, bufferWidth, bufferHeight, topLeft, topRight,
                                             bottomLeft, bottomRight);
                break;
            default:
                ASSERT_TRUE(false) << "unsupported layer type: " << mLayerType;
        }
    }

    virtual void fillBufferQueueLayerQuadrant(const sp<SurfaceControl>& layer, int32_t bufferWidth,
                                              int32_t bufferHeight, const Color& topLeft,
                                              const Color& topRight, const Color& bottomLeft,
                                              const Color& bottomRight) {
        ANativeWindow_Buffer buffer;
        ASSERT_NO_FATAL_FAILURE(buffer = getBufferQueueLayerBuffer(layer));
        ASSERT_TRUE(bufferWidth % 2 == 0 && bufferHeight % 2 == 0);

        const int32_t halfW = bufferWidth / 2;
        const int32_t halfH = bufferHeight / 2;
        fillANativeWindowBufferColor(buffer, Rect(0, 0, halfW, halfH), topLeft);
        fillANativeWindowBufferColor(buffer, Rect(halfW, 0, bufferWidth, halfH), topRight);
        fillANativeWindowBufferColor(buffer, Rect(0, halfH, halfW, bufferHeight), bottomLeft);
        fillANativeWindowBufferColor(buffer, Rect(halfW, halfH, bufferWidth, bufferHeight),
                                     bottomRight);

        postBufferQueueLayerBuffer(layer);
    }

    virtual void fillBufferStateLayerQuadrant(const sp<SurfaceControl>& layer, int32_t bufferWidth,
                                              int32_t bufferHeight, const Color& topLeft,
                                              const Color& topRight, const Color& bottomLeft,
                                              const Color& bottomRight) {
        sp<GraphicBuffer> buffer =
                new GraphicBuffer(bufferWidth, bufferHeight, PIXEL_FORMAT_RGBA_8888, 1,
                                  BufferUsage::CPU_READ_OFTEN | BufferUsage::CPU_WRITE_OFTEN |
                                          BufferUsage::COMPOSER_OVERLAY,
                                  "test");

        ASSERT_TRUE(bufferWidth % 2 == 0 && bufferHeight % 2 == 0);

        const int32_t halfW = bufferWidth / 2;
        const int32_t halfH = bufferHeight / 2;
        fillGraphicBufferColor(buffer, Rect(0, 0, halfW, halfH), topLeft);
        fillGraphicBufferColor(buffer, Rect(halfW, 0, bufferWidth, halfH), topRight);
        fillGraphicBufferColor(buffer, Rect(0, halfH, halfW, bufferHeight), bottomLeft);
        fillGraphicBufferColor(buffer, Rect(halfW, halfH, bufferWidth, bufferHeight), bottomRight);

        Transaction().setBuffer(layer, buffer).setSize(layer, bufferWidth, bufferHeight).apply();
    }

    std::unique_ptr<ScreenCapture> screenshot() {
        std::unique_ptr<ScreenCapture> screenshot;
        ScreenCapture::captureScreen(&screenshot);
        return screenshot;
    }

    void asTransaction(const std::function<void(Transaction&)>& exec) {
        Transaction t;
        exec(t);
        t.apply(true);
    }

    static status_t getBuffer(sp<GraphicBuffer>* outBuffer, sp<Fence>* outFence) {
        static BufferGenerator bufferGenerator;
        return bufferGenerator.get(outBuffer, outFence);
    }

    sp<SurfaceComposerClient> mClient;

    sp<IBinder> mDisplay;
    uint32_t mDisplayWidth;
    uint32_t mDisplayHeight;
    uint32_t mDisplayLayerStack;
    Rect mDisplayRect = Rect::INVALID_RECT;

    // leave room for ~256 layers
    const int32_t mLayerZBase = std::numeric_limits<int32_t>::max() - 256;

    sp<SurfaceControl> mBlackBgSurface;
    bool mColorManagementUsed;

private:
    void SetUpDisplay() {
        mDisplay = mClient->getInternalDisplayToken();
        ASSERT_FALSE(mDisplay == nullptr) << "failed to get display";

        // get display width/height
        DisplayInfo info;
        ASSERT_EQ(NO_ERROR, SurfaceComposerClient::getDisplayInfo(mDisplay, &info));
        mDisplayWidth = info.w;
        mDisplayHeight = info.h;
        mDisplayRect =
                Rect(static_cast<int32_t>(mDisplayWidth), static_cast<int32_t>(mDisplayHeight));

        // After a new buffer is queued, SurfaceFlinger is notified and will
        // latch the new buffer on next vsync.  Let's heuristically wait for 3
        // vsyncs.
        mBufferPostDelay = int32_t(1e6 / info.fps) * 3;

        mDisplayLayerStack = 0;

        mBlackBgSurface =
                createSurface(mClient, "BaseSurface", 0 /* buffer width */, 0 /* buffer height */,
                              PIXEL_FORMAT_RGBA_8888, ISurfaceComposerClient::eFXSurfaceColor);

        // set layer stack (b/68888219)
        Transaction t;
        t.setDisplayLayerStack(mDisplay, mDisplayLayerStack);
        t.setCrop_legacy(mBlackBgSurface, Rect(0, 0, mDisplayWidth, mDisplayHeight));
        t.setLayerStack(mBlackBgSurface, mDisplayLayerStack);
        t.setColor(mBlackBgSurface, half3{0, 0, 0});
        t.setLayer(mBlackBgSurface, mLayerZBase);
        t.apply();
    }

    void waitForLayerBuffers() {
        // Request an empty transaction to get applied synchronously to ensure the buffer is
        // latched.
        Transaction().apply(true);
        usleep(mBufferPostDelay);
    }

    int32_t mBufferPostDelay;

    friend class LayerRenderPathTestHarness;
};
enum class RenderPath { SCREENSHOT, VIRTUAL_DISPLAY };

class LayerRenderPathTestHarness {
public:
    LayerRenderPathTestHarness(LayerTransactionTest* delegate, RenderPath renderPath)
          : mDelegate(delegate), mRenderPath(renderPath) {}

    std::unique_ptr<ScreenCapture> getScreenCapture() {
        switch (mRenderPath) {
            case RenderPath::SCREENSHOT:
                return mDelegate->screenshot();
            case RenderPath::VIRTUAL_DISPLAY:

                const auto mainDisplay = SurfaceComposerClient::getInternalDisplayToken();
                DisplayInfo mainDisplayInfo;
                SurfaceComposerClient::getDisplayInfo(mainDisplay, &mainDisplayInfo);

                sp<IBinder> vDisplay;
                sp<IGraphicBufferProducer> producer;
                sp<IGraphicBufferConsumer> consumer;
                sp<BufferItemConsumer> itemConsumer;
                BufferQueue::createBufferQueue(&producer, &consumer);

                consumer->setConsumerName(String8("Virtual disp consumer"));
                consumer->setDefaultBufferSize(mainDisplayInfo.w, mainDisplayInfo.h);

                itemConsumer = new BufferItemConsumer(consumer,
                                                      // Sample usage bits from screenrecord
                                                      GRALLOC_USAGE_HW_VIDEO_ENCODER |
                                                              GRALLOC_USAGE_SW_READ_OFTEN);

                vDisplay = SurfaceComposerClient::createDisplay(String8("VirtualDisplay"),
                                                                false /*secure*/);

                SurfaceComposerClient::Transaction t;
                t.setDisplaySurface(vDisplay, producer);
                t.setDisplayLayerStack(vDisplay, 0);
                t.setDisplayProjection(vDisplay, mainDisplayInfo.orientation,
                                       Rect(mainDisplayInfo.viewportW, mainDisplayInfo.viewportH),
                                       Rect(mainDisplayInfo.w, mainDisplayInfo.h));
                t.apply();
                SurfaceComposerClient::Transaction().apply(true);
                BufferItem item;
                itemConsumer->acquireBuffer(&item, 0, true);
                auto sc = std::make_unique<ScreenCapture>(item.mGraphicBuffer);
                itemConsumer->releaseBuffer(item);
                SurfaceComposerClient::destroyDisplay(vDisplay);
                return sc;
        }
    }

protected:
    LayerTransactionTest* mDelegate;
    RenderPath mRenderPath;
};

class LayerTypeTransactionHarness : public LayerTransactionTest {
public:
    LayerTypeTransactionHarness(uint32_t layerType) : mLayerType(layerType) {}

    sp<SurfaceControl> createLayer(const char* name, uint32_t width, uint32_t height,
                                   uint32_t flags = 0, SurfaceControl* parent = nullptr) {
        // if the flags already have a layer type specified, return an error
        if (flags & ISurfaceComposerClient::eFXSurfaceMask) {
            return nullptr;
        }
        return LayerTransactionTest::createLayer(name, width, height, flags | mLayerType, parent);
    }

    void fillLayerColor(const sp<SurfaceControl>& layer, const Color& color, int32_t bufferWidth,
                        int32_t bufferHeight) {
        ASSERT_NO_FATAL_FAILURE(LayerTransactionTest::fillLayerColor(mLayerType, layer, color,
                                                                     bufferWidth, bufferHeight));
    }

    void fillLayerQuadrant(const sp<SurfaceControl>& layer, int32_t bufferWidth,
                           int32_t bufferHeight, const Color& topLeft, const Color& topRight,
                           const Color& bottomLeft, const Color& bottomRight) {
        ASSERT_NO_FATAL_FAILURE(LayerTransactionTest::fillLayerQuadrant(mLayerType, layer,
                                                                        bufferWidth, bufferHeight,
                                                                        topLeft, topRight,
                                                                        bottomLeft, bottomRight));
    }

protected:
    uint32_t mLayerType;
};

class LayerTypeTransactionTest : public LayerTypeTransactionHarness,
                                 public ::testing::WithParamInterface<uint32_t> {
public:
    LayerTypeTransactionTest() : LayerTypeTransactionHarness(GetParam()) {}
};

class LayerTypeAndRenderTypeTransactionTest
      : public LayerTypeTransactionHarness,
        public ::testing::WithParamInterface<std::tuple<uint32_t, RenderPath>> {
public:
    LayerTypeAndRenderTypeTransactionTest()
          : LayerTypeTransactionHarness(std::get<0>(GetParam())),
            mRenderPathHarness(LayerRenderPathTestHarness(this, std::get<1>(GetParam()))) {}

    std::unique_ptr<ScreenCapture> getScreenCapture() {
        return mRenderPathHarness.getScreenCapture();
    }

protected:
    LayerRenderPathTestHarness mRenderPathHarness;
};

// Environment for starting up binder threads. This is required for testing
// virtual displays, as BufferQueue parameters may be queried over binder.
class BinderEnvironment : public ::testing::Environment {
public:
    void SetUp() override { ProcessState::self()->startThreadPool(); }
};

::testing::Environment* const binderEnv =
        ::testing::AddGlobalTestEnvironment(new BinderEnvironment());

class LayerRenderTypeTransactionTest : public LayerTransactionTest,
                                       public ::testing::WithParamInterface<RenderPath> {
public:
    LayerRenderTypeTransactionTest() : mHarness(LayerRenderPathTestHarness(this, GetParam())) {}

    std::unique_ptr<ScreenCapture> getScreenCapture() { return mHarness.getScreenCapture(); }
    void setRelativeZBasicHelper(uint32_t layerType);
    void setRelativeZGroupHelper(uint32_t layerType);
    void setAlphaBasicHelper(uint32_t layerType);
    void setBackgroundColorHelper(uint32_t layerType, bool priorColor, bool bufferFill, float alpha,
                                  Color finalColor);

protected:
    LayerRenderPathTestHarness mHarness;
};

INSTANTIATE_TEST_CASE_P(
        LayerTypeAndRenderTypeTransactionTests, LayerTypeAndRenderTypeTransactionTest,
        ::testing::Combine(
                ::testing::Values(
                        static_cast<uint32_t>(ISurfaceComposerClient::eFXSurfaceBufferQueue),
                        static_cast<uint32_t>(ISurfaceComposerClient::eFXSurfaceBufferState)),
                ::testing::Values(RenderPath::VIRTUAL_DISPLAY, RenderPath::SCREENSHOT)));

INSTANTIATE_TEST_CASE_P(LayerRenderTypeTransactionTests, LayerRenderTypeTransactionTest,
                        ::testing::Values(RenderPath::VIRTUAL_DISPLAY, RenderPath::SCREENSHOT));

INSTANTIATE_TEST_CASE_P(
        LayerTypeTransactionTests, LayerTypeTransactionTest,
        ::testing::Values(static_cast<uint32_t>(ISurfaceComposerClient::eFXSurfaceBufferQueue),
                          static_cast<uint32_t>(ISurfaceComposerClient::eFXSurfaceBufferState)));

TEST_P(LayerRenderTypeTransactionTest, SetPositionBasic_BufferQueue) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createLayer("test", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(layer, Color::RED, 32, 32));

    {
        SCOPED_TRACE("default position");
        const Rect rect(0, 0, 32, 32);
        auto shot = getScreenCapture();
        shot->expectColor(rect, Color::RED);
        shot->expectBorder(rect, Color::BLACK);
    }

    Transaction().setPosition(layer, 5, 10).apply();
    {
        SCOPED_TRACE("new position");
        const Rect rect(5, 10, 37, 42);
        auto shot = getScreenCapture();
        shot->expectColor(rect, Color::RED);
        shot->expectBorder(rect, Color::BLACK);
    }
}

TEST_P(LayerRenderTypeTransactionTest, SetPositionRounding_BufferQueue) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createLayer("test", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(layer, Color::RED, 32, 32));

    // GLES requires only 4 bits of subpixel precision during rasterization
    // XXX GLES composition does not match HWC composition due to precision
    // loss (b/69315223)
    const float epsilon = 1.0f / 16.0f;
    Transaction().setPosition(layer, 0.5f - epsilon, 0.5f - epsilon).apply();
    {
        SCOPED_TRACE("rounding down");
        getScreenCapture()->expectColor(Rect(0, 0, 32, 32), Color::RED);
    }

    Transaction().setPosition(layer, 0.5f + epsilon, 0.5f + epsilon).apply();
    {
        SCOPED_TRACE("rounding up");
        getScreenCapture()->expectColor(Rect(1, 1, 33, 33), Color::RED);
    }
}

TEST_P(LayerRenderTypeTransactionTest, SetPositionOutOfBounds_BufferQueue) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createLayer("test", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(layer, Color::RED, 32, 32));

    Transaction().setPosition(layer, -32, -32).apply();
    {
        SCOPED_TRACE("negative coordinates");
        getScreenCapture()->expectColor(mDisplayRect, Color::BLACK);
    }

    Transaction().setPosition(layer, mDisplayWidth, mDisplayHeight).apply();
    {
        SCOPED_TRACE("positive coordinates");
        getScreenCapture()->expectColor(mDisplayRect, Color::BLACK);
    }
}

TEST_P(LayerRenderTypeTransactionTest, SetPositionPartiallyOutOfBounds_BufferQueue) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createLayer("test", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(layer, Color::RED, 32, 32));

    // partially out of bounds
    Transaction().setPosition(layer, -30, -30).apply();
    {
        SCOPED_TRACE("negative coordinates");
        getScreenCapture()->expectColor(Rect(0, 0, 2, 2), Color::RED);
    }

    Transaction().setPosition(layer, mDisplayWidth - 2, mDisplayHeight - 2).apply();
    {
        SCOPED_TRACE("positive coordinates");
        getScreenCapture()->expectColor(Rect(mDisplayWidth - 2, mDisplayHeight - 2, mDisplayWidth,
                                             mDisplayHeight),
                                        Color::RED);
    }
}

TEST_P(LayerRenderTypeTransactionTest, SetPositionWithResize_BufferQueue) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createLayer("test", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(layer, Color::RED, 32, 32));

    // setPosition is applied immediately by default, with or without resize
    // pending
    Transaction().setPosition(layer, 5, 10).setSize(layer, 64, 64).apply();
    {
        SCOPED_TRACE("resize pending");
        auto shot = getScreenCapture();
        const Rect rect(5, 10, 37, 42);
        shot->expectColor(rect, Color::RED);
        shot->expectBorder(rect, Color::BLACK);
    }

    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(layer, Color::RED, 64, 64));
    {
        SCOPED_TRACE("resize applied");
        getScreenCapture()->expectColor(Rect(5, 10, 69, 74), Color::RED);
    }
}

TEST_P(LayerRenderTypeTransactionTest, SetPositionWithNextResize_BufferQueue) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createLayer("test", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(layer, Color::RED, 32, 32));

    // request setPosition to be applied with the next resize
    Transaction().setPosition(layer, 5, 10).setGeometryAppliesWithResize(layer).apply();
    {
        SCOPED_TRACE("new position pending");
        getScreenCapture()->expectColor(Rect(0, 0, 32, 32), Color::RED);
    }

    Transaction().setPosition(layer, 15, 20).apply();
    {
        SCOPED_TRACE("pending new position modified");
        getScreenCapture()->expectColor(Rect(0, 0, 32, 32), Color::RED);
    }

    Transaction().setSize(layer, 64, 64).apply();
    {
        SCOPED_TRACE("resize pending");
        getScreenCapture()->expectColor(Rect(0, 0, 32, 32), Color::RED);
    }

    // finally resize and latch the buffer
    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(layer, Color::RED, 64, 64));
    {
        SCOPED_TRACE("new position applied");
        getScreenCapture()->expectColor(Rect(15, 20, 79, 84), Color::RED);
    }
}

TEST_P(LayerRenderTypeTransactionTest, SetPositionWithNextResizeScaleToWindow_BufferQueue) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createLayer("test", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(layer, Color::RED, 32, 32));

    // setPosition is not immediate even with SCALE_TO_WINDOW override
    Transaction()
            .setPosition(layer, 5, 10)
            .setSize(layer, 64, 64)
            .setOverrideScalingMode(layer, NATIVE_WINDOW_SCALING_MODE_SCALE_TO_WINDOW)
            .setGeometryAppliesWithResize(layer)
            .apply();
    {
        SCOPED_TRACE("new position pending");
        getScreenCapture()->expectColor(Rect(0, 0, 64, 64), Color::RED);
    }

    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(layer, Color::RED, 64, 64));
    {
        SCOPED_TRACE("new position applied");
        getScreenCapture()->expectColor(Rect(5, 10, 69, 74), Color::RED);
    }
}

TEST_P(LayerRenderTypeTransactionTest, SetSizeBasic_BufferQueue) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createLayer("test", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(layer, Color::RED, 32, 32));

    Transaction().setSize(layer, 64, 64).apply();
    {
        SCOPED_TRACE("resize pending");
        auto shot = getScreenCapture();
        const Rect rect(0, 0, 32, 32);
        shot->expectColor(rect, Color::RED);
        shot->expectBorder(rect, Color::BLACK);
    }

    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(layer, Color::RED, 64, 64));
    {
        SCOPED_TRACE("resize applied");
        auto shot = getScreenCapture();
        const Rect rect(0, 0, 64, 64);
        shot->expectColor(rect, Color::RED);
        shot->expectBorder(rect, Color::BLACK);
    }
}

TEST_P(LayerTypeAndRenderTypeTransactionTest, SetSizeInvalid) {
    // cannot test robustness against invalid sizes (zero or really huge)
}

TEST_P(LayerRenderTypeTransactionTest, SetSizeWithScaleToWindow_BufferQueue) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createLayer("test", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(layer, Color::RED, 32, 32));

    // setSize is immediate with SCALE_TO_WINDOW, unlike setPosition
    Transaction()
            .setSize(layer, 64, 64)
            .setOverrideScalingMode(layer, NATIVE_WINDOW_SCALING_MODE_SCALE_TO_WINDOW)
            .apply();
    getScreenCapture()->expectColor(Rect(0, 0, 64, 64), Color::RED);
}

TEST_P(LayerTypeAndRenderTypeTransactionTest, SetZBasic) {
    sp<SurfaceControl> layerR;
    sp<SurfaceControl> layerG;
    ASSERT_NO_FATAL_FAILURE(layerR = createLayer("test R", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillLayerColor(layerR, Color::RED, 32, 32));
    ASSERT_NO_FATAL_FAILURE(layerG = createLayer("test G", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillLayerColor(layerG, Color::GREEN, 32, 32));

    Transaction().setLayer(layerR, mLayerZBase + 1).apply();
    {
        SCOPED_TRACE("layerR");
        getScreenCapture()->expectColor(Rect(0, 0, 32, 32), Color::RED);
    }

    Transaction().setLayer(layerG, mLayerZBase + 2).apply();
    {
        SCOPED_TRACE("layerG");
        getScreenCapture()->expectColor(Rect(0, 0, 32, 32), Color::GREEN);
    }
}

TEST_P(LayerTypeAndRenderTypeTransactionTest, SetZNegative) {
    sp<SurfaceControl> parent =
            LayerTransactionTest::createLayer("Parent", 0 /* buffer width */, 0 /* buffer height */,
                                              ISurfaceComposerClient::eFXSurfaceContainer);
    Transaction().setCrop_legacy(parent, Rect(0, 0, mDisplayWidth, mDisplayHeight)).apply();
    sp<SurfaceControl> layerR;
    sp<SurfaceControl> layerG;
    ASSERT_NO_FATAL_FAILURE(layerR = createLayer("test R", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillLayerColor(layerR, Color::RED, 32, 32));
    ASSERT_NO_FATAL_FAILURE(layerG = createLayer("test G", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillLayerColor(layerG, Color::GREEN, 32, 32));

    Transaction()
            .reparent(layerR, parent->getHandle())
            .reparent(layerG, parent->getHandle())
            .apply();
    Transaction().setLayer(layerR, -1).setLayer(layerG, -2).apply();
    {
        SCOPED_TRACE("layerR");
        auto shot = getScreenCapture();
        shot->expectColor(Rect(0, 0, 32, 32), Color::RED);
    }

    Transaction().setLayer(layerR, -3).apply();
    {
        SCOPED_TRACE("layerG");
        auto shot = getScreenCapture();
        shot->expectColor(Rect(0, 0, 32, 32), Color::GREEN);
    }
}

void LayerRenderTypeTransactionTest::setRelativeZBasicHelper(uint32_t layerType) {
    sp<SurfaceControl> layerR;
    sp<SurfaceControl> layerG;
    ASSERT_NO_FATAL_FAILURE(layerR = createLayer("test R", 32, 32, layerType));
    ASSERT_NO_FATAL_FAILURE(fillLayerColor(layerType, layerR, Color::RED, 32, 32));
    ASSERT_NO_FATAL_FAILURE(layerG = createLayer("test G", 32, 32, layerType));
    ASSERT_NO_FATAL_FAILURE(fillLayerColor(layerType, layerG, Color::GREEN, 32, 32));

    switch (layerType) {
        case ISurfaceComposerClient::eFXSurfaceBufferQueue:
            Transaction()
                    .setPosition(layerG, 16, 16)
                    .setRelativeLayer(layerG, layerR->getHandle(), 1)
                    .apply();
            break;
        case ISurfaceComposerClient::eFXSurfaceBufferState:
            Transaction()
                    .setFrame(layerR, Rect(0, 0, 32, 32))
                    .setFrame(layerG, Rect(16, 16, 48, 48))
                    .setRelativeLayer(layerG, layerR->getHandle(), 1)
                    .apply();
            break;
        default:
            ASSERT_FALSE(true) << "Unsupported layer type";
    }
    {
        SCOPED_TRACE("layerG above");
        auto shot = getScreenCapture();
        shot->expectColor(Rect(0, 0, 16, 16), Color::RED);
        shot->expectColor(Rect(16, 16, 48, 48), Color::GREEN);
    }

    Transaction().setRelativeLayer(layerG, layerR->getHandle(), -1).apply();
    {
        SCOPED_TRACE("layerG below");
        auto shot = getScreenCapture();
        shot->expectColor(Rect(0, 0, 32, 32), Color::RED);
        shot->expectColor(Rect(32, 32, 48, 48), Color::GREEN);
    }
}

TEST_P(LayerRenderTypeTransactionTest, SetRelativeZBasic_BufferQueue) {
    ASSERT_NO_FATAL_FAILURE(setRelativeZBasicHelper(ISurfaceComposerClient::eFXSurfaceBufferQueue));
}

TEST_P(LayerRenderTypeTransactionTest, SetRelativeZBasic_BufferState) {
    ASSERT_NO_FATAL_FAILURE(setRelativeZBasicHelper(ISurfaceComposerClient::eFXSurfaceBufferState));
}

TEST_P(LayerTypeTransactionTest, SetRelativeZNegative) {
    sp<SurfaceControl> parent =
            LayerTransactionTest::createLayer("Parent", 0 /* buffer width */, 0 /* buffer height */,
                                              ISurfaceComposerClient::eFXSurfaceContainer);
    Transaction().setCrop_legacy(parent, Rect(0, 0, mDisplayWidth, mDisplayHeight)).apply();
    sp<SurfaceControl> layerR;
    sp<SurfaceControl> layerG;
    sp<SurfaceControl> layerB;
    ASSERT_NO_FATAL_FAILURE(layerR = createLayer("test R", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillLayerColor(layerR, Color::RED, 32, 32));
    ASSERT_NO_FATAL_FAILURE(layerG = createLayer("test G", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillLayerColor(layerG, Color::GREEN, 32, 32));
    ASSERT_NO_FATAL_FAILURE(layerB = createLayer("test B", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillLayerColor(layerB, Color::BLUE, 32, 32));

    Transaction()
            .reparent(layerB, parent->getHandle())
            .apply();

    // layerR = mLayerZBase, layerG = layerR - 1, layerB = -2
    Transaction().setRelativeLayer(layerG, layerR->getHandle(), -1).setLayer(layerB, -2).apply();

    std::unique_ptr<ScreenCapture> screenshot;
    // only layerB is in this range
    sp<IBinder> parentHandle = parent->getHandle();
    ScreenCapture::captureLayers(&screenshot, parentHandle, Rect(0, 0, 32, 32));
    screenshot->expectColor(Rect(0, 0, 32, 32), Color::BLUE);
}

TEST_P(LayerTypeTransactionTest, SetLayerAndRelative) {
    sp<SurfaceControl> parent =
            LayerTransactionTest::createLayer("Parent", 0 /* buffer width */, 0 /* buffer height */,
                                              ISurfaceComposerClient::eFXSurfaceColor);

    sp<SurfaceControl> childLayer;
    ASSERT_NO_FATAL_FAILURE(
            childLayer = LayerTransactionTest::createLayer("childLayer", 0 /* buffer width */,
                                                           0 /* buffer height */,
                                                           ISurfaceComposerClient::eFXSurfaceColor,
                                                           parent.get()));
    Transaction()
            .setColor(childLayer, half3{1.0f, 0.0f, 0.0f})
            .setColor(parent, half3{0.0f, 0.0f, 0.0f})
            .show(childLayer)
            .show(parent)
            .setCrop_legacy(parent, Rect(0, 0, mDisplayWidth, mDisplayHeight))
            .setCrop_legacy(childLayer, Rect(0, 0, 20, 30))
            .apply();

    Transaction()
            .setRelativeLayer(childLayer, parent->getHandle(), -1)
            .setLayer(childLayer, 1)
            .apply();

    {
        SCOPED_TRACE("setLayer above");
        // Set layer should get applied and place the child above.
        std::unique_ptr<ScreenCapture> screenshot;
        ScreenCapture::captureScreen(&screenshot);
        screenshot->expectColor(Rect(0, 0, 20, 30), Color::RED);
    }

    Transaction()
            .setLayer(childLayer, 1)
            .setRelativeLayer(childLayer, parent->getHandle(), -1)
            .apply();

    {
        SCOPED_TRACE("setRelative below");
        // Set relative layer should get applied and place the child below.
        std::unique_ptr<ScreenCapture> screenshot;
        ScreenCapture::captureScreen(&screenshot);
        screenshot->expectColor(Rect(0, 0, 20, 30), Color::BLACK);
    }
}

TEST_P(LayerTypeTransactionTest, HideRelativeParentHidesLayer) {
    sp<SurfaceControl> parent =
            LayerTransactionTest::createLayer("Parent", 0 /* buffer width */, 0 /* buffer height */,
                                              ISurfaceComposerClient::eFXSurfaceColor);
    sp<SurfaceControl> relativeParent =
            LayerTransactionTest::createLayer("RelativeParent", 0 /* buffer width */,
                    0 /* buffer height */, ISurfaceComposerClient::eFXSurfaceColor);

    sp<SurfaceControl> childLayer;
    ASSERT_NO_FATAL_FAILURE(
            childLayer = LayerTransactionTest::createLayer("childLayer", 0 /* buffer width */,
                                                           0 /* buffer height */,
                                                           ISurfaceComposerClient::eFXSurfaceColor,
                                                           parent.get()));
    Transaction()
            .setColor(childLayer, half3{1.0f, 0.0f, 0.0f})
            .setColor(parent, half3{0.0f, 0.0f, 0.0f})
            .setColor(relativeParent, half3{0.0f, 1.0f, 0.0f})
            .show(childLayer)
            .show(parent)
            .show(relativeParent)
            .setLayer(parent, mLayerZBase - 1)
            .setLayer(relativeParent, mLayerZBase)
            .apply();

    Transaction()
            .setRelativeLayer(childLayer, relativeParent->getHandle(), 1)
            .apply();

    {
        SCOPED_TRACE("setLayer above");
        // Set layer should get applied and place the child above.
        std::unique_ptr<ScreenCapture> screenshot;
        ScreenCapture::captureScreen(&screenshot);
        screenshot->expectColor(Rect(0, 0, 20, 30), Color::RED);
    }

    Transaction()
        .hide(relativeParent)
        .apply();

    {
        SCOPED_TRACE("hide relative parent");
        // The relative should no longer be visible.
        std::unique_ptr<ScreenCapture> screenshot;
        ScreenCapture::captureScreen(&screenshot);
        screenshot->expectColor(Rect(0, 0, 20, 30), Color::BLACK);
    }
}

void LayerRenderTypeTransactionTest::setRelativeZGroupHelper(uint32_t layerType) {
    sp<SurfaceControl> layerR;
    sp<SurfaceControl> layerG;
    sp<SurfaceControl> layerB;
    ASSERT_NO_FATAL_FAILURE(layerR = createLayer("test", 32, 32, layerType));
    ASSERT_NO_FATAL_FAILURE(fillLayerColor(layerType, layerR, Color::RED, 32, 32));
    ASSERT_NO_FATAL_FAILURE(layerG = createLayer("test", 32, 32, layerType));
    ASSERT_NO_FATAL_FAILURE(fillLayerColor(layerType, layerG, Color::GREEN, 32, 32));
    ASSERT_NO_FATAL_FAILURE(layerB = createLayer("test", 32, 32, layerType));
    ASSERT_NO_FATAL_FAILURE(fillLayerColor(layerType, layerB, Color::BLUE, 32, 32));

    // layerR = 0, layerG = layerR + 3, layerB = 2
    switch (layerType) {
        case ISurfaceComposerClient::eFXSurfaceBufferQueue:
            Transaction()
                    .setPosition(layerG, 8, 8)
                    .setRelativeLayer(layerG, layerR->getHandle(), 3)
                    .setPosition(layerB, 16, 16)
                    .setLayer(layerB, mLayerZBase + 2)
                    .apply();
            break;
        case ISurfaceComposerClient::eFXSurfaceBufferState:
            Transaction()
                    .setFrame(layerR, Rect(0, 0, 32, 32))
                    .setFrame(layerG, Rect(8, 8, 40, 40))
                    .setRelativeLayer(layerG, layerR->getHandle(), 3)
                    .setFrame(layerB, Rect(16, 16, 48, 48))
                    .setLayer(layerB, mLayerZBase + 2)
                    .apply();
            break;
        default:
            ASSERT_FALSE(true) << "Unsupported layer type";
    }

    {
        SCOPED_TRACE("(layerR < layerG) < layerB");
        auto shot = getScreenCapture();
        shot->expectColor(Rect(0, 0, 8, 8), Color::RED);
        shot->expectColor(Rect(8, 8, 16, 16), Color::GREEN);
        shot->expectColor(Rect(16, 16, 48, 48), Color::BLUE);
    }

    // layerR = 4, layerG = layerR + 3, layerB = 2
    Transaction().setLayer(layerR, mLayerZBase + 4).apply();
    {
        SCOPED_TRACE("layerB < (layerR < layerG)");
        auto shot = getScreenCapture();
        shot->expectColor(Rect(0, 0, 8, 8), Color::RED);
        shot->expectColor(Rect(8, 8, 40, 40), Color::GREEN);
        shot->expectColor(Rect(40, 40, 48, 48), Color::BLUE);
    }

    // layerR = 4, layerG = layerR - 3, layerB = 2
    Transaction().setRelativeLayer(layerG, layerR->getHandle(), -3).apply();
    {
        SCOPED_TRACE("layerB < (layerG < layerR)");
        auto shot = getScreenCapture();
        shot->expectColor(Rect(0, 0, 32, 32), Color::RED);
        shot->expectColor(Rect(32, 32, 40, 40), Color::GREEN);
        shot->expectColor(Rect(40, 40, 48, 48), Color::BLUE);
    }

    // restore to absolute z
    // layerR = 4, layerG = 0, layerB = 2
    Transaction().setLayer(layerG, mLayerZBase).apply();
    {
        SCOPED_TRACE("layerG < layerB < layerR");
        auto shot = getScreenCapture();
        shot->expectColor(Rect(0, 0, 32, 32), Color::RED);
        shot->expectColor(Rect(32, 32, 48, 48), Color::BLUE);
    }

    // layerR should not affect layerG anymore
    // layerR = 1, layerG = 0, layerB = 2
    Transaction().setLayer(layerR, mLayerZBase + 1).apply();
    {
        SCOPED_TRACE("layerG < layerR < layerB");
        auto shot = getScreenCapture();
        shot->expectColor(Rect(0, 0, 16, 16), Color::RED);
        shot->expectColor(Rect(16, 16, 48, 48), Color::BLUE);
    }
}

TEST_P(LayerRenderTypeTransactionTest, SetRelativeZGroup_BufferQueue) {
    ASSERT_NO_FATAL_FAILURE(setRelativeZGroupHelper(ISurfaceComposerClient::eFXSurfaceBufferQueue));
}

TEST_P(LayerRenderTypeTransactionTest, SetRelativeZGroup_BufferState) {
    ASSERT_NO_FATAL_FAILURE(setRelativeZGroupHelper(ISurfaceComposerClient::eFXSurfaceBufferState));
}

TEST_P(LayerTypeAndRenderTypeTransactionTest, SetRelativeZBug64572777) {
    sp<SurfaceControl> layerR;
    sp<SurfaceControl> layerG;

    ASSERT_NO_FATAL_FAILURE(layerR = createLayer("test R", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillLayerColor(layerR, Color::RED, 32, 32));
    ASSERT_NO_FATAL_FAILURE(layerG = createLayer("test G", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillLayerColor(layerG, Color::GREEN, 32, 32));

    Transaction()
            .setPosition(layerG, 16, 16)
            .setRelativeLayer(layerG, layerR->getHandle(), 1)
            .apply();

    layerG.clear();
    // layerG should have been removed
    getScreenCapture()->expectColor(Rect(0, 0, 32, 32), Color::RED);
}

TEST_P(LayerTypeAndRenderTypeTransactionTest, SetFlagsHidden) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createLayer("test", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillLayerColor(layer, Color::RED, 32, 32));

    Transaction().setFlags(layer, layer_state_t::eLayerHidden, layer_state_t::eLayerHidden).apply();
    {
        SCOPED_TRACE("layer hidden");
        getScreenCapture()->expectColor(mDisplayRect, Color::BLACK);
    }

    Transaction().setFlags(layer, 0, layer_state_t::eLayerHidden).apply();
    {
        SCOPED_TRACE("layer shown");
        getScreenCapture()->expectColor(Rect(0, 0, 32, 32), Color::RED);
    }
}

TEST_P(LayerTypeAndRenderTypeTransactionTest, SetFlagsOpaque) {
    const Color translucentRed = {100, 0, 0, 100};
    sp<SurfaceControl> layerR;
    sp<SurfaceControl> layerG;
    ASSERT_NO_FATAL_FAILURE(layerR = createLayer("test R", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillLayerColor(layerR, translucentRed, 32, 32));
    ASSERT_NO_FATAL_FAILURE(layerG = createLayer("test G", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillLayerColor(layerG, Color::GREEN, 32, 32));

    Transaction()
            .setLayer(layerR, mLayerZBase + 1)
            .setFlags(layerR, layer_state_t::eLayerOpaque, layer_state_t::eLayerOpaque)
            .apply();
    {
        SCOPED_TRACE("layerR opaque");
        getScreenCapture()->expectColor(Rect(0, 0, 32, 32), {100, 0, 0, 255});
    }

    Transaction().setFlags(layerR, 0, layer_state_t::eLayerOpaque).apply();
    {
        SCOPED_TRACE("layerR translucent");
        const uint8_t g = uint8_t(255 - translucentRed.a);
        getScreenCapture()->expectColor(Rect(0, 0, 32, 32), {100, g, 0, 255});
    }
}

TEST_P(LayerTypeTransactionTest, SetFlagsSecure) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createLayer("test", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillLayerColor(layer, Color::RED, 32, 32));

    sp<ISurfaceComposer> composer = ComposerService::getComposerService();
    sp<GraphicBuffer> outBuffer;
    Transaction()
            .setFlags(layer, layer_state_t::eLayerSecure, layer_state_t::eLayerSecure)
            .apply(true);
    ASSERT_EQ(PERMISSION_DENIED,
              composer->captureScreen(mDisplay, &outBuffer, Rect(), 0, 0, false));

    Transaction().setFlags(layer, 0, layer_state_t::eLayerSecure).apply(true);
    ASSERT_EQ(NO_ERROR,
              composer->captureScreen(mDisplay, &outBuffer, Rect(), 0, 0, false));
}

/** RAII Wrapper around get/seteuid */
class UIDFaker {
    uid_t oldId;
public:
    UIDFaker(uid_t uid) {
        oldId = geteuid();
        seteuid(uid);
    }
    ~UIDFaker() {
        seteuid(oldId);
    }
};

TEST_F(LayerTransactionTest, SetFlagsSecureEUidSystem) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createLayer("test", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(layer, Color::RED, 32, 32));

    sp<ISurfaceComposer> composer = ComposerService::getComposerService();
    sp<GraphicBuffer> outBuffer;
    Transaction()
            .setFlags(layer, layer_state_t::eLayerSecure, layer_state_t::eLayerSecure)
            .apply(true);
    ASSERT_EQ(PERMISSION_DENIED,
              composer->captureScreen(mDisplay, &outBuffer, Rect(), 0, 0, false));

    UIDFaker f(AID_SYSTEM);

    // By default the system can capture screenshots with secure layers but they
    // will be blacked out
    ASSERT_EQ(NO_ERROR,
              composer->captureScreen(mDisplay, &outBuffer, Rect(), 0, 0, false));

    {
        SCOPED_TRACE("as system");
        auto shot = screenshot();
        shot->expectColor(Rect(0, 0, 32, 32), Color::BLACK);
    }

    // Here we pass captureSecureLayers = true and since we are AID_SYSTEM we should be able
    // to receive them...we are expected to take care with the results.
    bool outCapturedSecureLayers;
    ASSERT_EQ(NO_ERROR,
              composer->captureScreen(mDisplay, &outBuffer, outCapturedSecureLayers,
                                      ui::Dataspace::V0_SRGB, ui::PixelFormat::RGBA_8888, Rect(), 0,
                                      0, false, ISurfaceComposer::eRotateNone, true));
    ASSERT_EQ(true, outCapturedSecureLayers);
    ScreenCapture sc(outBuffer);
    sc.expectColor(Rect(0, 0, 32, 32), Color::RED);
}

TEST_P(LayerRenderTypeTransactionTest, SetTransparentRegionHintBasic_BufferQueue) {
    const Rect top(0, 0, 32, 16);
    const Rect bottom(0, 16, 32, 32);
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createLayer("test", 32, 32));

    ANativeWindow_Buffer buffer;
    ASSERT_NO_FATAL_FAILURE(buffer = getBufferQueueLayerBuffer(layer));
    ASSERT_NO_FATAL_FAILURE(fillANativeWindowBufferColor(buffer, top, Color::TRANSPARENT));
    ASSERT_NO_FATAL_FAILURE(fillANativeWindowBufferColor(buffer, bottom, Color::RED));
    // setTransparentRegionHint always applies to the following buffer
    Transaction().setTransparentRegionHint(layer, Region(top)).apply();
    ASSERT_NO_FATAL_FAILURE(postBufferQueueLayerBuffer(layer));
    {
        SCOPED_TRACE("top transparent");
        auto shot = getScreenCapture();
        shot->expectColor(top, Color::BLACK);
        shot->expectColor(bottom, Color::RED);
    }

    Transaction().setTransparentRegionHint(layer, Region(bottom)).apply();
    {
        SCOPED_TRACE("transparent region hint pending");
        auto shot = getScreenCapture();
        shot->expectColor(top, Color::BLACK);
        shot->expectColor(bottom, Color::RED);
    }

    ASSERT_NO_FATAL_FAILURE(buffer = getBufferQueueLayerBuffer(layer));
    ASSERT_NO_FATAL_FAILURE(fillANativeWindowBufferColor(buffer, top, Color::RED));
    ASSERT_NO_FATAL_FAILURE(fillANativeWindowBufferColor(buffer, bottom, Color::TRANSPARENT));
    ASSERT_NO_FATAL_FAILURE(postBufferQueueLayerBuffer(layer));
    {
        SCOPED_TRACE("bottom transparent");
        auto shot = getScreenCapture();
        shot->expectColor(top, Color::RED);
        shot->expectColor(bottom, Color::BLACK);
    }
}

TEST_P(LayerRenderTypeTransactionTest, SetTransparentRegionHintBasic_BufferState) {
    const Rect top(0, 0, 32, 16);
    const Rect bottom(0, 16, 32, 32);
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(
            layer = createLayer("test", 32, 32, ISurfaceComposerClient::eFXSurfaceBufferState));

    sp<GraphicBuffer> buffer =
            new GraphicBuffer(32, 32, PIXEL_FORMAT_RGBA_8888, 1,
                              BufferUsage::CPU_READ_OFTEN | BufferUsage::CPU_WRITE_OFTEN |
                                      BufferUsage::COMPOSER_OVERLAY,
                              "test");

    ASSERT_NO_FATAL_FAILURE(fillGraphicBufferColor(buffer, top, Color::TRANSPARENT));
    ASSERT_NO_FATAL_FAILURE(fillGraphicBufferColor(buffer, bottom, Color::RED));
    Transaction()
            .setTransparentRegionHint(layer, Region(top))
            .setBuffer(layer, buffer)
            .setFrame(layer, Rect(0, 0, 32, 32))
            .apply();
    {
        SCOPED_TRACE("top transparent");
        auto shot = getScreenCapture();
        shot->expectColor(top, Color::BLACK);
        shot->expectColor(bottom, Color::RED);
    }

    Transaction().setTransparentRegionHint(layer, Region(bottom)).apply();
    {
        SCOPED_TRACE("transparent region hint intermediate");
        auto shot = getScreenCapture();
        shot->expectColor(top, Color::BLACK);
        shot->expectColor(bottom, Color::BLACK);
    }

    buffer = new GraphicBuffer(32, 32, PIXEL_FORMAT_RGBA_8888, 1,
                               BufferUsage::CPU_READ_OFTEN | BufferUsage::CPU_WRITE_OFTEN |
                                       BufferUsage::COMPOSER_OVERLAY,
                               "test");

    ASSERT_NO_FATAL_FAILURE(fillGraphicBufferColor(buffer, top, Color::RED));
    ASSERT_NO_FATAL_FAILURE(fillGraphicBufferColor(buffer, bottom, Color::TRANSPARENT));
    Transaction().setBuffer(layer, buffer).apply();
    {
        SCOPED_TRACE("bottom transparent");
        auto shot = getScreenCapture();
        shot->expectColor(top, Color::RED);
        shot->expectColor(bottom, Color::BLACK);
    }
}

TEST_P(LayerRenderTypeTransactionTest, SetTransparentRegionHintOutOfBounds_BufferQueue) {
    sp<SurfaceControl> layerTransparent;
    sp<SurfaceControl> layerR;
    ASSERT_NO_FATAL_FAILURE(layerTransparent = createLayer("test transparent", 32, 32));
    ASSERT_NO_FATAL_FAILURE(layerR = createLayer("test R", 32, 32));

    // check that transparent region hint is bound by the layer size
    Transaction()
            .setTransparentRegionHint(layerTransparent, Region(mDisplayRect))
            .setPosition(layerR, 16, 16)
            .setLayer(layerR, mLayerZBase + 1)
            .apply();
    ASSERT_NO_FATAL_FAILURE(
            fillBufferQueueLayerColor(layerTransparent, Color::TRANSPARENT, 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(layerR, Color::RED, 32, 32));
    getScreenCapture()->expectColor(Rect(16, 16, 48, 48), Color::RED);
}

TEST_P(LayerRenderTypeTransactionTest, SetTransparentRegionHintOutOfBounds_BufferState) {
    sp<SurfaceControl> layerTransparent;
    sp<SurfaceControl> layerR;
    ASSERT_NO_FATAL_FAILURE(layerTransparent = createLayer("test transparent", 32, 32));
    ASSERT_NO_FATAL_FAILURE(
            layerR = createLayer("test", 32, 32, ISurfaceComposerClient::eFXSurfaceBufferState));

    // check that transparent region hint is bound by the layer size
    Transaction()
            .setTransparentRegionHint(layerTransparent, Region(mDisplayRect))
            .setFrame(layerR, Rect(16, 16, 48, 48))
            .setLayer(layerR, mLayerZBase + 1)
            .apply();
    ASSERT_NO_FATAL_FAILURE(
            fillBufferQueueLayerColor(layerTransparent, Color::TRANSPARENT, 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillBufferStateLayerColor(layerR, Color::RED, 32, 32));
    getScreenCapture()->expectColor(Rect(16, 16, 48, 48), Color::RED);
}

void LayerRenderTypeTransactionTest::setAlphaBasicHelper(uint32_t layerType) {
    sp<SurfaceControl> layer1;
    sp<SurfaceControl> layer2;
    ASSERT_NO_FATAL_FAILURE(layer1 = createLayer("test 1", 32, 32, layerType));
    ASSERT_NO_FATAL_FAILURE(layer2 = createLayer("test 2", 32, 32, layerType));
    ASSERT_NO_FATAL_FAILURE(fillLayerColor(layerType, layer1, {64, 0, 0, 255}, 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillLayerColor(layerType, layer2, {0, 64, 0, 255}, 32, 32));

    switch (layerType) {
        case ISurfaceComposerClient::eFXSurfaceBufferQueue:
            Transaction()
                    .setAlpha(layer1, 0.25f)
                    .setAlpha(layer2, 0.75f)
                    .setPosition(layer2, 16, 0)
                    .setLayer(layer2, mLayerZBase + 1)
                    .apply();
            break;
        case ISurfaceComposerClient::eFXSurfaceBufferState:
            Transaction()
                    .setAlpha(layer1, 0.25f)
                    .setAlpha(layer2, 0.75f)
                    .setFrame(layer1, Rect(0, 0, 32, 32))
                    .setFrame(layer2, Rect(16, 0, 48, 32))
                    .setLayer(layer2, mLayerZBase + 1)
                    .apply();
            break;
        default:
            ASSERT_FALSE(true) << "Unsupported layer type";
    }
    {
        auto shot = getScreenCapture();
        uint8_t r = 16; // 64 * 0.25f
        uint8_t g = 48; // 64 * 0.75f
        shot->expectColor(Rect(0, 0, 16, 32), {r, 0, 0, 255});
        shot->expectColor(Rect(32, 0, 48, 32), {0, g, 0, 255});

        r /= 4; // r * (1.0f - 0.75f)
        shot->expectColor(Rect(16, 0, 32, 32), {r, g, 0, 255});
    }
}

TEST_P(LayerRenderTypeTransactionTest, SetAlphaBasic_BufferQueue) {
    ASSERT_NO_FATAL_FAILURE(setAlphaBasicHelper(ISurfaceComposerClient::eFXSurfaceBufferQueue));
}

TEST_P(LayerRenderTypeTransactionTest, SetAlphaBasic_BufferState) {
    ASSERT_NO_FATAL_FAILURE(setAlphaBasicHelper(ISurfaceComposerClient::eFXSurfaceBufferState));
}

TEST_P(LayerTypeAndRenderTypeTransactionTest, SetAlphaClamped) {
    const Color color = {64, 0, 0, 255};
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createLayer("test", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillLayerColor(layer, color, 32, 32));

    Transaction().setAlpha(layer, 2.0f).apply();
    {
        SCOPED_TRACE("clamped to 1.0f");
        getScreenCapture()->expectColor(Rect(0, 0, 32, 32), color);
    }

    Transaction().setAlpha(layer, -1.0f).apply();
    {
        SCOPED_TRACE("clamped to 0.0f");
        getScreenCapture()->expectColor(Rect(0, 0, 32, 32), Color::BLACK);
    }
}

TEST_P(LayerTypeAndRenderTypeTransactionTest, SetCornerRadius) {
    sp<SurfaceControl> layer;
    const uint8_t size = 64;
    const uint8_t testArea = 4;
    const float cornerRadius = 20.0f;
    ASSERT_NO_FATAL_FAILURE(layer = createLayer("test", size, size));
    ASSERT_NO_FATAL_FAILURE(fillLayerColor(layer, Color::RED, size, size));

    Transaction()
            .setCornerRadius(layer, cornerRadius)
            .apply();
    {
        const uint8_t bottom = size - 1;
        const uint8_t right = size - 1;
        auto shot = getScreenCapture();
        // Transparent corners
        shot->expectColor(Rect(0, 0, testArea, testArea), Color::BLACK);
        shot->expectColor(Rect(size - testArea, 0, right, testArea), Color::BLACK);
        shot->expectColor(Rect(0, bottom - testArea, testArea, bottom), Color::BLACK);
        shot->expectColor(Rect(size - testArea, bottom - testArea, right, bottom), Color::BLACK);
    }
}

TEST_P(LayerTypeAndRenderTypeTransactionTest, SetCornerRadiusChildCrop) {
    sp<SurfaceControl> parent;
    sp<SurfaceControl> child;
    const uint8_t size = 64;
    const uint8_t testArea = 4;
    const float cornerRadius = 20.0f;
    ASSERT_NO_FATAL_FAILURE(parent = createLayer("parent", size, size));
    ASSERT_NO_FATAL_FAILURE(fillLayerColor(parent, Color::RED, size, size));
    ASSERT_NO_FATAL_FAILURE(child = createLayer("child", size, size / 2));
    ASSERT_NO_FATAL_FAILURE(fillLayerColor(child, Color::GREEN, size, size / 2));

    Transaction()
            .setCornerRadius(parent, cornerRadius)
            .reparent(child, parent->getHandle())
            .setPosition(child, 0, size / 2)
            .apply();
    {
        const uint8_t bottom = size - 1;
        const uint8_t right = size - 1;
        auto shot = getScreenCapture();
        // Top edge of child should not have rounded corners because it's translated in the parent
        shot->expectColor(Rect(0, size / 2, right, static_cast<int>(bottom - cornerRadius)),
            Color::GREEN);
        // But bottom edges should have been clipped according to parent bounds
        shot->expectColor(Rect(0, bottom - testArea, testArea, bottom), Color::BLACK);
        shot->expectColor(Rect(right - testArea, bottom - testArea, right, bottom), Color::BLACK);
    }
}

TEST_P(LayerRenderTypeTransactionTest, SetColorBasic) {
    sp<SurfaceControl> bufferLayer;
    sp<SurfaceControl> colorLayer;
    ASSERT_NO_FATAL_FAILURE(bufferLayer = createLayer("test bg", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(bufferLayer, Color::RED, 32, 32));
    ASSERT_NO_FATAL_FAILURE(colorLayer =
                                    createLayer("test", 0 /* buffer width */, 0 /* buffer height */,
                                                ISurfaceComposerClient::eFXSurfaceColor));

    Transaction()
            .setCrop_legacy(colorLayer, Rect(0, 0, 32, 32))
            .setLayer(colorLayer, mLayerZBase + 1)
            .apply();

    {
        SCOPED_TRACE("default color");
        getScreenCapture()->expectColor(Rect(0, 0, 32, 32), Color::BLACK);
    }

    const half3 color(15.0f / 255.0f, 51.0f / 255.0f, 85.0f / 255.0f);
    const Color expected = {15, 51, 85, 255};
    // this is handwavy, but the precison loss scaled by 255 (8-bit per
    // channel) should be less than one
    const uint8_t tolerance = 1;
    Transaction().setColor(colorLayer, color).apply();
    {
        SCOPED_TRACE("new color");
        getScreenCapture()->expectColor(Rect(0, 0, 32, 32), expected, tolerance);
    }
}

// RED: Color layer base color and BufferQueueLayer/BufferStateLayer fill
// BLUE: prior background color
// GREEN: final background color
// BLACK: no color or fill
void LayerRenderTypeTransactionTest::setBackgroundColorHelper(uint32_t layerType, bool priorColor,
                                                              bool bufferFill, float alpha,
                                                              Color finalColor) {
    sp<SurfaceControl> layer;
    int32_t width = 500;
    int32_t height = 500;

    Color fillColor = Color::RED;
    Color priorBgColor = Color::BLUE;
    Color expectedColor = Color::BLACK;
    switch (layerType) {
        case ISurfaceComposerClient::eFXSurfaceColor:
            ASSERT_NO_FATAL_FAILURE(layer = createLayer("test", 0, 0, layerType));
            Transaction()
                    .setCrop_legacy(layer, Rect(0, 0, width, height))
                    .setColor(layer, half3(1.0f, 0, 0))
                    .apply();
            expectedColor = fillColor;
            break;
        case ISurfaceComposerClient::eFXSurfaceBufferQueue:
            ASSERT_NO_FATAL_FAILURE(layer = createLayer("test", width, height));
            if (bufferFill) {
                ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(layer, fillColor, width, height));
                expectedColor = fillColor;
            }
            Transaction().setCrop_legacy(layer, Rect(0, 0, width, height)).apply();
            break;
        case ISurfaceComposerClient::eFXSurfaceBufferState:
            ASSERT_NO_FATAL_FAILURE(layer = createLayer("test", width, height, layerType));
            if (bufferFill) {
                ASSERT_NO_FATAL_FAILURE(fillBufferStateLayerColor(layer, fillColor, width, height));
                expectedColor = fillColor;
            }
            Transaction().setFrame(layer, Rect(0, 0, width, height)).apply();
            break;
        default:
            GTEST_FAIL() << "Unknown layer type in setBackgroundColorHelper";
            return;
    }

    if (priorColor && layerType != ISurfaceComposerClient::eFXSurfaceColor) {
        Transaction()
                .setBackgroundColor(layer, half3(0, 0, 1.0f), 1.0f, ui::Dataspace::UNKNOWN)
                .apply();
        if (!bufferFill) {
            expectedColor = priorBgColor;
        }
    }

    {
        SCOPED_TRACE("default before setting background color layer");
        screenshot()->expectColor(Rect(0, 0, width, height), expectedColor);
    }
    Transaction()
            .setBackgroundColor(layer, half3(0, 1.0f, 0), alpha, ui::Dataspace::UNKNOWN)
            .apply();

    {
        auto shot = screenshot();
        shot->expectColor(Rect(0, 0, width, height), finalColor);
        shot->expectBorder(Rect(0, 0, width, height), Color::BLACK);
    }
}

TEST_P(LayerRenderTypeTransactionTest, SetBackgroundColor_Color_NoEffect) {
    bool priorColor = false;
    bool bufferFill = false;
    float alpha = 1.0f;
    Color finalColor = Color::RED;
    ASSERT_NO_FATAL_FAILURE(setBackgroundColorHelper(ISurfaceComposerClient::eFXSurfaceColor,
                                                     priorColor, bufferFill, alpha, finalColor));
}

TEST_P(LayerRenderTypeTransactionTest,
       SetBackgroundColor_BufferQueue_BufferFill_NoPriorColor_Basic) {
    bool priorColor = false;
    bool bufferFill = true;
    float alpha = 1.0f;
    Color finalColor = Color::RED;
    ASSERT_NO_FATAL_FAILURE(setBackgroundColorHelper(ISurfaceComposerClient::eFXSurfaceBufferQueue,
                                                     priorColor, bufferFill, alpha, finalColor));
}

TEST_P(LayerRenderTypeTransactionTest,
       SetBackgroundColor_BufferQueue_NoBufferFill_NoPriorColor_Basic) {
    bool priorColor = false;
    bool bufferFill = false;
    float alpha = 1.0f;
    Color finalColor = Color::GREEN;
    ASSERT_NO_FATAL_FAILURE(setBackgroundColorHelper(ISurfaceComposerClient::eFXSurfaceBufferQueue,
                                                     priorColor, bufferFill, alpha, finalColor));
}

TEST_P(LayerRenderTypeTransactionTest, SetBackgroundColor_BufferQueue_BufferFill_PriorColor_Basic) {
    bool priorColor = true;
    bool bufferFill = true;
    float alpha = 1.0f;
    Color finalColor = Color::RED;
    ASSERT_NO_FATAL_FAILURE(setBackgroundColorHelper(ISurfaceComposerClient::eFXSurfaceBufferQueue,
                                                     priorColor, bufferFill, alpha, finalColor));
}

TEST_P(LayerRenderTypeTransactionTest,
       SetBackgroundColor_BufferQueue_NoBufferFill_PriorColor_Basic) {
    bool priorColor = true;
    bool bufferFill = false;
    float alpha = 1.0f;
    Color finalColor = Color::GREEN;
    ASSERT_NO_FATAL_FAILURE(setBackgroundColorHelper(ISurfaceComposerClient::eFXSurfaceBufferQueue,
                                                     priorColor, bufferFill, alpha, finalColor));
}
TEST_P(LayerRenderTypeTransactionTest,
       SetBackgroundColor_BufferQueue_NoPriorColor_ZeroAlpha_NoEffect) {
    bool priorColor = false;
    bool bufferFill = false;
    float alpha = 0;
    Color finalColor = Color::BLACK;
    ASSERT_NO_FATAL_FAILURE(setBackgroundColorHelper(ISurfaceComposerClient::eFXSurfaceBufferQueue,
                                                     priorColor, bufferFill, alpha, finalColor));
}

TEST_P(LayerRenderTypeTransactionTest,
       SetBackgroundColor_BufferQueue_PriorColor_ZeroAlpha_DeleteBackground) {
    bool priorColor = true;
    bool bufferFill = false;
    float alpha = 0;
    Color finalColor = Color::BLACK;
    ASSERT_NO_FATAL_FAILURE(setBackgroundColorHelper(ISurfaceComposerClient::eFXSurfaceBufferQueue,
                                                     priorColor, bufferFill, alpha, finalColor));
}

TEST_P(LayerRenderTypeTransactionTest,
       SetBackgroundColor_BufferState_BufferFill_NoPriorColor_Basic) {
    bool priorColor = false;
    bool bufferFill = true;
    float alpha = 1.0f;
    Color finalColor = Color::RED;
    ASSERT_NO_FATAL_FAILURE(setBackgroundColorHelper(ISurfaceComposerClient::eFXSurfaceBufferState,
                                                     priorColor, bufferFill, alpha, finalColor));
}

TEST_P(LayerRenderTypeTransactionTest,
       SetBackgroundColor_BufferState_NoBufferFill_NoPriorColor_Basic) {
    bool priorColor = false;
    bool bufferFill = false;
    float alpha = 1.0f;
    Color finalColor = Color::GREEN;
    ASSERT_NO_FATAL_FAILURE(setBackgroundColorHelper(ISurfaceComposerClient::eFXSurfaceBufferState,
                                                     priorColor, bufferFill, alpha, finalColor));
}

TEST_P(LayerRenderTypeTransactionTest,
       SetBackgroundColor_BufferState_NoBufferFill_PriorColor_Basic) {
    bool priorColor = true;
    bool bufferFill = false;
    float alpha = 1.0f;
    Color finalColor = Color::GREEN;
    ASSERT_NO_FATAL_FAILURE(setBackgroundColorHelper(ISurfaceComposerClient::eFXSurfaceBufferState,
                                                     priorColor, bufferFill, alpha, finalColor));
}

TEST_P(LayerRenderTypeTransactionTest,
       SetBackgroundColor_BufferState_NoPriorColor_ZeroAlpha_NoEffect) {
    bool priorColor = false;
    bool bufferFill = false;
    float alpha = 0;
    Color finalColor = Color::BLACK;
    ASSERT_NO_FATAL_FAILURE(setBackgroundColorHelper(ISurfaceComposerClient::eFXSurfaceBufferState,
                                                     priorColor, bufferFill, alpha, finalColor));
}

TEST_P(LayerRenderTypeTransactionTest,
       SetBackgroundColor_BufferState_PriorColor_ZeroAlpha_DeleteBackground) {
    bool priorColor = true;
    bool bufferFill = false;
    float alpha = 0;
    Color finalColor = Color::BLACK;
    ASSERT_NO_FATAL_FAILURE(setBackgroundColorHelper(ISurfaceComposerClient::eFXSurfaceBufferState,
                                                     priorColor, bufferFill, alpha, finalColor));
}

TEST_P(LayerRenderTypeTransactionTest, SetColorClamped) {
    sp<SurfaceControl> colorLayer;
    ASSERT_NO_FATAL_FAILURE(colorLayer =
                                    createLayer("test", 0 /* buffer width */, 0 /* buffer height */,
                                                ISurfaceComposerClient::eFXSurfaceColor));
    Transaction()
            .setCrop_legacy(colorLayer, Rect(0, 0, 32, 32))
            .setColor(colorLayer, half3(2.0f, -1.0f, 0.0f))
            .apply();

    getScreenCapture()->expectColor(Rect(0, 0, 32, 32), Color::RED);
}

TEST_P(LayerRenderTypeTransactionTest, SetColorWithAlpha) {
    sp<SurfaceControl> bufferLayer;
    sp<SurfaceControl> colorLayer;
    ASSERT_NO_FATAL_FAILURE(bufferLayer = createLayer("test bg", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(bufferLayer, Color::RED, 32, 32));
    ASSERT_NO_FATAL_FAILURE(colorLayer =
                                    createLayer("test", 0 /* buffer width */, 0 /* buffer height */,
                                                ISurfaceComposerClient::eFXSurfaceColor));
    Transaction().setCrop_legacy(colorLayer, Rect(0, 0, 32, 32)).apply();

    const half3 color(15.0f / 255.0f, 51.0f / 255.0f, 85.0f / 255.0f);
    const float alpha = 0.25f;
    const ubyte3 expected((vec3(color) * alpha + vec3(1.0f, 0.0f, 0.0f) * (1.0f - alpha)) * 255.0f);
    // this is handwavy, but the precison loss scaled by 255 (8-bit per
    // channel) should be less than one
    const uint8_t tolerance = 1;
    Transaction()
            .setColor(colorLayer, color)
            .setAlpha(colorLayer, alpha)
            .setLayer(colorLayer, mLayerZBase + 1)
            .apply();
    getScreenCapture()->expectColor(Rect(0, 0, 32, 32), {expected.r, expected.g, expected.b, 255},
                                    tolerance);
}

TEST_P(LayerRenderTypeTransactionTest, SetColorWithParentAlpha_Bug74220420) {
    sp<SurfaceControl> bufferLayer;
    sp<SurfaceControl> parentLayer;
    sp<SurfaceControl> colorLayer;
    ASSERT_NO_FATAL_FAILURE(bufferLayer = createLayer("test bg", 32, 32));
    ASSERT_NO_FATAL_FAILURE(parentLayer = createLayer("parentWithAlpha", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(bufferLayer, Color::RED, 32, 32));
    ASSERT_NO_FATAL_FAILURE(colorLayer = createLayer("childWithColor", 0 /* buffer width */,
                                                     0 /* buffer height */,
                                                     ISurfaceComposerClient::eFXSurfaceColor));
    Transaction().setCrop_legacy(colorLayer, Rect(0, 0, 32, 32)).apply();
    const half3 color(15.0f / 255.0f, 51.0f / 255.0f, 85.0f / 255.0f);
    const float alpha = 0.25f;
    const ubyte3 expected((vec3(color) * alpha + vec3(1.0f, 0.0f, 0.0f) * (1.0f - alpha)) * 255.0f);
    // this is handwavy, but the precision loss scaled by 255 (8-bit per
    // channel) should be less than one
    const uint8_t tolerance = 1;
    Transaction()
            .reparent(colorLayer, parentLayer->getHandle())
            .setColor(colorLayer, color)
            .setAlpha(parentLayer, alpha)
            .setLayer(parentLayer, mLayerZBase + 1)
            .apply();
    getScreenCapture()->expectColor(Rect(0, 0, 32, 32), {expected.r, expected.g, expected.b, 255},
                                    tolerance);
}

TEST_P(LayerTypeAndRenderTypeTransactionTest, SetColorWithBuffer) {
    sp<SurfaceControl> bufferLayer;
    ASSERT_NO_FATAL_FAILURE(bufferLayer = createLayer("test", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillLayerColor(bufferLayer, Color::RED, 32, 32));

    // color is ignored
    Transaction().setColor(bufferLayer, half3(0.0f, 1.0f, 0.0f)).apply();
    getScreenCapture()->expectColor(Rect(0, 0, 32, 32), Color::RED);
}

TEST_P(LayerTypeAndRenderTypeTransactionTest, SetLayerStackBasic) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createLayer("test", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillLayerColor(layer, Color::RED, 32, 32));

    Transaction().setLayerStack(layer, mDisplayLayerStack + 1).apply();
    {
        SCOPED_TRACE("non-existing layer stack");
        getScreenCapture()->expectColor(mDisplayRect, Color::BLACK);
    }

    Transaction().setLayerStack(layer, mDisplayLayerStack).apply();
    {
        SCOPED_TRACE("original layer stack");
        getScreenCapture()->expectColor(Rect(0, 0, 32, 32), Color::RED);
    }
}

TEST_P(LayerRenderTypeTransactionTest, SetMatrixBasic_BufferQueue) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createLayer("test", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerQuadrant(layer, 32, 32, Color::RED, Color::GREEN,
                                                         Color::BLUE, Color::WHITE));

    Transaction().setMatrix(layer, 1.0f, 0.0f, 0.0f, 1.0f).setPosition(layer, 0, 0).apply();
    {
        SCOPED_TRACE("IDENTITY");
        getScreenCapture()->expectQuadrant(Rect(0, 0, 32, 32), Color::RED, Color::GREEN,
                                           Color::BLUE, Color::WHITE);
    }

    Transaction().setMatrix(layer, -1.0f, 0.0f, 0.0f, 1.0f).setPosition(layer, 32, 0).apply();
    {
        SCOPED_TRACE("FLIP_H");
        getScreenCapture()->expectQuadrant(Rect(0, 0, 32, 32), Color::GREEN, Color::RED,
                                           Color::WHITE, Color::BLUE);
    }

    Transaction().setMatrix(layer, 1.0f, 0.0f, 0.0f, -1.0f).setPosition(layer, 0, 32).apply();
    {
        SCOPED_TRACE("FLIP_V");
        getScreenCapture()->expectQuadrant(Rect(0, 0, 32, 32), Color::BLUE, Color::WHITE,
                                           Color::RED, Color::GREEN);
    }

    Transaction().setMatrix(layer, 0.0f, 1.0f, -1.0f, 0.0f).setPosition(layer, 32, 0).apply();
    {
        SCOPED_TRACE("ROT_90");
        getScreenCapture()->expectQuadrant(Rect(0, 0, 32, 32), Color::BLUE, Color::RED,
                                           Color::WHITE, Color::GREEN);
    }

    Transaction().setMatrix(layer, 2.0f, 0.0f, 0.0f, 2.0f).setPosition(layer, 0, 0).apply();
    {
        SCOPED_TRACE("SCALE");
        getScreenCapture()->expectQuadrant(Rect(0, 0, 64, 64), Color::RED, Color::GREEN,
                                           Color::BLUE, Color::WHITE, true /* filtered */);
    }
}

TEST_P(LayerRenderTypeTransactionTest, SetMatrixBasic_BufferState) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(
            layer = createLayer("test", 32, 32, ISurfaceComposerClient::eFXSurfaceBufferState));
    ASSERT_NO_FATAL_FAILURE(fillBufferStateLayerQuadrant(layer, 32, 32, Color::RED, Color::GREEN,
                                                         Color::BLUE, Color::WHITE));

    Transaction()
            .setMatrix(layer, 1.0f, 0.0f, 0.0f, 1.0f)
            .setFrame(layer, Rect(0, 0, 32, 32))
            .apply();
    {
        SCOPED_TRACE("IDENTITY");
        getScreenCapture()->expectQuadrant(Rect(0, 0, 32, 32), Color::RED, Color::GREEN,
                                           Color::BLUE, Color::WHITE);
    }

    Transaction().setMatrix(layer, -1.0f, 0.0f, 0.0f, 1.0f).apply();
    {
        SCOPED_TRACE("FLIP_H");
        getScreenCapture()->expectQuadrant(Rect(0, 0, 32, 32), Color::RED, Color::GREEN,
                                           Color::BLUE, Color::WHITE);
    }

    Transaction().setMatrix(layer, 1.0f, 0.0f, 0.0f, -1.0f).apply();
    {
        SCOPED_TRACE("FLIP_V");
        getScreenCapture()->expectQuadrant(Rect(0, 0, 32, 32), Color::RED, Color::GREEN,
                                           Color::BLUE, Color::WHITE);
    }

    Transaction().setMatrix(layer, 0.0f, 1.0f, -1.0f, 0.0f).apply();
    {
        SCOPED_TRACE("ROT_90");
        getScreenCapture()->expectQuadrant(Rect(0, 0, 32, 32), Color::RED, Color::GREEN,
                                           Color::BLUE, Color::WHITE);
    }

    Transaction().setMatrix(layer, 2.0f, 0.0f, 0.0f, 2.0f).apply();
    {
        SCOPED_TRACE("SCALE");
        getScreenCapture()->expectQuadrant(Rect(0, 0, 32, 32), Color::RED, Color::GREEN,
                                           Color::BLUE, Color::WHITE);
    }
}

TEST_P(LayerRenderTypeTransactionTest, SetMatrixRot45_BufferQueue) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createLayer("test", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerQuadrant(layer, 32, 32, Color::RED, Color::GREEN,
                                                         Color::BLUE, Color::WHITE));

    const float rot = M_SQRT1_2; // 45 degrees
    const float trans = M_SQRT2 * 16.0f;
    Transaction().setMatrix(layer, rot, rot, -rot, rot).setPosition(layer, trans, 0).apply();

    auto shot = getScreenCapture();
    // check a 8x8 region inside each color
    auto get8x8Rect = [](int32_t centerX, int32_t centerY) {
        const int32_t halfL = 4;
        return Rect(centerX - halfL, centerY - halfL, centerX + halfL, centerY + halfL);
    };
    const int32_t unit = int32_t(trans / 2);
    shot->expectColor(get8x8Rect(2 * unit, 1 * unit), Color::RED);
    shot->expectColor(get8x8Rect(3 * unit, 2 * unit), Color::GREEN);
    shot->expectColor(get8x8Rect(1 * unit, 2 * unit), Color::BLUE);
    shot->expectColor(get8x8Rect(2 * unit, 3 * unit), Color::WHITE);
}

TEST_P(LayerRenderTypeTransactionTest, SetMatrixWithResize_BufferQueue) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createLayer("test", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(layer, Color::RED, 32, 32));

    // setMatrix is applied after any pending resize, unlike setPosition
    Transaction().setMatrix(layer, 2.0f, 0.0f, 0.0f, 2.0f).setSize(layer, 64, 64).apply();
    {
        SCOPED_TRACE("resize pending");
        auto shot = getScreenCapture();
        const Rect rect(0, 0, 32, 32);
        shot->expectColor(rect, Color::RED);
        shot->expectBorder(rect, Color::BLACK);
    }

    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(layer, Color::RED, 64, 64));
    {
        SCOPED_TRACE("resize applied");
        const Rect rect(0, 0, 128, 128);
        getScreenCapture()->expectColor(rect, Color::RED);
    }
}

TEST_P(LayerRenderTypeTransactionTest, SetMatrixWithScaleToWindow_BufferQueue) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createLayer("test", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(layer, Color::RED, 32, 32));

    // setMatrix is immediate with SCALE_TO_WINDOW, unlike setPosition
    Transaction()
            .setMatrix(layer, 2.0f, 0.0f, 0.0f, 2.0f)
            .setSize(layer, 64, 64)
            .setOverrideScalingMode(layer, NATIVE_WINDOW_SCALING_MODE_SCALE_TO_WINDOW)
            .apply();
    getScreenCapture()->expectColor(Rect(0, 0, 128, 128), Color::RED);
}

TEST_P(LayerRenderTypeTransactionTest, SetOverrideScalingModeBasic_BufferQueue) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createLayer("test", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerQuadrant(layer, 32, 32, Color::RED, Color::GREEN,
                                                         Color::BLUE, Color::WHITE));

    // XXX SCALE_CROP is not respected; calling setSize and
    // setOverrideScalingMode in separate transactions does not work
    // (b/69315456)
    Transaction()
            .setSize(layer, 64, 16)
            .setOverrideScalingMode(layer, NATIVE_WINDOW_SCALING_MODE_SCALE_TO_WINDOW)
            .apply();
    {
        SCOPED_TRACE("SCALE_TO_WINDOW");
        getScreenCapture()->expectQuadrant(Rect(0, 0, 64, 16), Color::RED, Color::GREEN,
                                           Color::BLUE, Color::WHITE, true /* filtered */);
    }
}

TEST_P(LayerTypeTransactionTest, RefreshRateIsInitialized) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createLayer("test", 32, 32));

    sp<IBinder> handle = layer->getHandle();
    ASSERT_TRUE(handle != nullptr);

    FrameStats frameStats;
    mClient->getLayerFrameStats(handle, &frameStats);

    ASSERT_GT(frameStats.refreshPeriodNano, static_cast<nsecs_t>(0));
}

TEST_P(LayerRenderTypeTransactionTest, SetCropBasic_BufferQueue) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createLayer("test", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(layer, Color::RED, 32, 32));
    const Rect crop(8, 8, 24, 24);

    Transaction().setCrop_legacy(layer, crop).apply();
    auto shot = getScreenCapture();
    shot->expectColor(crop, Color::RED);
    shot->expectBorder(crop, Color::BLACK);
}

TEST_P(LayerRenderTypeTransactionTest, SetCropBasic_BufferState) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(
            layer = createLayer("test", 32, 32, ISurfaceComposerClient::eFXSurfaceBufferState));
    ASSERT_NO_FATAL_FAILURE(fillBufferStateLayerColor(layer, Color::RED, 32, 32));
    const Rect crop(8, 8, 24, 24);

    Transaction().setCrop(layer, crop).apply();
    auto shot = getScreenCapture();
    shot->expectColor(Rect(0, 0, mDisplayWidth, mDisplayHeight), Color::RED);
    shot->expectBorder(Rect(0, 0, mDisplayWidth, mDisplayHeight), Color::BLACK);
}

TEST_P(LayerRenderTypeTransactionTest, SetCropEmpty_BufferQueue) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createLayer("test", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(layer, Color::RED, 32, 32));

    {
        SCOPED_TRACE("empty rect");
        Transaction().setCrop_legacy(layer, Rect(8, 8, 8, 8)).apply();
        getScreenCapture()->expectColor(Rect(0, 0, 32, 32), Color::RED);
    }

    {
        SCOPED_TRACE("negative rect");
        Transaction().setCrop_legacy(layer, Rect(8, 8, 0, 0)).apply();
        getScreenCapture()->expectColor(Rect(0, 0, 32, 32), Color::RED);
    }
}

TEST_P(LayerRenderTypeTransactionTest, SetCropEmpty_BufferState) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(
            layer = createLayer("test", 32, 32, ISurfaceComposerClient::eFXSurfaceBufferState));
    ASSERT_NO_FATAL_FAILURE(fillBufferStateLayerColor(layer, Color::RED, 32, 32));

    {
        SCOPED_TRACE("empty rect");
        Transaction().setCrop(layer, Rect(8, 8, 8, 8)).apply();
        getScreenCapture()->expectColor(Rect(0, 0, mDisplayWidth, mDisplayHeight), Color::RED);
    }

    {
        SCOPED_TRACE("negative rect");
        Transaction().setCrop(layer, Rect(8, 8, 0, 0)).apply();
        getScreenCapture()->expectColor(Rect(0, 0, mDisplayWidth, mDisplayHeight), Color::RED);
    }
}

TEST_P(LayerRenderTypeTransactionTest, SetCropOutOfBounds_BufferQueue) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createLayer("test", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(layer, Color::RED, 32, 32));

    Transaction().setCrop_legacy(layer, Rect(-128, -64, 128, 64)).apply();
    auto shot = getScreenCapture();
    shot->expectColor(Rect(0, 0, 32, 32), Color::RED);
    shot->expectBorder(Rect(0, 0, 32, 32), Color::BLACK);
}

TEST_P(LayerRenderTypeTransactionTest, SetCropOutOfBounds_BufferState) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(
            layer = createLayer("test", 32, 64, ISurfaceComposerClient::eFXSurfaceBufferState));
    sp<GraphicBuffer> buffer =
            new GraphicBuffer(32, 64, PIXEL_FORMAT_RGBA_8888, 1,
                              BufferUsage::CPU_READ_OFTEN | BufferUsage::CPU_WRITE_OFTEN |
                                      BufferUsage::COMPOSER_OVERLAY,
                              "test");
    fillGraphicBufferColor(buffer, Rect(0, 0, 32, 16), Color::BLUE);
    fillGraphicBufferColor(buffer, Rect(0, 16, 32, 64), Color::RED);

    Transaction().setFrame(layer, Rect(0, 0, 64, 64)).apply();

    Transaction().setBuffer(layer, buffer).apply();

    // Partially out of bounds in the negative (upper left) direction
    Transaction().setCrop(layer, Rect(-128, -128, 32, 16)).apply();
    {
        SCOPED_TRACE("out of bounds, negative (upper left) direction");
        auto shot = getScreenCapture();
        shot->expectColor(Rect(0, 0, 64, 64), Color::BLUE);
        shot->expectBorder(Rect(0, 0, 64, 64), Color::BLACK);
    }

    // Partially out of bounds in the positive (lower right) direction
    Transaction().setCrop(layer, Rect(0, 16, 128, 128)).apply();
    {
        SCOPED_TRACE("out of bounds, positive (lower right) direction");
        auto shot = getScreenCapture();
        shot->expectColor(Rect(0, 0, 64, 64), Color::RED);
        shot->expectBorder(Rect(0, 0, 64, 64), Color::BLACK);
    }

    // Fully out of buffer space bounds
    Transaction().setCrop(layer, Rect(-128, -128, -1, -1)).apply();
    {
        SCOPED_TRACE("Fully out of bounds");
        auto shot = getScreenCapture();
        shot->expectColor(Rect(0, 0, 64, 16), Color::BLUE);
        shot->expectColor(Rect(0, 16, 64, 64), Color::RED);
        shot->expectBorder(Rect(0, 0, 64, 64), Color::BLACK);
    }
}

TEST_P(LayerRenderTypeTransactionTest, SetCropWithTranslation_BufferQueue) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createLayer("test", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(layer, Color::RED, 32, 32));

    const Point position(32, 32);
    const Rect crop(8, 8, 24, 24);
    Transaction().setPosition(layer, position.x, position.y).setCrop_legacy(layer, crop).apply();
    auto shot = getScreenCapture();
    shot->expectColor(crop + position, Color::RED);
    shot->expectBorder(crop + position, Color::BLACK);
}

TEST_P(LayerRenderTypeTransactionTest, SetCropWithTranslation_BufferState) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(
            layer = createLayer("test", 32, 32, ISurfaceComposerClient::eFXSurfaceBufferState));
    ASSERT_NO_FATAL_FAILURE(fillBufferStateLayerColor(layer, Color::RED, 32, 32));

    const Rect frame(32, 32, 64, 64);
    const Rect crop(8, 8, 24, 24);
    Transaction().setFrame(layer, frame).setCrop(layer, crop).apply();
    auto shot = getScreenCapture();
    shot->expectColor(frame, Color::RED);
    shot->expectBorder(frame, Color::BLACK);
}

TEST_P(LayerRenderTypeTransactionTest, SetCropWithScale_BufferQueue) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createLayer("test", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(layer, Color::RED, 32, 32));

    // crop_legacy is affected by matrix
    Transaction()
            .setMatrix(layer, 2.0f, 0.0f, 0.0f, 2.0f)
            .setCrop_legacy(layer, Rect(8, 8, 24, 24))
            .apply();
    auto shot = getScreenCapture();
    shot->expectColor(Rect(16, 16, 48, 48), Color::RED);
    shot->expectBorder(Rect(16, 16, 48, 48), Color::BLACK);
}

TEST_P(LayerRenderTypeTransactionTest, SetCropWithResize_BufferQueue) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createLayer("test", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(layer, Color::RED, 32, 32));

    // setCrop_legacy is applied immediately by default, with or without resize pending
    Transaction().setCrop_legacy(layer, Rect(8, 8, 24, 24)).setSize(layer, 16, 16).apply();
    {
        SCOPED_TRACE("resize pending");
        auto shot = getScreenCapture();
        shot->expectColor(Rect(8, 8, 24, 24), Color::RED);
        shot->expectBorder(Rect(8, 8, 24, 24), Color::BLACK);
    }

    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(layer, Color::RED, 16, 16));
    {
        SCOPED_TRACE("resize applied");
        auto shot = getScreenCapture();
        shot->expectColor(Rect(8, 8, 16, 16), Color::RED);
        shot->expectBorder(Rect(8, 8, 16, 16), Color::BLACK);
    }
}

TEST_P(LayerRenderTypeTransactionTest, SetCropWithNextResize_BufferQueue) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createLayer("test", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(layer, Color::RED, 32, 32));

    // request setCrop_legacy to be applied with the next resize
    Transaction()
            .setCrop_legacy(layer, Rect(8, 8, 24, 24))
            .setGeometryAppliesWithResize(layer)
            .apply();
    {
        SCOPED_TRACE("waiting for next resize");
        getScreenCapture()->expectColor(Rect(0, 0, 32, 32), Color::RED);
    }

    Transaction().setCrop_legacy(layer, Rect(4, 4, 12, 12)).apply();
    {
        SCOPED_TRACE("pending crop modified");
        getScreenCapture()->expectColor(Rect(0, 0, 32, 32), Color::RED);
    }

    Transaction().setSize(layer, 16, 16).apply();
    {
        SCOPED_TRACE("resize pending");
        getScreenCapture()->expectColor(Rect(0, 0, 32, 32), Color::RED);
    }

    // finally resize
    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(layer, Color::RED, 16, 16));
    {
        SCOPED_TRACE("new crop applied");
        auto shot = getScreenCapture();
        shot->expectColor(Rect(4, 4, 12, 12), Color::RED);
        shot->expectBorder(Rect(4, 4, 12, 12), Color::BLACK);
    }
}

TEST_P(LayerRenderTypeTransactionTest, SetCropWithNextResizeScaleToWindow_BufferQueue) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createLayer("test", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(layer, Color::RED, 32, 32));

    // setCrop_legacy is not immediate even with SCALE_TO_WINDOW override
    Transaction()
            .setCrop_legacy(layer, Rect(4, 4, 12, 12))
            .setSize(layer, 16, 16)
            .setOverrideScalingMode(layer, NATIVE_WINDOW_SCALING_MODE_SCALE_TO_WINDOW)
            .setGeometryAppliesWithResize(layer)
            .apply();
    {
        SCOPED_TRACE("new crop pending");
        auto shot = getScreenCapture();
        shot->expectColor(Rect(0, 0, 16, 16), Color::RED);
        shot->expectBorder(Rect(0, 0, 16, 16), Color::BLACK);
    }

    // XXX crop is never latched without other geometry change (b/69315677)
    Transaction().setPosition(layer, 1, 0).setGeometryAppliesWithResize(layer).apply();
    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(layer, Color::RED, 16, 16));
    Transaction().setPosition(layer, 0, 0).apply();
    {
        SCOPED_TRACE("new crop applied");
        auto shot = getScreenCapture();
        shot->expectColor(Rect(4, 4, 12, 12), Color::RED);
        shot->expectBorder(Rect(4, 4, 12, 12), Color::BLACK);
    }
}

TEST_P(LayerRenderTypeTransactionTest, SetFrameBasic_BufferState) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(
            layer = createLayer("test", 32, 32, ISurfaceComposerClient::eFXSurfaceBufferState));
    ASSERT_NO_FATAL_FAILURE(fillBufferStateLayerColor(layer, Color::RED, 32, 32));
    const Rect frame(8, 8, 24, 24);

    Transaction().setFrame(layer, frame).apply();
    auto shot = getScreenCapture();
    shot->expectColor(frame, Color::RED);
    shot->expectBorder(frame, Color::BLACK);
}

TEST_P(LayerRenderTypeTransactionTest, SetFrameEmpty_BufferState) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(
            layer = createLayer("test", 32, 32, ISurfaceComposerClient::eFXSurfaceBufferState));
    ASSERT_NO_FATAL_FAILURE(fillBufferStateLayerColor(layer, Color::RED, 32, 32));

    {
        SCOPED_TRACE("empty rect");
        Transaction().setFrame(layer, Rect(8, 8, 8, 8)).apply();
        getScreenCapture()->expectColor(Rect(0, 0, 32, 32), Color::BLACK);
    }

    {
        SCOPED_TRACE("negative rect");
        Transaction().setFrame(layer, Rect(8, 8, 0, 0)).apply();
        getScreenCapture()->expectColor(Rect(0, 0, 32, 32), Color::BLACK);
    }
}

TEST_P(LayerRenderTypeTransactionTest, SetFrameDefaultParentless_BufferState) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(
            layer = createLayer("test", 32, 32, ISurfaceComposerClient::eFXSurfaceBufferState));
    ASSERT_NO_FATAL_FAILURE(fillBufferStateLayerColor(layer, Color::RED, 10, 10));

    // A parentless layer will default to a frame with the same size as the buffer
    auto shot = getScreenCapture();
    shot->expectColor(Rect(0, 0, mDisplayWidth, mDisplayHeight), Color::RED);
    shot->expectBorder(Rect(0, 0, mDisplayWidth, mDisplayHeight), Color::BLACK);
}

TEST_P(LayerRenderTypeTransactionTest, SetFrameDefaultBSParent_BufferState) {
    sp<SurfaceControl> parent, child;
    ASSERT_NO_FATAL_FAILURE(
            parent = createLayer("test", 32, 32, ISurfaceComposerClient::eFXSurfaceBufferState));
    ASSERT_NO_FATAL_FAILURE(fillBufferStateLayerColor(parent, Color::RED, 32, 32));
    Transaction().setFrame(parent, Rect(0, 0, 32, 32)).apply();

    ASSERT_NO_FATAL_FAILURE(
            child = createLayer("test", 32, 32, ISurfaceComposerClient::eFXSurfaceBufferState));
    ASSERT_NO_FATAL_FAILURE(fillBufferStateLayerColor(child, Color::BLUE, 10, 10));

    Transaction().reparent(child, parent->getHandle()).apply();

    // A layer will default to the frame of its parent
    auto shot = getScreenCapture();
    shot->expectColor(Rect(0, 0, 32, 32), Color::BLUE);
    shot->expectBorder(Rect(0, 0, 32, 32), Color::BLACK);
}

TEST_P(LayerRenderTypeTransactionTest, SetFrameDefaultBQParent_BufferState) {
    sp<SurfaceControl> parent, child;
    ASSERT_NO_FATAL_FAILURE(parent = createLayer("test", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(parent, Color::RED, 32, 32));

    ASSERT_NO_FATAL_FAILURE(
            child = createLayer("test", 32, 32, ISurfaceComposerClient::eFXSurfaceBufferState));
    ASSERT_NO_FATAL_FAILURE(fillBufferStateLayerColor(child, Color::BLUE, 10, 10));

    Transaction().reparent(child, parent->getHandle()).apply();

    // A layer will default to the frame of its parent
    auto shot = getScreenCapture();
    shot->expectColor(Rect(0, 0, 32, 32), Color::BLUE);
    shot->expectBorder(Rect(0, 0, 32, 32), Color::BLACK);
}

TEST_P(LayerRenderTypeTransactionTest, SetFrameUpdate_BufferState) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(
            layer = createLayer("test", 32, 32, ISurfaceComposerClient::eFXSurfaceBufferState));
    ASSERT_NO_FATAL_FAILURE(fillBufferStateLayerColor(layer, Color::RED, 32, 32));
    Transaction().setFrame(layer, Rect(0, 0, 32, 32)).apply();

    std::this_thread::sleep_for(500ms);

    Transaction().setFrame(layer, Rect(16, 16, 48, 48)).apply();

    auto shot = getScreenCapture();
    shot->expectColor(Rect(16, 16, 48, 48), Color::RED);
    shot->expectBorder(Rect(16, 16, 48, 48), Color::BLACK);
}

TEST_P(LayerRenderTypeTransactionTest, SetFrameOutsideBounds_BufferState) {
    sp<SurfaceControl> parent, child;
    ASSERT_NO_FATAL_FAILURE(
            parent = createLayer("test", 32, 32, ISurfaceComposerClient::eFXSurfaceBufferState));
    ASSERT_NO_FATAL_FAILURE(
            child = createLayer("test", 32, 32, ISurfaceComposerClient::eFXSurfaceBufferState));
    Transaction().reparent(child, parent->getHandle()).apply();

    ASSERT_NO_FATAL_FAILURE(fillBufferStateLayerColor(parent, Color::RED, 32, 32));
    Transaction().setFrame(parent, Rect(0, 0, 32, 32)).apply();

    ASSERT_NO_FATAL_FAILURE(fillBufferStateLayerColor(child, Color::BLUE, 10, 10));
    Transaction().setFrame(child, Rect(0, 16, 32, 32)).apply();

    auto shot = getScreenCapture();
    shot->expectColor(Rect(0, 0, 32, 16), Color::RED);
    shot->expectColor(Rect(0, 16, 32, 32), Color::BLUE);
    shot->expectBorder(Rect(0, 0, 32, 32), Color::BLACK);
}

TEST_P(LayerRenderTypeTransactionTest, SetBufferBasic_BufferState) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(
            layer = createLayer("test", 32, 32, ISurfaceComposerClient::eFXSurfaceBufferState));

    ASSERT_NO_FATAL_FAILURE(fillBufferStateLayerColor(layer, Color::RED, 32, 32));

    auto shot = getScreenCapture();
    shot->expectColor(Rect(0, 0, mDisplayWidth, mDisplayHeight), Color::RED);
    shot->expectBorder(Rect(0, 0, mDisplayWidth, mDisplayHeight), Color::BLACK);
}

TEST_P(LayerRenderTypeTransactionTest, SetBufferMultipleBuffers_BufferState) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(
            layer = createLayer("test", 32, 32, ISurfaceComposerClient::eFXSurfaceBufferState));

    ASSERT_NO_FATAL_FAILURE(fillBufferStateLayerColor(layer, Color::RED, 32, 32));

    {
        SCOPED_TRACE("set buffer 1");
        auto shot = getScreenCapture();
        shot->expectColor(Rect(0, 0, mDisplayWidth, mDisplayHeight), Color::RED);
        shot->expectBorder(Rect(0, 0, mDisplayWidth, mDisplayHeight), Color::BLACK);
    }

    ASSERT_NO_FATAL_FAILURE(fillBufferStateLayerColor(layer, Color::BLUE, 32, 32));

    {
        SCOPED_TRACE("set buffer 2");
        auto shot = getScreenCapture();
        shot->expectColor(Rect(0, 0, mDisplayWidth, mDisplayHeight), Color::BLUE);
        shot->expectBorder(Rect(0, 0, mDisplayWidth, mDisplayHeight), Color::BLACK);
    }

    ASSERT_NO_FATAL_FAILURE(fillBufferStateLayerColor(layer, Color::RED, 32, 32));

    {
        SCOPED_TRACE("set buffer 3");
        auto shot = getScreenCapture();
        shot->expectColor(Rect(0, 0, mDisplayWidth, mDisplayHeight), Color::RED);
        shot->expectBorder(Rect(0, 0, mDisplayWidth, mDisplayHeight), Color::BLACK);
    }
}

TEST_P(LayerRenderTypeTransactionTest, SetBufferMultipleLayers_BufferState) {
    sp<SurfaceControl> layer1;
    ASSERT_NO_FATAL_FAILURE(
            layer1 = createLayer("test", 64, 64, ISurfaceComposerClient::eFXSurfaceBufferState));

    sp<SurfaceControl> layer2;
    ASSERT_NO_FATAL_FAILURE(
            layer2 = createLayer("test", 32, 32, ISurfaceComposerClient::eFXSurfaceBufferState));

    ASSERT_NO_FATAL_FAILURE(fillBufferStateLayerColor(layer1, Color::RED, 64, 64));

    Transaction().setFrame(layer1, Rect(0, 0, 64, 64)).apply();
    {
        SCOPED_TRACE("set layer 1 buffer red");
        auto shot = getScreenCapture();
        shot->expectColor(Rect(0, 0, 64, 64), Color::RED);
    }

    ASSERT_NO_FATAL_FAILURE(fillBufferStateLayerColor(layer2, Color::BLUE, 32, 32));

    Transaction().setFrame(layer2, Rect(0, 0, 32, 32)).apply();
    {
        SCOPED_TRACE("set layer 2 buffer blue");
        auto shot = getScreenCapture();
        shot->expectColor(Rect(0, 0, 32, 32), Color::BLUE);
        shot->expectColor(Rect(0, 32, 64, 64), Color::RED);
        shot->expectColor(Rect(0, 32, 32, 64), Color::RED);
    }

    ASSERT_NO_FATAL_FAILURE(fillBufferStateLayerColor(layer1, Color::GREEN, 64, 64));
    {
        SCOPED_TRACE("set layer 1 buffer green");
        auto shot = getScreenCapture();
        shot->expectColor(Rect(0, 0, 32, 32), Color::BLUE);
        shot->expectColor(Rect(0, 32, 64, 64), Color::GREEN);
        shot->expectColor(Rect(0, 32, 32, 64), Color::GREEN);
    }

    ASSERT_NO_FATAL_FAILURE(fillBufferStateLayerColor(layer2, Color::WHITE, 32, 32));

    {
        SCOPED_TRACE("set layer 2 buffer white");
        auto shot = getScreenCapture();
        shot->expectColor(Rect(0, 0, 32, 32), Color::WHITE);
        shot->expectColor(Rect(0, 32, 64, 64), Color::GREEN);
        shot->expectColor(Rect(0, 32, 32, 64), Color::GREEN);
    }
}

TEST_P(LayerRenderTypeTransactionTest, SetBufferCaching_BufferState) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(
            layer = createLayer("test", 32, 32, ISurfaceComposerClient::eFXSurfaceBufferState));

    std::array<Color, 4> colors = {Color::RED, Color::BLUE, Color::WHITE, Color::GREEN};

    std::array<sp<GraphicBuffer>, 10> buffers;

    size_t idx = 0;
    for (auto& buffer : buffers) {
        buffer = new GraphicBuffer(32, 32, PIXEL_FORMAT_RGBA_8888, 1,
                                   BufferUsage::CPU_READ_OFTEN | BufferUsage::CPU_WRITE_OFTEN |
                                           BufferUsage::COMPOSER_OVERLAY,
                                   "test");
        Color color = colors[idx % colors.size()];
        fillGraphicBufferColor(buffer, Rect(0, 0, 32, 32), color);
        idx++;
    }

    // Set each buffer twice. The first time adds it to the cache, the second time tests that the
    // cache is working.
    idx = 0;
    for (auto& buffer : buffers) {
        for (int i = 0; i < 2; i++) {
            Transaction().setBuffer(layer, buffer).apply();

            Color color = colors[idx % colors.size()];
            auto shot = screenshot();
            shot->expectColor(Rect(0, 0, mDisplayWidth, mDisplayHeight), color);
            shot->expectBorder(Rect(0, 0, mDisplayWidth, mDisplayHeight), Color::BLACK);
        }
        idx++;
    }
}

TEST_P(LayerRenderTypeTransactionTest, SetBufferCaching_LeastRecentlyUsed_BufferState) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(
            layer = createLayer("test", 32, 32, ISurfaceComposerClient::eFXSurfaceBufferState));

    std::array<Color, 4> colors = {Color::RED, Color::BLUE, Color::WHITE, Color::GREEN};

    std::array<sp<GraphicBuffer>, 70> buffers;

    size_t idx = 0;
    for (auto& buffer : buffers) {
        buffer = new GraphicBuffer(32, 32, PIXEL_FORMAT_RGBA_8888, 1,
                                   BufferUsage::CPU_READ_OFTEN | BufferUsage::CPU_WRITE_OFTEN |
                                           BufferUsage::COMPOSER_OVERLAY,
                                   "test");
        Color color = colors[idx % colors.size()];
        fillGraphicBufferColor(buffer, Rect(0, 0, 32, 32), color);
        idx++;
    }

    // Set each buffer twice. The first time adds it to the cache, the second time tests that the
    // cache is working.
    idx = 0;
    for (auto& buffer : buffers) {
        for (int i = 0; i < 2; i++) {
            Transaction().setBuffer(layer, buffer).apply();

            Color color = colors[idx % colors.size()];
            auto shot = screenshot();
            shot->expectColor(Rect(0, 0, mDisplayWidth, mDisplayHeight), color);
            shot->expectBorder(Rect(0, 0, mDisplayWidth, mDisplayHeight), Color::BLACK);
        }
        idx++;
    }
}

TEST_P(LayerRenderTypeTransactionTest, SetBufferCaching_DestroyedBuffer_BufferState) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(
            layer = createLayer("test", 32, 32, ISurfaceComposerClient::eFXSurfaceBufferState));

    std::array<Color, 4> colors = {Color::RED, Color::BLUE, Color::WHITE, Color::GREEN};

    std::array<sp<GraphicBuffer>, 65> buffers;

    size_t idx = 0;
    for (auto& buffer : buffers) {
        buffer = new GraphicBuffer(32, 32, PIXEL_FORMAT_RGBA_8888, 1,
                                   BufferUsage::CPU_READ_OFTEN | BufferUsage::CPU_WRITE_OFTEN |
                                           BufferUsage::COMPOSER_OVERLAY,
                                   "test");
        Color color = colors[idx % colors.size()];
        fillGraphicBufferColor(buffer, Rect(0, 0, 32, 32), color);
        idx++;
    }

    // Set each buffer twice. The first time adds it to the cache, the second time tests that the
    // cache is working.
    idx = 0;
    for (auto& buffer : buffers) {
        for (int i = 0; i < 2; i++) {
            Transaction().setBuffer(layer, buffer).apply();

            Color color = colors[idx % colors.size()];
            auto shot = screenshot();
            shot->expectColor(Rect(0, 0, mDisplayWidth, mDisplayHeight), color);
            shot->expectBorder(Rect(0, 0, mDisplayWidth, mDisplayHeight), Color::BLACK);
        }
        if (idx == 0) {
            buffers[0].clear();
        }
        idx++;
    }
}

TEST_P(LayerRenderTypeTransactionTest, SetTransformRotate90_BufferState) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(
            layer = createLayer("test", 32, 32, ISurfaceComposerClient::eFXSurfaceBufferState));

    ASSERT_NO_FATAL_FAILURE(fillBufferStateLayerQuadrant(layer, 32, 32, Color::RED, Color::GREEN,
                                                         Color::BLUE, Color::WHITE));

    Transaction()
            .setFrame(layer, Rect(0, 0, 32, 32))
            .setTransform(layer, NATIVE_WINDOW_TRANSFORM_ROT_90)
            .apply();

    getScreenCapture()->expectQuadrant(Rect(0, 0, 32, 32), Color::BLUE, Color::RED, Color::WHITE,
                                       Color::GREEN, true /* filtered */);
}

TEST_P(LayerRenderTypeTransactionTest, SetTransformFlipH_BufferState) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(
            layer = createLayer("test", 32, 32, ISurfaceComposerClient::eFXSurfaceBufferState));

    ASSERT_NO_FATAL_FAILURE(fillBufferStateLayerQuadrant(layer, 32, 32, Color::RED, Color::GREEN,
                                                         Color::BLUE, Color::WHITE));

    Transaction()
            .setFrame(layer, Rect(0, 0, 32, 32))
            .setTransform(layer, NATIVE_WINDOW_TRANSFORM_FLIP_H)
            .apply();

    getScreenCapture()->expectQuadrant(Rect(0, 0, 32, 32), Color::GREEN, Color::RED, Color::WHITE,
                                       Color::BLUE, true /* filtered */);
}

TEST_P(LayerRenderTypeTransactionTest, SetTransformFlipV_BufferState) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(
            layer = createLayer("test", 32, 32, ISurfaceComposerClient::eFXSurfaceBufferState));

    ASSERT_NO_FATAL_FAILURE(fillBufferStateLayerQuadrant(layer, 32, 32, Color::RED, Color::GREEN,
                                                         Color::BLUE, Color::WHITE));

    Transaction()
            .setFrame(layer, Rect(0, 0, 32, 32))
            .setTransform(layer, NATIVE_WINDOW_TRANSFORM_FLIP_V)
            .apply();

    getScreenCapture()->expectQuadrant(Rect(0, 0, 32, 32), Color::BLUE, Color::WHITE, Color::RED,
                                       Color::GREEN, true /* filtered */);
}

TEST_F(LayerTransactionTest, SetTransformToDisplayInverse_BufferState) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(
            layer = createLayer("test", 32, 32, ISurfaceComposerClient::eFXSurfaceBufferState));

    Transaction().setTransformToDisplayInverse(layer, false).apply();

    ASSERT_NO_FATAL_FAILURE(fillBufferStateLayerColor(layer, Color::GREEN, 32, 32));

    Transaction().setTransformToDisplayInverse(layer, true).apply();
}

TEST_P(LayerRenderTypeTransactionTest, SetFenceBasic_BufferState) {
    sp<SurfaceControl> layer;
    Transaction transaction;
    ASSERT_NO_FATAL_FAILURE(
            layer = createLayer("test", 32, 32, ISurfaceComposerClient::eFXSurfaceBufferState));

    sp<GraphicBuffer> buffer =
            new GraphicBuffer(32, 32, PIXEL_FORMAT_RGBA_8888, 1,
                              BufferUsage::CPU_READ_OFTEN | BufferUsage::CPU_WRITE_OFTEN |
                                      BufferUsage::COMPOSER_OVERLAY,
                              "test");
    fillGraphicBufferColor(buffer, Rect(0, 0, 32, 32), Color::RED);

    sp<Fence> fence;
    if (getBuffer(nullptr, &fence) != NO_ERROR) {
        GTEST_SUCCEED() << "test not supported";
        return;
    }

    Transaction().setBuffer(layer, buffer).setAcquireFence(layer, fence).apply();

    status_t status = fence->wait(1000);
    ASSERT_NE(static_cast<status_t>(Fence::Status::Unsignaled), status);
    std::this_thread::sleep_for(200ms);

    auto shot = getScreenCapture();
    shot->expectColor(Rect(0, 0, mDisplayWidth, mDisplayHeight), Color::RED);
    shot->expectBorder(Rect(0, 0, mDisplayWidth, mDisplayHeight), Color::BLACK);
}

TEST_P(LayerRenderTypeTransactionTest, SetFenceNull_BufferState) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(
            layer = createLayer("test", 32, 32, ISurfaceComposerClient::eFXSurfaceBufferState));

    sp<GraphicBuffer> buffer =
            new GraphicBuffer(32, 32, PIXEL_FORMAT_RGBA_8888, 1,
                              BufferUsage::CPU_READ_OFTEN | BufferUsage::CPU_WRITE_OFTEN |
                                      BufferUsage::COMPOSER_OVERLAY,
                              "test");
    fillGraphicBufferColor(buffer, Rect(0, 0, 32, 32), Color::RED);

    sp<Fence> fence = Fence::NO_FENCE;

    Transaction()
            .setBuffer(layer, buffer)
            .setAcquireFence(layer, fence)
            .apply();

    auto shot = getScreenCapture();
    shot->expectColor(Rect(0, 0, mDisplayWidth, mDisplayHeight), Color::RED);
    shot->expectBorder(Rect(0, 0, mDisplayWidth, mDisplayHeight), Color::BLACK);
}

TEST_P(LayerRenderTypeTransactionTest, SetDataspaceBasic_BufferState) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(
            layer = createLayer("test", 32, 32, ISurfaceComposerClient::eFXSurfaceBufferState));

    sp<GraphicBuffer> buffer =
            new GraphicBuffer(32, 32, PIXEL_FORMAT_RGBA_8888, 1,
                              BufferUsage::CPU_READ_OFTEN | BufferUsage::CPU_WRITE_OFTEN |
                                      BufferUsage::COMPOSER_OVERLAY,
                              "test");
    fillGraphicBufferColor(buffer, Rect(0, 0, 32, 32), Color::RED);

    Transaction()
            .setBuffer(layer, buffer)
            .setDataspace(layer, ui::Dataspace::UNKNOWN)
            .apply();

    auto shot = getScreenCapture();
    shot->expectColor(Rect(0, 0, mDisplayWidth, mDisplayHeight), Color::RED);
    shot->expectBorder(Rect(0, 0, mDisplayWidth, mDisplayHeight), Color::BLACK);
}

TEST_P(LayerRenderTypeTransactionTest, SetHdrMetadataBasic_BufferState) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(
            layer = createLayer("test", 32, 32, ISurfaceComposerClient::eFXSurfaceBufferState));

    sp<GraphicBuffer> buffer =
            new GraphicBuffer(32, 32, PIXEL_FORMAT_RGBA_8888, 1,
                              BufferUsage::CPU_READ_OFTEN | BufferUsage::CPU_WRITE_OFTEN |
                                      BufferUsage::COMPOSER_OVERLAY,
                              "test");
    fillGraphicBufferColor(buffer, Rect(0, 0, 32, 32), Color::RED);

    HdrMetadata hdrMetadata;
    hdrMetadata.validTypes = 0;
    Transaction()
            .setBuffer(layer, buffer)
            .setHdrMetadata(layer, hdrMetadata)
            .apply();

    auto shot = getScreenCapture();
    shot->expectColor(Rect(0, 0, mDisplayWidth, mDisplayHeight), Color::RED);
    shot->expectBorder(Rect(0, 0, mDisplayWidth, mDisplayHeight), Color::BLACK);
}

TEST_P(LayerRenderTypeTransactionTest, SetSurfaceDamageRegionBasic_BufferState) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(
            layer = createLayer("test", 32, 32, ISurfaceComposerClient::eFXSurfaceBufferState));

    sp<GraphicBuffer> buffer =
            new GraphicBuffer(32, 32, PIXEL_FORMAT_RGBA_8888, 1,
                              BufferUsage::CPU_READ_OFTEN | BufferUsage::CPU_WRITE_OFTEN |
                                      BufferUsage::COMPOSER_OVERLAY,
                              "test");
    fillGraphicBufferColor(buffer, Rect(0, 0, 32, 32), Color::RED);

    Region region;
    region.set(32, 32);
    Transaction()
            .setBuffer(layer, buffer)
            .setSurfaceDamageRegion(layer, region)
            .apply();

    auto shot = getScreenCapture();
    shot->expectColor(Rect(0, 0, mDisplayWidth, mDisplayHeight), Color::RED);
    shot->expectBorder(Rect(0, 0, mDisplayWidth, mDisplayHeight), Color::BLACK);
}

TEST_P(LayerRenderTypeTransactionTest, SetApiBasic_BufferState) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(
            layer = createLayer("test", 32, 32, ISurfaceComposerClient::eFXSurfaceBufferState));

    sp<GraphicBuffer> buffer =
            new GraphicBuffer(32, 32, PIXEL_FORMAT_RGBA_8888, 1,
                              BufferUsage::CPU_READ_OFTEN | BufferUsage::CPU_WRITE_OFTEN |
                                      BufferUsage::COMPOSER_OVERLAY,
                              "test");
    fillGraphicBufferColor(buffer, Rect(0, 0, 32, 32), Color::RED);

    Transaction()
            .setBuffer(layer, buffer)
            .setApi(layer, NATIVE_WINDOW_API_CPU)
            .apply();

    auto shot = getScreenCapture();
    shot->expectColor(Rect(0, 0, mDisplayWidth, mDisplayHeight), Color::RED);
    shot->expectBorder(Rect(0, 0, mDisplayWidth, mDisplayHeight), Color::BLACK);
}

TEST_F(LayerTransactionTest, SetSidebandStreamNull_BufferState) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(
            layer = createLayer("test", 32, 32, ISurfaceComposerClient::eFXSurfaceBufferState));

    // verify this doesn't cause a crash
    Transaction().setSidebandStream(layer, nullptr).apply();
}

TEST_F(LayerTransactionTest, ReparentToSelf) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createLayer("test", 32, 32));
    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(layer, Color::RED, 32, 32));
    Transaction().reparent(layer, layer->getHandle()).apply();

    {
        // We expect the transaction to be silently dropped, but for SurfaceFlinger
        // to still be functioning.
        SCOPED_TRACE("after reparent to self");
        const Rect rect(0, 0, 32, 32);
        auto shot = screenshot();
        shot->expectColor(rect, Color::RED);
        shot->expectBorder(rect, Color::BLACK);
    }
}

class ColorTransformHelper {
public:
    static void DegammaColorSingle(half& s) {
        if (s <= 0.03928f)
            s = s / 12.92f;
        else
            s = pow((s + 0.055f) / 1.055f, 2.4f);
    }

    static void DegammaColor(half3& color) {
        DegammaColorSingle(color.r);
        DegammaColorSingle(color.g);
        DegammaColorSingle(color.b);
    }

    static void GammaColorSingle(half& s) {
        if (s <= 0.0031308f) {
            s = s * 12.92f;
        } else {
            s = 1.055f * pow(s, (1.0f / 2.4f)) - 0.055f;
        }
    }

    static void GammaColor(half3& color) {
        GammaColorSingle(color.r);
        GammaColorSingle(color.g);
        GammaColorSingle(color.b);
    }

    static void applyMatrix(half3& color, const mat3& mat) {
        half3 ret = half3(0);

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                ret[i] = ret[i] + color[j] * mat[j][i];
            }
        }
        color = ret;
    }
};

TEST_P(LayerRenderTypeTransactionTest, SetColorTransformBasic) {
    sp<SurfaceControl> colorLayer;
    ASSERT_NO_FATAL_FAILURE(colorLayer =
                                    createLayer("test", 0 /* buffer width */, 0 /* buffer height */,
                                                ISurfaceComposerClient::eFXSurfaceColor));
    Transaction()
            .setCrop_legacy(colorLayer, Rect(0, 0, 32, 32))
            .setLayer(colorLayer, mLayerZBase + 1)
            .apply();
    {
        SCOPED_TRACE("default color");
        getScreenCapture()->expectColor(Rect(0, 0, 32, 32), Color::BLACK);
    }

    const half3 color(50.0f / 255.0f, 100.0f / 255.0f, 150.0f / 255.0f);
    half3 expected = color;
    mat3 matrix;
    matrix[0][0] = 0.3; matrix[1][0] = 0.59; matrix[2][0] = 0.11;
    matrix[0][1] = 0.3; matrix[1][1] = 0.59; matrix[2][1] = 0.11;
    matrix[0][2] = 0.3; matrix[1][2] = 0.59; matrix[2][2] = 0.11;

    // degamma before applying the matrix
    if (mColorManagementUsed) {
        ColorTransformHelper::DegammaColor(expected);
    }

    ColorTransformHelper::applyMatrix(expected, matrix);

    if (mColorManagementUsed) {
        ColorTransformHelper::GammaColor(expected);
    }

    const Color expectedColor = {uint8_t(expected.r * 255), uint8_t(expected.g * 255),
                                 uint8_t(expected.b * 255), 255};

    // this is handwavy, but the precison loss scaled by 255 (8-bit per
    // channel) should be less than one
    const uint8_t tolerance = 1;

    Transaction().setColor(colorLayer, color)
        .setColorTransform(colorLayer, matrix, vec3()).apply();
    {
        SCOPED_TRACE("new color");
        getScreenCapture()->expectColor(Rect(0, 0, 32, 32), expectedColor, tolerance);
    }
}

TEST_P(LayerRenderTypeTransactionTest, SetColorTransformOnParent) {
    sp<SurfaceControl> parentLayer;
    sp<SurfaceControl> colorLayer;
    ASSERT_NO_FATAL_FAILURE(parentLayer = createLayer("parent", 0 /* buffer width */,
                                                      0 /* buffer height */,
                                                      ISurfaceComposerClient::eFXSurfaceContainer));
    ASSERT_NO_FATAL_FAILURE(
            colorLayer = createLayer("test", 0 /* buffer width */, 0 /* buffer height */,
                                     ISurfaceComposerClient::eFXSurfaceColor, parentLayer.get()));

    Transaction()
            .setCrop_legacy(parentLayer, Rect(0, 0, 100, 100))
            .setCrop_legacy(colorLayer, Rect(0, 0, 32, 32))
            .setLayer(parentLayer, mLayerZBase + 1)
            .apply();
    {
        SCOPED_TRACE("default color");
        getScreenCapture()->expectColor(Rect(0, 0, 32, 32), Color::BLACK);
    }

    const half3 color(50.0f / 255.0f, 100.0f / 255.0f, 150.0f / 255.0f);
    half3 expected = color;
    mat3 matrix;
    matrix[0][0] = 0.3; matrix[1][0] = 0.59; matrix[2][0] = 0.11;
    matrix[0][1] = 0.3; matrix[1][1] = 0.59; matrix[2][1] = 0.11;
    matrix[0][2] = 0.3; matrix[1][2] = 0.59; matrix[2][2] = 0.11;

    // degamma before applying the matrix
    if (mColorManagementUsed) {
        ColorTransformHelper::DegammaColor(expected);
    }

    ColorTransformHelper::applyMatrix(expected, matrix);

    if (mColorManagementUsed) {
        ColorTransformHelper::GammaColor(expected);
    }

    const Color expectedColor = {uint8_t(expected.r * 255), uint8_t(expected.g * 255),
                                 uint8_t(expected.b * 255), 255};

    // this is handwavy, but the precison loss scaled by 255 (8-bit per
    // channel) should be less than one
    const uint8_t tolerance = 1;

    Transaction()
            .setColor(colorLayer, color)
            .setColorTransform(parentLayer, matrix, vec3())
            .apply();
    {
        SCOPED_TRACE("new color");
        getScreenCapture()->expectColor(Rect(0, 0, 32, 32), expectedColor, tolerance);
    }
}

TEST_P(LayerRenderTypeTransactionTest, SetColorTransformOnChildAndParent) {
    sp<SurfaceControl> parentLayer;
    sp<SurfaceControl> colorLayer;
    ASSERT_NO_FATAL_FAILURE(parentLayer = createLayer("parent", 0 /* buffer width */,
                                                      0 /* buffer height */,
                                                      ISurfaceComposerClient::eFXSurfaceContainer));
    ASSERT_NO_FATAL_FAILURE(
            colorLayer = createLayer("test", 0 /* buffer width */, 0 /* buffer height */,
                                     ISurfaceComposerClient::eFXSurfaceColor, parentLayer.get()));

    Transaction()
            .setCrop_legacy(parentLayer, Rect(0, 0, 100, 100))
            .setCrop_legacy(colorLayer, Rect(0, 0, 32, 32))
            .setLayer(parentLayer, mLayerZBase + 1)
            .apply();
    {
        SCOPED_TRACE("default color");
        getScreenCapture()->expectColor(Rect(0, 0, 32, 32), Color::BLACK);
    }

    const half3 color(50.0f / 255.0f, 100.0f / 255.0f, 150.0f / 255.0f);
    half3 expected = color;
    mat3 matrixChild;
    matrixChild[0][0] = 0.3; matrixChild[1][0] = 0.59; matrixChild[2][0] = 0.11;
    matrixChild[0][1] = 0.3; matrixChild[1][1] = 0.59; matrixChild[2][1] = 0.11;
    matrixChild[0][2] = 0.3; matrixChild[1][2] = 0.59; matrixChild[2][2] = 0.11;
    mat3 matrixParent;
    matrixParent[0][0] = 0.2; matrixParent[1][0] = 0.4; matrixParent[2][0] = 0.10;
    matrixParent[0][1] = 0.2; matrixParent[1][1] = 0.4; matrixParent[2][1] = 0.10;
    matrixParent[0][2] = 0.2; matrixParent[1][2] = 0.4; matrixParent[2][2] = 0.10;

    // degamma before applying the matrix
    if (mColorManagementUsed) {
        ColorTransformHelper::DegammaColor(expected);
    }

    ColorTransformHelper::applyMatrix(expected, matrixChild);
    ColorTransformHelper::applyMatrix(expected, matrixParent);

    if (mColorManagementUsed) {
        ColorTransformHelper::GammaColor(expected);
    }

    const Color expectedColor = {uint8_t(expected.r * 255), uint8_t(expected.g * 255),
                                 uint8_t(expected.b * 255), 255};

    // this is handwavy, but the precison loss scaled by 255 (8-bit per
    // channel) should be less than one
    const uint8_t tolerance = 1;

    Transaction()
            .setColor(colorLayer, color)
            .setColorTransform(parentLayer, matrixParent, vec3())
            .setColorTransform(colorLayer, matrixChild, vec3())
            .apply();
    {
        SCOPED_TRACE("new color");
        getScreenCapture()->expectColor(Rect(0, 0, 32, 32), expectedColor, tolerance);
    }
}

struct CallbackData {
    CallbackData() = default;
    CallbackData(nsecs_t time, const sp<Fence>& fence,
                 const std::vector<SurfaceControlStats>& stats)
          : latchTime(time), presentFence(fence), surfaceControlStats(stats) {}

    nsecs_t latchTime;
    sp<Fence> presentFence;
    std::vector<SurfaceControlStats> surfaceControlStats;
};

class ExpectedResult {
public:
    enum Transaction {
        NOT_PRESENTED = 0,
        PRESENTED,
    };

    enum Buffer {
        NOT_ACQUIRED = 0,
        ACQUIRED,
    };

    enum PreviousBuffer {
        NOT_RELEASED = 0,
        RELEASED,
        UNKNOWN,
    };

    void reset() {
        mTransactionResult = ExpectedResult::Transaction::NOT_PRESENTED;
        mExpectedSurfaceResults.clear();
    }

    void addSurface(ExpectedResult::Transaction transactionResult, const sp<SurfaceControl>& layer,
                    ExpectedResult::Buffer bufferResult = ACQUIRED,
                    ExpectedResult::PreviousBuffer previousBufferResult = NOT_RELEASED) {
        mTransactionResult = transactionResult;
        mExpectedSurfaceResults.emplace(std::piecewise_construct, std::forward_as_tuple(layer),
                                        std::forward_as_tuple(bufferResult, previousBufferResult));
    }

    void addSurfaces(ExpectedResult::Transaction transactionResult,
                     const std::vector<sp<SurfaceControl>>& layers,
                     ExpectedResult::Buffer bufferResult = ACQUIRED,
                     ExpectedResult::PreviousBuffer previousBufferResult = NOT_RELEASED) {
        for (const auto& layer : layers) {
            addSurface(transactionResult, layer, bufferResult, previousBufferResult);
        }
    }

    void addExpectedPresentTime(nsecs_t expectedPresentTime) {
        mExpectedPresentTime = expectedPresentTime;
    }

    void verifyCallbackData(const CallbackData& callbackData) const {
        const auto& [latchTime, presentFence, surfaceControlStats] = callbackData;
        if (mTransactionResult == ExpectedResult::Transaction::PRESENTED) {
            ASSERT_GE(latchTime, 0) << "bad latch time";
            ASSERT_NE(presentFence, nullptr);
            if (mExpectedPresentTime >= 0) {
                ASSERT_EQ(presentFence->wait(3000), NO_ERROR);
                ASSERT_GE(presentFence->getSignalTime(), mExpectedPresentTime - nsecs_t(5 * 1e6));
                // if the panel is running at 30 hz, at the worst case, our expected time just
                // misses vsync and we have to wait another 33.3ms
                ASSERT_LE(presentFence->getSignalTime(),
                          mExpectedPresentTime + nsecs_t(66.666666 * 1e6));
            }
        } else {
            ASSERT_EQ(presentFence, nullptr) << "transaction shouldn't have been presented";
            ASSERT_EQ(latchTime, -1) << "unpresented transactions shouldn't be latched";
        }

        ASSERT_EQ(surfaceControlStats.size(), mExpectedSurfaceResults.size())
                << "wrong number of surfaces";

        for (const auto& stats : surfaceControlStats) {
            ASSERT_NE(stats.surfaceControl, nullptr) << "returned null surface control";

            const auto& expectedSurfaceResult = mExpectedSurfaceResults.find(stats.surfaceControl);
            ASSERT_NE(expectedSurfaceResult, mExpectedSurfaceResults.end())
                    << "unexpected surface control";
            expectedSurfaceResult->second.verifySurfaceControlStats(stats, latchTime);
        }
    }

private:
    class ExpectedSurfaceResult {
    public:
        ExpectedSurfaceResult(ExpectedResult::Buffer bufferResult,
                              ExpectedResult::PreviousBuffer previousBufferResult)
              : mBufferResult(bufferResult), mPreviousBufferResult(previousBufferResult) {}

        void verifySurfaceControlStats(const SurfaceControlStats& surfaceControlStats,
                                       nsecs_t latchTime) const {
            const auto& [surfaceControl, acquireTime, previousReleaseFence] = surfaceControlStats;

            ASSERT_EQ(acquireTime > 0, mBufferResult == ExpectedResult::Buffer::ACQUIRED)
                    << "bad acquire time";
            ASSERT_LE(acquireTime, latchTime) << "acquire time should be <= latch time";

            if (mPreviousBufferResult == ExpectedResult::PreviousBuffer::RELEASED) {
                ASSERT_NE(previousReleaseFence, nullptr)
                        << "failed to set release prev buffer fence";
            } else if (mPreviousBufferResult == ExpectedResult::PreviousBuffer::NOT_RELEASED) {
                ASSERT_EQ(previousReleaseFence, nullptr)
                        << "should not have set released prev buffer fence";
            }
        }

    private:
        ExpectedResult::Buffer mBufferResult;
        ExpectedResult::PreviousBuffer mPreviousBufferResult;
    };

    struct SCHash {
        std::size_t operator()(const sp<SurfaceControl>& sc) const {
            return std::hash<IBinder*>{}(sc->getHandle().get());
        }
    };
    ExpectedResult::Transaction mTransactionResult = ExpectedResult::Transaction::NOT_PRESENTED;
    nsecs_t mExpectedPresentTime = -1;
    std::unordered_map<sp<SurfaceControl>, ExpectedSurfaceResult, SCHash> mExpectedSurfaceResults;
};

class CallbackHelper {
public:
    static void function(void* callbackContext, nsecs_t latchTime, const sp<Fence>& presentFence,
                         const std::vector<SurfaceControlStats>& stats) {
        if (!callbackContext) {
            ALOGE("failed to get callback context");
        }
        CallbackHelper* helper = static_cast<CallbackHelper*>(callbackContext);
        std::lock_guard lock(helper->mMutex);
        helper->mCallbackDataQueue.emplace(latchTime, presentFence, stats);
        helper->mConditionVariable.notify_all();
    }

    void getCallbackData(CallbackData* outData) {
        std::unique_lock lock(mMutex);

        if (mCallbackDataQueue.empty()) {
            ASSERT_NE(mConditionVariable.wait_for(lock, std::chrono::seconds(3)),
                      std::cv_status::timeout)
                    << "did not receive callback";
        }

        *outData = std::move(mCallbackDataQueue.front());
        mCallbackDataQueue.pop();
    }

    void verifyFinalState() {
        // Wait to see if there are extra callbacks
        std::this_thread::sleep_for(500ms);

        std::lock_guard lock(mMutex);
        EXPECT_EQ(mCallbackDataQueue.size(), 0) << "extra callbacks received";
        mCallbackDataQueue = {};
    }

    void* getContext() { return static_cast<void*>(this); }

    std::mutex mMutex;
    std::condition_variable mConditionVariable;
    std::queue<CallbackData> mCallbackDataQueue;
};

class LayerCallbackTest : public LayerTransactionTest {
public:
    virtual sp<SurfaceControl> createBufferStateLayer() {
        return createLayer(mClient, "test", 0, 0, ISurfaceComposerClient::eFXSurfaceBufferState);
    }

    static int fillTransaction(Transaction& transaction, CallbackHelper* callbackHelper,
                               const sp<SurfaceControl>& layer = nullptr, bool setBuffer = true,
                               bool setBackgroundColor = false) {
        if (layer) {
            sp<GraphicBuffer> buffer;
            sp<Fence> fence;
            if (setBuffer) {
                int err = getBuffer(&buffer, &fence);
                if (err != NO_ERROR) {
                    return err;
                }

                transaction.setBuffer(layer, buffer);
                transaction.setAcquireFence(layer, fence);
            }

            if (setBackgroundColor) {
                transaction.setBackgroundColor(layer, /*color*/ half3(1.0f, 0, 0), /*alpha*/ 1.0f,
                                               ui::Dataspace::UNKNOWN);
            }
        }

        transaction.addTransactionCompletedCallback(callbackHelper->function,
                                                    callbackHelper->getContext());
        return NO_ERROR;
    }

    static void waitForCallback(CallbackHelper& helper, const ExpectedResult& expectedResult,
                                bool finalState = false) {
        CallbackData callbackData;
        ASSERT_NO_FATAL_FAILURE(helper.getCallbackData(&callbackData));
        EXPECT_NO_FATAL_FAILURE(expectedResult.verifyCallbackData(callbackData));

        if (finalState) {
            ASSERT_NO_FATAL_FAILURE(helper.verifyFinalState());
        }
    }

    static void waitForCallbacks(CallbackHelper& helper,
                                 const std::vector<ExpectedResult>& expectedResults,
                                 bool finalState = false) {
        for (const auto& expectedResult : expectedResults) {
            waitForCallback(helper, expectedResult);
        }
        if (finalState) {
            ASSERT_NO_FATAL_FAILURE(helper.verifyFinalState());
        }
    }
};

TEST_F(LayerCallbackTest, BufferColor) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createBufferStateLayer());

    Transaction transaction;
    CallbackHelper callback;
    int err = fillTransaction(transaction, &callback, layer, true, true);
    if (err) {
        GTEST_SUCCEED() << "test not supported";
        return;
    }

    transaction.apply();

    ExpectedResult expected;
    expected.addSurface(ExpectedResult::Transaction::PRESENTED, layer);
    EXPECT_NO_FATAL_FAILURE(waitForCallback(callback, expected, true));
}

TEST_F(LayerCallbackTest, NoBufferNoColor) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createBufferStateLayer());

    Transaction transaction;
    CallbackHelper callback;
    int err = fillTransaction(transaction, &callback, layer, false, false);
    if (err) {
        GTEST_SUCCEED() << "test not supported";
        return;
    }

    transaction.setFrame(layer, Rect(0, 0, 32, 32)).apply();

    ExpectedResult expected;
    expected.addSurface(ExpectedResult::Transaction::NOT_PRESENTED, layer,
                        ExpectedResult::Buffer::NOT_ACQUIRED);
    EXPECT_NO_FATAL_FAILURE(waitForCallback(callback, expected, true));
}

TEST_F(LayerCallbackTest, BufferNoColor) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createBufferStateLayer());

    Transaction transaction;
    CallbackHelper callback;
    int err = fillTransaction(transaction, &callback, layer, true, false);
    if (err) {
        GTEST_SUCCEED() << "test not supported";
        return;
    }

    transaction.setFrame(layer, Rect(0, 0, 32, 32)).apply();

    ExpectedResult expected;
    expected.addSurface(ExpectedResult::Transaction::PRESENTED, layer);
    EXPECT_NO_FATAL_FAILURE(waitForCallback(callback, expected, true));
}

TEST_F(LayerCallbackTest, NoBufferColor) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createBufferStateLayer());

    Transaction transaction;
    CallbackHelper callback;
    int err = fillTransaction(transaction, &callback, layer, false, true);
    if (err) {
        GTEST_SUCCEED() << "test not supported";
        return;
    }

    transaction.setFrame(layer, Rect(0, 0, 32, 32)).apply();

    ExpectedResult expected;
    expected.addSurface(ExpectedResult::Transaction::PRESENTED, layer,
                        ExpectedResult::Buffer::NOT_ACQUIRED);
    EXPECT_NO_FATAL_FAILURE(waitForCallback(callback, expected, true));
}

TEST_F(LayerCallbackTest, NoStateChange) {
    Transaction transaction;
    CallbackHelper callback;
    int err = fillTransaction(transaction, &callback);
    if (err) {
        GTEST_SUCCEED() << "test not supported";
        return;
    }

    transaction.apply();

    ExpectedResult expected;
    EXPECT_NO_FATAL_FAILURE(waitForCallback(callback, expected, true));
}

TEST_F(LayerCallbackTest, OffScreen) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createBufferStateLayer());

    Transaction transaction;
    CallbackHelper callback;
    int err = fillTransaction(transaction, &callback, layer);
    if (err) {
        GTEST_SUCCEED() << "test not supported";
        return;
    }

    transaction.setFrame(layer, Rect(-100, -100, 100, 100)).apply();

    ExpectedResult expected;
    expected.addSurface(ExpectedResult::Transaction::PRESENTED, layer);
    EXPECT_NO_FATAL_FAILURE(waitForCallback(callback, expected, true));
}

TEST_F(LayerCallbackTest, MergeBufferNoColor) {
    sp<SurfaceControl> layer1, layer2;
    ASSERT_NO_FATAL_FAILURE(layer1 = createBufferStateLayer());
    ASSERT_NO_FATAL_FAILURE(layer2 = createBufferStateLayer());

    Transaction transaction1, transaction2;
    CallbackHelper callback1, callback2;
    int err = fillTransaction(transaction1, &callback1, layer1);
    if (err) {
        GTEST_SUCCEED() << "test not supported";
        return;
    }
    err = fillTransaction(transaction2, &callback2, layer2);
    if (err) {
        GTEST_SUCCEED() << "test not supported";
        return;
    }

    transaction1.setFrame(layer1, Rect(0, 0, 32, 32));
    transaction2.setFrame(layer2, Rect(32, 32, 64, 64)).merge(std::move(transaction1)).apply();

    ExpectedResult expected;
    expected.addSurfaces(ExpectedResult::Transaction::PRESENTED, {layer1, layer2});
    EXPECT_NO_FATAL_FAILURE(waitForCallback(callback1, expected, true));
    EXPECT_NO_FATAL_FAILURE(waitForCallback(callback2, expected, true));
}

TEST_F(LayerCallbackTest, MergeNoBufferColor) {
    sp<SurfaceControl> layer1, layer2;
    ASSERT_NO_FATAL_FAILURE(layer1 = createBufferStateLayer());
    ASSERT_NO_FATAL_FAILURE(layer2 = createBufferStateLayer());

    Transaction transaction1, transaction2;
    CallbackHelper callback1, callback2;
    int err = fillTransaction(transaction1, &callback1, layer1, false, true);
    if (err) {
        GTEST_SUCCEED() << "test not supported";
        return;
    }
    err = fillTransaction(transaction2, &callback2, layer2, false, true);
    if (err) {
        GTEST_SUCCEED() << "test not supported";
        return;
    }

    transaction1.setFrame(layer1, Rect(0, 0, 32, 32));
    transaction2.setFrame(layer2, Rect(32, 32, 64, 64)).merge(std::move(transaction1)).apply();

    ExpectedResult expected;
    expected.addSurfaces(ExpectedResult::Transaction::PRESENTED, {layer1, layer2},
                         ExpectedResult::Buffer::NOT_ACQUIRED);
    EXPECT_NO_FATAL_FAILURE(waitForCallback(callback1, expected, true));
    EXPECT_NO_FATAL_FAILURE(waitForCallback(callback2, expected, true));
}

TEST_F(LayerCallbackTest, MergeOneBufferOneColor) {
    sp<SurfaceControl> layer1, layer2;
    ASSERT_NO_FATAL_FAILURE(layer1 = createBufferStateLayer());
    ASSERT_NO_FATAL_FAILURE(layer2 = createBufferStateLayer());

    Transaction transaction1, transaction2;
    CallbackHelper callback1, callback2;
    int err = fillTransaction(transaction1, &callback1, layer1);
    if (err) {
        GTEST_SUCCEED() << "test not supported";
        return;
    }
    err = fillTransaction(transaction2, &callback2, layer2, false, true);
    if (err) {
        GTEST_SUCCEED() << "test not supported";
        return;
    }

    transaction1.setFrame(layer1, Rect(0, 0, 32, 32));
    transaction2.setFrame(layer2, Rect(32, 32, 64, 64)).merge(std::move(transaction1)).apply();

    ExpectedResult expected;
    expected.addSurface(ExpectedResult::Transaction::PRESENTED, layer1);
    expected.addSurface(ExpectedResult::Transaction::PRESENTED, layer2,
                        ExpectedResult::Buffer::NOT_ACQUIRED);
    EXPECT_NO_FATAL_FAILURE(waitForCallback(callback1, expected, true));
    EXPECT_NO_FATAL_FAILURE(waitForCallback(callback2, expected, true));
}
TEST_F(LayerCallbackTest, Merge_SameCallback) {
    sp<SurfaceControl> layer1, layer2;
    ASSERT_NO_FATAL_FAILURE(layer1 = createBufferStateLayer());
    ASSERT_NO_FATAL_FAILURE(layer2 = createBufferStateLayer());

    Transaction transaction1, transaction2;
    CallbackHelper callback;
    int err = fillTransaction(transaction1, &callback, layer1);
    if (err) {
        GTEST_SUCCEED() << "test not supported";
        return;
    }
    err = fillTransaction(transaction2, &callback, layer2);
    if (err) {
        GTEST_SUCCEED() << "test not supported";
        return;
    }

    transaction2.merge(std::move(transaction1)).apply();

    ExpectedResult expected;
    expected.addSurfaces(ExpectedResult::Transaction::PRESENTED, {layer1, layer2});
    EXPECT_NO_FATAL_FAILURE(waitForCallback(callback, expected));
    EXPECT_NO_FATAL_FAILURE(waitForCallback(callback, expected, true));
}

TEST_F(LayerCallbackTest, Merge_SameLayer) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createBufferStateLayer());

    Transaction transaction1, transaction2;
    CallbackHelper callback1, callback2;
    int err = fillTransaction(transaction1, &callback1, layer);
    if (err) {
        GTEST_SUCCEED() << "test not supported";
        return;
    }
    err = fillTransaction(transaction2, &callback2, layer);
    if (err) {
        GTEST_SUCCEED() << "test not supported";
        return;
    }

    transaction2.merge(std::move(transaction1)).apply();

    ExpectedResult expected;
    expected.addSurface(ExpectedResult::Transaction::PRESENTED, layer);
    EXPECT_NO_FATAL_FAILURE(waitForCallback(callback1, expected, true));
    EXPECT_NO_FATAL_FAILURE(waitForCallback(callback2, expected, true));
}

TEST_F(LayerCallbackTest, Merge_DifferentClients) {
    sp<SurfaceComposerClient> client1(new SurfaceComposerClient),
            client2(new SurfaceComposerClient);

    ASSERT_EQ(NO_ERROR, client1->initCheck()) << "failed to create SurfaceComposerClient";
    ASSERT_EQ(NO_ERROR, client2->initCheck()) << "failed to create SurfaceComposerClient";

    sp<SurfaceControl> layer1, layer2;
    ASSERT_NO_FATAL_FAILURE(layer1 = createLayer(client1, "test", 0, 0,
                                                 ISurfaceComposerClient::eFXSurfaceBufferState));
    ASSERT_NO_FATAL_FAILURE(layer2 = createLayer(client2, "test", 0, 0,
                                                 ISurfaceComposerClient::eFXSurfaceBufferState));

    Transaction transaction1, transaction2;
    CallbackHelper callback1, callback2;
    int err = fillTransaction(transaction1, &callback1, layer1);
    if (err) {
        GTEST_SUCCEED() << "test not supported";
        return;
    }
    err = fillTransaction(transaction2, &callback2, layer2);
    if (err) {
        GTEST_SUCCEED() << "test not supported";
        return;
    }

    transaction1.setFrame(layer1, Rect(0, 0, 32, 32));
    transaction2.setFrame(layer2, Rect(32, 32, 64, 64)).merge(std::move(transaction1)).apply();

    ExpectedResult expected;
    expected.addSurfaces(ExpectedResult::Transaction::PRESENTED, {layer1, layer2});
    EXPECT_NO_FATAL_FAILURE(waitForCallback(callback1, expected, true));
    EXPECT_NO_FATAL_FAILURE(waitForCallback(callback2, expected, true));
}

TEST_F(LayerCallbackTest, MultipleTransactions) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createBufferStateLayer());

    Transaction transaction;
    CallbackHelper callback;
    for (size_t i = 0; i < 10; i++) {
        int err = fillTransaction(transaction, &callback, layer);
        if (err) {
            GTEST_SUCCEED() << "test not supported";
            return;
        }

        transaction.apply();

        ExpectedResult expected;
        expected.addSurface(ExpectedResult::Transaction::PRESENTED, layer,
                            ExpectedResult::Buffer::ACQUIRED,
                            (i == 0) ? ExpectedResult::PreviousBuffer::NOT_RELEASED
                                     : ExpectedResult::PreviousBuffer::RELEASED);
        EXPECT_NO_FATAL_FAILURE(waitForCallback(callback, expected));
    }
    ASSERT_NO_FATAL_FAILURE(callback.verifyFinalState());
}

TEST_F(LayerCallbackTest, MultipleTransactions_NoStateChange) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createBufferStateLayer());

    Transaction transaction;
    CallbackHelper callback;
    for (size_t i = 0; i < 10; i++) {
        ExpectedResult expected;

        if (i == 0) {
            int err = fillTransaction(transaction, &callback, layer);
            if (err) {
                GTEST_SUCCEED() << "test not supported";
                return;
            }
            expected.addSurface(ExpectedResult::Transaction::PRESENTED, layer);
        } else {
            int err = fillTransaction(transaction, &callback);
            if (err) {
                GTEST_SUCCEED() << "test not supported";
                return;
            }
        }

        transaction.apply();

        EXPECT_NO_FATAL_FAILURE(waitForCallback(callback, expected));
    }
    ASSERT_NO_FATAL_FAILURE(callback.verifyFinalState());
}

TEST_F(LayerCallbackTest, MultipleTransactions_SameStateChange) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createBufferStateLayer());

    Transaction transaction;
    CallbackHelper callback;
    for (size_t i = 0; i < 10; i++) {
        if (i == 0) {
            int err = fillTransaction(transaction, &callback, layer);
            if (err) {
                GTEST_SUCCEED() << "test not supported";
                return;
            }
        } else {
            int err = fillTransaction(transaction, &callback);
            if (err) {
                GTEST_SUCCEED() << "test not supported";
                return;
            }
        }

        transaction.setFrame(layer, Rect(0, 0, 32, 32)).apply();

        ExpectedResult expected;
        expected.addSurface((i == 0) ? ExpectedResult::Transaction::PRESENTED
                                     : ExpectedResult::Transaction::NOT_PRESENTED,
                            layer,
                            (i == 0) ? ExpectedResult::Buffer::ACQUIRED
                                     : ExpectedResult::Buffer::NOT_ACQUIRED);
        EXPECT_NO_FATAL_FAILURE(waitForCallback(callback, expected, i == 0));
    }
    ASSERT_NO_FATAL_FAILURE(callback.verifyFinalState());
}

TEST_F(LayerCallbackTest, MultipleTransactions_Merge) {
    sp<SurfaceControl> layer1, layer2;
    ASSERT_NO_FATAL_FAILURE(layer1 = createBufferStateLayer());
    ASSERT_NO_FATAL_FAILURE(layer2 = createBufferStateLayer());

    Transaction transaction1, transaction2;
    CallbackHelper callback1, callback2;
    for (size_t i = 0; i < 10; i++) {
        int err = fillTransaction(transaction1, &callback1, layer1);
        if (err) {
            GTEST_SUCCEED() << "test not supported";
            return;
        }
        err = fillTransaction(transaction2, &callback2, layer2);
        if (err) {
            GTEST_SUCCEED() << "test not supported";
            return;
        }

        transaction1.setFrame(layer1, Rect(0, 0, 32, 32));
        transaction2.setFrame(layer2, Rect(32, 32, 64, 64)).merge(std::move(transaction1)).apply();

        ExpectedResult expected;
        expected.addSurfaces(ExpectedResult::Transaction::PRESENTED, {layer1, layer2},
                             ExpectedResult::Buffer::ACQUIRED,
                             (i == 0) ? ExpectedResult::PreviousBuffer::NOT_RELEASED
                                      : ExpectedResult::PreviousBuffer::RELEASED);
        EXPECT_NO_FATAL_FAILURE(waitForCallback(callback1, expected));
        EXPECT_NO_FATAL_FAILURE(waitForCallback(callback2, expected));
    }
    ASSERT_NO_FATAL_FAILURE(callback1.verifyFinalState());
    ASSERT_NO_FATAL_FAILURE(callback2.verifyFinalState());
}

TEST_F(LayerCallbackTest, MultipleTransactions_Merge_DifferentClients) {
    sp<SurfaceComposerClient> client1(new SurfaceComposerClient),
            client2(new SurfaceComposerClient);
    ASSERT_EQ(NO_ERROR, client1->initCheck()) << "failed to create SurfaceComposerClient";
    ASSERT_EQ(NO_ERROR, client2->initCheck()) << "failed to create SurfaceComposerClient";

    sp<SurfaceControl> layer1, layer2;
    ASSERT_NO_FATAL_FAILURE(layer1 = createLayer(client1, "test", 0, 0,
                                                 ISurfaceComposerClient::eFXSurfaceBufferState));
    ASSERT_NO_FATAL_FAILURE(layer2 = createLayer(client2, "test", 0, 0,
                                                 ISurfaceComposerClient::eFXSurfaceBufferState));

    Transaction transaction1, transaction2;
    CallbackHelper callback1, callback2;
    for (size_t i = 0; i < 10; i++) {
        int err = fillTransaction(transaction1, &callback1, layer1);
        if (err) {
            GTEST_SUCCEED() << "test not supported";
            return;
        }
        err = fillTransaction(transaction2, &callback2, layer2);
        if (err) {
            GTEST_SUCCEED() << "test not supported";
            return;
        }

        transaction1.setFrame(layer1, Rect(0, 0, 32, 32));
        transaction2.setFrame(layer2, Rect(32, 32, 64, 64)).merge(std::move(transaction1)).apply();

        ExpectedResult expected;
        expected.addSurfaces(ExpectedResult::Transaction::PRESENTED, {layer1, layer2},
                             ExpectedResult::Buffer::ACQUIRED,
                             (i == 0) ? ExpectedResult::PreviousBuffer::NOT_RELEASED
                                      : ExpectedResult::PreviousBuffer::RELEASED);
        EXPECT_NO_FATAL_FAILURE(waitForCallback(callback1, expected));
        EXPECT_NO_FATAL_FAILURE(waitForCallback(callback2, expected));
    }
    ASSERT_NO_FATAL_FAILURE(callback1.verifyFinalState());
    ASSERT_NO_FATAL_FAILURE(callback2.verifyFinalState());
}

TEST_F(LayerCallbackTest, MultipleTransactions_Merge_DifferentClients_NoStateChange) {
    sp<SurfaceComposerClient> client1(new SurfaceComposerClient),
            client2(new SurfaceComposerClient);
    ASSERT_EQ(NO_ERROR, client1->initCheck()) << "failed to create SurfaceComposerClient";
    ASSERT_EQ(NO_ERROR, client2->initCheck()) << "failed to create SurfaceComposerClient";

    sp<SurfaceControl> layer1, layer2;
    ASSERT_NO_FATAL_FAILURE(layer1 = createLayer(client1, "test", 0, 0,
                                                 ISurfaceComposerClient::eFXSurfaceBufferState));
    ASSERT_NO_FATAL_FAILURE(layer2 = createLayer(client2, "test", 0, 0,
                                                 ISurfaceComposerClient::eFXSurfaceBufferState));

    Transaction transaction1, transaction2;
    CallbackHelper callback1, callback2;

    // Normal call to set up test
    int err = fillTransaction(transaction1, &callback1, layer1);
    if (err) {
        GTEST_SUCCEED() << "test not supported";
        return;
    }
    err = fillTransaction(transaction2, &callback2, layer2);
    if (err) {
        GTEST_SUCCEED() << "test not supported";
        return;
    }

    transaction1.setFrame(layer1, Rect(0, 0, 32, 32));
    transaction2.setFrame(layer2, Rect(32, 32, 64, 64)).merge(std::move(transaction1)).apply();

    ExpectedResult expected;
    expected.addSurfaces(ExpectedResult::Transaction::PRESENTED, {layer1, layer2});
    EXPECT_NO_FATAL_FAILURE(waitForCallback(callback1, expected, true));
    EXPECT_NO_FATAL_FAILURE(waitForCallback(callback2, expected, true));
    expected.reset();

    // Test
    err = fillTransaction(transaction1, &callback1);
    if (err) {
        GTEST_SUCCEED() << "test not supported";
        return;
    }
    err = fillTransaction(transaction2, &callback2);
    if (err) {
        GTEST_SUCCEED() << "test not supported";
        return;
    }

    transaction2.merge(std::move(transaction1)).apply();

    EXPECT_NO_FATAL_FAILURE(waitForCallback(callback1, expected, true));
    EXPECT_NO_FATAL_FAILURE(waitForCallback(callback2, expected, true));
}

TEST_F(LayerCallbackTest, MultipleTransactions_Merge_DifferentClients_SameStateChange) {
    sp<SurfaceComposerClient> client1(new SurfaceComposerClient),
            client2(new SurfaceComposerClient);

    ASSERT_EQ(NO_ERROR, client1->initCheck()) << "failed to create SurfaceComposerClient";
    ASSERT_EQ(NO_ERROR, client2->initCheck()) << "failed to create SurfaceComposerClient";

    sp<SurfaceControl> layer1, layer2;
    ASSERT_NO_FATAL_FAILURE(layer1 = createLayer(client1, "test", 0, 0,
                                                 ISurfaceComposerClient::eFXSurfaceBufferState));
    ASSERT_NO_FATAL_FAILURE(layer2 = createLayer(client2, "test", 0, 0,
                                                 ISurfaceComposerClient::eFXSurfaceBufferState));

    Transaction transaction1, transaction2;
    CallbackHelper callback1, callback2;

    // Normal call to set up test
    int err = fillTransaction(transaction1, &callback1, layer1);
    if (err) {
        GTEST_SUCCEED() << "test not supported";
        return;
    }
    err = fillTransaction(transaction2, &callback2, layer2);
    if (err) {
        GTEST_SUCCEED() << "test not supported";
        return;
    }

    transaction1.setFrame(layer1, Rect(0, 0, 32, 32));
    transaction2.setFrame(layer2, Rect(32, 32, 64, 64)).merge(std::move(transaction1)).apply();

    ExpectedResult expected;
    expected.addSurfaces(ExpectedResult::Transaction::PRESENTED, {layer1, layer2});
    EXPECT_NO_FATAL_FAILURE(waitForCallback(callback1, expected, true));
    EXPECT_NO_FATAL_FAILURE(waitForCallback(callback2, expected, true));
    expected.reset();

    // Test
    err = fillTransaction(transaction1, &callback1);
    if (err) {
        GTEST_SUCCEED() << "test not supported";
        return;
    }
    err = fillTransaction(transaction2, &callback2);
    if (err) {
        GTEST_SUCCEED() << "test not supported";
        return;
    }

    transaction2.setFrame(layer2, Rect(32, 32, 64, 64)).merge(std::move(transaction1)).apply();

    expected.addSurface(ExpectedResult::Transaction::NOT_PRESENTED, layer2,
                        ExpectedResult::Buffer::NOT_ACQUIRED);
    EXPECT_NO_FATAL_FAILURE(waitForCallback(callback1, expected, true));
    EXPECT_NO_FATAL_FAILURE(waitForCallback(callback2, expected, true));
}

TEST_F(LayerCallbackTest, MultipleTransactions_SingleFrame) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createBufferStateLayer());

    Transaction transaction;
    CallbackHelper callback;
    std::vector<ExpectedResult> expectedResults(50);
    for (auto& expected : expectedResults) {
        expected.reset();
        expected.addSurface(ExpectedResult::Transaction::PRESENTED, layer,
                            ExpectedResult::Buffer::ACQUIRED,
                            ExpectedResult::PreviousBuffer::UNKNOWN);

        int err = fillTransaction(transaction, &callback, layer);
        if (err) {
            GTEST_SUCCEED() << "test not supported";
            return;
        }

        transaction.apply();
    }
    EXPECT_NO_FATAL_FAILURE(waitForCallbacks(callback, expectedResults, true));
}

TEST_F(LayerCallbackTest, MultipleTransactions_SingleFrame_NoStateChange) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createBufferStateLayer());

    // Normal call to set up test
    Transaction transaction;
    CallbackHelper callback;
    int err = fillTransaction(transaction, &callback, layer);
    if (err) {
        GTEST_SUCCEED() << "test not supported";
        return;
    }

    transaction.apply();

    ExpectedResult expected;
    expected.addSurface(ExpectedResult::Transaction::PRESENTED, layer);
    EXPECT_NO_FATAL_FAILURE(waitForCallback(callback, expected, true));

    // Test
    std::vector<ExpectedResult> expectedResults(50);
    for (auto& expected : expectedResults) {
        expected.reset();

        err = fillTransaction(transaction, &callback);
        if (err) {
            GTEST_SUCCEED() << "test not supported";
            return;
        }

        transaction.apply();
    }
    EXPECT_NO_FATAL_FAILURE(waitForCallbacks(callback, expectedResults, true));
}

TEST_F(LayerCallbackTest, MultipleTransactions_SingleFrame_SameStateChange) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createBufferStateLayer());

    // Normal call to set up test
    Transaction transaction;
    CallbackHelper callback;
    int err = fillTransaction(transaction, &callback, layer);
    if (err) {
        GTEST_SUCCEED() << "test not supported";
        return;
    }

    transaction.setFrame(layer, Rect(0, 0, 32, 32)).apply();

    ExpectedResult expectedResult;
    expectedResult.addSurface(ExpectedResult::Transaction::PRESENTED, layer);
    EXPECT_NO_FATAL_FAILURE(waitForCallback(callback, expectedResult, true));

    // Test
    std::vector<ExpectedResult> expectedResults(50);
    for (auto& expected : expectedResults) {
        expected.reset();
        expected.addSurface(ExpectedResult::Transaction::NOT_PRESENTED, layer,
                            ExpectedResult::Buffer::NOT_ACQUIRED);

        err = fillTransaction(transaction, &callback);
        if (err) {
            GTEST_SUCCEED() << "test not supported";
            return;
        }

        transaction.setFrame(layer, Rect(0, 0, 32, 32)).apply();
    }
    EXPECT_NO_FATAL_FAILURE(waitForCallbacks(callback, expectedResults, true));
}

TEST_F(LayerCallbackTest, DesiredPresentTime) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createBufferStateLayer());

    Transaction transaction;
    CallbackHelper callback;
    int err = fillTransaction(transaction, &callback, layer);
    if (err) {
        GTEST_SUCCEED() << "test not supported";
        return;
    }

    // Try to present 100ms in the future
    nsecs_t time = systemTime() + (100 * 1e6);

    transaction.setDesiredPresentTime(time);
    transaction.apply();

    ExpectedResult expected;
    expected.addSurface(ExpectedResult::Transaction::PRESENTED, layer);
    expected.addExpectedPresentTime(time);
    EXPECT_NO_FATAL_FAILURE(waitForCallback(callback, expected, true));
}

TEST_F(LayerCallbackTest, DesiredPresentTime_Multiple) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createBufferStateLayer());

    Transaction transaction;
    CallbackHelper callback1;
    int err = fillTransaction(transaction, &callback1, layer);
    if (err) {
        GTEST_SUCCEED() << "test not supported";
        return;
    }

    // Try to present 100ms in the future
    nsecs_t time = systemTime() + (100 * 1e6);

    transaction.setDesiredPresentTime(time);
    transaction.apply();

    ExpectedResult expected1;
    expected1.addSurface(ExpectedResult::Transaction::PRESENTED, layer);
    expected1.addExpectedPresentTime(time);

    CallbackHelper callback2;
    err = fillTransaction(transaction, &callback2, layer);
    if (err) {
        GTEST_SUCCEED() << "test not supported";
        return;
    }

    // Try to present 33ms after the first frame
    time += (33.3 * 1e6);

    transaction.setDesiredPresentTime(time);
    transaction.apply();

    ExpectedResult expected2;
    expected2.addSurface(ExpectedResult::Transaction::PRESENTED, layer,
                         ExpectedResult::Buffer::ACQUIRED,
                         ExpectedResult::PreviousBuffer::RELEASED);
    expected2.addExpectedPresentTime(time);

    EXPECT_NO_FATAL_FAILURE(waitForCallback(callback1, expected1, true));
    EXPECT_NO_FATAL_FAILURE(waitForCallback(callback2, expected2, true));
}

TEST_F(LayerCallbackTest, DesiredPresentTime_OutOfOrder) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createBufferStateLayer());

    Transaction transaction;
    CallbackHelper callback1;
    int err = fillTransaction(transaction, &callback1, layer);
    if (err) {
        GTEST_SUCCEED() << "test not supported";
        return;
    }

    // Try to present 100ms in the future
    nsecs_t time = systemTime() + (100 * 1e6);

    transaction.setDesiredPresentTime(time);
    transaction.apply();

    ExpectedResult expected1;
    expected1.addSurface(ExpectedResult::Transaction::PRESENTED, layer);
    expected1.addExpectedPresentTime(time);

    CallbackHelper callback2;
    err = fillTransaction(transaction, &callback2, layer);
    if (err) {
        GTEST_SUCCEED() << "test not supported";
        return;
    }

    // Try to present 33ms before the previous frame
    time -= (33.3 * 1e6);

    transaction.setDesiredPresentTime(time);
    transaction.apply();

    ExpectedResult expected2;
    expected2.addSurface(ExpectedResult::Transaction::PRESENTED, layer,
                         ExpectedResult::Buffer::ACQUIRED,
                         ExpectedResult::PreviousBuffer::RELEASED);

    EXPECT_NO_FATAL_FAILURE(waitForCallback(callback1, expected1, true));
    EXPECT_NO_FATAL_FAILURE(waitForCallback(callback2, expected2, true));
}

TEST_F(LayerCallbackTest, DesiredPresentTime_Past) {
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createBufferStateLayer());

    Transaction transaction;
    CallbackHelper callback;
    int err = fillTransaction(transaction, &callback, layer);
    if (err) {
        GTEST_SUCCEED() << "test not supported";
        return;
    }

    // Try to present 100ms in the past
    nsecs_t time = systemTime() - (100 * 1e6);

    transaction.setDesiredPresentTime(time);
    transaction.apply();

    ExpectedResult expected;
    expected.addSurface(ExpectedResult::Transaction::PRESENTED, layer);
    expected.addExpectedPresentTime(systemTime());
    EXPECT_NO_FATAL_FAILURE(waitForCallback(callback, expected, true));
}

class LayerUpdateTest : public LayerTransactionTest {
protected:
    virtual void SetUp() {
        LayerTransactionTest::SetUp();
        ASSERT_EQ(NO_ERROR, mClient->initCheck());

        const auto display = SurfaceComposerClient::getInternalDisplayToken();
        ASSERT_FALSE(display == nullptr);

        DisplayInfo info;
        ASSERT_EQ(NO_ERROR, SurfaceComposerClient::getDisplayInfo(display, &info));

        ssize_t displayWidth = info.w;
        ssize_t displayHeight = info.h;

        // Background surface
        mBGSurfaceControl = createLayer(String8("BG Test Surface"), displayWidth,
                                               displayHeight, 0);
        ASSERT_TRUE(mBGSurfaceControl != nullptr);
        ASSERT_TRUE(mBGSurfaceControl->isValid());
        fillSurfaceRGBA8(mBGSurfaceControl, 63, 63, 195);

        // Foreground surface
        mFGSurfaceControl = createLayer(String8("FG Test Surface"), 64, 64, 0);

        ASSERT_TRUE(mFGSurfaceControl != nullptr);
        ASSERT_TRUE(mFGSurfaceControl->isValid());

        fillSurfaceRGBA8(mFGSurfaceControl, 195, 63, 63);

        // Synchronization surface
        mSyncSurfaceControl = createLayer(String8("Sync Test Surface"), 1, 1, 0);
        ASSERT_TRUE(mSyncSurfaceControl != nullptr);
        ASSERT_TRUE(mSyncSurfaceControl->isValid());

        fillSurfaceRGBA8(mSyncSurfaceControl, 31, 31, 31);

        asTransaction([&](Transaction& t) {
            t.setDisplayLayerStack(display, 0);

            t.setLayer(mBGSurfaceControl, INT32_MAX - 2).show(mBGSurfaceControl);

            t.setLayer(mFGSurfaceControl, INT32_MAX - 1)
                    .setPosition(mFGSurfaceControl, 64, 64)
                    .show(mFGSurfaceControl);

            t.setLayer(mSyncSurfaceControl, INT32_MAX - 1)
                    .setPosition(mSyncSurfaceControl, displayWidth - 2, displayHeight - 2)
                    .show(mSyncSurfaceControl);
        });
    }

    virtual void TearDown() {
        LayerTransactionTest::TearDown();
        mBGSurfaceControl = 0;
        mFGSurfaceControl = 0;
        mSyncSurfaceControl = 0;
    }

    void waitForPostedBuffers() {
        // Since the sync surface is in synchronous mode (i.e. double buffered)
        // posting three buffers to it should ensure that at least two
        // SurfaceFlinger::handlePageFlip calls have been made, which should
        // guaranteed that a buffer posted to another Surface has been retired.
        fillSurfaceRGBA8(mSyncSurfaceControl, 31, 31, 31);
        fillSurfaceRGBA8(mSyncSurfaceControl, 31, 31, 31);
        fillSurfaceRGBA8(mSyncSurfaceControl, 31, 31, 31);
    }


    sp<SurfaceControl> mBGSurfaceControl;
    sp<SurfaceControl> mFGSurfaceControl;

    // This surface is used to ensure that the buffers posted to
    // mFGSurfaceControl have been picked up by SurfaceFlinger.
    sp<SurfaceControl> mSyncSurfaceControl;
};

TEST_F(LayerUpdateTest, RelativesAreNotDetached) {

    std::unique_ptr<ScreenCapture> sc;

    sp<SurfaceControl> relative = createLayer(String8("relativeTestSurface"), 10, 10, 0);
    fillSurfaceRGBA8(relative, 10, 10, 10);
    waitForPostedBuffers();

    Transaction{}
            .setRelativeLayer(relative, mFGSurfaceControl->getHandle(), 1)
            .setPosition(relative, 64, 64)
            .apply();

    {
        // The relative should be on top of the FG control.
        ScreenCapture::captureScreen(&sc);
        sc->checkPixel(64, 64, 10, 10, 10);
    }
    Transaction{}.detachChildren(mFGSurfaceControl).apply();

    {
        // Nothing should change at this point.
        ScreenCapture::captureScreen(&sc);
        sc->checkPixel(64, 64, 10, 10, 10);
    }

    Transaction{}.hide(relative).apply();

    {
        // Ensure that the relative was actually hidden, rather than
        // being left in the detached but visible state.
        ScreenCapture::captureScreen(&sc);
        sc->expectFGColor(64, 64);
    }
}

class GeometryLatchingTest : public LayerUpdateTest {
protected:
    void EXPECT_INITIAL_STATE(const char* trace) {
        SCOPED_TRACE(trace);
        ScreenCapture::captureScreen(&sc);
        // We find the leading edge of the FG surface.
        sc->expectFGColor(127, 127);
        sc->expectBGColor(128, 128);
    }

    void lockAndFillFGBuffer() { fillSurfaceRGBA8(mFGSurfaceControl, 195, 63, 63, false); }

    void unlockFGBuffer() {
        sp<Surface> s = mFGSurfaceControl->getSurface();
        ASSERT_EQ(NO_ERROR, s->unlockAndPost());
        waitForPostedBuffers();
    }

    void completeFGResize() {
        fillSurfaceRGBA8(mFGSurfaceControl, 195, 63, 63);
        waitForPostedBuffers();
    }
    void restoreInitialState() {
        asTransaction([&](Transaction& t) {
            t.setSize(mFGSurfaceControl, 64, 64);
            t.setPosition(mFGSurfaceControl, 64, 64);
            t.setCrop_legacy(mFGSurfaceControl, Rect(0, 0, 64, 64));
        });

        EXPECT_INITIAL_STATE("After restoring initial state");
    }
    std::unique_ptr<ScreenCapture> sc;
};

class CropLatchingTest : public GeometryLatchingTest {
protected:
    void EXPECT_CROPPED_STATE(const char* trace) {
        SCOPED_TRACE(trace);
        ScreenCapture::captureScreen(&sc);
        // The edge should be moved back one pixel by our crop.
        sc->expectFGColor(126, 126);
        sc->expectBGColor(127, 127);
        sc->expectBGColor(128, 128);
    }

    void EXPECT_RESIZE_STATE(const char* trace) {
        SCOPED_TRACE(trace);
        ScreenCapture::captureScreen(&sc);
        // The FG is now resized too 128,128 at 64,64
        sc->expectFGColor(64, 64);
        sc->expectFGColor(191, 191);
        sc->expectBGColor(192, 192);
    }
};

TEST_F(LayerUpdateTest, DeferredTransactionTest) {
    std::unique_ptr<ScreenCapture> sc;
    {
        SCOPED_TRACE("before anything");
        ScreenCapture::captureScreen(&sc);
        sc->expectBGColor(32, 32);
        sc->expectFGColor(96, 96);
        sc->expectBGColor(160, 160);
    }

    // set up two deferred transactions on different frames
    asTransaction([&](Transaction& t) {
        t.setAlpha(mFGSurfaceControl, 0.75);
        t.deferTransactionUntil_legacy(mFGSurfaceControl, mSyncSurfaceControl->getHandle(),
                                       mSyncSurfaceControl->getSurface()->getNextFrameNumber());
    });

    asTransaction([&](Transaction& t) {
        t.setPosition(mFGSurfaceControl, 128, 128);
        t.deferTransactionUntil_legacy(mFGSurfaceControl, mSyncSurfaceControl->getHandle(),
                                       mSyncSurfaceControl->getSurface()->getNextFrameNumber() + 1);
    });

    {
        SCOPED_TRACE("before any trigger");
        ScreenCapture::captureScreen(&sc);
        sc->expectBGColor(32, 32);
        sc->expectFGColor(96, 96);
        sc->expectBGColor(160, 160);
    }

    // should trigger the first deferred transaction, but not the second one
    fillSurfaceRGBA8(mSyncSurfaceControl, 31, 31, 31);
    {
        SCOPED_TRACE("after first trigger");
        ScreenCapture::captureScreen(&sc);
        sc->expectBGColor(32, 32);
        sc->checkPixel(96, 96, 162, 63, 96);
        sc->expectBGColor(160, 160);
    }

    // should show up immediately since it's not deferred
    asTransaction([&](Transaction& t) { t.setAlpha(mFGSurfaceControl, 1.0); });

    // trigger the second deferred transaction
    fillSurfaceRGBA8(mSyncSurfaceControl, 31, 31, 31);
    {
        SCOPED_TRACE("after second trigger");
        ScreenCapture::captureScreen(&sc);
        sc->expectBGColor(32, 32);
        sc->expectBGColor(96, 96);
        sc->expectFGColor(160, 160);
    }
}

TEST_F(LayerUpdateTest, LayerWithNoBuffersResizesImmediately) {
    std::unique_ptr<ScreenCapture> sc;

    sp<SurfaceControl> childNoBuffer =
            createSurface(mClient, "Bufferless child", 0 /* buffer width */, 0 /* buffer height */,
                          PIXEL_FORMAT_RGBA_8888, 0, mFGSurfaceControl.get());
    sp<SurfaceControl> childBuffer = createSurface(mClient, "Buffered child", 20, 20,
                                                   PIXEL_FORMAT_RGBA_8888, 0, childNoBuffer.get());
    fillSurfaceRGBA8(childBuffer, 200, 200, 200);
    SurfaceComposerClient::Transaction{}
            .setCrop_legacy(childNoBuffer, Rect(0, 0, 10, 10))
            .show(childNoBuffer)
            .show(childBuffer)
            .apply(true);
    {
        ScreenCapture::captureScreen(&sc);
        sc->expectChildColor(73, 73);
        sc->expectFGColor(74, 74);
    }
    SurfaceComposerClient::Transaction{}
            .setCrop_legacy(childNoBuffer, Rect(0, 0, 20, 20))
            .apply(true);
    {
        ScreenCapture::captureScreen(&sc);
        sc->expectChildColor(73, 73);
        sc->expectChildColor(74, 74);
    }
}

TEST_F(LayerUpdateTest, MergingTransactions) {
    std::unique_ptr<ScreenCapture> sc;
    {
        SCOPED_TRACE("before move");
        ScreenCapture::captureScreen(&sc);
        sc->expectBGColor(0, 12);
        sc->expectFGColor(75, 75);
        sc->expectBGColor(145, 145);
    }

    Transaction t1, t2;
    t1.setPosition(mFGSurfaceControl, 128, 128);
    t2.setPosition(mFGSurfaceControl, 0, 0);
    // We expect that the position update from t2 now
    // overwrites the position update from t1.
    t1.merge(std::move(t2));
    t1.apply();

    {
        ScreenCapture::captureScreen(&sc);
        sc->expectFGColor(1, 1);
    }
}

class ChildLayerTest : public LayerUpdateTest {
protected:
    void SetUp() override {
        LayerUpdateTest::SetUp();
        mChild = createSurface(mClient, "Child surface", 10, 15, PIXEL_FORMAT_RGBA_8888, 0,
                               mFGSurfaceControl.get());
        fillSurfaceRGBA8(mChild, 200, 200, 200);

        {
            SCOPED_TRACE("before anything");
            mCapture = screenshot();
            mCapture->expectChildColor(64, 64);
        }
    }
    void TearDown() override {
        LayerUpdateTest::TearDown();
        mChild = 0;
    }

    sp<SurfaceControl> mChild;
    std::unique_ptr<ScreenCapture> mCapture;
};

TEST_F(ChildLayerTest, ChildLayerPositioning) {
    asTransaction([&](Transaction& t) {
        t.show(mChild);
        t.setPosition(mChild, 10, 10);
        t.setPosition(mFGSurfaceControl, 64, 64);
    });

    {
        mCapture = screenshot();
        // Top left of foreground must now be visible
        mCapture->expectFGColor(64, 64);
        // But 10 pixels in we should see the child surface
        mCapture->expectChildColor(74, 74);
        // And 10 more pixels we should be back to the foreground surface
        mCapture->expectFGColor(84, 84);
    }

    asTransaction([&](Transaction& t) { t.setPosition(mFGSurfaceControl, 0, 0); });

    {
        mCapture = screenshot();
        // Top left of foreground should now be at 0, 0
        mCapture->expectFGColor(0, 0);
        // But 10 pixels in we should see the child surface
        mCapture->expectChildColor(10, 10);
        // And 10 more pixels we should be back to the foreground surface
        mCapture->expectFGColor(20, 20);
    }
}

TEST_F(ChildLayerTest, ChildLayerCropping) {
    asTransaction([&](Transaction& t) {
        t.show(mChild);
        t.setPosition(mChild, 0, 0);
        t.setPosition(mFGSurfaceControl, 0, 0);
        t.setCrop_legacy(mFGSurfaceControl, Rect(0, 0, 5, 5));
    });

    {
        mCapture = screenshot();
        mCapture->expectChildColor(0, 0);
        mCapture->expectChildColor(4, 4);
        mCapture->expectBGColor(5, 5);
    }
}

TEST_F(ChildLayerTest, ChildLayerConstraints) {
    asTransaction([&](Transaction& t) {
        t.show(mChild);
        t.setPosition(mFGSurfaceControl, 0, 0);
        t.setPosition(mChild, 63, 63);
    });

    {
        mCapture = screenshot();
        mCapture->expectFGColor(0, 0);
        // Last pixel in foreground should now be the child.
        mCapture->expectChildColor(63, 63);
        // But the child should be constrained and the next pixel
        // must be the background
        mCapture->expectBGColor(64, 64);
    }
}

TEST_F(ChildLayerTest, ChildLayerScaling) {
    asTransaction([&](Transaction& t) { t.setPosition(mFGSurfaceControl, 0, 0); });

    // Find the boundary between the parent and child
    {
        mCapture = screenshot();
        mCapture->expectChildColor(9, 9);
        mCapture->expectFGColor(10, 10);
    }

    asTransaction([&](Transaction& t) { t.setMatrix(mFGSurfaceControl, 2.0, 0, 0, 2.0); });

    // The boundary should be twice as far from the origin now.
    // The pixels from the last test should all be child now
    {
        mCapture = screenshot();
        mCapture->expectChildColor(9, 9);
        mCapture->expectChildColor(10, 10);
        mCapture->expectChildColor(19, 19);
        mCapture->expectFGColor(20, 20);
    }
}

// A child with a scale transform should be cropped by its parent bounds.
TEST_F(ChildLayerTest, ChildLayerScalingCroppedByParent) {
    asTransaction([&](Transaction& t) {
        t.setPosition(mFGSurfaceControl, 0, 0);
        t.setPosition(mChild, 0, 0);
    });

    // Find the boundary between the parent and child.
    {
        mCapture = screenshot();
        mCapture->expectChildColor(0, 0);
        mCapture->expectChildColor(9, 9);
        mCapture->expectFGColor(10, 10);
    }

    asTransaction([&](Transaction& t) { t.setMatrix(mChild, 10.0, 0, 0, 10.0); });

    // The child should fill its parent bounds and be cropped by it.
    {
        mCapture = screenshot();
        mCapture->expectChildColor(0, 0);
        mCapture->expectChildColor(63, 63);
        mCapture->expectBGColor(64, 64);
    }
}

TEST_F(ChildLayerTest, ChildLayerAlpha) {
    fillSurfaceRGBA8(mBGSurfaceControl, 0, 0, 254);
    fillSurfaceRGBA8(mFGSurfaceControl, 254, 0, 0);
    fillSurfaceRGBA8(mChild, 0, 254, 0);
    waitForPostedBuffers();

    asTransaction([&](Transaction& t) {
        t.show(mChild);
        t.setPosition(mChild, 0, 0);
        t.setPosition(mFGSurfaceControl, 0, 0);
    });

    {
        mCapture = screenshot();
        // Unblended child color
        mCapture->checkPixel(0, 0, 0, 254, 0);
    }

    asTransaction([&](Transaction& t) { t.setAlpha(mChild, 0.5); });

    {
        mCapture = screenshot();
        // Child and BG blended.
        mCapture->checkPixel(0, 0, 127, 127, 0);
    }

    asTransaction([&](Transaction& t) { t.setAlpha(mFGSurfaceControl, 0.5); });

    {
        mCapture = screenshot();
        // Child and BG blended.
        mCapture->checkPixel(0, 0, 95, 64, 95);
    }
}

TEST_F(ChildLayerTest, ReparentChildren) {
    asTransaction([&](Transaction& t) {
        t.show(mChild);
        t.setPosition(mChild, 10, 10);
        t.setPosition(mFGSurfaceControl, 64, 64);
    });

    {
        mCapture = screenshot();
        // Top left of foreground must now be visible
        mCapture->expectFGColor(64, 64);
        // But 10 pixels in we should see the child surface
        mCapture->expectChildColor(74, 74);
        // And 10 more pixels we should be back to the foreground surface
        mCapture->expectFGColor(84, 84);
    }

    asTransaction([&](Transaction& t) {
        t.reparentChildren(mFGSurfaceControl, mBGSurfaceControl->getHandle());
    });

    {
        mCapture = screenshot();
        mCapture->expectFGColor(64, 64);
        // In reparenting we should have exposed the entire foreground surface.
        mCapture->expectFGColor(74, 74);
        // And the child layer should now begin at 10, 10 (since the BG
        // layer is at (0, 0)).
        mCapture->expectBGColor(9, 9);
        mCapture->expectChildColor(10, 10);
    }
}

TEST_F(ChildLayerTest, ChildrenSurviveParentDestruction) {
    sp<SurfaceControl> mGrandChild =
            createSurface(mClient, "Grand Child", 10, 10, PIXEL_FORMAT_RGBA_8888, 0, mChild.get());
    fillSurfaceRGBA8(mGrandChild, 111, 111, 111);

    {
        SCOPED_TRACE("Grandchild visible");
        ScreenCapture::captureScreen(&mCapture);
        mCapture->checkPixel(64, 64, 111, 111, 111);
    }

    mChild.clear();

    {
        SCOPED_TRACE("After destroying child");
        ScreenCapture::captureScreen(&mCapture);
        mCapture->expectFGColor(64, 64);
    }

    asTransaction([&](Transaction& t) {
         t.reparent(mGrandChild, mFGSurfaceControl->getHandle());
    });

    {
        SCOPED_TRACE("After reparenting grandchild");
        ScreenCapture::captureScreen(&mCapture);
        mCapture->checkPixel(64, 64, 111, 111, 111);
    }
}

TEST_F(ChildLayerTest, DetachChildrenSameClient) {
    asTransaction([&](Transaction& t) {
        t.show(mChild);
        t.setPosition(mChild, 10, 10);
        t.setPosition(mFGSurfaceControl, 64, 64);
    });

    {
        mCapture = screenshot();
        // Top left of foreground must now be visible
        mCapture->expectFGColor(64, 64);
        // But 10 pixels in we should see the child surface
        mCapture->expectChildColor(74, 74);
        // And 10 more pixels we should be back to the foreground surface
        mCapture->expectFGColor(84, 84);
    }


    asTransaction([&](Transaction& t) { t.detachChildren(mFGSurfaceControl); });

    asTransaction([&](Transaction& t) { t.hide(mChild); });

    // Since the child has the same client as the parent, it will not get
    // detached and will be hidden.
    {
        mCapture = screenshot();
        mCapture->expectFGColor(64, 64);
        mCapture->expectFGColor(74, 74);
        mCapture->expectFGColor(84, 84);
    }
}

TEST_F(ChildLayerTest, DetachChildrenDifferentClient) {
    sp<SurfaceComposerClient> mNewComposerClient = new SurfaceComposerClient;
    sp<SurfaceControl> mChildNewClient =
            createSurface(mNewComposerClient, "New Child Test Surface", 10, 10,
                          PIXEL_FORMAT_RGBA_8888, 0, mFGSurfaceControl.get());

    ASSERT_TRUE(mChildNewClient->isValid());

    fillSurfaceRGBA8(mChildNewClient, 200, 200, 200);

    asTransaction([&](Transaction& t) {
        t.hide(mChild);
        t.show(mChildNewClient);
        t.setPosition(mChildNewClient, 10, 10);
        t.setPosition(mFGSurfaceControl, 64, 64);
    });

    {
        mCapture = screenshot();
        // Top left of foreground must now be visible
        mCapture->expectFGColor(64, 64);
        // But 10 pixels in we should see the child surface
        mCapture->expectChildColor(74, 74);
        // And 10 more pixels we should be back to the foreground surface
        mCapture->expectFGColor(84, 84);
    }

    asTransaction([&](Transaction& t) { t.detachChildren(mFGSurfaceControl); });

    asTransaction([&](Transaction& t) { t.hide(mChildNewClient); });

    // Nothing should have changed.
    {
        mCapture = screenshot();
        mCapture->expectFGColor(64, 64);
        mCapture->expectChildColor(74, 74);
        mCapture->expectFGColor(84, 84);
    }
}

TEST_F(ChildLayerTest, DetachChildrenThenAttach) {
    sp<SurfaceComposerClient> newComposerClient = new SurfaceComposerClient;
    sp<SurfaceControl> childNewClient =
            newComposerClient->createSurface(String8("New Child Test Surface"), 10, 10,
                                             PIXEL_FORMAT_RGBA_8888, 0, mFGSurfaceControl.get());

    ASSERT_TRUE(childNewClient != nullptr);
    ASSERT_TRUE(childNewClient->isValid());

    fillSurfaceRGBA8(childNewClient, 200, 200, 200);

    Transaction()
            .hide(mChild)
            .show(childNewClient)
            .setPosition(childNewClient, 10, 10)
            .setPosition(mFGSurfaceControl, 64, 64)
            .apply();

    {
        mCapture = screenshot();
        // Top left of foreground must now be visible
        mCapture->expectFGColor(64, 64);
        // But 10 pixels in we should see the child surface
        mCapture->expectChildColor(74, 74);
        // And 10 more pixels we should be back to the foreground surface
        mCapture->expectFGColor(84, 84);
    }

    Transaction().detachChildren(mFGSurfaceControl).apply();
    Transaction().hide(childNewClient).apply();

    // Nothing should have changed.
    {
        mCapture = screenshot();
        mCapture->expectFGColor(64, 64);
        mCapture->expectChildColor(74, 74);
        mCapture->expectFGColor(84, 84);
    }

    sp<SurfaceControl> newParentSurface = createLayer(String8("New Parent Surface"), 32, 32, 0);
    fillLayerColor(ISurfaceComposerClient::eFXSurfaceBufferQueue, newParentSurface, Color::RED, 32,
                   32);
    Transaction()
            .setLayer(newParentSurface, INT32_MAX - 1)
            .show(newParentSurface)
            .setPosition(newParentSurface, 20, 20)
            .reparent(childNewClient, newParentSurface->getHandle())
            .apply();
    {
        mCapture = screenshot();
        // Child is now hidden.
        mCapture->expectColor(Rect(20, 20, 52, 52), Color::RED);
    }
}
TEST_F(ChildLayerTest, DetachChildrenWithDeferredTransaction) {
    sp<SurfaceComposerClient> newComposerClient = new SurfaceComposerClient;
    sp<SurfaceControl> childNewClient =
            newComposerClient->createSurface(String8("New Child Test Surface"), 10, 10,
                                             PIXEL_FORMAT_RGBA_8888, 0, mFGSurfaceControl.get());

    ASSERT_TRUE(childNewClient != nullptr);
    ASSERT_TRUE(childNewClient->isValid());

    fillSurfaceRGBA8(childNewClient, 200, 200, 200);

    Transaction()
            .hide(mChild)
            .show(childNewClient)
            .setPosition(childNewClient, 10, 10)
            .setPosition(mFGSurfaceControl, 64, 64)
            .apply();

    {
        mCapture = screenshot();
        Rect rect = Rect(74, 74, 84, 84);
        mCapture->expectBorder(rect, Color{195, 63, 63, 255});
        mCapture->expectColor(rect, Color{200, 200, 200, 255});
    }

    Transaction()
            .deferTransactionUntil_legacy(childNewClient, mFGSurfaceControl->getHandle(),
                                          mFGSurfaceControl->getSurface()->getNextFrameNumber())
            .apply();
    Transaction().detachChildren(mFGSurfaceControl).apply();
    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(mFGSurfaceControl, Color::RED, 32, 32));

    // BufferLayer can still dequeue buffers even though there's a detached layer with a
    // deferred transaction.
    {
        SCOPED_TRACE("new buffer");
        mCapture = screenshot();
        Rect rect = Rect(74, 74, 84, 84);
        mCapture->expectBorder(rect, Color::RED);
        mCapture->expectColor(rect, Color{200, 200, 200, 255});
    }
}

TEST_F(ChildLayerTest, ChildrenInheritNonTransformScalingFromParent) {
    asTransaction([&](Transaction& t) {
        t.show(mChild);
        t.setPosition(mChild, 0, 0);
        t.setPosition(mFGSurfaceControl, 0, 0);
    });

    {
        mCapture = screenshot();
        // We've positioned the child in the top left.
        mCapture->expectChildColor(0, 0);
        // But it's only 10x15.
        mCapture->expectFGColor(10, 15);
    }

    asTransaction([&](Transaction& t) {
        t.setOverrideScalingMode(mFGSurfaceControl, NATIVE_WINDOW_SCALING_MODE_SCALE_TO_WINDOW);
        // We cause scaling by 2.
        t.setSize(mFGSurfaceControl, 128, 128);
    });

    {
        mCapture = screenshot();
        // We've positioned the child in the top left.
        mCapture->expectChildColor(0, 0);
        mCapture->expectChildColor(10, 10);
        mCapture->expectChildColor(19, 29);
        // And now it should be scaled all the way to 20x30
        mCapture->expectFGColor(20, 30);
    }
}

// Regression test for b/37673612
TEST_F(ChildLayerTest, ChildrenWithParentBufferTransform) {
    asTransaction([&](Transaction& t) {
        t.show(mChild);
        t.setPosition(mChild, 0, 0);
        t.setPosition(mFGSurfaceControl, 0, 0);
    });

    {
        mCapture = screenshot();
        // We've positioned the child in the top left.
        mCapture->expectChildColor(0, 0);
        mCapture->expectChildColor(9, 14);
        // But it's only 10x15.
        mCapture->expectFGColor(10, 15);
    }
    // We set things up as in b/37673612 so that there is a mismatch between the buffer size and
    // the WM specified state size.
    asTransaction([&](Transaction& t) { t.setSize(mFGSurfaceControl, 128, 64); });
    sp<Surface> s = mFGSurfaceControl->getSurface();
    auto anw = static_cast<ANativeWindow*>(s.get());
    native_window_set_buffers_transform(anw, NATIVE_WINDOW_TRANSFORM_ROT_90);
    native_window_set_buffers_dimensions(anw, 64, 128);
    fillSurfaceRGBA8(mFGSurfaceControl, 195, 63, 63);
    waitForPostedBuffers();

    {
        // The child should still be in the same place and not have any strange scaling as in
        // b/37673612.
        mCapture = screenshot();
        mCapture->expectChildColor(0, 0);
        mCapture->expectFGColor(10, 10);
    }
}

// A child with a buffer transform from its parents should be cropped by its parent bounds.
TEST_F(ChildLayerTest, ChildCroppedByParentWithBufferTransform) {
    asTransaction([&](Transaction& t) {
        t.show(mChild);
        t.setPosition(mChild, 0, 0);
        t.setPosition(mFGSurfaceControl, 0, 0);
        t.setSize(mChild, 100, 100);
    });
    fillSurfaceRGBA8(mChild, 200, 200, 200);

    {
        mCapture = screenshot();

        mCapture->expectChildColor(0, 0);
        mCapture->expectChildColor(63, 63);
        mCapture->expectBGColor(64, 64);
    }

    asTransaction([&](Transaction& t) { t.setSize(mFGSurfaceControl, 128, 64); });
    sp<Surface> s = mFGSurfaceControl->getSurface();
    auto anw = static_cast<ANativeWindow*>(s.get());
    // Apply a 90 transform on the buffer.
    native_window_set_buffers_transform(anw, NATIVE_WINDOW_TRANSFORM_ROT_90);
    native_window_set_buffers_dimensions(anw, 64, 128);
    fillSurfaceRGBA8(mFGSurfaceControl, 195, 63, 63);
    waitForPostedBuffers();

    // The child should be cropped by the new parent bounds.
    {
        mCapture = screenshot();
        mCapture->expectChildColor(0, 0);
        mCapture->expectChildColor(99, 63);
        mCapture->expectFGColor(100, 63);
        mCapture->expectBGColor(128, 64);
    }
}

// A child with a scale transform from its parents should be cropped by its parent bounds.
TEST_F(ChildLayerTest, ChildCroppedByParentWithBufferScale) {
    asTransaction([&](Transaction& t) {
        t.show(mChild);
        t.setPosition(mChild, 0, 0);
        t.setPosition(mFGSurfaceControl, 0, 0);
        t.setSize(mChild, 200, 200);
    });
    fillSurfaceRGBA8(mChild, 200, 200, 200);

    {
        mCapture = screenshot();

        mCapture->expectChildColor(0, 0);
        mCapture->expectChildColor(63, 63);
        mCapture->expectBGColor(64, 64);
    }

    asTransaction([&](Transaction& t) {
        t.setOverrideScalingMode(mFGSurfaceControl, NATIVE_WINDOW_SCALING_MODE_SCALE_TO_WINDOW);
        // Set a scaling by 2.
        t.setSize(mFGSurfaceControl, 128, 128);
    });

    // Child should inherit its parents scale but should be cropped by its parent bounds.
    {
        mCapture = screenshot();
        mCapture->expectChildColor(0, 0);
        mCapture->expectChildColor(127, 127);
        mCapture->expectBGColor(128, 128);
    }
}

// Regression test for b/127368943
// Child should ignore the buffer transform but apply parent scale transform.
TEST_F(ChildLayerTest, ChildrenWithParentBufferTransformAndScale) {
    asTransaction([&](Transaction& t) {
        t.show(mChild);
        t.setPosition(mChild, 0, 0);
        t.setPosition(mFGSurfaceControl, 0, 0);
    });

    {
        mCapture = screenshot();
        mCapture->expectChildColor(0, 0);
        mCapture->expectChildColor(9, 14);
        mCapture->expectFGColor(10, 15);
    }

    // Change the size of the foreground to 128 * 64 so we can test rotation as well.
    asTransaction([&](Transaction& t) {
        t.setOverrideScalingMode(mFGSurfaceControl, NATIVE_WINDOW_SCALING_MODE_SCALE_TO_WINDOW);
        t.setSize(mFGSurfaceControl, 128, 64);
    });
    sp<Surface> s = mFGSurfaceControl->getSurface();
    auto anw = static_cast<ANativeWindow*>(s.get());
    // Apply a 90 transform on the buffer and submit a buffer half the expected size so that we
    // have an effective scale of 2.0 applied to the buffer along with a rotation transform.
    native_window_set_buffers_transform(anw, NATIVE_WINDOW_TRANSFORM_ROT_90);
    native_window_set_buffers_dimensions(anw, 32, 64);
    fillSurfaceRGBA8(mFGSurfaceControl, 195, 63, 63);
    waitForPostedBuffers();

    // The child should ignore the buffer transform but apply the 2.0 scale from parent.
    {
        mCapture = screenshot();
        mCapture->expectChildColor(0, 0);
        mCapture->expectChildColor(19, 29);
        mCapture->expectFGColor(20, 30);
    }
}

TEST_F(ChildLayerTest, Bug36858924) {
    // Destroy the child layer
    mChild.clear();

    // Now recreate it as hidden
    mChild = createSurface(mClient, "Child surface", 10, 10, PIXEL_FORMAT_RGBA_8888,
                           ISurfaceComposerClient::eHidden, mFGSurfaceControl.get());

    // Show the child layer in a deferred transaction
    asTransaction([&](Transaction& t) {
        t.deferTransactionUntil_legacy(mChild, mFGSurfaceControl->getHandle(),
                                       mFGSurfaceControl->getSurface()->getNextFrameNumber());
        t.show(mChild);
    });

    // Render the foreground surface a few times
    //
    // Prior to the bugfix for b/36858924, this would usually hang while trying to fill the third
    // frame because SurfaceFlinger would never process the deferred transaction and would therefore
    // never acquire/release the first buffer
    ALOGI("Filling 1");
    fillSurfaceRGBA8(mFGSurfaceControl, 0, 255, 0);
    ALOGI("Filling 2");
    fillSurfaceRGBA8(mFGSurfaceControl, 0, 0, 255);
    ALOGI("Filling 3");
    fillSurfaceRGBA8(mFGSurfaceControl, 255, 0, 0);
    ALOGI("Filling 4");
    fillSurfaceRGBA8(mFGSurfaceControl, 0, 255, 0);
}

TEST_F(ChildLayerTest, Reparent) {
    asTransaction([&](Transaction& t) {
        t.show(mChild);
        t.setPosition(mChild, 10, 10);
        t.setPosition(mFGSurfaceControl, 64, 64);
    });

    {
        mCapture = screenshot();
        // Top left of foreground must now be visible
        mCapture->expectFGColor(64, 64);
        // But 10 pixels in we should see the child surface
        mCapture->expectChildColor(74, 74);
        // And 10 more pixels we should be back to the foreground surface
        mCapture->expectFGColor(84, 84);
    }

    asTransaction([&](Transaction& t) { t.reparent(mChild, mBGSurfaceControl->getHandle()); });

    {
        mCapture = screenshot();
        mCapture->expectFGColor(64, 64);
        // In reparenting we should have exposed the entire foreground surface.
        mCapture->expectFGColor(74, 74);
        // And the child layer should now begin at 10, 10 (since the BG
        // layer is at (0, 0)).
        mCapture->expectBGColor(9, 9);
        mCapture->expectChildColor(10, 10);
    }
}

TEST_F(ChildLayerTest, ReparentToNoParent) {
    asTransaction([&](Transaction& t) {
        t.show(mChild);
        t.setPosition(mChild, 10, 10);
        t.setPosition(mFGSurfaceControl, 64, 64);
    });

    {
        mCapture = screenshot();
        // Top left of foreground must now be visible
        mCapture->expectFGColor(64, 64);
        // But 10 pixels in we should see the child surface
        mCapture->expectChildColor(74, 74);
        // And 10 more pixels we should be back to the foreground surface
        mCapture->expectFGColor(84, 84);
    }
    asTransaction([&](Transaction& t) { t.reparent(mChild, nullptr); });
    {
        mCapture = screenshot();
        // The surface should now be offscreen.
        mCapture->expectFGColor(64, 64);
        mCapture->expectFGColor(74, 74);
        mCapture->expectFGColor(84, 84);
    }
}

TEST_F(ChildLayerTest, ReparentFromNoParent) {
    sp<SurfaceControl> newSurface = createLayer(String8("New Surface"), 10, 10, 0);
    ASSERT_TRUE(newSurface != nullptr);
    ASSERT_TRUE(newSurface->isValid());

    fillSurfaceRGBA8(newSurface, 63, 195, 63);
    asTransaction([&](Transaction& t) {
        t.hide(mChild);
        t.show(newSurface);
        t.setPosition(newSurface, 10, 10);
        t.setLayer(newSurface, INT32_MAX - 2);
        t.setPosition(mFGSurfaceControl, 64, 64);
    });

    {
        mCapture = screenshot();
        // Top left of foreground must now be visible
        mCapture->expectFGColor(64, 64);
        // At 10, 10 we should see the new surface
        mCapture->checkPixel(10, 10, 63, 195, 63);
    }

    asTransaction([&](Transaction& t) { t.reparent(newSurface, mFGSurfaceControl->getHandle()); });

    {
        mCapture = screenshot();
        // newSurface will now be a child of mFGSurface so it will be 10, 10 offset from
        // mFGSurface, putting it at 74, 74.
        mCapture->expectFGColor(64, 64);
        mCapture->checkPixel(74, 74, 63, 195, 63);
        mCapture->expectFGColor(84, 84);
    }
}

TEST_F(ChildLayerTest, NestedChildren) {
    sp<SurfaceControl> grandchild = createSurface(mClient, "Grandchild surface", 10, 10,
                                                  PIXEL_FORMAT_RGBA_8888, 0, mChild.get());
    fillSurfaceRGBA8(grandchild, 50, 50, 50);

    {
        mCapture = screenshot();
        // Expect the grandchild to begin at 64, 64 because it's a child of mChild layer
        // which begins at 64, 64
        mCapture->checkPixel(64, 64, 50, 50, 50);
    }
}

TEST_F(ChildLayerTest, ChildLayerRelativeLayer) {
    sp<SurfaceControl> relative = createLayer(String8("Relative surface"), 128, 128, 0);
    fillSurfaceRGBA8(relative, 255, 255, 255);

    Transaction t;
    t.setLayer(relative, INT32_MAX)
            .setRelativeLayer(mChild, relative->getHandle(), 1)
            .setPosition(mFGSurfaceControl, 0, 0)
            .apply(true);

    // We expect that the child should have been elevated above our
    // INT_MAX layer even though it's not a child of it.
    {
        mCapture = screenshot();
        mCapture->expectChildColor(0, 0);
        mCapture->expectChildColor(9, 9);
        mCapture->checkPixel(10, 10, 255, 255, 255);
    }
}

class BoundlessLayerTest : public LayerUpdateTest {
protected:
    std::unique_ptr<ScreenCapture> mCapture;
};

// Verify setting a size on a buffer layer has no effect.
TEST_F(BoundlessLayerTest, BufferLayerIgnoresSize) {
    sp<SurfaceControl> bufferLayer =
            createSurface(mClient, "BufferLayer", 45, 45, PIXEL_FORMAT_RGBA_8888, 0,
                          mFGSurfaceControl.get());
    ASSERT_TRUE(bufferLayer->isValid());
    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(bufferLayer, Color::BLACK, 30, 30));
    asTransaction([&](Transaction& t) { t.show(bufferLayer); });
    {
        mCapture = screenshot();
        // Top left of background must now be visible
        mCapture->expectBGColor(0, 0);
        // Foreground Surface bounds must be color layer
        mCapture->expectColor(Rect(64, 64, 94, 94), Color::BLACK);
        // Buffer layer should not extend past buffer bounds
        mCapture->expectFGColor(95, 95);
    }
}

// Verify a boundless color layer will fill its parent bounds. The parent has a buffer size
// which will crop the color layer.
TEST_F(BoundlessLayerTest, BoundlessColorLayerFillsParentBufferBounds) {
    sp<SurfaceControl> colorLayer =
            createSurface(mClient, "ColorLayer", 0, 0, PIXEL_FORMAT_RGBA_8888,
                          ISurfaceComposerClient::eFXSurfaceColor, mFGSurfaceControl.get());
    ASSERT_TRUE(colorLayer->isValid());
    asTransaction([&](Transaction& t) {
        t.setColor(colorLayer, half3{0, 0, 0});
        t.show(colorLayer);
    });
    {
        mCapture = screenshot();
        // Top left of background must now be visible
        mCapture->expectBGColor(0, 0);
        // Foreground Surface bounds must be color layer
        mCapture->expectColor(Rect(64, 64, 128, 128), Color::BLACK);
        // Color layer should not extend past foreground bounds
        mCapture->expectBGColor(129, 129);
    }
}

// Verify a boundless color layer will fill its parent bounds. The parent has no buffer but has
// a crop which will be used to crop the color layer.
TEST_F(BoundlessLayerTest, BoundlessColorLayerFillsParentCropBounds) {
    sp<SurfaceControl> cropLayer = createSurface(mClient, "CropLayer", 0, 0, PIXEL_FORMAT_RGBA_8888,
                                                 0 /* flags */, mFGSurfaceControl.get());
    ASSERT_TRUE(cropLayer->isValid());
    sp<SurfaceControl> colorLayer =
            createSurface(mClient, "ColorLayer", 0, 0, PIXEL_FORMAT_RGBA_8888,
                          ISurfaceComposerClient::eFXSurfaceColor, cropLayer.get());
    ASSERT_TRUE(colorLayer->isValid());
    asTransaction([&](Transaction& t) {
        t.setCrop_legacy(cropLayer, Rect(5, 5, 10, 10));
        t.setColor(colorLayer, half3{0, 0, 0});
        t.show(cropLayer);
        t.show(colorLayer);
    });
    {
        mCapture = screenshot();
        // Top left of background must now be visible
        mCapture->expectBGColor(0, 0);
        // Top left of foreground must now be visible
        mCapture->expectFGColor(64, 64);
        // 5 pixels from the foreground we should see the child surface
        mCapture->expectColor(Rect(69, 69, 74, 74), Color::BLACK);
        // 10 pixels from the foreground we should be back to the foreground surface
        mCapture->expectFGColor(74, 74);
    }
}

// Verify for boundless layer with no children, their transforms have no effect.
TEST_F(BoundlessLayerTest, BoundlessColorLayerTransformHasNoEffect) {
    sp<SurfaceControl> colorLayer =
            createSurface(mClient, "ColorLayer", 0, 0, PIXEL_FORMAT_RGBA_8888,
                          ISurfaceComposerClient::eFXSurfaceColor, mFGSurfaceControl.get());
    ASSERT_TRUE(colorLayer->isValid());
    asTransaction([&](Transaction& t) {
        t.setPosition(colorLayer, 320, 320);
        t.setMatrix(colorLayer, 2, 0, 0, 2);
        t.setColor(colorLayer, half3{0, 0, 0});
        t.show(colorLayer);
    });
    {
        mCapture = screenshot();
        // Top left of background must now be visible
        mCapture->expectBGColor(0, 0);
        // Foreground Surface bounds must be color layer
        mCapture->expectColor(Rect(64, 64, 128, 128), Color::BLACK);
        // Color layer should not extend past foreground bounds
        mCapture->expectBGColor(129, 129);
    }
}

// Verify for boundless layer with children, their transforms have an effect.
TEST_F(BoundlessLayerTest, IntermediateBoundlessLayerCanSetTransform) {
    sp<SurfaceControl> boundlessLayerRightShift =
            createSurface(mClient, "BoundlessLayerRightShift", 0, 0, PIXEL_FORMAT_RGBA_8888,
                          0 /* flags */, mFGSurfaceControl.get());
    ASSERT_TRUE(boundlessLayerRightShift->isValid());
    sp<SurfaceControl> boundlessLayerDownShift =
            createSurface(mClient, "BoundlessLayerLeftShift", 0, 0, PIXEL_FORMAT_RGBA_8888,
                          0 /* flags */, boundlessLayerRightShift.get());
    ASSERT_TRUE(boundlessLayerDownShift->isValid());
    sp<SurfaceControl> colorLayer =
            createSurface(mClient, "ColorLayer", 0, 0, PIXEL_FORMAT_RGBA_8888,
                          ISurfaceComposerClient::eFXSurfaceColor, boundlessLayerDownShift.get());
    ASSERT_TRUE(colorLayer->isValid());
    asTransaction([&](Transaction& t) {
        t.setPosition(boundlessLayerRightShift, 32, 0);
        t.show(boundlessLayerRightShift);
        t.setPosition(boundlessLayerDownShift, 0, 32);
        t.show(boundlessLayerDownShift);
        t.setCrop_legacy(colorLayer, Rect(0, 0, 64, 64));
        t.setColor(colorLayer, half3{0, 0, 0});
        t.show(colorLayer);
    });
    {
        mCapture = screenshot();
        // Top left of background must now be visible
        mCapture->expectBGColor(0, 0);
        // Top left of foreground must now be visible
        mCapture->expectFGColor(64, 64);
        // Foreground Surface bounds must be color layer
        mCapture->expectColor(Rect(96, 96, 128, 128), Color::BLACK);
        // Color layer should not extend past foreground bounds
        mCapture->expectBGColor(129, 129);
    }
}

// Verify child layers do not get clipped if they temporarily move into the negative
// coordinate space as the result of an intermediate transformation.
TEST_F(BoundlessLayerTest, IntermediateBoundlessLayerDoNotCrop) {
    sp<SurfaceControl> boundlessLayer =
            mClient->createSurface(String8("BoundlessLayer"), 0, 0, PIXEL_FORMAT_RGBA_8888,
                                   0 /* flags */, mFGSurfaceControl.get());
    ASSERT_TRUE(boundlessLayer != nullptr);
    ASSERT_TRUE(boundlessLayer->isValid());
    sp<SurfaceControl> colorLayer =
            mClient->createSurface(String8("ColorLayer"), 0, 0, PIXEL_FORMAT_RGBA_8888,
                                   ISurfaceComposerClient::eFXSurfaceColor, boundlessLayer.get());
    ASSERT_TRUE(colorLayer != nullptr);
    ASSERT_TRUE(colorLayer->isValid());
    asTransaction([&](Transaction& t) {
        // shift child layer off bounds. If this layer was not boundless, we will
        // expect the child layer to be cropped.
        t.setPosition(boundlessLayer, 32, 32);
        t.show(boundlessLayer);
        t.setCrop_legacy(colorLayer, Rect(0, 0, 64, 64));
        // undo shift by parent
        t.setPosition(colorLayer, -32, -32);
        t.setColor(colorLayer, half3{0, 0, 0});
        t.show(colorLayer);
    });
    {
        mCapture = screenshot();
        // Top left of background must now be visible
        mCapture->expectBGColor(0, 0);
        // Foreground Surface bounds must be color layer
        mCapture->expectColor(Rect(64, 64, 128, 128), Color::BLACK);
        // Color layer should not extend past foreground bounds
        mCapture->expectBGColor(129, 129);
    }
}

// Verify for boundless root layers with children, their transforms have an effect.
TEST_F(BoundlessLayerTest, RootBoundlessLayerCanSetTransform) {
    sp<SurfaceControl> rootBoundlessLayer = createSurface(mClient, "RootBoundlessLayer", 0, 0,
                                                          PIXEL_FORMAT_RGBA_8888, 0 /* flags */);
    ASSERT_TRUE(rootBoundlessLayer->isValid());
    sp<SurfaceControl> colorLayer =
            createSurface(mClient, "ColorLayer", 0, 0, PIXEL_FORMAT_RGBA_8888,
                          ISurfaceComposerClient::eFXSurfaceColor, rootBoundlessLayer.get());

    ASSERT_TRUE(colorLayer->isValid());
    asTransaction([&](Transaction& t) {
        t.setLayer(rootBoundlessLayer, INT32_MAX - 1);
        t.setPosition(rootBoundlessLayer, 32, 32);
        t.show(rootBoundlessLayer);
        t.setCrop_legacy(colorLayer, Rect(0, 0, 64, 64));
        t.setColor(colorLayer, half3{0, 0, 0});
        t.show(colorLayer);
        t.hide(mFGSurfaceControl);
    });
    {
        mCapture = screenshot();
        // Top left of background must now be visible
        mCapture->expectBGColor(0, 0);
        // Top left of foreground must now be visible
        mCapture->expectBGColor(31, 31);
        // Foreground Surface bounds must be color layer
        mCapture->expectColor(Rect(32, 32, 96, 96), Color::BLACK);
        // Color layer should not extend past foreground bounds
        mCapture->expectBGColor(97, 97);
    }
}

class ScreenCaptureTest : public LayerUpdateTest {
protected:
    std::unique_ptr<ScreenCapture> mCapture;
};

TEST_F(ScreenCaptureTest, CaptureSingleLayer) {
    auto bgHandle = mBGSurfaceControl->getHandle();
    ScreenCapture::captureLayers(&mCapture, bgHandle);
    mCapture->expectBGColor(0, 0);
    // Doesn't capture FG layer which is at 64, 64
    mCapture->expectBGColor(64, 64);
}

TEST_F(ScreenCaptureTest, CaptureLayerWithChild) {
    auto fgHandle = mFGSurfaceControl->getHandle();

    sp<SurfaceControl> child = createSurface(mClient, "Child surface", 10, 10,
                                             PIXEL_FORMAT_RGBA_8888, 0, mFGSurfaceControl.get());
    fillSurfaceRGBA8(child, 200, 200, 200);

    SurfaceComposerClient::Transaction().show(child).apply(true);

    // Captures mFGSurfaceControl layer and its child.
    ScreenCapture::captureLayers(&mCapture, fgHandle);
    mCapture->expectFGColor(10, 10);
    mCapture->expectChildColor(0, 0);
}

TEST_F(ScreenCaptureTest, CaptureLayerChildOnly) {
    auto fgHandle = mFGSurfaceControl->getHandle();

    sp<SurfaceControl> child = createSurface(mClient, "Child surface", 10, 10,
                                             PIXEL_FORMAT_RGBA_8888, 0, mFGSurfaceControl.get());
    fillSurfaceRGBA8(child, 200, 200, 200);

    SurfaceComposerClient::Transaction().show(child).apply(true);

    // Captures mFGSurfaceControl's child
    ScreenCapture::captureChildLayers(&mCapture, fgHandle);
    mCapture->checkPixel(10, 10, 0, 0, 0);
    mCapture->expectChildColor(0, 0);
}

TEST_F(ScreenCaptureTest, CaptureLayerExclude) {
    auto fgHandle = mFGSurfaceControl->getHandle();

    sp<SurfaceControl> child = createSurface(mClient, "Child surface", 10, 10,
                                             PIXEL_FORMAT_RGBA_8888, 0, mFGSurfaceControl.get());
    fillSurfaceRGBA8(child, 200, 200, 200);
    sp<SurfaceControl> child2 = createSurface(mClient, "Child surface", 10, 10,
                                              PIXEL_FORMAT_RGBA_8888, 0, mFGSurfaceControl.get());
    fillSurfaceRGBA8(child2, 200, 0, 200);

    SurfaceComposerClient::Transaction()
            .show(child)
            .show(child2)
            .setLayer(child, 1)
            .setLayer(child2, 2)
            .apply(true);

    // Child2 would be visible but its excluded, so we should see child1 color instead.
    ScreenCapture::captureChildLayersExcluding(&mCapture, fgHandle, {child2->getHandle()});
    mCapture->checkPixel(10, 10, 0, 0, 0);
    mCapture->checkPixel(0, 0, 200, 200, 200);
}

// Like the last test but verifies that children are also exclude.
TEST_F(ScreenCaptureTest, CaptureLayerExcludeTree) {
    auto fgHandle = mFGSurfaceControl->getHandle();

    sp<SurfaceControl> child = createSurface(mClient, "Child surface", 10, 10,
                                             PIXEL_FORMAT_RGBA_8888, 0, mFGSurfaceControl.get());
    fillSurfaceRGBA8(child, 200, 200, 200);
    sp<SurfaceControl> child2 = createSurface(mClient, "Child surface", 10, 10,
                                              PIXEL_FORMAT_RGBA_8888, 0, mFGSurfaceControl.get());
    fillSurfaceRGBA8(child2, 200, 0, 200);
    sp<SurfaceControl> child3 = createSurface(mClient, "Child surface", 10, 10,
                                              PIXEL_FORMAT_RGBA_8888, 0, child2.get());
    fillSurfaceRGBA8(child2, 200, 0, 200);

    SurfaceComposerClient::Transaction()
            .show(child)
            .show(child2)
            .show(child3)
            .setLayer(child, 1)
            .setLayer(child2, 2)
            .apply(true);

    // Child2 would be visible but its excluded, so we should see child1 color instead.
    ScreenCapture::captureChildLayersExcluding(&mCapture, fgHandle, {child2->getHandle()});
    mCapture->checkPixel(10, 10, 0, 0, 0);
    mCapture->checkPixel(0, 0, 200, 200, 200);
}

TEST_F(ScreenCaptureTest, CaptureTransparent) {
    sp<SurfaceControl> child = createSurface(mClient, "Child surface", 10, 10,
                                             PIXEL_FORMAT_RGBA_8888, 0, mFGSurfaceControl.get());

    fillSurfaceRGBA8(child, 200, 200, 200);

    SurfaceComposerClient::Transaction().show(child).apply(true);

    auto childHandle = child->getHandle();

    // Captures child
    ScreenCapture::captureLayers(&mCapture, childHandle, {0, 0, 10, 20});
    mCapture->expectColor(Rect(0, 0, 9, 9), {200, 200, 200, 255});
    // Area outside of child's bounds is transparent.
    mCapture->expectColor(Rect(0, 10, 9, 19), {0, 0, 0, 0});
}

TEST_F(ScreenCaptureTest, DontCaptureRelativeOutsideTree) {
    auto fgHandle = mFGSurfaceControl->getHandle();

    sp<SurfaceControl> child = createSurface(mClient, "Child surface", 10, 10,
                                             PIXEL_FORMAT_RGBA_8888, 0, mFGSurfaceControl.get());
    ASSERT_NE(nullptr, child.get()) << "failed to create surface";
    sp<SurfaceControl> relative = createLayer(String8("Relative surface"), 10, 10, 0);
    fillSurfaceRGBA8(child, 200, 200, 200);
    fillSurfaceRGBA8(relative, 100, 100, 100);

    SurfaceComposerClient::Transaction()
            .show(child)
            // Set relative layer above fg layer so should be shown above when computing all layers.
            .setRelativeLayer(relative, fgHandle, 1)
            .show(relative)
            .apply(true);

    // Captures mFGSurfaceControl layer and its child. Relative layer shouldn't be captured.
    ScreenCapture::captureLayers(&mCapture, fgHandle);
    mCapture->expectFGColor(10, 10);
    mCapture->expectChildColor(0, 0);
}

TEST_F(ScreenCaptureTest, CaptureRelativeInTree) {
    auto fgHandle = mFGSurfaceControl->getHandle();

    sp<SurfaceControl> child = createSurface(mClient, "Child surface", 10, 10,
                                             PIXEL_FORMAT_RGBA_8888, 0, mFGSurfaceControl.get());
    sp<SurfaceControl> relative = createSurface(mClient, "Relative surface", 10, 10,
                                                PIXEL_FORMAT_RGBA_8888, 0, mFGSurfaceControl.get());
    fillSurfaceRGBA8(child, 200, 200, 200);
    fillSurfaceRGBA8(relative, 100, 100, 100);

    SurfaceComposerClient::Transaction()
            .show(child)
            // Set relative layer below fg layer but relative to child layer so it should be shown
            // above child layer.
            .setLayer(relative, -1)
            .setRelativeLayer(relative, child->getHandle(), 1)
            .show(relative)
            .apply(true);

    // Captures mFGSurfaceControl layer and its children. Relative layer is a child of fg so its
    // relative value should be taken into account, placing it above child layer.
    ScreenCapture::captureLayers(&mCapture, fgHandle);
    mCapture->expectFGColor(10, 10);
    // Relative layer is showing on top of child layer
    mCapture->expectColor(Rect(0, 0, 9, 9), {100, 100, 100, 255});
}

// In the following tests we verify successful skipping of a parent layer,
// so we use the same verification logic and only change how we mutate
// the parent layer to verify that various properties are ignored.
class ScreenCaptureChildOnlyTest : public LayerUpdateTest {
public:
    void SetUp() override {
        LayerUpdateTest::SetUp();

        mChild = createSurface(mClient, "Child surface", 10, 10, PIXEL_FORMAT_RGBA_8888, 0,
                               mFGSurfaceControl.get());
        fillSurfaceRGBA8(mChild, 200, 200, 200);

        SurfaceComposerClient::Transaction().show(mChild).apply(true);
    }

    void verify(std::function<void()> verifyStartingState) {
        // Verify starting state before a screenshot is taken.
        verifyStartingState();

        // Verify child layer does not inherit any of the properties of its
        // parent when its screenshot is captured.
        auto fgHandle = mFGSurfaceControl->getHandle();
        ScreenCapture::captureChildLayers(&mCapture, fgHandle);
        mCapture->checkPixel(10, 10, 0, 0, 0);
        mCapture->expectChildColor(0, 0);

        // Verify all assumptions are still true after the screenshot is taken.
        verifyStartingState();
    }

    std::unique_ptr<ScreenCapture> mCapture;
    sp<SurfaceControl> mChild;
};

// Regression test b/76099859
TEST_F(ScreenCaptureChildOnlyTest, CaptureLayerIgnoresParentVisibility) {

    SurfaceComposerClient::Transaction().hide(mFGSurfaceControl).apply(true);

    // Even though the parent is hidden we should still capture the child.

    // Before and after reparenting, verify child is properly hidden
    // when rendering full-screen.
    verify([&] { screenshot()->expectBGColor(64, 64); });
}

TEST_F(ScreenCaptureChildOnlyTest, CaptureLayerIgnoresParentCrop) {
    SurfaceComposerClient::Transaction()
            .setCrop_legacy(mFGSurfaceControl, Rect(0, 0, 1, 1))
            .apply(true);

    // Even though the parent is cropped out we should still capture the child.

    // Before and after reparenting, verify child is cropped by parent.
    verify([&] { screenshot()->expectBGColor(65, 65); });
}

// Regression test b/124372894
TEST_F(ScreenCaptureChildOnlyTest, CaptureLayerIgnoresTransform) {
    SurfaceComposerClient::Transaction().setMatrix(mFGSurfaceControl, 2, 0, 0, 2).apply(true);

    // We should not inherit the parent scaling.

    // Before and after reparenting, verify child is properly scaled.
    verify([&] { screenshot()->expectChildColor(80, 80); });
}


TEST_F(ScreenCaptureTest, CaptureLayerWithGrandchild) {
    auto fgHandle = mFGSurfaceControl->getHandle();

    sp<SurfaceControl> child = createSurface(mClient, "Child surface", 10, 10,
                                             PIXEL_FORMAT_RGBA_8888, 0, mFGSurfaceControl.get());
    fillSurfaceRGBA8(child, 200, 200, 200);

    sp<SurfaceControl> grandchild = createSurface(mClient, "Grandchild surface", 5, 5,
                                                  PIXEL_FORMAT_RGBA_8888, 0, child.get());

    fillSurfaceRGBA8(grandchild, 50, 50, 50);
    SurfaceComposerClient::Transaction()
            .show(child)
            .setPosition(grandchild, 5, 5)
            .show(grandchild)
            .apply(true);

    // Captures mFGSurfaceControl, its child, and the grandchild.
    ScreenCapture::captureLayers(&mCapture, fgHandle);
    mCapture->expectFGColor(10, 10);
    mCapture->expectChildColor(0, 0);
    mCapture->checkPixel(5, 5, 50, 50, 50);
}

TEST_F(ScreenCaptureTest, CaptureChildOnly) {
    sp<SurfaceControl> child = createSurface(mClient, "Child surface", 10, 10,
                                             PIXEL_FORMAT_RGBA_8888, 0, mFGSurfaceControl.get());
    fillSurfaceRGBA8(child, 200, 200, 200);
    auto childHandle = child->getHandle();

    SurfaceComposerClient::Transaction().setPosition(child, 5, 5).show(child).apply(true);

    // Captures only the child layer, and not the parent.
    ScreenCapture::captureLayers(&mCapture, childHandle);
    mCapture->expectChildColor(0, 0);
    mCapture->expectChildColor(9, 9);
}

TEST_F(ScreenCaptureTest, CaptureGrandchildOnly) {
    sp<SurfaceControl> child = createSurface(mClient, "Child surface", 10, 10,
                                             PIXEL_FORMAT_RGBA_8888, 0, mFGSurfaceControl.get());
    fillSurfaceRGBA8(child, 200, 200, 200);
    auto childHandle = child->getHandle();

    sp<SurfaceControl> grandchild = createSurface(mClient, "Grandchild surface", 5, 5,
                                                  PIXEL_FORMAT_RGBA_8888, 0, child.get());
    fillSurfaceRGBA8(grandchild, 50, 50, 50);

    SurfaceComposerClient::Transaction()
            .show(child)
            .setPosition(grandchild, 5, 5)
            .show(grandchild)
            .apply(true);

    auto grandchildHandle = grandchild->getHandle();

    // Captures only the grandchild.
    ScreenCapture::captureLayers(&mCapture, grandchildHandle);
    mCapture->checkPixel(0, 0, 50, 50, 50);
    mCapture->checkPixel(4, 4, 50, 50, 50);
}

TEST_F(ScreenCaptureTest, CaptureCrop) {
    sp<SurfaceControl> redLayer = createLayer(String8("Red surface"), 60, 60, 0);
    sp<SurfaceControl> blueLayer = createSurface(mClient, "Blue surface", 30, 30,
                                                 PIXEL_FORMAT_RGBA_8888, 0, redLayer.get());

    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(redLayer, Color::RED, 60, 60));
    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(blueLayer, Color::BLUE, 30, 30));

    SurfaceComposerClient::Transaction()
            .setLayer(redLayer, INT32_MAX - 1)
            .show(redLayer)
            .show(blueLayer)
            .apply(true);

    auto redLayerHandle = redLayer->getHandle();

    // Capturing full screen should have both red and blue are visible.
    ScreenCapture::captureLayers(&mCapture, redLayerHandle);
    mCapture->expectColor(Rect(0, 0, 29, 29), Color::BLUE);
    // red area below the blue area
    mCapture->expectColor(Rect(0, 30, 59, 59), Color::RED);
    // red area to the right of the blue area
    mCapture->expectColor(Rect(30, 0, 59, 59), Color::RED);

    const Rect crop = Rect(0, 0, 30, 30);
    ScreenCapture::captureLayers(&mCapture, redLayerHandle, crop);
    // Capturing the cropped screen, cropping out the shown red area, should leave only the blue
    // area visible.
    mCapture->expectColor(Rect(0, 0, 29, 29), Color::BLUE);
    mCapture->checkPixel(30, 30, 0, 0, 0);
}

TEST_F(ScreenCaptureTest, CaptureSize) {
    sp<SurfaceControl> redLayer = createLayer(String8("Red surface"), 60, 60, 0);
    sp<SurfaceControl> blueLayer = createSurface(mClient, "Blue surface", 30, 30,
                                                 PIXEL_FORMAT_RGBA_8888, 0, redLayer.get());

    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(redLayer, Color::RED, 60, 60));
    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(blueLayer, Color::BLUE, 30, 30));

    SurfaceComposerClient::Transaction()
            .setLayer(redLayer, INT32_MAX - 1)
            .show(redLayer)
            .show(blueLayer)
            .apply(true);

    auto redLayerHandle = redLayer->getHandle();

    // Capturing full screen should have both red and blue are visible.
    ScreenCapture::captureLayers(&mCapture, redLayerHandle);
    mCapture->expectColor(Rect(0, 0, 29, 29), Color::BLUE);
    // red area below the blue area
    mCapture->expectColor(Rect(0, 30, 59, 59), Color::RED);
    // red area to the right of the blue area
    mCapture->expectColor(Rect(30, 0, 59, 59), Color::RED);

    ScreenCapture::captureLayers(&mCapture, redLayerHandle, Rect::EMPTY_RECT, 0.5);
    // Capturing the downsized area (30x30) should leave both red and blue but in a smaller area.
    mCapture->expectColor(Rect(0, 0, 14, 14), Color::BLUE);
    // red area below the blue area
    mCapture->expectColor(Rect(0, 15, 29, 29), Color::RED);
    // red area to the right of the blue area
    mCapture->expectColor(Rect(15, 0, 29, 29), Color::RED);
    mCapture->checkPixel(30, 30, 0, 0, 0);
}

TEST_F(ScreenCaptureTest, CaptureInvalidLayer) {
    sp<SurfaceControl> redLayer = createLayer(String8("Red surface"), 60, 60, 0);

    ASSERT_NO_FATAL_FAILURE(fillBufferQueueLayerColor(redLayer, Color::RED, 60, 60));

    auto redLayerHandle = redLayer->getHandle();
    redLayer.clear();
    SurfaceComposerClient::Transaction().apply(true);

    sp<GraphicBuffer> outBuffer;

    // Layer was deleted so captureLayers should fail with NAME_NOT_FOUND
    sp<ISurfaceComposer> sf(ComposerService::getComposerService());
    ASSERT_EQ(NAME_NOT_FOUND, sf->captureLayers(redLayerHandle, &outBuffer, Rect::EMPTY_RECT, 1.0));
}


class DereferenceSurfaceControlTest : public LayerTransactionTest {
protected:
    void SetUp() override {
        LayerTransactionTest::SetUp();
        bgLayer = createLayer("BG layer", 20, 20);
        fillBufferQueueLayerColor(bgLayer, Color::RED, 20, 20);
        fgLayer = createLayer("FG layer", 20, 20);
        fillBufferQueueLayerColor(fgLayer, Color::BLUE, 20, 20);
        Transaction().setLayer(fgLayer, mLayerZBase + 1).apply();
        {
            SCOPED_TRACE("before anything");
            auto shot = screenshot();
            shot->expectColor(Rect(0, 0, 20, 20), Color::BLUE);
        }
    }
    void TearDown() override {
        LayerTransactionTest::TearDown();
        bgLayer = 0;
        fgLayer = 0;
    }

    sp<SurfaceControl> bgLayer;
    sp<SurfaceControl> fgLayer;
};

TEST_F(DereferenceSurfaceControlTest, LayerNotInTransaction) {
    fgLayer = nullptr;
    {
        SCOPED_TRACE("after setting null");
        auto shot = screenshot();
        shot->expectColor(Rect(0, 0, 20, 20), Color::RED);
    }
}

TEST_F(DereferenceSurfaceControlTest, LayerInTransaction) {
    auto transaction = Transaction().show(fgLayer);
    fgLayer = nullptr;
    {
        SCOPED_TRACE("after setting null");
        auto shot = screenshot();
        shot->expectColor(Rect(0, 0, 20, 20), Color::BLUE);
    }
}

class MultiDisplayLayerBoundsTest : public LayerTransactionTest {
protected:
    virtual void SetUp() {
        LayerTransactionTest::SetUp();
        ASSERT_EQ(NO_ERROR, mClient->initCheck());

        mMainDisplay = SurfaceComposerClient::getInternalDisplayToken();
        SurfaceComposerClient::getDisplayInfo(mMainDisplay, &mMainDisplayInfo);

        sp<IGraphicBufferConsumer> consumer;
        BufferQueue::createBufferQueue(&mProducer, &consumer);
        consumer->setConsumerName(String8("Virtual disp consumer"));
        consumer->setDefaultBufferSize(mMainDisplayInfo.w, mMainDisplayInfo.h);
    }

    virtual void TearDown() {
        SurfaceComposerClient::destroyDisplay(mVirtualDisplay);
        LayerTransactionTest::TearDown();
        mColorLayer = 0;
    }

    void createDisplay(const Rect& layerStackRect, uint32_t layerStack) {
        mVirtualDisplay =
                SurfaceComposerClient::createDisplay(String8("VirtualDisplay"), false /*secure*/);
        asTransaction([&](Transaction& t) {
            t.setDisplaySurface(mVirtualDisplay, mProducer);
            t.setDisplayLayerStack(mVirtualDisplay, layerStack);
            t.setDisplayProjection(mVirtualDisplay, mMainDisplayInfo.orientation, layerStackRect,
                                   Rect(mMainDisplayInfo.w, mMainDisplayInfo.h));
        });
    }

    void createColorLayer(uint32_t layerStack) {
        mColorLayer =
                createSurface(mClient, "ColorLayer", 0 /* buffer width */, 0 /* buffer height */,
                              PIXEL_FORMAT_RGBA_8888, ISurfaceComposerClient::eFXSurfaceColor);
        ASSERT_TRUE(mColorLayer != nullptr);
        ASSERT_TRUE(mColorLayer->isValid());
        asTransaction([&](Transaction& t) {
            t.setLayerStack(mColorLayer, layerStack);
            t.setCrop_legacy(mColorLayer, Rect(0, 0, 30, 40));
            t.setLayer(mColorLayer, INT32_MAX - 2);
            t.setColor(mColorLayer,
                       half3{mExpectedColor.r / 255.0f, mExpectedColor.g / 255.0f,
                             mExpectedColor.b / 255.0f});
            t.show(mColorLayer);
        });
    }

    DisplayInfo mMainDisplayInfo;
    sp<IBinder> mMainDisplay;
    sp<IBinder> mVirtualDisplay;
    sp<IGraphicBufferProducer> mProducer;
    sp<SurfaceControl> mColorLayer;
    Color mExpectedColor = {63, 63, 195, 255};
};

TEST_F(MultiDisplayLayerBoundsTest, RenderLayerInVirtualDisplay) {
    createDisplay({mMainDisplayInfo.viewportW, mMainDisplayInfo.viewportH}, 1 /* layerStack */);
    createColorLayer(1 /* layerStack */);

    asTransaction([&](Transaction& t) { t.setPosition(mColorLayer, 10, 10); });

    // Verify color layer does not render on main display.
    std::unique_ptr<ScreenCapture> sc;
    ScreenCapture::captureScreen(&sc, mMainDisplay);
    sc->expectColor(Rect(10, 10, 40, 50), {0, 0, 0, 255});
    sc->expectColor(Rect(0, 0, 9, 9), {0, 0, 0, 255});

    // Verify color layer renders correctly on virtual display.
    ScreenCapture::captureScreen(&sc, mVirtualDisplay);
    sc->expectColor(Rect(10, 10, 40, 50), mExpectedColor);
    sc->expectColor(Rect(1, 1, 9, 9), {0, 0, 0, 0});
}

TEST_F(MultiDisplayLayerBoundsTest, RenderLayerInMirroredVirtualDisplay) {
    // Create a display and set its layer stack to the main display's layer stack so
    // the contents of the main display are mirrored on to the virtual display.

    // Assumption here is that the new mirrored display has the same viewport as the
    // primary display that it is mirroring.
    createDisplay({mMainDisplayInfo.viewportW, mMainDisplayInfo.viewportH}, 0 /* layerStack */);
    createColorLayer(0 /* layerStack */);

    asTransaction([&](Transaction& t) { t.setPosition(mColorLayer, 10, 10); });

    // Verify color layer renders correctly on main display and it is mirrored on the
    // virtual display.
    std::unique_ptr<ScreenCapture> sc;
    ScreenCapture::captureScreen(&sc, mMainDisplay);
    sc->expectColor(Rect(10, 10, 40, 50), mExpectedColor);
    sc->expectColor(Rect(0, 0, 9, 9), {0, 0, 0, 255});

    ScreenCapture::captureScreen(&sc, mVirtualDisplay);
    sc->expectColor(Rect(10, 10, 40, 50), mExpectedColor);
    sc->expectColor(Rect(0, 0, 9, 9), {0, 0, 0, 255});
}

class DisplayActiveConfigTest : public ::testing::Test {
protected:
    void SetUp() override {
        mDisplayToken = SurfaceComposerClient::getInternalDisplayToken();
        SurfaceComposerClient::getDisplayConfigs(mDisplayToken, &mDisplayconfigs);
        EXPECT_GT(mDisplayconfigs.size(), 0);

        // set display power to on to make sure config can be changed
        SurfaceComposerClient::setDisplayPowerMode(mDisplayToken, HWC_POWER_MODE_NORMAL);
    }

    sp<IBinder> mDisplayToken;
    Vector<DisplayInfo> mDisplayconfigs;
};

TEST_F(DisplayActiveConfigTest, allConfigsAllowed) {
    std::vector<int32_t> allowedConfigs;

    // Add all configs to the allowed configs
    for (int i = 0; i < mDisplayconfigs.size(); i++) {
        allowedConfigs.push_back(i);
    }

    status_t res = SurfaceComposerClient::setAllowedDisplayConfigs(mDisplayToken, allowedConfigs);
    EXPECT_EQ(res, NO_ERROR);

    std::vector<int32_t> outConfigs;
    res = SurfaceComposerClient::getAllowedDisplayConfigs(mDisplayToken, &outConfigs);
    EXPECT_EQ(res, NO_ERROR);
    EXPECT_EQ(allowedConfigs, outConfigs);
}

TEST_F(DisplayActiveConfigTest, changeAllowedConfig) {
    // we need at least 2 configs available for this test
    if (mDisplayconfigs.size() <= 1) return;

    int activeConfig = SurfaceComposerClient::getActiveConfig(mDisplayToken);

    // We want to set the allowed config to everything but the active config
    std::vector<int32_t> allowedConfigs;
    for (int i = 0; i < mDisplayconfigs.size(); i++) {
        if (i != activeConfig) {
            allowedConfigs.push_back(i);
        }
    }

    status_t res = SurfaceComposerClient::setAllowedDisplayConfigs(mDisplayToken, allowedConfigs);
    EXPECT_EQ(res, NO_ERROR);

    // Allow some time for the config change
    std::this_thread::sleep_for(200ms);

    int newActiveConfig = SurfaceComposerClient::getActiveConfig(mDisplayToken);
    EXPECT_NE(activeConfig, newActiveConfig);

    // Make sure the new config is part of allowed config
    EXPECT_TRUE(std::find(allowedConfigs.begin(), allowedConfigs.end(), newActiveConfig) !=
                allowedConfigs.end());
}

class RelativeZTest : public LayerTransactionTest {
protected:
    virtual void SetUp() {
        LayerTransactionTest::SetUp();
        ASSERT_EQ(NO_ERROR, mClient->initCheck());

        const auto display = SurfaceComposerClient::getInternalDisplayToken();
        ASSERT_FALSE(display == nullptr);

        // Back layer
        mBackgroundLayer = createColorLayer("Background layer", Color::RED);

        // Front layer
        mForegroundLayer = createColorLayer("Foreground layer", Color::GREEN);

        asTransaction([&](Transaction& t) {
            t.setDisplayLayerStack(display, 0);
            t.setLayer(mBackgroundLayer, INT32_MAX - 2).show(mBackgroundLayer);
            t.setLayer(mForegroundLayer, INT32_MAX - 1).show(mForegroundLayer);
        });
    }

    virtual void TearDown() {
        LayerTransactionTest::TearDown();
        mBackgroundLayer = 0;
        mForegroundLayer = 0;
    }

    sp<SurfaceControl> mBackgroundLayer;
    sp<SurfaceControl> mForegroundLayer;
};

// When a layer is reparented offscreen, remove relative z order if the relative parent
// is still onscreen so that the layer is not drawn.
TEST_F(RelativeZTest, LayerRemoved) {
    std::unique_ptr<ScreenCapture> sc;

    // Background layer (RED)
    //   Child layer (WHITE) (relative to foregroud layer)
    // Foregroud layer (GREEN)
    sp<SurfaceControl> childLayer =
            createColorLayer("Child layer", Color::BLUE, mBackgroundLayer.get());

    Transaction{}
            .setRelativeLayer(childLayer, mForegroundLayer->getHandle(), 1)
            .show(childLayer)
            .apply();

    {
        // The childLayer should be in front of the FG control.
        ScreenCapture::captureScreen(&sc);
        sc->checkPixel(1, 1, Color::BLUE.r, Color::BLUE.g, Color::BLUE.b);
    }

    // Background layer (RED)
    // Foregroud layer (GREEN)
    Transaction{}.reparent(childLayer, nullptr).apply();

    // Background layer (RED)
    //   Child layer (WHITE)
    // Foregroud layer (GREEN)
    Transaction{}.reparent(childLayer, mBackgroundLayer->getHandle()).apply();

    {
        // The relative z info for child layer should be reset, leaving FG control on top.
        ScreenCapture::captureScreen(&sc);
        sc->checkPixel(1, 1, Color::GREEN.r, Color::GREEN.g, Color::GREEN.b);
    }
}

// When a layer is reparented offscreen, preseve relative z order if the relative parent
// is also offscreen. Regression test b/132613412
TEST_F(RelativeZTest, LayerRemovedOffscreenRelativeParent) {
    std::unique_ptr<ScreenCapture> sc;

    // Background layer (RED)
    // Foregroud layer (GREEN)
    //   child level 1 (WHITE)
    //     child level 2a (BLUE)
    //       child level 3 (GREEN) (relative to child level 2b)
    //     child level 2b (BLACK)
    sp<SurfaceControl> childLevel1 =
            createColorLayer("child level 1", Color::WHITE, mForegroundLayer.get());
    sp<SurfaceControl> childLevel2a =
            createColorLayer("child level 2a", Color::BLUE, childLevel1.get());
    sp<SurfaceControl> childLevel2b =
            createColorLayer("child level 2b", Color::BLACK, childLevel1.get());
    sp<SurfaceControl> childLevel3 =
            createColorLayer("child level 3", Color::GREEN, childLevel2a.get());

    Transaction{}
            .setRelativeLayer(childLevel3, childLevel2b->getHandle(), 1)
            .show(childLevel2a)
            .show(childLevel2b)
            .show(childLevel3)
            .apply();

    {
        // The childLevel3 should be in front of childLevel2b.
        ScreenCapture::captureScreen(&sc);
        sc->checkPixel(1, 1, Color::GREEN.r, Color::GREEN.g, Color::GREEN.b);
    }

    // Background layer (RED)
    // Foregroud layer (GREEN)
    Transaction{}.reparent(childLevel1, nullptr).apply();

    // Background layer (RED)
    // Foregroud layer (GREEN)
    //   child level 1 (WHITE)
    //     child level 2 back (BLUE)
    //       child level 3 (GREEN) (relative to child level 2b)
    //     child level 2 front (BLACK)
    Transaction{}.reparent(childLevel1, mForegroundLayer->getHandle()).apply();

    {
        // Nothing should change at this point since relative z info was preserved.
        ScreenCapture::captureScreen(&sc);
        sc->checkPixel(1, 1, Color::GREEN.r, Color::GREEN.g, Color::GREEN.b);
    }
}

// This test ensures that when we drop an app buffer in SurfaceFlinger, we merge
// the dropped buffer's damage region into the next buffer's damage region. If
// we don't do this, we'll report an incorrect damage region to hardware
// composer, resulting in broken rendering. This test checks the BufferQueue
// case.
//
// Unfortunately, we don't currently have a way to inspect the damage region
// SurfaceFlinger sends to hardware composer from a test, so this test requires
// the dev to manually watch the device's screen during the test to spot broken
// rendering. Because the results can't be automatically verified, this test is
// marked disabled.
TEST_F(LayerTransactionTest, DISABLED_BufferQueueLayerMergeDamageRegionWhenDroppingBuffers) {
    const int width = mDisplayWidth;
    const int height = mDisplayHeight;
    sp<SurfaceControl> layer;
    ASSERT_NO_FATAL_FAILURE(layer = createLayer("test", width, height));
    const auto producer = layer->getIGraphicBufferProducer();
    const sp<IProducerListener> dummyListener(new DummyProducerListener);
    IGraphicBufferProducer::QueueBufferOutput queueBufferOutput;
    ASSERT_EQ(OK,
              producer->connect(dummyListener, NATIVE_WINDOW_API_CPU, true, &queueBufferOutput));

    std::map<int, sp<GraphicBuffer>> slotMap;
    auto slotToBuffer = [&](int slot, sp<GraphicBuffer>* buf) {
        ASSERT_NE(nullptr, buf);
        const auto iter = slotMap.find(slot);
        ASSERT_NE(slotMap.end(), iter);
        *buf = iter->second;
    };

    auto dequeue = [&](int* outSlot) {
        ASSERT_NE(nullptr, outSlot);
        *outSlot = -1;
        int slot;
        sp<Fence> fence;
        uint64_t age;
        FrameEventHistoryDelta timestamps;
        const status_t dequeueResult =
                producer->dequeueBuffer(&slot, &fence, width, height, PIXEL_FORMAT_RGBA_8888,
                                        GRALLOC_USAGE_SW_READ_OFTEN | GRALLOC_USAGE_SW_WRITE_OFTEN,
                                        &age, &timestamps);
        if (dequeueResult == IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION) {
            sp<GraphicBuffer> newBuf;
            ASSERT_EQ(OK, producer->requestBuffer(slot, &newBuf));
            ASSERT_NE(nullptr, newBuf.get());
            slotMap[slot] = newBuf;
        } else {
            ASSERT_EQ(OK, dequeueResult);
        }
        *outSlot = slot;
    };

    auto queue = [&](int slot, const Region& damage, nsecs_t displayTime) {
        IGraphicBufferProducer::QueueBufferInput input(
                /*timestamp=*/displayTime, /*isAutoTimestamp=*/false, HAL_DATASPACE_UNKNOWN,
                /*crop=*/Rect::EMPTY_RECT, NATIVE_WINDOW_SCALING_MODE_SCALE_TO_WINDOW,
                /*transform=*/0, Fence::NO_FENCE);
        input.setSurfaceDamage(damage);
        IGraphicBufferProducer::QueueBufferOutput output;
        ASSERT_EQ(OK, producer->queueBuffer(slot, input, &output));
    };

    auto fillAndPostBuffers = [&](const Color& color) {
        int slot1;
        ASSERT_NO_FATAL_FAILURE(dequeue(&slot1));
        int slot2;
        ASSERT_NO_FATAL_FAILURE(dequeue(&slot2));

        sp<GraphicBuffer> buf1;
        ASSERT_NO_FATAL_FAILURE(slotToBuffer(slot1, &buf1));
        sp<GraphicBuffer> buf2;
        ASSERT_NO_FATAL_FAILURE(slotToBuffer(slot2, &buf2));
        fillGraphicBufferColor(buf1, Rect(width, height), color);
        fillGraphicBufferColor(buf2, Rect(width, height), color);

        const auto displayTime = systemTime() + milliseconds_to_nanoseconds(100);
        ASSERT_NO_FATAL_FAILURE(queue(slot1, Region::INVALID_REGION, displayTime));
        ASSERT_NO_FATAL_FAILURE(
                queue(slot2, Region(Rect(width / 3, height / 3, 2 * width / 3, 2 * height / 3)),
                      displayTime));
    };

    const auto startTime = systemTime();
    const std::array<Color, 3> colors = {Color::RED, Color::GREEN, Color::BLUE};
    int colorIndex = 0;
    while (nanoseconds_to_seconds(systemTime() - startTime) < 10) {
        ASSERT_NO_FATAL_FAILURE(fillAndPostBuffers(colors[colorIndex++ % colors.size()]));
        std::this_thread::sleep_for(1s);
    }

    ASSERT_EQ(OK, producer->disconnect(NATIVE_WINDOW_API_CPU));
}

} // namespace android
