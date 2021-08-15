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

#include <cstdint>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include <android-base/macros.h>
#include <gtest/gtest.h>

#include "netdutils/InternetAddresses.h"

namespace android {
namespace netdutils {
namespace {

enum Relation { EQ, LT };

std::ostream& operator<<(std::ostream& os, Relation relation) {
    switch (relation) {
        case EQ: os << "eq"; break;
        case LT: os << "lt"; break;
        default: os << "?!"; break;
    }
    return os;
}

template <typename T>
struct OperatorExpectation {
    const Relation relation;
    const T obj1;
    const T obj2;

    std::string toString() const {
        std::stringstream output;
        output << obj1 << " " << relation << " " << obj2;
        return output.str();
    }
};

template <typename T>
void testGamutOfOperators(const OperatorExpectation<T>& expectation) {
    switch (expectation.relation) {
        case EQ:
            EXPECT_TRUE(expectation.obj1 == expectation.obj2);
            EXPECT_TRUE(expectation.obj1 <= expectation.obj2);
            EXPECT_TRUE(expectation.obj1 >= expectation.obj2);
            EXPECT_FALSE(expectation.obj1 != expectation.obj2);
            EXPECT_FALSE(expectation.obj1 < expectation.obj2);
            EXPECT_FALSE(expectation.obj1 > expectation.obj2);
            break;

        case LT:
            EXPECT_TRUE(expectation.obj1 < expectation.obj2);
            EXPECT_TRUE(expectation.obj1 <= expectation.obj2);
            EXPECT_TRUE(expectation.obj1 != expectation.obj2);
            EXPECT_FALSE(expectation.obj1 > expectation.obj2);
            EXPECT_FALSE(expectation.obj1 >= expectation.obj2);
            EXPECT_FALSE(expectation.obj1 == expectation.obj2);
            break;

        default:
            FAIL() << "Unknown relation given in test expectation";
    }
}

const in_addr IPV4_ANY{htonl(INADDR_ANY)};
const in_addr IPV4_LOOPBACK{htonl(INADDR_LOOPBACK)};
const in_addr IPV4_ONES{~0U};
const in6_addr IPV6_ANY = IN6ADDR_ANY_INIT;
const in6_addr IPV6_LOOPBACK = IN6ADDR_LOOPBACK_INIT;
const in6_addr FE80{{{0xfe,0x80,0,0,0,0,0,0,0,0,0,0,0,0,0,0}}};
const in6_addr FE80_1{{{0xfe,0x80,0,0,0,0,0,0,0,0,0,0,0,0,0,1}}};
const in6_addr FE80_2{{{0xfe,0x80,0,0,0,0,0,0,0,0,0,0,0,0,0,2}}};
const uint8_t ff = std::numeric_limits<uint8_t>::max();
const in6_addr IPV6_ONES{{{ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff,ff}}};

TEST(IPAddressTest, GamutOfOperators) {
    const std::vector<OperatorExpectation<IPAddress>> kExpectations{
            {EQ, IPAddress(), IPAddress()},
            {EQ, IPAddress(IPV4_ONES), IPAddress(IPV4_ONES)},
            {EQ, IPAddress(IPV6_ONES), IPAddress(IPV6_ONES)},
            {EQ, IPAddress(FE80_1), IPAddress(FE80_1)},
            {EQ, IPAddress(FE80_2), IPAddress(FE80_2)},
            {LT, IPAddress(), IPAddress(IPV4_ANY)},
            {LT, IPAddress(), IPAddress(IPV4_ONES)},
            {LT, IPAddress(), IPAddress(IPV6_ANY)},
            {LT, IPAddress(), IPAddress(IPV6_ONES)},
            {LT, IPAddress(IPV4_ANY), IPAddress(IPV4_ONES)},
            {LT, IPAddress(IPV4_ANY), IPAddress(IPV6_ANY)},
            {LT, IPAddress(IPV4_ONES), IPAddress(IPV6_ANY)},
            {LT, IPAddress(IPV4_ONES), IPAddress(IPV6_ONES)},
            {LT, IPAddress(IPV6_ANY), IPAddress(IPV6_LOOPBACK)},
            {LT, IPAddress(IPV6_ANY), IPAddress(IPV6_ONES)},
            {LT, IPAddress(IPV6_LOOPBACK), IPAddress(IPV6_ONES)},
            {LT, IPAddress(FE80_1), IPAddress(FE80_2)},
            {LT, IPAddress(FE80_1), IPAddress(IPV6_ONES)},
            {LT, IPAddress(FE80_2), IPAddress(IPV6_ONES)},
            // Sort by scoped_id within the same address.
            {LT, IPAddress(FE80_1), IPAddress(FE80_1, 1)},
            {LT, IPAddress(FE80_1, 1), IPAddress(FE80_1, 2)},
            // Sort by address first, scope_id second.
            {LT, IPAddress(FE80_1, 2), IPAddress(FE80_2, 1)},
    };

    size_t tests_run = 0;
    for (const auto& expectation : kExpectations) {
        SCOPED_TRACE(expectation.toString());
        EXPECT_NO_FATAL_FAILURE(testGamutOfOperators(expectation));
        tests_run++;
    }
    EXPECT_EQ(kExpectations.size(), tests_run);
}

TEST(IPAddressTest, ScopeIds) {
    // Scope IDs ignored for IPv4 addresses.
    const IPAddress ones(IPV4_ONES);
    EXPECT_EQ(0U, ones.scope_id());
    const IPAddress ones22(ones, 22);
    EXPECT_EQ(0U, ones22.scope_id());
    EXPECT_EQ(ones, ones22);
    const IPAddress ones23(ones, 23);
    EXPECT_EQ(0U, ones23.scope_id());
    EXPECT_EQ(ones22, ones23);

    EXPECT_EQ("fe80::1%22", IPAddress(FE80_1, 22).toString());
    EXPECT_EQ("fe80::2%23", IPAddress(FE80_2, 23).toString());

    // Verify that given an IPAddress with a scope_id an address without a
    // scope_id can be constructed (just in case it's useful).
    const IPAddress fe80_intf22(FE80_1, 22);
    EXPECT_EQ(22U, fe80_intf22.scope_id());
    EXPECT_EQ(fe80_intf22, IPAddress(fe80_intf22));
    EXPECT_EQ(IPAddress(FE80_1), IPAddress(fe80_intf22, 0));
}

TEST(IPAddressTest, forString) {
    IPAddress ip;

    EXPECT_FALSE(IPAddress::forString("not_an_ip", &ip));
    EXPECT_FALSE(IPAddress::forString("not_an_ip", nullptr));
    EXPECT_EQ(IPAddress(), IPAddress::forString("not_an_ip"));

    EXPECT_EQ(IPAddress(IPV4_ANY), IPAddress::forString("0.0.0.0"));
    EXPECT_EQ(IPAddress(IPV4_ONES), IPAddress::forString("255.255.255.255"));
    EXPECT_EQ(IPAddress(IPV4_LOOPBACK), IPAddress::forString("127.0.0.1"));

    EXPECT_EQ(IPAddress(IPV6_ANY), IPAddress::forString("::"));
    EXPECT_EQ(IPAddress(IPV6_ANY), IPAddress::forString("::0"));
    EXPECT_EQ(IPAddress(IPV6_ANY), IPAddress::forString("0::"));
    EXPECT_EQ(IPAddress(IPV6_LOOPBACK), IPAddress::forString("::1"));
    EXPECT_EQ(IPAddress(IPV6_LOOPBACK), IPAddress::forString("0::1"));
    EXPECT_EQ(IPAddress(FE80_1), IPAddress::forString("fe80::1"));
    EXPECT_EQ(IPAddress(FE80_1, 22), IPAddress::forString("fe80::1%22"));
    // This relies upon having a loopback interface named "lo" with ifindex 1.
    EXPECT_EQ(IPAddress(FE80_1, 1), IPAddress::forString("fe80::1%lo"));
}

TEST(IPPrefixTest, IPv4Truncation) {
    const auto prefixStr = [](int length) -> std::string {
        return IPPrefix(IPAddress(IPV4_ONES), length).toString();
    };

    EXPECT_EQ("0.0.0.0/0", prefixStr(0));

    EXPECT_EQ("128.0.0.0/1", prefixStr(1));
    EXPECT_EQ("192.0.0.0/2", prefixStr(2));
    EXPECT_EQ("224.0.0.0/3", prefixStr(3));
    EXPECT_EQ("240.0.0.0/4", prefixStr(4));
    EXPECT_EQ("248.0.0.0/5", prefixStr(5));
    EXPECT_EQ("252.0.0.0/6", prefixStr(6));
    EXPECT_EQ("254.0.0.0/7", prefixStr(7));
    EXPECT_EQ("255.0.0.0/8", prefixStr(8));

    EXPECT_EQ("255.128.0.0/9", prefixStr(9));
    EXPECT_EQ("255.192.0.0/10", prefixStr(10));
    EXPECT_EQ("255.224.0.0/11", prefixStr(11));
    EXPECT_EQ("255.240.0.0/12", prefixStr(12));
    EXPECT_EQ("255.248.0.0/13", prefixStr(13));
    EXPECT_EQ("255.252.0.0/14", prefixStr(14));
    EXPECT_EQ("255.254.0.0/15", prefixStr(15));
    EXPECT_EQ("255.255.0.0/16", prefixStr(16));

    EXPECT_EQ("255.255.128.0/17", prefixStr(17));
    EXPECT_EQ("255.255.192.0/18", prefixStr(18));
    EXPECT_EQ("255.255.224.0/19", prefixStr(19));
    EXPECT_EQ("255.255.240.0/20", prefixStr(20));
    EXPECT_EQ("255.255.248.0/21", prefixStr(21));
    EXPECT_EQ("255.255.252.0/22", prefixStr(22));
    EXPECT_EQ("255.255.254.0/23", prefixStr(23));
    EXPECT_EQ("255.255.255.0/24", prefixStr(24));

    EXPECT_EQ("255.255.255.128/25", prefixStr(25));
    EXPECT_EQ("255.255.255.192/26", prefixStr(26));
    EXPECT_EQ("255.255.255.224/27", prefixStr(27));
    EXPECT_EQ("255.255.255.240/28", prefixStr(28));
    EXPECT_EQ("255.255.255.248/29", prefixStr(29));
    EXPECT_EQ("255.255.255.252/30", prefixStr(30));
    EXPECT_EQ("255.255.255.254/31", prefixStr(31));
    EXPECT_EQ("255.255.255.255/32", prefixStr(32));
}

TEST(IPPrefixTest, IPv6Truncation) {
    const auto prefixStr = [](int length) -> std::string {
        return IPPrefix(IPAddress(IPV6_ONES), length).toString();
    };

    EXPECT_EQ("::/0", prefixStr(0));

    EXPECT_EQ("8000::/1", prefixStr(1));
    EXPECT_EQ("c000::/2", prefixStr(2));
    EXPECT_EQ("e000::/3", prefixStr(3));
    EXPECT_EQ("f000::/4", prefixStr(4));
    EXPECT_EQ("f800::/5", prefixStr(5));
    EXPECT_EQ("fc00::/6", prefixStr(6));
    EXPECT_EQ("fe00::/7", prefixStr(7));
    EXPECT_EQ("ff00::/8", prefixStr(8));

    EXPECT_EQ("ff80::/9", prefixStr(9));
    EXPECT_EQ("ffc0::/10", prefixStr(10));
    EXPECT_EQ("ffe0::/11", prefixStr(11));
    EXPECT_EQ("fff0::/12", prefixStr(12));
    EXPECT_EQ("fff8::/13", prefixStr(13));
    EXPECT_EQ("fffc::/14", prefixStr(14));
    EXPECT_EQ("fffe::/15", prefixStr(15));
    EXPECT_EQ("ffff::/16", prefixStr(16));

    EXPECT_EQ("ffff:8000::/17", prefixStr(17));
    EXPECT_EQ("ffff:c000::/18", prefixStr(18));
    EXPECT_EQ("ffff:e000::/19", prefixStr(19));
    EXPECT_EQ("ffff:f000::/20", prefixStr(20));
    EXPECT_EQ("ffff:f800::/21", prefixStr(21));
    EXPECT_EQ("ffff:fc00::/22", prefixStr(22));
    EXPECT_EQ("ffff:fe00::/23", prefixStr(23));
    EXPECT_EQ("ffff:ff00::/24", prefixStr(24));

    EXPECT_EQ("ffff:ff80::/25", prefixStr(25));
    EXPECT_EQ("ffff:ffc0::/26", prefixStr(26));
    EXPECT_EQ("ffff:ffe0::/27", prefixStr(27));
    EXPECT_EQ("ffff:fff0::/28", prefixStr(28));
    EXPECT_EQ("ffff:fff8::/29", prefixStr(29));
    EXPECT_EQ("ffff:fffc::/30", prefixStr(30));
    EXPECT_EQ("ffff:fffe::/31", prefixStr(31));
    EXPECT_EQ("ffff:ffff::/32", prefixStr(32));

    EXPECT_EQ("ffff:ffff:8000::/33", prefixStr(33));
    EXPECT_EQ("ffff:ffff:c000::/34", prefixStr(34));
    EXPECT_EQ("ffff:ffff:e000::/35", prefixStr(35));
    EXPECT_EQ("ffff:ffff:f000::/36", prefixStr(36));
    EXPECT_EQ("ffff:ffff:f800::/37", prefixStr(37));
    EXPECT_EQ("ffff:ffff:fc00::/38", prefixStr(38));
    EXPECT_EQ("ffff:ffff:fe00::/39", prefixStr(39));
    EXPECT_EQ("ffff:ffff:ff00::/40", prefixStr(40));

    EXPECT_EQ("ffff:ffff:ff80::/41", prefixStr(41));
    EXPECT_EQ("ffff:ffff:ffc0::/42", prefixStr(42));
    EXPECT_EQ("ffff:ffff:ffe0::/43", prefixStr(43));
    EXPECT_EQ("ffff:ffff:fff0::/44", prefixStr(44));
    EXPECT_EQ("ffff:ffff:fff8::/45", prefixStr(45));
    EXPECT_EQ("ffff:ffff:fffc::/46", prefixStr(46));
    EXPECT_EQ("ffff:ffff:fffe::/47", prefixStr(47));
    EXPECT_EQ("ffff:ffff:ffff::/48", prefixStr(48));

    EXPECT_EQ("ffff:ffff:ffff:8000::/49", prefixStr(49));
    EXPECT_EQ("ffff:ffff:ffff:c000::/50", prefixStr(50));
    EXPECT_EQ("ffff:ffff:ffff:e000::/51", prefixStr(51));
    EXPECT_EQ("ffff:ffff:ffff:f000::/52", prefixStr(52));
    EXPECT_EQ("ffff:ffff:ffff:f800::/53", prefixStr(53));
    EXPECT_EQ("ffff:ffff:ffff:fc00::/54", prefixStr(54));
    EXPECT_EQ("ffff:ffff:ffff:fe00::/55", prefixStr(55));
    EXPECT_EQ("ffff:ffff:ffff:ff00::/56", prefixStr(56));

    EXPECT_EQ("ffff:ffff:ffff:ff80::/57", prefixStr(57));
    EXPECT_EQ("ffff:ffff:ffff:ffc0::/58", prefixStr(58));
    EXPECT_EQ("ffff:ffff:ffff:ffe0::/59", prefixStr(59));
    EXPECT_EQ("ffff:ffff:ffff:fff0::/60", prefixStr(60));
    EXPECT_EQ("ffff:ffff:ffff:fff8::/61", prefixStr(61));
    EXPECT_EQ("ffff:ffff:ffff:fffc::/62", prefixStr(62));
    EXPECT_EQ("ffff:ffff:ffff:fffe::/63", prefixStr(63));
    EXPECT_EQ("ffff:ffff:ffff:ffff::/64", prefixStr(64));

    EXPECT_EQ("ffff:ffff:ffff:ffff:8000::/65", prefixStr(65));
    EXPECT_EQ("ffff:ffff:ffff:ffff:c000::/66", prefixStr(66));
    EXPECT_EQ("ffff:ffff:ffff:ffff:e000::/67", prefixStr(67));
    EXPECT_EQ("ffff:ffff:ffff:ffff:f000::/68", prefixStr(68));
    EXPECT_EQ("ffff:ffff:ffff:ffff:f800::/69", prefixStr(69));
    EXPECT_EQ("ffff:ffff:ffff:ffff:fc00::/70", prefixStr(70));
    EXPECT_EQ("ffff:ffff:ffff:ffff:fe00::/71", prefixStr(71));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ff00::/72", prefixStr(72));

    EXPECT_EQ("ffff:ffff:ffff:ffff:ff80::/73", prefixStr(73));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffc0::/74", prefixStr(74));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffe0::/75", prefixStr(75));
    EXPECT_EQ("ffff:ffff:ffff:ffff:fff0::/76", prefixStr(76));
    EXPECT_EQ("ffff:ffff:ffff:ffff:fff8::/77", prefixStr(77));
    EXPECT_EQ("ffff:ffff:ffff:ffff:fffc::/78", prefixStr(78));
    EXPECT_EQ("ffff:ffff:ffff:ffff:fffe::/79", prefixStr(79));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff::/80", prefixStr(80));

    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:8000::/81", prefixStr(81));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:c000::/82", prefixStr(82));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:e000::/83", prefixStr(83));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:f000::/84", prefixStr(84));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:f800::/85", prefixStr(85));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:fc00::/86", prefixStr(86));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:fe00::/87", prefixStr(87));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:ff00::/88", prefixStr(88));

    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:ff80::/89", prefixStr(89));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:ffc0::/90", prefixStr(90));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:ffe0::/91", prefixStr(91));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:fff0::/92", prefixStr(92));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:fff8::/93", prefixStr(93));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:fffc::/94", prefixStr(94));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:fffe::/95", prefixStr(95));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:ffff::/96", prefixStr(96));

    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:ffff:8000:0/97", prefixStr(97));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:ffff:c000:0/98", prefixStr(98));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:ffff:e000:0/99", prefixStr(99));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:ffff:f000:0/100", prefixStr(100));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:ffff:f800:0/101", prefixStr(101));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:ffff:fc00:0/102", prefixStr(102));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:ffff:fe00:0/103", prefixStr(103));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:ffff:ff00:0/104", prefixStr(104));

    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:ffff:ff80:0/105", prefixStr(105));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:ffff:ffc0:0/106", prefixStr(106));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:ffff:ffe0:0/107", prefixStr(107));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:ffff:fff0:0/108", prefixStr(108));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:ffff:fff8:0/109", prefixStr(109));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:ffff:fffc:0/110", prefixStr(110));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:ffff:fffe:0/111", prefixStr(111));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:ffff:ffff:0/112", prefixStr(112));

    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:ffff:ffff:8000/113", prefixStr(113));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:ffff:ffff:c000/114", prefixStr(114));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:ffff:ffff:e000/115", prefixStr(115));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:ffff:ffff:f000/116", prefixStr(116));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:ffff:ffff:f800/117", prefixStr(117));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:ffff:ffff:fc00/118", prefixStr(118));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:ffff:ffff:fe00/119", prefixStr(119));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ff00/120", prefixStr(120));

    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ff80/121", prefixStr(121));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffc0/122", prefixStr(122));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffe0/123", prefixStr(123));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:ffff:ffff:fff0/124", prefixStr(124));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:ffff:ffff:fff8/125", prefixStr(125));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:ffff:ffff:fffc/126", prefixStr(126));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:ffff:ffff:fffe/127", prefixStr(127));
    EXPECT_EQ("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff/128", prefixStr(128));
}

