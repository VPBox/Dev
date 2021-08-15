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


import android.content.ContentUris;
import android.content.ContentValues;
import android.content.UriMatcher;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.net.Uri;
import android.os.Bundle;
import android.provider.BaseColumns;
import android.telephony.SubscriptionManager;
import android.test.mock.MockContentProvider;
import android.util.Log;

import androidx.test.InstrumentationRegistry;

public class FakeTelephonyProvider extends MockContentProvider {
    static final String TAG = "FakeTelephonyProvider";

    private InMemoryTelephonyProviderDbHelper mDbHelper =
            new InMemoryTelephonyProviderDbHelper();

    /**
     * An in memory DB.
     */
    private class InMemoryTelephonyProviderDbHelper extends SQLiteOpenHelper {
        InMemoryTelephonyProviderDbHelper() {
            super(InstrumentationRegistry.getTargetContext(),
                    null,    // db file name is null for in-memory db
                    null,    // CursorFactory is null by default
                    1);      // db version is no-op for tests
            Log.d(TAG, "InMemoryTelephonyProviderDbHelper creating in-memory database");
        }

        // This should always be consistent with TelephonyProvider#getStringForSimInfoTableCreation.
        private String getStringForSimInfoTableCreation(String tableName) {
            return "CREATE TABLE " + tableName + "("
                    + SubscriptionManager.UNIQUE_KEY_SUBSCRIPTION_ID
                    + " INTEGER PRIMARY KEY AUTOINCREMENT,"
                    + SubscriptionManager.ICC_ID + " TEXT NOT NULL,"
                    + SubscriptionManager.SIM_SLOT_INDEX
                    + " INTEGER DEFAULT " + SubscriptionManager.SIM_NOT_INSERTED + ","
                    + SubscriptionManager.DISPLAY_NAME + " TEXT,"
                    + SubscriptionManager.CARRIER_NAME + " TEXT,"
                    + SubscriptionManager.NAME_SOURCE
                    + " INTEGER DEFAULT " + SubscriptionManager.NAME_SOURCE_DEFAULT_SOURCE + ","
                    + SubscriptionManager.COLOR + " INTEGER DEFAULT "
                    + SubscriptionManager.COLOR_DEFAULT + ","
                    + SubscriptionManager.NUMBER + " TEXT,"
                    + SubscriptionManager.DISPLAY_NUMBER_FORMAT
                    + " INTEGER NOT NULL DEFAULT "
                    + SubscriptionManager.DISPLAY_NUMBER_DEFAULT + ","
                    + SubscriptionManager.DATA_ROAMING
                    + " INTEGER DEFAULT " + SubscriptionManager.DATA_ROAMING_DEFAULT + ","
                    + SubscriptionManager.MCC + " INTEGER DEFAULT 0,"
                    + SubscriptionManager.MNC + " INTEGER DEFAULT 0,"
                    + SubscriptionManager.MCC_STRING + " TEXT,"
                    + SubscriptionManager.MNC_STRING + " TEXT,"
                    + SubscriptionManager.EHPLMNS + " TEXT,"
                    + SubscriptionManager.HPLMNS + " TEXT,"
                    + SubscriptionManager.SIM_PROVISIONING_STATUS
                    + " INTEGER DEFAULT " + SubscriptionManager.SIM_PROVISIONED + ","
                    + SubscriptionManager.IS_EMBEDDED + " INTEGER DEFAULT 0,"
                    + SubscriptionManager.CARD_ID + " TEXT NOT NULL,"
                    + SubscriptionManager.ACCESS_RULES + " BLOB,"
                    + SubscriptionManager.IS_REMOVABLE + " INTEGER DEFAULT 0,"
                    + SubscriptionManager.CB_EXTREME_THREAT_ALERT + " INTEGER DEFAULT 1,"
                    + SubscriptionManager.CB_SEVERE_THREAT_ALERT + " INTEGER DEFAULT 1,"
                    + SubscriptionManager.CB_AMBER_ALERT + " INTEGER DEFAULT 1,"
                    + SubscriptionManager.CB_EMERGENCY_ALERT + " INTEGER DEFAULT 1,"
                    + SubscriptionManager.CB_ALERT_SOUND_DURATION + " INTEGER DEFAULT 4,"
                    + SubscriptionManager.CB_ALERT_REMINDER_INTERVAL + " INTEGER DEFAULT 0,"
                    + SubscriptionManager.CB_ALERT_VIBRATE + " INTEGER DEFAULT 1,"
                    + SubscriptionManager.CB_ALERT_SPEECH + " INTEGER DEFAULT 1,"
                    + SubscriptionManager.CB_ETWS_TEST_ALERT + " INTEGER DEFAULT 0,"
                    + SubscriptionManager.CB_CHANNEL_50_ALERT + " INTEGER DEFAULT 1,"
                    + SubscriptionManager.CB_CMAS_TEST_ALERT + " INTEGER DEFAULT 0,"
                    + SubscriptionManager.CB_OPT_OUT_DIALOG + " INTEGER DEFAULT 1,"
                    + SubscriptionManager.ENHANCED_4G_MODE_ENABLED + " INTEGER DEFAULT -1,"
                    + SubscriptionManager.VT_IMS_ENABLED + " INTEGER DEFAULT -1,"
                    + SubscriptionManager.WFC_IMS_ENABLED + " INTEGER DEFAULT -1,"
                    + SubscriptionManager.WFC_IMS_MODE + " INTEGER DEFAULT -1,"
                    + SubscriptionManager.WFC_IMS_ROAMING_MODE + " INTEGER DEFAULT -1,"
                    + SubscriptionManager.WFC_IMS_ROAMING_ENABLED + " INTEGER DEFAULT -1,"
                    + SubscriptionManager.IS_OPPORTUNISTIC + " INTEGER DEFAULT 0,"
                    + SubscriptionManager.GROUP_UUID + " TEXT,"
                    + SubscriptionManager.IS_METERED + " INTEGER DEFAULT 1,"
                    + SubscriptionManager.ISO_COUNTRY_CODE + " TEXT,"
                    + SubscriptionManager.CARRIER_ID + " INTEGER DEFAULT -1,"
                    + SubscriptionManager.PROFILE_CLASS
                    + " INTEGER DEFAULT " + SubscriptionManager.PROFILE_CLASS_DEFAULT + ","
                    + SubscriptionManager.SUBSCRIPTION_TYPE + " INTEGER DEFAULT 0,"
                    + SubscriptionManager.WHITE_LISTED_APN_DATA + " INTEGER DEFAULT 0,"
                    + SubscriptionManager.GROUP_OWNER + " TEXT,"
                    + SubscriptionManager.DATA_ENABLED_OVERRIDE_RULES + " TEXT,"
                    + SubscriptionManager.IMSI + " TEXT,"
                    + SubscriptionManager.ACCESS_RULES_FROM_CARRIER_CONFIGS + " BLOB"
                    + ");";
        }

