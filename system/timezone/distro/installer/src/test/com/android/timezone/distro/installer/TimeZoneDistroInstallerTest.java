/*
 * Copyright (C) 2015 The Android Open Source Project
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
package com.android.timezone.distro.installer;

import com.android.timezone.distro.DistroVersion;
import com.android.timezone.distro.FileUtils;
import com.android.timezone.distro.StagedDistroOperation;
import com.android.timezone.distro.TimeZoneDistro;
import com.android.timezone.distro.builder.TimeZoneDistroBuilder;

import junit.framework.TestCase;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.file.FileVisitResult;
import java.nio.file.FileVisitor;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.SimpleFileVisitor;
import java.nio.file.attribute.BasicFileAttributes;
import java.util.ArrayList;
import java.util.List;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;
import libcore.io.IoUtils;
import libcore.timezone.TzDataSetVersion;
import libcore.timezone.testing.ZoneInfoTestHelper;

import static org.junit.Assert.assertArrayEquals;

/**
 * Tests for {@link TimeZoneDistroInstaller}.
 */
public class TimeZoneDistroInstallerTest extends TestCase {

    // OLDER_RULES_VERSION < BASE_RULES_VERSION < NEW_RULES_VERSION < NEWER_RULES_VERSION
    private static final String OLDER_RULES_VERSION = "2030a";
    private static final String BASE_RULES_VERSION = "2030b";
    private static final String NEW_RULES_VERSION = "2030c";
    private static final String NEWER_RULES_VERSION = "2030d";

    private TimeZoneDistroInstaller installer;
    private File tempDir;
    private File testInstallDir;
    private File testBaseDataDir;

    @Override
    public void setUp() throws Exception {
        super.setUp();
        tempDir = createUniqueDirectory(null, "tempDir");
        testInstallDir = createSubDirectory(tempDir, "testInstall");
        testBaseDataDir =  createSubDirectory(tempDir, "testBaseData");

        // Create a tz_version file to indicate the base version of tz data on a device.
        TzDataSetVersion tzDataSetVersion =
                new TzDataSetVersion(
                        TzDataSetVersion.currentFormatMajorVersion(),
                        TzDataSetVersion.currentFormatMinorVersion(),
                        BASE_RULES_VERSION,
                        1 /* revision */);
        File testBaseVersionFile = new File(testBaseDataDir, TzDataSetVersion.DEFAULT_FILE_NAME);
        createFile(testBaseVersionFile, tzDataSetVersion.toBytes());

        installer = new TimeZoneDistroInstaller(
                "TimeZoneDistroInstallerTest", testBaseVersionFile, testInstallDir);
    }

    /**
     * Creates a unique temporary directory. rootDir can be null, in which case the directory will
     * be created beneath the directory pointed to by the java.io.tmpdir system property.
     */
    private static File createUniqueDirectory(File rootDir, String prefix) throws Exception {
        File dir = File.createTempFile(prefix, "", rootDir);
        assertTrue(dir.delete());
        assertTrue(dir.mkdir());
        return dir;
    }

    private static File createSubDirectory(File parent, String subDirName) {
        File dir = new File(parent, subDirName);
        assertTrue(dir.mkdir());
        return dir;
    }

    @Override
    public void tearDown() throws Exception {
        if (tempDir.exists()) {
            FileUtils.deleteRecursive(tempDir);
        }
        super.tearDown();
    }

    /** Tests the an update on a device will fail if the base tz_version file cannot be found. */
    public void testStageInstallWithErrorCode_badBaseFile() throws Exception {
        File doesNotExist = new File(testBaseDataDir, "doesNotExist");
        TimeZoneDistroInstaller brokenBaseInstaller = new TimeZoneDistroInstaller(
                "TimeZoneDistroInstallerTest", doesNotExist, testInstallDir);
        byte[] distroBytes = createValidTimeZoneDistroBytes(NEW_RULES_VERSION, 1);

        try {
            brokenBaseInstaller.stageInstallWithErrorCode(new TimeZoneDistro(distroBytes));
            fail();
        } catch (IOException expected) {}

        assertNoDistroOperationStaged();
        assertNoInstalledDistro();
    }

    /** Tests the first successful update on a device */
    public void testStageInstallWithErrorCode_successfulFirstUpdate() throws Exception {
        byte[] distroBytes = createValidTimeZoneDistroBytes(NEW_RULES_VERSION, 1);

        assertEquals(
                TimeZoneDistroInstaller.INSTALL_SUCCESS,
                installer.stageInstallWithErrorCode(new TimeZoneDistro(distroBytes)));
        assertInstallDistroStaged(distroBytes);
        assertNoInstalledDistro();
    }

