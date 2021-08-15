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

import com.android.timezone.distro.DistroException;
import com.android.timezone.distro.DistroVersion;
import com.android.timezone.distro.FileUtils;
import com.android.timezone.distro.StagedDistroOperation;
import com.android.timezone.distro.TimeZoneDistro;

import android.annotation.IntDef;
import android.util.Slog;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import libcore.timezone.TzDataSetVersion;
import libcore.timezone.TzDataSetVersion.TzDataSetException;
import libcore.timezone.TimeZoneFinder;
import libcore.timezone.ZoneInfoDB;

/**
 * A distro-validation / extraction class. Separate from the services code that uses it for easier
 * testing. This class is not thread-safe: callers are expected to handle mutual exclusion.
 */
public class TimeZoneDistroInstaller {

    @Retention(RetentionPolicy.SOURCE)
    @IntDef(prefix = { "INSTALL_" }, value = {
            INSTALL_SUCCESS,
            INSTALL_FAIL_BAD_DISTRO_STRUCTURE,
            INSTALL_FAIL_BAD_DISTRO_FORMAT_VERSION,
            INSTALL_FAIL_RULES_TOO_OLD,
            INSTALL_FAIL_VALIDATION_ERROR,
    })
    private @interface InstallResultType {}

    /** {@link #stageInstallWithErrorCode(TimeZoneDistro)} result code: Success. */
    public final static int INSTALL_SUCCESS = 0;

    /** {@link #stageInstallWithErrorCode(TimeZoneDistro)} result code: Distro corrupt. */
    public final static int INSTALL_FAIL_BAD_DISTRO_STRUCTURE = 1;

    /**
     * {@link #stageInstallWithErrorCode(TimeZoneDistro)} result code: Distro version incompatible.
     */
    public final static int INSTALL_FAIL_BAD_DISTRO_FORMAT_VERSION = 2;

    /**
     * {@link #stageInstallWithErrorCode(TimeZoneDistro)} result code: Distro rules too old for
     * device.
     */
    public final static int INSTALL_FAIL_RULES_TOO_OLD = 3;

    /**
     * {@link #stageInstallWithErrorCode(TimeZoneDistro)} result code: Distro content failed
     * validation.
     */
    public final static int INSTALL_FAIL_VALIDATION_ERROR = 4;

    @Retention(RetentionPolicy.SOURCE)
    @IntDef(prefix = { "UNINSTALL_" }, value = {
            UNINSTALL_SUCCESS,
            UNINSTALL_NOTHING_INSTALLED,
            UNINSTALL_FAIL,
    })
    private @interface UninstallResultType {}

    /**
     * {@link #stageUninstall()} result code: An uninstall has been successfully staged.
     */
    public final static int UNINSTALL_SUCCESS = 0;

    /**
     * {@link #stageUninstall()} result code: Nothing was installed that required an uninstall to be
     * staged.
     */
    public final static int UNINSTALL_NOTHING_INSTALLED = 1;

    /**
     * {@link #stageUninstall()} result code: The uninstall could not be staged.
     */
    public final static int UNINSTALL_FAIL = 2;

    // This constant must match one in system/timezone/tzdatacheck/tzdatacheck.cpp.
    private static final String STAGED_TZ_DATA_DIR_NAME = "staged";
    // This constant must match one in system/timezone/tzdatacheck/tzdatacheck.cpp.
    private static final String CURRENT_TZ_DATA_DIR_NAME = "current";
    private static final String WORKING_DIR_NAME = "working";
    private static final String OLD_TZ_DATA_DIR_NAME = "old";

    /**
     * The name of the file in the staged directory used to indicate a staged uninstallation.
     */
    // This constant must match one in system/timezone/tzdatacheck/tzdatacheck.cpp.
    // VisibleForTesting.
    public static final String UNINSTALL_TOMBSTONE_FILE_NAME = "STAGED_UNINSTALL_TOMBSTONE";

    private final String logTag;
    private final File baseVersionFile;
    private final File oldStagedDataDir;
    private final File stagedTzDataDir;
    private final File currentTzDataDir;
    private final File workingDir;

