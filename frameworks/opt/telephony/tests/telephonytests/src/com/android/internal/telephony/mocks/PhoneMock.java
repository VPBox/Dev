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

package com.android.internal.telephony;

import android.content.Context;
import android.net.LinkProperties;
import android.net.NetworkCapabilities;
import android.os.AsyncResult;
import android.os.Handler;
import android.os.Message;
import android.os.Registrant;
import android.os.RegistrantList;
import android.os.ResultReceiver;
import android.os.WorkSource;
import android.service.carrier.CarrierIdentifier;
import android.telephony.CellInfo;
import android.telephony.CellLocation;
import android.telephony.DataConnectionRealTimeInfo;
import android.telephony.NetworkScanRequest;
import android.telephony.ServiceState;
import android.telephony.SignalStrength;
import android.telephony.VoLteServiceState;

import com.android.internal.annotations.VisibleForTesting;
import com.android.internal.telephony.test.SimulatedRadioControl;
import com.android.internal.telephony.uicc.IccCardApplicationStatus.AppType;
import com.android.internal.telephony.uicc.IccFileHandler;
import com.android.internal.telephony.uicc.IsimRecords;
import com.android.internal.telephony.uicc.UiccCard;
import com.android.internal.telephony.uicc.UsimServiceTable;

import java.io.FileDescriptor;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.concurrent.atomic.AtomicBoolean;

/**
 * (<em>Not for SDK use</em>)
 * A base implementation for the com.android.internal.telephony.Phone interface.
 *
 * Note that implementations of Phone.java are expected to be used
 * from a single application thread. This should be the same thread that
 * originally called PhoneFactory to obtain the interface.
 *
 *  {@hide}
 *
 */

public class PhoneMock extends Phone {
    private static final String LOG_TAG = "PhoneMock";

    protected PhoneMock(String name, PhoneNotifier notifier, Context context, CommandsInterface ci,
            boolean unitTestMode) {
        super(name, notifier, context, ci, unitTestMode);
        throw new RuntimeException("not implemented");
    }

    protected PhoneMock(String name, PhoneNotifier notifier, Context context, CommandsInterface ci,
            boolean unitTestMode, int phoneId,
            TelephonyComponentFactory telephonyComponentFactory) {
        super(name, notifier, context, ci, unitTestMode, phoneId, telephonyComponentFactory);
        throw new RuntimeException("not implemented");
    }

    public String getPhoneName() {
        throw new RuntimeException("not implemented");
    }

    protected void setPhoneName(String name) {
        throw new RuntimeException("not implemented");
    }

    public String getNai(){
        throw new RuntimeException("not implemented");
    }

    public String getActionDetached() {
        return "phonemock.action.detached.not.implemented";
    }

    public String getActionAttached() {
        return "phonemock.action.attached.not.implemented";
    }

    public void setSystemProperty(String property, String value) {
        throw new RuntimeException("not implemented");
    }

    public String getSystemProperty(String property, String defValue) {
        throw new RuntimeException("not implemented");
    }

    protected final RegistrantList mPreciseCallStateRegistrants
            = new RegistrantList();

    protected final RegistrantList mHandoverRegistrants
             = new RegistrantList();

    protected final RegistrantList mNewRingingConnectionRegistrants
            = new RegistrantList();

    protected final RegistrantList mIncomingRingRegistrants
            = new RegistrantList();

    protected final RegistrantList mDisconnectRegistrants
            = new RegistrantList();

    protected final RegistrantList mServiceStateRegistrants
            = new RegistrantList();

    protected final RegistrantList mMmiCompleteRegistrants
            = new RegistrantList();

    protected final RegistrantList mMmiRegistrants
            = new RegistrantList();

    protected final RegistrantList mUnknownConnectionRegistrants
            = new RegistrantList();

    protected final RegistrantList mSuppServiceFailedRegistrants
            = new RegistrantList();

    protected final RegistrantList mRadioOffOrNotAvailableRegistrants
            = new RegistrantList();

    protected final RegistrantList mSimRecordsLoadedRegistrants
            = new RegistrantList();

    protected final RegistrantList mVideoCapabilityChangedRegistrants
            = new RegistrantList();

    protected final RegistrantList mEmergencyCallToggledRegistrants
            = new RegistrantList();


    public void startMonitoringImsService() {
        throw new RuntimeException("not implemented");
    }

    @Override
    public void handleMessage(Message msg) {
        throw new RuntimeException("not implemented");
    }

    @Override
    public boolean handleUssdRequest(String ussdRequest, ResultReceiver wrappedCallback) {
        throw new RuntimeException("not implemented");
    }

