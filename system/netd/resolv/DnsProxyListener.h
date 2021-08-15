/*
 * Copyright (C) 2010 The Android Open Source Project
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

#pragma once

#include <string>

#include <netd_resolv/resolv.h>  // android_net_context
#include <sysutils/FrameworkCommand.h>
#include <sysutils/FrameworkListener.h>

struct addrinfo;
struct hostent;

namespace android {
namespace net {

class NetworkDnsEventReported;

class DnsProxyListener : public FrameworkListener {
  public:
    DnsProxyListener();
    virtual ~DnsProxyListener() {}

    static constexpr const char* SOCKET_NAME = "dnsproxyd";

  private:
    class GetAddrInfoCmd : public FrameworkCommand {
      public:
        GetAddrInfoCmd();
        virtual ~GetAddrInfoCmd() {}
        int runCommand(SocketClient* c, int argc, char** argv) override;
    };

    /* ------ getaddrinfo ------*/
    class GetAddrInfoHandler {
      public:
        // Note: All of host, service, and hints may be NULL
        GetAddrInfoHandler(SocketClient* c, char* host, char* service, addrinfo* hints,
                           const android_net_context& netcontext);
        ~GetAddrInfoHandler();

        void run();

      private:
        void doDns64Synthesis(int32_t* rv, addrinfo** res, NetworkDnsEventReported* event);

        SocketClient* mClient;  // ref counted
        char* mHost;            // owned. TODO: convert to std::string.
        char* mService;         // owned. TODO: convert to std::string.
        addrinfo* mHints;       // owned
        android_net_context mNetContext;
    };

    /* ------ gethostbyname ------*/
    class GetHostByNameCmd : public FrameworkCommand {
      public:
        GetHostByNameCmd();
        virtual ~GetHostByNameCmd() {}
        int runCommand(SocketClient* c, int argc, char** argv) override;
    };

    class GetHostByNameHandler {
      public:
        GetHostByNameHandler(SocketClient* c, char* name, int af,
                             const android_net_context& netcontext);
        ~GetHostByNameHandler();

        void run();

      private:
        void doDns64Synthesis(int32_t* rv, hostent** hpp, NetworkDnsEventReported* event);

        SocketClient* mClient;  // ref counted
        char* mName;            // owned. TODO: convert to std::string.
        int mAf;
        android_net_context mNetContext;
    };

    /* ------ gethostbyaddr ------*/
    class GetHostByAddrCmd : public FrameworkCommand {
      public:
        GetHostByAddrCmd();
        virtual ~GetHostByAddrCmd() {}
        int runCommand(SocketClient* c, int argc, char** argv) override;
    };

    class GetHostByAddrHandler {
      public:
        GetHostByAddrHandler(SocketClient* c, void* address, int addressLen, int addressFamily,
                             const android_net_context& netcontext);
        ~GetHostByAddrHandler();

        void run();

      private:
        void doDns64ReverseLookup(hostent** hpp, NetworkDnsEventReported* event);

        SocketClient* mClient;  // ref counted
        void* mAddress;         // address to lookup; owned
        int mAddressLen;        // length of address to look up
        int mAddressFamily;     // address family
        android_net_context mNetContext;
    };

    /* ------ resnsend ------*/
    class ResNSendCommand : public FrameworkCommand {
      public:
        ResNSendCommand();
        virtual ~ResNSendCommand() {}
        int runCommand(SocketClient* c, int argc, char** argv) override;
    };

    class ResNSendHandler {
      public:
        ResNSendHandler(SocketClient* c, std::string msg, uint32_t flags,
                        const android_net_context& netcontext);
        ~ResNSendHandler();

        void run();

      private:
        SocketClient* mClient;  // ref counted
        std::string mMsg;
        uint32_t mFlags;
        android_net_context mNetContext;
    };

    /* ------ getdnsnetid ------*/
    class GetDnsNetIdCommand : public FrameworkCommand {
      public:
        GetDnsNetIdCommand();
        virtual ~GetDnsNetIdCommand() {}
        int runCommand(SocketClient* c, int argc, char** argv) override;
    };
};

}  // namespace net
}  // namespace android
