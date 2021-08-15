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
}