    public ArrayList<Connection> getHandoverConnection() {
        throw new RuntimeException("not implemented");
    }

    public void notifySrvccState(Call.SrvccState state) {
        throw new RuntimeException("not implemented");
    }

    public void registerForSilentRedial(Handler h, int what, Object obj) {
        throw new RuntimeException("not implemented");
    }

    public void unregisterForSilentRedial(Handler h) {
        throw new RuntimeException("not implemented");
    }

    public Context getContext() {
        return mContext;
    }

    protected void onUpdateIccAvailability() {
        throw new RuntimeException("not implemented");
    }

    public void disableDnsCheck(boolean b) {
        throw new RuntimeException("not implemented");
    }

    public boolean isDnsCheckDisabled() {
        throw new RuntimeException("not implemented");
    }

    public void registerForPreciseCallStateChanged(Handler h, int what, Object obj) {
        mPreciseCallStateRegistrants.addUnique(h, what, obj);
    }

    public void unregisterForPreciseCallStateChanged(Handler h) {
        mPreciseCallStateRegistrants.remove(h);
    }

    protected void notifyPreciseCallStateChangedP() {
        AsyncResult ar = new AsyncResult(null, this, null);
        mPreciseCallStateRegistrants.notifyRegistrants(ar);

        mNotifier.notifyPreciseCallState(this);
    }

    public void registerForHandoverStateChanged(Handler h, int what, Object obj) {
        mHandoverRegistrants.addUnique(h, what, obj);
    }

    public void unregisterForHandoverStateChanged(Handler h) {
        mHandoverRegistrants.remove(h);
    }

    public void notifyHandoverStateChanged(Connection cn) {
       AsyncResult ar = new AsyncResult(null, cn, null);
       mHandoverRegistrants.notifyRegistrants(ar);
    }

    public void migrateFrom(Phone from) {
        throw new RuntimeException("not implemented");
    }

    public void migrate(RegistrantList to, RegistrantList from) {
        throw new RuntimeException("not implemented");
    }

    public void registerForUnknownConnection(Handler h, int what, Object obj) {
        mUnknownConnectionRegistrants.addUnique(h, what, obj);
    }

    public void unregisterForUnknownConnection(Handler h) {
        mUnknownConnectionRegistrants.remove(h);
    }

    public void registerForNewRingingConnection(
            Handler h, int what, Object obj) {
        mNewRingingConnectionRegistrants.addUnique(h, what, obj);
    }

    public void unregisterForNewRingingConnection(Handler h) {
        mNewRingingConnectionRegistrants.remove(h);
    }

    public void registerForVideoCapabilityChanged(
            Handler h, int what, Object obj) {
        mVideoCapabilityChangedRegistrants.addUnique(h, what, obj);
        notifyForVideoCapabilityChanged(mIsVideoCapable);
    }

    public void unregisterForVideoCapabilityChanged(Handler h) {
        mVideoCapabilityChangedRegistrants.remove(h);
    }

    public void registerForInCallVoicePrivacyOn(Handler h, int what, Object obj){
        throw new RuntimeException("not implemented");
    }

    public void unregisterForInCallVoicePrivacyOn(Handler h){
        throw new RuntimeException("not implemented");
    }

    public void registerForInCallVoicePrivacyOff(Handler h, int what, Object obj){
        throw new RuntimeException("not implemented");
    }

    public void unregisterForInCallVoicePrivacyOff(Handler h){
        throw new RuntimeException("not implemented");
    }

    public void registerForIncomingRing(
            Handler h, int what, Object obj) {
        mIncomingRingRegistrants.addUnique(h, what, obj);
    }

    public void unregisterForIncomingRing(Handler h) {
        mIncomingRingRegistrants.remove(h);
    }

    public void registerForDisconnect(Handler h, int what, Object obj) {
        mDisconnectRegistrants.addUnique(h, what, obj);
    }

    public void unregisterForDisconnect(Handler h) {
        mDisconnectRegistrants.remove(h);
    }

    public void registerForSuppServiceFailed(Handler h, int what, Object obj) {
        mSuppServiceFailedRegistrants.addUnique(h, what, obj);
    }

    public void unregisterForSuppServiceFailed(Handler h) {
        mSuppServiceFailedRegistrants.remove(h);
    }

    public void registerForMmiInitiate(Handler h, int what, Object obj) {
        mMmiRegistrants.addUnique(h, what, obj);
    }

    public void unregisterForMmiInitiate(Handler h) {
        mMmiRegistrants.remove(h);
    }

    public void registerForMmiComplete(Handler h, int what, Object obj) {
        mMmiCompleteRegistrants.addUnique(h, what, obj);
    }

