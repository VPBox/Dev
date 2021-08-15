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

package com.android.ike.ikev2.message;

import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.fail;

import com.android.ike.ikev2.IkeIdentification;
import com.android.ike.ikev2.IkeIdentification.IkeIpv4AddrIdentification;
import com.android.ike.ikev2.IkeIdentification.IkeIpv6AddrIdentification;
import com.android.ike.ikev2.exceptions.AuthenticationFailedException;

import org.junit.Test;

import java.net.Inet4Address;
import java.net.Inet6Address;
import java.nio.ByteBuffer;

public final class IkeIdPayloadTest {

    private static final String IPV4_ADDR_ID_PAYLOAD_RESPONDER_HEX_STRING =
            "2700000c01000000c0000264";
    private static final String IPV4_ADDR_ID_PAYLOAD_RESPONDER_BODY_HEX_STRING = "01000000c0000264";
    private static final String IPV4_ADDR_STRING = "192.0.2.100";

    private static final String IPV6_ADDR_ID_PAYLOAD_RESPONDER_HEX_STRING =
            "27000018050000000000200100000db80000000000000001";
    private static final String IPV6_ADDR_ID_PAYLOAD_RESPONDER_BODY_HEX_STRING =
            "050000000000200100000db80000000000000001";
    private static final String IPV6_ADDR_STRING = "0:2001:0:db8::1";

    private static final int ID_TYPE_OFFSET = 0;

    @Test
    public void testDecodeIpv4AddrIdPayload() throws Exception {
        byte[] inputPacket =
                TestUtils.hexStringToByteArray(IPV4_ADDR_ID_PAYLOAD_RESPONDER_BODY_HEX_STRING);
        IkeIdPayload payload = new IkeIdPayload(false, inputPacket, false);

        assertEquals(IkePayload.PAYLOAD_TYPE_ID_RESPONDER, payload.payloadType);
        assertEquals(IkeIdentification.ID_TYPE_IPV4_ADDR, payload.ikeId.idType);
        IkeIpv4AddrIdentification ikeId = (IkeIpv4AddrIdentification) payload.ikeId;
        Inet4Address expectedAddr = (Inet4Address) Inet4Address.getByName(IPV4_ADDR_STRING);
        assertEquals(expectedAddr, ikeId.ipv4Address);
    }

    @Test
    public void testDecodeIpv6AddrIdPayload() throws Exception {
        byte[] inputPacket =
                TestUtils.hexStringToByteArray(IPV6_ADDR_ID_PAYLOAD_RESPONDER_BODY_HEX_STRING);
        IkeIdPayload payload = new IkeIdPayload(false, inputPacket, false);

        assertEquals(IkePayload.PAYLOAD_TYPE_ID_RESPONDER, payload.payloadType);
        assertEquals(IkeIdentification.ID_TYPE_IPV6_ADDR, payload.ikeId.idType);
        IkeIpv6AddrIdentification ikeId = (IkeIpv6AddrIdentification) payload.ikeId;
        Inet6Address expectedAddr = (Inet6Address) Inet6Address.getByName(IPV6_ADDR_STRING);
        assertEquals(expectedAddr, ikeId.ipv6Address);
    }

    @Test
    public void testDecodeUnsupportedIdType() throws Exception {
        byte[] inputPacket =
                TestUtils.hexStringToByteArray(IPV4_ADDR_ID_PAYLOAD_RESPONDER_BODY_HEX_STRING);
        inputPacket[ID_TYPE_OFFSET] = 0;

        try {
            new IkeIdPayload(false, inputPacket, true);
            fail("Expected AuthenticationFailedException: ID Type is unsupported.");
        } catch (AuthenticationFailedException expected) {
        }
    }

    @Test
    public void testConstructAndEncodeIpv4AddrIdPayload() throws Exception {
        Inet4Address ipv4Address = (Inet4Address) Inet4Address.getByName(IPV4_ADDR_STRING);
        IkeIdPayload payload = new IkeIdPayload(false, new IkeIpv4AddrIdentification(ipv4Address));

        ByteBuffer inputBuffer = ByteBuffer.allocate(payload.getPayloadLength());
        payload.encodeToByteBuffer(IkePayload.PAYLOAD_TYPE_AUTH, inputBuffer);

        byte[] expectedBytes =
                TestUtils.hexStringToByteArray(IPV4_ADDR_ID_PAYLOAD_RESPONDER_HEX_STRING);
        assertArrayEquals(expectedBytes, inputBuffer.array());
    }

    @Test
    public void testConstructAndEncodeIpv6AddrIdPayload() throws Exception {
        Inet6Address ipv6Address = (Inet6Address) Inet6Address.getByName(IPV6_ADDR_STRING);
        IkeIdPayload payload = new IkeIdPayload(false, new IkeIpv6AddrIdentification(ipv6Address));

        ByteBuffer inputBuffer = ByteBuffer.allocate(payload.getPayloadLength());
        payload.encodeToByteBuffer(IkePayload.PAYLOAD_TYPE_AUTH, inputBuffer);

        byte[] expectedBytes =
                TestUtils.hexStringToByteArray(IPV6_ADDR_ID_PAYLOAD_RESPONDER_HEX_STRING);
        assertArrayEquals(expectedBytes, inputBuffer.array());
    }
}
