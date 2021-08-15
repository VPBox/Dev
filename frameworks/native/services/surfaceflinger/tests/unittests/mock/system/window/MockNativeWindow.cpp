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

#include "mock/system/window/MockNativeWindow.h"

namespace android {
namespace mock {
namespace {

int dispatch_setSwapInterval(struct ANativeWindow* window, int interval) {
    return static_cast<NativeWindow*>(window)->setSwapInterval(interval);
}

int dispatch_dequeueBuffer_DEPRECATED(struct ANativeWindow* window,
                                      struct ANativeWindowBuffer** buffer) {
    return static_cast<NativeWindow*>(window)->dequeueBuffer_DEPRECATED(buffer);
}

int dispatch_lockBuffer_DEPRECATED(struct ANativeWindow* window,
                                   struct ANativeWindowBuffer* buffer) {
    return static_cast<NativeWindow*>(window)->lockBuffer_DEPRECATED(buffer);
}

int dispatch_queueBuffer_DEPRECATED(struct ANativeWindow* window,
                                    struct ANativeWindowBuffer* buffer) {
    return static_cast<NativeWindow*>(window)->queueBuffer_DEPRECATED(buffer);
}

int dispatch_query(const struct ANativeWindow* window, int what, int* value) {
    return static_cast<const NativeWindow*>(window)->query(what, value);
}

int dispatch_perform(struct ANativeWindow* window, int operation, ...) {
    // TODO: Handle the various operations and their varargs better.
    return static_cast<NativeWindow*>(window)->perform(operation);
}

int dispatch_cancelBuffer_DEPRECATED(struct ANativeWindow* window,
                                     struct ANativeWindowBuffer* buffer) {
    return static_cast<NativeWindow*>(window)->cancelBuffer_DEPRECATED(buffer);
}

int dispatch_dequeueBuffer(struct ANativeWindow* window, struct ANativeWindowBuffer** buffer,
                           int* fenceFd) {
    return static_cast<NativeWindow*>(window)->dequeueBuffer(buffer, fenceFd);
}

int dispatch_queueBuffer(struct ANativeWindow* window, struct ANativeWindowBuffer* buffer,
                         int fenceFd) {
    return static_cast<NativeWindow*>(window)->queueBuffer(buffer, fenceFd);
}

int dispatch_cancelBuffer(struct ANativeWindow* window, struct ANativeWindowBuffer* buffer,
                          int fenceFd) {
    return static_cast<NativeWindow*>(window)->cancelBuffer(buffer, fenceFd);
}

} // namespace

NativeWindow::NativeWindow() {
    // ANativeWindow is a structure with function pointers and not a C++ class.
    // Set all the pointers to dispatch functions, which will invoke the mock
    // interface functions.
    ANativeWindow::setSwapInterval = &dispatch_setSwapInterval;
    ANativeWindow::dequeueBuffer_DEPRECATED = &dispatch_dequeueBuffer_DEPRECATED;
    ANativeWindow::lockBuffer_DEPRECATED = &dispatch_lockBuffer_DEPRECATED;
    ANativeWindow::queueBuffer_DEPRECATED = &dispatch_queueBuffer_DEPRECATED;
    ANativeWindow::query = &dispatch_query;
    ANativeWindow::perform = &dispatch_perform;
    ANativeWindow::cancelBuffer_DEPRECATED = &dispatch_cancelBuffer_DEPRECATED;
    ANativeWindow::dequeueBuffer = &dispatch_dequeueBuffer;
    ANativeWindow::queueBuffer = &dispatch_queueBuffer;
    ANativeWindow::cancelBuffer = &dispatch_cancelBuffer;
}

// Explicit default instantiation is recommended.
NativeWindow::~NativeWindow() = default;

} // namespace mock
} // namespace android
