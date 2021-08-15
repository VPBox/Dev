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

package com.android.server.wifi;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.mockito.Matchers.anyString;
import static org.mockito.Matchers.contains;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyZeroInteractions;
import static org.mockito.Mockito.when;

import android.os.FileUtils;

import androidx.test.filters.SmallTest;

import libcore.io.IoUtils;

import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;
import org.mockito.Spy;

import java.io.File;
import java.io.PrintWriter;
import java.io.StringWriter;

/**
 * Unit tests for {@link LastMileLogger}.
 */
@SmallTest
public class LastMileLoggerTest {
    @Mock WifiInjector mWifiInjector;
    @Spy FakeWifiLog mLog;

    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        when(mWifiInjector.makeLog(anyString())).thenReturn(mLog);
        mTraceDataFile = File.createTempFile(TRACE_DATA_PREFIX, null);
        mTraceEnableFile = File.createTempFile(TRACE_ENABLE_PREFIX, null);
        mTraceReleaseFile = File.createTempFile(TRACE_RELEASE_PREFIX, null);
        mTraceDataFile.deleteOnExit();
        mTraceEnableFile.deleteOnExit();
        mTraceReleaseFile.deleteOnExit();
        FileUtils.stringToFile(mTraceEnableFile, "0");
        mLastMileLogger = new LastMileLogger(mWifiInjector, mTraceDataFile.getPath(),
                mTraceEnableFile.getPath(),  mTraceReleaseFile.getPath());
    }

    @Test
    public void ctorDoesNotCrash() throws Exception {
        new LastMileLogger(mWifiInjector, mTraceDataFile.getPath(), mTraceEnableFile.getPath(),
                mTraceReleaseFile.getPath());
        verifyZeroInteractions(mLog);
    }

    @Test
    public void connectionEventStartedEnablesTracing() throws Exception {
        mLastMileLogger.reportConnectionEvent(BaseWifiDiagnostics.CONNECTION_EVENT_STARTED);
        assertEquals("1", IoUtils.readFileAsString(mTraceEnableFile.getPath()));
    }

    @Test
    public void connectionEventStartedDoesNotCrashIfReleaseFileIsMissing() throws Exception {
        mTraceReleaseFile.delete();
        mLastMileLogger.reportConnectionEvent(BaseWifiDiagnostics.CONNECTION_EVENT_STARTED);
        verify(mLog).warn(contains("Failed to open free_buffer"));
    }

    @Test
    public void connectionEventStartedDoesNotEnableTracingIfReleaseFileIsMissing()
            throws Exception {
        mTraceReleaseFile.delete();
        mLastMileLogger.reportConnectionEvent(BaseWifiDiagnostics.CONNECTION_EVENT_STARTED);
        assertEquals("0", IoUtils.readFileAsString(mTraceEnableFile.getPath()));
    }

    @Test
    public void connectionEventStartedDoesNotAttemptToReopenReleaseFile() throws Exception {
        mLastMileLogger.reportConnectionEvent(BaseWifiDiagnostics.CONNECTION_EVENT_STARTED);

        // This is a rather round-about way of verifying that we don't attempt to re-open
        // the file. Namely: if we delete the |release| file, and CONNECTION_EVENT_STARTED
        // _did_ re-open the file, then we'd log an error message. Since the test is deleting the
        // |release| file, the absence of a warning message means that we didn't try to open the
        // file again.
        //
        // A more direct test would require the use of a factory for the creation of the
        // FileInputStream.
        mTraceReleaseFile.delete();
        mLastMileLogger.reportConnectionEvent(BaseWifiDiagnostics.CONNECTION_EVENT_STARTED);
        verifyZeroInteractions(mLog);
    }

    @Test
    public void connectionEventStartedDoesNotCrashIfEnableFileIsMissing() throws Exception {
        mTraceEnableFile.delete();
        mLastMileLogger.reportConnectionEvent(BaseWifiDiagnostics.CONNECTION_EVENT_STARTED);
    }

    @Test
    public void connectionEventStartedDoesNotCrashOnRepeatedCalls() throws Exception {
        mLastMileLogger.reportConnectionEvent(BaseWifiDiagnostics.CONNECTION_EVENT_STARTED);
        mLastMileLogger.reportConnectionEvent(BaseWifiDiagnostics.CONNECTION_EVENT_STARTED);
    }

    @Test
    public void connectionEventSucceededDisablesTracing() throws Exception {
        mLastMileLogger.reportConnectionEvent(BaseWifiDiagnostics.CONNECTION_EVENT_SUCCEEDED);
        assertEquals("0", IoUtils.readFileAsString(mTraceEnableFile.getPath()));
    }

    @Test
    public void connectionEventSucceededDoesNotCrashIfEnableFileIsMissing() throws Exception {
        mTraceEnableFile.delete();
        mLastMileLogger.reportConnectionEvent(BaseWifiDiagnostics.CONNECTION_EVENT_SUCCEEDED);
    }

    @Test
    public void connectionEventSucceededDoesNotCrashOnRepeatedCalls() throws Exception {
        mLastMileLogger.reportConnectionEvent(BaseWifiDiagnostics.CONNECTION_EVENT_SUCCEEDED);
        mLastMileLogger.reportConnectionEvent(BaseWifiDiagnostics.CONNECTION_EVENT_SUCCEEDED);
    }

    @Test
    public void connectionEventFailedDisablesTracingWhenPendingFails() throws Exception {
        mLastMileLogger.reportConnectionEvent(BaseWifiDiagnostics.CONNECTION_EVENT_STARTED);
        mLastMileLogger.reportConnectionEvent(BaseWifiDiagnostics.CONNECTION_EVENT_FAILED);
        assertEquals("0", IoUtils.readFileAsString(mTraceEnableFile.getPath()));
    }

    @Test
    public void connectionEventTimeoutDisablesTracing()
            throws Exception {
        mLastMileLogger.reportConnectionEvent(BaseWifiDiagnostics.CONNECTION_EVENT_STARTED);
        mLastMileLogger.reportConnectionEvent(BaseWifiDiagnostics.CONNECTION_EVENT_TIMEOUT);
        assertEquals("0", IoUtils.readFileAsString(mTraceEnableFile.getPath()));
    }

    @Test
    public void connectionEventFailedDoesNotCrashIfEnableFileIsMissing() throws Exception {
        mTraceEnableFile.delete();
        mLastMileLogger.reportConnectionEvent(BaseWifiDiagnostics.CONNECTION_EVENT_FAILED);
    }

    @Test
    public void connectionEventFailedDoesNotCrashIfDataFileIsMissing() throws Exception {
        mTraceDataFile.delete();
        mLastMileLogger.reportConnectionEvent(BaseWifiDiagnostics.CONNECTION_EVENT_FAILED);
    }

    @Test
    public void connectionEventFailedDoesNotCrashOnRepeatedCalls() throws Exception {
        mLastMileLogger.reportConnectionEvent(BaseWifiDiagnostics.CONNECTION_EVENT_FAILED);
        mLastMileLogger.reportConnectionEvent(BaseWifiDiagnostics.CONNECTION_EVENT_FAILED);
    }

    @Test
    public void dumpShowsFailureTrace() throws Exception {
        mLastMileLogger.reportConnectionEvent(BaseWifiDiagnostics.CONNECTION_EVENT_STARTED);
        FileUtils.stringToFile(mTraceDataFile.getPath(), "rdev_connect");
        mLastMileLogger.reportConnectionEvent(BaseWifiDiagnostics.CONNECTION_EVENT_FAILED);
        assertTrue(getDumpString().contains("--- Last failed"));
        assertTrue(getDumpString().contains("rdev_connect"));
    }


    @Test
    public void dumpShowsPendingConnectionTrace() throws Exception {
        mLastMileLogger.reportConnectionEvent(BaseWifiDiagnostics.CONNECTION_EVENT_STARTED);
        FileUtils.stringToFile(mTraceDataFile.getPath(), "rdev_connect");
        assertTrue(getDumpString().contains("No last mile log for \"Last failed"));
        assertTrue(getDumpString().contains("--- Latest"));
        assertTrue(getDumpString().contains("rdev_connect"));
    }

    @Test
    public void dumpShowsLastFailureTraceAndPendingConnectionTrace() throws Exception {
        mLastMileLogger.reportConnectionEvent(BaseWifiDiagnostics.CONNECTION_EVENT_STARTED);
        FileUtils.stringToFile(mTraceDataFile.getPath(), "rdev_connect try #1");
        mLastMileLogger.reportConnectionEvent(BaseWifiDiagnostics.CONNECTION_EVENT_FAILED);
        mLastMileLogger.reportConnectionEvent(BaseWifiDiagnostics.CONNECTION_EVENT_STARTED);
        FileUtils.stringToFile(mTraceDataFile.getPath(), "rdev_connect try #2");

        String dumpString = getDumpString();
        assertTrue(dumpString.contains("rdev_connect try #1"));
        assertTrue(dumpString.contains("rdev_connect try #2"));
    }

    @Test
    public void dumpShowsLastFailureTraceAndCurrentConnectionTrace() throws Exception {
        mLastMileLogger.reportConnectionEvent(BaseWifiDiagnostics.CONNECTION_EVENT_STARTED);
        FileUtils.stringToFile(mTraceDataFile.getPath(), "rdev_connect try #1");
        mLastMileLogger.reportConnectionEvent(BaseWifiDiagnostics.CONNECTION_EVENT_FAILED);
        mLastMileLogger.reportConnectionEvent(BaseWifiDiagnostics.CONNECTION_EVENT_STARTED);
        FileUtils.stringToFile(mTraceDataFile.getPath(), "rdev_connect try #2");
        mLastMileLogger.reportConnectionEvent(BaseWifiDiagnostics.CONNECTION_EVENT_SUCCEEDED);

        String dumpString = getDumpString();
        assertTrue(dumpString.contains("rdev_connect try #1"));
        assertTrue(dumpString.contains("rdev_connect try #2"));
    }

    @Test
    public void dumpDoesNotClearLastFailureData() throws Exception {
        mLastMileLogger.reportConnectionEvent(BaseWifiDiagnostics.CONNECTION_EVENT_STARTED);
        FileUtils.stringToFile(mTraceDataFile.getPath(), "rdev_connect");
        mLastMileLogger.reportConnectionEvent(BaseWifiDiagnostics.CONNECTION_EVENT_FAILED);

        getDumpString();
        String dumpString = getDumpString();
        assertTrue(dumpString.contains("rdev_connect"));
    }

    @Test
    public void dumpDoesNotClearPendingConnectionTrace() throws Exception {
        mLastMileLogger.reportConnectionEvent(BaseWifiDiagnostics.CONNECTION_EVENT_STARTED);
        FileUtils.stringToFile(mTraceDataFile.getPath(), "rdev_connect");

        getDumpString();
        String dumpString = getDumpString();
        assertTrue(dumpString.contains("rdev_connect"));
    }

    @Test
    public void dumpDoesNotCrashIfDataFileIsEmpty() throws Exception {
        getDumpString();
    }

    @Test
    public void dumpDoesNotCrashIfDataFileIsMissing() throws Exception {
        mTraceDataFile.delete();
        getDumpString();
    }

    private static final String TRACE_DATA_PREFIX = "last-mile-logger-trace-data";
    private static final String TRACE_ENABLE_PREFIX = "last-mile-logger-trace-enable";
    private static final String TRACE_RELEASE_PREFIX = "last-mile-logger-trace-release";
    private LastMileLogger mLastMileLogger;
    private File mTraceDataFile;
    private File mTraceEnableFile;
    private File mTraceReleaseFile;

    private String getDumpString() {
        StringWriter sw = new StringWriter();
        PrintWriter pw = new PrintWriter(sw);
        mLastMileLogger.dump(pw);
        return sw.toString();
    }
}
