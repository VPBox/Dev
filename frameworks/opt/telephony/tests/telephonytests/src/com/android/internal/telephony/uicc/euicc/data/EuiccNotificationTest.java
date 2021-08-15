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

package com.android.internal.telephony.uicc.euicc.data;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotEquals;
import static org.junit.Assert.assertTrue;

import android.telephony.euicc.EuiccNotification;

import org.junit.Test;

public class EuiccNotificationTest {
    @Test
    public void testEqualsHashCode() {
        EuiccNotification n =
                new EuiccNotification(1, "g.co", EuiccNotification.EVENT_DELETE, new byte[]{1});
        assertTrue(n.equals(n));
        assertFalse(n.equals(new Object()));

        EuiccNotification t = null;
        assertFalse(n.equals(t));

        t = new EuiccNotification(1, "g.co", EuiccNotification.EVENT_DELETE, new byte[]{1});
        assertTrue(n.equals(t));
        assertEquals(n.hashCode(), t.hashCode());

        t = new EuiccNotification(2, "g.co", EuiccNotification.EVENT_DELETE, new byte[]{1});
        assertFalse(n.equals(t));
        assertNotEquals(n.hashCode(), t.hashCode());

        t = new EuiccNotification(1, "x.co", EuiccNotification.EVENT_DELETE, new byte[]{1});
        assertFalse(n.equals(t));
        assertNotEquals(n.hashCode(), t.hashCode());

        t = new EuiccNotification(1, "g.co", 0, new byte[]{1});
        assertFalse(n.equals(t));
        assertNotEquals(n.hashCode(), t.hashCode());

        t = new EuiccNotification(1, "g.co", EuiccNotification.EVENT_DELETE, null);
        assertFalse(n.equals(t));
        assertNotEquals(n.hashCode(), t.hashCode());
    }
}
