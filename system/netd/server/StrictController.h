/*
 * Copyright (C) 2014 The Android Open Source Project
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

#ifndef _STRICT_CONTROLLER_H
#define _STRICT_CONTROLLER_H

#include <string>

#include "NetdConstants.h"

enum StrictPenalty { INVALID, ACCEPT, LOG, REJECT };

/*
 * Help apps catch unwanted low-level networking behavior, like
 * connections not wrapped in TLS.
 */
class StrictController {
public:
    StrictController();

    int setupIptablesHooks(void);
    int resetChains(void);

    int setUidCleartextPenalty(uid_t, StrictPenalty);

    static const char* LOCAL_OUTPUT;
    static const char* LOCAL_CLEAR_DETECT;
    static const char* LOCAL_CLEAR_CAUGHT;
    static const char* LOCAL_PENALTY_LOG;
    static const char* LOCAL_PENALTY_REJECT;
    std::mutex lock;

  protected:
    // For testing.
    friend class StrictControllerTest;
    static int (*execIptablesRestore)(IptablesTarget target, const std::string& commands);
};

#endif
