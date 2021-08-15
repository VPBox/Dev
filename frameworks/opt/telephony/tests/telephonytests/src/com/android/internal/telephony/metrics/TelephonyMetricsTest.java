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

import static android.telephony.ServiceState.RIL_RADIO_TECHNOLOGY_LTE;
import static android.telephony.ServiceState.ROAMING_TYPE_DOMESTIC;

import static com.android.internal.telephony.RILConstants.RIL_REQUEST_DEACTIVATE_DATA_CALL;
import static com.android.internal.telephony.RILConstants.RIL_REQUEST_SEND_SMS;
import static com.android.internal.telephony.RILConstants.RIL_REQUEST_SETUP_DATA_CALL;
import static com.android.internal.telephony.dataconnection.DcTrackerTest.FAKE_ADDRESS;
import static com.android.internal.telephony.dataconnection.DcTrackerTest.FAKE_DNS;
import static com.android.internal.telephony.dataconnection.DcTrackerTest.FAKE_GATEWAY;
import static com.android.internal.telephony.dataconnection.DcTrackerTest.FAKE_IFNAME;
import static com.android.internal.telephony.dataconnection.DcTrackerTest.FAKE_PCSCF_ADDRESS;
import static com.android.internal.telephony.nano.TelephonyProto.PdpType.PDP_TYPE_IPV4V6;

import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.mockito.Mockito.doReturn;

import android.net.LinkAddress;
import android.net.NetworkUtils;
import android.telephony.ServiceState;
import android.telephony.TelephonyManager;
import android.telephony.data.ApnSetting;
import android.telephony.data.DataCallResponse;
import android.telephony.emergency.EmergencyNumber;
import android.telephony.ims.ImsCallSession;
import android.telephony.ims.ImsReasonInfo;
import android.telephony.ims.feature.MmTelFeature;
import android.telephony.ims.stub.ImsRegistrationImplBase;
import android.test.suitebuilder.annotation.SmallTest;
import android.util.Base64;

import com.android.internal.telephony.Call;
import com.android.internal.telephony.CarrierResolver;
import com.android.internal.telephony.GsmCdmaConnection;
import com.android.internal.telephony.PhoneConstants;
import com.android.internal.telephony.SmsResponse;
import com.android.internal.telephony.TelephonyTest;
import com.android.internal.telephony.UUSInfo;
import com.android.internal.telephony.nano.TelephonyProto;
import com.android.internal.telephony.nano.TelephonyProto.ImsConnectionState;
import com.android.internal.telephony.nano.TelephonyProto.RadioAccessTechnology;
import com.android.internal.telephony.nano.TelephonyProto.SmsSession;
import com.android.internal.telephony.nano.TelephonyProto.TelephonyCallSession;
import com.android.internal.telephony.nano.TelephonyProto.TelephonyCallSession.Event.CallState;
import com.android.internal.telephony.nano.TelephonyProto.TelephonyCallSession.Event.ImsCommand;
import com.android.internal.telephony.nano.TelephonyProto.TelephonyCallSession.Event.RilCall;
import com.android.internal.telephony.nano.TelephonyProto.TelephonyEvent;
import com.android.internal.telephony.nano.TelephonyProto.TelephonyLog;
import com.android.internal.telephony.nano.TelephonyProto.TelephonyServiceState;
import com.android.internal.telephony.nano.TelephonyProto.TelephonyServiceState.RoamingType;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;

import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Arrays;

public class TelephonyMetricsTest extends TelephonyTest {

    @Mock
    private ImsCallSession mImsCallSession;

    @Mock
    private ServiceState mServiceState;

    @Mock
    private GsmCdmaConnection mConnection;

    private TelephonyMetrics mMetrics;

    private UUSInfo mUusInfo;

    private ImsReasonInfo mImsReasonInfo;

    @Before
    public void setUp() throws Exception {
        super.setUp(getClass().getSimpleName());
        mMetrics = new TelephonyMetrics();
        mUusInfo = new UUSInfo(1, 2, new byte[]{1, 2});
        doReturn("123").when(mImsCallSession).getCallId();
        mImsReasonInfo = new ImsReasonInfo();
        mImsReasonInfo.mExtraMessage = "extramessage";
        mImsReasonInfo.mCode = 123;
        mImsReasonInfo.mExtraCode = 456;

        doReturn(ROAMING_TYPE_DOMESTIC).when(mServiceState).getVoiceRoamingType();
        doReturn(ROAMING_TYPE_DOMESTIC).when(mServiceState).getDataRoamingType();
        doReturn("voiceshort").when(mServiceState).getVoiceOperatorAlphaShort();
        doReturn("voicelong").when(mServiceState).getVoiceOperatorAlphaLong();
        doReturn("datashort").when(mServiceState).getDataOperatorAlphaShort();
        doReturn("datalong").when(mServiceState).getDataOperatorAlphaLong();
        doReturn("123456").when(mServiceState).getVoiceOperatorNumeric();
        doReturn("123456").when(mServiceState).getDataOperatorNumeric();
        doReturn(RIL_RADIO_TECHNOLOGY_LTE).when(mServiceState).getRilVoiceRadioTechnology();
        doReturn(RIL_RADIO_TECHNOLOGY_LTE).when(mServiceState).getRilDataRadioTechnology();
    }

