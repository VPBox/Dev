package com.example.simpleperf.simpleperfexamplewithnative;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

public class MixActivity extends AppCompatActivity {

    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_mix);
        createBusyThread();
    }

    void createBusyThread() {
        new Thread(new Runnable() {
            volatile int i = 0;

            @Override
            public void run() {
                while (true) {
                    i = callFunction(i);
                }
            }

        }, "BusyThread").start();
    }

    private native int callFunction(int a);
}
