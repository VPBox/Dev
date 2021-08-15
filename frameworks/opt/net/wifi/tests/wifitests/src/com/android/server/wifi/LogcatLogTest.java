/*
 * Copyright (C) 2016 The Android Open Source Project
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

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.instanceOf;
import static org.junit.Assert.assertEquals;

import androidx.test.filters.SmallTest;

import org.junit.Before;
import org.junit.Test;

/**
 * Unit tests for {@link LogcatLog}.
 */
@SmallTest
public class LogcatLogTest {
    private static final String TAG = "LogcatLogTest";
    private LogcatLog mLogger;

    /** Initializes test fixture. */
    @Before
    public void setUp() {
        mLogger = new LogcatLog(TAG);
    }

    /**
     * Verifies that LogcatLog's LogMessage implementation correctly
     * handles a format with no parameters.
     *
     * Note: In practice, we expect clients to use eC() and friends
     * when the message is a literal. But we still want to make sure
     * this functionality works.
     */
    @Test
    public void logMessageWorksWithParameterlessFormat() {
        WifiLog.LogMessage logMessage = mLogger.err("hello world");
        logMessage.flush();
        assertEquals("hello world", logMessage.toString());
    }

    /** Verifies that LogMessage works with an empty format. */
    @Test
    public void logMessageWorksWithEmptyFormat() {
        WifiLog.LogMessage logMessage = mLogger.err("");
        logMessage.flush();
        assertEquals("", logMessage.toString());
    }

    /** Verifies that LogMessage works with a value-only format. */
    @Test
    public void logMessageWorksWithValueOnly() {
        WifiLog.LogMessage logMessage = mLogger.err("%");
        logMessage.c(1).flush();
        assertEquals("1", logMessage.toString());
    }

    /**
     * Verifies that LogMessage works when the placeholder is replaced
     * by the placeholder character.
     */
    @Test
    public void logMessageIsNotConfusedByPlaceholderInValue() {
        WifiLog.LogMessage logMessage = mLogger.err("%");
        logMessage.c('%').flush();
        assertEquals("%", logMessage.toString());
    }

    /** Verifies that LogMessage works when a value is at the start of the format. */
    @Test
    public void logMessageWorksWithValueAtBegin() {
        WifiLog.LogMessage logMessage = mLogger.err("%stuff");
        logMessage.c(1).flush();
        assertEquals("1stuff", logMessage.toString());
    }

    /** Verifies that LogMessage works when a value is in the middle of the format. */
    @Test
    public void logMessageWorksWithValueInMiddle() {
        WifiLog.LogMessage logMessage = mLogger.err("s%uff");
        logMessage.c(1).flush();
        assertEquals("s1uff", logMessage.toString());
    }

    /** Verifies that LogMessage works when a value is at the end of the format. */
    @Test
    public void logMessageWorksWithValueAtEnd() {
        WifiLog.LogMessage logMessage = mLogger.err("stuff%");
        logMessage.c(1).flush();
        assertEquals("stuff1", logMessage.toString());
    }

    /** Verifies that LogMessage works when a format has multiple values. */
    @Test
    public void logMessageWorksWithMultipleValues() {
        WifiLog.LogMessage logMessage = mLogger.err("% %");
        logMessage.c("hello").c("world").flush();
        assertEquals("hello world", logMessage.toString());
    }

    /** Verifies that LogMessage works when a format has multiple values and literals. */
    @Test
    public void logMessageWorksWithMultipleValuesAndLiterals() {
        WifiLog.LogMessage logMessage = mLogger.err("first:% second:%");
        logMessage.c("hello").c("world").flush();
        assertEquals("first:hello second:world", logMessage.toString());
    }

    /** Verifies that LogMessage works when a format has multiple adjacent values. */
    @Test
    public void logMessageWorksWithAdjacentValues() {
        WifiLog.LogMessage logMessage = mLogger.err("%%");
        logMessage.c("hello").c("world").flush();
        assertEquals("helloworld", logMessage.toString());
    }

    /** Verifies that LogMessage silently ignores extraneous values. */
    @Test
    public void logMessageSilentlyIgnoresExtraneousValues() {
        WifiLog.LogMessage logMessage = mLogger.err("%");
        logMessage.c("hello world").c("more stuff").flush();
        assertEquals("hello world", logMessage.toString());
    }

    /**
     * Verifies that LogMessage silently ignores extraneous values,
     * even with an empty format string.
     */
    @Test
    public void logMessageSilentlyIgnoresExtraneousValuesEvenForEmptyFormat() {
        WifiLog.LogMessage logMessage = mLogger.err("");
        logMessage.c("hello world").c("more stuff").flush();
        assertEquals("", logMessage.toString());
    }

    /**
     * Verifies that LogMessage silently ignores extraneous values,
     * even if the format string is all literals.
     */
    @Test
    public void logMessageSilentlyIgnoresExtraneousValuesEvenForFormatWithoutPlaceholders() {
        WifiLog.LogMessage logMessage = mLogger.err("literal format");
        logMessage.c("hello world").c("more stuff").flush();
        assertEquals("literal format", logMessage.toString());
    }

    /** Verifies that LogMessage copies an unused placeholder to output. */
    @Test
    public void logMessageCopiesUnusedPlaceholderToOutput() {
        WifiLog.LogMessage logMessage = mLogger.err("%");
        logMessage.flush();
        assertEquals("%", logMessage.toString());
    }

    /** Verifies that LogMessage copies multiple unused placeholders to output. */
    @Test
    public void logMessageCopiesMultipleUnusedPlaceholdersToOutput() {
        WifiLog.LogMessage logMessage = mLogger.err("%%%%%");
        logMessage.flush();
        assertEquals("%%%%%", logMessage.toString());
    }