    /**
     * Tests we can install an update with the same version as the base version.
     */
    public void testStageInstallWithErrorCode_successfulFirstUpdate_sameVersionAsBase()
            throws Exception {
        byte[] distroBytes = createValidTimeZoneDistroBytes(BASE_RULES_VERSION, 1);
        assertEquals(
                TimeZoneDistroInstaller.INSTALL_SUCCESS,
                installer.stageInstallWithErrorCode(new TimeZoneDistro(distroBytes)));
        assertInstallDistroStaged(distroBytes);
        assertNoInstalledDistro();
    }

    /**
     * Tests we cannot install an update older than the base version.
     */
    public void testStageInstallWithErrorCode_unsuccessfulFirstUpdate_olderVersionThanBase()
            throws Exception {
        byte[] distroBytes = createValidTimeZoneDistroBytes(OLDER_RULES_VERSION, 1);
        assertEquals(
                TimeZoneDistroInstaller.INSTALL_FAIL_RULES_TOO_OLD,
                installer.stageInstallWithErrorCode(new TimeZoneDistro(distroBytes)));
        assertNoDistroOperationStaged();
        assertNoInstalledDistro();
    }

    /**
     * Tests an update on a device when there is a prior update already staged.
     */
    public void testStageInstallWithErrorCode_successfulFollowOnUpdate_newerVersion()
            throws Exception {
        byte[] distro1Bytes = createValidTimeZoneDistroBytes(NEW_RULES_VERSION, 1);
        assertEquals(
                TimeZoneDistroInstaller.INSTALL_SUCCESS,
                installer.stageInstallWithErrorCode(new TimeZoneDistro(distro1Bytes)));
        assertInstallDistroStaged(distro1Bytes);

        byte[] distro2Bytes = createValidTimeZoneDistroBytes(NEW_RULES_VERSION, 2);
        assertEquals(
                TimeZoneDistroInstaller.INSTALL_SUCCESS,
                installer.stageInstallWithErrorCode(new TimeZoneDistro(distro2Bytes)));
        assertInstallDistroStaged(distro2Bytes);

        byte[] distro3Bytes = createValidTimeZoneDistroBytes(NEWER_RULES_VERSION, 1);
        assertEquals(
                TimeZoneDistroInstaller.INSTALL_SUCCESS,
                installer.stageInstallWithErrorCode(new TimeZoneDistro(distro3Bytes)));
        assertInstallDistroStaged(distro3Bytes);
        assertNoInstalledDistro();
    }

    /**
     * Tests an update on a device when there is a prior update already applied, but the follow
     * on update is older than the base version.
     */
    public void testStageInstallWithErrorCode_unsuccessfulFollowOnUpdate_olderVersion()
            throws Exception {
        byte[] distro1Bytes = createValidTimeZoneDistroBytes(NEW_RULES_VERSION, 2);
        assertEquals(
                TimeZoneDistroInstaller.INSTALL_SUCCESS,
                installer.stageInstallWithErrorCode(new TimeZoneDistro(distro1Bytes)));
        assertInstallDistroStaged(distro1Bytes);

        byte[] distro2Bytes = createValidTimeZoneDistroBytes(OLDER_RULES_VERSION, 1);
        assertEquals(
                TimeZoneDistroInstaller.INSTALL_FAIL_RULES_TOO_OLD,
                installer.stageInstallWithErrorCode(new TimeZoneDistro(distro2Bytes)));
        assertInstallDistroStaged(distro1Bytes);
        assertNoInstalledDistro();
    }

    /**
     * Tests staging an update when there's already an uninstall staged still results in a staged
     * install.
     */
    public void testStageInstallWithErrorCode_existingStagedUninstall()
            throws Exception {
        byte[] distro1Bytes = createValidTimeZoneDistroBytes(NEW_RULES_VERSION, 1);
        simulateInstalledDistro(distro1Bytes);
        assertInstalledDistro(distro1Bytes);

        assertEquals(TimeZoneDistroInstaller.UNINSTALL_SUCCESS, installer.stageUninstall());
        assertDistroUninstallStaged();
        assertInstalledDistro(distro1Bytes);

        byte[] distro2Bytes = createValidTimeZoneDistroBytes(NEWER_RULES_VERSION, 1);
        assertEquals(
                TimeZoneDistroInstaller.INSTALL_SUCCESS,
                installer.stageInstallWithErrorCode(new TimeZoneDistro(distro2Bytes)));
        assertInstalledDistro(distro1Bytes);
        assertInstallDistroStaged(distro2Bytes);
    }

