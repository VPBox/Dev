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

package com.android.internal.telephony.uicc;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNull;

import androidx.test.filters.SmallTest;

import org.junit.Test;

/**
 * Tests for {@link InstallCarrierAppUtils}
 */
public class InstallCarrierAppUtilsTest {
    @Test
    @SmallTest
    public void testParseAppNameMapFromString_emptyMap() {
        String appNameKey = "com.app.package1";
        String mapString = "";
        String appName =
                InstallCarrierAppUtils.getAppNameFromPackageName(appNameKey, mapString);
        assertNull(appName);
    }

    @Test
    @SmallTest
    public void testParseAppNameMapFromString_completeMap() {
        String appName1Key = "com.app.package1";
        String appName2Key = "com.app.package2";
        String expectedAppName1 = "AppName1";
        String expectedAppName2 = "AppName2";
        String mapString = appName1Key + ":" + expectedAppName1 + ";"
                + appName2Key + ":" + expectedAppName2 + ";";

        String appName1 =
                InstallCarrierAppUtils.getAppNameFromPackageName(appName1Key, mapString);
        String appName2 =
                InstallCarrierAppUtils.getAppNameFromPackageName(appName2Key, mapString);
        assertEquals(expectedAppName1, appName1);
        assertEquals(expectedAppName2, appName2);
    }

    @Test
    @SmallTest
    public void testParseAppNameMapFromString_packageCaseMismatch() {
        String appNameKey = "com.app.package1";
        String expectedAppName = "AppName1";
        String mapString = appNameKey + ":" + expectedAppName + ";";

        String appNameCaseTestKey = "cOm.ApP.pAcKaGe1";
        String appName1 =
                InstallCarrierAppUtils.getAppNameFromPackageName(appNameCaseTestKey, mapString);
        assertEquals(expectedAppName, appName1);
    }

    @Test
    @SmallTest
    public void testParseAppNameMapFromString_packageNotFound() {
        String appNameKey = "com.app.package1";
        String expectedAppName = "AppName1";
        String mapString = appNameKey + ":" + expectedAppName + ";";

        String missingAppNameKey = "AppName3";
        String missingAppName =
                InstallCarrierAppUtils.getAppNameFromPackageName(missingAppNameKey, mapString);
        assertNull(missingAppName);
    }
}