    @After
    public void tearDown() throws Exception {
        super.tearDown();
    }

    private TelephonyLog buildProto() throws Exception {
        Method method = TelephonyMetrics.class.getDeclaredMethod("buildProto");
        method.setAccessible(true);
        return (TelephonyLog) method.invoke(mMetrics);
    }

    private void reset() throws Exception {
        Method method = TelephonyMetrics.class.getDeclaredMethod("reset");
        method.setAccessible(true);
        method.invoke(mMetrics);
    }

    private String convertProtoToBase64String(TelephonyLog log) throws Exception {
        Class[] cArgs = new Class[1];
        cArgs[0] = TelephonyLog.class;
        Method method = TelephonyMetrics.class.getDeclaredMethod("convertProtoToBase64String",
                cArgs);
        method.setAccessible(true);
        return (String) method.invoke(null, log);
    }

    @Test
    @SmallTest
    public void testEventDropped() throws Exception {
        for (int i = 0; i < 1001; i++) {
            mMetrics.writeDataStallEvent(mPhone.getPhoneId(), i);
        }
        TelephonyLog log = buildProto();
        assertEquals(1000, log.events.length);
        assertEquals(0, log.callSessions.length);
        assertEquals(0, log.smsSessions.length);
        assertTrue(log.eventsDropped);
        assertEquals(1, log.events[0].dataStallAction);
    }

    // Test write data stall event
    @Test
    @SmallTest
    public void testWriteDataStallEvent() throws Exception {
        mMetrics.writeDataStallEvent(mPhone.getPhoneId(), 3);
        TelephonyLog log = buildProto();

        assertEquals(1, log.events.length);
        assertEquals(0, log.callSessions.length);
        assertEquals(0, log.smsSessions.length);
        assertEquals(mPhone.getPhoneId(), log.events[0].phoneId);
        assertEquals(3, log.events[0].dataStallAction);
    }

    // Test write modem restart event
    @Test
    @SmallTest
    public void testModemRestartEvent() throws Exception {
        mMetrics.writeModemRestartEvent(mPhone.getPhoneId(), "Test");
        TelephonyLog log = buildProto();

        assertEquals(1, log.events.length);
        assertEquals(0, log.callSessions.length);
        assertEquals(0, log.smsSessions.length);

        assertEquals(mPhone.getPhoneId(), log.events[0].phoneId);
        assertEquals("Test", log.events[0].modemRestart.reason);
    }

    // Test write Carrier Identification matching event
    @Test
    @SmallTest
    public void testWriteCarrierIdMatchingEventWithInvalidMatchingScore() throws Exception {
        CarrierResolver.CarrierMatchingRule simInfo = new CarrierResolver.CarrierMatchingRule(
                "mccmncTest",
                "imsiPrefixTest",
                "iccidPrefixTest",
                "gid1Test",
                "gid2Test",
                "plmnTest",
                "spnTest",
                "apnTest",
                new ArrayList<>(),
                -1, null, -1);

        mMetrics.writeCarrierIdMatchingEvent(mPhone.getPhoneId(), 1,
                TelephonyManager.UNKNOWN_CARRIER_ID,
                "unknownMccmncTest",
                "unknownGid1Test",
                simInfo);
        TelephonyLog log = buildProto();

        assertEquals(1, log.events.length);
        assertEquals(0, log.callSessions.length);
        assertEquals(0, log.smsSessions.length);

        assertEquals(mPhone.getPhoneId(), log.events[0].phoneId);
        assertEquals(1, log.events[0].carrierIdMatching.cidTableVersion);
        assertEquals(TelephonyEvent.Type.CARRIER_ID_MATCHING, log.events[0].type);
        assertEquals("unknownMccmncTest", log.events[0].carrierIdMatching.result.unknownMccmnc);
        assertTrue(log.events[0].carrierIdMatching.result.unknownGid1.isEmpty());
        assertEquals("iccidPrefixTest", log.events[0].carrierIdMatching.result.iccidPrefix);
        assertEquals("imsiPrefixTest", log.events[0].carrierIdMatching.result.imsiPrefix);
        assertEquals("spnTest", log.events[0].carrierIdMatching.result.spn);
        assertEquals("plmnTest", log.events[0].carrierIdMatching.result.pnn);
        assertEquals("apnTest", log.events[0].carrierIdMatching.result.preferApn);
        assertEquals("mccmncTest", log.events[0].carrierIdMatching.result.mccmnc);
        assertEquals("gid1Test", log.events[0].carrierIdMatching.result.gid1);
    }