TEST(IPPrefixTest, TruncationOther) {
    const struct {
        const char* ip;
        const int cidrLen;
        const char* ipTruncated;
    } testExpectations[] = {
            {"192.0.2.0", 24, "192.0.2.0"},
            {"192.0.2.0", 23, "192.0.2.0"},
            {"192.0.2.0", 22, "192.0.0.0"},
            {"192.0.2.0", 1, "128.0.0.0"},
            {"2001:db8:cafe:d00d::", 56, "2001:db8:cafe:d000::"},
            {"2001:db8:cafe:d00d::", 48, "2001:db8:cafe::"},
            {"2001:db8:cafe:d00d::", 47, "2001:db8:cafe::"},
            {"2001:db8:cafe:d00d::", 46, "2001:db8:cafc::"},
    };

    for (const auto& expectation : testExpectations) {
        IPAddress ip;
        EXPECT_TRUE(IPAddress::forString(expectation.ip, &ip))
                << "Failed to parse IP address " << expectation.ip;

        IPAddress ipTruncated;
        EXPECT_TRUE(IPAddress::forString(expectation.ipTruncated, &ipTruncated))
                << "Failed to parse IP address " << expectation.ipTruncated;

        IPPrefix prefix(ip, expectation.cidrLen);

        EXPECT_EQ(expectation.cidrLen, prefix.length())
                << "Unexpected cidrLen " << expectation.cidrLen;
        EXPECT_EQ(ipTruncated, prefix.ip())
                << "Unexpected IP truncation: " << prefix.ip() << ", expected: " << ipTruncated;
    }
}