    public void unregisterForMmiComplete(Handler h) {
        mMmiCompleteRegistrants.remove(h);
    }

    public void registerForSimRecordsLoaded(Handler h, int what, Object obj) {
        throw new RuntimeException("not implemented");
    }

    public void unregisterForSimRecordsLoaded(Handler h) {
        throw new RuntimeException("not implemented");
    }

    public void registerForTtyModeReceived(Handler h, int what, Object obj) {
        throw new RuntimeException("not implemented");
    }

    public void unregisterForTtyModeReceived(Handler h) {
        throw new RuntimeException("not implemented");
    }

    public void setNetworkSelectionModeAutomatic(Message response) {
        throw new RuntimeException("not implemented");
    }

    public void getNetworkSelectionMode(Message message) {
        throw new RuntimeException("not implemented");
    }

    public void selectNetworkManually(OperatorInfo network, boolean persistSelection,
            Message response) {
        throw new RuntimeException("not implemented");
    }

    public void registerForEmergencyCallToggle(Handler h, int what, Object obj) {
        Registrant r = new Registrant(h, what, obj);
        mEmergencyCallToggledRegistrants.add(r);
    }

    public void unregisterForEmergencyCallToggle(Handler h) {
        mEmergencyCallToggledRegistrants.remove(h);
    }

    public void restoreSavedNetworkSelection(Message response) {
        throw new RuntimeException("not implemented");
    }

    public void saveClirSetting(int commandInterfaceCLIRMode) {
        throw new RuntimeException("not implemented");
    }

    public void registerForServiceStateChanged(Handler h, int what, Object obj) {
        mServiceStateRegistrants.add(h, what, obj);
        throw new RuntimeException("not implemented");
    }

    public void unregisterForServiceStateChanged(Handler h) {
        mServiceStateRegistrants.remove(h);
        throw new RuntimeException("not implemented");
    }

    public void registerForRingbackTone(Handler h, int what, Object obj) {
        throw new RuntimeException("not implemented");
    }

    public void unregisterForRingbackTone(Handler h) {
        throw new RuntimeException("not implemented");
    }

    public void registerForOnHoldTone(Handler h, int what, Object obj) {
        throw new RuntimeException("not implemented");
    }

    public void unregisterForOnHoldTone(Handler h) {
        throw new RuntimeException("not implemented");
    }

    public void registerForResendIncallMute(Handler h, int what, Object obj) {
        throw new RuntimeException("not implemented");
    }

    public void unregisterForResendIncallMute(Handler h) {
        throw new RuntimeException("not implemented");
    }

    public void setEchoSuppressionEnabled() {
        throw new RuntimeException("not implemented");
    }

    public SimulatedRadioControl getSimulatedRadioControl() {
        throw new RuntimeException("not implemented");
    }

    public PhoneConstants.State getState() {
        throw new RuntimeException("not implemented");
    }

    public IccFileHandler getIccFileHandler(){
        throw new RuntimeException("not implemented");
    }

    public Handler getHandler() {
        return this;
    }

    public void updatePhoneObject(int voiceRadioTech) {
        throw new RuntimeException("not implemented");
    }

    public ServiceStateTracker getServiceStateTracker() {
        throw new RuntimeException("not implemented");
    }

    public CallTracker getCallTracker() {
        throw new RuntimeException("not implemented");
    }

    public AppType getCurrentUiccAppType() {
        throw new RuntimeException("not implemented");
    }

    public IccCard getIccCard() {
        throw new RuntimeException("not implemented");
    }

    public String getIccSerialNumber() {
        throw new RuntimeException("not implemented");
    }

    public boolean getIccRecordsLoaded() {
        throw new RuntimeException("not implemented");
    }

    public List<CellInfo> getAllCellInfo() {
        throw new RuntimeException("not implemented");
    }

    public void setCellInfoListRate(int rateInMillis) {
        throw new RuntimeException("not implemented");
    }

    public boolean getMessageWaitingIndicator() {
        throw new RuntimeException("not implemented");
    }

    public void setVoiceCallForwardingFlag(int line, boolean enable, String number) {
        throw new RuntimeException("not implemented");
    }

    public boolean getCallForwardingIndicator() {
        throw new RuntimeException("not implemented");
    }

    public void queryCdmaRoamingPreference(Message response) {
        throw new RuntimeException("not implemented");
    }

    public SignalStrength getSignalStrength() {
        throw new RuntimeException("not implemented");
    }

    public void setCdmaRoamingPreference(int cdmaRoamingType, Message response) {
        throw new RuntimeException("not implemented");
    }

