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
package com.android.timezone.distro;

import junit.framework.TestCase;

import android.system.Os;
import android.system.OsConstants;
import android.system.StructStat;

import libcore.testing.io.TestIoUtils;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

/**
 * Tests for {@link FileUtils}.
 */
public class FileUtilsTest extends TestCase {

    private List<File> testFiles = new ArrayList<>();

    @Override
    public void tearDown() throws Exception {
        // Delete in reverse order
        Collections.reverse(testFiles);
        for (File tempFile : testFiles) {
            tempFile.delete();
        }
        super.tearDown();
    }

    public void testDeleteRecursive() throws Exception {
        File dir = createTempDir();
        File file1 = createRegularFile(dir, "file1");
        File file2 = createRegularFile(dir, "file2");
        File symLink1 = createSymlink(file1, dir, "symLink1");
        File subDir = createDir(dir, "subDir");
        File file3 = createRegularFile(subDir, "subFile1");
        File file4 = createRegularFile(subDir, "subFile2");
        File symLink2 = createSymlink(file1, dir, "symLink2");

        File otherDir = createTempDir();
        File otherFile = createRegularFile(otherDir, "kept");

        File linkToOtherDir = createSymlink(otherDir, subDir, "linkToOtherDir");
        File linkToOtherFile = createSymlink(otherFile, subDir, "linkToOtherFile");

        File[] filesToDelete = { dir, file1, file2, symLink1, subDir, file3, file4, symLink2,
                linkToOtherDir, linkToOtherFile };
        File[] filesToKeep = { otherDir, otherFile };
        assertFilesExist(filesToDelete);
        assertFilesExist(filesToKeep);

        FileUtils.deleteRecursive(dir);
        assertFilesDoNotExist(filesToDelete);
        assertFilesExist(filesToKeep);
    }

    public void testIsSymlink() throws Exception {
        File dir = createTempDir();
        File subDir = createDir(dir, "subDir");
        File fileInSubDir = createRegularFile(subDir, "fileInSubDir");
        File normalFile = createRegularFile(dir, "normalFile");
        File symlinkToDir = createSymlink(subDir, dir, "symlinkToDir");
        File symlinkToFile = createSymlink(fileInSubDir, dir, "symlinkToFile");
        File symlinkToFileInSubDir = createSymlink(fileInSubDir, dir, "symlinkToFileInSubDir");
        File normalFileViaSymlink = new File(symlinkToDir, "normalFile");

        assertFalse(FileUtils.isSymlink(dir));
        assertFalse(FileUtils.isSymlink(subDir));
        assertFalse(FileUtils.isSymlink(fileInSubDir));
        assertFalse(FileUtils.isSymlink(normalFile));
        assertTrue(FileUtils.isSymlink(symlinkToDir));
        assertTrue(FileUtils.isSymlink(symlinkToFile));
        assertTrue(FileUtils.isSymlink(symlinkToFileInSubDir));
        assertFalse(FileUtils.isSymlink(normalFileViaSymlink));
    }

    public void testCreateSubFile() throws Exception {
        File dir1 = createTempDir().getCanonicalFile();

        File actualSubFile = FileUtils.createSubFile(dir1, "file");
        assertEquals(new File(dir1, "file"), actualSubFile);

        File existingSubFile = createRegularFile(dir1, "file");
        actualSubFile = FileUtils.createSubFile(dir1, "file");
        assertEquals(existingSubFile, actualSubFile);

        File existingSubDir = createDir(dir1, "subdir");
        actualSubFile = FileUtils.createSubFile(dir1, "subdir");
        assertEquals(existingSubDir, actualSubFile);

        assertCreateSubFileThrows(dir1, "../file");
        assertCreateSubFileThrows(dir1, "../../file");
        assertCreateSubFileThrows(dir1, "../otherdir/file");

        File dir2 = createTempDir().getCanonicalFile();
        createSymlink(dir2, dir1, "symlinkToDir2");
        assertCreateSubFileThrows(dir1, "symlinkToDir2");

        assertCreateSubFileThrows(dir1, "symlinkToDir2/fileInSymlinkedDir");

        createRegularFile(dir1, "symlinkToDir2/fileInSymlinkedDir");
        assertCreateSubFileThrows(dir1, "symlinkToDir2/fileInSymlinkedDir");
    }

