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

import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.fail;
import static org.mockito.ArgumentMatchers.anyString;
import static org.mockito.Matchers.anyBoolean;
import static org.mockito.Matchers.nullable;
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.anyInt;
import static org.mockito.Mockito.doAnswer;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.eq;

import android.app.ActivityManager;
import android.app.IActivityManager;
import android.content.ContentResolver;
import android.content.Context;
import android.content.IIntentSender;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.net.ConnectivityManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.IDeviceIdleController;
import android.os.Looper;
import android.os.RegistrantList;
import android.os.ServiceManager;
import android.provider.BlockedNumberContract;
import android.provider.DeviceConfig;
import android.provider.Settings;
import android.telephony.AccessNetworkConstants;
import android.telephony.NetworkRegistrationInfo;
import android.telephony.ServiceState;
import android.telephony.SubscriptionManager;
import android.telephony.TelephonyManager;
import android.telephony.emergency.EmergencyNumber;
import android.telephony.euicc.EuiccManager;
import android.telephony.ims.ImsCallProfile;
import android.test.mock.MockContentProvider;
import android.test.mock.MockContentResolver;
import android.util.Log;
import android.util.Singleton;

import com.android.ims.ImsCall;
import com.android.ims.ImsEcbm;
import com.android.ims.ImsManager;
import com.android.internal.telephony.cdma.CdmaSubscriptionSourceManager;
import com.android.internal.telephony.cdma.EriManager;
import com.android.internal.telephony.dataconnection.DataEnabledOverride;
import com.android.internal.telephony.dataconnection.DataEnabledSettings;
import com.android.internal.telephony.dataconnection.DcTracker;
import com.android.internal.telephony.dataconnection.TransportManager;
import com.android.internal.telephony.emergency.EmergencyNumberTracker;
import com.android.internal.telephony.imsphone.ImsExternalCallTracker;
import com.android.internal.telephony.imsphone.ImsPhone;
import com.android.internal.telephony.imsphone.ImsPhoneCallTracker;
import com.android.internal.telephony.test.SimulatedCommands;
import com.android.internal.telephony.test.SimulatedCommandsVerifier;
import com.android.internal.telephony.uicc.IccCardStatus;
import com.android.internal.telephony.uicc.IccRecords;
import com.android.internal.telephony.uicc.IsimUiccRecords;
import com.android.internal.telephony.uicc.RuimRecords;
import com.android.internal.telephony.uicc.SIMRecords;
import com.android.internal.telephony.uicc.UiccCard;
import com.android.internal.telephony.uicc.UiccCardApplication;
import com.android.internal.telephony.uicc.UiccController;
import com.android.internal.telephony.uicc.UiccProfile;

import org.mockito.Mock;
import org.mockito.MockitoAnnotations;
import org.mockito.invocation.InvocationOnMock;
import org.mockito.stubbing.Answer;

import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Set;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

public abstract class TelephonyTest {
    protected static String TAG;

    private static final int MAX_INIT_WAIT_MS = 30000; // 30 seconds

    private static final EmergencyNumber SAMPLE_EMERGENCY_NUMBER =
            new EmergencyNumber("911", "us", "30",
                    EmergencyNumber.EMERGENCY_SERVICE_CATEGORY_UNSPECIFIED,
            new ArrayList<String>(), EmergencyNumber.EMERGENCY_NUMBER_SOURCE_NETWORK_SIGNALING,
            EmergencyNumber.EMERGENCY_CALL_ROUTING_NORMAL);