TEST(IPPrefixTest, GamutOfOperators) {
    const std::vector<OperatorExpectation<IPPrefix>> kExpectations{
            {EQ, IPPrefix(), IPPrefix()},
            {EQ, IPPrefix(IPAddress(IPV4_ANY), 0), IPPrefix(IPAddress(IPV4_ANY), 0)},
            {EQ, IPPrefix(IPAddress(IPV4_ANY), IPV4_ADDR_BITS), IPPrefix(IPAddress(IPV4_ANY))},
            {EQ, IPPrefix(IPAddress(IPV6_ANY), 0), IPPrefix(IPAddress(IPV6_ANY), 0)},
            {EQ, IPPrefix(IPAddress(IPV6_ANY), IPV6_ADDR_BITS), IPPrefix(IPAddress(IPV6_ANY))},
            // Needlessly fully-specified IPv6 link-local address.
            {EQ, IPPrefix(IPAddress(FE80_1)), IPPrefix(IPAddress(FE80_1, 0), IPV6_ADDR_BITS)},
            // Different IPv6 link-local addresses within the same /64, no scoped_id: same /64.
            {EQ, IPPrefix(IPAddress(FE80_1), 64), IPPrefix(IPAddress(FE80_2), 64)},
            // Different IPv6 link-local address within the same /64, same scoped_id: same /64.
            {EQ, IPPrefix(IPAddress(FE80_1, 17), 64), IPPrefix(IPAddress(FE80_2, 17), 64)},
            // Unspecified < IPv4.
            {LT, IPPrefix(), IPPrefix(IPAddress(IPV4_ANY), 0)},
            // Same IPv4 base address sorts by prefix length.
            {LT, IPPrefix(IPAddress(IPV4_ANY), 0), IPPrefix(IPAddress(IPV4_ANY), 1)},
            {LT, IPPrefix(IPAddress(IPV4_ANY), 1), IPPrefix(IPAddress(IPV4_ANY), IPV4_ADDR_BITS)},
            // Truncation means each base IPv4 address is different.
            {LT, IPPrefix(IPAddress(IPV4_ONES), 0), IPPrefix(IPAddress(IPV4_ONES), 1)},
            {LT, IPPrefix(IPAddress(IPV4_ONES), 1), IPPrefix(IPAddress(IPV4_ONES), IPV4_ADDR_BITS)},
            // Sort by base IPv4 addresses first.
            {LT, IPPrefix(IPAddress(IPV4_ANY), 0), IPPrefix(IPAddress::forString("0.0.0.1"))},
            {LT, IPPrefix(IPAddress(IPV4_ANY), 1), IPPrefix(IPAddress::forString("0.0.0.1"))},
            {LT, IPPrefix(IPAddress(IPV4_ANY), 24), IPPrefix(IPAddress::forString("0.0.0.1"))},
            // IPv4 < IPv6.
            {LT, IPPrefix(IPAddress(IPV4_ANY), 0), IPPrefix(IPAddress(IPV6_ANY), 0)},
            {LT, IPPrefix(IPAddress(IPV4_ONES)), IPPrefix(IPAddress(IPV6_ANY))},
            // Unspecified < IPv6.
            {LT, IPPrefix(), IPPrefix(IPAddress(IPV6_ANY), 0)},
            // Same IPv6 base address sorts by prefix length.
            {LT, IPPrefix(IPAddress(IPV6_ANY), 0), IPPrefix(IPAddress(IPV6_ANY), 1)},
            {LT, IPPrefix(IPAddress(IPV6_ANY), 1), IPPrefix(IPAddress(IPV6_ANY), IPV6_ADDR_BITS)},
            // Truncation means each base IPv6 address is different.
            {LT, IPPrefix(IPAddress(IPV6_ONES), 0), IPPrefix(IPAddress(IPV6_ONES), 1)},
            {LT, IPPrefix(IPAddress(IPV6_ONES), 1), IPPrefix(IPAddress(IPV6_ONES), IPV6_ADDR_BITS)},
            // Different IPv6 link-local address in same /64, different scoped_id: different /64.
            {LT, IPPrefix(IPAddress(FE80_1, 17), 64), IPPrefix(IPAddress(FE80_2, 22), 64)},
            {LT, IPPrefix(IPAddress(FE80_1, 17), 64), IPPrefix(IPAddress(FE80_1, 18), 64)},
            {LT, IPPrefix(IPAddress(FE80_1, 18), 64), IPPrefix(IPAddress(FE80_1, 19), 64)},
    };

    size_t tests_run = 0;
    for (const auto& expectation : kExpectations) {
        SCOPED_TRACE(expectation.toString());
        EXPECT_NO_FATAL_FAILURE(testGamutOfOperators(expectation));
        tests_run++;
    }
    EXPECT_EQ(kExpectations.size(), tests_run);
}

