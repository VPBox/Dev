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
package com.android.internal.telephony.euicc;

import static junit.framework.Assert.assertEquals;
import static junit.framework.Assert.assertFalse;
import static junit.framework.Assert.assertTrue;

import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.anyInt;
import static org.mockito.Mockito.doAnswer;
import static org.mockito.Mockito.when;

import android.content.Intent;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import android.provider.Settings;
import android.telephony.TelephonyManager;
import android.telephony.euicc.EuiccManager;

import androidx.test.runner.AndroidJUnit4;

import com.android.internal.telephony.TelephonyTest;
import com.android.internal.telephony.uicc.IccCardStatus.CardState;
import com.android.internal.telephony.uicc.UiccController;
import com.android.internal.telephony.uicc.UiccSlot;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;
import org.mockito.invocation.InvocationOnMock;
import org.mockito.stubbing.Answer;

import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

@RunWith(AndroidJUnit4.class)
public class EuiccCardControllerTest extends TelephonyTest {
    private static final String KEY_LAST_BOOT_COUNT = "last_boot_count";

    private int mBootCount;
    private int mLastBootCount;
    private EuiccCardController mEuiccCardController;
    private SharedPreferences mSp;
    @Mock
    private UiccSlot mInactivatedEsimSlot;
    @Mock
    private UiccSlot mActivatedEsimSlot;
    @Mock
    private UiccSlot mNotPresentEsimSlot;
    @Mock
    private UiccSlot mActivatedRemovableSlot;
    @Mock
    private EuiccController mEuiccController;
    @Mock
    private UiccController mUiccController;
    private boolean mOtaStarted;
    private CountDownLatch mOtaLatch;


    @Before
    public void setUp() throws Exception {
        super.setUp("EuiccCardControllerTest");
        MockitoAnnotations.initMocks(this);
        mSp = PreferenceManager.getDefaultSharedPreferences(mContext);

        mLastBootCount = mSp.getInt(KEY_LAST_BOOT_COUNT, -1);
        mBootCount = Settings.Global.getInt(
                mContext.getContentResolver(), Settings.Global.BOOT_COUNT, -1);
        mOtaStarted = false;
        mOtaLatch = new CountDownLatch(1);

        when(mEuiccController.getOtaStatus(anyInt())).thenReturn(EuiccManager.EUICC_OTA_SUCCEEDED);
        doAnswer(new Answer<Void>() {
                @Override
                public Void answer(InvocationOnMock invocation) throws Throwable {
                    mOtaStarted = true;
                    mOtaLatch.countDown();
                    return null;
                }
        }).when(mEuiccController).startOtaUpdatingIfNecessary();
        when(mTelephonyManager.switchSlots(any())).thenReturn(true);
        when(mTelephonyManager.getPhoneCount()).thenReturn(1);
        when(mActivatedEsimSlot.isEuicc()).thenReturn(true);
        when(mActivatedEsimSlot.isActive()).thenReturn(true);
        when(mActivatedEsimSlot.getCardState()).thenReturn(CardState.CARDSTATE_PRESENT);
        when(mNotPresentEsimSlot.isEuicc()).thenReturn(true);
        when(mNotPresentEsimSlot.isActive()).thenReturn(true);
        when(mNotPresentEsimSlot.getCardState()).thenReturn(CardState.CARDSTATE_ERROR);
        when(mInactivatedEsimSlot.isEuicc()).thenReturn(true);
        when(mInactivatedEsimSlot.isActive()).thenReturn(false);
        when(mInactivatedEsimSlot.isRemovable()).thenReturn(false);
        when(mActivatedRemovableSlot.isEuicc()).thenReturn(false);
        when(mActivatedRemovableSlot.isActive()).thenReturn(true);
        when(mActivatedRemovableSlot.isRemovable()).thenReturn(true);
    }

    @After
    public void tearDown() throws Exception {
        super.tearDown();
        if (mBootCount == -1) {
            Settings.Global.resetToDefaults(mContext.getContentResolver(), KEY_LAST_BOOT_COUNT);
        } else {
            Settings.Global.putInt(mContext.getContentResolver(),
                    Settings.Global.BOOT_COUNT, mBootCount);
        }
        SharedPreferences sp = PreferenceManager.getDefaultSharedPreferences(mContext);
        sp.edit().putInt(KEY_LAST_BOOT_COUNT, mLastBootCount).apply();
    }

    @Test
    public void testIsBootUp() {
        mSp.edit().remove(KEY_LAST_BOOT_COUNT);
        Settings.Global.putInt(mContext.getContentResolver(), Settings.Global.BOOT_COUNT, 0);
        assertTrue(EuiccCardController.isBootUp(mContext));

        mSp.edit().putInt(KEY_LAST_BOOT_COUNT, 1).apply();
        Settings.Global.resetToDefaults(mContext.getContentResolver(), KEY_LAST_BOOT_COUNT);
        assertTrue(EuiccCardController.isBootUp(mContext));

        mSp.edit().putInt(KEY_LAST_BOOT_COUNT, 1).apply();
        Settings.Global.putInt(mContext.getContentResolver(), Settings.Global.BOOT_COUNT, 1);
        assertFalse(EuiccCardController.isBootUp(mContext));

        mSp.edit().putInt(KEY_LAST_BOOT_COUNT, 2).apply();
        assertTrue(EuiccCardController.isBootUp(mContext));
        assertEquals(mSp.getInt(KEY_LAST_BOOT_COUNT, -1), 1);
    }

