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

import static org.mockito.Mockito.any;
import static org.mockito.Mockito.anyInt;
import static org.mockito.Mockito.doAnswer;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.eq;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.spy;
import static org.mockito.Mockito.when;

import android.app.AlarmManager;
import android.app.AppOpsManager;
import android.app.DownloadManager;
import android.app.NotificationManager;
import android.app.usage.UsageStatsManager;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.ContentProvider;
import android.content.ContentResolver;
import android.content.ContentValues;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.content.SharedPreferences;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.pm.ResolveInfo;
import android.content.pm.ServiceInfo;
import android.content.res.AssetManager;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.database.Cursor;
import android.database.MatrixCursor;
import android.net.ConnectivityManager;
import android.net.Uri;
import android.net.wifi.WifiManager;
import android.os.BatteryManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.IInterface;
import android.os.PersistableBundle;
import android.os.UserHandle;
import android.os.UserManager;
import android.preference.PreferenceManager;
import android.provider.Settings;
import android.provider.Telephony.ServiceStateTable;
import android.telecom.TelecomManager;
import android.telephony.CarrierConfigManager;
import android.telephony.SubscriptionManager;
import android.telephony.TelephonyManager;
import android.telephony.euicc.EuiccManager;
import android.test.mock.MockContentProvider;
import android.test.mock.MockContentResolver;
import android.test.mock.MockContext;
import android.util.Log;

import com.google.common.collect.ArrayListMultimap;
import com.google.common.collect.Multimap;

import org.mockito.MockitoAnnotations;
import org.mockito.invocation.InvocationOnMock;
import org.mockito.stubbing.Answer;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Locale;
import java.util.Map;

/**
 * Controls a test {@link Context} as would be provided by the Android framework to an
 * {@code Activity}, {@code Service} or other system-instantiated component.
 *
 * Contains Fake<Component> classes like FakeContext for components that require complex and
 * reusable stubbing. Others can be mocked using Mockito functions in tests or constructor/public
 * methods of this class.
 */
public class ContextFixture implements TestFixture<Context> {
    private static final String TAG = "ContextFixture";
    public static final String PERMISSION_ENABLE_ALL = "android.permission.STUB_PERMISSION";

    public class FakeContentProvider extends MockContentProvider {
        private String[] mColumns = {"name", "value"};
        private HashMap<String, String> mKeyValuePairs = new HashMap<String, String>();
        private int mNumKeyValuePairs = 0;

        @Override
        public int delete(Uri uri, String selection, String[] selectionArgs) {
            return 0;
        }

        @Override
        public Uri insert(Uri uri, ContentValues values) {
            Uri newUri = null;
            if (values != null) {
                mKeyValuePairs.put(values.getAsString("name"), values.getAsString("value"));
                mNumKeyValuePairs++;
                newUri = Uri.withAppendedPath(uri, "" + mNumKeyValuePairs);
            }
            logd("insert called, new mNumKeyValuePairs: " + mNumKeyValuePairs + " uri: " + uri +
                    " newUri: " + newUri);
            return newUri;
        }

        @Override
        public Cursor query(Uri uri, String[] projection, String selection, String[] selectionArgs,
                            String sortOrder) {
            //assuming query will always be of the form 'name = ?'
            logd("query called, mNumKeyValuePairs: " + mNumKeyValuePairs + " uri: " + uri);
            if (mKeyValuePairs.containsKey(selectionArgs[0])) {
                MatrixCursor cursor = new MatrixCursor(projection);
                cursor.addRow(new String[]{mKeyValuePairs.get(selectionArgs[0])});
                return cursor;
            }
            return null;
        }

        @Override
        public Bundle call(String method, String request, Bundle args) {
            logd("call called, mNumKeyValuePairs: " + mNumKeyValuePairs + " method: " + method +
                    " request: " + request);
            switch(method) {
                case Settings.CALL_METHOD_GET_GLOBAL:
                case Settings.CALL_METHOD_GET_SECURE:
                case Settings.CALL_METHOD_GET_SYSTEM:
                    if (mKeyValuePairs.containsKey(request)) {
                        Bundle b = new Bundle(1);
                        b.putCharSequence("value", mKeyValuePairs.get(request));
                        logd("returning value pair: " + mKeyValuePairs.get(request) + " for " +
                                request);
                        return b;
                    }
                    break;
                case Settings.CALL_METHOD_PUT_GLOBAL:
                case Settings.CALL_METHOD_PUT_SECURE:
                case Settings.CALL_METHOD_PUT_SYSTEM:
                    logd("adding key-value pair: " + request + "-" + (String)args.get("value"));
                    mKeyValuePairs.put(request, (String)args.get("value"));
                    mNumKeyValuePairs++;
                    break;
            }
            return null;
        }
    }

