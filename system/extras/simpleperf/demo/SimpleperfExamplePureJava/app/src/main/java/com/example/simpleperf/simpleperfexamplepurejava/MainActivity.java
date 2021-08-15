package com.example.simpleperf.simpleperfexamplepurejava;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

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

            private int callFunction(int a) {
                return a+1;
            }
        }, "BusyThread").start();
    }
}
