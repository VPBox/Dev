/**
 * Copyright (c) 2016, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package android.net.metrics;

/**
 * Logs netd events.
 *
 * {@hide}
 */
oneway interface INetdEventListener {
    const int EVENT_GETADDRINFO = 1;
    const int EVENT_GETHOSTBYNAME = 2;
    const int EVENT_GETHOSTBYADDR = 3;
    const int EVENT_RES_NSEND = 4;

    const int REPORTING_LEVEL_NONE = 0;
    const int REPORTING_LEVEL_METRICS = 1;
    const int REPORTING_LEVEL_FULL = 2;

    // Maximum number of IP addresses logged for DNS lookups before we truncate the full list.
    const int DNS_REPORTED_IP_ADDRESSES_LIMIT = 10;

    /**
     * Logs a DNS lookup function call (getaddrinfo and gethostbyname).
     *
     * @param netId the ID of the network the lookup was performed on.
     * @param eventType one of the EVENT_* constants in this interface.
     * @param returnCode the return value of the function call.
     * @param latencyMs the latency of the function call.
     * @param hostname the name that was looked up.
     * @param ipAddresses (possibly a subset of) the IP addresses returned.
     *        At most {@link #DNS_REPORTED_IP_ADDRESSES_LIMIT} addresses are logged.
     * @param ipAddressesCount the number of IP addresses returned. May be different from the length
     *        of ipAddresses if there were too many addresses to log.
     * @param uid the UID of the application that performed the query.
     */
    void onDnsEvent(int netId, int eventType, int returnCode, int latencyMs,
            @utf8InCpp String hostname, in @utf8InCpp String[] ipAddresses,
            int ipAddressesCount, int uid);

    /**
     * Represents a private DNS validation success or failure.
     *
     * @param netId the ID of the network the validation was performed on.
     * @param ipAddress the IP address for which validation was performed.
     * @param hostname the hostname for which validation was performed.
     * @param validated whether or not validation was successful.
     */
    void onPrivateDnsValidationEvent(int netId, String ipAddress, String hostname,
            boolean validated);

    /**
     * Logs a single connect library call.
     *
     * @param netId the ID of the network the connect was performed on.
     * @param error 0 if the connect call succeeded, otherwise errno if it failed.
     * @param latencyMs the latency of the connect call.
     * @param ipAddr destination IP address.
     * @param port destination port number.
     * @param uid the UID of the application that performed the connection.
     */
    void onConnectEvent(int netId, int error, int latencyMs, String ipAddr, int port, int uid);

    /**
     * Logs a single RX packet which caused the main CPU to exit sleep state.
     * @param prefix arbitrary string provided via wakeupAddInterface()
     * @param uid UID of the destination process or -1 if no UID is available.
     * @param ethertype of the RX packet encoded in an int in native order, or -1 if not available.
     * @param ipNextHeader ip protocol of the RX packet as IPPROTO_* number,
              or -1 if the packet was not IPv4 or IPv6.
     * @param dstHw destination hardware address, or 0 if not available.
     * @param srcIp source IP address, or null if not available.
     * @param dstIp destination IP address, or null if not available.
     * @param srcPort src port of RX packet in native order, or -1 if the packet was not UDP or TCP.
     * @param dstPort dst port of RX packet in native order, or -1 if the packet was not UDP or TCP.
     * @param timestampNs receive timestamp for the offending packet. In units of nanoseconds and
     *        synchronized to CLOCK_MONOTONIC.
     */
    void onWakeupEvent(String prefix, int uid, int ethertype, int ipNextHeader, in byte[] dstHw,
            String srcIp, String dstIp, int srcPort, int dstPort, long timestampNs);

    /**
     * An event sent after every Netlink sock_diag poll performed by Netd. This reported batch
     * groups TCP socket stats aggregated by network id. Per-network data are stored in a
     * structure-of-arrays style where networkIds, sentPackets, lostPackets, rttUs, and
     * sentAckDiffMs have the same length. Stats for the i-th network is spread across all these
     * arrays at index i.
     * @param networkIds an array of network ids for which there was tcp socket stats to collect in
     *        the last sock_diag poll.
     * @param sentPackets an array of packet sent across all TCP sockets still alive and new
              TCP sockets since the last sock_diag poll, summed per network id.
     * @param lostPackets, an array of packet lost across all TCP sockets still alive and new
              TCP sockets since the last sock_diag poll, summed per network id.
     * @param rttUs an array of smoothed round trip times in microseconds, averaged across all TCP
              sockets since the last sock_diag poll for a given network id.
     * @param sentAckDiffMs an array of milliseconds duration between the last packet sent and the
              last ack received for a socket, averaged across all TCP sockets for a network id.
     */
    void onTcpSocketStatsEvent(in int[] networkIds, in int[] sentPackets,
            in int[] lostPackets, in int[] rttUs, in int[] sentAckDiffMs);

    /**
     * Represents adding or removing a NAT64 prefix.
     *
     * @param netId the ID of the network the prefix was discovered on.
     * @param added true if the NAT64 prefix was added, or false if the NAT64 prefix was removed.
     *        There is only one prefix at a time for each netId. If a prefix is added, it replaces
     *        the previous-added prefix.
     * @param prefixString the detected NAT64 prefix as a string literal.
     * @param prefixLength the prefix length associated with this NAT64 prefix.
     */
    void onNat64PrefixEvent(int netId, boolean added, @utf8InCpp String prefixString,
            int prefixLength);
}
