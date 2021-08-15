/*
 * Copyright (C) 2009 The Android Open Source Project
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


#ifndef ANDROID_UI_EGLUTILS_H
#define ANDROID_UI_EGLUTILS_H

#include <stdint.h>
#include <stdlib.h>
#include <vector>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <system/window.h>
#include <utils/Errors.h>
#include <utils/String8.h>

extern "C" EGLAPI const char* eglQueryStringImplementationANDROID(EGLDisplay dpy, EGLint name);

// ----------------------------------------------------------------------------
namespace android {
// ----------------------------------------------------------------------------

class EGLUtils
{
public:

    static inline const char *strerror(EGLint err);

    static inline status_t selectConfigForPixelFormat(
            EGLDisplay dpy,
            EGLint const* attrs,
            int32_t format,
            EGLConfig* outConfig);

    static inline status_t selectConfigForNativeWindow(
            EGLDisplay dpy,
            EGLint const* attrs,
            EGLNativeWindowType window,
            EGLConfig* outConfig);

    static inline String8 printGLString(const char* name, GLenum s);
    static inline String8 printEGLString(EGLDisplay dpy, const char* name, GLenum s);
    static inline String8 checkEglError(const char* op, EGLBoolean returnVal);
    static inline String8 checkGlError(const char* op);
    static inline String8 printEGLConfiguration(EGLDisplay dpy, EGLConfig config);
    static inline bool printEGLConfigurations(EGLDisplay dpy, String8& msg);
    static inline bool printEGLConfigurations(FILE* output, EGLDisplay dpy);
    static inline String8 decodeColorSpace(EGLint colorSpace);
    static inline bool hasEglExtension(EGLDisplay dpy, const char* name);
    static inline bool hasExtension(const char* exts, const char* name);
};

// ----------------------------------------------------------------------------

const char *EGLUtils::strerror(EGLint err)
{
    switch (err){
        case EGL_SUCCESS:           return "EGL_SUCCESS";
        case EGL_NOT_INITIALIZED:   return "EGL_NOT_INITIALIZED";
        case EGL_BAD_ACCESS:        return "EGL_BAD_ACCESS";
        case EGL_BAD_ALLOC:         return "EGL_BAD_ALLOC";
        case EGL_BAD_ATTRIBUTE:     return "EGL_BAD_ATTRIBUTE";
        case EGL_BAD_CONFIG:        return "EGL_BAD_CONFIG";
        case EGL_BAD_CONTEXT:       return "EGL_BAD_CONTEXT";
        case EGL_BAD_CURRENT_SURFACE: return "EGL_BAD_CURRENT_SURFACE";
        case EGL_BAD_DISPLAY:       return "EGL_BAD_DISPLAY";
        case EGL_BAD_MATCH:         return "EGL_BAD_MATCH";
        case EGL_BAD_NATIVE_PIXMAP: return "EGL_BAD_NATIVE_PIXMAP";
        case EGL_BAD_NATIVE_WINDOW: return "EGL_BAD_NATIVE_WINDOW";
        case EGL_BAD_PARAMETER:     return "EGL_BAD_PARAMETER";
        case EGL_BAD_SURFACE:       return "EGL_BAD_SURFACE";
        case EGL_CONTEXT_LOST:      return "EGL_CONTEXT_LOST";
        default: return "UNKNOWN";
    }
}

status_t EGLUtils::selectConfigForPixelFormat(
        EGLDisplay dpy,
        EGLint const* attrs,
        int32_t format,
        EGLConfig* outConfig)
{
    EGLint numConfigs = -1, n=0;

    if (!attrs)
        return BAD_VALUE;

    if (outConfig == nullptr)
        return BAD_VALUE;

    // Get all the "potential match" configs...
    if (eglGetConfigs(dpy, nullptr, 0, &numConfigs) == EGL_FALSE)
        return BAD_VALUE;

    std::vector<EGLConfig> configs(numConfigs);
    if (eglChooseConfig(dpy, attrs, configs.data(), numConfigs, &n) == EGL_FALSE) {
        return BAD_VALUE;
    }

    int i;
    EGLConfig config = nullptr;
    for (i=0 ; i<n ; i++) {
        EGLint nativeVisualId = 0;
        eglGetConfigAttrib(dpy, configs[i], EGL_NATIVE_VISUAL_ID, &nativeVisualId);
        if (nativeVisualId>0 && format == nativeVisualId) {
            config = configs[i];
            break;
        }
    }

    if (i<n) {
        *outConfig = config;
        return NO_ERROR;
    }

    return NAME_NOT_FOUND;
}

status_t EGLUtils::selectConfigForNativeWindow(
        EGLDisplay dpy,
        EGLint const* attrs,
        EGLNativeWindowType window,
        EGLConfig* outConfig)
{
    int err;
    int format;

    if (!window)
        return BAD_VALUE;

    if ((err = window->query(window, NATIVE_WINDOW_FORMAT, &format)) < 0) {
        return err;
    }

    return selectConfigForPixelFormat(dpy, attrs, format, outConfig);
}

String8 EGLUtils::printGLString(const char* name, GLenum s) {
    String8 msg;
    const char* v = reinterpret_cast<const char*>(glGetString(s));
    msg.appendFormat("GL %s = %s\n", name, v);
    return msg;
}

String8 EGLUtils::printEGLString(EGLDisplay dpy, const char* name, GLenum s) {
    String8 msg;
    const char* v = static_cast<const char*>(eglQueryString(dpy, s));
    msg.appendFormat("GL %s = %s\n", name, v);
    const char* va = (const char*)eglQueryStringImplementationANDROID(dpy, s);
    msg.appendFormat("ImplementationANDROID: %s = %s\n", name, va);
    return msg;
}

String8 EGLUtils::checkEglError(const char* op, EGLBoolean returnVal = EGL_TRUE) {
    String8 msg;
    if (returnVal != EGL_TRUE) {
        msg.appendFormat("%s() returned %d\n", op, returnVal);
    }

    for (EGLint error = eglGetError(); error != EGL_SUCCESS; error = eglGetError()) {
        msg.appendFormat("after %s() eglError %s (0x%x)\n", op, EGLUtils::strerror(error), error);
    }
    return msg;
}

String8 EGLUtils::checkGlError(const char* op) {
    String8 msg;
    for (GLint error = glGetError(); error != GL_NO_ERROR; error = glGetError()) {
        msg.appendFormat("after %s() glError (0x%x)\n", op, error);
    }
    return msg;
}

String8 EGLUtils::printEGLConfiguration(EGLDisplay dpy, EGLConfig config) {
#define X(VAL) \
    { VAL, #VAL }
    struct {
        EGLint attribute;
        const char* name;
    } names[] = {
            X(EGL_BUFFER_SIZE),
            X(EGL_ALPHA_SIZE),
            X(EGL_BLUE_SIZE),
            X(EGL_GREEN_SIZE),
            X(EGL_RED_SIZE),
            X(EGL_DEPTH_SIZE),
            X(EGL_STENCIL_SIZE),
            X(EGL_CONFIG_CAVEAT),
            X(EGL_CONFIG_ID),
            X(EGL_LEVEL),
            X(EGL_MAX_PBUFFER_HEIGHT),
            X(EGL_MAX_PBUFFER_PIXELS),
            X(EGL_MAX_PBUFFER_WIDTH),
            X(EGL_NATIVE_RENDERABLE),
            X(EGL_NATIVE_VISUAL_ID),
            X(EGL_NATIVE_VISUAL_TYPE),
            X(EGL_SAMPLES),
            X(EGL_SAMPLE_BUFFERS),
            X(EGL_SURFACE_TYPE),
            X(EGL_TRANSPARENT_TYPE),
            X(EGL_TRANSPARENT_RED_VALUE),
            X(EGL_TRANSPARENT_GREEN_VALUE),
            X(EGL_TRANSPARENT_BLUE_VALUE),
            X(EGL_BIND_TO_TEXTURE_RGB),
            X(EGL_BIND_TO_TEXTURE_RGBA),
            X(EGL_MIN_SWAP_INTERVAL),
            X(EGL_MAX_SWAP_INTERVAL),
            X(EGL_LUMINANCE_SIZE),
            X(EGL_ALPHA_MASK_SIZE),
            X(EGL_COLOR_BUFFER_TYPE),
            X(EGL_RENDERABLE_TYPE),
            X(EGL_CONFORMANT),
    };
#undef X

    String8 msg;
    for (size_t j = 0; j < sizeof(names) / sizeof(names[0]); j++) {
        EGLint value = -1;
        EGLint returnVal = eglGetConfigAttrib(dpy, config, names[j].attribute, &value);
        EGLint error = eglGetError();
        if (returnVal && error == EGL_SUCCESS) {
            msg.appendFormat(" %s: %d (0x%x)", names[j].name, value, value);
        }
    }
    msg.append("\n");
    return msg;
}

bool EGLUtils::printEGLConfigurations(EGLDisplay dpy, String8& msg) {
    EGLint numConfig = 0;
    EGLint returnVal = eglGetConfigs(dpy, nullptr, 0, &numConfig);
    msg.append(checkEglError("eglGetConfigs", returnVal));
    if (!returnVal) {
        return false;
    }

    msg.appendFormat("Number of EGL configuration: %d\n", numConfig);

    std::vector<EGLConfig> configs(numConfig);

    returnVal = eglGetConfigs(dpy, configs.data(), numConfig, &numConfig);
    msg.append(checkEglError("eglGetConfigs", returnVal));
    if (!returnVal) {
        return false;
    }

    for (int i = 0; i < numConfig; i++) {
        msg.appendFormat("Configuration %d\n", i);
        msg.append(printEGLConfiguration(dpy, configs[i]));
    }

    return true;
}

bool EGLUtils::printEGLConfigurations(FILE* output, EGLDisplay dpy) {
    String8 msg;
    bool status = printEGLConfigurations(dpy, msg);
    fprintf(output, "%s", msg.c_str());
    return status;
}

String8 EGLUtils::decodeColorSpace(EGLint colorSpace) {
    switch (colorSpace) {
        case EGL_GL_COLORSPACE_SRGB_KHR:
            return String8("EGL_GL_COLORSPACE_SRGB_KHR");
        case EGL_GL_COLORSPACE_DISPLAY_P3_EXT:
            return String8("EGL_GL_COLORSPACE_DISPLAY_P3_EXT");
        case EGL_GL_COLORSPACE_DISPLAY_P3_PASSTHROUGH_EXT:
            return String8("EGL_GL_COLORSPACE_DISPLAY_P3_PASSTHROUGH_EXT");
        case  EGL_GL_COLORSPACE_LINEAR_KHR:
            return String8("EGL_GL_COLORSPACE_LINEAR_KHR");
        default:
            return String8::format("UNKNOWN ColorSpace %d", colorSpace);
    }
}

bool EGLUtils::hasExtension(const char* exts, const char* name) {
    size_t nameLen = strlen(name);
    if (exts) {
        for (const char* match = strstr(exts, name); match; match = strstr(match + nameLen, name)) {
            if (match[nameLen] == '\0' || match[nameLen] == ' ') {
                return true;
            }
        }
    }
    return false;
}

bool EGLUtils::hasEglExtension(EGLDisplay dpy, const char* name) {
    return hasExtension(eglQueryString(dpy, EGL_EXTENSIONS), name);
}

// ----------------------------------------------------------------------------
}; // namespace android
// ----------------------------------------------------------------------------

#endif /* ANDROID_UI_EGLUTILS_H */
