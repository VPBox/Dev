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

package com.android.server.wifi.rtt;

import static org.hamcrest.core.IsEqual.equalTo;
import static org.mockito.Mockito.when;

import android.hardware.wifi.V1_0.RttResult;
import android.hardware.wifi.V1_0.RttStatus;
import android.net.MacAddress;
import android.net.wifi.rtt.RangingRequest;
import android.net.wifi.rtt.ResponderConfig;
import android.os.WorkSource;
import android.util.Log;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.Clock;
import com.android.server.wifi.nano.WifiMetricsProto;

import org.junit.Before;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.ErrorCollector;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

import java.io.PrintWriter;
import java.io.StringWriter;
import java.util.ArrayList;
import java.util.List;

/**
 * Unit test harness for RttMetrics
 */
@SmallTest
public class RttMetricsTest {
    private RttMetrics mDut;

    @Mock
    Clock mClock;

    @Rule
    public ErrorCollector collector = new ErrorCollector();

    /**
     * Pre-test configuration. Initialize and install mocks.
     */
    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);

        setTime(1);
        mDut = new RttMetrics(mClock);
    }

    /**
     * Verify that recordRequest() records valid metrics.
     */
    @Test
    public void testRecordRequest() {
        WifiMetricsProto.WifiRttLog log;

        // no requests
        log = mDut.consolidateProto();
        checkMainStats("No requests", log, 0, 0);
        checkPeerStats("No requests: AP", log.rttToAp, 0, 0, 0, 0, 0, 0, 0, 0);
        checkPeerStats("No requests: Aware", log.rttToAware, 0, 0, 0, 0, 0, 0, 0, 0);

        // multiple AP requests from multiple sources
        WorkSource ws1 = new WorkSource(10);
        WorkSource ws2 = new WorkSource(20);
        ws2.add(10);

        RangingRequest requestAp1 = getDummyRangingRequest(1, 0);
        RangingRequest requestAp2 = getDummyRangingRequest(2, 0);
        RangingRequest requestAp5 = getDummyRangingRequest(5, 0);
        RangingRequest requestAp6 = getDummyRangingRequest(6, 0);

        mDut.clear();
        mDut.recordRequest(ws1, requestAp1);
        setTime(10); // delta = 9
        mDut.recordRequest(ws1, requestAp2);
        setTime(20); // delta = 10
        mDut.recordRequest(ws1, requestAp5);
        setTime(21); // delta = 1
        mDut.recordRequest(ws1, requestAp6);
        setTime(1000); // delta = 979
        mDut.recordRequest(ws1, requestAp5);
        setTime(5000); // delta = 4,000
        mDut.recordRequest(ws1, requestAp5);
        setTime(1000000); // delta = 995,000
        mDut.recordRequest(ws1, requestAp2);
        mDut.recordRequest(ws2, requestAp5);
        mDut.recordRequest(ws2, requestAp5);
        mDut.recordRequest(ws2, requestAp5);

        log = mDut.consolidateProto();
        checkMainStats("Sequence AP-only", log, 10, 0);

        checkPeerStats("Sequence AP-only: AP", log.rttToAp, 10, 41, 2, 2, 4, 0, 0, 5);

        validateProtoHistBucket("Sequence AP-only: rttToAp.histogramNumRequestsPerApp[0]",
                log.rttToAp.histogramNumRequestsPerApp[0], 1, 10, 1);
        validateProtoHistBucket("Sequence AP-only: rttToAp.histogramNumRequestsPerApp[1]",
                log.rttToAp.histogramNumRequestsPerApp[1], 10, 100, 1);

        validateProtoHistBucket("Sequence AP-only: rttToAp.histogramNumPeersPerRequest[0]",
                log.rttToAp.histogramNumPeersPerRequest[0], 1, 1, 1);
        validateProtoHistBucket("Sequence AP-only: rttToAp.histogramNumPeersPerRequest[1]",
                log.rttToAp.histogramNumPeersPerRequest[1], 2, 2, 2);
        validateProtoHistBucket("Sequence AP-only: rttToAp.histogramNumPeersPerRequest[2]",
                log.rttToAp.histogramNumPeersPerRequest[2], 5, 5, 6);
        validateProtoHistBucket("Sequence AP-only: rttToAp.histogramNumPeersPerRequest[3]",
                log.rttToAp.histogramNumPeersPerRequest[3], 6, 6, 1);

        validateProtoHistBucket("Sequence AP-only: rttToAp.histogramRequestIntervalMs[0]",
                log.rttToAp.histogramRequestIntervalMs[0], 1, 10, 5);
        validateProtoHistBucket("Sequence AP-only: rttToAp.histogramRequestIntervalMs[1]",
                log.rttToAp.histogramRequestIntervalMs[1], 10, 100, 1);
        validateProtoHistBucket("Sequence AP-only: rttToAp.histogramRequestIntervalMs[2]",
                log.rttToAp.histogramRequestIntervalMs[2], 100, 1000, 1);
        validateProtoHistBucket("Sequence AP-only: rttToAp.histogramRequestIntervalMs[3]",
                log.rttToAp.histogramRequestIntervalMs[3], 1000, 10000, 1);
        validateProtoHistBucket("Sequence AP-only: rttToAp.histogramRequestIntervalMs[4]",
                log.rttToAp.histogramRequestIntervalMs[4], 100000, 1000000, 1);

        checkPeerStats("Sequence AP-only: Aware", log.rttToAware, 0, 0, 0, 0, 0, 0, 0, 0);

        // mix of AP and Aware requests
        WorkSource ws3 = new WorkSource(30);
        ws3.add(20);
        ws3.add(40);

        RangingRequest requestMixed03 = getDummyRangingRequest(0, 3);
        RangingRequest requestMixed25 = getDummyRangingRequest(2, 5);
        RangingRequest requestMixed50 = getDummyRangingRequest(5, 0);
        RangingRequest requestMixed08 = getDummyRangingRequest(0, 8);

        mDut.clear();
        setTime(100);
        mDut.recordRequest(ws3, requestMixed03);
        setTime(101);
        mDut.recordRequest(ws3, requestMixed25);
        setTime(102);
        mDut.recordRequest(ws3, requestMixed50);
        setTime(103);
        mDut.recordRequest(ws3, requestMixed08);

        log = mDut.consolidateProto();
        checkMainStats("Sequence Mixed AP/Aware", log, 4, 0);

        checkPeerStats("Sequence Mixed AP/Aware: AP", log.rttToAp, 2, 7, 3, 1, 2, 0, 0, 1);

        validateProtoHistBucket("Sequence Mixed AP/Aware: rttToAp.histogramNumRequestsPerApp[0]",
                log.rttToAp.histogramNumRequestsPerApp[0], 1, 10, 3);

        validateProtoHistBucket("Sequence Mixed AP/Aware: rttToAp.histogramNumPeersPerRequest[0]",
                log.rttToAp.histogramNumPeersPerRequest[0], 2, 2, 1);
        validateProtoHistBucket("Sequence Mixed AP/Aware: rttToAp.histogramNumPeersPerRequest[1]",
                log.rttToAp.histogramNumPeersPerRequest[1], 5, 5, 1);

        validateProtoHistBucket("Sequence Mixed AP/Aware: rttToAp.histogramRequestIntervalMs[0]",
                log.rttToAp.histogramRequestIntervalMs[0], 1, 10, 1);

        checkPeerStats("Sequence Mixed AP/Aware: Aware", log.rttToAware, 3, 16, 3, 1, 3, 0, 0, 1);

        validateProtoHistBucket("Sequence Mixed AP/Aware: rttToAware.histogramNumRequestsPerApp[0]",
                log.rttToAware.histogramNumRequestsPerApp[0], 1, 10, 3);

        validateProtoHistBucket(
                "Sequence Mixed AP/Aware: rttToAware.histogramNumPeersPerRequest[0]",
                log.rttToAware.histogramNumPeersPerRequest[0], 3, 3, 1);
        validateProtoHistBucket(
                "Sequence Mixed AP/Aware: rttToAware.histogramNumPeersPerRequest[1]",
                log.rttToAware.histogramNumPeersPerRequest[1], 5, 5, 1);
        validateProtoHistBucket(
                "Sequence Mixed AP/Aware: rttToAware.histogramNumPeersPerRequest[2]",
                log.rttToAware.histogramNumPeersPerRequest[2], 8, 8, 1);

        validateProtoHistBucket("Sequence Mixed AP/Aware: rttToAware.histogramRequestIntervalMs[0]",
                log.rttToAware.histogramRequestIntervalMs[0], 1, 10, 2);
    }

    /**
     * Verify that recordResult() records valid metrics.
     */
    @Test
    public void testRecordResult() {
        WifiMetricsProto.WifiRttLog log;

        // no requests
        log = mDut.consolidateProto();
        checkMainStats("No requests", log, 0, 0);
        checkPeerStats("No requests: AP", log.rttToAp, 0, 0, 0, 0, 0, 0, 0, 0);
        checkPeerStats("No requests: Aware", log.rttToAware, 0, 0, 0, 0, 0, 0, 0, 0);

        // multiple AP results
        RangingRequest requestAp1 = getDummyRangingRequest(1, 0);
        RangingRequest requestAp2 = getDummyRangingRequest(2, 0);
        RangingRequest requestAp5 = getDummyRangingRequest(5, 0);
        RangingRequest requestAp6 = getDummyRangingRequest(6, 0);

        mDut.clear();
        mDut.recordResult(requestAp1, getDummyRangingResults(requestAp1, 5, 0));
        mDut.recordResult(requestAp2, getDummyRangingResults(requestAp2, 10, 30));
        mDut.recordResult(requestAp5, getDummyRangingResults(requestAp5, 0.3, -0.2));
        mDut.recordResult(requestAp6, getDummyRangingResults(requestAp6, 40, 30));
        log = mDut.consolidateProto();

        checkMainStats("Sequence AP-only", log, 0, 0);

        checkPeerStats("Sequence AP-only: AP", log.rttToAp, 0, 0, 0, 0, 0, 1, 6, 0);

        validateProtoIndividualStatusHistBucket(
                "Sequence AP-only: rttToAp.histogramIndividualStatus[0]",
                log.rttToAp.histogramIndividualStatus[0], WifiMetricsProto.WifiRttLog.SUCCESS, 14);

        validateProtoHistBucket("Sequence AP-only: rttToAp.histogramDistance[0]",
                log.rttToAp.histogramDistance[0], Integer.MIN_VALUE, 0, 3);
        validateProtoHistBucket("Sequence AP-only: rttToAp.histogramDistance[1]",
                log.rttToAp.histogramDistance[1], 0, 5 * 1000, 2);
        validateProtoHistBucket("Sequence AP-only: rttToAp.histogramDistance[2]",
                log.rttToAp.histogramDistance[2], 5 * 1000, 15 * 1000, 2);
        validateProtoHistBucket("Sequence AP-only: rttToAp.histogramDistance[3]",
                log.rttToAp.histogramDistance[3], 30 * 1000, 60 * 1000, 2);
        validateProtoHistBucket("Sequence AP-only: rttToAp.histogramDistance[4]",
                log.rttToAp.histogramDistance[4], 60 * 1000, 100 * 1000, 1);
        validateProtoHistBucket("Sequence AP-only: rttToAp.histogramDistance[5]",
                log.rttToAp.histogramDistance[5], 100 * 1000, Integer.MAX_VALUE, 4);

        checkPeerStats("Sequence AP-only: Aware", log.rttToAware, 0, 0, 0, 0, 0, 0, 0, 0);

        // mix of AP and Aware requests
        RangingRequest requestMixed03 = getDummyRangingRequest(0, 3);
        RangingRequest requestMixed25 = getDummyRangingRequest(2, 5);
        RangingRequest requestMixed50 = getDummyRangingRequest(5, 0);
        RangingRequest requestMixed08 = getDummyRangingRequest(0, 8);

        mDut.clear();
        mDut.recordResult(requestMixed03, getDummyRangingResults(requestMixed03, 5, 0));
        mDut.recordResult(requestMixed25, getDummyRangingResults(requestMixed25, 10, 30));
        mDut.recordResult(requestMixed50, getDummyRangingResults(requestMixed50, 0.3, -0.2));
        mDut.recordResult(requestMixed08, getDummyRangingResults(requestMixed08, 40, 30));
        log = mDut.consolidateProto();

        checkMainStats("Sequence Mixed AP/Aware", log, 0, 0);

        checkPeerStats("Sequence Mixed AP/Aware: AP", log.rttToAp, 0, 0, 0, 0, 0, 1, 4, 0);

        validateProtoIndividualStatusHistBucket(
                "Sequence Mixed AP/Aware: rttToAp.histogramIndividualStatus[0]",
                log.rttToAp.histogramIndividualStatus[0], WifiMetricsProto.WifiRttLog.SUCCESS, 7);

        validateProtoHistBucket("Sequence Mixed AP/Aware: rttToAp.histogramDistance[0]",
                log.rttToAp.histogramDistance[0], Integer.MIN_VALUE, 0, 3);
        validateProtoHistBucket("Sequence Mixed AP/Aware: rttToAp.histogramDistance[1]",
                log.rttToAp.histogramDistance[1], 0, 5 * 1000, 2);
        validateProtoHistBucket("Sequence Mixed AP/Aware: rttToAp.histogramDistance[2]",
                log.rttToAp.histogramDistance[2], 5 * 1000, 15 * 1000, 1);
        validateProtoHistBucket("Sequence Mixed AP/Aware: rttToAp.histogramDistance[3]",
                log.rttToAp.histogramDistance[3], 30 * 1000, 60 * 1000, 1);

        checkPeerStats("Sequence Mixed AP/Aware: Aware", log.rttToAware, 0, 0, 0, 0, 0, 1, 4, 0);

        validateProtoIndividualStatusHistBucket(
                "Sequence Mixed AP/Aware: rttToAware.histogramIndividualStatus[0]",
                log.rttToAware.histogramIndividualStatus[0], WifiMetricsProto.WifiRttLog.SUCCESS,
                16);

        validateProtoHistBucket("Sequence Mixed AP/Aware: rttToAware.histogramDistance[0]",
                log.rttToAware.histogramDistance[0], 5 * 1000, 15 * 1000, 3);
        validateProtoHistBucket("Sequence Mixed AP/Aware: rttToAware.histogramDistance[1]",
                log.rttToAware.histogramDistance[1], 30 * 1000, 60 * 1000, 1);
        validateProtoHistBucket("Sequence Mixed AP/Aware: rttToAware.histogramDistance[2]",
                log.rttToAware.histogramDistance[2], 60 * 1000, 100 * 1000, 2);
        validateProtoHistBucket("Sequence Mixed AP/Aware: rttToAware.histogramDistance[3]",
                log.rttToAware.histogramDistance[3], 100 * 1000, Integer.MAX_VALUE, 10);
    }

    /**
     * Verify the behavior when the HAL returns with missing results or some results set to null.
     */
    @Test
    public void testRecordMissingResults() {
        WifiMetricsProto.WifiRttLog log;

        mDut.clear();
        RangingRequest requestMixed25 = getDummyRangingRequest(2, 5);
        List<RttResult> resultMixed25 = getDummyRangingResults(requestMixed25, 10, 30);
        // remove some results
        resultMixed25.remove(3); // Second Aware result: distance = 100
        resultMixed25.remove(0); // First AP result: distance = 10
        resultMixed25.add(null);
        mDut.recordResult(requestMixed25, resultMixed25);

        log = mDut.consolidateProto();

        checkMainStats("Sequence Mixed AP/Aware", log, 0, 0);

        checkPeerStats("Sequence Mixed AP/Aware: AP", log.rttToAp, 0, 0, 0, 0, 0, 2, 1, 0);

        validateProtoIndividualStatusHistBucket(
                "Sequence Mixed AP/Aware: rttToAp.histogramIndividualStatus[0]",
                log.rttToAp.histogramIndividualStatus[0], WifiMetricsProto.WifiRttLog.SUCCESS, 1);
        validateProtoIndividualStatusHistBucket(
                "Sequence Mixed AP/Aware: rttToAp.histogramIndividualStatus[1]",
                log.rttToAp.histogramIndividualStatus[1],
                WifiMetricsProto.WifiRttLog.MISSING_RESULT, 1);

        validateProtoHistBucket("Sequence Mixed AP/Aware: rttToAp.histogramDistance[0]",
                log.rttToAp.histogramDistance[0], 30 * 1000, 60 * 1000, 1);

        checkPeerStats("Sequence Mixed AP/Aware: Aware", log.rttToAware, 0, 0, 0, 0, 0, 2, 2, 0);

        validateProtoIndividualStatusHistBucket(
                "Sequence Mixed AP/Aware: rttToAware.histogramIndividualStatus[0]",
                log.rttToAware.histogramIndividualStatus[0], WifiMetricsProto.WifiRttLog.SUCCESS,
                4);
        validateProtoIndividualStatusHistBucket(
                "Sequence Mixed AP/Aware: rttToAware.histogramIndividualStatus[1]",
                log.rttToAware.histogramIndividualStatus[1],
                WifiMetricsProto.WifiRttLog.MISSING_RESULT, 1);

        validateProtoHistBucket("Sequence Mixed AP/Aware: rttToAware.histogramDistance[0]",
                log.rttToAware.histogramDistance[0], 60 * 1000, 100 * 1000, 1);
        validateProtoHistBucket("Sequence Mixed AP/Aware: rttToAware.histogramDistance[1]",
                log.rttToAware.histogramDistance[1], 100 * 1000, Integer.MAX_VALUE, 3);
    }

    /**
     * Verify the behavior when the HAL returns with NULL array.
     */
    @Test
    public void testRecordNullArrayResults() {
        WifiMetricsProto.WifiRttLog log;

        mDut.clear();
        RangingRequest requestMixed25 = getDummyRangingRequest(2, 5);
        mDut.recordResult(requestMixed25, null);

        log = mDut.consolidateProto();

        checkMainStats("Sequence Mixed AP/Aware", log, 0, 0);

        checkPeerStats("Sequence Mixed AP/Aware: AP", log.rttToAp, 0, 0, 0, 0, 0, 1, 0, 0);

        validateProtoIndividualStatusHistBucket(
                "Sequence Mixed AP/Aware: rttToAp.histogramIndividualStatus[1]",
                log.rttToAp.histogramIndividualStatus[0],
                WifiMetricsProto.WifiRttLog.MISSING_RESULT, 2);

        checkPeerStats("Sequence Mixed AP/Aware: Aware", log.rttToAware, 0, 0, 0, 0, 0, 1, 0, 0);

        validateProtoIndividualStatusHistBucket(
                "Sequence Mixed AP/Aware: rttToAware.histogramIndividualStatus[0]",
                log.rttToAware.histogramIndividualStatus[0],
                WifiMetricsProto.WifiRttLog.MISSING_RESULT, 5);
    }

    /**
     * Verify that all individual status codes are translated correctly.
     */
    @Test
    public void testRecordResultsStatuses() {
        WifiMetricsProto.WifiRttLog log;

        mDut.clear();

        recordResultNTimes(RttStatus.SUCCESS, 5);
        recordResultNTimes(RttStatus.FAILURE, 6);
        recordResultNTimes(RttStatus.FAIL_NO_RSP, 7);
        recordResultNTimes(RttStatus.FAIL_REJECTED, 8);
        recordResultNTimes(RttStatus.FAIL_NOT_SCHEDULED_YET, 9);
        recordResultNTimes(RttStatus.FAIL_TM_TIMEOUT, 10);
        recordResultNTimes(RttStatus.FAIL_AP_ON_DIFF_CHANNEL, 11);
        recordResultNTimes(RttStatus.FAIL_NO_CAPABILITY, 12);
        recordResultNTimes(RttStatus.ABORTED, 13);
        recordResultNTimes(RttStatus.FAIL_INVALID_TS, 14);
        recordResultNTimes(RttStatus.FAIL_PROTOCOL, 15);
        recordResultNTimes(RttStatus.FAIL_SCHEDULE, 16);
        recordResultNTimes(RttStatus.FAIL_BUSY_TRY_LATER, 17);
        recordResultNTimes(RttStatus.INVALID_REQ, 18);
        recordResultNTimes(RttStatus.NO_WIFI, 19);
        recordResultNTimes(RttStatus.FAIL_FTM_PARAM_OVERRIDE, 20);

        log = mDut.consolidateProto();

        collector.checkThat("AP histogramIndividualStatus.length",
                log.rttToAp.histogramIndividualStatus.length, equalTo(16));

        validateProtoIndividualStatusHistBucket("rttToAp.histogramIndividualStatus[0]",
                log.rttToAp.histogramIndividualStatus[0], WifiMetricsProto.WifiRttLog.SUCCESS, 5);
        validateProtoIndividualStatusHistBucket("rttToAp.histogramIndividualStatus[1]",
                log.rttToAp.histogramIndividualStatus[1], WifiMetricsProto.WifiRttLog.FAILURE, 6);
        validateProtoIndividualStatusHistBucket("rttToAp.histogramIndividualStatus[2]",
                log.rttToAp.histogramIndividualStatus[2], WifiMetricsProto.WifiRttLog.FAIL_NO_RSP,
                7);
        validateProtoIndividualStatusHistBucket("rttToAp.histogramIndividualStatus[3]",
                log.rttToAp.histogramIndividualStatus[3], WifiMetricsProto.WifiRttLog.FAIL_REJECTED,
                8);
        validateProtoIndividualStatusHistBucket("rttToAp.histogramIndividualStatus[4]",
                log.rttToAp.histogramIndividualStatus[4],
                WifiMetricsProto.WifiRttLog.FAIL_NOT_SCHEDULED_YET, 9);
        validateProtoIndividualStatusHistBucket("rttToAp.histogramIndividualStatus[5]",
                log.rttToAp.histogramIndividualStatus[5],
                WifiMetricsProto.WifiRttLog.FAIL_TM_TIMEOUT, 10);
        validateProtoIndividualStatusHistBucket("rttToAp.histogramIndividualStatus[6]",
                log.rttToAp.histogramIndividualStatus[6],
                WifiMetricsProto.WifiRttLog.FAIL_AP_ON_DIFF_CHANNEL, 11);
        validateProtoIndividualStatusHistBucket("rttToAp.histogramIndividualStatus[7]",
                log.rttToAp.histogramIndividualStatus[7],
                WifiMetricsProto.WifiRttLog.FAIL_NO_CAPABILITY, 12);
        validateProtoIndividualStatusHistBucket("rttToAp.histogramIndividualStatus[8]",
                log.rttToAp.histogramIndividualStatus[8], WifiMetricsProto.WifiRttLog.ABORTED, 13);
        validateProtoIndividualStatusHistBucket("rttToAp.histogramIndividualStatus[9]",
                log.rttToAp.histogramIndividualStatus[9],
                WifiMetricsProto.WifiRttLog.FAIL_INVALID_TS, 14);
        validateProtoIndividualStatusHistBucket("rttToAp.histogramIndividualStatus[10]",
                log.rttToAp.histogramIndividualStatus[10],
                WifiMetricsProto.WifiRttLog.FAIL_PROTOCOL, 15);
        validateProtoIndividualStatusHistBucket("rttToAp.histogramIndividualStatus[11]",
                log.rttToAp.histogramIndividualStatus[11],
                WifiMetricsProto.WifiRttLog.FAIL_SCHEDULE, 16);
        validateProtoIndividualStatusHistBucket("rttToAp.histogramIndividualStatus[12]",
                log.rttToAp.histogramIndividualStatus[12],
                WifiMetricsProto.WifiRttLog.FAIL_BUSY_TRY_LATER, 17);
        validateProtoIndividualStatusHistBucket("rttToAp.histogramIndividualStatus[13]",
                log.rttToAp.histogramIndividualStatus[13], WifiMetricsProto.WifiRttLog.INVALID_REQ,
                18);
        validateProtoIndividualStatusHistBucket("rttToAp.histogramIndividualStatus[14]",
                log.rttToAp.histogramIndividualStatus[14], WifiMetricsProto.WifiRttLog.NO_WIFI, 19);
        validateProtoIndividualStatusHistBucket("rttToAp.histogramIndividualStatus[15]",
                log.rttToAp.histogramIndividualStatus[15],
                WifiMetricsProto.WifiRttLog.FAIL_FTM_PARAM_OVERRIDE, 20);

        collector.checkThat("Aware histogramIndividualStatus.length",
                log.rttToAware.histogramIndividualStatus.length, equalTo(0));
    }

    /**
     * Verify that all overall status codes are recorded correctly.
     */
    @Test
    public void testRecordOverallStatus() {
        WifiMetricsProto.WifiRttLog log;

        mDut.clear();

        recordOverallStatusNTimes(WifiMetricsProto.WifiRttLog.OVERALL_SUCCESS, 5);
        recordOverallStatusNTimes(WifiMetricsProto.WifiRttLog.OVERALL_FAIL, 6);
        recordOverallStatusNTimes(WifiMetricsProto.WifiRttLog.OVERALL_RTT_NOT_AVAILABLE, 7);
        recordOverallStatusNTimes(WifiMetricsProto.WifiRttLog.OVERALL_TIMEOUT, 8);
        recordOverallStatusNTimes(WifiMetricsProto.WifiRttLog.OVERALL_THROTTLE, 9);
        recordOverallStatusNTimes(WifiMetricsProto.WifiRttLog.OVERALL_HAL_FAILURE, 10);
        recordOverallStatusNTimes(WifiMetricsProto.WifiRttLog.OVERALL_AWARE_TRANSLATION_FAILURE,
                11);
        recordOverallStatusNTimes(WifiMetricsProto.WifiRttLog.OVERALL_LOCATION_PERMISSION_MISSING,
                12);

        log = mDut.consolidateProto();

        collector.checkThat("histogramOverallStatus.length", log.histogramOverallStatus.length,
                equalTo(8));

        validateProtoOverallStatusHistBucket("histogramOverallStatus[0]",
                log.histogramOverallStatus[0], WifiMetricsProto.WifiRttLog.OVERALL_SUCCESS, 5);
        validateProtoOverallStatusHistBucket("histogramOverallStatus[1]",
                log.histogramOverallStatus[1], WifiMetricsProto.WifiRttLog.OVERALL_FAIL, 6);
        validateProtoOverallStatusHistBucket("histogramOverallStatus[2]",
                log.histogramOverallStatus[2],
                WifiMetricsProto.WifiRttLog.OVERALL_RTT_NOT_AVAILABLE, 7);
        validateProtoOverallStatusHistBucket("histogramOverallStatus[3]",
                log.histogramOverallStatus[3], WifiMetricsProto.WifiRttLog.OVERALL_TIMEOUT, 8);
        validateProtoOverallStatusHistBucket("histogramOverallStatus[4]",
                log.histogramOverallStatus[4], WifiMetricsProto.WifiRttLog.OVERALL_THROTTLE, 9);
        validateProtoOverallStatusHistBucket("histogramOverallStatus[5]",
                log.histogramOverallStatus[5], WifiMetricsProto.WifiRttLog.OVERALL_HAL_FAILURE, 10);
        validateProtoOverallStatusHistBucket("histogramOverallStatus[6]",
                log.histogramOverallStatus[6],
                WifiMetricsProto.WifiRttLog.OVERALL_AWARE_TRANSLATION_FAILURE, 11);
        validateProtoOverallStatusHistBucket("histogramOverallStatus[7]",
                log.histogramOverallStatus[7],
                WifiMetricsProto.WifiRttLog.OVERALL_LOCATION_PERMISSION_MISSING, 12);
    }

    // Utilities

    /**
     * Mock the elapsed time since boot to the input argument.
     */
    private void setTime(long timeMs) {
        when(mClock.getElapsedSinceBootMillis()).thenReturn(timeMs);
    }

    private void validateProtoHistBucket(String logPrefix,
            WifiMetricsProto.WifiRttLog.HistogramBucket bucket, long start, long end, int count) {
        collector.checkThat(logPrefix + ": start", bucket.start, equalTo(start));
        collector.checkThat(logPrefix + ": end", bucket.end, equalTo(end));
        collector.checkThat(logPrefix + ": count", bucket.count, equalTo(count));
    }

    private void validateProtoOverallStatusHistBucket(String logPrefix,
            WifiMetricsProto.WifiRttLog.RttOverallStatusHistogramBucket bucket, int status,
            int count) {
        collector.checkThat(logPrefix + ": statusType", bucket.statusType, equalTo(status));
        collector.checkThat(logPrefix + ": count", bucket.count, equalTo(count));
    }

    private void validateProtoIndividualStatusHistBucket(String logPrefix,
            WifiMetricsProto.WifiRttLog.RttIndividualStatusHistogramBucket bucket, int status,
            int count) {
        collector.checkThat(logPrefix + ": statusType", bucket.statusType, equalTo(status));
        collector.checkThat(logPrefix + ": count", bucket.count, equalTo(count));
    }

    private void checkMainStats(String msgPrefix, WifiMetricsProto.WifiRttLog log, int numRequests,
            int histogramOverallStatusLength) {
        collector.checkThat(msgPrefix + ": numRequests", log.numRequests, equalTo(numRequests));
        collector.checkThat(msgPrefix + ": histogramOverallStatus.length",
                log.histogramOverallStatus.length,
                equalTo(histogramOverallStatusLength));
    }

    private void checkPeerStats(String msgPrefix, WifiMetricsProto.WifiRttLog.RttToPeerLog log,
            int numRequests, int numIndividualRequests,
            int numApps, int histogramNumRequestsPerAppLength,
            int histogramNumPeersPerRequestLength, int histogramIndividualStatusLength,
            int histogramDistanceLength, int histogramRequestIntervalMsLength) {
        collector.checkThat(msgPrefix + ": numRequests", log.numRequests, equalTo(numRequests));
        collector.checkThat(msgPrefix + ": numIndividualRequests", log.numIndividualRequests,
                equalTo(numIndividualRequests));
        collector.checkThat(msgPrefix + ": numApps", log.numApps, equalTo(numApps));
        collector.checkThat(msgPrefix + ": histogramNumRequestsPerApp.length",
                log.histogramNumRequestsPerApp.length, equalTo(histogramNumRequestsPerAppLength));
        collector.checkThat(msgPrefix + ": histogramNumPeersPerRequest.length",
                log.histogramNumPeersPerRequest.length, equalTo(histogramNumPeersPerRequestLength));
        collector.checkThat(msgPrefix + ": histogramIndividualStatus.length",
                log.histogramIndividualStatus.length, equalTo(histogramIndividualStatusLength));
        collector.checkThat(msgPrefix + ": histogramDistance.length",
                log.histogramDistance.length, equalTo(histogramDistanceLength));
        collector.checkThat(msgPrefix + ": histogramRequestIntervalMs.length",
                log.histogramRequestIntervalMs.length, equalTo(histogramRequestIntervalMsLength));
    }

    private RangingRequest getDummyRangingRequest(int countAp, int countAware) {
        RangingRequest.Builder builder = new RangingRequest.Builder();
        byte[] dummyMacBase = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5};

        for (int i = 0; i < countAp; ++i) {
            dummyMacBase[0]++;
            builder.addResponder(new ResponderConfig(MacAddress.fromBytes(dummyMacBase),
                    ResponderConfig.RESPONDER_AP, true, 0, 0, 0, 0, 0));
        }
        for (int i = 0; i < countAware; ++i) {
            dummyMacBase[0]++;
            builder.addResponder(new ResponderConfig(MacAddress.fromBytes(dummyMacBase),
                    ResponderConfig.RESPONDER_AWARE, true, 0, 0, 0, 0, 0));
        }

        return builder.build();
    }

    private List<RttResult> getDummyRangingResults(RangingRequest request, double baseDistanceM,
            double incrDistanceM) {
        List<RttResult> halResults = new ArrayList<>();
        double distance = baseDistanceM;

        for (ResponderConfig peer : request.mRttPeers) {
            RttResult rttResult = new RttResult();
            rttResult.status = RttStatus.SUCCESS;
            System.arraycopy(peer.macAddress.toByteArray(), 0, rttResult.addr, 0, 6);
            rttResult.distanceInMm = (int) (distance * 1000);
            distance += incrDistanceM;
            halResults.add(rttResult);
        }

        return halResults;
    }

    private void recordResultNTimes(int status, int n) {
        RangingRequest request = getDummyRangingRequest(1, 0);
        List<RttResult> results = getDummyRangingResults(request, 0, 0);
        RttResult result = results.get(0);
        result.status = status;

        for (int i = 0; i < n; ++i) {
            mDut.recordResult(request, results);
        }
    }

    private void recordOverallStatusNTimes(int status, int n) {
        for (int i = 0; i < n; ++i) {
            mDut.recordOverallStatus(status);
        }
    }

    private void dumpDut(String prefix) {
        StringWriter sw = new StringWriter();
        mDut.dump(null, new PrintWriter(sw), null);
        Log.e("RttMetrics", prefix + sw.toString());
    }
}
