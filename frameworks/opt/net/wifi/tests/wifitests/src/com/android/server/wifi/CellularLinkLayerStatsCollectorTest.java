/*
 * Copyright (C) 2019 The Android Open Source Project
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

import static android.telephony.TelephonyManager.NETWORK_TYPE_CDMA;
import static android.telephony.TelephonyManager.NETWORK_TYPE_EVDO_0;
import static android.telephony.TelephonyManager.NETWORK_TYPE_GSM;
import static android.telephony.TelephonyManager.NETWORK_TYPE_LTE;
import static android.telephony.TelephonyManager.NETWORK_TYPE_NR;
import static android.telephony.TelephonyManager.NETWORK_TYPE_TD_SCDMA;
import static android.telephony.TelephonyManager.NETWORK_TYPE_UMTS;
import static android.telephony.TelephonyManager.NETWORK_TYPE_UNKNOWN;

import static com.android.dx.mockito.inline.extended.ExtendedMockito.mockitoSession;
import static com.android.dx.mockito.inline.extended.ExtendedMockito.when;

import static org.junit.Assert.assertEquals;
import static org.mockito.Matchers.anyInt;
import static org.mockito.Mockito.validateMockitoUsage;

import android.content.Context;
import android.telephony.CellInfo;
import android.telephony.CellInfoCdma;
import android.telephony.CellInfoGsm;
import android.telephony.CellInfoLte;
import android.telephony.CellInfoTdscdma;
import android.telephony.CellInfoWcdma;
import android.telephony.CellSignalStrengthCdma;
import android.telephony.CellSignalStrengthGsm;
import android.telephony.CellSignalStrengthLte;
import android.telephony.CellSignalStrengthNr;
import android.telephony.CellSignalStrengthTdscdma;
import android.telephony.CellSignalStrengthWcdma;
import android.telephony.SignalStrength;
import android.telephony.SubscriptionManager;
import android.telephony.TelephonyManager;
import android.telephony.TelephonyManager.NetworkType;

import androidx.test.filters.SmallTest;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;
import org.mockito.MockitoSession;

import java.util.ArrayList;
import java.util.List;

/**
 * Unit tests for {@link com.android.server.wifi.CellularLinkLayerStatsCollector}.
 */
