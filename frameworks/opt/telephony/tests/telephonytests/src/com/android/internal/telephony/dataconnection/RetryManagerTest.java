/**
 * Copyright (C) 2015 The Android Open Source Project
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

package com.android.internal.telephony.dataconnection;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import android.os.PersistableBundle;
import android.telephony.CarrierConfigManager;
import android.telephony.data.ApnSetting;
import android.test.suitebuilder.annotation.SmallTest;

import com.android.internal.telephony.RetryManager;
import com.android.internal.telephony.SubscriptionController;
import com.android.internal.telephony.TelephonyTest;
import com.android.internal.telephony.uicc.UiccController;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import java.util.ArrayList;

/**
 * APN retry manager tests
 */
public class RetryManagerTest extends TelephonyTest {

    // This is the real APN data for the Japanese carrier NTT Docomo.
    private ApnSetting mApn1 = ApnSetting.makeApnSetting(
            2163,                   // id
            "44010",                // numeric
            "sp-mode",              // name
            "spmode.ne.jp",         // apn
            null,                     // proxy
            -1,                     // port
            null,                     // mmsc
            null,                     // mmsproxy
            -1,                     // mmsport
            "",                     // user
            "",                     // password
            -1,                     // authtype
            ApnSetting.TYPE_DEFAULT | ApnSetting.TYPE_SUPL, // types
            ApnSetting.PROTOCOL_IP, // protocol
            ApnSetting.PROTOCOL_IP, // roaming_protocol
            true,                   // carrier_enabled
            0,                      // networktype_bitmask
            0,                      // profile_id
            false,                  // modem_cognitive
            0,                      // max_conns
            0,                      // wait_time
            0,                      // max_conns_time
            0,                      // mtu
            -1,                     // mvno_type
            "");                    // mnvo_match_data

    private ApnSetting mApn2 = ApnSetting.makeApnSetting(
            2164,                   // id
            "44010",                // numeric
            "mopera U",             // name
            "mopera.net",           // apn
            null,                     // proxy
            -1,                     // port
            null,                     // mmsc
            null,                     // mmsproxy
            -1,                     // mmsport
            "",                     // user
            "",                     // password
            -1,                     // authtype
            ApnSetting.TYPE_DEFAULT | ApnSetting.TYPE_SUPL,     // types
            ApnSetting.PROTOCOL_IP,                   // protocol
            ApnSetting.PROTOCOL_IP,                   // roaming_protocol
            true,                   // carrier_enabled
            0,                      // networktype_bitmask
            0,                      // profile_id
            false,                  // modem_cognitive
            0,                      // max_conns
            0,                      // wait_time
            0,                      // max_conns_time
            0,                      // mtu
            -1,                     // mvno_type
            "");                    // mnvo_match_data

    private ApnSetting mApn3 = ApnSetting.makeApnSetting(
            2165,                   // id
            "44010",                // numeric
            "b-mobile for Nexus",   // name
            "bmobile.ne.jp",        // apn
            null,                     // proxy
            -1,                     // port
            null,                     // mmsc
            null,                     // mmsproxy
            -1,                     // mmsport
            "",                     // user
            "",                     // password
            3,                      // authtype
            ApnSetting.TYPE_DEFAULT | ApnSetting.TYPE_SUPL,     // types
            ApnSetting.PROTOCOL_IP,                   // protocol
            ApnSetting.PROTOCOL_IP,                   // roaming_protocol
            true,                   // carrier_enabled
            0,                      // networktype_bitmask
            0,                      // profile_id
            false,                  // modem_cognitive
            0,                      // max_conns
            0,                      // wait_time
            0,                      // max_conns_time
            0,                      // mtu
            -1,                     // mvno_type
            "");                    // mnvo_match_data

    private PersistableBundle mBundle;

    @Before
    public void setUp() throws Exception {
        super.setUp(getClass().getSimpleName());
        mBundle = mContextFixture.getCarrierConfigBundle();

        replaceInstance(SubscriptionController.class, "sInstance", null, mSubscriptionController);
        replaceInstance(UiccController.class, "mInstance", null, mUiccController);
    }