    public void setCdmaSubscription(int cdmaSubscriptionType, Message response) {
        throw new RuntimeException("not implemented");
    }

    public void setPreferredNetworkType(int networkType, Message response) {
        throw new RuntimeException("not implemented");
    }

    public void getPreferredNetworkType(Message response) {
        throw new RuntimeException("not implemented");
    }

    public void getSmscAddress(Message result) {
        throw new RuntimeException("not implemented");
    }

    public void setSmscAddress(String address, Message result) {
        throw new RuntimeException("not implemented");
    }

    public void setTTYMode(int ttyMode, Message onComplete) {
        throw new RuntimeException("not implemented");
    }

    public void setUiTTYMode(int uiTtyMode, Message onComplete) {
        throw new RuntimeException("not implemented");
    }

    public void queryTTYMode(Message onComplete) {
        throw new RuntimeException("not implemented");
    }

    public void enableEnhancedVoicePrivacy(boolean enable, Message onComplete) {
        throw new RuntimeException("not implemented");
    }

    public void getEnhancedVoicePrivacy(Message onComplete) {
        throw new RuntimeException("not implemented");
    }

    public void setBandMode(int bandMode, Message response) {
        throw new RuntimeException("not implemented");
    }

    public void queryAvailableBandMode(Message response) {
        throw new RuntimeException("not implemented");
    }

    public void invokeOemRilRequestRaw(byte[] data, Message response) {
        throw new RuntimeException("not implemented");
    }

    public void invokeOemRilRequestStrings(String[] strings, Message response) {
        throw new RuntimeException("not implemented");
    }

    public void nvReadItem(int itemID, Message response) {
        throw new RuntimeException("not implemented");
    }

    public void nvWriteItem(int itemID, String itemValue, Message response) {
        throw new RuntimeException("not implemented");
    }

    public void nvWriteCdmaPrl(byte[] preferredRoamingList, Message response) {
        throw new RuntimeException("not implemented");
    }

    public void nvResetConfig(int resetType, Message response) {
        throw new RuntimeException("not implemented");
    }

    public void notifyDataActivity() {
        throw new RuntimeException("not implemented");
    }

    public void notifyMessageWaitingIndicator() {
        throw new RuntimeException("not implemented");
    }

    public void notifyDataConnection(String reason, String apnType,
            PhoneConstants.DataState state) {
        throw new RuntimeException("not implemented");
    }

    public void notifyDataConnection(String reason, String apnType) {
        throw new RuntimeException("not implemented");
    }

    public void notifyDataConnection(String reason) {
        throw new RuntimeException("not implemented");
    }

    public void notifyOtaspChanged(int otaspMode) {
        throw new RuntimeException("not implemented");
    }

    public void notifySignalStrength() {
        throw new RuntimeException("not implemented");
    }

    public void notifyCellInfo(List<CellInfo> cellInfo) {
        throw new RuntimeException("not implemented");
    }

    public void notifyDataConnectionRealTimeInfo(DataConnectionRealTimeInfo dcRtInfo) {
        throw new RuntimeException("not implemented");
    }

    public void notifyVoLteServiceStateChanged(VoLteServiceState lteState) {
        throw new RuntimeException("not implemented");
    }

    private final AtomicBoolean mInEmergencyCall = new AtomicBoolean(false);

    public boolean isInEmergencyCall() {
        return mInEmergencyCall.get();
    }

    @VisibleForTesting
    public void setInEmergencyCall(boolean value) {
        final boolean oldValue = mInEmergencyCall.getAndSet(value);
        if (oldValue != value) {
            mEmergencyCallToggledRegistrants.notifyRegistrants();
        }
    }

    private final AtomicBoolean mInEcm = new AtomicBoolean(false);

    public boolean isInEcm() {
        return mInEcm.get();
    }

    @VisibleForTesting
    public void setInEcm(boolean value) {
        final boolean oldValue = mInEcm.getAndSet(value);
        if (oldValue != value) {
            mEmergencyCallToggledRegistrants.notifyRegistrants();
        }
    }

    public boolean isVideoCallPresent() {
        throw new RuntimeException("not implemented");
    }

    public int getPhoneType() {
        return PhoneConstants.PHONE_TYPE_GSM;
    }

    public int getPrecisePhoneType() {
        throw new RuntimeException("not implemented");
    }

    public int getVoiceMessageCount(){
        throw new RuntimeException("not implemented");
    }

    public void setVoiceMessageCount(int countWaiting) {
        throw new RuntimeException("not implemented");
    }

    public int getCdmaEriIconIndex() {
        throw new RuntimeException("not implemented");
    }