    @Mock
    protected GsmCdmaPhone mPhone;
    @Mock
    protected ImsPhone mImsPhone;
    @Mock
    protected ServiceStateTracker mSST;
    @Mock
    protected EmergencyNumberTracker mEmergencyNumberTracker;
    @Mock
    protected GsmCdmaCallTracker mCT;
    @Mock
    protected ImsPhoneCallTracker mImsCT;
    @Mock
    protected UiccController mUiccController;
    @Mock
    protected UiccProfile mUiccProfile;
    @Mock
    protected CallManager mCallManager;
    @Mock
    protected PhoneNotifier mNotifier;
    @Mock
    protected TelephonyComponentFactory mTelephonyComponentFactory;
    @Mock
    protected CdmaSubscriptionSourceManager mCdmaSSM;
    @Mock
    protected RegistrantList mRegistrantList;
    @Mock
    protected IccPhoneBookInterfaceManager mIccPhoneBookIntManager;
    @Mock
    protected ImsManager mImsManager;
    @Mock
    protected DcTracker mDcTracker;
    @Mock
    protected GsmCdmaCall mGsmCdmaCall;
    @Mock
    protected ImsCall mImsCall;
    @Mock
    protected ImsEcbm mImsEcbm;
    @Mock
    protected SubscriptionController mSubscriptionController;
    @Mock
    protected ServiceState mServiceState;

    protected NetworkRegistrationInfo mNetworkRegistrationInfo =
            new NetworkRegistrationInfo.Builder()
            .setAccessNetworkTechnology(TelephonyManager.NETWORK_TYPE_LTE)
            .setRegistrationState(NetworkRegistrationInfo.REGISTRATION_STATE_HOME)
            .build();
    @Mock
    protected SimulatedCommandsVerifier mSimulatedCommandsVerifier;
    @Mock
    protected IDeviceIdleController mIDeviceIdleController;
    @Mock
    protected InboundSmsHandler mInboundSmsHandler;
    @Mock
    protected WspTypeDecoder mWspTypeDecoder;
    @Mock
    protected UiccCardApplication mUiccCardApplication3gpp;
    @Mock
    protected UiccCardApplication mUiccCardApplication3gpp2;
    @Mock
    protected UiccCardApplication mUiccCardApplicationIms;
    @Mock
    protected SIMRecords mSimRecords;
    @Mock
    protected RuimRecords mRuimRecords;
    @Mock
    protected IsimUiccRecords mIsimUiccRecords;
    @Mock
    protected ProxyController mProxyController;
    @Mock
    protected Singleton<IActivityManager> mIActivityManagerSingleton;
    @Mock
    protected IActivityManager mIActivityManager;
    @Mock
    protected IIntentSender mIIntentSender;
    @Mock
    protected IBinder mIBinder;
    @Mock
    protected SmsStorageMonitor mSmsStorageMonitor;
    @Mock
    protected SmsUsageMonitor mSmsUsageMonitor;
    @Mock
    protected PackageInfo mPackageInfo;
    @Mock
    protected ApplicationInfo mApplicationInfo;
    @Mock
    protected EriManager mEriManager;
    @Mock
    protected IBinder mConnMetLoggerBinder;
    @Mock
    protected CarrierSignalAgent mCarrierSignalAgent;
    @Mock
    protected CarrierActionAgent mCarrierActionAgent;
    @Mock
    protected ImsExternalCallTracker mImsExternalCallTracker;
    @Mock
    protected AppSmsManager mAppSmsManager;
    @Mock
    protected IccSmsInterfaceManager mIccSmsInterfaceManager;
    @Mock
    protected SmsDispatchersController mSmsDispatchersController;
    @Mock
    protected DeviceStateMonitor mDeviceStateMonitor;
    @Mock
    protected TransportManager mTransportManager;
    @Mock
    protected IntentBroadcaster mIntentBroadcaster;
    @Mock
    protected NitzStateMachine mNitzStateMachine;
    @Mock
    protected RadioConfig mMockRadioConfig;
    @Mock
    protected SubscriptionInfoUpdater mSubInfoRecordUpdater;
    @Mock
    protected LocaleTracker mLocaleTracker;
    @Mock
    protected RestrictedState mRestrictedState;
    @Mock
    protected DataEnabledSettings mDataEnabledSettings;
    @Mock
    protected DataEnabledOverride mDataEnabledOverride;
    @Mock
    protected PhoneConfigurationManager mPhoneConfigurationManager;
    @Mock
    protected CellularNetworkValidator mCellularNetworkValidator;
    @Mock
    protected UiccCard mUiccCard;
    @Mock
    protected MultiSimSettingController mMultiSimSettingController;