    private final HashMap<String, Object> mSystemServices = new HashMap<String, Object>();

    public void setSystemService(String name, Object service) {
        synchronized (mSystemServices) {
            mSystemServices.put(name, service);
        }
    }

    public class FakeContext extends MockContext {
        @Override
        public PackageManager getPackageManager() {
            return mPackageManager;
        }

        @Override
        public boolean bindService(
                Intent serviceIntent,
                ServiceConnection connection,
                int flags) {
            if (mServiceByServiceConnection.containsKey(connection)) {
                throw new RuntimeException("ServiceConnection already bound: " + connection);
            }
            IInterface service = mServiceByComponentName.get(serviceIntent.getComponent());
            if (service == null) {
                service = mServiceByPackageName.get(serviceIntent.getPackage());
            }
            if (service == null) {
                throw new RuntimeException(
                        String.format("ServiceConnection not found for component: %s, package: %s",
                                serviceIntent.getComponent(), serviceIntent.getPackage()));
            }
            mServiceByServiceConnection.put(connection, service);
            connection.onServiceConnected(serviceIntent.getComponent(), service.asBinder());
            return true;
        }

        @Override
        public void unbindService(
                ServiceConnection connection) {
            IInterface service = mServiceByServiceConnection.remove(connection);
            if (service == null) {
                throw new RuntimeException("ServiceConnection not found: " + connection);
            }
            connection.onServiceDisconnected(mComponentNameByService.get(service));
        }

        @Override
        public Object getSystemService(String name) {
            synchronized (mSystemServices) {
                Object service = mSystemServices.get(name);
                if (service != null) return service;
            }
            switch (name) {
                case Context.TELEPHONY_SERVICE:
                    return mTelephonyManager;
                case Context.APP_OPS_SERVICE:
                    return mAppOpsManager;
                case Context.NOTIFICATION_SERVICE:
                    return mNotificationManager;
                case Context.USER_SERVICE:
                    return mUserManager;
                case Context.CARRIER_CONFIG_SERVICE:
                    return mCarrierConfigManager;
                case Context.TELEPHONY_SUBSCRIPTION_SERVICE:
                    return mSubscriptionManager;
                case Context.WIFI_SERVICE:
                    return mWifiManager;
                case Context.ALARM_SERVICE:
                    return mAlarmManager;
                case Context.CONNECTIVITY_SERVICE:
                    return mConnectivityManager;
                case Context.USAGE_STATS_SERVICE:
                    return mUsageStatManager;
                case Context.BATTERY_SERVICE:
                    return mBatteryManager;
                case Context.EUICC_SERVICE:
                    return mEuiccManager;
                case Context.TELECOM_SERVICE:
                    return mTelecomManager;
                case Context.DOWNLOAD_SERVICE:
                    return mDownloadManager;
                case Context.DISPLAY_SERVICE:
                case Context.POWER_SERVICE:
                    // PowerManager and DisplayManager are final classes so cannot be mocked,
                    // return real services.
                    return TestApplication.getAppContext().getSystemService(name);
                default:
                    return null;
            }
        }

        @Override
        public String getSystemServiceName(Class<?> serviceClass) {
            if (serviceClass == SubscriptionManager.class) {
                return Context.TELEPHONY_SUBSCRIPTION_SERVICE;
            } else if (serviceClass == AppOpsManager.class) {
                return Context.APP_OPS_SERVICE;
            } else if (serviceClass == TelecomManager.class) {
                return Context.TELECOM_SERVICE;
            }
            return super.getSystemServiceName(serviceClass);
        }

        @Override
        public int getUserId() {
            return 0;
        }

        @Override
        public AssetManager getAssets() {
            return mAssetManager;
        }

        @Override
        public Resources getResources() {
            return mResources;
        }

