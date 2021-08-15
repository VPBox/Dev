package android.net;
interface INetdUnsolicitedEventListener {
  oneway void onInterfaceClassActivityChanged(boolean isActive, int timerLabel, long timestampNs, int uid);
  oneway void onQuotaLimitReached(@utf8InCpp String alertName, @utf8InCpp String ifName);
  oneway void onInterfaceDnsServerInfo(@utf8InCpp String ifName, long lifetimeS, in @utf8InCpp String[] servers);
  oneway void onInterfaceAddressUpdated(@utf8InCpp String addr, @utf8InCpp String ifName, int flags, int scope);
  oneway void onInterfaceAddressRemoved(@utf8InCpp String addr, @utf8InCpp String ifName, int flags, int scope);
  oneway void onInterfaceAdded(@utf8InCpp String ifName);
  oneway void onInterfaceRemoved(@utf8InCpp String ifName);
  oneway void onInterfaceChanged(@utf8InCpp String ifName, boolean up);
  oneway void onInterfaceLinkStateChanged(@utf8InCpp String ifName, boolean up);
  oneway void onRouteChanged(boolean updated, @utf8InCpp String route, @utf8InCpp String gateway, @utf8InCpp String ifName);
  oneway void onStrictCleartextDetected(int uid, @utf8InCpp String hex);
}