    protected ImsCallProfile mImsCallProfile;
    protected TelephonyManager mTelephonyManager;
    protected SubscriptionManager mSubscriptionManager;
    protected EuiccManager mEuiccManager;
    protected PackageManager mPackageManager;
    protected ConnectivityManager mConnectivityManager;
    protected SimulatedCommands mSimulatedCommands;
    protected ContextFixture mContextFixture;
    protected Context mContext;
    protected FakeBlockedNumberContentProvider mFakeBlockedNumberContentProvider;
    private Object mLock = new Object();
    private boolean mReady;
    protected HashMap<String, IBinder> mServiceManagerMockedServices = new HashMap<>();
    protected Phone[] mPhones;


    protected HashMap<Integer, ImsManager> mImsManagerInstances = new HashMap<>();
    private HashMap<InstanceKey, Object> mOldInstances = new HashMap<InstanceKey, Object>();

    private LinkedList<InstanceKey> mInstanceKeys = new LinkedList<InstanceKey>();

    private class InstanceKey {
        public final Class mClass;
        public final String mInstName;
        public final Object mObj;
        InstanceKey(final Class c, final String instName, final Object obj) {
            mClass = c;
            mInstName = instName;
            mObj = obj;
        }

        @Override
        public int hashCode() {
            return (mClass.getName().hashCode() * 31 + mInstName.hashCode()) * 31;
        }

        @Override
        public boolean equals(Object obj) {
            if (obj == null || obj.getClass() != getClass()) {
                return false;
            }

            InstanceKey other = (InstanceKey) obj;
            return (other.mClass == mClass && other.mInstName.equals(mInstName)
                    && other.mObj == mObj);
        }
    }

    protected void waitUntilReady() {
        synchronized (mLock) {
            if (!mReady) {
                try {
                    mLock.wait(MAX_INIT_WAIT_MS);
                } catch (InterruptedException ie) {
                }

                if (!mReady) {
                    fail("Telephony tests failed to initialize");
                }
            }
        }
    }

    protected void setReady(boolean ready) {
        synchronized (mLock) {
            mReady = ready;
            mLock.notifyAll();
        }
    }

    protected synchronized void replaceInstance(final Class c, final String instanceName,
                                                final Object obj, final Object newValue)
            throws Exception {
        Field field = c.getDeclaredField(instanceName);
        field.setAccessible(true);

        InstanceKey key = new InstanceKey(c, instanceName, obj);
        if (!mOldInstances.containsKey(key)) {
            mOldInstances.put(key, field.get(obj));
            mInstanceKeys.add(key);
        }
        field.set(obj, newValue);
    }

    protected synchronized void restoreInstance(final Class c, final String instanceName,
                                                final Object obj) throws Exception {
        InstanceKey key = new InstanceKey(c, instanceName, obj);
        if (mOldInstances.containsKey(key)) {
            Field field = c.getDeclaredField(instanceName);
            field.setAccessible(true);
            field.set(obj, mOldInstances.get(key));
            mOldInstances.remove(key);
            mInstanceKeys.remove(key);
        }
    }

    protected synchronized void restoreInstances() throws Exception {
        Iterator<InstanceKey> it = mInstanceKeys.descendingIterator();

        while (it.hasNext()) {
            InstanceKey key = it.next();
            Field field = key.mClass.getDeclaredField(key.mInstName);
            field.setAccessible(true);
            field.set(key.mObj, mOldInstances.get(key));
        }

        mInstanceKeys.clear();
        mOldInstances.clear();
    }