    // Test write Carrier Identification matching event
    @Test
    @SmallTest
    public void testWriteCarrierIdMatchingEvent() throws Exception {
        CarrierResolver.CarrierMatchingRule simInfo = new CarrierResolver.CarrierMatchingRule(
                "mccmncTest",
                "imsiPrefixTest",
                "iccidPrefixTest",
                "gid1Test",
                "gid2Test",
                "plmnTest",
                "spnTest",
                "apnTest",
                new ArrayList<>(),
                -1, null, -1);

        mMetrics.writeCarrierIdMatchingEvent(mPhone.getPhoneId(), 1, 1,
                "unknownMccmncTest", "unknownGid1Test", simInfo);
        TelephonyLog log = buildProto();

        assertEquals(1, log.events.length);
        assertEquals(0, log.callSessions.length);
        assertEquals(0, log.smsSessions.length);

        assertEquals(mPhone.getPhoneId(), log.events[0].phoneId);
        assertEquals(TelephonyEvent.Type.CARRIER_ID_MATCHING, log.events[0].type);
        assertEquals(1, log.events[0].carrierIdMatching.cidTableVersion);
        assertEquals(1, log.events[0].carrierIdMatching.result.carrierId);
        assertEquals("unknownMccmncTest", log.events[0].carrierIdMatching.result.unknownMccmnc);
        assertEquals("unknownGid1Test", log.events[0].carrierIdMatching.result.unknownGid1);
        assertEquals("iccidPrefixTest", log.events[0].carrierIdMatching.result.iccidPrefix);
        assertEquals("imsiPrefixTest", log.events[0].carrierIdMatching.result.imsiPrefix);
        assertEquals("spnTest", log.events[0].carrierIdMatching.result.spn);
        assertEquals("plmnTest", log.events[0].carrierIdMatching.result.pnn);
        assertEquals("apnTest", log.events[0].carrierIdMatching.result.preferApn);
        assertEquals("mccmncTest", log.events[0].carrierIdMatching.result.mccmnc);
        assertEquals("gid1Test", log.events[0].carrierIdMatching.result.gid1);
    }

    // Test write Emergency Number update event
    @Test
    @SmallTest
    public void testWriteEmergencyNumberUpdateEvent() throws Exception {
        EmergencyNumber number = new EmergencyNumber(
                "911",
                "us",
                "30",
                EmergencyNumber.EMERGENCY_SERVICE_CATEGORY_UNSPECIFIED,
                new ArrayList<String>(),
                EmergencyNumber.EMERGENCY_NUMBER_SOURCE_NETWORK_SIGNALING,
                EmergencyNumber.EMERGENCY_CALL_ROUTING_NORMAL);

        mMetrics.writeEmergencyNumberUpdateEvent(mPhone.getPhoneId(), number);
        TelephonyLog log = buildProto();

        assertEquals(1, log.events.length);
        assertEquals(0, log.callSessions.length);
        assertEquals(0, log.smsSessions.length);

        assertEquals(mPhone.getPhoneId(), log.events[0].phoneId);
        assertEquals(TelephonyEvent.Type.EMERGENCY_NUMBER_REPORT, log.events[0].type);
        assertEquals("911", log.events[0].updatedEmergencyNumber.address);
        assertEquals("30", log.events[0].updatedEmergencyNumber.mnc);
        assertEquals(EmergencyNumber.EMERGENCY_SERVICE_CATEGORY_UNSPECIFIED,
                log.events[0].updatedEmergencyNumber.serviceCategoriesBitmask);
        assertEquals(0, log.events[0].updatedEmergencyNumber.urns.length);
        assertEquals(EmergencyNumber.EMERGENCY_NUMBER_SOURCE_NETWORK_SIGNALING,
                log.events[0].updatedEmergencyNumber.numberSourcesBitmask);
        assertEquals(EmergencyNumber.EMERGENCY_CALL_ROUTING_NORMAL,
                log.events[0].updatedEmergencyNumber.routing);
    }