    @Test
    public void testIsEmbeddedSlotActivated() {
        mEuiccCardController =
                new EuiccCardController(mContext, null, mEuiccController, mUiccController);
        when(mUiccController.getUiccSlots())
                .thenReturn(new UiccSlot[] {mActivatedRemovableSlot});
        assertFalse(mEuiccCardController.isEmbeddedSlotActivated());

        when(mUiccController.getUiccSlots())
                .thenReturn(new UiccSlot[] {mActivatedEsimSlot});
        assertTrue(mEuiccCardController.isEmbeddedSlotActivated());

        when(mUiccController.getUiccSlots())
                .thenReturn(new UiccSlot[] {mInactivatedEsimSlot});
        assertFalse(mEuiccCardController.isEmbeddedSlotActivated());

        when(mUiccController.getUiccSlots())
                .thenReturn(new UiccSlot[] {
                        mActivatedEsimSlot, mInactivatedEsimSlot, mActivatedRemovableSlot});
        assertTrue(mEuiccCardController.isEmbeddedSlotActivated());

        when(mUiccController.getUiccSlots())
                .thenReturn(new UiccSlot[] {
                        mInactivatedEsimSlot, mActivatedEsimSlot, mActivatedRemovableSlot});
        assertTrue(mEuiccCardController.isEmbeddedSlotActivated());
    }

    @Test
    public void testIsEmbeddedCardPresent() {
        mEuiccCardController =
                new EuiccCardController(mContext, null, mEuiccController, mUiccController);
        when(mUiccController.getUiccSlots())
                .thenReturn(new UiccSlot[] {mActivatedRemovableSlot});
        assertFalse(mEuiccCardController.isEmbeddedCardPresent());

        when(mUiccController.getUiccSlots())
                .thenReturn(new UiccSlot[] {mActivatedEsimSlot});
        assertTrue(mEuiccCardController.isEmbeddedCardPresent());

        when(mUiccController.getUiccSlots())
                .thenReturn(new UiccSlot[] {mNotPresentEsimSlot});
        assertFalse(mEuiccCardController.isEmbeddedCardPresent());

        when(mUiccController.getUiccSlots())
                .thenReturn(new UiccSlot[] {
                        mActivatedEsimSlot, mNotPresentEsimSlot, mActivatedRemovableSlot});
        assertTrue(mEuiccCardController.isEmbeddedCardPresent());

        when(mUiccController.getUiccSlots())
                .thenReturn(new UiccSlot[] {
                        mNotPresentEsimSlot, mActivatedEsimSlot, mActivatedRemovableSlot});
        assertTrue(mEuiccCardController.isEmbeddedCardPresent());
    }

    @Test
    public void testStartOtaUpdatingIfNecessary_onEmbeddedSlot_Present() {
        // isBootUp = true
        mSp.edit().remove(KEY_LAST_BOOT_COUNT);
        Settings.Global.putInt(mContext.getContentResolver(), Settings.Global.BOOT_COUNT, 0);
        when(mUiccController.getUiccSlots()).thenReturn(new UiccSlot[] {mActivatedEsimSlot});

        mEuiccCardController =
                new EuiccCardController(mContext, null, mEuiccController, mUiccController);
        mContext.sendBroadcast(new Intent(TelephonyManager.ACTION_SIM_SLOT_STATUS_CHANGED));
        try {
            mOtaLatch.await(5000, TimeUnit.MILLISECONDS);
            assertTrue(mOtaStarted);
        } catch (InterruptedException ignore) { }
    }

    @Test
    public void testStartOtaUpdatingIfNecessary_onEmbeddedSlot_NotPresent() {
        // isBootUp = true
        mSp.edit().remove(KEY_LAST_BOOT_COUNT);
        Settings.Global.putInt(mContext.getContentResolver(), Settings.Global.BOOT_COUNT, 0);
        when(mUiccController.getUiccSlots()).thenReturn(new UiccSlot[] {mNotPresentEsimSlot});

        mEuiccCardController =
            new EuiccCardController(mContext, null, mEuiccController, mUiccController);
        mContext.sendBroadcast(new Intent(TelephonyManager.ACTION_SIM_SLOT_STATUS_CHANGED));
        try {
            mOtaLatch.await(5000, TimeUnit.MILLISECONDS);
            assertFalse(mOtaStarted);
        } catch (InterruptedException ignore) { }
    }

    @Test
    public void testStartOtaUpdatingIfNecessary_notBootUp() {
        // isBootUp = false
        mSp.edit().putInt(KEY_LAST_BOOT_COUNT, 1).apply();
        Settings.Global.putInt(mContext.getContentResolver(), Settings.Global.BOOT_COUNT, 1);
        when(mUiccController.getUiccSlots()).thenReturn(new UiccSlot[] {mActivatedEsimSlot});
        mEuiccCardController =
                new EuiccCardController(mContext, null, mEuiccController, mUiccController);

        mContext.sendBroadcast(new Intent(TelephonyManager.ACTION_SIM_SLOT_STATUS_CHANGED));
        assertFalse(mOtaStarted);
    }

    @Test
    public void testStartOtaUpdatingIfNecessary_onRemovableSlot() {
        // isBootUp = true
        mSp.edit().remove(KEY_LAST_BOOT_COUNT);
        Settings.Global.putInt(mContext.getContentResolver(), Settings.Global.BOOT_COUNT, 0);
        when(mUiccController.getUiccSlots())
                .thenReturn(new UiccSlot[] {mActivatedRemovableSlot, mInactivatedEsimSlot});

        mEuiccCardController =
                new EuiccCardController(mContext, null, mEuiccController, mUiccController);
        mContext.sendBroadcast(new Intent(TelephonyManager.ACTION_SIM_SLOT_STATUS_CHANGED));
        assertFalse(mOtaStarted);
    }
}