    protected void setUp(String tag) throws Exception {
        TAG = tag;
        MockitoAnnotations.initMocks(this);

        mPhones = new Phone[] {mPhone};
        mImsCallProfile = new ImsCallProfile();
        mSimulatedCommands = new SimulatedCommands();
        mContextFixture = new ContextFixture();
        mContext = mContextFixture.getTestDouble();
        mFakeBlockedNumberContentProvider = new FakeBlockedNumberContentProvider();
        ((MockContentResolver)mContext.getContentResolver()).addProvider(
                BlockedNumberContract.AUTHORITY, mFakeBlockedNumberContentProvider);
        mPhone.mCi = mSimulatedCommands;
        mCT.mCi = mSimulatedCommands;
        doReturn(mUiccCard).when(mPhone).getUiccCard();
        doReturn(mUiccProfile).when(mUiccCard).getUiccProfile();

        mTelephonyManager = (TelephonyManager) mContext.getSystemService(Context.TELEPHONY_SERVICE);
        mSubscriptionManager = (SubscriptionManager) mContext.getSystemService(
                Context.TELEPHONY_SUBSCRIPTION_SERVICE);
        mEuiccManager = (EuiccManager) mContext.getSystemService(Context.EUICC_SERVICE);
        mConnectivityManager = (ConnectivityManager)
                mContext.getSystemService(Context.CONNECTIVITY_SERVICE);
        mPackageManager = mContext.getPackageManager();

        //mTelephonyComponentFactory
        doReturn(mTelephonyComponentFactory).when(mTelephonyComponentFactory).inject(anyString());
        doReturn(mSST).when(mTelephonyComponentFactory)
                .makeServiceStateTracker(nullable(GsmCdmaPhone.class),
                        nullable(CommandsInterface.class));
        doReturn(mEmergencyNumberTracker).when(mTelephonyComponentFactory)
                .makeEmergencyNumberTracker(nullable(Phone.class),
                        nullable(CommandsInterface.class));
        doReturn(getTestEmergencyNumber()).when(mEmergencyNumberTracker)
                .getEmergencyNumber(any());
        doReturn(mUiccProfile).when(mTelephonyComponentFactory)
                .makeUiccProfile(nullable(Context.class), nullable(CommandsInterface.class),
                        nullable(IccCardStatus.class), anyInt(), nullable(UiccCard.class),
                        nullable(Object.class));
        doReturn(mCT).when(mTelephonyComponentFactory)
                .makeGsmCdmaCallTracker(nullable(GsmCdmaPhone.class));
        doReturn(mIccPhoneBookIntManager).when(mTelephonyComponentFactory)
                .makeIccPhoneBookInterfaceManager(nullable(Phone.class));
        doReturn(mDcTracker).when(mTelephonyComponentFactory)
                .makeDcTracker(nullable(Phone.class), anyInt());
        doReturn(mWspTypeDecoder).when(mTelephonyComponentFactory)
                .makeWspTypeDecoder(nullable(byte[].class));
        doReturn(mImsCT).when(mTelephonyComponentFactory)
                .makeImsPhoneCallTracker(nullable(ImsPhone.class));
        doReturn(mCdmaSSM).when(mTelephonyComponentFactory)
                .getCdmaSubscriptionSourceManagerInstance(nullable(Context.class),
                        nullable(CommandsInterface.class), nullable(Handler.class),
                        anyInt(), nullable(Object.class));
        doReturn(mIDeviceIdleController).when(mTelephonyComponentFactory)
                .getIDeviceIdleController();
        doReturn(mImsExternalCallTracker).when(mTelephonyComponentFactory)
                .makeImsExternalCallTracker(nullable(ImsPhone.class));
        doReturn(mAppSmsManager).when(mTelephonyComponentFactory)
                .makeAppSmsManager(nullable(Context.class));
        doReturn(mCarrierSignalAgent).when(mTelephonyComponentFactory)
                .makeCarrierSignalAgent(nullable(Phone.class));
        doReturn(mCarrierActionAgent).when(mTelephonyComponentFactory)
                .makeCarrierActionAgent(nullable(Phone.class));
        doReturn(mDeviceStateMonitor).when(mTelephonyComponentFactory)
                .makeDeviceStateMonitor(nullable(Phone.class));
        doReturn(mTransportManager).when(mTelephonyComponentFactory)
                .makeTransportManager(nullable(Phone.class));
        doReturn(mNitzStateMachine).when(mTelephonyComponentFactory)
                .makeNitzStateMachine(nullable(GsmCdmaPhone.class));
        doReturn(mLocaleTracker).when(mTelephonyComponentFactory)
                .makeLocaleTracker(nullable(Phone.class), nullable(NitzStateMachine.class),
                        nullable(Looper.class));
        doReturn(mDataEnabledSettings).when(mTelephonyComponentFactory)
                .makeDataEnabledSettings(nullable(Phone.class));
        doReturn(mEriManager).when(mTelephonyComponentFactory)
                .makeEriManager(nullable(Phone.class), anyInt());

        //mPhone
        doReturn(mContext).when(mPhone).getContext();
        doReturn(mContext).when(mImsPhone).getContext();
        doReturn(true).when(mPhone).getUnitTestMode();
        doReturn(mUiccProfile).when(mPhone).getIccCard();
        doReturn(mServiceState).when(mPhone).getServiceState();
        doReturn(mServiceState).when(mImsPhone).getServiceState();
        doReturn(mPhone).when(mImsPhone).getDefaultPhone();
        doReturn(true).when(mPhone).isPhoneTypeGsm();
        doReturn(PhoneConstants.PHONE_TYPE_GSM).when(mPhone).getPhoneType();
        doReturn(mCT).when(mPhone).getCallTracker();
        doReturn(mSST).when(mPhone).getServiceStateTracker();
        doReturn(mEmergencyNumberTracker).when(mPhone).getEmergencyNumberTracker();
        doReturn(mCarrierSignalAgent).when(mPhone).getCarrierSignalAgent();
        doReturn(mCarrierActionAgent).when(mPhone).getCarrierActionAgent();
        doReturn(mAppSmsManager).when(mPhone).getAppSmsManager();
        doReturn(mIccSmsInterfaceManager).when(mPhone).getIccSmsInterfaceManager();
        doReturn(mTransportManager).when(mPhone).getTransportManager();
        doReturn(mDataEnabledSettings).when(mPhone).getDataEnabledSettings();
        doReturn(mDcTracker).when(mPhone).getDcTracker(anyInt());
        mIccSmsInterfaceManager.mDispatchersController = mSmsDispatchersController;

        //mUiccController
        doReturn(mUiccCardApplication3gpp).when(mUiccController).getUiccCardApplication(anyInt(),
                eq(UiccController.APP_FAM_3GPP));
        doReturn(mUiccCardApplication3gpp2).when(mUiccController).getUiccCardApplication(anyInt(),
                eq(UiccController.APP_FAM_3GPP2));
        doReturn(mUiccCardApplicationIms).when(mUiccController).getUiccCardApplication(anyInt(),
                eq(UiccController.APP_FAM_IMS));

        doAnswer(new Answer<IccRecords>() {
            public IccRecords answer(InvocationOnMock invocation) {
                switch ((Integer) invocation.getArguments()[1]) {
                    case UiccController.APP_FAM_3GPP:
                        return mSimRecords;
                    case UiccController.APP_FAM_3GPP2:
                        return mRuimRecords;
                    case UiccController.APP_FAM_IMS:
                        return mIsimUiccRecords;
                    default:
                        logd("Unrecognized family " + invocation.getArguments()[1]);
                        return null;
                }
            }
        }).when(mUiccController).getIccRecords(anyInt(), anyInt());

        //UiccCardApplication
        doReturn(mSimRecords).when(mUiccCardApplication3gpp).getIccRecords();
        doReturn(mRuimRecords).when(mUiccCardApplication3gpp2).getIccRecords();
        doReturn(mIsimUiccRecords).when(mUiccCardApplicationIms).getIccRecords();

        //mUiccProfile
        doReturn(mSimRecords).when(mUiccProfile).getIccRecords();
        doAnswer(new Answer<IccRecords>() {
            public IccRecords answer(InvocationOnMock invocation) {
                return (mPhone.isPhoneTypeGsm()) ? mSimRecords : mRuimRecords;
            }
        }).when(mUiccProfile).getIccRecords();

        //mUiccProfile
        doReturn(mUiccCardApplication3gpp).when(mUiccProfile).getApplication(
                eq(UiccController.APP_FAM_3GPP));
        doReturn(mUiccCardApplication3gpp2).when(mUiccProfile).getApplication(
                eq(UiccController.APP_FAM_3GPP2));
        doReturn(mUiccCardApplicationIms).when(mUiccProfile).getApplication(
                eq(UiccController.APP_FAM_IMS));

        //SMS
        doReturn(true).when(mSmsStorageMonitor).isStorageAvailable();
        doReturn(true).when(mSmsUsageMonitor).check(nullable(String.class), anyInt());
        doReturn(true).when(mTelephonyManager).getSmsReceiveCapableForPhone(anyInt(), anyBoolean());
        doReturn(true).when(mTelephonyManager).getSmsSendCapableForPhone(
                anyInt(), anyBoolean());

        //Misc
        doReturn(ServiceState.RIL_RADIO_TECHNOLOGY_UMTS).when(mServiceState).
                getRilDataRadioTechnology();
        doReturn(mPhone).when(mCT).getPhone();
        mImsManagerInstances.put(mPhone.getPhoneId(), mImsManager);
        doReturn(mImsEcbm).when(mImsManager).getEcbmInterface();
        doReturn(mPhone).when(mInboundSmsHandler).getPhone();
        doReturn(mImsCallProfile).when(mImsCall).getCallProfile();
        doReturn(mIBinder).when(mIIntentSender).asBinder();
        doReturn(mIIntentSender).when(mIActivityManager).getIntentSender(anyInt(),
                nullable(String.class), nullable(IBinder.class), nullable(String.class), anyInt(),
                nullable(Intent[].class), nullable(String[].class), anyInt(),
                nullable(Bundle.class), anyInt());
        doReturn(mTelephonyManager).when(mTelephonyManager).createForSubscriptionId(anyInt());
        doReturn(mServiceState).when(mSST).getServiceState();
        mSST.mSS = mServiceState;
        mSST.mRestrictedState = mRestrictedState;
        mServiceManagerMockedServices.put("connectivity_metrics_logger", mConnMetLoggerBinder);
        doReturn(new int[]{AccessNetworkConstants.TRANSPORT_TYPE_WWAN,
                AccessNetworkConstants.TRANSPORT_TYPE_WLAN})
                .when(mTransportManager).getAvailableTransports();
        doReturn(AccessNetworkConstants.TRANSPORT_TYPE_WWAN).when(mTransportManager)
                .getCurrentTransport(anyInt());
        doReturn(true).when(mDataEnabledSettings).isDataEnabled();
        doReturn(true).when(mDataEnabledSettings).isDataEnabled(anyInt());
        doReturn(true).when(mDataEnabledSettings).isInternalDataEnabled();
        doReturn(mNetworkRegistrationInfo).when(mServiceState).getNetworkRegistrationInfo(
                anyInt(), anyInt());
        doReturn(new HalVersion(1, 4)).when(mPhone).getHalVersion();

        //SIM
        doReturn(1).when(mTelephonyManager).getSimCount();
        doReturn(1).when(mTelephonyManager).getPhoneCount();

        //Data
        //Initial state is: userData enabled, provisioned.
        ContentResolver resolver = mContext.getContentResolver();
        Settings.Global.putInt(resolver, Settings.Global.MOBILE_DATA, 1);
        Settings.Global.putInt(resolver, Settings.Global.DEVICE_PROVISIONED, 1);
        Settings.Global.putInt(resolver,
                Settings.Global.DEVICE_PROVISIONING_MOBILE_DATA_ENABLED, 1);

        // CellularNetworkValidator
        doReturn(SubscriptionManager.INVALID_PHONE_INDEX)
                .when(mCellularNetworkValidator).getSubIdInValidation();
        doReturn(true).when(mCellularNetworkValidator).isValidationFeatureSupported();

        //Use reflection to mock singletons
        replaceInstance(CallManager.class, "INSTANCE", null, mCallManager);
        replaceInstance(TelephonyComponentFactory.class, "sInstance", null,
                mTelephonyComponentFactory);
        replaceInstance(UiccController.class, "mInstance", null, mUiccController);
        replaceInstance(CdmaSubscriptionSourceManager.class, "sInstance", null, mCdmaSSM);
        replaceInstance(ImsManager.class, "sImsManagerInstances", null, mImsManagerInstances);
        replaceInstance(SubscriptionController.class, "sInstance", null, mSubscriptionController);
        replaceInstance(ProxyController.class, "sProxyController", null, mProxyController);
        replaceInstance(ActivityManager.class, "IActivityManagerSingleton", null,
                mIActivityManagerSingleton);
        replaceInstance(CdmaSubscriptionSourceManager.class,
                "mCdmaSubscriptionSourceChangedRegistrants", mCdmaSSM, mRegistrantList);
        replaceInstance(SimulatedCommandsVerifier.class, "sInstance", null,
                mSimulatedCommandsVerifier);
        replaceInstance(Singleton.class, "mInstance", mIActivityManagerSingleton,
                mIActivityManager);
        replaceInstance(ServiceManager.class, "sCache", null, mServiceManagerMockedServices);
        replaceInstance(IntentBroadcaster.class, "sIntentBroadcaster", null, mIntentBroadcaster);
        replaceInstance(TelephonyManager.class, "sInstance", null,
                mContext.getSystemService(Context.TELEPHONY_SERVICE));
        replaceInstance(PhoneFactory.class, "sMadeDefaults", null, true);
        replaceInstance(PhoneFactory.class, "sPhone", null, mPhone);
        replaceInstance(PhoneFactory.class, "sPhones", null, mPhones);
        replaceInstance(PhoneFactory.class, "sSubInfoRecordUpdater", null, mSubInfoRecordUpdater);
        replaceInstance(RadioConfig.class, "sRadioConfig", null, mMockRadioConfig);
        replaceInstance(PhoneConfigurationManager.class, "sInstance", null,
                mPhoneConfigurationManager);
        replaceInstance(CellularNetworkValidator.class, "sInstance", null,
                mCellularNetworkValidator);
        replaceInstance(MultiSimSettingController.class, "sInstance", null,
                mMultiSimSettingController);
        replaceInstance(SubscriptionInfoUpdater.class, "sIsSubInfoInitialized", null, true);

        assertNotNull("Failed to set up SubscriptionController singleton",
                SubscriptionController.getInstance());
        setReady(false);
    }