    // Test write on IMS call start
    @Test
    @SmallTest
    public void testWriteOnImsCallStart() throws Exception {
        mMetrics.writeOnImsCallStart(mPhone.getPhoneId(), mImsCallSession);
        mMetrics.writePhoneState(mPhone.getPhoneId(), PhoneConstants.State.IDLE);
        TelephonyLog log = buildProto();

        assertEquals(0, log.events.length);
        assertEquals(1, log.callSessions.length);
        assertEquals(0, log.smsSessions.length);

        assertEquals(mPhone.getPhoneId(), log.callSessions[0].phoneId);

        assertFalse(log.callSessions[0].eventsDropped);

        assertEquals(1, log.callSessions[0].events.length);

        assertEquals(123, log.callSessions[0].events[0].callIndex);

        assertEquals(ImsCommand.IMS_CMD_START, log.callSessions[0].events[0].imsCommand);
    }

    // Test write on IMS call received
    @Test
    @SmallTest
    public void testWriteOnImsCallReceive() throws Exception {
        mMetrics.writeOnImsCallReceive(mPhone.getPhoneId(), mImsCallSession);
        mMetrics.writePhoneState(mPhone.getPhoneId(), PhoneConstants.State.IDLE);
        TelephonyLog log = buildProto();

        assertEquals(0, log.events.length);
        assertEquals(1, log.callSessions.length);
        assertEquals(0, log.smsSessions.length);
        assertEquals(mPhone.getPhoneId(), log.callSessions[0].phoneId);

        assertFalse(log.callSessions[0].eventsDropped);

        assertEquals(1, log.callSessions[0].events.length);

        assertEquals(123, log.callSessions[0].events[0].callIndex);
    }

    // Test write ims call state
    @Test
    @SmallTest
    public void testWriteImsCallState() throws Exception {
        mMetrics.writeOnImsCallStart(mPhone.getPhoneId(), mImsCallSession);
        mMetrics.writeImsCallState(mPhone.getPhoneId(), mImsCallSession, Call.State.ACTIVE);
        mMetrics.writePhoneState(mPhone.getPhoneId(), PhoneConstants.State.IDLE);
        TelephonyLog log = buildProto();

        assertEquals(0, log.events.length);
        assertEquals(1, log.callSessions.length);
        assertEquals(0, log.smsSessions.length);
        assertEquals(2, log.callSessions[0].events.length);
        assertFalse(log.callSessions[0].eventsDropped);

        assertEquals(123, log.callSessions[0].events[1].callIndex);

        assertEquals(CallState.CALL_ACTIVE, log.callSessions[0].events[1].callState);
    }

    // Test write ims set feature value
    @Test
    @SmallTest
    public void testWriteImsSetFeatureValue() throws Exception {
        mMetrics.writeOnImsCallStart(mPhone.getPhoneId(), mImsCallSession);
        mMetrics.writeImsSetFeatureValue(mPhone.getPhoneId(),
                MmTelFeature.MmTelCapabilities.CAPABILITY_TYPE_VOICE,
                ImsRegistrationImplBase.REGISTRATION_TECH_LTE, 1);
        mMetrics.writeImsSetFeatureValue(mPhone.getPhoneId(),
                MmTelFeature.MmTelCapabilities.CAPABILITY_TYPE_VOICE,
                ImsRegistrationImplBase.REGISTRATION_TECH_LTE, 1);
        mMetrics.writePhoneState(mPhone.getPhoneId(), PhoneConstants.State.IDLE);
        TelephonyLog log = buildProto();

        assertEquals(1, log.events.length);
        assertEquals(1, log.callSessions.length);
        assertEquals(0, log.smsSessions.length);
        assertEquals(2, log.callSessions[0].events.length);
        assertFalse(log.callSessions[0].eventsDropped);
        assertTrue(log.callSessions[0].events[1].settings.isEnhanced4GLteModeEnabled);
    }

    // Test write on ims call handover event
    @Test
    @SmallTest
    public void testWriteOnImsCallHandoverEvent() throws Exception {
        mMetrics.writeOnImsCallStart(mPhone.getPhoneId(), mImsCallSession);
        mMetrics.writeOnImsCallHandoverEvent(mPhone.getPhoneId(),
                TelephonyCallSession.Event.Type.IMS_CALL_HANDOVER, mImsCallSession, 5, 6,
                mImsReasonInfo);
        mMetrics.writePhoneState(mPhone.getPhoneId(), PhoneConstants.State.IDLE);
        TelephonyLog log = buildProto();

        assertEquals(0, log.events.length);
        assertEquals(1, log.callSessions.length);
        assertEquals(0, log.smsSessions.length);
        assertEquals(2, log.callSessions[0].events.length);
        assertFalse(log.callSessions[0].eventsDropped);
        assertEquals(TelephonyCallSession.Event.Type.IMS_CALL_HANDOVER,
                log.callSessions[0].events[1].type);
        assertEquals(123, log.callSessions[0].events[1].callIndex);
        assertEquals(5, log.callSessions[0].events[1].srcAccessTech);
        assertEquals(6, log.callSessions[0].events[1].targetAccessTech);

        assertEquals("extramessage", log.callSessions[0].events[1].reasonInfo.extraMessage);
        assertEquals(456, log.callSessions[0].events[1].reasonInfo.extraCode);
        assertEquals(123, log.callSessions[0].events[1].reasonInfo.reasonCode);
    }