    @After
    public void tearDown() throws Exception {
        super.tearDown();
    }

    /**
     * Test the behavior of a retry manager with no waiting APNs set.
     */
    @Test
    @SmallTest
    public void testRetryManagerEmpty() throws Exception {

        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_DATA_CALL_RETRY_CONFIG_STRINGS,
                new String[]{"default:2000"});

        RetryManager rm = new RetryManager(mPhone, "default");

        long delay = rm.getDelayForNextApn(false);
        assertEquals(RetryManager.NO_RETRY, delay);

        ApnSetting nextApn = rm.getNextApnSetting();
        assertTrue(nextApn == null);
        delay = rm.getDelayForNextApn(false);
        assertEquals(RetryManager.NO_RETRY, delay);
    }

    /**
     * Test the basic retry scenario where only one APN and no retry configured.
     */
    @Test
    @SmallTest
    public void testRetryManagerOneApnNoRetry() throws Exception {

        mBundle.putStringArray(
                CarrierConfigManager.KEY_CARRIER_DATA_CALL_RETRY_CONFIG_STRINGS,
                new String[]{"default:"});

        ArrayList<ApnSetting> waitingApns = new ArrayList<ApnSetting>();
        waitingApns.add(ApnSetting.makeApnSetting(mApn1));

        RetryManager rm = new RetryManager(mPhone, "default");
        rm.setWaitingApns(waitingApns);

        ApnSetting nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        long delay = rm.getDelayForNextApn(false);
        assertEquals(RetryManager.NO_RETRY, delay);
    }

    /**
     * Test the basic retry scenario where only one APN with two retries configured.
     */
    @Test
    @SmallTest
    public void testRetryManagerOneApnTwoRetries() throws Exception {

        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_DATA_CALL_RETRY_CONFIG_STRINGS,
                new String[]{"supl:2000,3000"});

        ArrayList<ApnSetting> waitingApns = new ArrayList<ApnSetting>();
        waitingApns.add(ApnSetting.makeApnSetting(mApn1));

        RetryManager rm = new RetryManager(mPhone, "supl");
        rm.setWaitingApns(waitingApns);

        ApnSetting nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        long delay = rm.getDelayForNextApn(false);
        assertEquals(2000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        delay = rm.getDelayForNextApn(false);
        assertEquals(3000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        delay = rm.getDelayForNextApn(false);
        assertEquals(RetryManager.NO_RETRY, delay);

        // No matter how many times we call getNextApnSetting, it should always return the next APN
        // with NO_RETRY because we've already reached the maximum retry count.
        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        delay = rm.getDelayForNextApn(false);
        assertEquals(RetryManager.NO_RETRY, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        delay = rm.getDelayForNextApn(false);
        assertEquals(RetryManager.NO_RETRY, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        delay = rm.getDelayForNextApn(false);
        assertEquals(RetryManager.NO_RETRY, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        delay = rm.getDelayForNextApn(false);
        assertEquals(RetryManager.NO_RETRY, delay);
    }

    /**
     * Test the basic retry scenario where two waiting APNs with one retry configured.
     */
    @Test
    @SmallTest
    public void testRetryManagerTwoApnsOneRetry() throws Exception {

        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_DATA_CALL_RETRY_CONFIG_STRINGS,
                new String[]{"others:2000"});

        ArrayList<ApnSetting> waitingApns = new ArrayList<ApnSetting>();
        waitingApns.add(ApnSetting.makeApnSetting(mApn1));
        waitingApns.add(ApnSetting.makeApnSetting(mApn2));

        RetryManager rm = new RetryManager(mPhone, "default");
        rm.setWaitingApns(waitingApns);

        ApnSetting nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        long delay = rm.getDelayForNextApn(false);
        assertEquals(20000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        delay = rm.getDelayForNextApn(false);
        assertEquals(2000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        delay = rm.getDelayForNextApn(false);
        assertEquals(20000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        delay = rm.getDelayForNextApn(false);
        assertEquals(RetryManager.NO_RETRY, delay);
    }

    /**
     * Test the basic retry scenario where two waiting APNs with two retries configured.
     */
    @Test
    @SmallTest
    public void testRetryManagerTwoApnsTwoRetries() throws Exception {

        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_DATA_CALL_RETRY_CONFIG_STRINGS,
                new String[]{"dun:2000,5000"});

        ArrayList<ApnSetting> waitingApns = new ArrayList<ApnSetting>();
        waitingApns.add(ApnSetting.makeApnSetting(mApn1));
        waitingApns.add(ApnSetting.makeApnSetting(mApn2));

        RetryManager rm = new RetryManager(mPhone, "dun");
        rm.setWaitingApns(waitingApns);

        ApnSetting nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        long delay = rm.getDelayForNextApn(false);
        assertEquals(20000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        delay = rm.getDelayForNextApn(false);
        assertEquals(2000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        delay = rm.getDelayForNextApn(false);
        assertEquals(20000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        delay = rm.getDelayForNextApn(false);
        assertEquals(5000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        delay = rm.getDelayForNextApn(false);
        assertEquals(20000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        delay = rm.getDelayForNextApn(false);
        assertEquals(RetryManager.NO_RETRY, delay);
    }

    /**
     * Test the basic retry scenario where two mms APNs with two retries configured.
     */
    @Test
    @SmallTest
    public void testRetryManagerTwoMmsApnsTwoRetries() throws Exception {

        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_DATA_CALL_RETRY_CONFIG_STRINGS,
                new String[]{"mms:      3000,6000"});

        ArrayList<ApnSetting> waitingApns = new ArrayList<ApnSetting>();
        waitingApns.add(ApnSetting.makeApnSetting(mApn1));
        waitingApns.add(ApnSetting.makeApnSetting(mApn2));

        RetryManager rm = new RetryManager(mPhone, "mms");
        rm.setWaitingApns(waitingApns);

        ApnSetting nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        long delay = rm.getDelayForNextApn(false);
        assertEquals(20000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        delay = rm.getDelayForNextApn(false);
        assertEquals(3000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        delay = rm.getDelayForNextApn(false);
        assertEquals(20000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        delay = rm.getDelayForNextApn(false);
        assertEquals(6000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        delay = rm.getDelayForNextApn(false);
        assertEquals(20000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        delay = rm.getDelayForNextApn(false);
        assertEquals(RetryManager.NO_RETRY, delay);
    }

    /**
     * Test the permanent fail scenario with one APN configured.
     */
    @Test
    @SmallTest
    public void testRetryManagerApnPermanentFailed() throws Exception {

        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_DATA_CALL_RETRY_CONFIG_STRINGS,
                new String[]{"fota:1000,4000,7000"});

        ArrayList<ApnSetting> waitingApns = new ArrayList<ApnSetting>();
        ApnSetting apn = ApnSetting.makeApnSetting(mApn1);
        waitingApns.add(apn);

        RetryManager rm = new RetryManager(mPhone, "fota");
        rm.setWaitingApns(waitingApns);

        ApnSetting nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        long delay = rm.getDelayForNextApn(false);
        assertEquals(1000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        delay = rm.getDelayForNextApn(false);
        assertEquals(4000, delay);

        rm.markApnPermanentFailed(apn);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn == null);
    }

    /**
     * Test the permanent fail scenario with two APNs configured.
     */
    @Test
    @SmallTest
    public void testRetryManagerApnPermanentFailedWithTwoApns() throws Exception {

        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_DATA_CALL_RETRY_CONFIG_STRINGS,
                new String[]{"xyz  :   1000,4000,7000"});

        ArrayList<ApnSetting> waitingApns = new ArrayList<ApnSetting>();
        ApnSetting myApn1 = ApnSetting.makeApnSetting(mApn1);
        ApnSetting myApn2 = ApnSetting.makeApnSetting(mApn2);
        waitingApns.add(myApn1);
        waitingApns.add(myApn2);

        RetryManager rm = new RetryManager(mPhone, "xyz");
        rm.setWaitingApns(waitingApns);

        ApnSetting nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        long delay = rm.getDelayForNextApn(false);
        assertEquals(20000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        delay = rm.getDelayForNextApn(false);
        assertEquals(1000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        delay = rm.getDelayForNextApn(false);
        assertEquals(20000, delay);

        rm.markApnPermanentFailed(myApn1);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        delay = rm.getDelayForNextApn(false);
        assertEquals(4000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        delay = rm.getDelayForNextApn(false);
        assertEquals(7000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        delay = rm.getDelayForNextApn(false);
        assertEquals(RetryManager.NO_RETRY, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        delay = rm.getDelayForNextApn(false);
        assertEquals(RetryManager.NO_RETRY, delay);
    }

    /**
     * Test the permanent fail scenario with three APNs configured.
     */
    @Test
    @SmallTest
    public void testRetryManagerApnPermanentFailedWithThreeApns() throws Exception {

        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_DATA_CALL_RETRY_CONFIG_STRINGS,
                new String[]{"default:2000:2000,3000:3000", "ims:1000,4000"});

        ArrayList<ApnSetting> waitingApns = new ArrayList<ApnSetting>();
        ApnSetting myApn1 = ApnSetting.makeApnSetting(mApn1);
        ApnSetting myApn2 = ApnSetting.makeApnSetting(mApn2);
        ApnSetting myApn3 = ApnSetting.makeApnSetting(mApn3);
        waitingApns.add(myApn1);
        waitingApns.add(myApn2);
        waitingApns.add(myApn3);

        RetryManager rm = new RetryManager(mPhone, "ims");
        rm.setWaitingApns(waitingApns);

        ApnSetting nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        long delay = rm.getDelayForNextApn(false);
        assertEquals(20000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        delay = rm.getDelayForNextApn(false);
        assertEquals(20000, delay);

        rm.markApnPermanentFailed(myApn2);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn3));
        delay = rm.getDelayForNextApn(false);
        assertEquals(1000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        delay = rm.getDelayForNextApn(false);
        assertEquals(20000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn3));
        delay = rm.getDelayForNextApn(false);
        assertEquals(4000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        delay = rm.getDelayForNextApn(false);
        assertEquals(20000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn3));
        delay = rm.getDelayForNextApn(false);
        assertEquals(RetryManager.NO_RETRY, delay);
    }

    /**
     * Test the permanent fail scenario with two APN all failed
     */
    @Test
    @SmallTest
    public void testRetryManagerApnPermanentFailedAll() throws Exception {

        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_DATA_CALL_RETRY_CONFIG_STRINGS,
                new String[]{"default:1000,4000,7000,9000", "mms:1234,4123"});

        ArrayList<ApnSetting> waitingApns = new ArrayList<ApnSetting>();
        ApnSetting myApn1 = ApnSetting.makeApnSetting(mApn1);
        ApnSetting myApn2 = ApnSetting.makeApnSetting(mApn2);
        waitingApns.add(myApn1);
        waitingApns.add(myApn2);

        RetryManager rm = new RetryManager(mPhone, "default");
        rm.setWaitingApns(waitingApns);

        ApnSetting nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        long delay = rm.getDelayForNextApn(false);
        assertEquals(20000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        delay = rm.getDelayForNextApn(false);
        assertEquals(1000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        delay = rm.getDelayForNextApn(false);
        assertEquals(20000, delay);

        rm.markApnPermanentFailed(myApn1);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        delay = rm.getDelayForNextApn(false);
        assertEquals(4000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        delay = rm.getDelayForNextApn(false);
        assertEquals(7000, delay);

        rm.markApnPermanentFailed(myApn2);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn == null);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn == null);
    }

    /**
     * Test the randomized delay scenario.
     */
    @Test
    @SmallTest
    public void testRetryManagerDelayWithRandomization() throws Exception {

        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_DATA_CALL_RETRY_CONFIG_STRINGS,
                new String[]{"default:default_randomization=1000,3000:2000,6000:3000,10000"});

        ArrayList<ApnSetting> waitingApns = new ArrayList<ApnSetting>();
        waitingApns.add(ApnSetting.makeApnSetting(mApn1));

        RetryManager rm = new RetryManager(mPhone, "default");
        rm.setWaitingApns(waitingApns);

        ApnSetting nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        long delay = rm.getDelayForNextApn(false);
        assertTrue(delay >= 3000 && delay < 5000);    // 3s + 2s rand

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        delay = rm.getDelayForNextApn(false);
        assertTrue(delay >= 6000 && delay < 9000);    // 6s + 3s rand

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        delay = rm.getDelayForNextApn(false);
        assertTrue(delay >= 10000 && delay < 11000);  // 10s + 1s default rand

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        delay = rm.getDelayForNextApn(false);
        assertEquals(RetryManager.NO_RETRY, delay);
    }

    /**
     * Test the retry forever scenario
     */
    @Test
    @SmallTest
    public void testRetryManagerRetryForever() throws Exception {

        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_DATA_CALL_RETRY_CONFIG_STRINGS,
                new String[]{"default:max_retries=infinite,1000,2000"});

        ArrayList<ApnSetting> waitingApns = new ArrayList<ApnSetting>();
        waitingApns.add(ApnSetting.makeApnSetting(mApn1));
        waitingApns.add(ApnSetting.makeApnSetting(mApn2));

        RetryManager rm = new RetryManager(mPhone, "default");
        rm.setWaitingApns(waitingApns);

        ApnSetting nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        long delay = rm.getDelayForNextApn(false);
        assertEquals(20000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        delay = rm.getDelayForNextApn(false);
        assertEquals(1000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        delay = rm.getDelayForNextApn(false);
        assertEquals(20000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        delay = rm.getDelayForNextApn(false);
        assertEquals(2000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        delay = rm.getDelayForNextApn(false);
        assertEquals(20000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        delay = rm.getDelayForNextApn(false);
        assertEquals(2000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        delay = rm.getDelayForNextApn(false);
        assertEquals(20000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        delay = rm.getDelayForNextApn(false);
        assertEquals(2000, delay);
    }

    /**
     * Test the explicit max retry scenario.
     */
    @Test
    @SmallTest
    public void testRetryManagerExplicitMaxRetry() throws Exception {

        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_DATA_CALL_RETRY_CONFIG_STRINGS,
                new String[]{"hipri:  max_retries=4,1000,2000"});

        ArrayList<ApnSetting> waitingApns = new ArrayList<ApnSetting>();
        waitingApns.add(ApnSetting.makeApnSetting(mApn1));
        waitingApns.add(ApnSetting.makeApnSetting(mApn2));

        RetryManager rm = new RetryManager(mPhone, "hipri");
        rm.setWaitingApns(waitingApns);

        ApnSetting nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        long delay = rm.getDelayForNextApn(false);
        assertEquals(20000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        delay = rm.getDelayForNextApn(false);
        assertEquals(1000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        delay = rm.getDelayForNextApn(false);
        assertEquals(20000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        delay = rm.getDelayForNextApn(false);
        assertEquals(2000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        delay = rm.getDelayForNextApn(false);
        assertEquals(20000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        delay = rm.getDelayForNextApn(false);
        assertEquals(2000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        delay = rm.getDelayForNextApn(false);
        assertEquals(20000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        delay = rm.getDelayForNextApn(false);
        assertEquals(2000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        delay = rm.getDelayForNextApn(false);
        assertEquals(20000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        delay = rm.getDelayForNextApn(false);
        assertEquals(RetryManager.NO_RETRY, delay);
    }

    /**
     * Test the fail fast scenario.
     */
    @Test
    @SmallTest
    public void testRetryManagerFailFast() throws Exception {

        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_DATA_CALL_RETRY_CONFIG_STRINGS,
                new String[]{"default:1000,5000"});

        mBundle.putLong(CarrierConfigManager.KEY_CARRIER_DATA_CALL_APN_DELAY_FASTER_LONG, 2000);

        ArrayList<ApnSetting> waitingApns = new ArrayList<ApnSetting>();
        waitingApns.add(ApnSetting.makeApnSetting(mApn1));
        waitingApns.add(ApnSetting.makeApnSetting(mApn2));

        RetryManager rm = new RetryManager(mPhone, "default");
        rm.setWaitingApns(waitingApns);

        ApnSetting nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        long delay = rm.getDelayForNextApn(true);
        assertEquals(2000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        delay = rm.getDelayForNextApn(true);
        assertEquals(1000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        delay = rm.getDelayForNextApn(false);
        assertEquals(20000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        delay = rm.getDelayForNextApn(true);
        assertEquals(2000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        delay = rm.getDelayForNextApn(true);
        assertEquals(2000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        delay = rm.getDelayForNextApn(true);
        assertEquals(RetryManager.NO_RETRY, delay);
    }

    /**
     * Test the permanent fail scenario with two APN all failed and then reset
     */
    @Test
    @SmallTest
    public void testRetryManagerApnPermanentFailedAllAndThenReset() throws Exception {

        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_DATA_CALL_RETRY_CONFIG_STRINGS,
                new String[]{"dun:1000,4000,7000,9000"});

        ArrayList<ApnSetting> waitingApns = new ArrayList<ApnSetting>();
        ApnSetting myApn1 = ApnSetting.makeApnSetting(mApn1);
        ApnSetting myApn2 = ApnSetting.makeApnSetting(mApn2);
        waitingApns.add(myApn1);
        waitingApns.add(myApn2);

        RetryManager rm = new RetryManager(mPhone, "dun");
        rm.setWaitingApns(waitingApns);

        ApnSetting nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        long delay = rm.getDelayForNextApn(false);
        assertEquals(20000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        delay = rm.getDelayForNextApn(false);
        assertEquals(1000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        delay = rm.getDelayForNextApn(false);
        assertEquals(20000, delay);

        rm.markApnPermanentFailed(myApn1);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        delay = rm.getDelayForNextApn(false);
        assertEquals(4000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        delay = rm.getDelayForNextApn(false);
        assertEquals(7000, delay);

        rm.markApnPermanentFailed(myApn2);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn == null);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn == null);

        // reset the retry manager

        ApnSetting myApn3 = ApnSetting.makeApnSetting(mApn3);
        waitingApns.clear();
        waitingApns.add(myApn3);

        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_DATA_CALL_RETRY_CONFIG_STRINGS,
                new String[]{"dun:3000,8000"});

        rm.setWaitingApns(waitingApns);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn3));
        delay = rm.getDelayForNextApn(false);
        assertEquals(3000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn3));
        delay = rm.getDelayForNextApn(false);
        assertEquals(8000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn3));
        delay = rm.getDelayForNextApn(false);
        assertEquals(RetryManager.NO_RETRY, delay);
    }

    /**
     * Test the scenario where modem suggests retry the current APN once
     */
    @Test
    @SmallTest
    public void testRetryManagerModemSuggestedRetryOnce() throws Exception {

        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_DATA_CALL_RETRY_CONFIG_STRINGS,
                new String[]{"others:1000,4000,7000,9000"});

        ArrayList<ApnSetting> waitingApns = new ArrayList<ApnSetting>();
        ApnSetting myApn1 = ApnSetting.makeApnSetting(mApn1);
        ApnSetting myApn2 = ApnSetting.makeApnSetting(mApn2);
        waitingApns.add(myApn1);
        waitingApns.add(myApn2);

        RetryManager rm = new RetryManager(mPhone, "mms");
        rm.setWaitingApns(waitingApns);

        ApnSetting nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        long delay = rm.getDelayForNextApn(false);
        assertEquals(20000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        delay = rm.getDelayForNextApn(false);
        assertEquals(1000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        // Modem suggests retrying the current APN
        rm.setModemSuggestedDelay(2500);
        delay = rm.getDelayForNextApn(false);
        assertEquals(2500, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        rm.setModemSuggestedDelay(RetryManager.NO_SUGGESTED_RETRY_DELAY);
        delay = rm.getDelayForNextApn(false);
        assertEquals(20000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        // Modem suggests retrying the current APN
        rm.setModemSuggestedDelay(30000);
        delay = rm.getDelayForNextApn(false);
        assertEquals(30000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        rm.setModemSuggestedDelay(RetryManager.NO_SUGGESTED_RETRY_DELAY);
        delay = rm.getDelayForNextApn(false);
        assertEquals(4000, delay);
    }

    /**
     * Test the scenario where modem suggests not retrying
     */
    @Test
    @SmallTest
    public void testRetryManagerModemSuggestedNoRetry() throws Exception {

        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_DATA_CALL_RETRY_CONFIG_STRINGS,
                new String[]{"default:1000,4000,7000,9000"});

        ArrayList<ApnSetting> waitingApns = new ArrayList<ApnSetting>();
        ApnSetting myApn1 = ApnSetting.makeApnSetting(mApn1);
        ApnSetting myApn2 = ApnSetting.makeApnSetting(mApn2);
        waitingApns.add(myApn1);
        waitingApns.add(myApn2);

        RetryManager rm = new RetryManager(mPhone, "default");
        rm.setWaitingApns(waitingApns);

        ApnSetting nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        long delay = rm.getDelayForNextApn(false);
        assertEquals(20000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        delay = rm.getDelayForNextApn(false);
        assertEquals(1000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        // Modem suggests retrying the current APN
        rm.setModemSuggestedDelay(2500);
        delay = rm.getDelayForNextApn(false);
        assertEquals(2500, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        rm.setModemSuggestedDelay(RetryManager.NO_RETRY);
        delay = rm.getDelayForNextApn(false);
        assertEquals(RetryManager.NO_RETRY, delay);
    }

    /**
     * Test the scenario where modem suggests the same retry for too many times
     */
    @Test
    @SmallTest
    public void testRetryManagerModemSuggestedRetryTooManyTimes() throws Exception {

        mBundle.putStringArray(CarrierConfigManager.KEY_CARRIER_DATA_CALL_RETRY_CONFIG_STRINGS,
                new String[]{"mms:2000,3000", "default:1000,4000,7000,9000"});

        ArrayList<ApnSetting> waitingApns = new ArrayList<ApnSetting>();
        ApnSetting myApn1 = ApnSetting.makeApnSetting(mApn1);
        ApnSetting myApn2 = ApnSetting.makeApnSetting(mApn2);
        waitingApns.add(myApn1);
        waitingApns.add(myApn2);

        RetryManager rm = new RetryManager(mPhone, "default");
        rm.setWaitingApns(waitingApns);

        ApnSetting nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        long delay = rm.getDelayForNextApn(false);
        assertEquals(20000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        delay = rm.getDelayForNextApn(false);
        assertEquals(1000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        rm.setModemSuggestedDelay(2500);
        delay = rm.getDelayForNextApn(false);
        assertEquals(2500, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        rm.setModemSuggestedDelay(2500);
        delay = rm.getDelayForNextApn(false);
        assertEquals(2500, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        rm.setModemSuggestedDelay(2500);
        delay = rm.getDelayForNextApn(false);
        assertEquals(2500, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn1));
        rm.setModemSuggestedDelay(2500);
        delay = rm.getDelayForNextApn(false);
        assertEquals(20000, delay);

        nextApn = rm.getNextApnSetting();
        assertTrue(nextApn.equals(mApn2));
        rm.setModemSuggestedDelay(RetryManager.NO_SUGGESTED_RETRY_DELAY);
        delay = rm.getDelayForNextApn(false);
        assertEquals(4000, delay);
    }
}
