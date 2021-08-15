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

package com.cells.cellswitch.secure.view;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

import android.app.CellsPrivateServiceManager;
import android.os.RemoteException;

/**
 * Performs a number of miscellaneous, non-system-critical actions
 * after the system has finished booting.
 */
public class BootCompleteReceiver extends BroadcastReceiver {
	private static final String TAG = "BootCompleteReceiver";
    private boolean mBootCompleted = false;

    @Override
    public void onReceive(final Context context, Intent intent) {
        if (mBootCompleted) return;
        
        /*CellsPrivateServiceManager mCellsService = new CellsPrivateServiceManager(context);
        if(mCellsService != null){
            try{
                mCellsService.vmSystemReady();
                Log.e(TAG, "vmSystemReady.");
            }catch(RemoteException e){
                e.printStackTrace();
            }
        }*/

        Log.e(TAG, "BootCompleted = true.");
        mBootCompleted = true;
    }
}