    // Test write on ims command
    @Test
    @SmallTest
    public void testWriteOnImsCommand() throws Exception {
        mMetrics.writeOnImsCallStart(mPhone.getPhoneId(), mImsCallSession);
        mMetrics.writeOnImsCommand(mPhone.getPhoneId(), mImsCallSession, 123);
        mMetrics.writePhoneState(mPhone.getPhoneId(), PhoneConstants.State.IDLE);
        TelephonyLog log = buildProto();

        assertEquals(0, log.events.length);
        assertEquals(1, log.callSessions.length);
        assertEquals(0, log.smsSessions.length);
        assertEquals(2, log.callSessions[0].events.length);

        assertFalse(log.callSessions[0].eventsDropped);

        assertEquals(TelephonyCallSession.Event.Type.IMS_COMMAND,
                log.callSessions[0].events[1].type);

        assertEquals(123, log.callSessions[0].events[1].imsCommand);

        assertEquals(123, log.callSessions[0].events[1].callIndex);
    }

    // Test write on ims connection state
    @Test
    @SmallTest
    public void testWriteOnImsConnectionState() throws Exception {
        mMetrics.writeOnImsCallStart(mPhone.getPhoneId(), mImsCallSession);
        mMetrics.writeOnImsConnectionState(mPhone.getPhoneId(),
                ImsConnectionState.State.CONNECTED, mImsReasonInfo);
        mMetrics.writeOnImsConnectionState(mPhone.getPhoneId(),
                ImsConnectionState.State.CONNECTED, mImsReasonInfo);
        mMetrics.writePhoneState(mPhone.getPhoneId(), PhoneConstants.State.IDLE);
        TelephonyLog log = buildProto();

        assertEquals(1, log.events.length);
        assertEquals(1, log.callSessions.length);
        assertEquals(0, log.smsSessions.length);
        assertEquals(2, log.callSessions[0].events.length);
        assertFalse(log.eventsDropped);
        assertEquals(TelephonyEvent.Type.IMS_CONNECTION_STATE_CHANGED, log.events[0].type);
        assertEquals(ImsConnectionState.State.CONNECTED,
                log.events[0].imsConnectionState.state);
        assertEquals(123, log.events[0].imsConnectionState.reasonInfo.reasonCode);
        assertEquals(456, log.events[0].imsConnectionState.reasonInfo.extraCode);
        assertEquals("extramessage", log.events[0].imsConnectionState.reasonInfo.extraMessage);
        assertFalse(log.callSessions[0].eventsDropped);
        assertEquals(TelephonyCallSession.Event.Type.IMS_CONNECTION_STATE_CHANGED,
                log.callSessions[0].events[1].type);
        assertEquals(ImsConnectionState.State.CONNECTED,
                log.callSessions[0].events[1].imsConnectionState.state);
    }

    // Test write on setup data call response
    @Test
    @SmallTest
    public void testWriteOnSetupDataCallResponse() throws Exception {
        DataCallResponse response = new DataCallResponse(
                5, /* status */
                6, /* suggestedRetryTime */
                7, /* cid */
                8, /* active */
                ApnSetting.PROTOCOL_IPV4V6, /* protocolType */
                FAKE_IFNAME, /* ifname */
                Arrays.asList(new LinkAddress(
                       NetworkUtils.numericToInetAddress(FAKE_ADDRESS), 0)), /* addresses */
                Arrays.asList(NetworkUtils.numericToInetAddress(FAKE_DNS)), /* dnses */
                Arrays.asList(NetworkUtils.numericToInetAddress(FAKE_GATEWAY)), /* gateways */
                Arrays.asList(NetworkUtils.numericToInetAddress(FAKE_PCSCF_ADDRESS)), /* pcscfs */
                1440 /* mtu */);

        mMetrics.writeOnRilSolicitedResponse(mPhone.getPhoneId(), 1, 2,
                RIL_REQUEST_SETUP_DATA_CALL, response);
        TelephonyLog log = buildProto();

        assertEquals(1, log.events.length);
        assertEquals(0, log.callSessions.length);
        assertEquals(0, log.smsSessions.length);
        assertFalse(log.eventsDropped);

        TelephonyEvent.RilSetupDataCallResponse respProto = log.events[0].setupDataCallResponse;

        assertEquals(5, respProto.status);
        assertEquals(6, respProto.suggestedRetryTimeMillis);
        assertEquals(7, respProto.call.cid);
        assertEquals(PDP_TYPE_IPV4V6, respProto.call.type);
        assertEquals(FAKE_IFNAME, respProto.call.iframe);
    }