    public int getCdmaEriIconMode() {
        throw new RuntimeException("not implemented");
    }

    public String getCdmaEriText() {
        throw new RuntimeException("not implemented");
    }

    public String getCdmaMin() {
        throw new RuntimeException("not implemented");
    }

    public boolean isMinInfoReady() {
        throw new RuntimeException("not implemented");
    }

    public String getCdmaPrlVersion(){
        throw new RuntimeException("not implemented");
    }

    public void sendBurstDtmf(String dtmfString, int on, int off, Message onComplete) {
        throw new RuntimeException("not implemented");
    }

    public void exitEmergencyCallbackMode() {
        throw new RuntimeException("not implemented");
    }

    public void registerForCdmaOtaStatusChange(Handler h, int what, Object obj) {
        throw new RuntimeException("not implemented");
    }

    public void unregisterForCdmaOtaStatusChange(Handler h) {
        throw new RuntimeException("not implemented");
    }

    public void registerForSubscriptionInfoReady(Handler h, int what, Object obj) {
        throw new RuntimeException("not implemented");
    }

    public void unregisterForSubscriptionInfoReady(Handler h) {
        throw new RuntimeException("not implemented");
    }

    public boolean needsOtaServiceProvisioning() {
        throw new RuntimeException("not implemented");
    }

    public  boolean isOtaSpNumber(String dialStr) {
        throw new RuntimeException("not implemented");
    }

    public void registerForCallWaiting(Handler h, int what, Object obj){
        throw new RuntimeException("not implemented");
    }

    public void unregisterForCallWaiting(Handler h){
        throw new RuntimeException("not implemented");
    }

    public void registerForEcmTimerReset(Handler h, int what, Object obj) {
        throw new RuntimeException("not implemented");
    }

    public void unregisterForEcmTimerReset(Handler h) {
        throw new RuntimeException("not implemented");
    }

    public void registerForSignalInfo(Handler h, int what, Object obj) {
        throw new RuntimeException("not implemented");
    }

    public void unregisterForSignalInfo(Handler h) {
        throw new RuntimeException("not implemented");
    }

    public void registerForDisplayInfo(Handler h, int what, Object obj) {
        throw new RuntimeException("not implemented");
    }

    public void unregisterForDisplayInfo(Handler h) {
        throw new RuntimeException("not implemented");
    }

    public void registerForNumberInfo(Handler h, int what, Object obj) {
        throw new RuntimeException("not implemented");
    }

    public void unregisterForNumberInfo(Handler h) {
        throw new RuntimeException("not implemented");
    }

    public void registerForRedirectedNumberInfo(Handler h, int what, Object obj) {
        throw new RuntimeException("not implemented");
    }

    public void unregisterForRedirectedNumberInfo(Handler h) {
        throw new RuntimeException("not implemented");
    }

    public void registerForLineControlInfo(Handler h, int what, Object obj) {
        throw new RuntimeException("not implemented");
    }

    public void unregisterForLineControlInfo(Handler h) {
        throw new RuntimeException("not implemented");
    }

    public void registerFoT53ClirlInfo(Handler h, int what, Object obj) {
        throw new RuntimeException("not implemented");
    }

    public void unregisterForT53ClirInfo(Handler h) {
        throw new RuntimeException("not implemented");
    }

    public void registerForT53AudioControlInfo(Handler h, int what, Object obj) {
        throw new RuntimeException("not implemented");
    }

    public void unregisterForT53AudioControlInfo(Handler h) {
        throw new RuntimeException("not implemented");
    }

    public void setOnEcbModeExitResponse(Handler h, int what, Object obj){
        throw new RuntimeException("not implemented");
    }

    public void unsetOnEcbModeExitResponse(Handler h){
        throw new RuntimeException("not implemented");
    }

    public void registerForRadioOffOrNotAvailable(Handler h, int what, Object obj) {
        throw new RuntimeException("not implemented");
    }

    public void unregisterForRadioOffOrNotAvailable(Handler h) {
        throw new RuntimeException("not implemented");
    }

    public String[] getActiveApnTypes() {
        throw new RuntimeException("not implemented");
    }

    public boolean hasMatchedTetherApnSetting() {
        throw new RuntimeException("not implemented");
    }

    public String getActiveApnHost(String apnType) {
        throw new RuntimeException("not implemented");
    }

    public LinkProperties getLinkProperties(String apnType) {
        throw new RuntimeException("not implemented");
    }

    public NetworkCapabilities getNetworkCapabilities(String apnType) {
        throw new RuntimeException("not implemented");
    }

    public boolean isDataAllowed() {
        throw new RuntimeException("not implemented");
    }

