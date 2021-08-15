package com.cells.cellswitch.secure.wifip2p;

import android.content.Context;
import android.app.Activity;
import android.os.AsyncTask;
import android.util.Log;
import android.widget.Toast;

import java.io.File;

import com.cells.cellswitch.secure.wifip2p.FileBean;
import com.cells.cellswitch.secure.wifip2p.ProgressDialog;
import com.cells.cellswitch.secure.wifip2p.SendSocket;

/**
 * description: Client send file details
 */

public class SendTask extends AsyncTask<String, Integer, Void> implements SendSocket.ProgressSendListener {

    private static final String TAG = "SendTask";

    private FileBean mFileBean;
    private Context mContext;
    private SendSocket mSendSocket;
    private ProgressDialog mProgressDialog;


    public SendTask(Context ctx, FileBean fileBean) {
        mFileBean = fileBean;
        mContext = ctx;
    }

    @Override
    protected void onPreExecute() {
        super.onPreExecute();
        //Create progress bar
        mProgressDialog = new ProgressDialog(mContext);
    }

    @Override
    protected Void doInBackground(String... strings) {
        mSendSocket = new SendSocket(mFileBean, strings[0], this);
        mSendSocket.createSendSocket();
        return null;
    }


    @Override
    public void onProgressChanged(File file, int progress) {
       // Log.e(TAG, "Send progress：" + progress);
        mProgressDialog.setProgress(progress);
        mProgressDialog.setProgressText(progress + "%");;
    }

    @Override
    public void onFinished(File file) {
        Log.e(TAG, "Send complete");
        if (mProgressDialog != null) {
            mProgressDialog.dismiss();
            mProgressDialog = null;
        }
        if(mContext instanceof Activity) {
            if(!((Activity)mContext).isFinishing() && !((Activity)mContext).isDestroyed())
                Toast.makeText(mContext, file.getName() + " Send complete！", Toast.LENGTH_SHORT).show();
        }
    }

    @Override
    public void onFaliure(File file) {
        Log.e(TAG, "Failed to send");
        if (mProgressDialog != null) {
            mProgressDialog.dismiss();
            mProgressDialog = null;
        }

        if(mContext instanceof Activity) {
            if(!((Activity)mContext).isFinishing() && !((Activity)mContext).isDestroyed())
                Toast.makeText(mContext, "Sending failed, please try again！", Toast.LENGTH_SHORT).show();
        }
    }
}
