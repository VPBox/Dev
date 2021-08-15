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

package com.android.tests.apex;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import android.service.runtime.DebugEntryProto;
import android.service.runtime.RuntimeServiceInfoProto;

import com.android.tradefed.device.DeviceNotAvailableException;
import com.android.tradefed.testtype.DeviceJUnit4ClassRunner;
import com.android.tradefed.util.CommandResult;

import com.google.protobuf.InvalidProtocolBufferException;

import org.junit.Test;
import org.junit.runner.RunWith;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.util.List;
import java.util.Optional;

/**
 * Test to check if Apex can be staged, activated and uninstalled successfully.
 */
@RunWith(DeviceJUnit4ClassRunner.class)
public class TimezoneDataHostTest extends ApexE2EBaseHostTest {
    /**
     * Tests that if Apex package can be staged, activated and uninstalled successfully.
     */
    @Test
    public void testStageActivateUninstallApexPackage()
                                throws DeviceNotAvailableException, IOException {
        doTestStageActivateUninstallApexPackage();
    }

    @Override
    public void additionalCheck() {
        // Check that the device reports the expected information for when the test APEX is
        // installed.
        RuntimeServiceInfoProto runtimeServiceInfo = getRuntimeServiceInfoProto();
        List<DebugEntryProto> debugEntries = runtimeServiceInfo.getDebugEntryList();

        // Check the status of APK update (can't be present) and time zone data module
        // (must be present).
        assertEntryValue(debugEntries, "core_library.timezone.source.data_status", "NOT_FOUND");
        assertEntryValue(debugEntries, "core_library.timezone.source.tzdata_module_status", "OK");

        // Check the time zone data module has the expected version for test1_com.android.tzdata.
        String testDataVersion = "2030a";
        assertEntryValue(debugEntries, "core_library.timezone.source.tzdata_module_rulesVersion",
                testDataVersion);

        // Check all the libraries are reporting the same version.
        assertEntryValue(debugEntries, "core_library.timezone.lib.icu4j.tzdb_version",
                testDataVersion);
        assertEntryValue(debugEntries, "core_library.timezone.lib.libcore.tzdb_version",
                testDataVersion);
        assertEntryValue(debugEntries, "core_library.timezone.lib.icu4c.tzdb_version",
                testDataVersion);
    }

    private RuntimeServiceInfoProto getRuntimeServiceInfoProto() {
        try {
            CommandResult commandResult =
                    getDevice().executeShellV2Command("dumpsys runtime --proto");
            assertEquals(0, (int) commandResult.getExitCode());

            String outputString = commandResult.getStdout();
            byte[] outputBytes = outputString.getBytes(StandardCharsets.US_ASCII);
            return RuntimeServiceInfoProto.parseFrom(outputBytes);
        } catch (DeviceNotAvailableException e) {
            throw new AssertionError("Unable to run dumpsys", e);
        } catch (InvalidProtocolBufferException e) {
            throw new AssertionError("Unable to parse dumpsys output", e);
        }
    }

    private static void assertEntryValue(
            List<DebugEntryProto> entries, String entryKey, String expectedValue) {
        Optional<DebugEntryProto> matchingEntry =
                entries.stream().filter(x -> x.getKey().equals(entryKey)).findFirst();
        assertTrue("DebugEntryProto with key " + entryKey + " not found.",
                matchingEntry.isPresent());
        assertEquals("DebugEntryProto with key " + entryKey + " has bad value",
                expectedValue, matchingEntry.get().getStringValue());
    }
}