    public void notifyNewRingingConnectionP(Connection cn) {
        throw new RuntimeException("not implemented");
    }

    public void notifyUnknownConnectionP(Connection cn) {
        throw new RuntimeException("not implemented");
    }

    public void notifyForVideoCapabilityChanged(boolean isVideoCallCapable) {
        throw new RuntimeException("not implemented");
    }

    public boolean isCspPlmnEnabled() {
        throw new RuntimeException("not implemented");
    }

    public IsimRecords getIsimRecords() {
        throw new RuntimeException("not implemented");
    }

    public String getMsisdn() {
        throw new RuntimeException("not implemented");
    }

    public PhoneConstants.DataState getDataConnectionState() {
        throw new RuntimeException("not implemented");
    }

    public void notifyCallForwardingIndicator() {
        throw new RuntimeException("not implemented");
    }

    public void notifyDataConnectionFailed(String reason, String apnType) {
        throw new RuntimeException("not implemented");
    }

    public void notifyPreciseDataConnectionFailed(String reason, String apnType, String apn,
            String failCause) {
        throw new RuntimeException("not implemented");
    }

    public int getLteOnCdmaMode() {
        throw new RuntimeException("not implemented");
    }

    public void setVoiceMessageWaiting(int line, int countWaiting) {
        throw new RuntimeException("not implemented");
    }

    public UsimServiceTable getUsimServiceTable() {
        throw new RuntimeException("not implemented");
    }

    public UiccCard getUiccCard() {
        throw new RuntimeException("not implemented");
    }

    public String[] getPcscfAddress(String apnType) {
        throw new RuntimeException("not implemented");
    }

    public void setImsRegistrationState(boolean registered) {
        throw new RuntimeException("not implemented");
    }

    public Phone getImsPhone() {
        throw new RuntimeException("not implemented");
    }

    public boolean isUtEnabled() {
        throw new RuntimeException("not implemented");
    }

    public void dispose() {
        throw new RuntimeException("not implemented");
    }

    public int getSubId() {
        throw new RuntimeException("not implemented");
    }

    public int getPhoneId() {
        throw new RuntimeException("not implemented");
    }

    public int getVoicePhoneServiceState() {
        throw new RuntimeException("not implemented");
    }

    public boolean setOperatorBrandOverride(String brand) {
        throw new RuntimeException("not implemented");
    }

    public boolean setRoamingOverride(List<String> gsmRoamingList,
            List<String> gsmNonRoamingList, List<String> cdmaRoamingList,
            List<String> cdmaNonRoamingList) {
        throw new RuntimeException("not implemented");
    }

    public boolean isMccMncMarkedAsRoaming(String mccMnc) {
        throw new RuntimeException("not implemented");
    }

    public boolean isMccMncMarkedAsNonRoaming(String mccMnc) {
        throw new RuntimeException("not implemented");
    }

    public boolean isSidMarkedAsRoaming(int SID) {
        throw new RuntimeException("not implemented");
    }

    public boolean isSidMarkedAsNonRoaming(int SID) {
        throw new RuntimeException("not implemented");
    }

    public boolean isImsRegistered() {
        throw new RuntimeException("not implemented");
    }

    public boolean isWifiCallingEnabled() {
        throw new RuntimeException("not implemented");
    }

    public boolean isVolteEnabled() {
        throw new RuntimeException("not implemented");
    }

    public boolean isRadioAvailable() {
        throw new RuntimeException("not implemented");
    }

    public boolean isRadioOn() {
        throw new RuntimeException("not implemented");
    }

    public void shutdownRadio() {
        throw new RuntimeException("not implemented");
    }

    public boolean isShuttingDown() {
        throw new RuntimeException("not implemented");
    }

    public void setRadioCapability(RadioCapability rc, Message response) {
        throw new RuntimeException("not implemented");
    }

    public int getRadioAccessFamily() {
        throw new RuntimeException("not implemented");
    }

    public String getModemUuId() {
        throw new RuntimeException("not implemented");
    }

    public RadioCapability getRadioCapability() {
        throw new RuntimeException("not implemented");
    }

    public void radioCapabilityUpdated(RadioCapability rc) {
        throw new RuntimeException("not implemented");
    }

    public void sendSubscriptionSettings(boolean restoreNetworkSelection) {
        throw new RuntimeException("not implemented");
    }

    public void registerForRadioCapabilityChanged(Handler h, int what, Object obj) {
        throw new RuntimeException("not implemented");
    }

    public void unregisterForRadioCapabilityChanged(Handler h) {
        throw new RuntimeException("not implemented");
    }

    public boolean isImsUseEnabled() {
        throw new RuntimeException("not implemented");
    }