    /** Tests that a distro with a missing tzdata file will not update the content. */
    public void testStageInstallWithErrorCode_missingTzDataFile() throws Exception {
        byte[] stagedDistroBytes = createValidTimeZoneDistroBytes(NEW_RULES_VERSION, 1);
        assertEquals(
                TimeZoneDistroInstaller.INSTALL_SUCCESS,
                installer.stageInstallWithErrorCode(new TimeZoneDistro(stagedDistroBytes)));
        assertInstallDistroStaged(stagedDistroBytes);

        byte[] incompleteDistroBytes =
                createValidTimeZoneDistroBuilder(NEWER_RULES_VERSION, 1)
                        .clearTzDataForTests()
                        .buildUnvalidatedBytes();
        assertEquals(
                TimeZoneDistroInstaller.INSTALL_FAIL_BAD_DISTRO_STRUCTURE,
                installer.stageInstallWithErrorCode(new TimeZoneDistro(incompleteDistroBytes)));
        assertInstallDistroStaged(stagedDistroBytes);
        assertNoInstalledDistro();
    }

    /** Tests that a distro with a missing ICU file will not update the content. */
    public void testStageInstallWithErrorCode_missingIcuFile() throws Exception {
        byte[] stagedDistroBytes = createValidTimeZoneDistroBytes(NEW_RULES_VERSION, 1);
        assertEquals(
                TimeZoneDistroInstaller.INSTALL_SUCCESS,
                installer.stageInstallWithErrorCode(new TimeZoneDistro(stagedDistroBytes)));
        assertInstallDistroStaged(stagedDistroBytes);

        byte[] incompleteDistroBytes =
                createValidTimeZoneDistroBuilder(NEWER_RULES_VERSION, 1)
                        .clearIcuDataForTests()
                        .buildUnvalidatedBytes();
        assertEquals(
                TimeZoneDistroInstaller.INSTALL_FAIL_BAD_DISTRO_STRUCTURE,
                installer.stageInstallWithErrorCode(new TimeZoneDistro(incompleteDistroBytes)));
        assertInstallDistroStaged(stagedDistroBytes);
        assertNoInstalledDistro();
    }

    /** Tests that a distro with a missing tzlookup file will not update the content. */
    public void testStageInstallWithErrorCode_missingTzLookupFile() throws Exception {
        byte[] stagedDistroBytes = createValidTimeZoneDistroBytes(NEW_RULES_VERSION, 1);
        assertEquals(
                TimeZoneDistroInstaller.INSTALL_SUCCESS,
                installer.stageInstallWithErrorCode(new TimeZoneDistro(stagedDistroBytes)));
        assertInstallDistroStaged(stagedDistroBytes);

        byte[] incompleteDistroBytes =
                createValidTimeZoneDistroBuilder(NEWER_RULES_VERSION, 1)
                        .setTzLookupXml(null)
                        .buildUnvalidatedBytes();
        assertEquals(
                TimeZoneDistroInstaller.INSTALL_FAIL_BAD_DISTRO_STRUCTURE,
                installer.stageInstallWithErrorCode(new TimeZoneDistro(incompleteDistroBytes)));
        assertInstallDistroStaged(stagedDistroBytes);
        assertNoInstalledDistro();
    }

    /** Tests that a distro with a bad tzlookup file will not update the content. */
    public void testStageInstallWithErrorCode_badTzLookupFile() throws Exception {
        byte[] stagedDistroBytes = createValidTimeZoneDistroBytes(NEW_RULES_VERSION, 1);
        assertEquals(
                TimeZoneDistroInstaller.INSTALL_SUCCESS,
                installer.stageInstallWithErrorCode(new TimeZoneDistro(stagedDistroBytes)));
        assertInstallDistroStaged(stagedDistroBytes);

        byte[] incompleteDistroBytes =
                createValidTimeZoneDistroBuilder(NEWER_RULES_VERSION, 1)
                        .setTzLookupXml("<foo />")
                        .buildUnvalidatedBytes();
        assertEquals(
                TimeZoneDistroInstaller.INSTALL_FAIL_VALIDATION_ERROR,
                installer.stageInstallWithErrorCode(new TimeZoneDistro(incompleteDistroBytes)));
        assertInstallDistroStaged(stagedDistroBytes);
        assertNoInstalledDistro();
    }

