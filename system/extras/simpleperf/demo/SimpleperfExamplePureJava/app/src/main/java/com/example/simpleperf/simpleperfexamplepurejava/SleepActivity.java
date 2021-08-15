package com.example.simpleperf.simpleperfexamplepurejava;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

public class SleepActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_sleep);
        createRunSleepThread();
    }

    void createRunSleepThread() {
        new Thread(new Runnable() {
            volatile int counter = 0;
            long totalRunTimeInNs = 0;
            long totalSleepTimeInNs = 0;

            private long RunFunction() {
                long startTimeInNs = System.nanoTime();
                for (int i = 0; i < 10000000; ++i) {
                    counter = callFunction(counter);
                }
                return System.nanoTime() - startTimeInNs;
            }

            private long SleepFunction(long sleepTimeInNs) {
                long startTimeInNs = System.nanoTime();
                try {
                    Thread.sleep(sleepTimeInNs / 1000000, (int) (sleepTimeInNs % 1000000));
                } catch (Exception e) {
                }
                return System.nanoTime() - startTimeInNs;
            }

            @Override
            public void run() {
                while (true) {
                    totalRunTimeInNs += RunFunction();
                    if (totalSleepTimeInNs < totalRunTimeInNs) {
                        totalSleepTimeInNs += SleepFunction(
                                totalRunTimeInNs - totalSleepTimeInNs);
                    }
                }
            }

            private int callFunction(int a) {
                return a+1;
            }
        }, "RunSleepThread").start();
    }

}