    protected void tearDown() throws Exception {

        mSimulatedCommands.dispose();

        SharedPreferences sharedPreferences = mContext.getSharedPreferences((String) null, 0);
        sharedPreferences.edit().clear().commit();

        restoreInstances();
    }

    protected static void logd(String s) {
        Log.d(TAG, s);
    }

    public static class FakeBlockedNumberContentProvider extends MockContentProvider {
        public Set<String> mBlockedNumbers = new HashSet<>();
        public int mNumEmergencyContactNotifications = 0;

        @Override
        public Bundle call(String method, String arg, Bundle extras) {
            switch (method) {
                case BlockedNumberContract.SystemContract.METHOD_SHOULD_SYSTEM_BLOCK_NUMBER:
                    Bundle bundle = new Bundle();
                    int blockStatus = mBlockedNumbers.contains(arg)
                            ? BlockedNumberContract.STATUS_BLOCKED_IN_LIST
                            : BlockedNumberContract.STATUS_NOT_BLOCKED;
                    bundle.putInt(BlockedNumberContract.RES_BLOCK_STATUS, blockStatus);
                    return bundle;
                case BlockedNumberContract.SystemContract.METHOD_NOTIFY_EMERGENCY_CONTACT:
                    mNumEmergencyContactNotifications++;
                    return new Bundle();
                default:
                    fail("Method not expected: " + method);
            }
            return null;
        }
    }

