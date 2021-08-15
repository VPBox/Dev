/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include <string>

#include <selinux/android.h>
#include <selinux/avc.h>

namespace android {

class AccessControl {
public:
    AccessControl();

    struct CallingContext {
        bool sidPresent;
        std::string sid;
        pid_t pid;
    };
    static CallingContext getCallingContext(pid_t sourcePid);

    bool canAdd(const std::string& fqName, const CallingContext& callingContext);
    bool canGet(const std::string& fqName, const CallingContext& callingContext);
    bool canList(const CallingContext& callingContext);

private:

    bool checkPermission(const CallingContext& source, const char *targetContext, const char *perm, const char *interface);
    bool checkPermission(const CallingContext& source, const char *perm, const char *interface);

    static int auditCallback(void *data, security_class_t cls, char *buf, size_t len);

    char*                  mSeContext;
    struct selabel_handle* mSeHandle;
    union selinux_callback mSeCallbacks;
};

} // namespace android
