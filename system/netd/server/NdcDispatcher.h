/*
 * Copyright (C) 2019 The Android Open Source Project
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

#ifndef _NDC_DISPATCHER_H__
#define _NDC_DISPATCHER_H__

#include <string>

#include <android-base/logging.h>
#include <android/net/IDnsResolver.h>
#include <android/net/INetd.h>
#include "binder/IServiceManager.h"

#include "NetdConstants.h"

namespace android {
namespace net {

class NdcClient {
  public:
    NdcClient() = default;
    ~NdcClient() = default;

    int sendMsg(int code, const char* msg, bool addErrno) {
        if (addErrno) {
            printf("%d 0 %s (%s)\n", code, msg, strerror(errno));
        } else {
            printf("%d 0 %s\n", code, msg);
        }
        return 0;
    }
};

class NdcNetdCommand {
  public:
    NdcNetdCommand(std::string cmd) : mCommand(std::move(cmd)) {}
    virtual ~NdcNetdCommand() {}

    virtual int runCommand(NdcClient* c, int argc, char** argv) const = 0;

    const std::string& getCommand() const { return mCommand; }

  private:
    std::string mCommand;
};

class NdcDispatcher {
  public:
    // Matches the restrictions previously imposed by CommandListener.cpp.
    static const int CMD_ARGS_MAX = 26;
    // Default log level is set to minimum one.
    static const android::base::LogSeverity LOGLEVEL = android::base::VERBOSE;

    NdcDispatcher();
    ~NdcDispatcher() = default;

    static sp<INetd> mNetd;
    static sp<IDnsResolver> mDnsResolver;
    NdcClient mNdc;

    int dispatchCommand(int argc, char** argv);
    void registerCmd(NdcNetdCommand* cmd);

  private:
    std::vector<NdcNetdCommand*> mCommands;

    class InterfaceCmd : public NdcNetdCommand {
      public:
        InterfaceCmd();
        virtual ~InterfaceCmd() {}
        int runCommand(NdcClient* cli, int argc, char** argv) const;
    };

    class IpFwdCmd : public NdcNetdCommand {
      public:
        IpFwdCmd();
        virtual ~IpFwdCmd() {}
        int runCommand(NdcClient* cli, int argc, char** argv) const;
    };

    class TetherCmd : public NdcNetdCommand {
      public:
        TetherCmd();
        virtual ~TetherCmd() {}
        int runCommand(NdcClient* cli, int argc, char** argv) const;
    };

    class NatCmd : public NdcNetdCommand {
      public:
        NatCmd();
        virtual ~NatCmd() {}
        int runCommand(NdcClient* cli, int argc, char** argv) const;
    };

    class BandwidthControlCmd : public NdcNetdCommand {
      public:
        BandwidthControlCmd();
        virtual ~BandwidthControlCmd() {}
        int runCommand(NdcClient* cli, int argc, char** argv) const;

      protected:
        void sendGenericOkFail(NdcClient* cli, int cond) const;
        void sendGenericOpFailed(NdcClient* cli, const char* errMsg) const;
        void sendGenericSyntaxError(NdcClient* cli, const char* usageMsg) const;
    };

    class IdletimerControlCmd : public NdcNetdCommand {
      public:
        IdletimerControlCmd();
        virtual ~IdletimerControlCmd() {}
        int runCommand(NdcClient* cli, int argc, char** argv) const;
    };

    class FirewallCmd : public NdcNetdCommand {
      public:
        FirewallCmd();
        virtual ~FirewallCmd() {}
        int runCommand(NdcClient* cli, int argc, char** argv) const;

      protected:
        int sendGenericOkFail(NdcClient* cli, int cond) const;
        static int parseRule(const char* arg);
        static int parseFirewallType(const char* arg);
        static int parseChildChain(const char* arg);
    };

    class ClatdCmd : public NdcNetdCommand {
      public:
        ClatdCmd();
        virtual ~ClatdCmd() {}
        int runCommand(NdcClient* cli, int argc, char** argv) const;
    };

    class StrictCmd : public NdcNetdCommand {
      public:
        StrictCmd();
        virtual ~StrictCmd() {}
        int runCommand(NdcClient* cli, int argc, char** argv) const;

      protected:
        int sendGenericOkFail(NdcClient* cli, int cond) const;
        static int parsePenalty(const char* arg);
    };

    class NetworkCommand : public NdcNetdCommand {
      public:
        NetworkCommand();
        virtual ~NetworkCommand() {}
        int runCommand(NdcClient* cli, int argc, char** argv) const;

      private:
        int syntaxError(NdcClient* cli, const char* message) const;
        int operationError(NdcClient* cli, const char* message, int ret) const;
        int success(NdcClient* cli) const;
    };
};

}  // namespace net
}  // namespace android

#endif
