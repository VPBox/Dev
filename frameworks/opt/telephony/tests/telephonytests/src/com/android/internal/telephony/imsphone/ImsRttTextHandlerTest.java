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
 * limitations under the License
 */

package com.android.internal.telephony.imsphone;

import android.os.HandlerThread;
import android.os.ParcelFileDescriptor;
import android.telecom.Connection;

import androidx.test.filters.FlakyTest;

import com.android.internal.telephony.TelephonyTest;

import org.junit.After;
import org.junit.Assert;
import org.junit.Before;
import org.junit.ComparisonFailure;
import org.junit.Test;

import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

public class ImsRttTextHandlerTest extends TelephonyTest {
    private static final int TEST_TIMEOUT = 1000;
    private static final int READ_BUFFER_SIZE = 1000;
    private static final String LONG_TEXT = "No Soldier shall, in time of peace be quartered in " +
            "any house, without the consent of the Owner, nor in time of war, but in a manner to " +
            "be prescribed by law.";

    char[] buffer = new char[READ_BUFFER_SIZE];

    public class MockNetworkWriter implements ImsRttTextHandler.NetworkWriter {
        private String totalWritten = "";
        private int numWrites = 0;

        @Override
        public synchronized void write(String s) {
            totalWritten += s;
            numWrites += 1;
        }

        public synchronized void reset() {
            totalWritten = "";
            numWrites = 0;
        }

        public synchronized String getContents() {
            return totalWritten;
        }

        public synchronized int getNumWrites() {
            return numWrites;
        }
    }

    Connection.RttTextStream mRttTextStream;
    MockNetworkWriter mNetworkWriter = new MockNetworkWriter();
    ImsRttTextHandler mRttTextHandler;
    HandlerThread mHandlerThread;

    OutputStreamWriter mPipeToHandler;
    InputStreamReader mPipeFromHandler;
    InputStreamReader mHandlerSideOfPipeToHandler;

    @Before
    public void setUp() throws Exception {
        super.setUp(getClass().getSimpleName());
        mNetworkWriter.reset();
        mHandlerThread = new HandlerThread("TestImsRttTextHandler");
        mHandlerThread.start();
        mRttTextHandler = new ImsRttTextHandler(mHandlerThread.getLooper(), mNetworkWriter);

        // Construct some pipes to use
        ParcelFileDescriptor[] toTextHandler = ParcelFileDescriptor.createReliablePipe();
        ParcelFileDescriptor[] fromTextHandler = ParcelFileDescriptor.createReliablePipe();
        mRttTextStream = new Connection.RttTextStream(fromTextHandler[1],toTextHandler[0]);

        mRttTextHandler.initialize(mRttTextStream);

        mPipeFromHandler = new InputStreamReader(
                new ParcelFileDescriptor.AutoCloseInputStream(fromTextHandler[0]));
        mPipeToHandler = new OutputStreamWriter(
                new ParcelFileDescriptor.AutoCloseOutputStream(toTextHandler[1]));
        mHandlerSideOfPipeToHandler = new InputStreamReader(
                new ParcelFileDescriptor.AutoCloseInputStream(toTextHandler[1]));
    }

    /**
     * Test that the text handler won't send characters before a timeout or enough characters
     * have accumulated.
     */
    @Test
    public void testProperCharacterBuffering() throws Exception {
        // Send four characters
        mPipeToHandler.write("abcd");
        mPipeToHandler.flush();
        waitForHandlerAction(mRttTextHandler, TEST_TIMEOUT);
        // make sure at it hasn't been sent.
        Assert.assertEquals("", mNetworkWriter.getContents());
        // Wait for 300ms
        waitForHandlerActionDelayed(mRttTextHandler, TEST_TIMEOUT,
                ImsRttTextHandler.MAX_BUFFERING_DELAY_MILLIS + 100);
        // make sure that it has been sent and check that it's correct
        Assert.assertEquals("abcd", mNetworkWriter.getContents());
    }

