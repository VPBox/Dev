package com.example.simpleperf.simpleperfexamplewithnative;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

public class SleepActivity extends AppCompatActivity {

    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_sleep);
        createSleepThreadFromJNI();
    }

    private native void createSleepThreadFromJNI();
}
