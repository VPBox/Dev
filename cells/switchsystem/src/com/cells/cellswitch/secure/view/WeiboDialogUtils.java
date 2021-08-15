package com.cells.cellswitch.secure.view;

import android.app.Dialog;
import android.content.Context;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.LinearLayout;
import android.widget.TextView;
import com.cells.cellswitch.secure.R;

public class WeiboDialogUtils {
	public static Dialog createLoadingDialog(Context context, String msg) {
		LayoutInflater inflater = LayoutInflater.from(context);
		View v = inflater.inflate(R.layout.dialog_loading, null);
		LinearLayout layout = (LinearLayout) v.findViewById(R.id.dialog_loading_view);
		TextView tipTextView = (TextView) v.findViewById(R.id.tipTextView);
		tipTextView.setText(msg);

		Dialog loadingDialog = new Dialog(context, R.style.MyDialogStyle);
		loadingDialog.setCancelable(false); 
		loadingDialog.setCanceledOnTouchOutside(false); 
		loadingDialog.setContentView(layout, new LinearLayout.LayoutParams(
				LinearLayout.LayoutParams.MATCH_PARENT,
				LinearLayout.LayoutParams.MATCH_PARENT));

		Window window = loadingDialog.getWindow();
		WindowManager.LayoutParams lp = window.getAttributes();
		lp.width = WindowManager.LayoutParams.MATCH_PARENT;
		lp.height = WindowManager.LayoutParams.WRAP_CONTENT;
		window.setGravity(Gravity.CENTER);
		window.setAttributes(lp);
		window.setWindowAnimations(R.style.PopWindowAnimStyle);
		loadingDialog.show();

		return loadingDialog;
	}

	public static void closeDialog(Dialog mDialogUtils) {
		if (mDialogUtils != null && mDialogUtils.isShowing()) {
			mDialogUtils.dismiss();
		}
	}

	public static void setText(Dialog mDialogUtils,String msg) {
		if (mDialogUtils != null && mDialogUtils.isShowing()) {
			LayoutInflater inflater = mDialogUtils.getLayoutInflater();
			View v = inflater.inflate(R.layout.dialog_loading, null);
			TextView tipTextView = (TextView) v.findViewById(R.id.tipTextView);
			tipTextView.setText(msg);
		}
	}

}