@SmallTest
public class CellularLinkLayerStatsCollectorTest {
    private CellularLinkLayerStatsCollector mCollector;
    private static final String TAG = "CellCollectorTest";
    private static final int DBM_VAL = -110;
    private static final int DB_VAL = -20;
    private static final int DB_VAL_EVDO = 4;
    private static final int SUBID = SubscriptionManager.DEFAULT_SUBSCRIPTION_ID;
    MockitoSession mMockingSession = null;
    @Mock Context mContext;
    @Mock TelephonyManager mTelephonyManager;
    @Mock SubscriptionManager mSubscriptionManager;

    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        when(mContext.getSystemService(Context.TELEPHONY_SERVICE))
                .thenReturn(mTelephonyManager);
        when(mContext.getSystemService(Context.TELEPHONY_SUBSCRIPTION_SERVICE))
                .thenReturn(mSubscriptionManager);
        when(mTelephonyManager.createForSubscriptionId(anyInt()))
                .thenReturn(mTelephonyManager);
        mCollector = new CellularLinkLayerStatsCollector(mContext);
        mMockingSession = mockitoSession().mockStatic(SubscriptionManager.class).startMocking();
        when(SubscriptionManager.getDefaultDataSubscriptionId()).thenReturn(SUBID);
        when(SubscriptionManager.getDefaultSubscriptionId()).thenReturn(SUBID);
    }

    @After
    public void cleanUp() throws Exception {
        validateMockitoUsage();
        if (mMockingSession != null) {
            mMockingSession.finishMocking();
        }
    }

    private List<CellInfo> generateCellInfoList(@NetworkType int networkType) {
        List<CellInfo> cil = new ArrayList<>();
        int numCellInfo = 2;
        for (int i = 0; i < numCellInfo; ++i) {
            CellInfo ci;
            if (networkType == NETWORK_TYPE_LTE) {
                ci = new CellInfoLte();
            } else if (networkType == NETWORK_TYPE_CDMA || networkType == NETWORK_TYPE_EVDO_0) {
                ci = new CellInfoCdma();
            } else if (networkType == NETWORK_TYPE_GSM) {
                ci = new CellInfoGsm();
            } else if (networkType == NETWORK_TYPE_TD_SCDMA) {
                ci = new CellInfoTdscdma();
            } else if (networkType == NETWORK_TYPE_UMTS) {
                ci = new CellInfoWcdma();
            } else if (networkType == NETWORK_TYPE_NR) {
                // TODO: CellInfoNr() is not supported yet.
                ci = new CellInfoLte();
            } else {
                ci = new CellInfoLte();
            }
            if (i == 0 && networkType != NETWORK_TYPE_UNKNOWN) {
                ci.setRegistered(true);
            } else {
                ci.setRegistered(false);
            }
            cil.add(ci);
        }
        return cil;
    }

    private SignalStrength generateSignalStrength(int dBmVal, int dBVal,
                @NetworkType int networkType) {
        int dummy = 1000;
        CellSignalStrengthLte mLte = new CellSignalStrengthLte();
        CellSignalStrengthNr mNr = new CellSignalStrengthNr();
        CellSignalStrengthGsm mGsm = new CellSignalStrengthGsm();
        CellSignalStrengthCdma mCdma = new CellSignalStrengthCdma();
        CellSignalStrengthTdscdma mTdscdma = new CellSignalStrengthTdscdma();
        CellSignalStrengthWcdma mWcdma = new CellSignalStrengthWcdma();

        if (networkType == NETWORK_TYPE_UNKNOWN) {
            return new SignalStrength();
        } else if (networkType == NETWORK_TYPE_LTE) {
            mLte = new CellSignalStrengthLte(dummy, dBmVal, dBVal, dummy, dummy, dummy);
        } else if (networkType == NETWORK_TYPE_CDMA) {
            mCdma = new CellSignalStrengthCdma(dBmVal, dBVal, dBmVal, dummy,
                    SignalStrength.INVALID);
        } else if (networkType == NETWORK_TYPE_EVDO_0) {
            mCdma = new CellSignalStrengthCdma(dBmVal, dummy, dBmVal, dummy, dBVal);
        } else if (networkType == NETWORK_TYPE_TD_SCDMA) {
            mTdscdma = new CellSignalStrengthTdscdma(dummy, dummy, dBmVal);
        } else if (networkType == NETWORK_TYPE_UMTS) {
            mWcdma = new CellSignalStrengthWcdma(dummy, dummy, dBmVal, dBVal);
        } else if (networkType == NETWORK_TYPE_GSM) {
            mGsm = new CellSignalStrengthGsm(dBmVal, dummy, dummy);
        } else if (networkType == NETWORK_TYPE_NR) {
            mNr = new CellSignalStrengthNr(dBmVal, dummy, dBVal, dummy, dummy, dummy);
        } else {
            return null;
        }
        return new SignalStrength(mCdma, mGsm, mWcdma, mTdscdma, mLte, mNr);
    }

    private void testCollectorUpdate(@NetworkType int networkType, boolean isSignalStrengthEmpty,
                CellularLinkLayerStats trueStats) throws Exception {
        int dBmVal = DBM_VAL;
        int dBVal;
        if (networkType == NETWORK_TYPE_EVDO_0) {
            dBVal = DB_VAL_EVDO;
        } else {
            dBVal = DB_VAL;
        }

        SignalStrength ss = null;
        if (!isSignalStrengthEmpty) ss = generateSignalStrength(dBmVal, dBVal, networkType);
        List<CellInfo> allList = generateCellInfoList(networkType);
        when(mTelephonyManager.getSignalStrength()).thenReturn(ss);
        when(mTelephonyManager.getAllCellInfo()).thenReturn(allList);
        when(mTelephonyManager.getDataNetworkType()).thenReturn(networkType);

        CellularLinkLayerStats mStats = mCollector.update();

        assertEquals(SUBID, SubscriptionManager.getDefaultDataSubscriptionId());
        assertEquals(SUBID, SubscriptionManager.getDefaultSubscriptionId());

        assertEquals(trueStats.getSignalStrengthDbm(), mStats.getSignalStrengthDbm());
        assertEquals(trueStats.getSignalStrengthDb(), mStats.getSignalStrengthDb());
        assertEquals(trueStats.getDataNetworkType(), mStats.getDataNetworkType());
        assertEquals(trueStats.getIsSameRegisteredCell(), mStats.getIsSameRegisteredCell());
    }

    @Test
    public void testEmptySignalStrengthLte() throws Exception {
        @NetworkType int networkType;
        CellularLinkLayerStats trueStats = new CellularLinkLayerStats();

        networkType = NETWORK_TYPE_LTE;
        trueStats.setIsSameRegisteredCell(false);
        trueStats.setSignalStrengthDb(SignalStrength.INVALID);
        trueStats.setSignalStrengthDbm(SignalStrength.INVALID);
        trueStats.setDataNetworkType(NETWORK_TYPE_UNKNOWN);
        testCollectorUpdate(networkType, true, trueStats);
    }

    @Test
    public void testRepeatCellInfoTypeTwice() throws Exception {
        @NetworkType int networkType;
        CellularLinkLayerStats trueStats = new CellularLinkLayerStats();

        networkType = NETWORK_TYPE_LTE;
        trueStats.setIsSameRegisteredCell(false);
        trueStats.setSignalStrengthDb(DB_VAL);
        trueStats.setSignalStrengthDbm(DBM_VAL);
        trueStats.setDataNetworkType(networkType);
        testCollectorUpdate(networkType, false, trueStats);

        networkType = NETWORK_TYPE_LTE;
        trueStats.setIsSameRegisteredCell(true);
        trueStats.setSignalStrengthDb(DB_VAL);
        trueStats.setSignalStrengthDbm(DBM_VAL);
        trueStats.setDataNetworkType(networkType);
        testCollectorUpdate(networkType, false, trueStats);

        networkType = NETWORK_TYPE_EVDO_0;
        trueStats.setIsSameRegisteredCell(false);
        trueStats.setSignalStrengthDb(DB_VAL_EVDO);
        trueStats.setSignalStrengthDbm(DBM_VAL);
        trueStats.setDataNetworkType(networkType);
        testCollectorUpdate(networkType, false, trueStats);

        networkType = NETWORK_TYPE_EVDO_0;
        trueStats.setIsSameRegisteredCell(true);
        trueStats.setSignalStrengthDb(DB_VAL_EVDO);
        trueStats.setSignalStrengthDbm(DBM_VAL);
        trueStats.setDataNetworkType(networkType);
        testCollectorUpdate(networkType, false, trueStats);

        networkType = NETWORK_TYPE_NR;
        trueStats.setIsSameRegisteredCell(false);
        trueStats.setSignalStrengthDb(DB_VAL);
        trueStats.setSignalStrengthDbm(DBM_VAL);
        trueStats.setDataNetworkType(networkType);
        testCollectorUpdate(networkType, false, trueStats);

        networkType = NETWORK_TYPE_NR;
        trueStats.setIsSameRegisteredCell(true);
        trueStats.setSignalStrengthDb(DB_VAL);
        trueStats.setSignalStrengthDbm(DBM_VAL);
        trueStats.setDataNetworkType(networkType);
        testCollectorUpdate(networkType, false, trueStats);
    }

    @Test
    public void testLoopOverAllNetworksWithoutRepeat() throws Exception {
        @NetworkType int networkType;
        CellularLinkLayerStats trueStats = new CellularLinkLayerStats();

        networkType = NETWORK_TYPE_UNKNOWN;
        trueStats.setIsSameRegisteredCell(false);
        trueStats.setSignalStrengthDb(SignalStrength.INVALID);
        trueStats.setSignalStrengthDbm(SignalStrength.INVALID);
        trueStats.setDataNetworkType(networkType);
        testCollectorUpdate(networkType, false, trueStats);

        networkType = NETWORK_TYPE_LTE;
        trueStats.setIsSameRegisteredCell(false);
        trueStats.setSignalStrengthDb(DB_VAL);
        trueStats.setSignalStrengthDbm(DBM_VAL);
        trueStats.setDataNetworkType(networkType);
        testCollectorUpdate(networkType, false, trueStats);

        networkType = NETWORK_TYPE_CDMA;
        trueStats.setIsSameRegisteredCell(false);
        trueStats.setSignalStrengthDb(DB_VAL);
        trueStats.setSignalStrengthDbm(DBM_VAL);
        trueStats.setDataNetworkType(networkType);
        testCollectorUpdate(networkType, false, trueStats);

        networkType = NETWORK_TYPE_EVDO_0;
        trueStats.setIsSameRegisteredCell(false);
        trueStats.setSignalStrengthDb(DB_VAL_EVDO);
        trueStats.setSignalStrengthDbm(DBM_VAL);
        trueStats.setDataNetworkType(networkType);
        testCollectorUpdate(networkType, false, trueStats);

        networkType = NETWORK_TYPE_TD_SCDMA;
        trueStats.setIsSameRegisteredCell(false);
        trueStats.setSignalStrengthDb(SignalStrength.INVALID);
        trueStats.setSignalStrengthDbm(DBM_VAL);
        trueStats.setDataNetworkType(networkType);
        testCollectorUpdate(networkType, false, trueStats);

        networkType = NETWORK_TYPE_UMTS;
        trueStats.setIsSameRegisteredCell(false);
        trueStats.setSignalStrengthDb(DB_VAL);
        trueStats.setSignalStrengthDbm(DBM_VAL);
        trueStats.setDataNetworkType(networkType);
        testCollectorUpdate(networkType, false, trueStats);

        networkType = NETWORK_TYPE_GSM;
        trueStats.setIsSameRegisteredCell(false);
        trueStats.setSignalStrengthDb(SignalStrength.INVALID);
        trueStats.setSignalStrengthDbm(DBM_VAL);
        trueStats.setDataNetworkType(networkType);
        testCollectorUpdate(networkType, false, trueStats);

        networkType = NETWORK_TYPE_NR;
        trueStats.setIsSameRegisteredCell(false);
        trueStats.setSignalStrengthDb(DB_VAL);
        trueStats.setSignalStrengthDbm(DBM_VAL);
        trueStats.setDataNetworkType(networkType);
        testCollectorUpdate(networkType, false, trueStats);

    }
}
