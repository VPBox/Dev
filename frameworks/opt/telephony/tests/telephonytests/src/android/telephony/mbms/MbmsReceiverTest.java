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

package android.telephony.mbms;

import static org.junit.Assert.assertEquals;

import android.test.suitebuilder.annotation.SmallTest;

import androidx.test.runner.AndroidJUnit4;

import org.junit.Test;
import org.junit.runner.RunWith;

@RunWith(AndroidJUnit4.class)
public class MbmsReceiverTest {
    @Test
    @SmallTest
    public void testFileHierarchyRecreation() throws Exception {
        String rootPath = "http://www.example.com/files/";
        assertEquals("subdir1/file.txt",
                MbmsDownloadReceiver.getFileRelativePath(rootPath, rootPath + "/subdir1/file.txt"));
        assertEquals("subdir1/subdir2/file.txt",
                MbmsDownloadReceiver.getFileRelativePath(
                        rootPath,
                        rootPath + "/subdir1/subdir2/file.txt"));
        assertEquals("file.txt",
                MbmsDownloadReceiver.getFileRelativePath(
                        rootPath + "/subdir1/file.*",
                        rootPath + "/subdir1/file.txt"));
        assertEquals("file.txt",
                MbmsDownloadReceiver.getFileRelativePath(
                        rootPath + "/subdir1/*",
                        rootPath + "/subdir1/file.txt"));
        assertEquals("subdir1/file.txt",
                MbmsDownloadReceiver.getFileRelativePath(
                        rootPath + "/subdir*",
                        rootPath + "/subdir1/file.txt"));
        assertEquals("file.txt",
                MbmsDownloadReceiver.getFileRelativePath(
                        rootPath,
                        rootPath + "/file.txt"));
        assertEquals("file.txt",
                MbmsDownloadReceiver.getFileRelativePath(
                        rootPath + "/*",
                        rootPath + "/file.txt"));
    }
}
