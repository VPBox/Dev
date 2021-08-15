/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef SIMPLE_H
#define SIMPLE_H

#include <stdint.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#include <cutils/native_handle.h>

#include <hardware/hardware.h>

__BEGIN_DECLS

#define FORGROUND_COLOR "#133742"
#define ACTOR_COLOR "#424242"

/* Simple example */
typedef struct simple_t {
    /*
     * Common methods of the simple device.
     */
    struct hw_device_t common;

    /* resolution of the framebuffer's display panel in pixel per inch*/
    const float     xdpi;
    const float     ydpi;

    /* framebuffer's display panel refresh rate in frames per second */
    const float     fps;

    int (*setSwapInterval)(struct simple_t* window,
            int interval);

    /*
     * This hook is OPTIONAL.
     */
    int (*setUpdateRect)(struct simple_t* window,
            int left, int top, int width, int height);

} simple_t;

/* Holds pixel coordinates */
typedef struct {
    int px;
    int py;

    /*
     * If non NULL it will be caused by SurfaceFlinger on dumpsys
     */
    void (*doDump)(int foo, char *buff, int buff_len);

} simple_location_t;

/* convenience API for coloring */

static inline int showColor(const struct hw_module_t* module,
        struct simple_t** device) {
    return module->methods->open(module,
            FORGROUND_COLOR, (struct simple_t**)device);
}

static inline int hideColor(struct simple_t* device) {
    return device->common.close(&device->common);
}

__END_DECLS

#endif  // SIMPLE_H
