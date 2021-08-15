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
 * limitations under the License
 */

// Note: Package name is intentionally wrong for this test; the internal junk class is used to test
// that parcelables of types other than android.* are stripped out.
package com.android.internal.telephony.ims;

import static junit.framework.Assert.assertEquals;
import static junit.framework.Assert.assertFalse;
import static junit.framework.Assert.assertNotNull;

import android.os.Parcel;
import android.os.Parcelable;
import android.telecom.DisconnectCause;
import android.telephony.ims.ImsCallProfile;
import android.test.suitebuilder.annotation.SmallTest;

import androidx.test.runner.AndroidJUnit4;

import org.junit.Test;
import org.junit.runner.RunWith;

/**
 * Tests for the {@link ImsCallProfile} class.
 *
 * Test must NOT be in the "android." namespace.
 */
@RunWith(AndroidJUnit4.class)
public class ImsCallProfileTest {
    // A test-only parcelable class which is not in the android.* namespace.
    private static class JunkParcelable implements Parcelable {
        private int mTest;

        JunkParcelable() {
        }

        protected JunkParcelable(Parcel in) {
            mTest = in.readInt();
        }

        public static final Creator<JunkParcelable> CREATOR = new Creator<ImsCallProfileTest.JunkParcelable>() {
            @Override
            public ImsCallProfileTest.JunkParcelable createFromParcel(Parcel in) {
                return new ImsCallProfileTest.JunkParcelable(in);
            }

            @Override
            public ImsCallProfileTest.JunkParcelable[] newArray(int size) {
                return new ImsCallProfileTest.JunkParcelable[size];
            }
        };

        @Override
        public int describeContents() {
            return 0;
        }

        @Override
        public void writeToParcel(Parcel dest, int flags) {
            dest.writeInt(mTest);
        }
    }

    /**
     * Ensures that the {@link ImsCallProfile} will discard invalid extras when it is parceled.
     */
    @Test
    @SmallTest
    public void testExtrasCleanup() {
        ImsCallProfile srcParcel = new ImsCallProfile();
        // Put in a private parcelable type.
        srcParcel.mCallExtras.putParcelable("JUNK", new JunkParcelable());
        // Put in an api defined parcelable type.
        srcParcel.mCallExtras.putParcelable("NOTJUNK", new DisconnectCause(DisconnectCause.BUSY));
        // Put in some valid things.
        srcParcel.mCallExtras.putInt("INT", 1);
        srcParcel.mCallExtras.putString("STRING", "hello");

        // Parcel it.
        Parcel parcel = Parcel.obtain();
        srcParcel.writeToParcel(parcel, 0);
        byte[] parcelBytes = parcel.marshall();
        parcel.recycle();

        // Unparcel it.
        parcel = Parcel.obtain();
        parcel.unmarshall(parcelBytes, 0, parcelBytes.length);
        parcel.setDataPosition(0);
        ImsCallProfile unparceledProfile = ImsCallProfile.CREATOR.createFromParcel(parcel);
        parcel.recycle();

        assertNotNull(unparceledProfile.mCallExtras);
        assertEquals(3, unparceledProfile.mCallExtras.size());
        assertEquals(1, unparceledProfile.getCallExtraInt("INT"));
        assertEquals("hello", unparceledProfile.getCallExtra("STRING"));
        assertFalse(unparceledProfile.mCallExtras.containsKey("JUNK"));

        DisconnectCause parceledCause = unparceledProfile.mCallExtras.getParcelable("NOTJUNK");
        assertEquals(DisconnectCause.BUSY, parceledCause.getCode());
    }
}