        @Override
        public ApplicationInfo getApplicationInfo() {
            return mApplicationInfo;
        }

        @Override
        public String getOpPackageName() {
            return "com.android.internal.telephony";
        }

        @Override
        public ContentResolver getContentResolver() {
            return mContentResolver;
        }

        @Override
        public Resources.Theme getTheme() {
            return null;
        }

        @Override
        public void unregisterReceiver(BroadcastReceiver receiver) {
        }

        @Override
        public Intent registerReceiver(BroadcastReceiver receiver, IntentFilter filter) {
            return registerReceiver(receiver, filter, null, null);
        }

        @Override
        public Intent registerReceiver(BroadcastReceiver receiver, IntentFilter filter,
                String broadcastPermission, Handler scheduler) {
            return registerReceiverAsUser(receiver, null, filter, broadcastPermission, scheduler);
        }

        @Override
        public Intent registerReceiverAsUser(BroadcastReceiver receiver, UserHandle user,
                IntentFilter filter, String broadcastPermission, Handler scheduler) {
            Intent result = null;
            synchronized (mBroadcastReceiversByAction) {
                for (int i = 0 ; i < filter.countActions() ; i++) {
                    mBroadcastReceiversByAction.put(filter.getAction(i), receiver);
                    if (result == null) {
                        result = mStickyBroadcastByAction.get(filter.getAction(i));
                    }
                }
            }

            return result;
        }

        @Override
        public void sendBroadcast(Intent intent) {
            logd("sendBroadcast called for " + intent.getAction());
            synchronized (mBroadcastReceiversByAction) {
                for (BroadcastReceiver broadcastReceiver :
                        mBroadcastReceiversByAction.get(intent.getAction())) {
                    broadcastReceiver.onReceive(mContext, intent);
                }
            }
        }

        @Override
        public void sendBroadcast(Intent intent, String receiverPermission) {
            logd("sendBroadcast called for " + intent.getAction());
            sendBroadcast(intent);
        }

        @Override
        public void sendOrderedBroadcast(Intent intent, String receiverPermission) {
            logd("sendOrderedBroadcast called for " + intent.getAction());
            sendBroadcast(intent);
        }

        @Override
        public void sendOrderedBroadcast(Intent intent, String receiverPermission,
                BroadcastReceiver resultReceiver, Handler scheduler, int initialCode,
                String initialData, Bundle initialExtras) {
            sendOrderedBroadcast(intent, receiverPermission);
            if (resultReceiver != null) {
                synchronized (mOrderedBroadcastReceivers) {
                    mOrderedBroadcastReceivers.put(intent, resultReceiver);
                }
            }
        }

        @Override
        public void sendOrderedBroadcast(Intent intent, String receiverPermission, Bundle options,
                BroadcastReceiver resultReceiver, Handler scheduler, int initialCode,
                String initialData, Bundle initialExtras) {
            mLastBroadcastOptions = options;
            sendOrderedBroadcast(intent, receiverPermission, resultReceiver, scheduler,
                    initialCode, initialData, initialExtras);
        }

        @Override
        public void sendOrderedBroadcast(Intent intent, String receiverPermission, int appOp,
                BroadcastReceiver resultReceiver, Handler scheduler, int initialCode,
                String initialData, Bundle initialExtras) {
            sendOrderedBroadcast(intent, receiverPermission, resultReceiver, scheduler,
                    initialCode, initialData, initialExtras);
        }

        @Override
        public void sendBroadcastAsUser(Intent intent, UserHandle user) {
            sendBroadcast(intent);
        }

        @Override
        public void sendBroadcastAsUser(Intent intent, UserHandle user,
                                        String receiverPermission) {
            sendBroadcast(intent);
        }

        @Override
        public void sendBroadcastAsUser(Intent intent, UserHandle user,
                                        String receiverPermission, int appOp) {
            sendBroadcast(intent);
        }

        @Override
        public void sendOrderedBroadcastAsUser(Intent intent, UserHandle user,
                String receiverPermission, BroadcastReceiver resultReceiver, Handler scheduler,
                int initialCode, String initialData, Bundle initialExtras) {
            logd("sendOrderedBroadcastAsUser called for " + intent.getAction());
            sendBroadcast(intent);
            if (resultReceiver != null) {
                synchronized (mOrderedBroadcastReceivers) {
                    mOrderedBroadcastReceivers.put(intent, resultReceiver);
                }
            }
        }

