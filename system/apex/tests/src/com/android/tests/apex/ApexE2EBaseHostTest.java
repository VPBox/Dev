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

import com.android.tradefed.config.Option;
import com.android.tradefed.config.Option.Importance;
import com.android.tradefed.device.DeviceNotAvailableException;
import com.android.tradefed.device.ITestDevice.ApexInfo;
import com.android.tradefed.log.LogUtil.CLog;
import com.android.tradefed.testtype.junit4.BaseHostJUnit4Test;
import com.android.tradefed.util.CommandResult;

import org.junit.After;
import org.junit.Assert;
import org.junit.Before;

import java.io.File;
import java.io.IOException;
import java.time.Duration;
import java.util.Set;
import java.util.regex.Pattern;
import java.util.stream.Stream;

/**
 * Base test to check if Apex can be staged, activated and uninstalled successfully.
 */
public abstract class ApexE2EBaseHostTest extends BaseHostJUnit4Test {
    private static final Duration WAIT_FOR_SESSION_READY_TTL = Duration.ofSeconds(10);
    private static final Duration SLEEP_FOR = Duration.ofMillis(200);

    protected static final String OPTION_APEX_FILE_NAME = "apex_file_name";

    protected final Pattern mIsSessionReadyPattern = Pattern.compile("isStagedSessionReady = true");
    protected final Pattern mIsSessionAppliedPattern =
            Pattern.compile("isStagedSessionApplied = true;");

    /* protected so that derived tests can have access to test utils automatically */
    protected final ApexTestUtils mUtils = new ApexTestUtils(this);

    @Option(name = OPTION_APEX_FILE_NAME,
            description = "The file name of the apex module.",
            importance = Importance.IF_UNSET,
            mandatory = true
    )
    protected String mApexFileName = null;

    @Before
    public synchronized void setUp() throws Exception {
        uninstallApex();
    }

    /**
     * Check if Apex package can be staged, activated and uninstalled successfully.
     */
    public void doTestStageActivateUninstallApexPackage()
                                throws DeviceNotAvailableException, IOException {

        File testAppFile = mUtils.getTestFile(mApexFileName);
        CLog.i("Found test apex file: " + testAppFile.getAbsoluteFile());

        // Install apex package
        String installResult = getDevice().installPackage(testAppFile, false);
        Assert.assertNull(
                String.format("failed to install test app %s. Reason: %s",
                    mApexFileName, installResult),
                installResult);

        // TODO: implement wait for session ready logic inside PackageManagerShellCommand instead.
        boolean sessionReady = false;
        Duration spentWaiting = Duration.ZERO;
        while (spentWaiting.compareTo(WAIT_FOR_SESSION_READY_TTL) < 0) {
            CommandResult res = getDevice().executeShellV2Command("pm get-stagedsessions");
            Assert.assertEquals("", res.getStderr());
            sessionReady = Stream.of(res.getStdout().split("\n")).anyMatch(this::isReadyNotApplied);
            if (sessionReady) {
                CLog.i("Done waiting after " + spentWaiting);
                break;
            }
            try {
                Thread.sleep(SLEEP_FOR.toMillis());
                spentWaiting = spentWaiting.plus(SLEEP_FOR);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                throw new RuntimeException(e);
            }
        }
        Assert.assertTrue("Staged session wasn't ready in " + WAIT_FOR_SESSION_READY_TTL,
                sessionReady);
        ApexInfo testApexInfo = mUtils.getApexInfo(testAppFile);
        Assert.assertNotNull(testApexInfo);

        getDevice().reboot(); // for install to take affect

        Set<ApexInfo> activatedApexes = getDevice().getActiveApexes();
        Assert.assertTrue(
                String.format("Failed to activate %s %s",
                    testApexInfo.name, testApexInfo.versionCode),
                activatedApexes.contains(testApexInfo));

        additionalCheck();
    }

    private boolean isReadyNotApplied(String sessionInfo) {
        boolean isReady = mIsSessionReadyPattern.matcher(sessionInfo).find();
        boolean isApplied = mIsSessionAppliedPattern.matcher(sessionInfo).find();
        return isReady && !isApplied;
    }

    /**
     * Do some additional check, invoked by doTestStageActivateUninstallApexPackage.
     */
    public abstract void additionalCheck();

    @After
    public void tearDown() throws Exception {
        uninstallApex();
    }

    private void uninstallApex() throws Exception {
        ApexInfo apex = mUtils.getApexInfo(mUtils.getTestFile(mApexFileName));
        String uninstallResult = getDevice().uninstallPackage(apex.name);
        if (uninstallResult != null) {
            // Uninstall failed. Most likely this means that there were no apex installed. No need
            // to reboot.
            CLog.w("Failed to uninstall apex " + apex.name + " : " + uninstallResult);
        } else {
            // Uninstall succeeded. Need to reboot.
            getDevice().reboot(); // for the uninstall to take affect
        }
    }
}
