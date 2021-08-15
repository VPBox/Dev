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

package com.android.server.wifi.hotspot2.omadm;

import static org.junit.Assert.assertEquals;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

import android.os.Build;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.hotspot2.SystemInfo;

import org.junit.Test;

/**
 * Unit tests for {@link DevInfoMo}.
 * TODO(b/80300806): change the test to verify the XML in terms of the structure of XML.
 */
@SmallTest
public class DevInfoMoTest {
    private static final String TEST_DEV_ID = "12312341";
    private static final String TEST_MANUFACTURER = Build.MANUFACTURER;
    private static final String TEST_MODEL = Build.MODEL;
    private static final String TEST_LANGUAGE = "en";

    @Test
    public void serializeDevInfo() {
        SystemInfo systemInfo = mock(SystemInfo.class);
        when(systemInfo.getDeviceId()).thenReturn(TEST_DEV_ID);
        when(systemInfo.getDeviceManufacturer()).thenReturn(TEST_MANUFACTURER);
        when(systemInfo.getDeviceModel()).thenReturn(TEST_MODEL);
        when(systemInfo.getLanguage()).thenReturn(TEST_LANGUAGE);
        String expected = String.format(
                "<MgmtTree>"
                        + "<VerDTD>%s</VerDTD>"
                        + "<Node><NodeName>DevInfo</NodeName>"
                        + "<RTProperties><Type><DDFName>%s</DDFName></Type></RTProperties></Node>"
                        + "<Node><NodeName>DevID</NodeName><Value>%s</Value></Node>"
                        + "<Node><NodeName>Man</NodeName><Value>%s</Value></Node>"
                        + "<Node><NodeName>Mod</NodeName><Value>%s</Value></Node>"
                        + "<Node><NodeName>DmV</NodeName><Value>%s</Value></Node>"
                        + "<Node><NodeName>Lang</NodeName><Value>%s</Value></Node>"
                        + "</MgmtTree>",
                MoSerializer.DM_VERSION, DevInfoMo.URN, TEST_DEV_ID, TEST_MANUFACTURER,
                TEST_MODEL, MoSerializer.DM_VERSION, TEST_LANGUAGE);
        assertEquals(expected, DevInfoMo.serializeToXml(systemInfo));
    }
}