        @Override
        public void sendOrderedBroadcastAsUser(Intent intent, UserHandle user,
                String receiverPermission, int appOp, BroadcastReceiver resultReceiver,
                Handler scheduler, int initialCode, String initialData, Bundle initialExtras) {
            logd("sendOrderedBroadcastAsUser called for " + intent.getAction());
            sendBroadcast(intent);
            if (resultReceiver != null) {
                synchronized (mOrderedBroadcastReceivers) {
                    mOrderedBroadcastReceivers.put(intent, resultReceiver);
                }
            }
        }

        @Override
        public void sendOrderedBroadcastAsUser(Intent intent, UserHandle user,
                String receiverPermission, int appOp, Bundle options,
                BroadcastReceiver resultReceiver, Handler scheduler, int initialCode,
                String initialData, Bundle initialExtras) {
            logd("sendOrderedBroadcastAsUser called for " + intent.getAction());
            mLastBroadcastOptions = options;
            sendBroadcast(intent);
            if (resultReceiver != null) {
                synchronized (mOrderedBroadcastReceivers) {
                    mOrderedBroadcastReceivers.put(intent, resultReceiver);
                }
            }
        }

        @Override
        public void sendStickyBroadcast(Intent intent) {
            logd("sendStickyBroadcast called for " + intent.getAction());
            synchronized (mBroadcastReceiversByAction) {
                sendBroadcast(intent);
                mStickyBroadcastByAction.put(intent.getAction(), intent);
            }
        }

        @Override
        public void sendStickyBroadcastAsUser(Intent intent, UserHandle ignored) {
            logd("sendStickyBroadcastAsUser called for " + intent.getAction());
            sendStickyBroadcast(intent);
        }

        @Override
        public Context createPackageContextAsUser(String packageName, int flags, UserHandle user)
                throws PackageManager.NameNotFoundException {
            return this;
        }

        @Override
        public void enforceCallingOrSelfPermission(String permission, String message) {
            if (mPermissionTable.contains(permission)
                    || mPermissionTable.contains(PERMISSION_ENABLE_ALL)) {
                return;
            }
            logd("requested permission: " + permission + " got denied");
            throw new SecurityException(permission + " denied: " + message);
        }

        @Override
        public void enforcePermission(String permission, int pid, int uid, String message) {
            enforceCallingOrSelfPermission(permission, message);
        }

        @Override
        public int checkCallingOrSelfPermission(String permission) {
            if (mPermissionTable.contains(permission)
                    || mPermissionTable.contains(PERMISSION_ENABLE_ALL)) {
                logd("checkCallingOrSelfPermission: " + permission + " return GRANTED");
                return PackageManager.PERMISSION_GRANTED;
            } else {
                logd("checkCallingOrSelfPermission: " + permission + " return DENIED");
                return PackageManager.PERMISSION_DENIED;
            }
        }

        @Override
        public int checkPermission(String permission, int pid, int uid) {
            return checkCallingOrSelfPermission(permission);
        }

        @Override
        public SharedPreferences getSharedPreferences(String name, int mode) {
            return mSharedPreferences;
        }

        @Override
        public String getPackageName() {
            return "com.android.internal.telephony";
        }

        @Override
        public Context getApplicationContext() {
            return null;
        }

        @Override
        public void startActivity(Intent intent) {}
    }

    private final Multimap<String, ComponentName> mComponentNamesByAction =
            ArrayListMultimap.create();
    private final Map<ComponentName, IInterface> mServiceByComponentName =
            new HashMap<ComponentName, IInterface>();
    private final Map<String, IInterface> mServiceByPackageName =
            new HashMap<String, IInterface>();
    private final Map<ComponentName, ServiceInfo> mServiceInfoByComponentName =
            new HashMap<ComponentName, ServiceInfo>();
    private final Map<ComponentName, IntentFilter> mIntentFilterByComponentName = new HashMap<>();
    private final Map<IInterface, ComponentName> mComponentNameByService =
            new HashMap<IInterface, ComponentName>();
    private final Map<ServiceConnection, IInterface> mServiceByServiceConnection =
            new HashMap<ServiceConnection, IInterface>();
    private final Multimap<String, BroadcastReceiver> mBroadcastReceiversByAction =
            ArrayListMultimap.create();
    private final HashMap<String, Intent> mStickyBroadcastByAction =
            new HashMap<String, Intent>();
    private final Multimap<Intent, BroadcastReceiver> mOrderedBroadcastReceivers =
            ArrayListMultimap.create();
    private final HashSet<String> mPermissionTable = new HashSet<>();
    private final HashSet<String> mSystemFeatures = new HashSet<>();
    private Bundle mLastBroadcastOptions;