TEST(IPSockAddrTest, GamutOfOperators) {
    const std::vector<OperatorExpectation<IPSockAddr>> kExpectations{
            {EQ, IPSockAddr(), IPSockAddr()},
            {EQ, IPSockAddr(IPAddress(IPV4_ANY)), IPSockAddr(IPAddress(IPV4_ANY), 0)},
            {EQ, IPSockAddr(IPAddress(IPV6_ANY)), IPSockAddr(IPAddress(IPV6_ANY), 0)},
            {EQ, IPSockAddr(IPAddress(FE80_1), 80), IPSockAddr(IPAddress(FE80_1), 80)},
            {EQ, IPSockAddr(IPAddress(FE80_1, 17)), IPSockAddr(IPAddress(FE80_1, 17), 0)},
            {LT, IPSockAddr(IPAddress(IPV4_ANY), 0), IPSockAddr(IPAddress(IPV4_ANY), 1)},
            {LT, IPSockAddr(IPAddress(IPV4_ANY), 53), IPSockAddr(IPAddress(IPV4_ANY), 123)},
            {LT, IPSockAddr(IPAddress(IPV4_ONES), 123), IPSockAddr(IPAddress(IPV6_ANY), 53)},
            {LT, IPSockAddr(IPAddress(IPV6_ANY), 0), IPSockAddr(IPAddress(IPV6_ANY), 1)},
            {LT, IPSockAddr(IPAddress(IPV6_ANY), 53), IPSockAddr(IPAddress(IPV6_ANY), 123)},
            {LT, IPSockAddr(IPAddress(FE80_1), 80), IPSockAddr(IPAddress(FE80_1, 17), 80)},
            {LT, IPSockAddr(IPAddress(FE80_1, 17), 80), IPSockAddr(IPAddress(FE80_1, 22), 80)},
    };

    size_t tests_run = 0;
    for (const auto& expectation : kExpectations) {
        SCOPED_TRACE(expectation.toString());
        EXPECT_NO_FATAL_FAILURE(testGamutOfOperators(expectation));
        tests_run++;
    }
    EXPECT_EQ(kExpectations.size(), tests_run);
}

