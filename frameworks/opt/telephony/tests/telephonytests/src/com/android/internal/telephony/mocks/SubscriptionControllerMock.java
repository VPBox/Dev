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
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.internal.telephony.mocks;

import static android.telephony.SubscriptionManager.DEFAULT_SUBSCRIPTION_ID;
import static android.telephony.SubscriptionManager.INVALID_PHONE_INDEX;
import static android.telephony.SubscriptionManager.INVALID_SUBSCRIPTION_ID;

import android.content.Context;
import android.content.Intent;
import android.os.RemoteException;
import android.os.UserHandle;
import android.telephony.SubscriptionInfo;

import com.android.internal.telephony.CommandsInterface;
import com.android.internal.telephony.ITelephonyRegistry;
import com.android.internal.telephony.Phone;
import com.android.internal.telephony.PhoneConstants;
import com.android.internal.telephony.SubscriptionController;
import com.android.internal.telephony.TelephonyIntents;

import java.io.FileDescriptor;
import java.io.PrintWriter;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;

// must extend SubscriptionController as some people use it directly within-process
public class SubscriptionControllerMock extends SubscriptionController {
    final AtomicInteger mDefaultDataSubId = new AtomicInteger(INVALID_SUBSCRIPTION_ID);
    final AtomicInteger mDefaultVoiceSubId = new AtomicInteger(INVALID_SUBSCRIPTION_ID);
    final ITelephonyRegistry.Stub mTelephonyRegistry;
    final int[][] mSlotIndexToSubId;

    public static SubscriptionController init(Phone phone) {
        throw new RuntimeException("not implemented");
    }
    public static SubscriptionController init(Context c, CommandsInterface[] ci) {
        throw new RuntimeException("not implemented");
    }
    public static SubscriptionController getInstance() {
        throw new RuntimeException("not implemented");
    }

    public SubscriptionControllerMock(Context c, ITelephonyRegistry.Stub tr, int phoneCount) {
        super(c);
        mTelephonyRegistry = tr;
        mSlotIndexToSubId = new int[phoneCount][];
        for (int i = 0; i < phoneCount; i++) {
            mSlotIndexToSubId[i] = new int[1];
            mSlotIndexToSubId[i][0] = INVALID_SUBSCRIPTION_ID;
        }
    }

    protected void init(Context c) {
        mContext = c;
    }

    @Override
    public int getDefaultDataSubId() {
        return mDefaultDataSubId.get();
    }

    @Override
    public void setDefaultDataSubId(int subId) {
        if (subId == DEFAULT_SUBSCRIPTION_ID) {
            throw new RuntimeException("setDefaultDataSubId called with DEFAULT_SUB_ID");
        }

        mDefaultDataSubId.set(subId);
        broadcastDefaultDataSubIdChanged(subId);
    }

    private void broadcastDefaultDataSubIdChanged(int subId) {
        // Broadcast an Intent for default data sub change
        Intent intent = new Intent(TelephonyIntents.ACTION_DEFAULT_DATA_SUBSCRIPTION_CHANGED);
        intent.addFlags(Intent.FLAG_RECEIVER_REPLACE_PENDING);
        intent.putExtra(PhoneConstants.SUBSCRIPTION_KEY, subId);
        mContext.sendStickyBroadcastAsUser(intent, UserHandle.ALL);
    }

    @Override
    public int getSubIdUsingPhoneId(int phoneId) {
        int[] subIds = getSubId(phoneId);
        if (subIds == null || subIds.length == 0) {
            return INVALID_SUBSCRIPTION_ID;
        }
        return subIds[0];
    }

