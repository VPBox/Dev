/*
 * Copyright (C) 2019 The Android Open Source Project
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

import com.android.tradefed.device.DeviceNotAvailableException;
import com.android.tradefed.device.IManagedTestDevice;
import com.android.tradefed.testtype.DeviceJUnit4ClassRunner;
import com.android.tradefed.util.CommandResult;

import org.junit.Test;
import org.junit.runner.RunWith;

import java.io.IOException;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Test to check if Apex can be staged, activated and uninstalled successfully.
 */
@RunWith(DeviceJUnit4ClassRunner.class)
public class MediaHostTest extends ApexE2EBaseHostTest {
    // Pattern for extractor info in dumpsys media.extractor. e.g.:
    // AAC Extractor: plugin_version(3), uuid(4fd80eae03d24d729eb948fa6bb54613), \
    // version(1), path(/apex/com.android.media/lib/extractors/libaacextractor.so), \
    // supports: aac
    private static final Pattern EXTRACTOR_PLUGIN_REGEX =
            Pattern.compile("\\s+(.*): plugin_version\\((\\d+)\\), uuid\\((.+)\\)"
                    + ", version\\((\\d+)\\), path\\((.+)\\), supports: (.*)");
    private static final int DEFAULT_EXTRACTOR_PLUGIN_COUNT = 10;
    private static final String MEDIA_APEX_PATH = "/apex/com.android.media";

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
        assertTrue(((IManagedTestDevice) getDevice()).getMonitor().waitForBootComplete(60000));
        checkMediaExtractor();
    }

    private void checkMediaExtractor() {
        try {
            CommandResult commandResult =
                    getDevice().executeShellV2Command("dumpsys media.extractor");
            assertEquals(0, (int) commandResult.getExitCode());

            String outputString = commandResult.getStdout();
            Scanner in = new Scanner(outputString);
            int extractorCount = 0;
            while (in.hasNextLine()) {
                String line = in.nextLine();
                Matcher m = EXTRACTOR_PLUGIN_REGEX.matcher(line);
                if (m.matches()) {
                    assertTrue("Plugin path does not start with " + MEDIA_APEX_PATH
                            + ". dumpsys output: " + line,
                            m.group(5).startsWith(MEDIA_APEX_PATH));
                    extractorCount++;
                }
            }
            assertTrue("Failed to find enough plug-ins. expected: " + DEFAULT_EXTRACTOR_PLUGIN_COUNT
                    + " actual: " + extractorCount + " dumpsys output: " + outputString,
                    DEFAULT_EXTRACTOR_PLUGIN_COUNT <= extractorCount);
        } catch (DeviceNotAvailableException e) {
            throw new AssertionError("Unable to run dumpsys", e);
        }
    }
}
