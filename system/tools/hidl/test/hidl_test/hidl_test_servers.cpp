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

#define LOG_TAG "hidl_test_servers"

#include "hidl_test.h"

#include <android-base/logging.h>

#include <android/hardware/tests/baz/1.0/IBaz.h>

#include <hidl/LegacySupport.h>

#include <hwbinder/IPCThreadState.h>

#include <sys/wait.h>
#include <signal.h>

#include <string>
#include <utility>
#include <vector>

using ::android::hardware::tests::baz::V1_0::IBaz;

using ::android::hardware::defaultPassthroughServiceImplementation;
using ::android::hardware::IPCThreadState;

static std::vector<std::pair<std::string, pid_t>> gPidList;

void signal_handler_server(int signal) {
    if (signal == SIGTERM) {
        IPCThreadState::shutdown();
        exit(0);
    }
}

template <class T>
struct ForkServer {
    static void run(const std::string& serviceName) {
        pid_t pid;

        if ((pid = fork()) == 0) {
            // in child process
            signal(SIGTERM, signal_handler_server);
            int status = defaultPassthroughServiceImplementation<T>(serviceName);
            exit(status);
        }

        gPidList.push_back({serviceName, pid});
    }
};

static void killServer(pid_t pid, const char *serverName) {
    if (kill(pid, SIGTERM)) {
        ALOGE("Could not kill %s; errno = %d", serverName, errno);
    } else {
        int status;
        ALOGE("Waiting for %s to exit...", serverName);
        waitpid(pid, &status, 0);
        if (status != 0) {
            ALOGE("%s terminates abnormally with status %d", serverName, status);
        } else {
            ALOGE("%s killed successfully", serverName);
        }
        ALOGE("Continuing...");
    }
}

void signal_handler(int signal) {
    if (signal == SIGTERM) {
        for (const auto& p : gPidList) {
            killServer(p.second, p.first.c_str());
        }
        exit(0);
    }
}

int main(int /* argc */, char* /* argv */ []) {
    setenv("TREBLE_TESTING_OVERRIDE", "true", true);

    runOnEachServer<ForkServer>();

    ForkServer<IBaz>::run("dyingBaz");

    signal(SIGTERM, signal_handler);
    // Parent process should not exit before the forked child processes.
    pause();

    return 0;
}
