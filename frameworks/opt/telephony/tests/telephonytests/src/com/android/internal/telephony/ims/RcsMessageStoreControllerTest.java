/*
 * Copyright 2019 The Android Open Source Project
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

package com.android.internal.telephony.ims;

import static android.provider.Telephony.RcsColumns.Rcs1To1ThreadColumns.FALLBACK_THREAD_ID_COLUMN;
import static android.provider.Telephony.RcsColumns.RcsGroupThreadColumns.GROUP_ICON_COLUMN;
import static android.provider.Telephony.RcsColumns.RcsGroupThreadColumns.GROUP_NAME_COLUMN;
import static android.provider.Telephony.RcsColumns.RcsGroupThreadColumns.OWNER_PARTICIPANT_COLUMN;
import static android.telephony.ims.RcsThreadQueryParams.THREAD_TYPE_GROUP;

import static com.google.common.truth.Truth.assertThat;

import static org.mockito.Mockito.doReturn;

import android.content.ContentValues;
import android.database.MatrixCursor;
import android.net.Uri;
import android.provider.Telephony;
import android.provider.Telephony.RcsColumns.RcsParticipantColumns;
import android.telephony.ims.RcsParticipant;
import android.telephony.ims.RcsThreadQueryParams;
import android.test.mock.MockContentResolver;

import com.android.internal.telephony.TelephonyTest;
import com.android.internal.telephony.ims.FakeProviderWithAsserts.ExpectedInsert;
import com.android.internal.telephony.ims.FakeProviderWithAsserts.ExpectedQuery;
import com.android.internal.telephony.ims.FakeProviderWithAsserts.ExpectedUpdate;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

public class RcsMessageStoreControllerTest extends TelephonyTest {

    private RcsMessageStoreController mRcsMessageStoreController;
    private MockContentResolver mContentResolver;
    private FakeProviderWithAsserts mFakeRcsProvider;

    @Mock
    RcsParticipant mMockParticipant;

    @Before
    public void setUp() throws Exception {
        super.setUp("RcsMessageStoreControllerTest");
        MockitoAnnotations.initMocks(this);

        mFakeRcsProvider = new FakeProviderWithAsserts();
        mContentResolver = (MockContentResolver) mContext.getContentResolver();
        mContentResolver.addProvider("rcs", mFakeRcsProvider);

        mRcsMessageStoreController = new RcsMessageStoreController(mContext);
    }

    @After
    public void tearDown() throws Exception {
        super.tearDown();
    }

    @Test
    public void testGetRcsThreads() {
        doReturn(123).when(mMockParticipant).getId();
        RcsThreadQueryParams queryParameters =
                new RcsThreadQueryParams.Builder().setParticipant(mMockParticipant)
                        .setThreadType(THREAD_TYPE_GROUP).setResultLimit(30).build();

        // TODO - limit the query as per queryParameters. This will change how the query is executed
        mFakeRcsProvider.addExpectedOperation(new ExpectedQuery(
                Uri.parse("content://rcs/thread"), null, null, null, null, null));

        try {
            mRcsMessageStoreController.getRcsThreads(queryParameters, getPackageName());
        } catch (RuntimeException e) {
            // eat the exception as there is no provider - we care about the expected update assert
        }
    }

    @Test
    public void testCreateRcsParticipant() {
        String canonicalAddress = "+5551234567";

        // verify the first query to canonical addresses
        MatrixCursor canonicalAddressQueryCursor = new MatrixCursor(
                new String[]{Telephony.CanonicalAddressesColumns._ID});
        canonicalAddressQueryCursor.addRow(new Object[]{456});

        Uri expectedCanonicalAddressUri = Uri.parse("content://rcs/canonical-address")
                .buildUpon()
                .appendQueryParameter("address", canonicalAddress)
                .build();

        mFakeRcsProvider.addExpectedOperation(new ExpectedQuery(
                expectedCanonicalAddressUri, null, null, null, null, canonicalAddressQueryCursor));


        // verify the final insert on rcs participants
        ContentValues expectedRcsValues = new ContentValues(1);
        expectedRcsValues.put(RcsParticipantColumns.CANONICAL_ADDRESS_ID_COLUMN, 456);
        expectedRcsValues.put(RcsParticipantColumns.RCS_ALIAS_COLUMN, "alias");
        mFakeRcsProvider.addExpectedOperation(new ExpectedInsert(
                Uri.parse("content://rcs/participant"), expectedRcsValues,
                Uri.parse("content://rcs/participant/1001")));

        int participantId =
                mRcsMessageStoreController.createRcsParticipant(canonicalAddress, "alias",
                        getPackageName());

        assertThat(participantId).isEqualTo(1001);
    }

    @Test
    public void testUpdateRcsParticipantAlias() {
        ContentValues contentValues = new ContentValues(1);
        contentValues.put("rcs_alias", "New Alias");
        mFakeRcsProvider.addExpectedOperation(new ExpectedUpdate(
                Uri.parse("content://rcs/participant/551"), null, null, contentValues, 0));

        try {
            mRcsMessageStoreController.setRcsParticipantAlias(551, "New Alias", getPackageName());
        } catch (RuntimeException e) {
            // eat the exception as there is no provider - we care about the expected update assert
        }
    }

    @Test
    public void testSet1To1ThreadFallbackThreadId() {
        ContentValues contentValues = new ContentValues(1);
        contentValues.put(FALLBACK_THREAD_ID_COLUMN, 456L);
        mFakeRcsProvider.addExpectedOperation(new ExpectedUpdate(
                Uri.parse("content://rcs/p2p_thread/123"), null, null, contentValues, 0));
        try {
            mRcsMessageStoreController.set1To1ThreadFallbackThreadId(123, 456L, getPackageName());
        } catch (RuntimeException e) {
            // eat the exception as there is no provider - we care about the expected update assert
        }
    }

    @Test
    public void testSetGroupThreadName() {
        ContentValues contentValues = new ContentValues(1);
        contentValues.put(GROUP_NAME_COLUMN, "new name");
        mFakeRcsProvider.addExpectedOperation(new ExpectedUpdate(
                Uri.parse("content://rcs/group_thread/345"), null, null, contentValues, 0));

        try {
            mRcsMessageStoreController.setGroupThreadName(345, "new name", getPackageName());
        } catch (RuntimeException e) {
            // eat the exception as there is no provider - we care about the expected update assert
        }
    }

    @Test
    public void testSetGroupThreadIcon() {
        ContentValues contentValues = new ContentValues(1);
        contentValues.put(GROUP_ICON_COLUMN, "newIcon");
        mFakeRcsProvider.addExpectedOperation(new ExpectedUpdate(
                Uri.parse("content://rcs/group_thread/345"), null, null, contentValues, 0));

        try {
            mRcsMessageStoreController.setGroupThreadIcon(345, Uri.parse("newIcon"),
                    getPackageName());
        } catch (RuntimeException e) {
            // eat the exception as there is no provider - we care about the expected update assert
        }
    }

    @Test
    public void testSetGroupThreadOwner() {
        ContentValues contentValues = new ContentValues(1);
        contentValues.put(OWNER_PARTICIPANT_COLUMN, 9);
        mFakeRcsProvider.addExpectedOperation(new ExpectedUpdate(
                Uri.parse("content://rcs/group_thread/454"), null, null, contentValues, 0));

        try {
            mRcsMessageStoreController.setGroupThreadOwner(454, 9, getPackageName());
        } catch (RuntimeException e) {
            // eat the exception as there is no provider - we care about the expected update assert
        }
    }

    private String getPackageName() {
        return mContext.getOpPackageName();
    }
}