    public boolean isVideoEnabled() {
        throw new RuntimeException("not implemented");
    }

    public int getLceStatus() {
        throw new RuntimeException("not implemented");
    }

    public void getModemActivityInfo(Message response)  {
        throw new RuntimeException("not implemented");
    }

    public void setAllowedCarriers(List<CarrierIdentifier> carrierList, Message response) {
        throw new RuntimeException("not implemented");
    }

    public void getAllowedCarriers(Message response) {
        throw new RuntimeException("not implemented");
    }

    public void startLceAfterRadioIsAvailable() {
        throw new RuntimeException("not implemented");
    }

    public Locale getLocaleFromSimAndCarrierPrefs() {
        throw new RuntimeException("not implemented");
    }

    public void updateDataConnectionTracker() {
        throw new RuntimeException("not implemented");
    }

    public void setInternalDataEnabled(boolean enable, Message onCompleteMsg) {
        throw new RuntimeException("not implemented");
    }

    public boolean updateCurrentCarrierInProvider() {
        throw new RuntimeException("not implemented");
    }

    public void registerForAllDataDisconnected(Handler h, int what) {
        throw new RuntimeException("not implemented");
    }

    public void unregisterForAllDataDisconnected(Handler h) {
        throw new RuntimeException("not implemented");
    }

    public IccSmsInterfaceManager getIccSmsInterfaceManager(){
        throw new RuntimeException("not implemented");
    }

    public static void checkWfcWifiOnlyModeBeforeDial(Phone imsPhone, Context context) {
        throw new RuntimeException("not implemented");
    }

    public void startRingbackTone() {
        throw new RuntimeException("not implemented");
    }

    public void stopRingbackTone() {
        throw new RuntimeException("not implemented");
    }

    public void callEndCleanupHandOverCallIfAny() {
        throw new RuntimeException("not implemented");
    }

    public void cancelUSSD(Message msg) {
        throw new RuntimeException("not implemented");
    }

    public void setBroadcastEmergencyCallStateChanges(boolean broadcast) {
        throw new RuntimeException("not implemented");
    }

    public void sendEmergencyCallStateChange(boolean callActive) {
        throw new RuntimeException("not implemented");
    }

    public Phone getDefaultPhone() {
        throw new RuntimeException("not implemented");
    }

    /** From PhoneInternalInterface - man this class has alot of functions */
    public ServiceState getServiceState() {
        throw new RuntimeException("not implemented");
    }

    public CellLocation getCellLocation(WorkSource workSource) {
        throw new RuntimeException("not implemented");
    }

    public DataActivityState getDataActivityState() {
        throw new RuntimeException("not implemented");
    }

    public List<? extends MmiCode> getPendingMmiCodes() {
        throw new RuntimeException("not implemented");
    }

    public void sendUssdResponse(String ussdMessge) {
        throw new RuntimeException("not implemented");
    }

    public void registerForSuppServiceNotification(Handler h, int what, Object obj) {
        throw new RuntimeException("not implemented");
    }

    public void unregisterForSuppServiceNotification(Handler h) {
        throw new RuntimeException("not implemented");
    }

    public void acceptCall(int videoState) throws CallStateException {
        throw new RuntimeException("not implemented");
    }

    public void rejectCall() throws CallStateException {
        throw new RuntimeException("not implemented");
    }

    public void switchHoldingAndActive() throws CallStateException {
        throw new RuntimeException("not implemented");
    }

    public boolean canConference() {
        throw new RuntimeException("not implemented");
    }

    public void conference() throws CallStateException {
        throw new RuntimeException("not implemented");
    }

    public boolean canTransfer() {
        throw new RuntimeException("not implemented");
    }

    public void explicitCallTransfer() throws CallStateException {
        throw new RuntimeException("not implemented");
    }

    public void clearDisconnected() {
        throw new RuntimeException("not implemented");
    }

    public Call getForegroundCall() {
        throw new RuntimeException("not implemented");
    }

    public Call getBackgroundCall() {
        throw new RuntimeException("not implemented");
    }

    public Call getRingingCall() {
        throw new RuntimeException("not implemented");
    }

    @Override
    public Connection dial(String dialString, DialArgs dialArgs) throws CallStateException {
        throw new RuntimeException("not implemented");
    }

    public boolean handlePinMmi(String dialString) {
        throw new RuntimeException("not implemented");
    }

    public boolean handleUssdServiceCall(String dialString, Callback wrappedCallback) {
        throw new RuntimeException("not implemented");
    }

    public boolean handleInCallMmiCommands(String command) throws CallStateException {
        throw new RuntimeException("not implemented");
    }