    @Override
    public void notifySubscriptionInfoChanged() {
        try {
            mTelephonyRegistry.notifySubscriptionInfoChanged();
        } catch (RemoteException ex) {}
    }
    @Override
    public SubscriptionInfo getActiveSubscriptionInfo(int subId, String callingPackage) {
        throw new RuntimeException("not implemented");
    }
    @Override
    public SubscriptionInfo getActiveSubscriptionInfoForIccId(String iccId, String callingPackage) {
        throw new RuntimeException("not implemented");
    }
    @Override
    public SubscriptionInfo getActiveSubscriptionInfoForSimSlotIndex(int slotIndex, String cp){
        throw new RuntimeException("not implemented");
    }
    @Override
    public List<SubscriptionInfo> getAllSubInfoList(String callingPackage) {
        throw new RuntimeException("not implemented");
    }
    @Override
    public List<SubscriptionInfo> getActiveSubscriptionInfoList(String callingPackage) {
        throw new RuntimeException("not implemented");
    }
    @Override
    public int getActiveSubInfoCount(String callingPackage) {
        throw new RuntimeException("not implemented");
    }
    @Override
    public int getAllSubInfoCount(String callingPackage) {
        throw new RuntimeException("not implemented");
    }
    @Override
    public int getActiveSubInfoCountMax() {
        throw new RuntimeException("not implemented");
    }
    @Override
    public int addSubInfoRecord(String iccId, int slotIndex) {
        throw new RuntimeException("not implemented");
    }
    @Override
    public int addSubInfo(String uniqueId, String displayName, int slotIndex,
            int subscriptionType) {
        throw new RuntimeException("not implemented");
    }
    @Override
    public int removeSubInfo(String uniqueId, int subscriptionType) {
        throw new RuntimeException("not implemented");
    }
    @Override
    public boolean setPlmnSpn(int slotIndex, boolean showPlmn, String plmn, boolean showSpn,
            String spn) {
        throw new RuntimeException("not implemented");
    }
    @Override
    public int setIconTint(int tint, int subId) {
        throw new RuntimeException("not implemented");
    }
    @Override
    public int setDisplayNameUsingSrc(String displayName, int subId, int nameSource) {
        throw new RuntimeException("not implemented");
    }
    @Override
    public int setDisplayNumber(String number, int subId) {
        throw new RuntimeException("not implemented");
    }
    @Override
    public int setDataRoaming(int roaming, int subId) {
        throw new RuntimeException("not implemented");
    }
    @Override
    public int setMccMnc(String mccMnc, int subId) {
        throw new RuntimeException("not implemented");
    }
    @Override
    public int getSlotIndex(int subId) {
        throw new RuntimeException("not implemented");
    }

    private boolean isInvalidslotIndex(int slotIndex) {
        if (slotIndex < 0 || slotIndex >= mSlotIndexToSubId.length) return true;
        return false;
    }

    @Override
    public int[] getSubId(int slotIndex) {
        if (isInvalidslotIndex(slotIndex)) {
            return null;
        }
        return mSlotIndexToSubId[slotIndex];
    }
    public void setSlotSubId(int slotIndex, int subId) {
        if (isInvalidslotIndex(slotIndex)) {
            throw new RuntimeException("invalid slot specified" + slotIndex);
        }
        if (mSlotIndexToSubId[slotIndex][0] != subId) {
            mSlotIndexToSubId[slotIndex][0] = subId;
            try {
                mTelephonyRegistry.notifySubscriptionInfoChanged();
            } catch (RemoteException ex) {}
        }
    }
    @Override
    public int getPhoneId(int subId) {
        if (subId <= INVALID_SUBSCRIPTION_ID) return INVALID_PHONE_INDEX;

        for (int i = 0; i < mSlotIndexToSubId.length; i++) {
            if (mSlotIndexToSubId[i][0] == subId) return i;
        }
        return INVALID_PHONE_INDEX;
    }
    @Override
    public int clearSubInfo() {
        throw new RuntimeException("not implemented");
    }
    @Override
    public int getDefaultSubId() {
        throw new RuntimeException("not implemented");
    }
    @Override
    public void setDefaultSmsSubId(int subId) {
        throw new RuntimeException("not implemented");
    }
    @Override
    public int getDefaultSmsSubId() {
        throw new RuntimeException("not implemented");
    }
    @Override
    public void setDefaultVoiceSubId(int subId) {
        if (subId == DEFAULT_SUBSCRIPTION_ID) {
            throw new RuntimeException("setDefaultDataSubId called with DEFAULT_SUB_ID");
        }
        mDefaultVoiceSubId.set(subId);
        broadcastDefaultVoiceSubIdChanged(subId);
    }
    @Override
    public int getDefaultVoiceSubId() {
        if (mDefaultVoiceSubId != null) {
            return mDefaultVoiceSubId.get();
        } else {
            return INVALID_SUBSCRIPTION_ID;
        }
    }
    @Override
    public void updatePhonesAvailability(Phone[] phones) {
        throw new RuntimeException("not implemented");
    }
    @Override
    public int[] getActiveSubIdList(boolean visibleOnly) {
        throw new RuntimeException("not implemented");
    }
    @Override
    public boolean isActiveSubId(int subId) {
        return getPhoneId(subId) != INVALID_PHONE_INDEX;
    }
    @Override
    public int getSimStateForSlotIndex(int slotIndex) {
        throw new RuntimeException("not implemented");
    }
    @Override
    public int setSubscriptionProperty(int subId, String propKey, String propValue) {
        throw new RuntimeException("not implemented");
    }
    @Override
    public String getSubscriptionProperty(int subId, String propKey, String callingPackage) {
        throw new RuntimeException("not implemented");
    }
    @Override
    public void dump(FileDescriptor fd, PrintWriter pw, String[] args) {
        throw new RuntimeException("not implemented");
    }
}