    /**
     * Verifies that LogMessage copies an unused placeholder to output,
     * even if preceded by non-placeholders.
     */
    @Test
    public void logMessageCopiesUnusedPlaceholderAtEndToOutput() {
        WifiLog.LogMessage logMessage = mLogger.err("foo%");
        logMessage.flush();
        assertEquals("foo%", logMessage.toString());
    }

    /**
     * Verifies that LogMessage copies an unused placeholder to output,
     * even if followed by non-placeholders.
     */
    @Test
    public void logMessageCopiesUnusedPlaceholderAtBeginToOutput() {
        WifiLog.LogMessage logMessage = mLogger.err("%foo");
        logMessage.flush();
        assertEquals("%foo", logMessage.toString());
    }

    /**
     * Verifies that LogMessage copies an unused placeholder to output,
     * even if it is in the middle of non-placeholders.
     */
    @Test
    public void logMessageCopiesUnusedPlaceholderInMiddleToOutput() {
        WifiLog.LogMessage logMessage = mLogger.err("f%o");
        logMessage.flush();
        assertEquals("f%o", logMessage.toString());
    }

    /**
     * Verifies that LogMessage copies multiple unused placeholders to output,
     * even if they are embedded amongst non-placeholders.
     */
    @Test
    public void logMessageCopiesUnusedPlaceholdersInMiddleToOutput() {
        WifiLog.LogMessage logMessage = mLogger.err("f%o%o%d");
        logMessage.flush();
        assertEquals("f%o%o%d", logMessage.toString());
    }

    /**
     * Verifies that LogMessage preserves meta-characters in format string.
     *
     * Note that we deliberately test only the meta-characters that we
     * expect to find in log messages. (Newline might also be
     * preserved, but clients shouldn't depend on that, as messages
     * that have newlines make logs hard to read.)
     */
    @Test
    public void logMessagePreservesMetaCharactersInFormat() {
        WifiLog.LogMessage logMessage = mLogger.err("\\hello\tworld\\");
        logMessage.flush();
        assertEquals("\\hello\tworld\\", logMessage.toString());
    }

    /** Verifies that LogMessage propagates meta-characters in char values. */
    @Test
    public void logMessagePropagatesMetaCharactersInCharValues() {
        WifiLog.LogMessage logMessage = mLogger.err("hello%big%world");
        logMessage.c('\t').c('\\').flush();
        assertEquals("hello\tbig\\world", logMessage.toString());
    }

    /** Verifies that LogMessage propagates meta-characters in String values. */
    @Test
    public void logMessagePropagatesMetaCharactersInStringValues() {
        WifiLog.LogMessage logMessage = mLogger.err("%%world");
        logMessage.c("hello\t").c("big\\").flush();
        assertEquals("hello\tbig\\world", logMessage.toString());
    }

    @Test
    public void traceLogMessageIncludesCallerName() {
        try {
            LogcatLog.enableVerboseLogging(1);
            WifiLog.LogMessage logMessage = mLogger.trace("%");
            logMessage.c("says hello").flush();
            assertEquals("traceLogMessageIncludesCallerName says hello",
                    logMessage.toString());
        } finally {
            LogcatLog.enableVerboseLogging(0);
        }
    }

    @Test
    public void traceLogMessageRespectsNumFramesToIgnore() {
        try {
            LogcatLog.enableVerboseLogging(1);
            WifiLog.LogMessage logMessage = traceHelper("%");
            logMessage.c("says hello").flush();
            assertEquals("traceLogMessageRespectsNumFramesToIgnore says hello",
                    logMessage.toString());
        } finally {
            LogcatLog.enableVerboseLogging(0);
        }
    }

    @Test
    public void traceLogMessageDoesNotCrashOnOversizedNumFramesToIgnore() {
        try {
            LogcatLog.enableVerboseLogging(1);
            WifiLog.LogMessage logMessage = mLogger.trace("%",
                    (new Throwable()).getStackTrace().length);
            logMessage.c("says hello").flush();
            assertEquals("<unknown> says hello", logMessage.toString());
        } finally {
            LogcatLog.enableVerboseLogging(0);
        }
    }

    @Test
    public void traceLogMessageDoesNotCrashOnOverflowingNumFramesToIgnore() {
        try {
            LogcatLog.enableVerboseLogging(1);
            WifiLog.LogMessage logMessage = mLogger.trace("%", Integer.MAX_VALUE);
            logMessage.c("says hello").flush();
            assertEquals("<unknown> says hello", logMessage.toString());
        } finally {
            LogcatLog.enableVerboseLogging(0);
        }
    }

    @Test
    public void traceLogMessageDoesNotCrashOnUndersizedNumFramesToIgnore() {
        try {
            LogcatLog.enableVerboseLogging(1);
            WifiLog.LogMessage logMessage = mLogger.trace("%", Integer.MIN_VALUE);
            logMessage.c("says hello").flush();
            assertEquals("<unknown> says hello", logMessage.toString());
        } finally {
            LogcatLog.enableVerboseLogging(0);
        }
    }

    @Test
    public void traceLogMessageReturnsDummyLogMessageByDefault() {
        assertThat(mLogger.trace("%"), instanceOf(DummyLogMessage.class));
    }

    @Test
    public void dumpLogMessageReturnsDummyLogMessageByDefault() {
        assertThat(mLogger.dump("%"), instanceOf(DummyLogMessage.class));
    }

    private WifiLog.LogMessage traceHelper(String format) {
        return mLogger.trace(format, 1);
    }
}
