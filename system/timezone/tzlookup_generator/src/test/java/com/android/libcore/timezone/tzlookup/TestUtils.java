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

package com.android.libcore.timezone.tzlookup;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.FileVisitResult;
import java.nio.file.FileVisitor;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.SimpleFileVisitor;
import java.nio.file.attribute.BasicFileAttributes;
import java.util.Arrays;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

/**
 * Arbitrary static utility methods to help with testing.
 */
class TestUtils {

    private TestUtils() {}

    static String createFile(Path dir, String... lines) throws IOException {
        Path tempFile = Files.createTempFile(dir, "tmp", null /* suffix */);
        Files.write(tempFile, Arrays.asList(lines), StandardCharsets.US_ASCII);
        return tempFile.toString();
    }

    static void deleteDir(Path tempDir) throws IOException {
        FileVisitor<? super Path> deleter = new SimpleFileVisitor<Path>() {
            @Override
            public FileVisitResult visitFile(Path file, BasicFileAttributes attrs)
                    throws IOException {
                return delete(file);
            }

            @Override
            public FileVisitResult postVisitDirectory(Path dir, IOException exc)
                    throws IOException {
                return delete(dir);
            }

            private FileVisitResult delete(Path file) throws IOException {
                Files.delete(file);
                return FileVisitResult.CONTINUE;
            }
        };
        Files.walkFileTree(tempDir, deleter);
    }

    static void assertAbsent(String s, String... absents) {
        for (String absent : absents) {
            assertFalse(s + " must not contain " + absent, s.contains(absent));
        }
    }

    static void assertContains(String s, String... expecteds) {
        for (String expected : expecteds) {
            assertTrue(s + " must contain " + expected, s.contains(expected));
        }
    }
}
