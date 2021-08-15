/*
 * Copyright 2014 The Android Open Source Project
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

#include <WindowSurface.h>

#include <gui/SurfaceComposerClient.h>
#include <gui/ISurfaceComposer.h>
#include <gui/Surface.h>
#include <ui/DisplayInfo.h>

using namespace android;

WindowSurface::WindowSurface() {
    status_t err;

    sp<SurfaceComposerClient> surfaceComposerClient = new SurfaceComposerClient;
    err = surfaceComposerClient->initCheck();
    if (err != NO_ERROR) {
        fprintf(stderr, "SurfaceComposerClient::initCheck error: %#x\n", err);
        return;
    }

    // Get main display parameters.
    const auto mainDpy = SurfaceComposerClient::getInternalDisplayToken();
    if (mainDpy == nullptr) {
        fprintf(stderr, "ERROR: no display\n");
        return;
    }

    DisplayInfo mainDpyInfo;
    err = SurfaceComposerClient::getDisplayInfo(mainDpy, &mainDpyInfo);
    if (err != NO_ERROR) {
        fprintf(stderr, "ERROR: unable to get display characteristics\n");
        return;
    }

    uint32_t width, height;
    if (mainDpyInfo.orientation != DISPLAY_ORIENTATION_0 &&
            mainDpyInfo.orientation != DISPLAY_ORIENTATION_180) {
        // rotated
        width = mainDpyInfo.h;
        height = mainDpyInfo.w;
    } else {
        width = mainDpyInfo.w;
        height = mainDpyInfo.h;
    }

    sp<SurfaceControl> sc = surfaceComposerClient->createSurface(
            String8("Benchmark"), width, height,
            PIXEL_FORMAT_RGBX_8888, ISurfaceComposerClient::eOpaque);
    if (sc == nullptr || !sc->isValid()) {
        fprintf(stderr, "Failed to create SurfaceControl\n");
        return;
    }

    SurfaceComposerClient::Transaction{}
            .setLayer(sc, 0x7FFFFFFF)
            .show(sc)
            .apply();

    mSurfaceControl = sc;
}

EGLNativeWindowType WindowSurface::getSurface() const {
    sp<ANativeWindow> anw = mSurfaceControl->getSurface();
    return (EGLNativeWindowType) anw.get();
}

