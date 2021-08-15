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

#ifndef ANDROID_AUDIO_LIMITER_H
#define ANDROID_AUDIO_LIMITER_H

#include <sys/cdefs.h>

/** \cond */
__BEGIN_DECLS
/** \endcond */

#ifdef __cplusplus
extern "C" {
#endif
    /**
     * This limiter function is the identity for [-sqrt(0.5), sqrt(0.5)]
     * and then approximates atan or spline.
     * \param in input in range [-sqrt(2), sqrt(2)]; out of range, inf, and NaN are not permitted
     * \return the limited output in range no larger than [-1.0, 1.0].
     * The implementation is permitted to limit the output to a slightly smaller range,
     * so the minimum and maximum outputs may not be achievable.
     */
    extern float limiter(float in);
#ifdef __cplusplus
}
#endif

/** \cond */
__END_DECLS
/** \endcond */

#endif /* !ANDROID_AUDIO_LIMITER_H */
