///////////////////////////////////////////////////////////////////////////////
// THIS FILE IS IMMUTABLE. DO NOT EDIT IN ANY CASE.                          //
///////////////////////////////////////////////////////////////////////////////

// This file is a frozen snapshot of an AIDL interface (or parcelable). Do not
// try to edit this file. It looks like you are doing that because you have
// modified an AIDL interface in a backward-incompatible way, e.g., deleting a
// function from an interface or a field from a parcelable and it broke the
// build. That breakage is intended.
//
// You must not make a backward incompatible changes to the AIDL files built
// with the aidl_interface module type with versions property set. The module
// type is used to build AIDL files in a way that they can be used across
// independently updatable components of the system. If a device is shipped
// with such a backward incompatible change, it has a high risk of breaking
// later when a module using the interface is updated, e.g., Mainline modules.

package android.net.metrics;
interface INetdEventListener {
  oneway void onDnsEvent(int netId, int eventType, int returnCode, int latencyMs, @utf8InCpp String hostname, in @utf8InCpp String[] ipAddresses, int ipAddressesCount, int uid);
  oneway void onPrivateDnsValidationEvent(int netId, String ipAddress, String hostname, boolean validated);
  oneway void onConnectEvent(int netId, int error, int latencyMs, String ipAddr, int port, int uid);
  oneway void onWakeupEvent(String prefix, int uid, int ethertype, int ipNextHeader, in byte[] dstHw, String srcIp, String dstIp, int srcPort, int dstPort, long timestampNs);
  oneway void onTcpSocketStatsEvent(in int[] networkIds, in int[] sentPackets, in int[] lostPackets, in int[] rttUs, in int[] sentAckDiffMs);
  oneway void onNat64PrefixEvent(int netId, boolean added, @utf8InCpp String prefixString, int prefixLength);
  const int EVENT_GETADDRINFO = 1;
  const int EVENT_GETHOSTBYNAME = 2;
  const int EVENT_GETHOSTBYADDR = 3;
  const int EVENT_RES_NSEND = 4;
  const int REPORTING_LEVEL_NONE = 0;
  const int REPORTING_LEVEL_METRICS = 1;
  const int REPORTING_LEVEL_FULL = 2;
  const int DNS_REPORTED_IP_ADDRESSES_LIMIT = 10;
}
