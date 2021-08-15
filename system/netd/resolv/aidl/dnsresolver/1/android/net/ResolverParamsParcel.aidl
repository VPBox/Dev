package android.net;
parcelable ResolverParamsParcel {
  int netId;
  int sampleValiditySeconds;
  int successThreshold;
  int minSamples;
  int maxSamples;
  int baseTimeoutMsec;
  int retryCount;
  @utf8InCpp String[] servers;
  @utf8InCpp String[] domains;
  @utf8InCpp String tlsName;
  @utf8InCpp String[] tlsServers;
  @utf8InCpp String[] tlsFingerprints;
}
