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

import android.annotation.NonNull;
import android.annotation.Nullable;
import android.content.ContentValues;
import android.content.UriMatcher;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.net.Uri;
import android.telephony.SubscriptionManager;
import android.test.mock.MockContentProvider;

public class FakeSmsContentProvider extends MockContentProvider {
    private static final String RAW_TABLE_NAME = "raw";
    public SQLiteOpenHelper mDbHelper = new InMemorySmsDbHelper();

    private static final UriMatcher sURLMatcher =
            new UriMatcher(UriMatcher.NO_MATCH);
    private static final int SMS_RAW_MESSAGE = 1;
    private static final int SMS_RAW_MESSAGE_PERMANENT_DELETE = 2;
    static {
        sURLMatcher.addURI("sms", "raw", SMS_RAW_MESSAGE);
        sURLMatcher.addURI("sms", "raw/permanentDelete", SMS_RAW_MESSAGE_PERMANENT_DELETE);
    }

    private class InMemorySmsDbHelper extends SQLiteOpenHelper {
        public InMemorySmsDbHelper() {
            super(getContext(),
                    null,   //db file name - null for in-memory db
                    null,   //CursorFactory - null for default
                    1);     //db version - no-op for tests
        }

        @Override
        public void onCreate(SQLiteDatabase db) {
            db.execSQL("CREATE TABLE raw (" +
                    "_id INTEGER PRIMARY KEY," +
                    "date INTEGER," +
                    "reference_number INTEGER," + // one per full message
                    "count INTEGER," + // the number of parts
                    "sequence INTEGER," + // the part number of this message
                    "destination_port INTEGER," +
                    "address TEXT," +
                    "sub_id INTEGER DEFAULT " +
                    SubscriptionManager.INVALID_SUBSCRIPTION_ID + ", " +
                    "pdu TEXT," + // the raw PDU for this part
                    "deleted INTEGER DEFAULT 0," + // bool to indicate if row is deleted
                    "message_body TEXT," + // message body
                    "display_originating_addr TEXT);");// display address
        }

        @Override
        public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        }
    }

    @Override
    public Cursor query(@NonNull Uri uri, @Nullable String[] projection,
                        @Nullable String selection, @Nullable String[] selectionArgs,
                        @Nullable String sortOrder) {
        SQLiteDatabase db = mDbHelper.getReadableDatabase();
        return db.query(RAW_TABLE_NAME, projection, selection, selectionArgs, null, null,
                sortOrder);
    }

    @Override
    public Uri insert(@NonNull Uri uri, @Nullable ContentValues values) {
        SQLiteDatabase db = mDbHelper.getWritableDatabase();
        long rowId = db.insert(RAW_TABLE_NAME, null, values);
        return Uri.parse("content://raw/" + rowId);
    }

    @Override
    public int delete(@NonNull Uri uri, @Nullable String selection,
                      @Nullable String[] selectionArgs) {
        SQLiteDatabase db = mDbHelper.getWritableDatabase();
        int match = sURLMatcher.match(uri);
        int count = 0;
        switch (match) {
            case SMS_RAW_MESSAGE:
                ContentValues cv = new ContentValues();
                cv.put("deleted", 1);
                count = db.update(RAW_TABLE_NAME, cv, selection, selectionArgs);
                break;

            case SMS_RAW_MESSAGE_PERMANENT_DELETE:
                count = db.delete(RAW_TABLE_NAME, selection, selectionArgs);
                break;
        }
        return count;
    }

    @Override
    public void shutdown() {
        mDbHelper.close();
    }

    public int getNumRows() {
        int numRows = 0;
        Cursor c = query(null, null, null, null, null);
        if (c != null) {
            numRows = c.getCount();
            c.close();
        }
        return numRows;
    }
}