    public void testEnsureDirectoryExists() throws Exception {
        File dir = createTempDir();

        File exists = new File(dir, "exists");
        assertTrue(exists.mkdir());
        assertTrue(exists.setReadable(true /* readable */, true /* ownerOnly */));
        assertTrue(exists.setExecutable(true /* readable */, true /* ownerOnly */));
        FileUtils.ensureDirectoriesExist(exists, true /* makeWorldReadable */);
        assertDirExistsAndIsAccessible(exists, false /* requireWorldReadable */);

        File subDir = new File(dir, "subDir");
        assertFalse(subDir.exists());
        FileUtils.ensureDirectoriesExist(subDir, true /* makeWorldReadable */);
        assertDirExistsAndIsAccessible(subDir, true /* requireWorldReadable */);

        File one = new File(dir, "one");
        File two = new File(one, "two");
        File three = new File(two, "three");
        FileUtils.ensureDirectoriesExist(three, true /* makeWorldReadable */);
        assertDirExistsAndIsAccessible(one, true /* requireWorldReadable */);
        assertDirExistsAndIsAccessible(two, true /* requireWorldReadable */);
        assertDirExistsAndIsAccessible(three, true /* requireWorldReadable */);
    }

    public void testEnsureDirectoriesExist_noPermissions() throws Exception {
        File dir = createTempDir();
        assertDirExistsAndIsAccessible(dir, false /* requireWorldReadable */);

        File unreadableSubDir = new File(dir, "unreadableSubDir");
        assertTrue(unreadableSubDir.mkdir());
        assertTrue(unreadableSubDir.setReadable(false /* readable */, true /* ownerOnly */));
        assertTrue(unreadableSubDir.setExecutable(false /* readable */, true /* ownerOnly */));

        File toCreate = new File(unreadableSubDir, "toCreate");
        try {
            FileUtils.ensureDirectoriesExist(toCreate, true /* makeWorldReadable */);
            fail();
        } catch (IOException expected) {
        }
        assertDirExistsAndIsAccessible(dir, false /* requireWorldReadable */);
        assertFalse(unreadableSubDir.canRead() && unreadableSubDir.canExecute());
        assertFalse(toCreate.exists());
    }

    public void testEnsureFileDoesNotExist() throws Exception {
        File dir = createTempDir();

        FileUtils.ensureFileDoesNotExist(new File(dir, "doesNotExist"));

        File exists1 = createRegularFile(dir, "exists1");
        assertTrue(exists1.exists());
        FileUtils.ensureFileDoesNotExist(exists1);
        assertFalse(exists1.exists());

        exists1 = createRegularFile(dir, "exists1");
        File symlink = createSymlink(exists1, dir, "symlinkToFile");
        assertTrue(symlink.exists());
        FileUtils.ensureFileDoesNotExist(symlink);
        assertFalse(symlink.exists());
        assertTrue(exists1.exists());

        // Only files and symlinks supported. We do not delete directories.
        File emptyDir = createTempDir();
        try {
            FileUtils.ensureFileDoesNotExist(emptyDir);
            fail();
        } catch (IOException expected) {
        }
        assertTrue(emptyDir.exists());
    }

    // This test does not pass when run as root because root can do anything even if the permissions
    // don't allow it.
    public void testEnsureFileDoesNotExist_noPermission() throws Exception {
        File dir = createTempDir();

        File protectedDir = createDir(dir, "protected");
        File undeletable = createRegularFile(protectedDir, "undeletable");
        assertTrue(protectedDir.setWritable(false));
        assertTrue(undeletable.exists());
        try {
            FileUtils.ensureFileDoesNotExist(undeletable);
            fail();
        } catch (IOException expected) {
        } finally {
            assertTrue(protectedDir.setWritable(true)); // Reset for clean-up
        }
        assertTrue(undeletable.exists());
    }

    public void testCheckFilesExist() throws Exception {
        File dir = createTempDir();
        createRegularFile(dir, "exists1");
        File subDir = createDir(dir, "subDir");
        createRegularFile(subDir, "exists2");
        assertTrue(FileUtils.filesExist(dir, "exists1", "subDir/exists2"));
        assertFalse(FileUtils.filesExist(dir, "doesNotExist"));
        assertFalse(FileUtils.filesExist(dir, "subDir/doesNotExist"));
    }