    public TimeZoneDistroInstaller(String logTag, File baseVersionFile, File installDir) {
        this.logTag = logTag;
        this.baseVersionFile = baseVersionFile;
        oldStagedDataDir = new File(installDir, OLD_TZ_DATA_DIR_NAME);
        stagedTzDataDir = new File(installDir, STAGED_TZ_DATA_DIR_NAME);
        currentTzDataDir = new File(installDir, CURRENT_TZ_DATA_DIR_NAME);
        workingDir = new File(installDir, WORKING_DIR_NAME);
    }

    // VisibleForTesting
    File getOldStagedDataDir() {
        return oldStagedDataDir;
    }

    // VisibleForTesting
    File getStagedTzDataDir() {
        return stagedTzDataDir;
    }

    // VisibleForTesting
    File getCurrentTzDataDir() {
        return currentTzDataDir;
    }

    // VisibleForTesting
    File getWorkingDir() {
        return workingDir;
    }

    /**
     * Stage an install of the supplied content, to be installed the next time the device boots.
     *
     * <p>Errors during unpacking or staging will throw an {@link IOException}.
     * Returns {@link #INSTALL_SUCCESS} on success, or one of the failure codes.
     */
    public @InstallResultType int stageInstallWithErrorCode(TimeZoneDistro distro)
            throws IOException {
        if (oldStagedDataDir.exists()) {
            FileUtils.deleteRecursive(oldStagedDataDir);
        }
        if (workingDir.exists()) {
            FileUtils.deleteRecursive(workingDir);
        }

        Slog.i(logTag, "Unpacking / verifying time zone update");
        try {
            unpackDistro(distro, workingDir);

            DistroVersion distroVersion;
            try {
                distroVersion = readDistroVersion(workingDir);
            } catch (DistroException e) {
                Slog.i(logTag, "Invalid distro version: " + e.getMessage());
                return INSTALL_FAIL_BAD_DISTRO_STRUCTURE;
            }
            if (distroVersion == null) {
                Slog.i(logTag, "Update not applied: Distro version could not be loaded");
                return INSTALL_FAIL_BAD_DISTRO_STRUCTURE;
            }

            // The TzDataSetVersion class replaces the DistroVersion class after P. Convert to the
            // new class so we can use the isCompatibleWithThisDevice() method.
            TzDataSetVersion distroTzDataSetVersion;
            try {
                distroTzDataSetVersion = new TzDataSetVersion(
                        distroVersion.formatMajorVersion, distroVersion.formatMinorVersion,
                        distroVersion.rulesVersion, distroVersion.revision);
            } catch (TzDataSetException e) {
                Slog.i(logTag, "Update not applied: Distro version could not be converted", e);
                return INSTALL_FAIL_BAD_DISTRO_STRUCTURE;
            }
            if (!TzDataSetVersion.isCompatibleWithThisDevice(distroTzDataSetVersion)) {
                Slog.i(logTag, "Update not applied: Distro format version check failed: "
                        + distroVersion);
                return INSTALL_FAIL_BAD_DISTRO_FORMAT_VERSION;
            }

            if (!checkDistroDataFilesExist(workingDir)) {
                Slog.i(logTag, "Update not applied: Distro is missing required data file(s)");
                return INSTALL_FAIL_BAD_DISTRO_STRUCTURE;
            }

            if (!checkDistroRulesNewerThanBase(baseVersionFile, distroVersion)) {
                Slog.i(logTag, "Update not applied: Distro rules version check failed");
                return INSTALL_FAIL_RULES_TOO_OLD;
            }

            // Validate the tzdata file.
            File zoneInfoFile = new File(workingDir, TimeZoneDistro.TZDATA_FILE_NAME);
            ZoneInfoDB.TzData tzData = ZoneInfoDB.TzData.loadTzData(zoneInfoFile.getPath());
            if (tzData == null) {
                Slog.i(logTag, "Update not applied: " + zoneInfoFile + " could not be loaded");
                return INSTALL_FAIL_VALIDATION_ERROR;
            }
            try {
                tzData.validate();
            } catch (IOException e) {
                Slog.i(logTag, "Update not applied: " + zoneInfoFile + " failed validation", e);
                return INSTALL_FAIL_VALIDATION_ERROR;
            } finally {
                tzData.close();
            }

            // Validate the tzlookup.xml file.
            File tzLookupFile = new File(workingDir, TimeZoneDistro.TZLOOKUP_FILE_NAME);
            if (!tzLookupFile.exists()) {
                Slog.i(logTag, "Update not applied: " + tzLookupFile + " does not exist");
                return INSTALL_FAIL_BAD_DISTRO_STRUCTURE;
            }
            try {
                TimeZoneFinder timeZoneFinder =
                        TimeZoneFinder.createInstance(tzLookupFile.getPath());
                timeZoneFinder.validate();
            } catch (IOException e) {
                Slog.i(logTag, "Update not applied: " + tzLookupFile + " failed validation", e);
                return INSTALL_FAIL_VALIDATION_ERROR;
            }

            // TODO(nfuller): Add validity checks for ICU data / canarying before applying.
            // http://b/64016752

            Slog.i(logTag, "Applying time zone update");
            FileUtils.makeDirectoryWorldAccessible(workingDir);

            // Check if there is already a staged install or uninstall and remove it if there is.
            if (!stagedTzDataDir.exists()) {
                Slog.i(logTag, "Nothing to unstage at " + stagedTzDataDir);
            } else {
                Slog.i(logTag, "Moving " + stagedTzDataDir + " to " + oldStagedDataDir);
                // Move stagedTzDataDir out of the way in one operation so we can't partially delete
                // the contents.
                FileUtils.rename(stagedTzDataDir, oldStagedDataDir);
            }

            // Move the workingDir to be the new staged directory.
            Slog.i(logTag, "Moving " + workingDir + " to " + stagedTzDataDir);
            FileUtils.rename(workingDir, stagedTzDataDir);
            Slog.i(logTag, "Install staged: " + stagedTzDataDir + " successfully created");
            return INSTALL_SUCCESS;
        } finally {
            deleteBestEffort(oldStagedDataDir);
            deleteBestEffort(workingDir);
        }
    }

