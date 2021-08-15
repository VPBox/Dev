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

import static com.google.common.truth.Truth.assertThat;

import android.content.ContentValues;
import android.database.Cursor;
import android.net.Uri;
import android.test.mock.MockContentProvider;

import java.util.LinkedList;
import java.util.Queue;

/**
 * Mocking/spying ContentProviders break in different ways. Use a fake instead. The
 * RcsMessageStoreController doesn't care if RcsProvider works as expected (and doesn't have
 * visibility into it) - so verifying whether we use the correct parameters should suffice.
 */
class FakeProviderWithAsserts extends MockContentProvider {
    /**
     * Interface implemented by all classes that describe expected operations.
     */
    private interface ExpectedOperation {}

    static class ExpectedQuery implements ExpectedOperation {
        private final Uri mUri;
        private final String[] mProjection;
        private final String mWhereClause;
        private final String[] mWhereArgs;
        private final String mSortOrder;
        private final Cursor mReturnValue;

        ExpectedQuery(Uri uri, String[] projection, String whereClause, String[] whereArgs,
                String sortOrder, Cursor returnValue) {
            mUri = uri;
            mProjection = projection;
            mWhereClause = whereClause;
            mWhereArgs = whereArgs;
            mSortOrder = sortOrder;
            mReturnValue = returnValue;
        }
    }

    static class ExpectedInsert implements ExpectedOperation {
        private Uri mUri;
        private ContentValues mContentValues;
        private Uri mReturnValue;

        ExpectedInsert(Uri uri, ContentValues contentValues, Uri returnValue) {
            mUri = uri;
            mContentValues = contentValues;
            mReturnValue = returnValue;
        }
    }

    static class ExpectedUpdate implements ExpectedOperation {
        private Uri mUri;
        private String mWhereClause;
        private String[] mWhereArgs;
        private ContentValues mContentValues;
        private int mReturnValue;

        ExpectedUpdate(Uri uri, String whereClause, String[] whereArgs,
                ContentValues contentValues, int returnValue) {
            mUri = uri;
            mWhereClause = whereClause;
            mWhereArgs = whereArgs;
            mContentValues = contentValues;
            mReturnValue = returnValue;
        }
    }

    private Queue<ExpectedOperation> mExpectedOperations = new LinkedList<>();

    void addExpectedOperation(ExpectedOperation expectedOperation) {
        mExpectedOperations.add(expectedOperation);
    }

    private <T extends ExpectedOperation> T getExpectedOperation(Class<T> clazz) {
        ExpectedOperation expectedOperation = mExpectedOperations.remove();
        assertThat(expectedOperation).isNotNull();
        assertThat(expectedOperation).isInstanceOf(clazz);
        return clazz.cast(expectedOperation);
    }

    @Override
    public Cursor query(Uri uri, String[] projection, String whereClause, String[] whereArgs,
            String sortOrder) {
        ExpectedQuery expectedQuery = getExpectedOperation(ExpectedQuery.class);
        assertThat(uri).isEqualTo(expectedQuery.mUri);
        assertThat(projection).isEqualTo(expectedQuery.mProjection);
        assertThat(whereClause).isEqualTo(expectedQuery.mWhereClause);
        assertThat(whereArgs).isEqualTo(expectedQuery.mWhereArgs);
        assertThat(sortOrder).isEqualTo(expectedQuery.mSortOrder);
        return expectedQuery.mReturnValue;
    }

    @Override
    public Uri insert(Uri uri, ContentValues contentValues) {
        ExpectedInsert expectedInsert = getExpectedOperation(ExpectedInsert.class);
        assertThat(uri).isEqualTo(expectedInsert.mUri);
        assertThatContentValuesAreEquivalent(contentValues, expectedInsert.mContentValues);
        return expectedInsert.mReturnValue;
    }

    @Override
    public int update(Uri uri, ContentValues contentValues, String whereClause,
            String[] whereArgs) {
        ExpectedUpdate expectedUpdate = getExpectedOperation(ExpectedUpdate.class);
        assertThat(uri).isEqualTo(expectedUpdate.mUri);
        assertThatContentValuesAreEquivalent(contentValues, expectedUpdate.mContentValues);
        assertThat(whereClause).isEqualTo(expectedUpdate.mWhereClause);
        assertThat(whereArgs).isEqualTo(expectedUpdate.mWhereArgs);
        return expectedUpdate.mReturnValue;
    }

    private void assertThatContentValuesAreEquivalent(
            ContentValues actual, ContentValues expected) {
        assertThat(actual.size()).isEqualTo(expected.size());
        for (String key : expected.keySet()) {
            assertThat(actual.get(key)).isEqualTo(expected.get(key));
        }
    }
}
