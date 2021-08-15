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

package com.android.server.wifi;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;

import android.os.Binder;

import androidx.test.filters.SmallTest;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

/**
 * Unit tests for {@link com.android.server.wifi.BinderUtil}.
 */
@SmallTest
public class BinderUtilTest {
    static final int FAKE_UID = 30000000;

    private long mToken;

    /**
     * Sets up the test harness before running a test.
     */
    @Before
    public void setUp() {
        mToken = Binder.clearCallingIdentity();
    }

    /**
     * Cleans up the test harness after running a test.
     */
    @After
    public void cleanUp() {
        Binder.restoreCallingIdentity(mToken);
    }

    /**
     * Test using {@link BinderUtil.setUid} to set and restore the Binder uid.
     */
    @Test
    public void setUid() {
        final int pid = Binder.getCallingPid();
        final int uid = Binder.getCallingUid();
        assertFalse(uid == FAKE_UID);

        // Verify that setUid() can be used to fake the Binder uid without affecting the pid.
        BinderUtil.setUid(FAKE_UID);
        assertEquals(pid, Binder.getCallingPid());
        assertEquals(FAKE_UID, Binder.getCallingUid());

        // Verify that setUid() can be used to restore the original Binder uid without affecting the
        // pid.
        BinderUtil.setUid(uid);
        assertEquals(pid, Binder.getCallingPid());
        assertEquals(uid, Binder.getCallingUid());
    }

    /**
     * Test using {@link BinderUtil.setUid} to set the Binder uid and
     * {@link Binder.restoreCallingIdentity} to restore it.
     */
    @Test
    public void setUidAndRestoreCallingIdentity() {
        final int pid = Binder.getCallingPid();
        final int uid = Binder.getCallingUid();
        assertFalse(uid == FAKE_UID);

        // Verify that setUid() can be used to fake the Binder uid without affecting the pid.
        BinderUtil.setUid(FAKE_UID);
        assertEquals(pid, Binder.getCallingPid());
        assertEquals(FAKE_UID, Binder.getCallingUid());

        // Verify that the setUid() calls above did not break Binder.restoreCallingIdentity().
        Binder.restoreCallingIdentity(mToken);
        assertEquals(pid, Binder.getCallingPid());
        assertEquals(uid, Binder.getCallingUid());
    }
}