    /**
     * Stage an uninstall of the current timezone update in /data which, on reboot, will return the
     * device to using the base data. If there was something else already staged it will be
     * removed by this call.
     *
     * Returns {@link #UNINSTALL_SUCCESS} if staging the uninstallation was
     * successful and reboot will be required. Returns {@link #UNINSTALL_NOTHING_INSTALLED} if
     * there was nothing installed in /data that required an uninstall to be staged, anything that
     * was staged will have been removed and therefore no reboot is required.
     *
     * <p>Errors encountered during uninstallation will throw an {@link IOException}.
     */
    public @UninstallResultType int stageUninstall() throws IOException {
        Slog.i(logTag, "Uninstalling time zone update");

        if (oldStagedDataDir.exists()) {
            // If we can't remove this, an exception is thrown and we don't continue.
            FileUtils.deleteRecursive(oldStagedDataDir);
        }
        if (workingDir.exists()) {
            FileUtils.deleteRecursive(workingDir);
        }

        try {
            // Check if there is already an install or uninstall staged and remove it.
            if (!stagedTzDataDir.exists()) {
                Slog.i(logTag, "Nothing to unstage at " + stagedTzDataDir);
            } else {
                Slog.i(logTag, "Moving " + stagedTzDataDir + " to " + oldStagedDataDir);
                // Move stagedTzDataDir out of the way in one operation so we can't partially delete
                // the contents.
                FileUtils.rename(stagedTzDataDir, oldStagedDataDir);
            }

            // If there's nothing actually installed, there's nothing to uninstall so no need to
            // stage anything.
            if (!currentTzDataDir.exists()) {
                Slog.i(logTag, "Nothing to uninstall at " + currentTzDataDir);
                return UNINSTALL_NOTHING_INSTALLED;
            }

            // Stage an uninstall in workingDir.
            FileUtils.ensureDirectoriesExist(workingDir, true /* makeWorldReadable */);
            FileUtils.createEmptyFile(new File(workingDir, UNINSTALL_TOMBSTONE_FILE_NAME));

            // Move the workingDir to be the new staged directory.
            Slog.i(logTag, "Moving " + workingDir + " to " + stagedTzDataDir);
            FileUtils.rename(workingDir, stagedTzDataDir);
            Slog.i(logTag, "Uninstall staged: " + stagedTzDataDir + " successfully created");

            return UNINSTALL_SUCCESS;
        } finally {
            deleteBestEffort(oldStagedDataDir);
            deleteBestEffort(workingDir);
        }
    }

    /**
     * Reads the currently installed distro version. Returns {@code null} if there is no distro
     * installed.
     *
     * @throws IOException if there was a problem reading data from /data
     * @throws DistroException if there was a problem with the installed distro format/structure
     */
    public DistroVersion getInstalledDistroVersion() throws DistroException, IOException {
        if (!currentTzDataDir.exists()) {
            return null;
        }
        return readDistroVersion(currentTzDataDir);
    }