    // Test write on deactivate data call response
    @Test
    @SmallTest
    public void testWriteOnDeactivateDataCallResponse() throws Exception {
        mMetrics.writeOnRilSolicitedResponse(mPhone.getPhoneId(), 2, 3,
                RIL_REQUEST_DEACTIVATE_DATA_CALL, null);
        TelephonyLog log = buildProto();

        assertEquals(1, log.events.length);
        assertEquals(0, log.callSessions.length);
        assertEquals(0, log.smsSessions.length);
        assertFalse(log.eventsDropped);

        assertEquals(TelephonyEvent.Type.DATA_CALL_DEACTIVATE_RESPONSE, log.events[0].type);
        assertEquals(4, log.events[0].error);
    }

    // Test write RIL send SMS
    @Test
    @SmallTest
    public void testWriteRilSendSms() throws Exception {
        mMetrics.writeRilSendSms(mPhone.getPhoneId(), 1, 2, 1);
        mMetrics.writeRilSendSms(mPhone.getPhoneId(), 4, 5, 2);

        SmsResponse response = new SmsResponse(0, null, 123);

        mMetrics.writeOnRilSolicitedResponse(mPhone.getPhoneId(), 1, 0, RIL_REQUEST_SEND_SMS,
                response);
        response = new SmsResponse(0, null, 456);
        mMetrics.writeOnRilSolicitedResponse(mPhone.getPhoneId(), 4, 0, RIL_REQUEST_SEND_SMS,
                response);
        TelephonyLog log = buildProto();

        assertEquals(0, log.events.length);
        assertEquals(0, log.callSessions.length);
        assertEquals(1, log.smsSessions.length);
        assertFalse(log.eventsDropped);

        SmsSession.Event[] events = log.smsSessions[0].events;
        assertEquals(4, events.length);
        assertEquals(SmsSession.Event.Type.SMS_SEND, events[0].type);
        assertEquals(1, events[0].rilRequestId);
        assertEquals(2, events[0].tech);
        assertEquals(1, events[0].format);

        assertEquals(SmsSession.Event.Type.SMS_SEND, events[1].type);
        assertEquals(4, events[1].rilRequestId);
        assertEquals(5, events[1].tech);
        assertEquals(2, events[1].format);

        assertEquals(SmsSession.Event.Type.SMS_SEND_RESULT, events[2].type);
        assertEquals(1, events[2].rilRequestId);
        assertEquals(1, events[2].error);
        assertEquals(123, events[2].errorCode);

        assertEquals(SmsSession.Event.Type.SMS_SEND_RESULT, events[3].type);
        assertEquals(4, events[3].rilRequestId);
        assertEquals(1, events[3].error);
        assertEquals(456, events[3].errorCode);
    }

    // Test write phone state
    @Test
    @SmallTest
    public void testWritePhoneState() throws Exception {
        mMetrics.writeOnImsCallStart(mPhone.getPhoneId(), mImsCallSession);
        mMetrics.writePhoneState(mPhone.getPhoneId(), PhoneConstants.State.OFFHOOK);
        mMetrics.writePhoneState(mPhone.getPhoneId(), PhoneConstants.State.IDLE);
        TelephonyLog log = buildProto();

        assertEquals(0, log.events.length);
        assertEquals(1, log.callSessions.length);
        assertEquals(0, log.smsSessions.length);
        assertFalse(log.eventsDropped);

        assertEquals(mPhone.getPhoneId(), log.callSessions[0].phoneId);
        assertEquals(2, log.callSessions[0].events.length);
        assertEquals(TelephonyCallSession.Event.Type.PHONE_STATE_CHANGED,
                log.callSessions[0].events[1].type);
        assertEquals(TelephonyCallSession.Event.PhoneState.STATE_OFFHOOK,
                log.callSessions[0].events[1].phoneState);
    }

