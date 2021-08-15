/*
 * Copyright (C) 2016, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package android.aidl.tests;

import android.util.Log;
import java.io.PrintWriter;
import java.io.IOException;
import android.content.Context;

public class TestLogger {
    private static final String TAG = "TestServiceClient";
    private PrintWriter mLogFile;

    public TestLogger(Context context) {
        try {
            mLogFile = new PrintWriter(context.openFileOutput(
                    "test-client.log", Context.MODE_PRIVATE));
        } catch (IOException ex) {
            throw new RuntimeException("Failed to open log file for writing.");
        }
    }

    public void log(String line) {
        Log.i(TAG, line);
        mLogFile.println(line);
    }

    public void logAndThrow(String line) throws TestFailException {
        Log.e(TAG, line);
        mLogFile.println(line);
        throw new TestFailException(line);
    }

    public void close() {
        if (mLogFile != null) {
            mLogFile.close();
        }
    }
}
