/**
 * Copyright (c) 2018, The Android Open Source Project
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

package android.net;

/**
 * Unsolicited netd events which are reported by the kernel via netlink.
 * This one-way interface groups asynchronous notifications sent
 * by netd to any process that registered itself via INetd.registerUnsolEventListener.
 *
 * {@hide}
 */
oneway interface INetdUnsolicitedEventListener {

    /**
     * Notifies that an interface has been idle/active for a certain period of time.
     * It is the event for idletimer.
     *
     * @param isActive true for active status, false for idle
     * @param timerLabel unique identifier of the idletimer.
     *              Since NMS only set the identifier as int, only report event with int label.
     * @param timestampNs kernel timestamp of this event, 0 for no timestamp
     * @param uid uid of this event, -1 for no uid.
     *            It represents the uid that was responsible for waking the radio.
     */
    void onInterfaceClassActivityChanged(
            boolean isActive,
            int timerLabel,
            long timestampNs,
            int uid);

    /**
     * Notifies that a specific interface reached its quota limit.
     *
     * @param alertName alert name of the quota limit
     * @param ifName interface which reached the limit
     */
    void onQuotaLimitReached(@utf8InCpp String alertName, @utf8InCpp String ifName);

    /**
     * Provides information on IPv6 DNS servers on a specific interface.
     *
     * @param ifName interface name
     * @param lifetimeS lifetime for the DNS servers in seconds
     * @param servers the address of servers.
     *                  e.g. IpV6: "2001:4860:4860::6464"
     *
     */
    void onInterfaceDnsServerInfo(
            @utf8InCpp String ifName, long lifetimeS, in @utf8InCpp String[] servers);

    /**
     * Notifies that an address has updated on a specific interface.
     *
     * @param addr address that is being updated
     * @param ifName the name of the interface on which the address is configured
     * @param flags address flags, see ifa_flags in if_addr.h
     * @param scope current scope of the address
     */
    void onInterfaceAddressUpdated(
            @utf8InCpp String addr,
            @utf8InCpp String ifName,
            int flags,
            int scope);

    /**
     * Notifies that an address has been removed on a specific interface.
     *
     * @param addr address of this change
     * @param ifName the name of the interface that changed addresses
     * @param flags address flags, see ifa_flags in if_addr.h
     * @param scope address address scope
     */
    void onInterfaceAddressRemoved(
            @utf8InCpp String addr,
            @utf8InCpp String ifName,
            int flags,
            int scope);

    /**
     * Notifies that an interface has been added.
     *
     * @param ifName the name of the added interface
     */
    void onInterfaceAdded(@utf8InCpp String ifName);

    /**
     * Notifies that an interface has been removed.
     *
     * @param ifName the name of the removed interface
     */
    void onInterfaceRemoved(@utf8InCpp String ifName);

    /**
     * Notifies that the status of the specific interface has changed.
     *
     * @param ifName the name of the interface that changed status
     * @param up true for interface up, false for down
     */
    void onInterfaceChanged(@utf8InCpp String ifName, boolean up);

    /**
     * Notifies that the link state of the specific interface has changed.
     *
     * @param ifName the name of the interface whose link state has changed
     * @param up true for interface link state up, false for link state down
     */
    void onInterfaceLinkStateChanged(@utf8InCpp String ifName, boolean up);

    /**
     * Notifies that an IP route has changed.
     *
     * @param updated true for update, false for remove
     * @param route destination prefix of this route, e.g., "2001:db8::/64"
     * @param gateway address of gateway, empty string for no gateway
     * @param ifName interface name of this route, empty string for no interface
     */
    void onRouteChanged(
            boolean updated,
            @utf8InCpp String route,
            @utf8InCpp String gateway,
            @utf8InCpp String ifName);

    /**
     * Notifies that kernel has detected a socket sending data not wrapped
     * inside a layer of SSL/TLS encryption.
     *
     * @param uid uid of this event
     * @param hex packet content in hex format
     */
    void onStrictCleartextDetected(int uid, @utf8InCpp String hex);
}
