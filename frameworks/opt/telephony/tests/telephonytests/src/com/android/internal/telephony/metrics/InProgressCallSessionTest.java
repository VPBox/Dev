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

package com.android.internal.telephony.metrics;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import android.test.suitebuilder.annotation.SmallTest;

import com.android.internal.telephony.TelephonyTest;
import com.android.internal.telephony.nano.TelephonyProto;
import com.android.internal.telephony.nano.TelephonyProto.TelephonyCallSession.Event.CallQuality;
import com.android.internal.telephony.nano.TelephonyProto.TelephonyCallSession.Event.CallQualitySummary;
import com.android.internal.telephony.nano.TelephonyProto.TelephonyCallSession.Event.SignalStrength;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

public class InProgressCallSessionTest extends TelephonyTest {

    private InProgressCallSession mCallSession;

    @Before
    public void setUp() throws Exception {
        super.setUp(getClass().getSimpleName());
        mCallSession = new InProgressCallSession(mPhone.getPhoneId());
    }

    @After
    public void tearDown() throws Exception {
        super.tearDown();
    }

    // Test add event
    @Test
    @SmallTest
    public void testAddEvent() {
        CallSessionEventBuilder builder = new CallSessionEventBuilder(
                TelephonyProto.TelephonyCallSession.Event.Type.RIL_RESPONSE)
                .setRilRequest(1)
                .setRilRequestId(2)
                .setRilError(3);
        mCallSession.addEvent(builder);
        assertEquals(builder.build(), mCallSession.events.getFirst());
        assertFalse(mCallSession.isEventsDropped());
    }

    // Test dropped event scenario
    @Test
    @SmallTest
    public void testEventDropped() {
        for (int i = 0; i < 301; i++) {
            CallSessionEventBuilder builder = new CallSessionEventBuilder(
                    TelephonyProto.TelephonyCallSession.Event.Type.RIL_RESPONSE)
                    .setRilRequest(1)
                    .setRilRequestId(i + 1)
                    .setRilError(3);
            mCallSession.addEvent(builder);
        }

        assertTrue(mCallSession.isEventsDropped());
        assertEquals(2, mCallSession.events.getFirst().rilRequestId);
    }

    @Test
    @SmallTest
    public void testAddCallQualityEvent() {
        CallQuality cq = new CallQuality();
        cq.downlinkLevel = CallQuality.CallQualityLevel.GOOD;
        cq.uplinkLevel = CallQuality.CallQualityLevel.GOOD;
        CallSessionEventBuilder builder = new CallSessionEventBuilder(
                TelephonyProto.TelephonyCallSession.Event.Type.CALL_QUALITY_CHANGED)
                .setCallQuality(cq);
        mCallSession.addEvent(builder);
        assertEquals(builder.build(), mCallSession.events.getFirst());
    }

    @Test
    @SmallTest
    public void testAddCallQualitySummaryEvent() {
        SignalStrength ss1 = new SignalStrength();
        ss1.lteSnr = 22;

        SignalStrength ss2 = new SignalStrength();
        ss2.lteSnr = 44;

        CallQuality cq1 = new CallQuality();
        cq1.downlinkLevel = CallQuality.CallQualityLevel.GOOD;
        cq1.uplinkLevel = CallQuality.CallQualityLevel.GOOD;

        CallQuality cq2 = new CallQuality();
        cq1.downlinkLevel = CallQuality.CallQualityLevel.BAD;
        cq1.uplinkLevel = CallQuality.CallQualityLevel.BAD;

        CallQualitySummary cqs = new CallQualitySummary();
        cqs.totalGoodQualityDurationInSeconds = 2;
        cqs.totalBadQualityDurationInSeconds = 3;
        cqs.totalDurationWithQualityInformationInSeconds = 5;
        cqs.snapshotOfWorstSsWithGoodQuality = cq2;
        cqs.snapshotOfBestSsWithGoodQuality = cq2;
        cqs.snapshotOfWorstSsWithBadQuality = cq1;
        cqs.snapshotOfBestSsWithBadQuality = cq1;
        cqs.worstSsWithGoodQuality = ss1;
        cqs.bestSsWithGoodQuality = ss2;
        cqs.worstSsWithBadQuality = ss1;
        cqs.bestSsWithBadQuality = ss2;

        CallSessionEventBuilder builder = new CallSessionEventBuilder(
                TelephonyProto.TelephonyCallSession.Event.Type.CALL_QUALITY_CHANGED)
                .setCallQualitySummaryDl(cqs)
                .setCallQualitySummaryUl(cqs);
        mCallSession.addEvent(builder);
        assertEquals(builder.build(), mCallSession.events.getFirst());
    }
}