    /**
     * Tests that an update will be unpacked even if there is a partial update from a previous run.
     */
    public void testStageInstallWithErrorCode_withWorkingDir() throws Exception {
        File workingDir = installer.getWorkingDir();
        assertTrue(workingDir.mkdir());
        createFile(new File(workingDir, "myFile"), new byte[] { 'a' });

        byte[] distroBytes = createValidTimeZoneDistroBytes(NEW_RULES_VERSION, 1);
        assertEquals(
                TimeZoneDistroInstaller.INSTALL_SUCCESS,
                installer.stageInstallWithErrorCode(new TimeZoneDistro(distroBytes)));
        assertInstallDistroStaged(distroBytes);
        assertNoInstalledDistro();
    }

    /**
     * Tests that a distro without a distro version file will be rejected.
     */
    public void testStageInstallWithErrorCode_withMissingDistroVersionFile() throws Exception {
        // Create a distro without a version file.
        byte[] distroBytes = createValidTimeZoneDistroBuilder(NEW_RULES_VERSION, 1)
                .clearVersionForTests()
                .buildUnvalidatedBytes();
        assertEquals(
                TimeZoneDistroInstaller.INSTALL_FAIL_BAD_DISTRO_STRUCTURE,
                installer.stageInstallWithErrorCode(new TimeZoneDistro(distroBytes)));
        assertNoDistroOperationStaged();
        assertNoInstalledDistro();
    }

    /**
     * Tests that a distro with an newer distro version will be rejected.
     */
    public void testStageInstallWithErrorCode_withNewerDistroVersion() throws Exception {
        // Create a distro that will appear to be newer than the one currently supported.
        byte[] distroBytes = createValidTimeZoneDistroBuilder(NEW_RULES_VERSION, 1)
                .replaceFormatVersionForTests(
                        TzDataSetVersion.currentFormatMajorVersion() + 1,
                        TzDataSetVersion.currentFormatMinorVersion())
                .buildUnvalidatedBytes();
        assertEquals(
                TimeZoneDistroInstaller.INSTALL_FAIL_BAD_DISTRO_FORMAT_VERSION,
                installer.stageInstallWithErrorCode(new TimeZoneDistro(distroBytes)));
        assertNoDistroOperationStaged();
        assertNoInstalledDistro();
    }

    /**
     * Tests that a distro with a badly formed distro version will be rejected.
     */
    public void testStageInstallWithErrorCode_withBadlyFormedDistroVersion() throws Exception {
        // Create a distro that has an invalid major distro version. It should be 3 numeric
        // characters, "." and 3 more numeric characters.
        byte[] invalidFormatVersionBytes =
                createValidTimeZoneDistroBuilder(NEW_RULES_VERSION, 1).buildUnvalidatedBytes();
        invalidFormatVersionBytes[0] = 'A';

        TimeZoneDistro distro = createTimeZoneDistroWithVersionBytes(invalidFormatVersionBytes);
        assertEquals(
                TimeZoneDistroInstaller.INSTALL_FAIL_BAD_DISTRO_STRUCTURE,
                installer.stageInstallWithErrorCode(distro));
        assertNoDistroOperationStaged();
        assertNoInstalledDistro();
    }

    /**
     * Tests that a distro with a badly formed revision will be rejected.
     */
    public void testStageInstallWithErrorCode_withBadlyFormedRevision() throws Exception {
        // Create a distro that has an invalid revision. It should be 3 numeric characters.
        byte[] invalidRevisionBytes =
                createValidTimeZoneDistroBuilder(NEW_RULES_VERSION, 1).buildUnvalidatedBytes();
        invalidRevisionBytes[invalidRevisionBytes.length - 3] = 'A';

        TimeZoneDistro distro = createTimeZoneDistroWithVersionBytes(invalidRevisionBytes);
        assertEquals(
                TimeZoneDistroInstaller.INSTALL_FAIL_BAD_DISTRO_STRUCTURE,
                installer.stageInstallWithErrorCode(distro));
        assertNoDistroOperationStaged();
        assertNoInstalledDistro();
    }

