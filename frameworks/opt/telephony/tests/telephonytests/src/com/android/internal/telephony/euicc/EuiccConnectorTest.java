/*
 * Copyright (C) 2017 The Android Open Source Project
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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertSame;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;
import static org.mockito.Matchers.anyInt;
import static org.mockito.Matchers.anyString;
import static org.mockito.Mockito.doAnswer;
import static org.mockito.Mockito.doNothing;
import static org.mockito.Mockito.doThrow;
import static org.mockito.Mockito.when;

import android.Manifest;
import android.content.ComponentName;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.content.pm.ServiceInfo;
import android.net.Uri;
import android.os.RemoteException;
import android.os.UserHandle;
import android.os.test.TestLooper;
import android.service.euicc.EuiccService;
import android.service.euicc.IEuiccService;
import android.service.euicc.IGetEidCallback;

import androidx.test.runner.AndroidJUnit4;

import com.android.internal.telephony.TelephonyTest;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;
import org.mockito.Mockito;
import org.mockito.MockitoAnnotations;
import org.mockito.invocation.InvocationOnMock;
import org.mockito.stubbing.Answer;

import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicReference;

@RunWith(AndroidJUnit4.class)
public class EuiccConnectorTest extends TelephonyTest {
    private static final String EUICC_APP_PACKAGE_NAME = "euicc.app";

    private TestLooper mLooper;
    private EuiccConnector mConnector;
    @Mock private IEuiccService.Stub mEuiccService;

    private static final int CARD_ID = 15;

    @Before
    public void setUp() throws Exception {
        super.setUp("EuiccConnectorTest");
        MockitoAnnotations.initMocks(this);
        when(mEuiccService.queryLocalInterface(anyString())).thenReturn(mEuiccService);
        when(mEuiccService.asBinder()).thenReturn(mEuiccService);
        mLooper = new TestLooper();
    }

    @After
    public void tearDown() throws Exception {
        super.tearDown();
    }

    @Test
    public void testInitialState_unavailable_noPermission() {
        prepareEuiccApp(false /* hasPermission */, true /* requiresBindPermission */,
                true /* hasPriority */);
        mConnector = new EuiccConnector(mContext, mLooper.getLooper());
        mLooper.dispatchAll();
        assertSame(mConnector.mUnavailableState, mConnector.getCurrentState());
    }

    @Test
    public void testInitialState_unavailable_noBindPermission() {
        prepareEuiccApp(true /* hasPermission */, false /* requiresBindPermission */,
                true /* hasPriority */);
        mConnector = new EuiccConnector(mContext, mLooper.getLooper());
        mLooper.dispatchAll();
        assertSame(mConnector.mUnavailableState, mConnector.getCurrentState());
    }

    @Test
    public void testInitialState_unavailable_noPriority() {
        prepareEuiccApp(true /* hasPermission */, true /* requiresBindPermission */,
                false /* hasPriority */);
        mConnector = new EuiccConnector(mContext, mLooper.getLooper());
        mLooper.dispatchAll();
        assertSame(mConnector.mUnavailableState, mConnector.getCurrentState());
    }

    @Test
    public void testInitialState_commandRejected() {
        prepareEuiccApp(false /* hasPermission */, false /* requiresBindPermission */,
                false /* hasPriority */);
        mConnector = new EuiccConnector(mContext, mLooper.getLooper());
        final AtomicBoolean called = new AtomicBoolean(false);
        mConnector.getEid(CARD_ID, new EuiccConnector.GetEidCommandCallback() {
            @Override
            public void onGetEidComplete(String eid) {
                fail("Command should have failed");
            }

            @Override
            public void onEuiccServiceUnavailable() {
                assertTrue("Callback called twice", called.compareAndSet(false, true));
            }
        });
        mLooper.dispatchAll();
        assertTrue(called.get());
    }

    @Test
    public void testInitialState_switchCommandRejected() {
        prepareEuiccApp(false /* hasPermission */, false /* requiresBindPermission */,
                false /* hasPriority */);
        mConnector = new EuiccConnector(mContext, mLooper.getLooper());
        final AtomicBoolean called = new AtomicBoolean(false);
        mConnector.switchToSubscription(CARD_ID, "12345", true, new
                EuiccConnector.SwitchCommandCallback() {
            @Override
            public void onSwitchComplete(int result) {
                fail("Command should have failed");
            }

            @Override
            public void onEuiccServiceUnavailable() {
                assertTrue("Callback called twice", called.compareAndSet(false, true));
            }
        });
        mLooper.dispatchAll();
        assertTrue(called.get());
    }

    @Test
    public void testInitialState_available() {
        prepareEuiccApp(true /* hasPermission */, true /* requiresBindPermission */,
                true /* hasPriority */);
        mConnector = new EuiccConnector(mContext, mLooper.getLooper());
        mLooper.dispatchAll();
        assertSame(mConnector.mAvailableState, mConnector.getCurrentState());
    }

    @Test
    public void testPackageChange_unavailableToAvailable() {
        mConnector = new EuiccConnector(mContext, mLooper.getLooper());
        mLooper.dispatchAll();
        assertSame(mConnector.mUnavailableState, mConnector.getCurrentState());

        // Now install the eUICC app.
        prepareEuiccApp(true /* hasPermission */, true /* requiresBindPermission */,
                true /* hasPriority */);
        Intent intent = new Intent(Intent.ACTION_PACKAGE_ADDED,
                Uri.parse("package://" + EUICC_APP_PACKAGE_NAME));
        intent.putExtra(Intent.EXTRA_USER_HANDLE, UserHandle.USER_CURRENT);
        mContext.sendBroadcast(intent);
        mLooper.dispatchAll();
        assertSame(mConnector.mAvailableState, mConnector.getCurrentState());
    }

    @Test
    public void testPackageChange_availableToUnavailable() {
        prepareEuiccApp(true /* hasPermission */, true /* requiresBindPermission */,
                true /* hasPriority */);
        mConnector = new EuiccConnector(mContext, mLooper.getLooper());
        mLooper.dispatchAll();
        assertSame(mConnector.mAvailableState, mConnector.getCurrentState());

        // Now update the eUICC app but remove the WRITE_EMBEDDED_SUBSCRIPTIONS permission.
        Mockito.reset(mPackageManager);
        prepareEuiccApp(false /* hasPermission */, true /* requiresBindPermission */,
                true /* hasPriority */);
        Intent intent = new Intent(Intent.ACTION_PACKAGE_ADDED,
                Uri.parse("package://" + EUICC_APP_PACKAGE_NAME));
        intent.putExtra(Intent.EXTRA_USER_HANDLE, UserHandle.USER_CURRENT);
        mContext.sendBroadcast(intent);
        mLooper.dispatchAll();
        assertSame(mConnector.mUnavailableState, mConnector.getCurrentState());
    }

    @Test
    public void testCommandDispatch_success() throws Exception {
        prepareEuiccApp(true /* hasPermission */, true /* requiresBindPermission */,
                true /* hasPriority */);
        mConnector = new EuiccConnector(mContext, mLooper.getLooper());
        doAnswer(new Answer<Void>() {
            @Override
            public Void answer(InvocationOnMock invocation) throws Exception {
                IGetEidCallback callback = invocation.getArgument(1);
                callback.onSuccess("ABCDE");
                return null;
            }
        }).when(mEuiccService).getEid(anyInt(), Mockito.<IGetEidCallback>any());
        final AtomicReference<String> eidRef = new AtomicReference<>();
        mConnector.getEid(CARD_ID, new EuiccConnector.GetEidCommandCallback() {
            @Override
            public void onGetEidComplete(String eid) {
                if (eidRef.get() != null) {
                    fail("Callback called twice");
                }
                eidRef.set(eid);
            }

            @Override
            public void onEuiccServiceUnavailable() {
                fail("Command should have succeeded");
            }
        });
        mLooper.dispatchAll();
        assertEquals("ABCDE", eidRef.get());
    }

    @Test
    public void testCommandDispatch_remoteException() throws Exception {
        prepareEuiccApp(true /* hasPermission */, true /* requiresBindPermission */,
                true /* hasPriority */);
        mConnector = new EuiccConnector(mContext, mLooper.getLooper());
        doThrow(new RemoteException("failure"))
                .when(mEuiccService).getEid(anyInt(), Mockito.<IGetEidCallback>any());
        final AtomicBoolean called = new AtomicBoolean(false);
        mConnector.getEid(CARD_ID, new EuiccConnector.GetEidCommandCallback() {
            @Override
            public void onGetEidComplete(String eid) {
                fail("Command should have failed");
            }

            @Override
            public void onEuiccServiceUnavailable() {
                assertTrue("Callback called twice", called.compareAndSet(false, true));
            }
        });
        mLooper.dispatchAll();
        assertTrue(called.get());
    }

    @Test
    public void testCommandDispatch_processDied() throws Exception {
        // Kick off the asynchronous command.
        prepareEuiccApp(true /* hasPermission */, true /* requiresBindPermission */,
                true /* hasPriority */);
        mConnector = new EuiccConnector(mContext, mLooper.getLooper());
        final AtomicBoolean called = new AtomicBoolean(false);
        mConnector.getEid(CARD_ID, new EuiccConnector.GetEidCommandCallback() {
            @Override
            public void onGetEidComplete(String eid) {
                fail("Unexpected command success callback");
            }

            @Override
            public void onEuiccServiceUnavailable() {
                assertTrue("Callback called twice", called.compareAndSet(false, true));
            }
        });
        mLooper.dispatchAll();
        assertFalse(called.get());

        // Now, pretend the remote process died.
        mConnector.onServiceDisconnected(null /* name */);
        mLooper.dispatchAll();

        // Callback should have been called.
        assertTrue(called.get());
    }

    @Test
    public void testLinger() throws Exception {
        prepareEuiccApp(true /* hasPermission */, true /* requiresBindPermission */,
                true /* hasPriority */);
        mConnector = new EuiccConnector(mContext, mLooper.getLooper());
        ArgumentCaptor<IGetEidCallback> callbackCaptor =
                ArgumentCaptor.forClass(IGetEidCallback.class);
        doNothing().when(mEuiccService).getEid(anyInt(), callbackCaptor.capture());
        mConnector.getEid(CARD_ID, new EuiccConnector.GetEidCommandCallback() {
            @Override public void onGetEidComplete(String eid) {}
            @Override public void onEuiccServiceUnavailable() {}
        });
        mLooper.dispatchAll();
        assertEquals(mConnector.mConnectedState, mConnector.getCurrentState());
        // Even after linger timeout, no change in state since command is still in flight.
        mLooper.moveTimeForward(EuiccConnector.LINGER_TIMEOUT_MILLIS);
        mLooper.dispatchAll();
        assertEquals(mConnector.mConnectedState, mConnector.getCurrentState());
        // Now complete the command - should still be connected.
        callbackCaptor.getValue().onSuccess("ABCDE");
        mLooper.dispatchAll();
        assertEquals(mConnector.mConnectedState, mConnector.getCurrentState());
        // After linger timeout, should now drop back to available state.
        mLooper.moveTimeForward(EuiccConnector.LINGER_TIMEOUT_MILLIS);
        mLooper.dispatchAll();
        assertEquals(mConnector.mAvailableState, mConnector.getCurrentState());
    }

    @Test
    public void testLinger_twoCommands() throws Exception {
        prepareEuiccApp(true /* hasPermission */, true /* requiresBindPermission */,
                true /* hasPriority */);
        mConnector = new EuiccConnector(mContext, mLooper.getLooper());
        ArgumentCaptor<IGetEidCallback> callbackCaptor =
                ArgumentCaptor.forClass(IGetEidCallback.class);
        doNothing().when(mEuiccService).getEid(anyInt(), callbackCaptor.capture());
        mConnector.getEid(CARD_ID, new EuiccConnector.GetEidCommandCallback() {
            @Override public void onGetEidComplete(String eid) {}
            @Override public void onEuiccServiceUnavailable() {}
        });
        mConnector.getEid(CARD_ID, new EuiccConnector.GetEidCommandCallback() {
            @Override public void onGetEidComplete(String eid) {}
            @Override public void onEuiccServiceUnavailable() {}
        });
        mLooper.dispatchAll();
        assertEquals(mConnector.mConnectedState, mConnector.getCurrentState());
        // Even after linger timeout, no change in state since two commands are still in flight.
        mLooper.moveTimeForward(EuiccConnector.LINGER_TIMEOUT_MILLIS);
        mLooper.dispatchAll();
        assertEquals(mConnector.mConnectedState, mConnector.getCurrentState());
        // Now complete one command - should still be connected.
        callbackCaptor.getAllValues().get(0).onSuccess("ABCDE");
        mLooper.dispatchAll();
        assertEquals(mConnector.mConnectedState, mConnector.getCurrentState());
        // Even after linger timeout, no change in state since one command is still in flight.
        mLooper.moveTimeForward(EuiccConnector.LINGER_TIMEOUT_MILLIS);
        mLooper.dispatchAll();
        assertEquals(mConnector.mConnectedState, mConnector.getCurrentState());
        // Now complete second command - should still be connected.
        callbackCaptor.getAllValues().get(1).onSuccess("ABCDE");
        mLooper.dispatchAll();
        assertEquals(mConnector.mConnectedState, mConnector.getCurrentState());
        // After linger timeout, should now drop back to available state.
        mLooper.moveTimeForward(EuiccConnector.LINGER_TIMEOUT_MILLIS);
        mLooper.dispatchAll();
        assertEquals(mConnector.mAvailableState, mConnector.getCurrentState());
    }

    private void prepareEuiccApp(
            boolean hasPermission, boolean requiresBindPermission, boolean hasPriority) {
        when(mPackageManager.checkPermission(
                Manifest.permission.WRITE_EMBEDDED_SUBSCRIPTIONS, EUICC_APP_PACKAGE_NAME))
                .thenReturn(hasPermission
                        ? PackageManager.PERMISSION_GRANTED : PackageManager.PERMISSION_DENIED);

        ServiceInfo serviceInfo =  new ServiceInfo();
        serviceInfo.packageName = EUICC_APP_PACKAGE_NAME;
        serviceInfo.name = "EuiccServiceImpl";
        if (requiresBindPermission) {
            serviceInfo.permission = Manifest.permission.BIND_EUICC_SERVICE;
        }
        IntentFilter filter = new IntentFilter();
        if (hasPriority) {
            filter.setPriority(100);
        }
        mContextFixture.addService(
                EuiccService.EUICC_SERVICE_INTERFACE,
                new ComponentName(EUICC_APP_PACKAGE_NAME, "EuiccServiceImpl"),
                EUICC_APP_PACKAGE_NAME,
                mEuiccService,
                serviceInfo,
                filter);
    }
}
