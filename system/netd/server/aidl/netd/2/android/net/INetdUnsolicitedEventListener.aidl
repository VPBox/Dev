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