    /**
     * Tests that a distro with a badly formed rules version will be rejected.
     */
    public void testStageInstallWithErrorCode_withBadlyFormedRulesVersion() throws Exception {
        // Create a distro that has an invalid rules version. It should be in the form "2016c".
        byte[] invalidRulesVersionBytes =
                createValidTimeZoneDistroBuilder(NEW_RULES_VERSION, 1).buildUnvalidatedBytes();
        invalidRulesVersionBytes[invalidRulesVersionBytes.length - 6] = 'B';

        TimeZoneDistro distro = createTimeZoneDistroWithVersionBytes(invalidRulesVersionBytes);
        assertEquals(
                TimeZoneDistroInstaller.INSTALL_FAIL_BAD_DISTRO_STRUCTURE,
                installer.stageInstallWithErrorCode(distro));
        assertNoDistroOperationStaged();
        assertNoInstalledDistro();
    }

    /** Tests what happens if a stageUninstall() is attempted when there's nothing installed. */
    public void testStageUninstall_noExistingDistro() throws Exception {
        // To stage an uninstall, there would need to be installed rules.
        assertEquals(
                TimeZoneDistroInstaller.UNINSTALL_NOTHING_INSTALLED,
                installer.stageUninstall());

        assertNoDistroOperationStaged();
        assertNoInstalledDistro();
    }

    /** Tests what happens if a stageUninstall() is attempted when there's something installed. */
    public void testStageUninstall_existingInstalledDataDistro() throws Exception {
        // To stage an uninstall, we need to have some installed rules.
        byte[] installedDistroBytes = createValidTimeZoneDistroBytes(NEW_RULES_VERSION, 1);
        simulateInstalledDistro(installedDistroBytes);

        File stagedDataDir = installer.getStagedTzDataDir();
        assertTrue(stagedDataDir.mkdir());

        assertEquals(
                TimeZoneDistroInstaller.UNINSTALL_SUCCESS,
                installer.stageUninstall());
        assertDistroUninstallStaged();
        assertInstalledDistro(installedDistroBytes);
    }

    /**
     * Tests what happens if a stageUninstall() is attempted when there's something installed
     * and there's a staged install.
     */
    public void testStageUninstall_existingStagedInstall() throws Exception {
        File stagedDataDir = installer.getStagedTzDataDir();
        assertTrue(stagedDataDir.mkdir());

        // Stage an install.
        byte[] distroBytes = createValidTimeZoneDistroBytes(NEW_RULES_VERSION, 1);
        assertEquals(TimeZoneDistroInstaller.INSTALL_SUCCESS,
                installer.stageInstallWithErrorCode(new TimeZoneDistro(distroBytes)));

        // Now uninstall. It should just remove the staged install.
        assertEquals(
                TimeZoneDistroInstaller.UNINSTALL_NOTHING_INSTALLED,
                installer.stageUninstall());
        assertNoDistroOperationStaged();
    }

    /**
     * Tests what happens if a stageUninstall() is attempted when there's something installed
     * and there's a staged uninstall.
     */
    public void testStageUninstall_existingStagedUninstall() throws Exception {
        // To stage an uninstall, we need to have some installed rules.
        byte[] installedDistroBytes = createValidTimeZoneDistroBytes(NEW_RULES_VERSION, 1);
        simulateInstalledDistro(installedDistroBytes);

        File stagedDataDir = installer.getStagedTzDataDir();
        assertTrue(stagedDataDir.mkdir());

        assertEquals(
                TimeZoneDistroInstaller.UNINSTALL_SUCCESS,
                installer.stageUninstall());
        assertDistroUninstallStaged();
        assertInstalledDistro(installedDistroBytes);

        // Now stage a second uninstall.
        assertEquals(
                TimeZoneDistroInstaller.UNINSTALL_SUCCESS,
                installer.stageUninstall());
        assertDistroUninstallStaged();
        assertInstalledDistro(installedDistroBytes);
    }

    /**
     * Tests what happens if a stageUninstall() is attempted when there are unexpected working
     * directories present.
     */
    public void testStageUninstall_oldDirsAlreadyExists() throws Exception {
        // To stage an uninstall, we need to have some installed rules.
        byte[] installedDistroBytes = createValidTimeZoneDistroBytes(NEW_RULES_VERSION, 1);
        simulateInstalledDistro(installedDistroBytes);

        File oldStagedDataDir = installer.getOldStagedDataDir();
        assertTrue(oldStagedDataDir.mkdir());

        File workingDir = installer.getWorkingDir();
        assertTrue(workingDir.mkdir());

        assertEquals(
                TimeZoneDistroInstaller.UNINSTALL_SUCCESS,
                installer.stageUninstall());

        assertDistroUninstallStaged();
        assertFalse(workingDir.exists());
        assertFalse(oldStagedDataDir.exists());
        assertInstalledDistro(installedDistroBytes);
    }

