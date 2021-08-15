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
import com.android.tradefed.device.ITestDevice.ApexInfo;
import com.android.tradefed.log.LogUtil.CLog;
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
public class MediaSwCodecHostTest extends ApexE2EBaseHostTest {
    private static final String CMD_LSHAL_CODECS =
            "lshal debug android.hardware.media.c2@1.0::IComponentStore/software";
    private static final Pattern CODEC_NAME_REGEX =
            Pattern.compile("\\s+name: (.+)");

    /**
     * Tests that if Apex package can be staged, activated and uninstalled successfully.
     */
    @Test
    public void testStageActivateUninstallApexPackage()
                                throws DeviceNotAvailableException, IOException {
        // Run tests only when the device has swcodec apex installed.
        for (ApexInfo info : getDevice().getActiveApexes()) {
            if (info.name.equals("com.android.media.swcodec")
                    || info.name.equals("com.google.android.media.swcodec")) {
                doTestStageActivateUninstallApexPackage();
                return;
            }
        }
        CLog.i("The device does not have swcodec apex installed. Skipping the test.");
    }

    @Override
    public void additionalCheck() {
        assertTrue(((IManagedTestDevice) getDevice()).getMonitor().waitForBootComplete(60000));
        checkCodecs();
    }

    private void checkCodecs() {
        try {
            CommandResult commandResult = getDevice().executeShellV2Command(CMD_LSHAL_CODECS);
            assertEquals("Couldn't list software codecs on device", 
                    0, (int) commandResult.getExitCode());

            String outputString = commandResult.getStdout();
            Scanner in = new Scanner(outputString);
            int codecCount = 0;
            while (in.hasNextLine()) {
                String line = in.nextLine();
                Matcher m = CODEC_NAME_REGEX.matcher(line);
                if (m.matches()) {
                    codecCount++;
                }
            }
            CLog.i("Found " + codecCount + " codecs");
            assertTrue("Failed to load software codecs. ", codecCount > 0);
        } catch (DeviceNotAvailableException e) {
            throw new AssertionError("Unable to run lshal", e);
        }
    }
}