    public void testReadBytes() throws Exception {
        File dir = createTempDir();

        try {
            FileUtils.readBytes(new File(dir, "doesNotExist"), 0);
            fail();
        } catch (IllegalArgumentException expected) {
        }

        try {
            FileUtils.readBytes(new File(dir, "doesNotExist"), 10);
            fail();
        } catch (FileNotFoundException expected) {
        }

        byte[] contents = "One\nTwo\nThree\n".getBytes(StandardCharsets.US_ASCII);
        File file = createFile(contents);
        byte[] oneByte = FileUtils.readBytes(file, 1);
        assertEquals(1, oneByte.length);
        assertEquals(contents[0], oneByte[0]);

        byte[] allBytes = FileUtils.readBytes(file, contents.length);
        assertTrue(Arrays.equals(contents, allBytes));

        byte[] exhaustedFileRead = FileUtils.readBytes(file, contents.length + 1);
        assertTrue(Arrays.equals(contents, exhaustedFileRead));
    }

    public void testCreateEmptyFile() throws Exception {
        File dir = createTempDir();
        File file = new File(dir, "one");
        assertFalse(file.exists());
        FileUtils.createEmptyFile(file);
        assertTrue(file.exists());
        assertEquals(0, file.length());
    }

    public void testCreateEmptyFile_isDir() throws Exception {
        File dir = createTempDir();
        assertTrue(dir.exists());
        assertTrue(dir.isDirectory());

        try {
            FileUtils.createEmptyFile(dir);
        } catch (FileNotFoundException expected) {
        }
        assertTrue(dir.exists());
        assertTrue(dir.isDirectory());
    }

    public void testCreateEmptyFile_truncatesExisting() throws Exception {
        File dir = createTempDir();
        File file = new File(dir, "one");

        try (FileOutputStream fos = new FileOutputStream(file)) {
            fos.write(new byte[1000]);
        }
        assertTrue(file.exists());
        assertEquals(1000, file.length());

        FileUtils.createEmptyFile(file);
        assertTrue(file.exists());
        assertEquals(0, file.length());
    }

    private File createFile(byte[] contents) throws IOException {
        File file = File.createTempFile(getClass().getSimpleName(), ".txt");
        try (FileOutputStream fos = new FileOutputStream(file)) {
            fos.write(contents);
        }
        return file;
    }

    private File createSymlink(File file, File symlinkDir, String symlinkName) throws Exception {
        assertTrue(file.exists());

        File symlink = new File(symlinkDir, symlinkName);
        Os.symlink(file.getAbsolutePath(), symlink.getAbsolutePath());
        testFiles.add(symlink);
        return symlink;
    }

    private static void assertCreateSubFileThrows(File parentDir, String name) {
        try {
            FileUtils.createSubFile(parentDir, name);
            fail();
        } catch (IOException expected) {
            assertTrue(expected.getMessage().contains("must exist beneath"));
        }
    }

    private static void assertFilesDoNotExist(File... files) {
        for (File f : files) {
            assertFalse(f + " unexpectedly exists", f.exists());
        }
    }

    private static void assertFilesExist(File... files) {
        for (File f : files) {
            assertTrue(f + " expected to exist", f.exists());
        }
    }

    private static void assertDirExistsAndIsAccessible(File dir, boolean requireWorldReadable)
            throws Exception {
        assertTrue(dir.exists() && dir.isDirectory() && dir.canRead() && dir.canExecute());

        String path = dir.getCanonicalPath();
        StructStat sb = Os.stat(path);
        int mask = OsConstants.S_IXUSR | OsConstants.S_IRUSR;
        if (requireWorldReadable) {
            mask = mask | OsConstants.S_IXGRP | OsConstants.S_IRGRP
                    | OsConstants.S_IXOTH | OsConstants.S_IROTH;
        }
        assertTrue("Permission mask required: " + Integer.toOctalString(mask),
                (sb.st_mode & mask) == mask);
    }

    private File createTempDir() {
        final String tempPrefix = getClass().getSimpleName();
        File tempDir = TestIoUtils.createTemporaryDirectory(tempPrefix);
        testFiles.add(tempDir);
        return tempDir;
    }

    private File createDir(File parentDir, String name) {
        File dir = new File(parentDir, name);
        assertTrue(dir.mkdir());
        testFiles.add(dir);
        return dir;
    }

    private File createRegularFile(File dir, String name) throws Exception {
        File file = new File(dir, name);
        try (FileOutputStream fos = new FileOutputStream(file)) {
            fos.write("Hello".getBytes());
        }
        testFiles.add(file);
        return file;
    }
}
