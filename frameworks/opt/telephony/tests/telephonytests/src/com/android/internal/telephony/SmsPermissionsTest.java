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
package com.android.internal.telephony;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import android.Manifest;
import android.app.AppOpsManager;
import android.content.Context;
import android.os.Binder;
import android.os.Handler;
import android.os.HandlerThread;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.Mockito;
import org.mockito.MockitoAnnotations;

import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

public class SmsPermissionsTest {
    private static final String PACKAGE = "com.example.package";
    private static final String MESSAGE = "msg";

    private HandlerThread mHandlerThread;

    @Mock
    private Phone mMockPhone;
    @Mock
    private Context mMockContext;
    @Mock
    private AppOpsManager mMockAppOps;

    private SmsPermissions mSmsPermissionsTest;

    private boolean mCallerHasCarrierPrivileges;

    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        mHandlerThread = new HandlerThread("IccSmsInterfaceManagerTest");
        mHandlerThread.start();
        final CountDownLatch initialized = new CountDownLatch(1);
        new Handler(mHandlerThread.getLooper()).post(() -> {
            mSmsPermissionsTest = new SmsPermissions(
                    mMockPhone, mMockContext, mMockAppOps) {
                @Override
                public void enforceCallerIsImsAppOrCarrierApp(String message) {
                    if (!mCallerHasCarrierPrivileges) {
                        throw new SecurityException(message);
                    }
                }
            };
            initialized.countDown();
        });
        // Wait for object to initialize.
        if (!initialized.await(30, TimeUnit.SECONDS)) {
            fail("Could not initialize IccSmsInterfaceManager");
        }
    }

    @After
    public void tearDown() throws Exception {
        mHandlerThread.quit();
    }

    @Test
    public void testCheckCallingSendTextPermissions_persist_grant() {
        assertTrue(mSmsPermissionsTest.checkCallingCanSendText(
                true /* persistMessageForNonDefaultSmsApp */, PACKAGE, MESSAGE));
    }

    @Test
    public void testCheckCallingSendTextPermissions_persist_noGrant() {
        Mockito.doThrow(new SecurityException(MESSAGE)).when(mMockContext)
                .enforceCallingPermission(Manifest.permission.SEND_SMS, MESSAGE);
        try {
            mSmsPermissionsTest.checkCallingCanSendText(
                    true /* persistMessageForNonDefaultSmsApp */, PACKAGE, MESSAGE);
            fail();
        } catch (SecurityException e) {
            // expected
        }
    }

    @Test
    public void testCheckCallingSendTextPermissions_persist_noAppOps() {
        Mockito.when(mMockAppOps.noteOp(
                AppOpsManager.OP_SEND_SMS, Binder.getCallingUid(), PACKAGE))
                .thenReturn(AppOpsManager.MODE_ERRORED);
        assertFalse(mSmsPermissionsTest.checkCallingCanSendText(
                true /* persistMessageForNonDefaultSmsApp */, PACKAGE, MESSAGE));
    }

    @Test
    public void testCheckCallingSendTextPermissions_noPersist_grantViaCarrierApp() {
        mCallerHasCarrierPrivileges = true;
        // Other permissions shouldn't matter.
        Mockito.doThrow(new SecurityException(MESSAGE)).when(mMockContext)
                .enforceCallingPermission(Manifest.permission.MODIFY_PHONE_STATE, MESSAGE);
        Mockito.doThrow(new SecurityException(MESSAGE)).when(mMockContext)
                .enforceCallingPermission(Manifest.permission.SEND_SMS, MESSAGE);
        Mockito.when(mMockAppOps.noteOp(
                AppOpsManager.OP_SEND_SMS, Binder.getCallingUid(), PACKAGE))
                .thenReturn(AppOpsManager.MODE_ERRORED);

        assertTrue(mSmsPermissionsTest.checkCallingCanSendText(
                false /* persistMessageForNonDefaultSmsApp */, PACKAGE, MESSAGE));
    }

    @Test
    public void testCheckCallingSendTextPermissions_noPersist_grantViaModifyAndSend() {
        assertTrue(mSmsPermissionsTest.checkCallingCanSendText(
                false /* persistMessageForNonDefaultSmsApp */, PACKAGE, MESSAGE));
    }

    @Test
    public void testCheckCallingSendTextPermissions_noPersist_noModify() {
        Mockito.doThrow(new SecurityException(MESSAGE)).when(mMockContext)
                .enforceCallingPermission(Manifest.permission.MODIFY_PHONE_STATE, MESSAGE);
        try {
            mSmsPermissionsTest.checkCallingCanSendText(
                    false /* persistMessageForNonDefaultSmsApp */, PACKAGE, MESSAGE);
            fail();
        } catch (SecurityException e) {
            // expected
        }
    }

    @Test
    public void testCheckCallingSendTextPermissions_noPersist_noSendSmsPermission() {
        Mockito.doThrow(new SecurityException(MESSAGE)).when(mMockContext)
                .enforceCallingPermission(Manifest.permission.SEND_SMS, MESSAGE);
        try {
            mSmsPermissionsTest.checkCallingCanSendText(
                    false /* persistMessageForNonDefaultSmsApp */, PACKAGE, MESSAGE);
            fail();
        } catch (SecurityException e) {
            // expected
        }
    }

    @Test
    public void testCheckCallingSendTextPermissions_noPersist_noAppOps() {
        Mockito.when(mMockAppOps.noteOp(
                AppOpsManager.OP_SEND_SMS, Binder.getCallingUid(), PACKAGE))
                .thenReturn(AppOpsManager.MODE_ERRORED);
        assertFalse(mSmsPermissionsTest.checkCallingCanSendText(
                false /* persistMessageForNonDefaultSmsApp */, PACKAGE, MESSAGE));
    }
}