    public static class FakeSettingsConfigProvider extends MockContentProvider {
        private static final String PROPERTY_DEVICE_IDENTIFIER_ACCESS_RESTRICTIONS_DISABLED =
                DeviceConfig.NAMESPACE_PRIVACY + "/"
                        + "device_identifier_access_restrictions_disabled";

        @Override
        public Bundle call(String method, String arg, Bundle extras) {
            switch (method) {
                case Settings.CALL_METHOD_GET_CONFIG: {
                    switch (arg) {
                        case PROPERTY_DEVICE_IDENTIFIER_ACCESS_RESTRICTIONS_DISABLED: {
                            Bundle bundle = new Bundle();
                            bundle.putString(
                                    PROPERTY_DEVICE_IDENTIFIER_ACCESS_RESTRICTIONS_DISABLED,
                                    "0");
                            return bundle;
                        }
                        default: {
                            fail("arg not expected: " + arg);
                        }
                    }
                    break;
                }
                default:
                    fail("Method not expected: " + method);
            }
            return null;
        }
    }

    protected void setupMockPackagePermissionChecks() throws Exception {
        doReturn(new String[]{TAG}).when(mPackageManager).getPackagesForUid(anyInt());
        doReturn(mPackageInfo).when(mPackageManager).getPackageInfo(eq(TAG), anyInt());
        doReturn(mPackageInfo).when(mPackageManager).getPackageInfoAsUser(
                eq(TAG), anyInt(), anyInt());
    }