    // Test write RIL dial and hangup
    @Test
    @SmallTest
    public void testWriteRilDialHangup() throws Exception {
        doReturn(Call.State.DIALING).when(mConnection).getState();
        mMetrics.writeRilDial(mPhone.getPhoneId(), mConnection, 2, mUusInfo);
        doReturn(Call.State.DISCONNECTED).when(mConnection).getState();
        mMetrics.writeRilHangup(mPhone.getPhoneId(), mConnection, 3, "");
        mMetrics.writePhoneState(mPhone.getPhoneId(), PhoneConstants.State.IDLE);
        TelephonyLog log = buildProto();

        assertEquals(0, log.events.length);
        assertEquals(1, log.callSessions.length);
        assertEquals(0, log.smsSessions.length);
        assertFalse(log.eventsDropped);

        TelephonyCallSession.Event[] events = log.callSessions[0].events;

        assertEquals(2, events.length);
        assertEquals(TelephonyCallSession.Event.Type.RIL_REQUEST, events[0].type);

        assertEquals(TelephonyCallSession.Event.RilRequest.RIL_REQUEST_DIAL,
                events[0].rilRequest);
        RilCall[] calls = events[0].calls;
        assertEquals(CallState.CALL_DIALING, calls[0].state);

        assertEquals(TelephonyCallSession.Event.RilRequest.RIL_REQUEST_HANGUP,
                events[1].rilRequest);
        calls = events[1].calls;
        assertEquals(3, calls[0].index);
        assertEquals(CallState.CALL_DISCONNECTED, calls[0].state);
    }

    // Test write RIL setup data call
    @Test
    @SmallTest
    public void testWriteRilSetupDataCall() throws Exception {
        mMetrics.writeSetupDataCall(
                mPhone.getPhoneId(), 14, 3, "apn", ApnSetting.PROTOCOL_IPV4V6);

        TelephonyLog log = buildProto();

        assertEquals(1, log.events.length);
        assertEquals(0, log.callSessions.length);
        assertEquals(0, log.smsSessions.length);

        assertFalse(log.eventsDropped);


        assertEquals(TelephonyEvent.Type.DATA_CALL_SETUP, log.events[0].type);

        TelephonyEvent.RilSetupDataCall setupDataCall = log.events[0].setupDataCall;

        assertEquals("apn", setupDataCall.apn);

        assertEquals(14, setupDataCall.rat);

        assertEquals(4, setupDataCall.dataProfile);

        assertEquals(PDP_TYPE_IPV4V6, setupDataCall.type);
    }

    // Test write service state changed
    @Test
    @SmallTest
    public void testWriteServiceStateChanged() throws Exception {
        mMetrics.writeServiceStateChanged(mPhone.getPhoneId(), mServiceState);
        mMetrics.writeServiceStateChanged(mPhone.getPhoneId(), mServiceState);
        TelephonyLog log = buildProto();

        assertEquals(1, log.events.length);
        assertEquals(0, log.callSessions.length);
        assertEquals(0, log.smsSessions.length);

        assertFalse(log.eventsDropped);

        TelephonyEvent event = log.events[0];

        assertEquals(TelephonyEvent.Type.RIL_SERVICE_STATE_CHANGED, event.type);

        TelephonyServiceState state = event.serviceState;

        assertEquals(RadioAccessTechnology.RAT_LTE, state.voiceRat);

        assertEquals(RadioAccessTechnology.RAT_LTE, state.dataRat);

        assertEquals(RoamingType.ROAMING_TYPE_DOMESTIC, state.voiceRoamingType);

        assertEquals(RoamingType.ROAMING_TYPE_DOMESTIC, state.dataRoamingType);

        assertEquals("voicelong", state.voiceOperator.alphaLong);

        assertEquals("voiceshort", state.voiceOperator.alphaShort);

        assertEquals("123456", state.voiceOperator.numeric);

        assertEquals("datalong", state.dataOperator.alphaLong);

        assertEquals("datashort", state.dataOperator.alphaShort);

        assertEquals("123456", state.dataOperator.numeric);
    }

    // Test reset scenario
    @Test
    @SmallTest
    public void testReset() throws Exception {
        mMetrics.writeServiceStateChanged(mPhone.getPhoneId(), mServiceState);
        reset();

        TelephonyLog log = buildProto();

        Object[] serviceStateEvents = Arrays.stream(log.events)
                .filter(event -> event.type == TelephonyEvent.Type.RIL_SERVICE_STATE_CHANGED)
                .toArray();

        assertEquals(1, serviceStateEvents.length);

        assertFalse(log.eventsDropped);

        TelephonyEvent event = (TelephonyEvent) serviceStateEvents[0];

        assertEquals(TelephonyEvent.Type.RIL_SERVICE_STATE_CHANGED, event.type);

        TelephonyServiceState state = event.serviceState;

        assertEquals(RadioAccessTechnology.RAT_LTE, state.voiceRat);

        assertEquals(RadioAccessTechnology.RAT_LTE, state.dataRat);

        assertEquals(RoamingType.ROAMING_TYPE_DOMESTIC, state.voiceRoamingType);

        assertEquals(RoamingType.ROAMING_TYPE_DOMESTIC, state.dataRoamingType);

        assertEquals("voicelong", state.voiceOperator.alphaLong);

        assertEquals("voiceshort", state.voiceOperator.alphaShort);

        assertEquals("123456", state.voiceOperator.numeric);

        assertEquals("datalong", state.dataOperator.alphaLong);

        assertEquals("datashort", state.dataOperator.alphaShort);

        assertEquals("123456", state.dataOperator.numeric);
    }