TEST(IPSockAddrTest, toString) {
    EXPECT_EQ("<unspecified>:0", IPSockAddr().toString());
    EXPECT_EQ("0.0.0.0:0", IPSockAddr(IPAddress(IPV4_ANY)).toString());
    EXPECT_EQ("255.255.255.255:67", IPSockAddr(IPAddress(IPV4_ONES), 67).toString());
    EXPECT_EQ("[::]:0", IPSockAddr(IPAddress(IPV6_ANY)).toString());
    EXPECT_EQ("[::1]:53", IPSockAddr(IPAddress(IPV6_LOOPBACK), 53).toString());
    EXPECT_EQ("[fe80::1]:0", IPSockAddr(IPAddress(FE80_1)).toString());
    EXPECT_EQ("[fe80::2%17]:123", IPSockAddr(IPAddress(FE80_2, 17), 123).toString());
}

TEST(CompatIPDataTest, ConversionsClearUnneededValues) {
    const uint32_t idx = 17;
    const IPSockAddr linkLocalNtpSockaddr(IPAddress(FE80_2, idx), 123);
    EXPECT_EQ(IPAddress(FE80_2, idx), linkLocalNtpSockaddr.ip());
    // IPSockAddr(IPSockaddr.ip()) see the port cleared.
    EXPECT_EQ(0, IPSockAddr(linkLocalNtpSockaddr.ip()).port());
    const IPPrefix linkLocalPrefix(linkLocalNtpSockaddr.ip(), 64);
    EXPECT_EQ(IPAddress(FE80, idx), linkLocalPrefix.ip());
    // IPPrefix(IPPrefix.ip()) see the CIDR length cleared.
    EXPECT_EQ(IPV6_ADDR_BITS, IPPrefix(linkLocalPrefix.ip()).length());
}

}  // namespace
}  // namespace netdutils
}  // namespace android