    /**
     * Test that the text handler sends after enough characters have been sent from in-call
     * @throws Exception
     */
    @FlakyTest
    @Test
    public void testSendAfterEnoughChars() throws Exception {
        // Register a read notifier
        CountDownLatch readNotifier = new CountDownLatch(1);
        mRttTextHandler.setReadNotifier(readNotifier);
        // Send four characters
        mPipeToHandler.write("abcd");
        mPipeToHandler.flush();
        // Wait for the stream to consume the characters
        readNotifier.await(TEST_TIMEOUT, TimeUnit.MILLISECONDS);
        waitForHandlerAction(mRttTextHandler, TEST_TIMEOUT);
        waitForHandlerAction(mRttTextHandler, TEST_TIMEOUT);
        // make sure at it hasn't been sent.
        Assert.assertEquals("", mNetworkWriter.getContents());

        // Send the second part
        Thread.sleep(10);
        // Register a read notifier
        readNotifier = new CountDownLatch(1);
        mRttTextHandler.setReadNotifier(readNotifier);
        // Send four more characters
        mPipeToHandler.write("efgh");
        mPipeToHandler.flush();
        // Wait for the stream to consume the characters
        boolean res = readNotifier.await(TEST_TIMEOUT, TimeUnit.MILLISECONDS);
        // Wait for the handler to write to the mock network writer
        waitForHandlerAction(mRttTextHandler, TEST_TIMEOUT);
        waitForHandlerAction(mRttTextHandler, TEST_TIMEOUT);
        // make sure that all characters were sent.
        try {
            Assert.assertEquals("abcdefgh", mNetworkWriter.getContents());
        } catch (ComparisonFailure e) {
            throw new ComparisonFailure(e.getMessage()
                    + ", network buffer=" + mRttTextHandler.getNetworkBufferText()
                    + ", res=" + res,
                    e.getExpected(), e.getActual());
        }
    }

    /**
     * Test that the text handler sends its characters as a batch after enough of them have been
     * buffered.
     * @throws Exception
     */
    @Test
    public void testBufferedCharactersSentAsBatch() throws Exception {
        // Send 5 characters, one at a time, pausing for 10ms between each one.
        char[] characters = new char[] {'a', 'b', 'c', 'd', 'e'};
        for (char c : characters) {
            mPipeToHandler.write(String.valueOf(c));
            mPipeToHandler.flush();
            Thread.sleep(10);
        }
        waitForHandlerAction(mRttTextHandler, TEST_TIMEOUT);
        waitForHandlerAction(mRttTextHandler, TEST_TIMEOUT);

        // Make sure that the ordering is correct and that there was only one write
        Assert.assertEquals("abcde", mNetworkWriter.getContents());
        Assert.assertEquals(1, mNetworkWriter.getNumWrites());
    }

    @Test
    public void testProperThrottling() throws Exception {
        // Send a lot of characters in rapid succession, 3 at a time
        char[] characters = LONG_TEXT.toCharArray();
        for (int i = 0; i < characters.length; i += 3) {
            String toSend = new String(characters, i, Math.min(3, characters.length - i));
            mPipeToHandler.write(toSend);
            mPipeToHandler.flush();
            Thread.sleep(10);
        }
        waitForHandlerAction(mRttTextHandler, TEST_TIMEOUT);
        waitForHandlerAction(mRttTextHandler, TEST_TIMEOUT);

        // Wait one second and see how many characters are sent in that time.
        int numCharsSoFar = mNetworkWriter.getContents().length();
        Thread.sleep(1000);
        int numCharsInOneSec = mNetworkWriter.getContents().length() - numCharsSoFar;
        Assert.assertTrue(numCharsInOneSec <= ImsRttTextHandler.MAX_CODEPOINTS_PER_SECOND);

        // Wait 5 seconds for all the chars to make it through
        Thread.sleep(5000);
        Assert.assertEquals(LONG_TEXT, mNetworkWriter.getContents());
    }

    @Test
    public void testProperTransmissionFromNetworkToInCall() throws Exception {
        // Make sure that nothing is in the pipe from the network to incall (us)
        Assert.assertFalse(mPipeFromHandler.ready());
        // Send a chunk of text
        mRttTextHandler.sendToInCall(LONG_TEXT);
        waitForHandlerAction(mRttTextHandler, TEST_TIMEOUT);
        // Make sure we get it immediately
        Assert.assertEquals(LONG_TEXT, readAll(mPipeFromHandler));
    }

    @After
    public void tearDown() throws Exception {
        mPipeFromHandler.close();
        mPipeToHandler.close();
        mRttTextHandler.tearDown();
        waitForHandlerAction(mRttTextHandler, TEST_TIMEOUT);
        mHandlerThread.quit();
        super.tearDown();
    }

    private String readAll(InputStreamReader inputStreamReader) throws IOException {
        if (!inputStreamReader.ready()) {
            return null;
        }
        int len = inputStreamReader.read(buffer, 0, READ_BUFFER_SIZE);
        return new String(buffer, 0, len);
    }
}
