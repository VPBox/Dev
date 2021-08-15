/*
 * Copyright (C) 2006 The Android Open Source Project
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

package com.android.internal.telephony;

import static android.telephony.SubscriptionManager.INVALID_SUBSCRIPTION_ID;

import com.android.internal.telephony.MccTable;
import com.android.internal.telephony.mocks.SubscriptionControllerMock;
import com.android.internal.telephony.mocks.TelephonyRegistryMock;

import android.content.Context;
import android.os.AsyncResult;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;
import android.os.Message;

import android.test.AndroidTestCase;
import android.test.suitebuilder.annotation.SmallTest;

import android.telephony.Rlog;

import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;

public class SubscriptionMonitorTest extends AndroidTestCase {
    private final static String LOG_TAG = "SubscriptionMonitorTest";

    static void failAndStack(String str) {
        fail(str + "\n" + SubscriptionMonitorTest.stack());
    }

    static String stack() {
        StringBuilder sb = new StringBuilder();
        for(StackTraceElement e : Thread.currentThread().getStackTrace()) {
            sb.append(e.toString()).append("\n");
        }
        return sb.toString();
    }

    private static class TestHandler extends Handler {
        public final static int SUBSCRIPTION_CHANGED = 1;
        public final static int DEFAULT_SUBSCRIPTION_CHANGED = 2;
        public final static int IN_IDLE = 3;

        HandlerThread handlerThread;

        public TestHandler(Looper looper) {
            super(looper);
        }

        public void die() {
            if(handlerThread != null) {
                handlerThread.quit();
                handlerThread = null;
            }
        }

        public void blockTilIdle() {
            Object lock = new Object();
            synchronized (lock) {
                Message msg = this.obtainMessage(IN_IDLE, lock);
                msg.sendToTarget();
                try {
                    lock.wait();
                } catch (InterruptedException e) {}
            }
        }

        public static TestHandler makeHandler() {
            final HandlerThread handlerThread = new HandlerThread("TestHandler");
            handlerThread.start();
            final TestHandler result = new TestHandler(handlerThread.getLooper());
            result.handlerThread = handlerThread;
            return result;
        }

        private boolean objectEquals(Object o1, Object o2) {
            if (o1 == null) return (o2 == null);
            return o1.equals(o2);
        }

        private void failAndStack(String str) {
            SubscriptionMonitorTest.failAndStack(str);
        }

        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case SUBSCRIPTION_CHANGED: {
                    AsyncResult ar = (AsyncResult)(msg.obj);
                    if (objectEquals(ar.userObj, mSubscriptionChangedObject.get()) == false) {
                        failAndStack("Subscription Changed object is incorrect!");
                    }
                    mSubscriptionChangedCount.incrementAndGet();
                    Rlog.d(LOG_TAG, "SUBSCRIPTION_CHANGED, inc to " +
                            mSubscriptionChangedCount.get());
                    break;
                }
                case DEFAULT_SUBSCRIPTION_CHANGED: {
                    AsyncResult ar = (AsyncResult)(msg.obj);
                    if (objectEquals(ar.userObj,
                            mDefaultSubscriptionChangedObject.get()) == false) {
                        failAndStack("Default Subscription Changed object is incorrect!");
                    }
                    mDefaultSubscriptionChangedCount.incrementAndGet();
                    Rlog.d(LOG_TAG, "DEFAULT_SUBSCRIPTION_CHANGED, inc to " +
                            mDefaultSubscriptionChangedCount.get());
                    break;
                }
                case IN_IDLE: {
                    Object lock = msg.obj;
                    synchronized (lock) {
                        lock.notify();
                    }
                    break;
                }
            }
        }

        private final AtomicInteger mSubscriptionChangedCount = new AtomicInteger(0);
        private final AtomicReference<Object> mSubscriptionChangedObject =
                new AtomicReference<Object>();

        private final AtomicInteger mDefaultSubscriptionChangedCount = new AtomicInteger(0);
        private final AtomicReference<Object> mDefaultSubscriptionChangedObject =
                new AtomicReference<Object>();

        public void reset() {
            mSubscriptionChangedCount.set(0);
            mSubscriptionChangedObject.set(null);

            mDefaultSubscriptionChangedCount.set(0);
            mDefaultSubscriptionChangedObject.set(null);
        }

        public void setSubscriptionChangedObject(Object o) {
            mSubscriptionChangedObject.set(o);
        }
        public void setDefaultSubscriptionChangedObject(Object o) {
            mDefaultSubscriptionChangedObject.set(o);
        }

        public int getSubscriptionChangedCount() {
            return mSubscriptionChangedCount.get();
        }
        public int getDefaultSubscriptionChangedCount() {
            return mDefaultSubscriptionChangedCount.get();
        }
    }

    /**
     * Register and unregister normally.
     * Verify register worked by causing an event.
     * Verify unregister by causing another event.
     */
    @SmallTest
    public void testRegister() throws Exception {
        final int numPhones = 2;
        final ContextFixture contextFixture = new ContextFixture();
        final Context context = contextFixture.getTestDouble();
        ITelephonyRegistry.Stub telRegistry = new TelephonyRegistryMock();
        SubscriptionControllerMock subController =
                new SubscriptionControllerMock(context, telRegistry, numPhones);

        SubscriptionMonitor testedSubMonitor =
                new SubscriptionMonitor(telRegistry, context, subController, numPhones);

        TestHandler testHandler = TestHandler.makeHandler();
        Object subChangedObject = new Object();
        testHandler.setSubscriptionChangedObject(subChangedObject);

        Object defaultSubChangedObject = new Object();
        testHandler.setDefaultSubscriptionChangedObject(defaultSubChangedObject);

        // try events before registering
        subController.setDefaultDataSubId(0);
        subController.setSlotSubId(0, 0);

        if (testHandler.getSubscriptionChangedCount() != 0) {
            fail("pretest of SubscriptionChangedCount");
        }
        if (testHandler.getDefaultSubscriptionChangedCount() != 0) {
            fail("pretest of DefaultSubscriptionChangedCount");
        }

        testedSubMonitor.registerForSubscriptionChanged(0, testHandler,
                  TestHandler.SUBSCRIPTION_CHANGED, subChangedObject);
        testHandler.blockTilIdle();

        if (testHandler.getSubscriptionChangedCount() != 1) {
            fail("test1 of SubscriptionChangedCount");
        }
        if (testHandler.getDefaultSubscriptionChangedCount() != 0) {
            fail("test1 of DefaultSubscriptionChangedCount");
        }

        testedSubMonitor.registerForDefaultDataSubscriptionChanged(0, testHandler,
                TestHandler.DEFAULT_SUBSCRIPTION_CHANGED, defaultSubChangedObject);
        testHandler.blockTilIdle();

        if (testHandler.getSubscriptionChangedCount() != 1) {
            fail("test2 of SubscriptionChangedCount");
        }
        if (testHandler.getDefaultSubscriptionChangedCount() != 1) {
            fail("test2 of DefaultSubscriptionChangedCount");
        }

        subController.setDefaultDataSubId(1);
        testHandler.blockTilIdle();

        if (testHandler.getSubscriptionChangedCount() != 1) {
            fail("test3 of SubscriptionChangedCount, " +
                    testHandler.getSubscriptionChangedCount() + " vs 1");
        }
        if (testHandler.getDefaultSubscriptionChangedCount() != 2) {
            fail("test3 of DefaultSubscriptionChangedCount, " +
                    testHandler.getDefaultSubscriptionChangedCount() + " vs 2");
        }

        subController.setSlotSubId(0, 1);
        testHandler.blockTilIdle();

        if (testHandler.getSubscriptionChangedCount() != 2) {
            fail("test4 of SubscriptionChangedCount");
        }
        if (testHandler.getDefaultSubscriptionChangedCount() != 3) {
            fail("test4 of DefaultSubscriptionChangedCount");
        }

        testedSubMonitor.unregisterForDefaultDataSubscriptionChanged(0, testHandler);
        subController.setSlotSubId(0, 0);
        testHandler.blockTilIdle();

        if (testHandler.getSubscriptionChangedCount() != 3) {
            fail("test5 of SubscriptionChangedCount, " +
                    testHandler.getSubscriptionChangedCount() + " vs 3");
        }
        if (testHandler.getDefaultSubscriptionChangedCount() != 3) {
            fail("test5 of DefaultSubscriptionChangedCount, " +
                    testHandler.getDefaultSubscriptionChangedCount() + " vs 3");
        }

        testedSubMonitor.unregisterForSubscriptionChanged(0, testHandler);

        subController.setSlotSubId(0, 1);
        subController.setDefaultDataSubId(0);
        testHandler.blockTilIdle();

        if (testHandler.getSubscriptionChangedCount() != 3) {
            fail("test6 of SubscriptionChangedCount, " +
                    testHandler.getSubscriptionChangedCount() + " vs 3");
        }
        if (testHandler.getDefaultSubscriptionChangedCount() != 3) {
            fail("test6 of DefaultSubscriptionChangedCount, " +
                    testHandler.getDefaultSubscriptionChangedCount() + " vs 3");
        }

        testHandler.die();
    }

    /**
     * Bad register/unregisters
     *
     * Try phoneId that doesn't exist.
     * Cause an event and verify don't get notified.
     * Try to unregister multiple times.
     */
    @SmallTest
    public void testBadRegister() throws Exception {
        final int numPhones = 2;
        final ContextFixture contextFixture = new ContextFixture();
        final Context context = contextFixture.getTestDouble();
        ITelephonyRegistry.Stub telRegistry = new TelephonyRegistryMock();
        SubscriptionControllerMock subController =
                new SubscriptionControllerMock(context, telRegistry, numPhones);

        SubscriptionMonitor testedSubMonitor =
                new SubscriptionMonitor(telRegistry, context, subController, numPhones);

        TestHandler testHandler = TestHandler.makeHandler();
        Object subChangedObject = new Object();
        testHandler.setSubscriptionChangedObject(subChangedObject);

        Object defaultSubChangedObject = new Object();
        testHandler.setDefaultSubscriptionChangedObject(defaultSubChangedObject);

        try {
            testedSubMonitor.registerForSubscriptionChanged(-1, testHandler,
                      TestHandler.SUBSCRIPTION_CHANGED, subChangedObject);
            fail("IllegalArgumentException expected with bad phoneId");
        } catch (IllegalArgumentException e) {}
        try {
            testedSubMonitor.registerForDefaultDataSubscriptionChanged(-1, testHandler,
                    TestHandler.DEFAULT_SUBSCRIPTION_CHANGED, defaultSubChangedObject);
            fail("IllegalArgumentException expected with bad phoneId");
        } catch (IllegalArgumentException e) {}
        try {
            testedSubMonitor.registerForSubscriptionChanged(numPhones, testHandler,
                      TestHandler.SUBSCRIPTION_CHANGED, subChangedObject);
            fail("IllegalArgumentException expected with bad phoneId");
        } catch (IllegalArgumentException e) {}
        try {
            testedSubMonitor.registerForDefaultDataSubscriptionChanged(numPhones, testHandler,
                    TestHandler.DEFAULT_SUBSCRIPTION_CHANGED, defaultSubChangedObject);
            fail("IllegalArgumentException expected with bad phoneId");
        } catch (IllegalArgumentException e) {}

        subController.setDefaultDataSubId(0);
        subController.setSlotSubId(0, 0);

        if (testHandler.getSubscriptionChangedCount() != 0) {
            fail("getSubscriptionChangedCount reported non-zero!");
        }
        if (testHandler.getDefaultSubscriptionChangedCount() != 0) {
            fail("getDefaultSubscriptionChangedCount reported non-zero!");
        }

        testHandler.die();
    }

    /**
     * Try to force spurious notifications - register/unregister in tight loop with
     * events happening in the unregistered gap.
     */
    @SmallTest
    public void testSpuriousNotifications() throws Exception {
        final int numPhones = 2;
        final ContextFixture contextFixture = new ContextFixture();
        final Context context = contextFixture.getTestDouble();
        ITelephonyRegistry.Stub telRegistry = new TelephonyRegistryMock();
        SubscriptionControllerMock subController =
                new SubscriptionControllerMock(context, telRegistry, numPhones);

        SubscriptionMonitor testedSubMonitor =
                new SubscriptionMonitor(telRegistry, context, subController, numPhones);

        TestHandler testHandler = TestHandler.makeHandler();
        Object subChangedObject = new Object();
        testHandler.setSubscriptionChangedObject(subChangedObject);

        Object defaultSubChangedObject = new Object();
        testHandler.setDefaultSubscriptionChangedObject(defaultSubChangedObject);

        final int PHONE_ID = 0;
        final int FIRST_SUB_ID = 0;
        final int SECOND_SUB_ID = 1;

        testedSubMonitor.registerForSubscriptionChanged(PHONE_ID, testHandler,
                TestHandler.SUBSCRIPTION_CHANGED, subChangedObject);
        testedSubMonitor.registerForDefaultDataSubscriptionChanged(PHONE_ID, testHandler,
                TestHandler.DEFAULT_SUBSCRIPTION_CHANGED, defaultSubChangedObject);
        final int LOOP_COUNT = 1000;
        for (int i = 0; i < LOOP_COUNT; i++) {
            testedSubMonitor.unregisterForSubscriptionChanged(PHONE_ID, testHandler);
            testedSubMonitor.unregisterForDefaultDataSubscriptionChanged(PHONE_ID, testHandler);

            subController.setDefaultDataSubId(FIRST_SUB_ID);
            subController.setSlotSubId(PHONE_ID, FIRST_SUB_ID);

            subController.setDefaultDataSubId(SECOND_SUB_ID);
            subController.setSlotSubId(PHONE_ID, SECOND_SUB_ID);

            testedSubMonitor.registerForSubscriptionChanged(PHONE_ID, testHandler,
                    TestHandler.SUBSCRIPTION_CHANGED, subChangedObject);
            testedSubMonitor.registerForDefaultDataSubscriptionChanged(PHONE_ID, testHandler,
                    TestHandler.DEFAULT_SUBSCRIPTION_CHANGED, defaultSubChangedObject);
        }
        testHandler.blockTilIdle();

        // should get one for every registration
        if (testHandler.getSubscriptionChangedCount() != 1 + LOOP_COUNT) {
            fail("getSubscriptionChangedCount reported " +
                    testHandler.getSubscriptionChangedCount() + " != " + (1 + LOOP_COUNT));
        }
        if (testHandler.getDefaultSubscriptionChangedCount() != 1 + LOOP_COUNT) {
            fail("getDefaultSubscriptionChangedCount reported " +
                    testHandler.getDefaultSubscriptionChangedCount() + " != " + (1 + LOOP_COUNT));
        }

        testHandler.die();
    }

    /**
     * Test duplicate registrations - both should survive
     * Also test duplicate unreg - shouldn't crash..
     */
    @SmallTest
    public void testMultiRegUnregistration() throws Exception {
        final int numPhones = 2;
        final ContextFixture contextFixture = new ContextFixture();
        final Context context = contextFixture.getTestDouble();
        ITelephonyRegistry.Stub telRegistry = new TelephonyRegistryMock();
        SubscriptionControllerMock subController =
                new SubscriptionControllerMock(context, telRegistry, numPhones);

        SubscriptionMonitor testedSubMonitor =
                new SubscriptionMonitor(telRegistry, context, subController, numPhones);

        TestHandler testHandler = TestHandler.makeHandler();
        Object subChangedObject = new Object();
        testHandler.setSubscriptionChangedObject(subChangedObject);

        Object defaultSubChangedObject = new Object();
        testHandler.setDefaultSubscriptionChangedObject(defaultSubChangedObject);

        final int PHONE_ID = 0;
        final int FIRST_SUB_ID = 0;
        final int SECOND_SUB_ID = 1;

        testedSubMonitor.registerForSubscriptionChanged(PHONE_ID, testHandler,
                TestHandler.SUBSCRIPTION_CHANGED, subChangedObject);
        testedSubMonitor.registerForDefaultDataSubscriptionChanged(PHONE_ID, testHandler,
                TestHandler.DEFAULT_SUBSCRIPTION_CHANGED, defaultSubChangedObject);

        testedSubMonitor.registerForSubscriptionChanged(PHONE_ID, testHandler,
                TestHandler.SUBSCRIPTION_CHANGED, subChangedObject);
        testedSubMonitor.registerForDefaultDataSubscriptionChanged(PHONE_ID, testHandler,
                TestHandler.DEFAULT_SUBSCRIPTION_CHANGED, defaultSubChangedObject);

        subController.setDefaultDataSubId(FIRST_SUB_ID);
        subController.setSlotSubId(PHONE_ID, FIRST_SUB_ID);

        subController.setDefaultDataSubId(SECOND_SUB_ID);
        subController.setSlotSubId(PHONE_ID, SECOND_SUB_ID);

        testHandler.blockTilIdle();

        // should get 1 for each registration and 4 for the two events
        if (testHandler.getSubscriptionChangedCount() != 6) {
            fail("getSubscriptionChangedCount reported " +
                    testHandler.getSubscriptionChangedCount() + " != 6");
        }
        // 2 for the 2 registrations, 2 for the single event in the first cluster (2 listeners)
        // 2 for the setDefatulDataSub in the second cluster (lost data sub)
        // 2 for the setSlotSubId (regain default)
        if (testHandler.getDefaultSubscriptionChangedCount() != 8) {
            fail("getDefaultSubscriptionChangedCount reported " +
                    testHandler.getDefaultSubscriptionChangedCount() + " != 8");
        }

        testedSubMonitor.unregisterForSubscriptionChanged(PHONE_ID, testHandler);
        testedSubMonitor.unregisterForDefaultDataSubscriptionChanged(PHONE_ID, testHandler);
        testedSubMonitor.unregisterForSubscriptionChanged(PHONE_ID, testHandler);
        testedSubMonitor.unregisterForDefaultDataSubscriptionChanged(PHONE_ID, testHandler);

        testHandler.die();
    }

    /**
     * Try event flood while registered - verify receive all.
     */
    @SmallTest
    public void testEventFloodNotifications() throws Exception {
        final int numPhones = 2;
        final ContextFixture contextFixture = new ContextFixture();
        final Context context = contextFixture.getTestDouble();
        ITelephonyRegistry.Stub telRegistry = new TelephonyRegistryMock();
        SubscriptionControllerMock subController =
                new SubscriptionControllerMock(context, telRegistry, numPhones);

        SubscriptionMonitor testedSubMonitor =
                new SubscriptionMonitor(telRegistry, context, subController, numPhones);

        TestHandler testHandler = TestHandler.makeHandler();
        Object subChangedObject = new Object();
        testHandler.setSubscriptionChangedObject(subChangedObject);

        Object defaultSubChangedObject = new Object();
        testHandler.setDefaultSubscriptionChangedObject(defaultSubChangedObject);

        final int PHONE_ID = 0;
        final int FIRST_SUB_ID = 0;
        final int SECOND_SUB_ID = 1;

        testedSubMonitor.registerForSubscriptionChanged(PHONE_ID, testHandler,
                TestHandler.SUBSCRIPTION_CHANGED, subChangedObject);
        testedSubMonitor.registerForDefaultDataSubscriptionChanged(PHONE_ID, testHandler,
                TestHandler.DEFAULT_SUBSCRIPTION_CHANGED, defaultSubChangedObject);

        final int LOOP_COUNT = 1;
        for (int i = 0; i < LOOP_COUNT; i++) {
            subController.setDefaultDataSubId(FIRST_SUB_ID);
            subController.setSlotSubId(PHONE_ID, FIRST_SUB_ID);

            subController.setDefaultDataSubId(SECOND_SUB_ID);
            subController.setSlotSubId(PHONE_ID, SECOND_SUB_ID);
        }
        testHandler.blockTilIdle();

        // should get one for registration + 2 per loop
        if (testHandler.getSubscriptionChangedCount() != 1 + (2 * LOOP_COUNT)) {
            fail("getSubscriptionChangedCount reported " +
                    testHandler.getSubscriptionChangedCount() + " != " + (1 + (2 * LOOP_COUNT)));
        }
        // should get one for registration + 3 for first loop + 4 for subsequent loops
        if (testHandler.getDefaultSubscriptionChangedCount() != (4 * LOOP_COUNT)) {
            fail("getDefaultSubscriptionChangedCount reported " +
                    testHandler.getDefaultSubscriptionChangedCount() + " != " +
                    (4 * LOOP_COUNT));
        }

        testHandler.die();
    }

    /**
     * Try tests with no default set
     */
    @SmallTest
    public void testNoDefaultNotifications() throws Exception {
        final int numPhones = 2;
        final ContextFixture contextFixture = new ContextFixture();
        final Context context = contextFixture.getTestDouble();
        ITelephonyRegistry.Stub telRegistry = new TelephonyRegistryMock();
        SubscriptionControllerMock subController =
                new SubscriptionControllerMock(context, telRegistry, numPhones);

        SubscriptionMonitor testedSubMonitor =
                new SubscriptionMonitor(telRegistry, context, subController, numPhones);

        TestHandler testHandler = TestHandler.makeHandler();
        Object subChangedObject = new Object();
        testHandler.setSubscriptionChangedObject(subChangedObject);

        Object defaultSubChangedObject = new Object();
        testHandler.setDefaultSubscriptionChangedObject(defaultSubChangedObject);

        final int PHONE_ID = 0;
        final int FIRST_SUB_ID = 0;
        final int SECOND_SUB_ID = 1;

        subController.setDefaultDataSubId(INVALID_SUBSCRIPTION_ID);
        subController.setSlotSubId(PHONE_ID, FIRST_SUB_ID);

        testedSubMonitor.registerForSubscriptionChanged(PHONE_ID, testHandler,
                TestHandler.SUBSCRIPTION_CHANGED, subChangedObject);
        testedSubMonitor.registerForDefaultDataSubscriptionChanged(PHONE_ID, testHandler,
                TestHandler.DEFAULT_SUBSCRIPTION_CHANGED, defaultSubChangedObject);


        subController.setSlotSubId(PHONE_ID, SECOND_SUB_ID);
        subController.setSlotSubId(PHONE_ID, FIRST_SUB_ID);

        testHandler.blockTilIdle();

        if (testHandler.getSubscriptionChangedCount() != 3) {
            fail("getSubscriptionChangedCount reported " +
                    testHandler.getSubscriptionChangedCount() + " != 3");
        }
        if (testHandler.getDefaultSubscriptionChangedCount() != 1) {
            fail("getDefaultSubscriptionChangedCount reported " +
                    testHandler.getDefaultSubscriptionChangedCount() + " != 1");
        }

        testHandler.die();
    }

    @SmallTest
    public void testNoSubChange() throws Exception {
        String TAG = "testNoSubChange";
        final int numPhones = 2;
        final ContextFixture contextFixture = new ContextFixture();
        final Context context = contextFixture.getTestDouble();
        ITelephonyRegistry.Stub telRegistry = new TelephonyRegistryMock();
        SubscriptionControllerMock subController =
                new SubscriptionControllerMock(context, telRegistry, numPhones);

        SubscriptionMonitor testedSubMonitor =
                new SubscriptionMonitor(telRegistry, context, subController, numPhones);

        TestHandler testHandler = TestHandler.makeHandler();
        Object subChangedObject = new Object();
        testHandler.setSubscriptionChangedObject(subChangedObject);

        Object defaultSubChangedObject = new Object();
        testHandler.setDefaultSubscriptionChangedObject(defaultSubChangedObject);

        final int PHONE_ID = 0;
        final int FIRST_SUB_ID = 0;
        final int SECOND_SUB_ID = 1;

        testHandler.blockTilIdle();
        Rlog.d(TAG, "1");

        testedSubMonitor.registerForSubscriptionChanged(PHONE_ID, testHandler,
                TestHandler.SUBSCRIPTION_CHANGED, subChangedObject);

        testHandler.blockTilIdle();
        Rlog.d(TAG, "2");

        testedSubMonitor.registerForDefaultDataSubscriptionChanged(PHONE_ID, testHandler,
                TestHandler.DEFAULT_SUBSCRIPTION_CHANGED, defaultSubChangedObject);

        testHandler.blockTilIdle();
        Rlog.d(TAG, "3");

        subController.setSlotSubId(PHONE_ID, FIRST_SUB_ID);

        testHandler.blockTilIdle();
        Rlog.d(TAG, "4");

        subController.setDefaultDataSubId(FIRST_SUB_ID);

        testHandler.blockTilIdle();
        Rlog.d(TAG, "5");

        if (testHandler.getSubscriptionChangedCount() != 2) {
            fail("getSubscriptionChangedCount reported " +
                    testHandler.getSubscriptionChangedCount() + " != 2");
        }
        // 1 gained for reg  and 1 for the setting above
        if (testHandler.getDefaultSubscriptionChangedCount() != 2) {
            fail("getDefaultSubscriptionChangedCount reported " +
                    testHandler.getDefaultSubscriptionChangedCount() + " != 2");
        }

        Rlog.d(TAG, "6");

        // cause a notification that subscription info changed
        subController.notifySubscriptionInfoChanged();
        testHandler.blockTilIdle();

        Rlog.d(TAG, "7");

        if (testHandler.getSubscriptionChangedCount() != 2) {
            fail("getSubscriptionChangedCount reported " +
                    testHandler.getSubscriptionChangedCount() + " != 2");
        }
        if (testHandler.getDefaultSubscriptionChangedCount() != 2) {
            fail("getDefaultSubscriptionChangedCount reported " +
                    testHandler.getDefaultSubscriptionChangedCount() + " != 2");
        }

        // now change the default - should cause a default notification (we lost the default)
        subController.setDefaultDataSubId(SECOND_SUB_ID);

        testHandler.blockTilIdle();
        Rlog.d(TAG, "8");

        if (testHandler.getSubscriptionChangedCount() != 2) {
            fail("getSubscriptionChangedCount reported " +
                    testHandler.getSubscriptionChangedCount() + " != 2");
        }
        if (testHandler.getDefaultSubscriptionChangedCount() != 3) {
            fail("getDefaultSubscriptionChangedCount reported " +
                    testHandler.getDefaultSubscriptionChangedCount() + " != 3");
        }
        testHandler.die();
    }

    /**
     * Try setting the subIds first and then the default subId and verify we get all our
     * notifications.
     */
    @SmallTest
    public void testSubBeforeDefaultNotifications() throws Exception {
        final int numPhones = 2;
        final ContextFixture contextFixture = new ContextFixture();
        final Context context = contextFixture.getTestDouble();
        ITelephonyRegistry.Stub telRegistry = new TelephonyRegistryMock();
        SubscriptionControllerMock subController =
                new SubscriptionControllerMock(context, telRegistry, numPhones);

        SubscriptionMonitor testedSubMonitor =
                new SubscriptionMonitor(telRegistry, context, subController, numPhones);

        TestHandler testHandler = TestHandler.makeHandler();
        Object subChangedObject = new Object();
        testHandler.setSubscriptionChangedObject(subChangedObject);

        Object defaultSubChangedObject = new Object();
        testHandler.setDefaultSubscriptionChangedObject(defaultSubChangedObject);

        final int PHONE_ID = 0;
        final int SECOND_PHONE_ID = 1;
        final int FIRST_SUB_ID = 0;
        final int SECOND_SUB_ID = 1;
        testedSubMonitor.registerForSubscriptionChanged(PHONE_ID, testHandler,
                TestHandler.SUBSCRIPTION_CHANGED, subChangedObject);
        testedSubMonitor.registerForDefaultDataSubscriptionChanged(PHONE_ID, testHandler,
                TestHandler.DEFAULT_SUBSCRIPTION_CHANGED, defaultSubChangedObject);
        subController.setSlotSubId(PHONE_ID, -2);
        subController.setSlotSubId(SECOND_PHONE_ID, -3);
        testHandler.blockTilIdle();
        // should get one for registration and 1 for the change
        if (testHandler.getSubscriptionChangedCount() != 2) {
            fail("test1 " + testHandler.getSubscriptionChangedCount() + " != 2");
        }
        // should get one for registration
        if (testHandler.getDefaultSubscriptionChangedCount() != 1) {
            fail("test2 " + testHandler.getDefaultSubscriptionChangedCount() + " != 1");
        }

        subController.setDefaultDataSubId(FIRST_SUB_ID);
        testHandler.blockTilIdle();

        // no change
        if (testHandler.getSubscriptionChangedCount() != 2) {
            fail("test3 " + testHandler.getSubscriptionChangedCount() + " != 2");
        }
        if (testHandler.getDefaultSubscriptionChangedCount() != 1) {
            fail("test4 " + testHandler.getDefaultSubscriptionChangedCount() + " != 1");
        }

        subController.setSlotSubId(PHONE_ID, FIRST_SUB_ID);
        testHandler.blockTilIdle();

        // should get one more default-change-notification
        if (testHandler.getSubscriptionChangedCount() != 3) {
            fail("test5 " + testHandler.getSubscriptionChangedCount() + " != 3");
        }
        if (testHandler.getDefaultSubscriptionChangedCount() != 2) {
            fail("test6 " + testHandler.getDefaultSubscriptionChangedCount() + " != 2");
        }

        subController.setDefaultDataSubId(SECOND_SUB_ID);
        testHandler.blockTilIdle();

        // should get one more default-change-notification
        if (testHandler.getSubscriptionChangedCount() != 3) {
            fail("test7 " + testHandler.getSubscriptionChangedCount() + " != 3");
        }
        if (testHandler.getDefaultSubscriptionChangedCount() != 3) {
            fail("test8 " + testHandler.getDefaultSubscriptionChangedCount() + " != 3");
        }

        subController.setDefaultDataSubId(FIRST_SUB_ID);
        testHandler.blockTilIdle();

        // should get one more default-change-notification
        if (testHandler.getSubscriptionChangedCount() != 3) {
            fail("test9 " + testHandler.getSubscriptionChangedCount() + " != 3");
        }
        if (testHandler.getDefaultSubscriptionChangedCount() != 4) {
            fail("test10 " + testHandler.getDefaultSubscriptionChangedCount() + " != 4");
        }

        testHandler.die();
    }

    /**
     * It turns out when we swap sims on a single sim we do something like:
     *   Phone[0] subId  1 -> -2
     *   Phone[0] subId -2 ->  2
     *   Default change  1 ->  2
     * Try that and verify we get all the subId and default changes we expect.
     */
    @SmallTest
    public void testSimSwapNotifications() throws Exception {
        final int numPhones = 1;
        final ContextFixture contextFixture = new ContextFixture();
        final Context context = contextFixture.getTestDouble();
        ITelephonyRegistry.Stub telRegistry = new TelephonyRegistryMock();
        SubscriptionControllerMock subController =
                new SubscriptionControllerMock(context, telRegistry, numPhones);

        SubscriptionMonitor testedSubMonitor =
                new SubscriptionMonitor(telRegistry, context, subController, numPhones);

        TestHandler testHandler = TestHandler.makeHandler();
        Object subChangedObject = new Object();
        testHandler.setSubscriptionChangedObject(subChangedObject);

        Object defaultSubChangedObject = new Object();
        testHandler.setDefaultSubscriptionChangedObject(defaultSubChangedObject);

        final int PHONE_ID = 0;
        final int FIRST_SUB_ID = 0;
        final int SECOND_SUB_ID = 1;
        testedSubMonitor.registerForSubscriptionChanged(PHONE_ID, testHandler,
                TestHandler.SUBSCRIPTION_CHANGED, subChangedObject);
        testedSubMonitor.registerForDefaultDataSubscriptionChanged(PHONE_ID, testHandler,
                TestHandler.DEFAULT_SUBSCRIPTION_CHANGED, defaultSubChangedObject);
        subController.setSlotSubId(PHONE_ID, -2);
        testHandler.blockTilIdle();
        // should get one for registration and 1 for the change
        if (testHandler.getSubscriptionChangedCount() != 2) {
            fail("test1 " + testHandler.getSubscriptionChangedCount() + " != 2");
        }
        // should get one for registration
        if (testHandler.getDefaultSubscriptionChangedCount() != 1) {
            fail("test2 " + testHandler.getDefaultSubscriptionChangedCount() + " != 1");
        }

        subController.setSlotSubId(PHONE_ID, FIRST_SUB_ID);
        testHandler.blockTilIdle();
        if (testHandler.getSubscriptionChangedCount() != 3) {
            fail("test3 " + testHandler.getSubscriptionChangedCount() + " != 3");
        }

        subController.setDefaultDataSubId(FIRST_SUB_ID);
        testHandler.blockTilIdle();
        if (testHandler.getDefaultSubscriptionChangedCount() != 2) {
            fail("test4 " + testHandler.getDefaultSubscriptionChangedCount() + " != 2");
        }

        // ok - now for the sim swap
        subController.setSlotSubId(PHONE_ID, -2);
        testHandler.blockTilIdle();
        if (testHandler.getDefaultSubscriptionChangedCount() != 3) {
            fail("test5 " + testHandler.getDefaultSubscriptionChangedCount() + " != 3");
        }
        if (testHandler.getSubscriptionChangedCount() != 4) {
            fail("test6 " + testHandler.getSubscriptionChangedCount() + " != 4");
        }

        subController.setSlotSubId(PHONE_ID, SECOND_SUB_ID);
        testHandler.blockTilIdle();

        if (testHandler.getSubscriptionChangedCount() != 5) {
            fail("test7 " + testHandler.getSubscriptionChangedCount() + " != 5");
        }

        subController.setDefaultDataSubId(SECOND_SUB_ID);
        testHandler.blockTilIdle();

        if (testHandler.getDefaultSubscriptionChangedCount() != 4) {
            fail("test8 " + testHandler.getDefaultSubscriptionChangedCount() + " != 4");
        }
        // no change
        if (testHandler.getSubscriptionChangedCount() != 5) {
            fail("test9 " + testHandler.getSubscriptionChangedCount() + " != 5");
        }

        testHandler.die();
    }
}