    // The application context is the most important object this class provides to the system
    // under test.
    private final Context mContext = spy(new FakeContext());

    // We then create a spy on the application context allowing standard Mockito-style
    // when(...) logic to be used to add specific little responses where needed.

    private final Resources mResources = mock(Resources.class);
    private final ApplicationInfo mApplicationInfo = mock(ApplicationInfo.class);
    private final PackageManager mPackageManager = mock(PackageManager.class);
    private final TelephonyManager mTelephonyManager = mock(TelephonyManager.class);
    private final DownloadManager mDownloadManager = mock(DownloadManager.class);
    private final AppOpsManager mAppOpsManager = mock(AppOpsManager.class);
    private final NotificationManager mNotificationManager = mock(NotificationManager.class);
    private final UserManager mUserManager = mock(UserManager.class);
    private final CarrierConfigManager mCarrierConfigManager = mock(CarrierConfigManager.class);
    private final SubscriptionManager mSubscriptionManager = mock(SubscriptionManager.class);
    private final AlarmManager mAlarmManager = mock(AlarmManager.class);
    private final AssetManager mAssetManager = new AssetManager();
    private final ConnectivityManager mConnectivityManager = mock(ConnectivityManager.class);
    private final UsageStatsManager mUsageStatManager = null;
    private final WifiManager mWifiManager = mock(WifiManager.class);
    private final BatteryManager mBatteryManager = mock(BatteryManager.class);
    private final EuiccManager mEuiccManager = mock(EuiccManager.class);
    private final TelecomManager mTelecomManager = mock(TelecomManager.class);
    private final PackageInfo mPackageInfo = mock(PackageInfo.class);

    private final ContentProvider mContentProvider = spy(new FakeContentProvider());

    private final Configuration mConfiguration = new Configuration();
    private final SharedPreferences mSharedPreferences = PreferenceManager
            .getDefaultSharedPreferences(TestApplication.getAppContext());
    private final MockContentResolver mContentResolver = new MockContentResolver();
    private final PersistableBundle mBundle = new PersistableBundle();

    public ContextFixture() {
        MockitoAnnotations.initMocks(this);

        doAnswer(new Answer<List<ResolveInfo>>() {
            @Override
            public List<ResolveInfo> answer(InvocationOnMock invocation) throws Throwable {
                return doQueryIntentServices(
                        (Intent) invocation.getArguments()[0],
                        (Integer) invocation.getArguments()[1]);
            }
        }).when(mPackageManager).queryIntentServices((Intent) any(), anyInt());

        doAnswer(new Answer<List<ResolveInfo>>() {
            @Override
            public List<ResolveInfo> answer(InvocationOnMock invocation) throws Throwable {
                return doQueryIntentServices(
                        (Intent) invocation.getArguments()[0],
                        (Integer) invocation.getArguments()[1]);
            }
        }).when(mPackageManager).queryIntentServicesAsUser((Intent) any(), anyInt(), anyInt());

        try {
            doReturn(mPackageInfo).when(mPackageManager).getPackageInfoAsUser(any(), anyInt(),
                    anyInt());
        } catch (NameNotFoundException e) {
        }

        doAnswer((Answer<Boolean>)
                invocation -> mSystemFeatures.contains((String) invocation.getArgument(0)))
                .when(mPackageManager).hasSystemFeature(any());

        doReturn(mBundle).when(mCarrierConfigManager).getConfigForSubId(anyInt());
        //doReturn(mBundle).when(mCarrierConfigManager).getConfig(anyInt());
        doReturn(mBundle).when(mCarrierConfigManager).getConfig();

        doReturn(true).when(mEuiccManager).isEnabled();

        mConfiguration.locale = Locale.US;
        doReturn(mConfiguration).when(mResources).getConfiguration();

        mContentResolver.addProvider(Settings.AUTHORITY, mContentProvider);
        // Settings caches the provider after first get/set call, this is needed to make sure
        // Settings is using mContentProvider as the cached provider across all tests.
        Settings.Global.getInt(mContentResolver, Settings.Global.AIRPLANE_MODE_ON, 0);
        mContentResolver.addProvider(ServiceStateTable.AUTHORITY, mContentProvider);
        mPermissionTable.add(PERMISSION_ENABLE_ALL);
    }