    public void testReadBaseRulesVersion() throws Exception {
        TzDataSetVersion actualBaseVersion = installer.readBaseVersion();
        assertEquals(BASE_RULES_VERSION, actualBaseVersion.rulesVersion);
    }

    public void testGetInstalledDistroVersion() throws Exception {
        // Check result when nothing installed.
        assertNull(installer.getInstalledDistroVersion());
        assertNoDistroOperationStaged();
        assertNoInstalledDistro();

        // Now simulate there being an existing install active.
        byte[] distroBytes = createValidTimeZoneDistroBytes(NEW_RULES_VERSION, 1);
        simulateInstalledDistro(distroBytes);
        assertInstalledDistro(distroBytes);

        // Check result when something installed.
        assertEquals(new TimeZoneDistro(distroBytes).getDistroVersion(),
                installer.getInstalledDistroVersion());
        assertNoDistroOperationStaged();
        assertInstalledDistro(distroBytes);
    }

    public void testGetStagedDistroOperation() throws Exception {
        byte[] distro1Bytes = createValidTimeZoneDistroBytes(NEW_RULES_VERSION, 1);
        byte[] distro2Bytes = createValidTimeZoneDistroBytes(NEWER_RULES_VERSION, 1);

        // Check result when nothing staged.
        assertNull(installer.getStagedDistroOperation());
        assertNoDistroOperationStaged();
        assertNoInstalledDistro();

        // Check result after unsuccessfully staging an uninstall.
        // Can't stage an uninstall without an installed distro.
        assertEquals(
                TimeZoneDistroInstaller.UNINSTALL_NOTHING_INSTALLED,
                installer.stageUninstall());
        assertNull(installer.getStagedDistroOperation());
        assertNoDistroOperationStaged();
        assertNoInstalledDistro();

        // Check result after staging an install.
        assertEquals(
                TimeZoneDistroInstaller.INSTALL_SUCCESS,
                installer.stageInstallWithErrorCode(new TimeZoneDistro(distro1Bytes)));
        StagedDistroOperation expectedStagedInstall =
                StagedDistroOperation.install(new TimeZoneDistro(distro1Bytes).getDistroVersion());
        assertEquals(expectedStagedInstall, installer.getStagedDistroOperation());
        assertInstallDistroStaged(distro1Bytes);
        assertNoInstalledDistro();

        // Check result after unsuccessfully staging an uninstall (but after removing a staged
        // install). Can't stage an uninstall without an installed distro.
        assertEquals(
                TimeZoneDistroInstaller.UNINSTALL_NOTHING_INSTALLED,
                installer.stageUninstall());
        assertNull(installer.getStagedDistroOperation());
        assertNoDistroOperationStaged();
        assertNoInstalledDistro();

        // Now simulate there being an existing install active.
        simulateInstalledDistro(distro1Bytes);
        assertInstalledDistro(distro1Bytes);

        // Check state after successfully staging an uninstall.
        assertEquals(
                TimeZoneDistroInstaller.UNINSTALL_SUCCESS,
                installer.stageUninstall());
        StagedDistroOperation expectedStagedUninstall = StagedDistroOperation.uninstall();
        assertEquals(expectedStagedUninstall, installer.getStagedDistroOperation());
        assertDistroUninstallStaged();
        assertInstalledDistro(distro1Bytes);

        // Check state after successfully staging an install.
        assertEquals(TimeZoneDistroInstaller.INSTALL_SUCCESS,
                installer.stageInstallWithErrorCode(new TimeZoneDistro(distro2Bytes)));
        StagedDistroOperation expectedStagedInstall2 =
                StagedDistroOperation.install(new TimeZoneDistro(distro2Bytes).getDistroVersion());
        assertEquals(expectedStagedInstall2, installer.getStagedDistroOperation());
        assertInstallDistroStaged(distro2Bytes);
        assertInstalledDistro(distro1Bytes);
    }

    private static byte[] createValidTimeZoneDistroBytes(
            String rulesVersion, int revision) throws Exception {
        return createValidTimeZoneDistroBuilder(rulesVersion, revision).buildBytes();
    }

