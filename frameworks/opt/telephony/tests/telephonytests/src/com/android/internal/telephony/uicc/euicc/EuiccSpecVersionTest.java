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

package com.android.internal.telephony.uicc.euicc;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotEquals;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;

import com.android.internal.telephony.uicc.IccUtils;

import org.junit.Test;

public class EuiccSpecVersionTest {
    @Test
    public void testCreate() {
        EuiccSpecVersion ver = new EuiccSpecVersion(1, 2, 3);
        assertEquals(1, ver.getMajor());
        assertEquals(2, ver.getMinor());
        assertEquals(3, ver.getRevision());
    }

    @Test
    public void testCreateFromBytes() {
        EuiccSpecVersion ver = new EuiccSpecVersion(new byte[] {1, 2, 3});
        assertEquals(1, ver.getMajor());
        assertEquals(2, ver.getMinor());
        assertEquals(3, ver.getRevision());
    }

    @Test
    public void testCompareTo() {
        EuiccSpecVersion ver1 = new EuiccSpecVersion(1, 2, 3);
        EuiccSpecVersion ver2 = new EuiccSpecVersion(1, 2, 3);
        assertEquals(0, ver1.compareTo(ver2));
        assertEquals(0, ver2.compareTo(ver1));
        assertTrue(ver1.equals(ver2));
        assertFalse(ver1.equals(null));
        assertFalse(ver1.equals(new Object()));
        assertEquals(ver1.hashCode(), ver2.hashCode());

        ver2 = new EuiccSpecVersion(2, 2, 3);
        assertEquals(-1, ver1.compareTo(ver2));
        assertEquals(1, ver2.compareTo(ver1));
        assertFalse(ver2.equals(ver1));
        assertNotEquals(ver1.hashCode(), ver2.hashCode());

        ver2 = new EuiccSpecVersion(1, 3, 3);
        assertEquals(-1, ver1.compareTo(ver2));
        assertEquals(1, ver2.compareTo(ver1));
        assertFalse(ver2.equals(ver1));
        assertNotEquals(ver1.hashCode(), ver2.hashCode());

        ver2 = new EuiccSpecVersion(1, 2, 4);
        assertEquals(-1, ver1.compareTo(ver2));
        assertEquals(1, ver2.compareTo(ver1));
        assertFalse(ver2.equals(ver1));
        assertNotEquals(ver1.hashCode(), ver2.hashCode());

        ver2 = new EuiccSpecVersion(2, 2, 2);
        assertEquals(-1, ver1.compareTo(ver2));
        assertEquals(1, ver2.compareTo(ver1));

        ver2 = new EuiccSpecVersion(2, 1, 3);
        assertEquals(-1, ver1.compareTo(ver2));
        assertEquals(1, ver2.compareTo(ver1));

        ver2 = new EuiccSpecVersion(1, 3, 2);
        assertEquals(-1, ver1.compareTo(ver2));
        assertEquals(1, ver2.compareTo(ver1));
    }

    @Test
    public void testFromOpenChannelResponse() {
        assertEquals(new EuiccSpecVersion(2, 1, 3),
                EuiccSpecVersion.fromOpenChannelResponse(
                        IccUtils.hexStringToBytes("E00582030201039000")));
    }

    @Test
    public void testFromOpenChannelResponseError() {
        // Invalid data
        assertNull(EuiccSpecVersion.fromOpenChannelResponse(
                IccUtils.hexStringToBytes("E00F05820202039000")));
        // Wrong length
        assertNull(EuiccSpecVersion.fromOpenChannelResponse(
                IccUtils.hexStringToBytes("E005820202039000")));
        // Wrong tag
        assertNull(EuiccSpecVersion.fromOpenChannelResponse(
                IccUtils.hexStringToBytes("E10482030201039000")));
    }
}