    @Override
    public Context getTestDouble() {
        return mContext;
    }

    public void putResource(int id, final String value) {
        when(mResources.getText(eq(id))).thenReturn(value);
        when(mResources.getString(eq(id))).thenReturn(value);
        when(mResources.getString(eq(id), any())).thenAnswer(new Answer<String>() {
            @Override
            public String answer(InvocationOnMock invocation) {
                Object[] args = invocation.getArguments();
                return String.format(value, Arrays.copyOfRange(args, 1, args.length));
            }
        });
    }

    public void putBooleanResource(int id, boolean value) {
        when(mResources.getBoolean(eq(id))).thenReturn(value);
    }

    public void putStringArrayResource(int id, String[] values) {
        doReturn(values).when(mResources).getStringArray(eq(id));
    }

    public void putIntArrayResource(int id, int[] values) {
        doReturn(values).when(mResources).getIntArray(eq(id));
    }

    public void putIntResource(int id, int value) {
        doReturn(value).when(mResources).getInteger(eq(id));
    }

    public PersistableBundle getCarrierConfigBundle() {
        return mBundle;
    }

    public void addService(String action, ComponentName name, String packageName,
                           IInterface service, ServiceInfo serviceInfo) {
        addService(action, name, packageName, service, serviceInfo, null /* filter */);
    }

    public void addService(String action, ComponentName name, String packageName,
            IInterface service, ServiceInfo serviceInfo, IntentFilter filter) {
        mComponentNamesByAction.put(action, name);
        mServiceInfoByComponentName.put(name, serviceInfo);
        mIntentFilterByComponentName.put(name, filter);
        mServiceByComponentName.put(name, service);
        mServiceByPackageName.put(packageName, service);
        mComponentNameByService.put(service, name);
    }

    private List<ResolveInfo> doQueryIntentServices(Intent intent, int flags) {
        List<ResolveInfo> result = new ArrayList<ResolveInfo>();
        for (ComponentName componentName : mComponentNamesByAction.get(intent.getAction())) {
            ResolveInfo resolveInfo = new ResolveInfo();
            resolveInfo.serviceInfo = mServiceInfoByComponentName.get(componentName);
            resolveInfo.filter = mIntentFilterByComponentName.get(componentName);
            result.add(resolveInfo);
        }
        return result;
    }

    public void sendBroadcastToOrderedBroadcastReceivers() {
        synchronized (mOrderedBroadcastReceivers) {
            // having a map separate from mOrderedBroadcastReceivers is helpful here as onReceive()
            // call within the loop may lead to sendOrderedBroadcast() which can add to
            // mOrderedBroadcastReceivers
            Collection<Map.Entry<Intent, BroadcastReceiver>> map =
                    mOrderedBroadcastReceivers.entries();
            for (Map.Entry<Intent, BroadcastReceiver> entry : map) {
                entry.getValue().onReceive(mContext, entry.getKey());
                mOrderedBroadcastReceivers.remove(entry.getKey(), entry.getValue());
            }
        }
    }

    public void addCallingOrSelfPermission(String permission) {
        synchronized (mPermissionTable) {
            if (mPermissionTable != null && permission != null) {
                mPermissionTable.remove(PERMISSION_ENABLE_ALL);
                mPermissionTable.add(permission);
            }
        }
    }

    public void removeCallingOrSelfPermission(String permission) {
        synchronized (mPermissionTable) {
            if (mPermissionTable != null && permission != null) {
                mPermissionTable.remove(permission);
            }
        }
    }

    public void addSystemFeature(String feature) {
        mSystemFeatures.add(feature);
    }

    public Bundle getLastBroadcastOptions() {
        return mLastBroadcastOptions;
    }

    private static void logd(String s) {
        Log.d(TAG, s);
    }
}