        @Override
        public void onCreate(SQLiteDatabase db) {
            // TODO: set up other tables when needed.
            // set up the siminfo table
            Log.d(TAG, "InMemoryTelephonyProviderDbHelper onCreate creating the siminfo table");
            db.execSQL(getStringForSimInfoTableCreation("siminfo"));
        }

        @Override
        public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
            Log.d(TAG, "InMemoryTelephonyProviderDbHelper onUpgrade doing nothing");
            return;
        }
    }

    @Override
    public Uri insert(Uri uri, ContentValues values) {
        SQLiteDatabase db = mDbHelper.getWritableDatabase();
        long id = db.insert("siminfo", null, values);
        return ContentUris.withAppendedId(SubscriptionManager.CONTENT_URI, id);
    }

    @Override
    public synchronized int delete(Uri url, String where, String[] whereArgs) {
        return mDbHelper.getWritableDatabase().delete("siminfo", where, whereArgs);
    }

    @Override
    public Cursor query(Uri uri, String[] projection, String selection, String[] selectionArgs,
            String sortOrder) {
        return mDbHelper.getReadableDatabase().query("siminfo", projection, selection,
                selectionArgs, null, null, sortOrder);
    }

    @Override
    public Bundle call(String method, String request, Bundle args) {
        return null;
    }

    @Override
    public final int update(Uri uri, ContentValues values, String where, String[] selectionArgs) {
        // handle URI with appended subId
        final int urlSimInfoSubId = 0;
        UriMatcher matcher = new UriMatcher(UriMatcher.NO_MATCH);
        matcher.addURI("telephony", "siminfo/#", urlSimInfoSubId);
        if (matcher.match(uri) == urlSimInfoSubId) {
            where = BaseColumns._ID + "=" + uri.getLastPathSegment();
        }

        int count = mDbHelper.getWritableDatabase().update("siminfo", values, where,
                selectionArgs);
        return count;
    }
}
