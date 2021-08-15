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

package com.android.internal.telephony.mocks;

import android.net.LinkProperties;
import android.net.NetworkCapabilities;
import android.os.Bundle;
import android.os.IBinder;
import android.os.RemoteException;
import android.os.UserHandle;
import android.telephony.CallQuality;
import android.telephony.CellInfo;
import android.telephony.DataFailCause;
import android.telephony.PhoneCapability;
import android.telephony.PhysicalChannelConfig;
import android.telephony.ServiceState;
import android.telephony.SignalStrength;
import android.telephony.SubscriptionManager;
import android.telephony.ims.ImsReasonInfo;

import com.android.internal.telephony.IOnSubscriptionsChangedListener;
import com.android.internal.telephony.IPhoneStateListener;
import com.android.internal.telephony.ITelephonyRegistry;

import java.util.ArrayList;
import java.util.List;

public class TelephonyRegistryMock extends ITelephonyRegistry.Stub {

    private static class Record {
        String callingPackage;

        IBinder binder;

        IPhoneStateListener callback;
        IOnSubscriptionsChangedListener onSubscriptionsChangedListenerCallback;
        IOnSubscriptionsChangedListener onOpportunisticSubscriptionsChangedListenerCallback;

        int callerUserId;

        int events;

        int subId = SubscriptionManager.INVALID_SUBSCRIPTION_ID;

        int phoneId = SubscriptionManager.INVALID_PHONE_INDEX;

        boolean canReadPhoneState;

        boolean matchPhoneStateListenerEvent(int events) {
            return (callback != null) && ((events & this.events) != 0);
        }

        boolean matchOnSubscriptionsChangedListener() {
            return (onSubscriptionsChangedListenerCallback != null);
        }

        boolean matchOnOpportunisticSubscriptionsChangedListener() {
            return (onOpportunisticSubscriptionsChangedListenerCallback != null);
        }


        @Override
        public String toString() {
            return "{callingPackage=" + callingPackage + " binder=" + binder
                    + " callback=" + callback
                    + " onSubscriptionsChangedListenerCallback="
                    + onSubscriptionsChangedListenerCallback
                    + " onOpportunisticSubscriptionsChangedListenerCallback="
                    + onOpportunisticSubscriptionsChangedListenerCallback
                    + " callerUserId=" + callerUserId + " subId=" + subId + " phoneId=" + phoneId
                    + " events=" + Integer.toHexString(events)
                    + " canReadPhoneState=" + canReadPhoneState + "}";
        }
    }

    private final ArrayList<IBinder> mRemoveList = new ArrayList<IBinder>();
    private final ArrayList<Record> mRecords = new ArrayList<Record>();
    private boolean mHasNotifySubscriptionInfoChangedOccurred = false;
    private boolean mHasNotifyOpportunisticSubscriptionInfoChangedOccurred = false;

    public TelephonyRegistryMock() {
    }

    private void remove(IBinder binder) {
        synchronized (mRecords) {
            final int recordCount = mRecords.size();
            for (int i = 0; i < recordCount; i++) {
                if (mRecords.get(i).binder == binder) {
                    mRecords.remove(i);
                    return;
                }
            }
        }
    }

    private void handleRemoveListLocked() {
        int size = mRemoveList.size();
        if (size > 0) {
            for (IBinder b: mRemoveList) {
                remove(b);
            }
            mRemoveList.clear();
        }
    }


    @Override
    public void addOnSubscriptionsChangedListener(String callingPackage,
            IOnSubscriptionsChangedListener callback) {
        Record r;

        synchronized (mRecords) {
            // register
            find_and_add: {
                IBinder b = callback.asBinder();
                final int N = mRecords.size();
                for (int i = 0; i < N; i++) {
                    r = mRecords.get(i);
                    if (b == r.binder) {
                        break find_and_add;
                    }
                }
                r = new Record();
                r.binder = b;
                mRecords.add(r);
            }

            r.onSubscriptionsChangedListenerCallback = callback;
            r.callingPackage = callingPackage;
            r.callerUserId = UserHandle.getCallingUserId();
            r.events = 0;
            r.canReadPhoneState = true; // permission has been enforced above
            // Always notify when registration occurs if there has been a notification.
            if (mHasNotifySubscriptionInfoChangedOccurred) {
                try {
                    r.onSubscriptionsChangedListenerCallback.onSubscriptionsChanged();
                } catch (RemoteException e) {
                    remove(r.binder);
                }
            } else {
                //log("listen oscl: mHasNotifySubscriptionInfoChangedOccurred==false no callback");
            }
        }

    }