    protected void setupMocksForTelephonyPermissions() throws Exception {
        // If the calling package does not meet the new requirements for device identifier access
        // TelephonyPermissions will query the PackageManager for the ApplicationInfo of the package
        // to determine the target SDK. For apps targeting Q a SecurityException is thrown
        // regardless of if the package satisfies the previous requirements for device ID access.
        mApplicationInfo.targetSdkVersion = Build.VERSION_CODES.Q;
        doReturn(mApplicationInfo).when(mPackageManager).getApplicationInfoAsUser(eq(TAG), anyInt(),
                anyInt());

        // TelephonyPermissions queries DeviceConfig to determine if the identifier access
        // restrictions should be enabled; this results in a NPE when DeviceConfig uses
        // Activity.currentActivity.getContentResolver as the resolver for Settings.Config.getString
        // since the IContentProvider in the NameValueCache's provider holder is null.
        Class c = Class.forName("android.provider.Settings$Config");
        Field field = c.getDeclaredField("sNameValueCache");
        field.setAccessible(true);
        Object cache = field.get(null);

        c = Class.forName("android.provider.Settings$NameValueCache");
        field = c.getDeclaredField("mProviderHolder");
        field.setAccessible(true);
        Object providerHolder = field.get(cache);

        FakeSettingsConfigProvider fakeSettingsProvider = new FakeSettingsConfigProvider();
        field = MockContentProvider.class.getDeclaredField("mIContentProvider");
        field.setAccessible(true);
        Object iContentProvider = field.get(fakeSettingsProvider);

        replaceInstance(Class.forName("android.provider.Settings$ContentProviderHolder"),
                "mContentProvider", providerHolder, iContentProvider);
    }

