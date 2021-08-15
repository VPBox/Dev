package com.example.simpleperf.simpleperfexamplepurejava;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;

public class MultiProcessActivity extends AppCompatActivity {
    public static final String TAG = "MultiProcessActivity";

    Messenger mService = null;
    boolean mBound;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_multi_process);

        bindService(new Intent(this, MultiProcessService.class), mConnection,
                Context.BIND_AUTO_CREATE);
        createBusyThread();
    }

    private ServiceConnection mConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName componentName, IBinder iBinder) {
            mService = new Messenger(iBinder);
            mBound = true;
            Message message = new Message();
            message.what = MultiProcessService.MSG_START_BUSY_THREAD;
            try {
                mService.send(message);
            } catch (RemoteException e) {
                Log.d(TAG, e.toString());
            }
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            mService = null;
            mBound = false;
        }
    };

    @Override
    protected void onDestroy() {
        super.onDestroy();

        if (mBound) {
            unbindService(mConnection);
            mBound = false;
        }
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
