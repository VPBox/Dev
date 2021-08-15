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

package android.net;
interface IDnsResolver {
  boolean isAlive();
  void registerEventListener(android.net.metrics.INetdEventListener listener);
  void setResolverConfiguration(in android.net.ResolverParamsParcel resolverParams);
  void getResolverInfo(int netId, out @utf8InCpp String[] servers, out @utf8InCpp String[] domains, out @utf8InCpp String[] tlsServers, out int[] params, out int[] stats, out int[] wait_for_pending_req_timeout_count);
  void startPrefix64Discovery(int netId);
  void stopPrefix64Discovery(int netId);
  @utf8InCpp String getPrefix64(int netId);
  void createNetworkCache(int netId);
  void destroyNetworkCache(int netId);
  void setLogSeverity(int logSeverity);
  const int RESOLVER_PARAMS_SAMPLE_VALIDITY = 0;
  const int RESOLVER_PARAMS_SUCCESS_THRESHOLD = 1;
  const int RESOLVER_PARAMS_MIN_SAMPLES = 2;
  const int RESOLVER_PARAMS_MAX_SAMPLES = 3;
  const int RESOLVER_PARAMS_BASE_TIMEOUT_MSEC = 4;
  const int RESOLVER_PARAMS_RETRY_COUNT = 5;
  const int RESOLVER_PARAMS_COUNT = 6;
  const int RESOLVER_STATS_SUCCESSES = 0;
  const int RESOLVER_STATS_ERRORS = 1;
  const int RESOLVER_STATS_TIMEOUTS = 2;
  const int RESOLVER_STATS_INTERNAL_ERRORS = 3;
  const int RESOLVER_STATS_RTT_AVG = 4;
  const int RESOLVER_STATS_LAST_SAMPLE_TIME = 5;
  const int RESOLVER_STATS_USABLE = 6;
  const int RESOLVER_STATS_COUNT = 7;
  const int DNS_RESOLVER_LOG_VERBOSE = 0;
  const int DNS_RESOLVER_LOG_DEBUG = 1;
  const int DNS_RESOLVER_LOG_INFO = 2;
  const int DNS_RESOLVER_LOG_WARNING = 3;
  const int DNS_RESOLVER_LOG_ERROR = 4;
}
