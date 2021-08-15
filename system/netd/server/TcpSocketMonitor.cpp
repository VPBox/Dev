/*
 * Copyright (C) 2018 The Android Open Source Project
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

#define LOG_TAG "TcpSocketMonitor"

#include <chrono>
#include <cinttypes>
#include <thread>
#include <vector>

#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <linux/tcp.h>

#include "Controllers.h"
#include "SockDiag.h"
#include "TcpSocketMonitor.h"
#include "netdutils/DumpWriter.h"

using android::netdutils::DumpWriter;
using android::netdutils::ScopedIndent;

namespace android {
namespace net {

using std::chrono::duration_cast;
using std::chrono::steady_clock;

constexpr const char* getTcpStateName(int t) {
    switch (t) {
        case TCP_ESTABLISHED:
            return "ESTABLISHED";
        case TCP_SYN_SENT:
            return "SYN-SENT";
        case TCP_SYN_RECV:
            return "SYN-RECV";
        case TCP_FIN_WAIT1:
            return "FIN-WAIT-1";
        case TCP_FIN_WAIT2:
            return "FIN-WAIT-2";
        case TCP_TIME_WAIT:
            return "TIME-WAIT";
        case TCP_CLOSE:
            return "CLOSE";
        case TCP_CLOSE_WAIT:
            return "CLOSE-WAIT";
        case TCP_LAST_ACK:
            return "LAST-ACK";
        case TCP_LISTEN:
            return "LISTEN";
        case TCP_CLOSING:
            return "CLOSING";
        default:
            return "UNKNOWN";
    }
}

// Helper macro for reading fields into struct tcp_info and handling different struct tcp_info
// versions in the kernel.
#define TCPINFO_GET(ptr, fld, len, zero) \
        (((ptr) != nullptr && (offsetof(struct tcp_info, fld) + sizeof((ptr)->fld)) < len) ? \
        (ptr)->fld : zero)

static void tcpInfoPrint(DumpWriter &dw, Fwmark mark, const struct inet_diag_msg *sockinfo,
                         const struct tcp_info *tcpinfo, uint32_t tcpinfoLen) {
    char saddr[INET6_ADDRSTRLEN] = {};
    char daddr[INET6_ADDRSTRLEN] = {};
    inet_ntop(sockinfo->idiag_family, &(sockinfo->id.idiag_src), saddr, sizeof(saddr));
    inet_ntop(sockinfo->idiag_family, &(sockinfo->id.idiag_dst), daddr, sizeof(daddr));

    dw.println(
            "netId=%d uid=%u mark=0x%x saddr=%s daddr=%s sport=%u dport=%u tcp_state=%s(%u) "
            "rtt=%gms sent=%u lost=%u",
            mark.netId,
            sockinfo->idiag_uid,
            mark.intValue,
            saddr,
            daddr,
            ntohs(sockinfo->id.idiag_sport),
            ntohs(sockinfo->id.idiag_dport),
            getTcpStateName(sockinfo->idiag_state), sockinfo->idiag_state,
            TCPINFO_GET(tcpinfo, tcpi_rtt, tcpinfoLen, 0) / 1000.0,
            TCPINFO_GET(tcpinfo, tcpi_segs_out, tcpinfoLen, 0),
            TCPINFO_GET(tcpinfo, tcpi_lost, tcpinfoLen, 0));
}

const String16 TcpSocketMonitor::DUMP_KEYWORD = String16("tcp_socket_info");
const milliseconds TcpSocketMonitor::kDefaultPollingInterval = milliseconds(30000);

void TcpSocketMonitor::dump(DumpWriter& dw) {
    std::lock_guard guard(mLock);

    dw.println("TcpSocketMonitor");
    ScopedIndent tcpSocketMonitorDetails(dw);

    const auto now = steady_clock::now();
    const auto d = duration_cast<milliseconds>(now - mLastPoll);
    dw.println("running=%d, suspended=%d, last poll %lld ms ago",
            mIsRunning, mIsSuspended, d.count());

    if (!mNetworkStats.empty()) {
        dw.blankline();
        dw.println("Network stats:");
        for (const std::pair<const uint32_t, TcpStats>& stats : mNetworkStats) {
            if (stats.second.nSockets == 0) {
                continue;
            }
            dw.println("netId=%d sent=%d lost=%d rttMs=%gms sentAckDiff=%dms",
                       stats.first,
                       stats.second.sent,
                       stats.second.lost,
                       stats.second.rttUs / 1000.0 / stats.second.nSockets,
                       stats.second.sentAckDiffMs / stats.second.nSockets);
        }
    }

    if (!mSocketEntries.empty()) {
        dw.blankline();
        dw.println("Socket entries:");
        for (const std::pair<const uint64_t, SocketEntry>& stats : mSocketEntries) {
            dw.println("netId=%u uid=%u cookie=%" PRIu64, stats.second.mark.netId, stats.second.uid,
                       stats.first);
        }
    }

    SockDiag sd;
    if (sd.open()) {
        dw.blankline();
        dw.println("Current socket dump:");
        const auto tcpInfoReader = [&dw](Fwmark mark, const struct inet_diag_msg *sockinfo,
                                         const struct tcp_info *tcpinfo, uint32_t tcpinfoLen) {
            tcpInfoPrint(dw, mark, sockinfo, tcpinfo, tcpinfoLen);
        };

        if (int ret = sd.getLiveTcpInfos(tcpInfoReader)) {
            ALOGE("Failed to dump TCP socket info: %s", strerror(-ret));
        }
    } else {
        ALOGE("Error opening sock diag for dumping TCP socket info");
    }
}

void TcpSocketMonitor::setPollingInterval(milliseconds nextSleepDurationMs) {
    std::lock_guard guard(mLock);

    mNextSleepDurationMs = nextSleepDurationMs;

    ALOGD("tcpinfo polling interval set to %lld ms", mNextSleepDurationMs.count());
}

void TcpSocketMonitor::resumePolling() {
    bool wasSuspended;
    {
        std::lock_guard guard(mLock);

        wasSuspended = mIsSuspended;
        mIsSuspended = false;
        ALOGD("resuming tcpinfo polling (interval=%lldms)", mNextSleepDurationMs.count());
    }

    if (wasSuspended) {
        mCv.notify_all();
    }
}

void TcpSocketMonitor::suspendPolling() {
    std::lock_guard guard(mLock);

    bool wasSuspended = mIsSuspended;
    mIsSuspended = true;
    ALOGD("suspending tcpinfo polling");

    if (!wasSuspended) {
        mSocketEntries.clear();
    }
}

void TcpSocketMonitor::poll() {
    std::lock_guard guard(mLock);

    if (mIsSuspended) {
        return;
    }

    SockDiag sd;
    if (!sd.open()) {
        ALOGE("Error opening sock diag for polling TCP socket info");
        return;
    }

    const auto now = steady_clock::now();
    const auto tcpInfoReader = [this, now](Fwmark mark, const struct inet_diag_msg *sockinfo,
                                           const struct tcp_info *tcpinfo,
                                           uint32_t tcpinfoLen) NO_THREAD_SAFETY_ANALYSIS {
        if (sockinfo == nullptr || tcpinfo == nullptr || tcpinfoLen == 0 || mark.intValue == 0) {
            return;
        }
        updateSocketStats(now, mark, sockinfo, tcpinfo, tcpinfoLen);
    };

    // Reset mNetworkStats
    mNetworkStats.clear();

    if (int ret = sd.getLiveTcpInfos(tcpInfoReader)) {
        ALOGE("Failed to poll TCP socket info: %s", strerror(-ret));
        return;
    }

    // Remove any SocketEntry not updated
    for (auto it = mSocketEntries.cbegin(); it != mSocketEntries.cend();) {
        if (it->second.lastUpdate < now) {
            it = mSocketEntries.erase(it);
        } else {
            it++;
        }
    }

    const auto listener = gCtls->eventReporter.getNetdEventListener();
    if (listener != nullptr) {
        std::vector<int> netIds;
        std::vector<int> sentPackets;
        std::vector<int> lostPackets;
        std::vector<int> rtts;
        std::vector<int> sentAckDiffs;
        for (auto const& stats : mNetworkStats) {
            int32_t nSockets = stats.second.nSockets;
            if (nSockets == 0) {
                continue;
            }
            netIds.push_back(stats.first);
            sentPackets.push_back(stats.second.sent);
            lostPackets.push_back(stats.second.lost);
            rtts.push_back(stats.second.rttUs / nSockets);
            sentAckDiffs.push_back(stats.second.sentAckDiffMs / nSockets);
        }
        listener->onTcpSocketStatsEvent(netIds, sentPackets, lostPackets, rtts, sentAckDiffs);
    }

    mLastPoll = now;
}

void TcpSocketMonitor::waitForNextPoll() {
    bool isSuspended;
    milliseconds nextSleepDurationMs;
    {
        std::lock_guard guard(mLock);
        isSuspended = mIsSuspended;
        nextSleepDurationMs= mNextSleepDurationMs;
    }

    std::unique_lock<std::mutex> ul(mLock);
    if (isSuspended) {
        mCv.wait(ul);
    } else {
        mCv.wait_for(ul, nextSleepDurationMs);
    }
}

bool TcpSocketMonitor::isRunning() {
    std::lock_guard guard(mLock);
    return mIsRunning;
}

void TcpSocketMonitor::updateSocketStats(time_point now, Fwmark mark,
                                         const struct inet_diag_msg *sockinfo,
                                         const struct tcp_info *tcpinfo,
                                         uint32_t tcpinfoLen) NO_THREAD_SAFETY_ANALYSIS {
    int32_t lastAck = TCPINFO_GET(tcpinfo, tcpi_last_ack_recv, tcpinfoLen, 0);
    int32_t lastSent = TCPINFO_GET(tcpinfo, tcpi_last_data_sent, tcpinfoLen, 0);
    TcpStats diff = {
        .sent = TCPINFO_GET(tcpinfo, tcpi_segs_out, tcpinfoLen, 0),
        .lost = TCPINFO_GET(tcpinfo, tcpi_lost, tcpinfoLen, 0),
        .rttUs = TCPINFO_GET(tcpinfo, tcpi_rtt, tcpinfoLen, 0),
        .sentAckDiffMs = lastAck - lastSent,
        .nSockets = 1,
    };

    {
        // Update socket stats with the newest entry, computing the diff w.r.t the previous entry.
        const uint64_t cookie = (static_cast<uint64_t>(sockinfo->id.idiag_cookie[0]) << 32)
                | static_cast<uint64_t>(sockinfo->id.idiag_cookie[1]);
        const SocketEntry previous = mSocketEntries[cookie];
        mSocketEntries[cookie] = {
            .sent = diff.sent,
            .lost = diff.lost,
            .lastUpdate = now,
            .mark = mark,
            .uid = sockinfo->idiag_uid,
        };

        diff.sent -= previous.sent;
        diff.lost -= previous.lost;
    }

    {
        // Aggregate the diff per network id.
        auto& stats = mNetworkStats[mark.netId];
        stats.sent += diff.sent;
        stats.lost += diff.lost;
        stats.rttUs += diff.rttUs;
        stats.sentAckDiffMs += diff.sentAckDiffMs;
        stats.nSockets += diff.nSockets;
    }
}

TcpSocketMonitor::TcpSocketMonitor() {
    std::lock_guard guard(mLock);

    mNextSleepDurationMs = kDefaultPollingInterval;
    mIsRunning = true;
    mIsSuspended = true;
    mPollingThread = std::thread([this] {
        (void) this;
        while (isRunning()) {
            poll();
            waitForNextPoll();
        }
    });
}

TcpSocketMonitor::~TcpSocketMonitor() {
    {
        std::lock_guard guard(mLock);
        mIsRunning = false;
        mIsSuspended = true;
    }
    mCv.notify_all();
    mPollingThread.join();
}

}  // namespace net
}  // namespace android