    @Override
    public void addOnOpportunisticSubscriptionsChangedListener(String callingPackage,
            IOnSubscriptionsChangedListener callback) {
        Record r;

        synchronized (mRecords) {
            // register
            find_and_add: {
                IBinder b = callback.asBinder();
                final int n = mRecords.size();
                for (int i = 0; i < n; i++) {
                    r = mRecords.get(i);
                    if (b == r.binder) {
                        break find_and_add;
                    }
                }
                r = new Record();
                r.binder = b;
                mRecords.add(r);
            }

            r.onOpportunisticSubscriptionsChangedListenerCallback = callback;
            r.callingPackage = callingPackage;
            r.callerUserId = UserHandle.getCallingUserId();
            r.events = 0;
            r.canReadPhoneState = true; // permission has been enforced above
            // Always notify when registration occurs if there has been a notification.
            if (mHasNotifyOpportunisticSubscriptionInfoChangedOccurred) {
                try {
                    r.onOpportunisticSubscriptionsChangedListenerCallback.onSubscriptionsChanged();
                } catch (RemoteException e) {
                    remove(r.binder);
                }
            } else {
                //log("listen oscl: mHasNotifySubscriptionInfoChangedOccurred==false no callback");
            }
        }

    }

    @Override
    public void removeOnSubscriptionsChangedListener(String pkgForDebug,
            IOnSubscriptionsChangedListener callback) {
        remove(callback.asBinder());
    }

    @Override
    public void notifySubscriptionInfoChanged() {
        synchronized (mRecords) {
            if (!mHasNotifySubscriptionInfoChangedOccurred) {
                //log("notifySubscriptionInfoChanged: first invocation mRecords.size="
                //        + mRecords.size());
            }
            mHasNotifySubscriptionInfoChangedOccurred = true;
            mRemoveList.clear();
            for (Record r : mRecords) {
                if (r.matchOnSubscriptionsChangedListener()) {
                    try {
                        r.onSubscriptionsChangedListenerCallback.onSubscriptionsChanged();
                    } catch (RemoteException ex) {
                        mRemoveList.add(r.binder);
                    }
                }
            }
            handleRemoveListLocked();
        }
    }

    @Override
    public void notifyOpportunisticSubscriptionInfoChanged() {
        synchronized (mRecords) {
            if (!mHasNotifyOpportunisticSubscriptionInfoChangedOccurred) {
                //log("notifySubscriptionInfoChanged: first invocation mRecords.size="
                //        + mRecords.size());
            }
            mHasNotifyOpportunisticSubscriptionInfoChangedOccurred = true;
            mRemoveList.clear();
            for (Record r : mRecords) {
                if (r.matchOnOpportunisticSubscriptionsChangedListener()) {
                    try {
                        r.onOpportunisticSubscriptionsChangedListenerCallback
                                .onSubscriptionsChanged();
                    } catch (RemoteException ex) {
                        mRemoveList.add(r.binder);
                    }
                }
            }
            handleRemoveListLocked();
        }
    }

    @Override
    public void listen(String pkg, IPhoneStateListener callback, int events, boolean notifyNow) {
        throw new RuntimeException("Not implemented");
    }

    @Override
    public void listenForSubscriber(int subId, String pkg, IPhoneStateListener callback, int events,
                                    boolean notifyNow) {
        throw new RuntimeException("Not implemented");
    }

    @Override
    public void notifyCallState(int state, String incomingNumber) {
        throw new RuntimeException("Not implemented");
    }

    @Override
    public void notifyCallStateForPhoneId(int phoneId, int subId, int state,
                String incomingNumber) {
        throw new RuntimeException("Not implemented");
    }

    @Override
    public void notifyServiceStateForPhoneId(int phoneId, int subId, ServiceState state) {
        throw new RuntimeException("Not implemented");
    }

    @Override
    public void notifySignalStrengthForPhoneId(int phoneId, int subId,
                SignalStrength signalStrength) {
        throw new RuntimeException("Not implemented");
    }

    @Override
    public void notifyMessageWaitingChangedForPhoneId(int phoneId, int subId, boolean mwi) {
        throw new RuntimeException("Not implemented");
    }

    @Override
    public void notifyCallForwardingChanged(boolean cfi) {
        throw new RuntimeException("Not implemented");
    }