    protected final void waitForHandlerAction(Handler h, long timeoutMillis) {
        final CountDownLatch lock = new CountDownLatch(1);
        h.post(lock::countDown);
        while (lock.getCount() > 0) {
            try {
                lock.await(timeoutMillis, TimeUnit.MILLISECONDS);
            } catch (InterruptedException e) {
                // do nothing
            }
        }
    }

    protected final void waitForHandlerActionDelayed(Handler h, long timeoutMillis, long delayMs) {
        final CountDownLatch lock = new CountDownLatch(1);
        h.postDelayed(lock::countDown, delayMs);
        while (lock.getCount() > 0) {
            try {
                lock.await(timeoutMillis, TimeUnit.MILLISECONDS);
            } catch (InterruptedException e) {
                // do nothing
            }
        }
    }

    protected final EmergencyNumber getTestEmergencyNumber() {
        return SAMPLE_EMERGENCY_NUMBER;
    }

    public static Object invokeMethod(
            Object instance, String methodName, Class<?>[] parameterClasses, Object[] parameters) {
        try {
            Method method = instance.getClass().getDeclaredMethod(methodName, parameterClasses);
            method.setAccessible(true);
            return method.invoke(instance, parameters);
        } catch (NoSuchMethodException | IllegalAccessException | InvocationTargetException e) {
            fail(instance.getClass() + " " + methodName + " " + e.getClass().getName());
        }
        return null;
    }
}
