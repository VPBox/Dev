package com.cells.cellswitch.secure.wifip2p;

import android.app.IntentService;
import android.os.RemoteException;
import android.content.Intent;
import android.content.Context;
import android.app.Activity;
import android.widget.Toast;
import android.os.Binder;
import android.os.IBinder;
import android.support.annotation.Nullable;
import android.util.Log;

import java.io.File;

import com.cells.cellswitch.secure.wifip2p.ReceiveSocket;
import com.cells.cellswitch.secure.wifip2p.ProgressDialog;
import com.cells.cellswitch.secure.view.SwitchActivity;

public class Wifip2pService extends IntentService implements ReceiveSocket.ProgressReceiveListener {

    private static final String TAG = "Wifip2pService";
    private ReceiveSocket mReceiveSocket;
    private ProgressDialog mReceiveProgress;
    private Context mContext;

    public Wifip2pService() {
        super("Wifip2pService");
    }

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return new MyBinder();
    }

    public class MyBinder extends Binder {

        public MyBinder() {
            super();
        }
        public void init(Context ctx){
            mContext = ctx;
            mReceiveSocket.setOnProgressReceiveListener(Wifip2pService.this);
        }
    }

    @Override
    public void onCreate() {
        super.onCreate();

        Log.e(TAG, "Service start.");
    }

    @Override
    protected void onHandleIntent(Intent intent) {
        mReceiveSocket = new ReceiveSocket();

        while(true){

            mReceiveSocket.createServerSocket();

            mReceiveSocket.clear();

            Log.e(TAG, "Transmission completed.");

            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    public void onDestroy() {
        super.onDestroy();

        mReceiveSocket.clear();
    }

    @Override
    public void onReceive() {
        mReceiveProgress = new ProgressDialog(mContext);
    }

    @Override
    public void onProgressChanged(File file, int progress) {
        mReceiveProgress.setProgress(progress);
        mReceiveProgress.setProgressText(progress + "%");
    }

    @Override
    public void onFinished(File file) {
        Log.e(TAG, "Receive complete.");

        if (mReceiveProgress != null) {
            mReceiveProgress.dismiss();
            mReceiveProgress = null;
        }

        if(mContext instanceof Activity) {
            if(!((Activity)mContext).isFinishing() && !((Activity)mContext).isDestroyed())
                Toast.makeText(mContext, file.getName() + " Received.", Toast.LENGTH_SHORT).show();
        }

        if(mContext instanceof Activity) {
            if(!((Activity)mContext).isFinishing() && !((Activity)mContext).isDestroyed()){
                SwitchActivity sa = (SwitchActivity)mContext;
                sa.btnStop(null);
                sa.btnStart(null);
            }
        }

    }

    @Override
    public void onFaliure(File file) {
        Log.e(TAG, "Receive failed.");

        if (mReceiveProgress != null) {
            mReceiveProgress.dismiss();
            mReceiveProgress = null;
        }

        if(mContext instanceof Activity) {
            if(!((Activity)mContext).isFinishing() && !((Activity)mContext).isDestroyed())
                Toast.makeText(mContext, "Reception failed, please try again.", Toast.LENGTH_SHORT).show();
        }
    }
}