    @Override
    public void notifyCallForwardingChangedForSubscriber(int subId, boolean cfi) {
        throw new RuntimeException("Not implemented");
    }

    @Override
    public void notifyDataActivity(int state) {
        throw new RuntimeException("Not implemented");
    }

    @Override
    public void notifyDataActivityForSubscriber(int subId, int state) {
        throw new RuntimeException("Not implemented");
    }

    @Override
    public void notifyDataConnection(int state, boolean isDataConnectivityPossible,
            String apn, String apnType, LinkProperties linkProperties,
            NetworkCapabilities networkCapabilities, int networkType, boolean roaming) {
        throw new RuntimeException("Not implemented");
    }

    @Override
    public void notifyDataConnectionForSubscriber(int phoneId, int subId, int state,
            boolean isDataConnectivityPossible, String apn, String apnType,
            LinkProperties linkProperties, NetworkCapabilities networkCapabilities,
            int networkType, boolean roaming) {
        throw new RuntimeException("Not implemented");
    }

    @Override
    public void notifyDataConnectionFailed(String apnType) {
        throw new RuntimeException("Not implemented");
    }

    @Override
    public void notifyDataConnectionFailedForSubscriber(int phoneId, int subId, String apnType) {
        throw new RuntimeException("Not implemented");
    }

    @Override
    public void notifyCellLocation(Bundle cellLocation) {
        throw new RuntimeException("Not implemented");
    }

    @Override
    public void notifyCellLocationForSubscriber(int subId, Bundle cellLocation) {
        throw new RuntimeException("Not implemented");
    }

    @Override
    public void notifyOtaspChanged(int subId, int otaspMode) {
        throw new RuntimeException("Not implemented");
    }

    @Override
    public void notifyCellInfo(List<CellInfo> cellInfo) {
        throw new RuntimeException("Not implemented");
    }

    @Override
    public void notifyPhysicalChannelConfigurationForSubscriber(int phoneId, int subId,
            List<PhysicalChannelConfig> configs) {
        throw new RuntimeException("Not implemented");
    }

    @Override
    public void notifyEmergencyNumberList(int phoneId, int subId) {
        throw new RuntimeException("Not implemented");
    }

    @Override
    public void notifyCallQualityChanged(CallQuality callQuality, int phoneId, int subId,
            int callNetworkType) {
        throw new RuntimeException("Not implemented");
    }

    @Override
    public void notifyPreciseCallState(int phoneId, int subId, int ringingCallState,
                                       int foregroundCallState, int backgroundCallState) {
        throw new RuntimeException("Not implemented");
    }

    @Override
    public void notifyDisconnectCause(int phoneId, int subId, int disconnectCause,
                                      int preciseDisconnectCause) {
        throw new RuntimeException("Not implemented");
    }

    @Override
    public void notifyPreciseDataConnectionFailed(int phoneId, int subId,
                                                  String apnType, String apn,
                                                  @DataFailCause.FailCause int failCause) {
        throw new RuntimeException("Not implemented");
    }

    @Override
    public void notifyCellInfoForSubscriber(int subId, List<CellInfo> cellInfo) {
        throw new RuntimeException("Not implemented");
    }

    @Override
    public void notifySrvccStateChanged(int subId, int state) {
        throw new RuntimeException("Not implemented");
    }

    @Override
    public void notifyOemHookRawEventForSubscriber(int phoneId, int subId, byte[] rawData) {
        throw new RuntimeException("Not implemented");
    }

    @Override
    public void notifyCarrierNetworkChange(boolean active) {
        throw new RuntimeException("Not implemented");
    }

    @Override
    public void notifySimActivationStateChangedForPhoneId(int phoneId, int subId,
                                                          int activationType, int state) {
        throw new RuntimeException("Not implemented");
    }

    @Override
    public void notifyUserMobileDataStateChangedForPhoneId(int phoneId, int subId, boolean state) {
    }

    @Override
    public void notifyPhoneCapabilityChanged(PhoneCapability capability) {
        throw new RuntimeException("Not implemented");
    }

    @Override
    public void notifyActiveDataSubIdChanged(int subId) {
        throw new RuntimeException("Not implemented");
    }

    @Override
    public void notifyRadioPowerStateChanged(int phoneId, int subId, int state) {
        throw new RuntimeException("Not implemented");
    }

    @Override
    public void notifyImsDisconnectCause(int subId, ImsReasonInfo imsReasonInfo)  {
        throw new RuntimeException("Not implemented");
    }
}