    private static TimeZoneDistroBuilder createValidTimeZoneDistroBuilder(
            String rulesVersion, int revision) throws Exception {

        byte[] tzData = createTzData(rulesVersion);
        byte[] icuData = new byte[] { 'a' };
        String tzlookupXml = "<timezones ianaversion=\"" + rulesVersion + "\">\n"
                + "  <countryzones>\n"
                + "    <country code=\"us\" default=\"America/New_York\" everutc=\"n\">\n"
                + "      <id>America/New_York</id>\n"
                + "      <id>America/Los_Angeles</id>\n"
                + "    </country>\n"
                + "    <country code=\"gb\" default=\"Europe/London\" everutc=\"y\">\n"
                + "      <id>Europe/London</id>\n"
                + "    </country>\n"
                + "  </countryzones>\n"
                + "</timezones>\n";
        DistroVersion distroVersion = new DistroVersion(
                TzDataSetVersion.currentFormatMajorVersion(),
                TzDataSetVersion.currentFormatMinorVersion(),
                rulesVersion,
                revision);
        return new TimeZoneDistroBuilder()
                .setDistroVersion(distroVersion)
                .setTzDataFile(tzData)
                .setIcuDataFile(icuData)
                .setTzLookupXml(tzlookupXml);
    }

    private void assertInstallDistroStaged(byte[] expectedDistroBytes) throws Exception {
        assertTrue(testInstallDir.exists());

        File stagedTzDataDir = installer.getStagedTzDataDir();
        assertTrue(stagedTzDataDir.exists());

        File distroVersionFile =
                new File(stagedTzDataDir, TimeZoneDistro.DISTRO_VERSION_FILE_NAME);
        assertTrue(distroVersionFile.exists());

        File tzdataFile = new File(stagedTzDataDir, TimeZoneDistro.TZDATA_FILE_NAME);
        assertTrue(tzdataFile.exists());

        File icuFile = new File(stagedTzDataDir, TimeZoneDistro.ICU_DATA_FILE_NAME);
        assertTrue(icuFile.exists());

        File tzLookupFile = new File(stagedTzDataDir, TimeZoneDistro.TZLOOKUP_FILE_NAME);
        assertTrue(tzLookupFile.exists());

        // Assert getStagedDistroState() is reporting correctly.
        StagedDistroOperation stagedDistroOperation = installer.getStagedDistroOperation();
        assertNotNull(stagedDistroOperation);
        assertFalse(stagedDistroOperation.isUninstall);
        assertEquals(new TimeZoneDistro(expectedDistroBytes).getDistroVersion(),
                stagedDistroOperation.distroVersion);

        File expectedZipContentDir = createUniqueDirectory(tempDir, "expectedZipContent");
        new TimeZoneDistro(expectedDistroBytes).extractTo(expectedZipContentDir);

        assertContentsMatches(
                new File(expectedZipContentDir, TimeZoneDistro.DISTRO_VERSION_FILE_NAME),
                distroVersionFile);
        assertContentsMatches(
                new File(expectedZipContentDir, TimeZoneDistro.ICU_DATA_FILE_NAME),
                icuFile);
        assertContentsMatches(
                new File(expectedZipContentDir, TimeZoneDistro.TZDATA_FILE_NAME),
                tzdataFile);
        assertContentsMatches(
                new File(expectedZipContentDir, TimeZoneDistro.TZLOOKUP_FILE_NAME),
                tzLookupFile);
        assertFileCount(4, expectedZipContentDir);

        // Also check no working directory is left lying around.
        File workingDir = installer.getWorkingDir();
        assertFalse(workingDir.exists());
    }

    private static void assertFileCount(int expectedFiles, File rootDir) throws Exception {
        final List<Path> paths = new ArrayList<>();
        FileVisitor<Path> visitor = new SimpleFileVisitor<Path>() {
            @Override
            public FileVisitResult visitFile(Path filePath, BasicFileAttributes attrs)
                        throws IOException {
                paths.add(filePath);
                return FileVisitResult.CONTINUE;
            }
        };
        Files.walkFileTree(rootDir.toPath(), visitor);
        assertEquals("Found: " + paths, expectedFiles, paths.size());
    }

    private void assertContentsMatches(File expected, File actual) throws IOException {
        byte[] actualBytes = IoUtils.readFileAsByteArray(actual.getPath());
        byte[] expectedBytes = IoUtils.readFileAsByteArray(expected.getPath());
        assertArrayEquals(expectedBytes, actualBytes);
    }

    private void assertNoDistroOperationStaged() throws Exception {
        assertNull(installer.getStagedDistroOperation());

        File stagedTzDataDir = installer.getStagedTzDataDir();
        assertFalse(stagedTzDataDir.exists());

        // Also check no working directories are left lying around.
        File workingDir = installer.getWorkingDir();
        assertFalse(workingDir.exists());

        File oldDataDir = installer.getOldStagedDataDir();
        assertFalse(oldDataDir.exists());
    }