    public void sendDtmf(char c) {
        throw new RuntimeException("not implemented");
    }

    public void startDtmf(char c) {
        throw new RuntimeException("not implemented");
    }

    public void stopDtmf() {
        throw new RuntimeException("not implemented");
    }

    public void setRadioPower(boolean power) {
        throw new RuntimeException("not implemented");
    }

    public String getLine1Number() {
        throw new RuntimeException("not implemented");
    }

    public String getLine1AlphaTag() {
        throw new RuntimeException("not implemented");
    }

    public boolean setLine1Number(String alphaTag, String number, Message onComplete) {
        throw new RuntimeException("not implemented");
    }

    public String getVoiceMailNumber() {
        throw new RuntimeException("not implemented");
    }

    public String getVoiceMailAlphaTag() {
        throw new RuntimeException("not implemented");
    }

    public void setVoiceMailNumber(String alphaTag, String voiceMailNumber, Message onComplete) {
        throw new RuntimeException("not implemented");
    }

    public void getCallForwardingOption(int commandInterfaceCFReason, Message onComplete) {
        throw new RuntimeException("not implemented");
    }

    public void setCallForwardingOption(int commandInterfaceCFReason, int commandInterfaceCFAction,
            String dialingNumber, int timerSeconds, Message onComplete) {
        throw new RuntimeException("not implemented");
    }

    public void getCallBarring(String facility, String password, Message onComplete,
            int serviceClass) {
    }

    public void setCallBarring(String facility, boolean lockState, String password,
            Message onComplete, int serviceClass) {
    }

    public void getOutgoingCallerIdDisplay(Message onComplete) {
        throw new RuntimeException("not implemented");
    }

    public void setOutgoingCallerIdDisplay(int commandInterfaceCLIRMode, Message onComplete) {
        throw new RuntimeException("not implemented");
    }

    public void getCallWaiting(Message onComplete) {
        throw new RuntimeException("not implemented");
    }

    public void setCallWaiting(boolean enable, Message onComplete) {
        throw new RuntimeException("not implemented");
    }

    public void getAvailableNetworks(Message response) {
        throw new RuntimeException("not implemented");
    }

    public void startNetworkScan(NetworkScanRequest nsr, Message response) {
        throw new RuntimeException("not implemented");
    }

    public void stopNetworkScan(Message response) {
        throw new RuntimeException("not implemented");
    }

    public void setOnPostDialCharacter(Handler h, int what, Object obj) {
        throw new RuntimeException("not implemented");
    }

    public void setMute(boolean muted) {
        throw new RuntimeException("not implemented");
    }

    public boolean getMute() {
        throw new RuntimeException("not implemented");
    }

    public void getDataCallList(Message response) {
        throw new RuntimeException("not implemented");
    }

    public void updateServiceLocation() {
        throw new RuntimeException("not implemented");
    }

    public void enableLocationUpdates() {
        throw new RuntimeException("not implemented");
    }

    public void disableLocationUpdates() {
        throw new RuntimeException("not implemented");
    }

    public boolean getDataRoamingEnabled() {
        throw new RuntimeException("not implemented");
    }

    public void setDataRoamingEnabled(boolean enable) {
        throw new RuntimeException("not implemented");
    }

    public boolean isUserDataEnabled() {
        throw new RuntimeException("not implemented");
    }

    public String getDeviceId() {
        throw new RuntimeException("not implemented");
    }

    public String getDeviceSvn() {
        throw new RuntimeException("not implemented");
    }

    public String getSubscriberId() {
        throw new RuntimeException("not implemented");
    }

    public String getGroupIdLevel1() {
        throw new RuntimeException("not implemented");
    }

    public String getGroupIdLevel2() {
        throw new RuntimeException("not implemented");
    }

    public String getEsn() {
        throw new RuntimeException("not implemented");
    }

    public String getMeid() {
        throw new RuntimeException("not implemented");
    }

    public String getImei() {
        throw new RuntimeException("not implemented");
    }

    public IccPhoneBookInterfaceManager getIccPhoneBookInterfaceManager() {
        throw new RuntimeException("not implemented");
    }

    public void activateCellBroadcastSms(int activate, Message response) {
        throw new RuntimeException("not implemented");
    }

    public void getCellBroadcastSmsConfig(Message response) {
        throw new RuntimeException("not implemented");
    }

    public void setCellBroadcastSmsConfig(int[] configValuesArray, Message response) {
        throw new RuntimeException("not implemented");
    }

    public void dump(FileDescriptor fd, PrintWriter pw, String[] args) {
        throw new RuntimeException("not implemented");
    }
}
