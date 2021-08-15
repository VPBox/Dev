/*
 * Copyright (C) 2015 The Android Open Source Project
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
package com.android.example.cannylive;

import android.Manifest;
import android.app.Activity;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageManager;
import android.os.Bundle;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ViewFlipper;

import com.android.example.cannylive.R;

import java.text.DecimalFormat;
import java.util.Timer;
import java.util.TimerTask;

/**
 * Main Activity for this app
 * It presents a ui for setting ISO, Shutter speed, and focus
 */
public class MainActivity extends Activity {
    private static final String TAG = "MainActivity";
    private static final long ONE_SECOND = 1000000000;
    private CameraView mPreviewView;


    private Timer mTimer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(new TextView(this));
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);

        checkPermission();
    }
    protected void onPermissionOK() {

        setContentView(R.layout.activity_main);
        mPreviewView = (CameraView) findViewById(R.id.preview);

        mTimer = new Timer();

        mTimer.scheduleAtFixedRate(new TimerTask() {

            @Override
            public void run() {

                runOnUiThread(new Runnable() {
                    DecimalFormat df = new DecimalFormat("##.##");
                    public void run() {
                        setTitle("RS Camera (" + df.format( mPreviewView.getFps()) + "fps)");
                    }
                });

            }
        }, 250, 250);
    }

    public static final int PERMISSION_CHECK_ID =  22;
    private void  checkPermission() {
        String []permission = {
                Manifest.permission.CAMERA,
                Manifest.permission.RECORD_AUDIO,
                Manifest.permission.WRITE_EXTERNAL_STORAGE,
                Manifest.permission.READ_EXTERNAL_STORAGE
        };
        boolean permission_ok = true;
        boolean []permissionDenied = new boolean[permission.length];
        for (int i = 0; i < permission.length; i++) {
            String per = permission[i];
            int permissionCheck = ContextCompat.checkSelfPermission(this,
                    permission[i] );
            permissionDenied[i] = permissionCheck != PackageManager.PERMISSION_GRANTED;
            if (permissionDenied[i]) {
                permission_ok = false;
            }
        }
        Log.v(TAG, "Permission ok = " + permission_ok);

        if (! permission_ok ) {
            boolean request = false;
            for (int i = 0; i < permissionDenied.length; i++) {
                if ( permissionDenied[i]);
                if (ActivityCompat.shouldShowRequestPermissionRationale(this,
                        permission[i])) {
                    request = true;
                }
            }
            if (request) {
                Toast.makeText(this, "We need this permission", Toast.LENGTH_LONG).show();
                finish();
            } else {
                ActivityCompat.requestPermissions(this,
                        permission,
                        PERMISSION_CHECK_ID);

            }
        } else {
            onPermissionOK();
        }
    }
    public void onRequestPermissionsResult(int requestCode,
                                           String permissions[], int[] grantResults) {
        switch (requestCode) {
            case PERMISSION_CHECK_ID: {
                // If request is cancelled, the result arrays are empty.
                if (grantResults.length > 0
                        && grantResults[0] == PackageManager.PERMISSION_GRANTED) {

                    onPermissionOK();

                } else {
                    Toast.makeText(this, "We need permissions", Toast.LENGTH_LONG).show();

                    finish();
                }
                return;
            }

            // other 'case' lines to check for other
            // permissions this app might request
        }
    }
    @Override
    protected void onResume() {
        super.onResume();
        if (mPreviewView != null) {
            mPreviewView.resume(this);
        }
    }

    public void changeEffectMode(View v) {
        mPreviewView.changeEffectMode();
    }

    @Override
    protected void onPause() {
        Log.v(TAG,">>>>>>>>>>>>>> onPause()");
        super.onPause();
        if (mPreviewView != null) {
            mPreviewView.pause();
        }
    }

    @Override
    protected void onStop() {
        Log.v(TAG,">>>>>>>>>>>>>> onStop()");
        super.onStop();
    }

    @Override
    protected void onDestroy() {
        Log.v(TAG,">>>>>>>>>>>>>> onDestroy()");

        super.onDestroy();
    }

    public void capture(View v) {
        mPreviewView.takePicture(mPreviewView.getEffect());
    }

}