    private void assertDistroUninstallStaged() throws Exception {
        assertEquals(StagedDistroOperation.uninstall(), installer.getStagedDistroOperation());

        File stagedTzDataDir = installer.getStagedTzDataDir();
        assertTrue(stagedTzDataDir.exists());
        assertTrue(stagedTzDataDir.isDirectory());

        File uninstallTombstone =
                new File(stagedTzDataDir, TimeZoneDistroInstaller.UNINSTALL_TOMBSTONE_FILE_NAME);
        assertTrue(uninstallTombstone.exists());
        assertTrue(uninstallTombstone.isFile());

        // Also check no working directories are left lying around.
        File workingDir = installer.getWorkingDir();
        assertFalse(workingDir.exists());

        File oldDataDir = installer.getOldStagedDataDir();
        assertFalse(oldDataDir.exists());
    }

    private void simulateInstalledDistro(byte[] distroBytes) throws Exception {
        File currentTzDataDir = installer.getCurrentTzDataDir();
        assertFalse(currentTzDataDir.exists());
        assertTrue(currentTzDataDir.mkdir());
        new TimeZoneDistro(distroBytes).extractTo(currentTzDataDir);
    }

    private void assertNoInstalledDistro() {
        assertFalse(installer.getCurrentTzDataDir().exists());
    }

    private void assertInstalledDistro(byte[] distroBytes) throws Exception {
        File currentTzDataDir = installer.getCurrentTzDataDir();
        assertTrue(currentTzDataDir.exists());
        File versionFile = new File(currentTzDataDir, TimeZoneDistro.DISTRO_VERSION_FILE_NAME);
        assertTrue(versionFile.exists());
        byte[] expectedVersionBytes = new TimeZoneDistro(distroBytes).getDistroVersion().toBytes();
        byte[] actualVersionBytes = FileUtils.readBytes(versionFile, expectedVersionBytes.length);
        assertArrayEquals(expectedVersionBytes, actualVersionBytes);
    }

    private static byte[] createTzData(String rulesVersion) {
        return new ZoneInfoTestHelper.TzDataBuilder()
                .initializeToValid()
                .setHeaderMagic("tzdata" + rulesVersion)
                .build();
    }

    private static void createFile(File file, byte[] bytes) {
        try (FileOutputStream fos = new FileOutputStream(file)) {
            fos.write(bytes);
        } catch (IOException e) {
            fail(e.getMessage());
        }
    }

    /**
     * Creates a TimeZoneDistro containing arbitrary bytes in the version file. Used for testing
     * distros with badly formed version info.
     */
    private TimeZoneDistro createTimeZoneDistroWithVersionBytes(byte[] versionBytes)
            throws Exception {

        // Extract a valid distro to a working dir.
        byte[] distroBytes = createValidTimeZoneDistroBytes(NEW_RULES_VERSION, 1);
        File workingDir = createUniqueDirectory(tempDir, "versionBytes");
        new TimeZoneDistro(distroBytes).extractTo(workingDir);

        // Modify the version file.
        File versionFile = new File(workingDir, TimeZoneDistro.DISTRO_VERSION_FILE_NAME);
        assertTrue(versionFile.exists());
        try (FileOutputStream fos = new FileOutputStream(versionFile, false /* append */)) {
            fos.write(versionBytes);
        }

        // Zip the distro back up again.
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        try (ZipOutputStream zos = new ZipOutputStream(baos)) {
            Path workingDirPath = workingDir.toPath();
            Files.walkFileTree(workingDirPath, new SimpleFileVisitor<Path>() {
                @Override
                public FileVisitResult visitFile(Path file, BasicFileAttributes attrs)
                        throws IOException {
                    byte[] bytes = IoUtils.readFileAsByteArray(file.toString());
                    String relativeFileName = workingDirPath.relativize(file).toString();
                    addZipEntry(zos, relativeFileName, bytes);
                    return FileVisitResult.CONTINUE;
                }
            });
        }

        return new TimeZoneDistro(baos.toByteArray());
    }

    private static void addZipEntry(ZipOutputStream zos, String name, byte[] content)
            throws IOException {
        ZipEntry zipEntry = new ZipEntry(name);
        zipEntry.setSize(content.length);
        zos.putNextEntry(zipEntry);
        zos.write(content);
        zos.closeEntry();
    }
}
