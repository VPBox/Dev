package android.net;
parcelable InterfaceConfigurationParcel {
  @utf8InCpp String ifName;
  @utf8InCpp String hwAddr;
  @utf8InCpp String ipv4Addr;
  int prefixLength;
  @utf8InCpp String[] flags;
}