    /**
     * Reads information about any currently staged distro operation. Returns {@code null} if there
     * is no distro operation staged.
     *
     * @throws IOException if there was a problem reading data from /data
     * @throws DistroException if there was a problem with the staged distro format/structure
     */
    public StagedDistroOperation getStagedDistroOperation() throws DistroException, IOException {
        if (!stagedTzDataDir.exists()) {
            return null;
        }
        if (new File(stagedTzDataDir, UNINSTALL_TOMBSTONE_FILE_NAME).exists()) {
            return StagedDistroOperation.uninstall();
        } else {
            return StagedDistroOperation.install(readDistroVersion(stagedTzDataDir));
        }
    }

    /**
     * Reads the base time zone rules version. i.e. the version that would be present after an
     * installed update is removed.
     *
     * @throws IOException if there was a problem reading data
     */
    public TzDataSetVersion readBaseVersion() throws IOException {
        return readBaseVersion(baseVersionFile);
    }

    private TzDataSetVersion readBaseVersion(File baseVersionFile) throws IOException {
        if (!baseVersionFile.exists()) {
            Slog.i(logTag, "version file cannot be found in " + baseVersionFile);
            throw new FileNotFoundException(
                    "base version file does not exist: " + baseVersionFile);
        }
        try {
            return TzDataSetVersion.readFromFile(baseVersionFile);
        } catch (TzDataSetException e) {
            throw new IOException("Unable to read: " + baseVersionFile, e);
        }
    }

    private void deleteBestEffort(File dir) {
        if (dir.exists()) {
            Slog.i(logTag, "Deleting " + dir);
            try {
                FileUtils.deleteRecursive(dir);
            } catch (IOException e) {
                // Logged but otherwise ignored.
                Slog.w(logTag, "Unable to delete " + dir, e);
            }
        }
    }

    private void unpackDistro(TimeZoneDistro distro, File targetDir) throws IOException {
        Slog.i(logTag, "Unpacking update content to: " + targetDir);
        distro.extractTo(targetDir);
    }

    private boolean checkDistroDataFilesExist(File unpackedContentDir) throws IOException {
        Slog.i(logTag, "Verifying distro contents");
        return FileUtils.filesExist(unpackedContentDir,
                TimeZoneDistro.TZDATA_FILE_NAME,
                TimeZoneDistro.ICU_DATA_FILE_NAME);
    }

    private DistroVersion readDistroVersion(File distroDir) throws DistroException, IOException {
        Slog.d(logTag, "Reading distro format version: " + distroDir);
        File distroVersionFile = new File(distroDir, TimeZoneDistro.DISTRO_VERSION_FILE_NAME);
        if (!distroVersionFile.exists()) {
            throw new DistroException("No distro version file found: " + distroVersionFile);
        }
        byte[] versionBytes =
                FileUtils.readBytes(distroVersionFile, DistroVersion.DISTRO_VERSION_FILE_LENGTH);
        return DistroVersion.fromBytes(versionBytes);
    }

    /**
     * Returns true if the the distro IANA rules version is >= base IANA rules version.
     */
    private boolean checkDistroRulesNewerThanBase(
            File baseVersionFile, DistroVersion distroVersion) throws IOException {

        // We only check the base tz_version file and assume that data like ICU is in sync.
        // There is a CTS test that checks tz_version, ICU and bionic/libcore are in sync.
        Slog.i(logTag, "Reading base time zone rules version");
        TzDataSetVersion baseVersion = readBaseVersion(baseVersionFile);

        String baseRulesVersion = baseVersion.rulesVersion;
        String distroRulesVersion = distroVersion.rulesVersion;
        // canApply = distroRulesVersion >= baseRulesVersion
        boolean canApply = distroRulesVersion.compareTo(baseRulesVersion) >= 0;
        if (!canApply) {
            Slog.i(logTag, "Failed rules version check: distroRulesVersion="
                    + distroRulesVersion + ", baseRulesVersion=" + baseRulesVersion);
        } else {
            Slog.i(logTag, "Passed rules version check: distroRulesVersion="
                    + distroRulesVersion + ", baseRulesVersion=" + baseRulesVersion);
        }
        return canApply;
    }
}
