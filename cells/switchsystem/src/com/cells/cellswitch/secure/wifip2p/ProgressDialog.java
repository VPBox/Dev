package com.cells.cellswitch.secure.wifip2p;

import android.content.Context;
import android.content.ContextWrapper;
import android.support.annotation.NonNull;
import android.support.annotation.StyleRes;
import android.app.AlertDialog;
import android.app.Activity;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.WindowManager;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.cells.cellswitch.secure.wifip2p.ScreenUtils;
import com.cells.cellswitch.secure.R;

public class ProgressDialog extends AlertDialog.Builder {

    private TextView mProgressText;
    private ProgressBar mProgress;

    private AlertDialog mDialog;
    private View mView;

    public ProgressDialog(@NonNull Context context) {
        super(context);
        init();
    }

    public ProgressDialog(@NonNull Context context, @StyleRes int themeResId) {
        super(context, themeResId);
        init();
    }

    private void init() {
        mDialog = this.create();
        WindowManager.LayoutParams params = mDialog.getWindow().getAttributes();
        params.height = ScreenUtils.dip2px(getContext(), 130);
        params.gravity = Gravity.CENTER;
        mDialog.getWindow().setAttributes(params);

        mView = LayoutInflater.from(getContext()).inflate(R.layout.progressbar, null);
        mProgressText = (TextView)mView.findViewById(R.id.tv_text);
        mProgress = (ProgressBar)mView.findViewById(R.id.pb_process);
        mDialog.show();
        mDialog.setContentView(mView);
        mDialog.setCancelable(false);
    }

    public void setProgressText(String text){
        String show = "      " + text + "      ";
        mProgressText.setText(show);
    }

    public void setProgress(int progress){
        mProgress.setProgress(progress);
    }

    public void dismiss(){
        if(mDialog != null) {
            if(mDialog.isShowing()) {
                Context context = ((ContextWrapper)mDialog.getContext()).getBaseContext(); 
                if(context instanceof Activity) {
                    if(!((Activity)context).isFinishing() && !((Activity)context).isDestroyed())
                        mDialog.dismiss();
                }else {
                    mDialog.dismiss();
                }
            } 
            mDialog = null; 
        }
    }

}
