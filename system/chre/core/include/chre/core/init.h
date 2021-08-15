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

#ifndef CHRE_CORE_INIT_H_
#define CHRE_CORE_INIT_H_

namespace chre {

/**
 * Performs initialization of CHRE. The CHRE is designed to be portable and as
 * such the order of initialization must be as follows:
 *
 * 1) [optional] Perform platform initialization if needed.
 * 2) [required] Perform CHRE initialization by invoking this function. All
 *               platforms must invoke chre::init() prior to loading apps.
 *
 * When step 2) is performed, the platform must be ready for any and all
 * platform-specific code to be invoked as part of CHRE initialization. (as an
 * example for the Linux platform, any functions provided by the platform/linux
 * directory must be available). If any platform specific initialization is
 * required, it must be performed in step 1).
 */
void init();

/**
 * Performs deinitialization of CHRE. The order of deinitialization must be as
 * follows:
 *
 * 1) [required] Perform CHRE deinitialization by invoking this function. All
 *               platforms must invoke chre::deinit() after unloading all apps
 *               and upon termination.
 * 2) [optional] Perform any additional teardown as required by the platform but
 *               not covered by common deinitialization logic.
 *
 * After step 1 is complete, chre common code has been deinitialized and calling
 * into it is undefined behavior. Portions of the platform (ie: sensors) are
 * also deinitialized by step 1. Only perform additional deinitialization after
 * this function as not handled by common CHRE code. The goal is to minimize
 * this additional teardown as much as possible.
 */
void deinit();

}  // namespace chre

#endif  // CHRE_CORE_INIT_H_
