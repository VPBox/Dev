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

#ifndef NETD_SERVER_IPTABLES_RESTORE_CONTROLLER_H
#define NETD_SERVER_IPTABLES_RESTORE_CONTROLLER_H

#include <memory>
#include <mutex>
#include <sys/types.h>

#include "NetdConstants.h"

class IptablesProcess;

class IptablesRestoreInterface {
  public:
    virtual ~IptablesRestoreInterface() = default;

    // Execute |commands| on the given |target|, and populate |output| with stdout.
    virtual int execute(const IptablesTarget target, const std::string& commands,
                        std::string* output) = 0;
};

class IptablesRestoreController final : public IptablesRestoreInterface {
  public:
    // Not for general use. Use gCtls->iptablesRestoreCtrl
    // to get an instance of this class.
    IptablesRestoreController();

    ~IptablesRestoreController() override;

    int execute(const IptablesTarget target, const std::string& commands,
                std::string* output) override;

    enum IptablesProcessType {
        IPTABLES_PROCESS,
        IP6TABLES_PROCESS,
        INVALID_PROCESS = -1,
    };

    // Called by the SIGCHLD signal handler when it detects that one
    // of the forked iptables[6]-restore process has died.
    IptablesProcessType notifyChildTermination(pid_t pid);

protected:
    friend class IptablesRestoreControllerTest;
    pid_t getIpRestorePid(const IptablesProcessType type);

    // The maximum number of times we poll(2) for a response on our set of polled
    // fds. Chosen so that the overall timeout is 5s. The timeout is so high because
    // our version of iptables still polls every second in xtables_lock.
    static int MAX_RETRIES;

    // The timeout (in millis) for each call to poll. The maximum wait is
    // |POLL_TIMEOUT_MS * MAX_RETRIES|. Chosen so that the overall timeout is 1s.
    static int POLL_TIMEOUT_MS;

    void Init();

private:
    static IptablesProcess* forkAndExec(const IptablesProcessType type);

    int sendCommand(const IptablesProcessType type, const std::string& command,
                    std::string *output);

    static bool drainAndWaitForAck(const std::unique_ptr<IptablesProcess> &process,
                                   const std::string& command,
                                   std::string *output);

    static void maybeLogStderr(const std::unique_ptr<IptablesProcess> &process,
                               const std::string& command);

    // Guards calls to execute().
    std::mutex mLock;

    std::unique_ptr<IptablesProcess> mIpRestore;
    std::unique_ptr<IptablesProcess> mIp6Restore;
};

#endif  // NETD_SERVER_IPTABLES_RESTORE_CONTROLLER_H