    // Test Proto Encoding/Decoding
    @Test
    @SmallTest
    public void testProtoEncodingDecoding() throws Exception {
        mMetrics.writeServiceStateChanged(mPhone.getPhoneId(), mServiceState);
        TelephonyLog log = buildProto();
        String encodedString = convertProtoToBase64String(log);

        byte[] decodedString = Base64.decode(encodedString, Base64.DEFAULT);
        assertArrayEquals(TelephonyProto.TelephonyLog.toByteArray(log), decodedString);
    }

    // Test write ims capabilities changed
    @Test
    @SmallTest
    public void testWriteOnImsCapabilities() throws Exception {
        MmTelFeature.MmTelCapabilities caps1 = new MmTelFeature.MmTelCapabilities();
        caps1.addCapabilities(MmTelFeature.MmTelCapabilities.CAPABILITY_TYPE_VOICE);
        caps1.addCapabilities(MmTelFeature.MmTelCapabilities.CAPABILITY_TYPE_UT);
        mMetrics.writeOnImsCapabilities(mPhone.getPhoneId(),
                ImsRegistrationImplBase.REGISTRATION_TECH_LTE, caps1);
        // The duplicate one should be filtered out.
        mMetrics.writeOnImsCapabilities(mPhone.getPhoneId(),
                ImsRegistrationImplBase.REGISTRATION_TECH_LTE, caps1);
        MmTelFeature.MmTelCapabilities caps2 = new MmTelFeature.MmTelCapabilities();
        caps2.addCapabilities(MmTelFeature.MmTelCapabilities.CAPABILITY_TYPE_VIDEO);
        caps2.addCapabilities(MmTelFeature.MmTelCapabilities.CAPABILITY_TYPE_UT);
        mMetrics.writeOnImsCapabilities(mPhone.getPhoneId(),
                ImsRegistrationImplBase.REGISTRATION_TECH_IWLAN, caps2);
        TelephonyLog log = buildProto();

        assertEquals(2, log.events.length);
        assertEquals(0, log.callSessions.length);
        assertEquals(0, log.smsSessions.length);

        TelephonyEvent event = log.events[0];

        assertEquals(TelephonyEvent.Type.IMS_CAPABILITIES_CHANGED, event.type);
        assertEquals(caps1.isCapable(MmTelFeature.MmTelCapabilities.CAPABILITY_TYPE_VOICE),
                event.imsCapabilities.voiceOverLte);
        assertEquals(caps1.isCapable(MmTelFeature.MmTelCapabilities.CAPABILITY_TYPE_VIDEO),
                event.imsCapabilities.videoOverLte);
        assertEquals(caps1.isCapable(MmTelFeature.MmTelCapabilities.CAPABILITY_TYPE_UT),
                event.imsCapabilities.utOverLte);
        assertEquals(false, event.imsCapabilities.voiceOverWifi);
        assertEquals(false, event.imsCapabilities.videoOverWifi);
        assertEquals(false, event.imsCapabilities.utOverWifi);

        event = log.events[1];

        assertEquals(TelephonyEvent.Type.IMS_CAPABILITIES_CHANGED, event.type);
        assertEquals(caps2.isCapable(MmTelFeature.MmTelCapabilities.CAPABILITY_TYPE_VOICE),
                event.imsCapabilities.voiceOverWifi);
        assertEquals(caps2.isCapable(MmTelFeature.MmTelCapabilities.CAPABILITY_TYPE_VIDEO),
                event.imsCapabilities.videoOverWifi);
        assertEquals(caps2.isCapable(MmTelFeature.MmTelCapabilities.CAPABILITY_TYPE_UT),
                event.imsCapabilities.utOverWifi);
        assertEquals(false, event.imsCapabilities.voiceOverLte);
        assertEquals(false, event.imsCapabilities.videoOverLte);
        assertEquals(false, event.imsCapabilities.utOverLte);
    }
}
