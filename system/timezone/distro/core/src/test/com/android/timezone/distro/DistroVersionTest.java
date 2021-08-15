/*
 * Copyright (C) 2017 The Android Open Source Project
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

package com.android.timezone.distro;

import junit.framework.TestCase;

public class DistroVersionTest extends TestCase {

    private static final int INVALID_VERSION_LOW = -1;
    private static final int VALID_VERSION = 23;
    private static final int INVALID_VERSION_HIGH = 1000;
    private static final String VALID_RULES_VERSION = "2016a";
    private static final String INVALID_RULES_VERSION = "A016a";

    public void testConstructorValidation() throws Exception {
        checkConstructorThrows(
                INVALID_VERSION_LOW, VALID_VERSION, VALID_RULES_VERSION, VALID_VERSION);
        checkConstructorThrows(
                INVALID_VERSION_HIGH, VALID_VERSION, VALID_RULES_VERSION, VALID_VERSION);
        checkConstructorThrows(
                VALID_VERSION, INVALID_VERSION_LOW, VALID_RULES_VERSION, VALID_VERSION);
        checkConstructorThrows(
                VALID_VERSION, INVALID_VERSION_HIGH, VALID_RULES_VERSION, VALID_VERSION);
        checkConstructorThrows(VALID_VERSION, VALID_VERSION, INVALID_RULES_VERSION, VALID_VERSION);
        checkConstructorThrows(VALID_VERSION, VALID_VERSION, VALID_RULES_VERSION,
                INVALID_VERSION_LOW);
        checkConstructorThrows(VALID_VERSION, VALID_VERSION, VALID_RULES_VERSION,
                INVALID_VERSION_HIGH);
    }

    private static void checkConstructorThrows(
            int majorVersion, int minorVersion, String rulesVersion, int revision) {
        try {
            new DistroVersion(majorVersion, minorVersion, rulesVersion, revision);
            fail();
        } catch (DistroException expected) {}
    }

    public void testConstructor() throws Exception {
        DistroVersion distroVersion = new DistroVersion(1, 2, VALID_RULES_VERSION, 3);
        assertEquals(1, distroVersion.formatMajorVersion);
        assertEquals(2, distroVersion.formatMinorVersion);
        assertEquals(VALID_RULES_VERSION, distroVersion.rulesVersion);
        assertEquals(3, distroVersion.revision);
    }

    public void testToFromBytesRoundTrip() throws Exception {
        DistroVersion distroVersion = new DistroVersion(1, 2, VALID_RULES_VERSION, 3);
        assertEquals(distroVersion, DistroVersion.fromBytes(distroVersion.toBytes()));
    }

    private DistroVersion createDistroVersion(int majorFormatVersion, int minorFormatVersion)
            throws DistroException {
        return new DistroVersion(majorFormatVersion, minorFormatVersion, VALID_RULES_VERSION, 3);
    }
}
