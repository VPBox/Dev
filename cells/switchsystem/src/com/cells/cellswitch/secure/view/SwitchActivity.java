package com.cells.cellswitch.secure.view;

import android.app.Activity;
import android.app.Dialog;
import android.util.Log;
import android.os.Bundle;
import android.os.IBinder;
import android.os.Handler;
import android.os.Message;
import android.os.SystemProperties;
import android.os.ServiceManager;
import android.os.Looper;
import android.os.RemoteException;
import android.view.View;
import android.view.WindowManager;
import android.view.animation.Animation;
import android.view.GestureDetector;
import android.view.GestureDetector.SimpleOnGestureListener;
import android.view.MotionEvent;
import android.view.View.OnTouchListener;
import android.widget.Toast;
import android.widget.Button;
import android.widget.LinearLayout;
import java.io.IOException;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileInputStream;
import com.cells.cellswitch.secure.R;
import android.Manifest;
import android.content.pm.PackageManager;
import android.support.v4.app.ActivityCompat;
import android.content.Context;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;
import android.app.CellsPrivateServiceManager;

import com.cells.cellswitch.secure.download.DownloadCallback;
import com.cells.cellswitch.secure.download.DownloadDispatcher;
import com.cells.cellswitch.secure.download.Utils;
import com.cells.cellswitch.secure.wifip2p.SendTask;
import com.cells.cellswitch.secure.wifip2p.Md5Util;
import com.cells.cellswitch.secure.wifip2p.FileUtils;
import com.cells.cellswitch.secure.wifip2p.FileBean;

import android.bluetooth.BluetoothAdapter;
import android.nfc.NfcAdapter;

public class SwitchActivity extends BaseActivity {
	private static final String TAG = "SwitchActivity";
	private static final int MSG_UP_CODE = 0x01;
	private static final int MSG_START_VM_CODE = 0x02;
	private static final int MSG_VM_BACK_CODE = 0x03;
	private static final int MSG_START_VM_TOAST_CODE = 0x04;
	private static final int MSG_P2P_CODE = 0x05;
	private static final int MSG_UP_CODE_PACK = 0x06;
	private static final int MSG_UP_CODE_CONNECT_ERR = 0x07;
	private static final int MSG_UP_CODE_SEND_ERR = 0x08;
	private static final int MSG_UP_CODE_SEND_SUCC = 0x09;
	private static final int REQUEST_PERMISSION_CODE = 0x088;
	private Dialog mWeiboDialog;
	private WakeLock mWakeLock = null;
	private Handler mHandler = new Handler() {
		@Override
		public void handleMessage(Message msg) {
			super.handleMessage(msg);
			switch (msg.what) {
				case MSG_UP_CODE:
				{
					up();
					break;
				}
				case MSG_P2P_CODE:
				{
					p2p();
					break;
				}
				case MSG_START_VM_CODE:
				{
					start();
					break;
				}
				case MSG_UP_CODE_CONNECT_ERR:
				{
					up_connect_err();
					break;
				}
				case MSG_UP_CODE_SEND_ERR:
				{
					up_send_err();
					break;
				}
				case MSG_UP_CODE_SEND_SUCC:
				{
					up_send_succ();
					break;
				}
				case MSG_START_VM_TOAST_CODE:
				{
					Toast.makeText(SwitchActivity.this, "Container failed to start.", Toast.LENGTH_SHORT).show();
					break;
				}
				case MSG_UP_CODE_PACK:
				{
					Toast.makeText(SwitchActivity.this,"Packaged successfully",Toast.LENGTH_SHORT).show();
					break;
				}
			}
		}
	};

	private void acquireWakeLock() {
		if (mWakeLock == null) {
			PowerManager pm = (PowerManager)getSystemService(Context.POWER_SERVICE);     
			mWakeLock = pm.newWakeLock(PowerManager.ACQUIRE_CAUSES_WAKEUP |
									 PowerManager.FULL_WAKE_LOCK | PowerManager.ON_AFTER_RELEASE, TAG);
			mWakeLock.acquire();
		}
	}

	private void releaseWakeLock() {
		if (mWakeLock != null && mWakeLock.isHeld()) {
			mWakeLock.release();
			mWakeLock = null;
		}
	}

	private GestureDetector mGD;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		acquireWakeLock();

		mCellsService = new CellsPrivateServiceManager(SwitchActivity.this,
																ServiceManager.getService("CellsPrivateService"));

		int isPermission = ActivityCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE);
		if (isPermission == PackageManager.PERMISSION_DENIED) {
			ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, REQUEST_PERMISSION_CODE);
		}

		if(SystemProperties.get("ro.boot.vm").equals("1")){
			Button btn_vm = (Button) findViewById(R.id.btn_vm);
			btn_vm.setText(R.string.back_string);
			btn_vm.setOnClickListener(new View.OnClickListener() {
				@Override
				public void onClick(View v) {
					btnBack(v);
				}
			});
		}else{
			Button btn_vm = (Button) findViewById(R.id.btn_vm);
			btn_vm.setText(R.string.start_string);
			btn_vm.setOnClickListener(new View.OnClickListener() {
				@Override
				public void onClick(View v) {
					btnStart(v);
				}
			});
		}

		mGD = new GestureDetector(SwitchActivity.this, new GestureDetector.SimpleOnGestureListener() {
			@Override
			public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) {
				int dx = (int) (e2.getX() - e1.getX());
				int dy = (int) (e2.getY() - e1.getY());

				if(Math.abs(dx) > 500 && Math.abs(velocityX) > Math.abs(velocityY)){
					if(velocityX > 0){
						btnGroup(null);
					}else{
						btnConnect(null);
					}
					return true;
				}

				if(Math.abs(dy) > 400 && Math.abs(velocityX) < Math.abs(velocityY)){
					if(velocityY <= 0){
						btnUp(null);
						return true;
					}
				}

				return false;
			}

			@Override
			public boolean onDoubleTap(MotionEvent e) {
				if(SystemProperties.get("ro.boot.vm").equals("0")){
					btnStop(null);
					return true;
				}
				return super.onDoubleTap(e);
			}

			@Override
			public void onLongPress(MotionEvent e) {
				super.onLongPress(e);
			}
		});

		LinearLayout main_layout = (LinearLayout)findViewById(R.id.main_layout); 
		main_layout.setClickable(true);  
		main_layout.setOnTouchListener(new OnTouchListener() {
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				return mGD.onTouchEvent(event);
			}
		});
	}

	@Override
	public void onDestroy() {
		releaseWakeLock();
		super.onDestroy();
	}

	public void btnBack(View v){
		BluetoothAdapter blueadapter = BluetoothAdapter.getDefaultAdapter();
		if(blueadapter != null)
			blueadapter.disable();
		NfcAdapter nfcAdapter = NfcAdapter.getDefaultAdapter(SwitchActivity.this);
		if(nfcAdapter != null)
			nfcAdapter.disable();

		try{
			mCellsService.switchCellsVM("host");
		}catch(RemoteException e){
			e.printStackTrace();
		}
		exit();
	}

	public void start(){
		if(mWeiboDialog != null){
			WeiboDialogUtils.closeDialog(mWeiboDialog);
			mWeiboDialog = null;
		}

		BluetoothAdapter blueadapter = BluetoothAdapter.getDefaultAdapter();
		if(blueadapter != null)
			blueadapter.disable();
		NfcAdapter nfcAdapter = NfcAdapter.getDefaultAdapter(SwitchActivity.this);
		if(nfcAdapter != null)
			nfcAdapter.disable();

		if(SystemProperties.get("persist.sys.cell1.init").equals("1")){
			try{
				mCellsService.switchCellsVM("cell1");
			}catch(RemoteException e){
				e.printStackTrace();
			}
			exit();
		}else{
			mHandler.sendEmptyMessage(MSG_START_VM_TOAST_CODE);
		}
	}

	public void btnStart(View v){
		mWeiboDialog = WeiboDialogUtils.createLoadingDialog(SwitchActivity.this, SwitchActivity.this.getString(R.string.starting_string));
		new Thread(new Runnable() {
			@Override
			public void run() {
				if(SystemProperties.get("persist.sys.cell1.init").equals("0"))
				{
					long beginTime=System.currentTimeMillis();
					try{
						mCellsService.untarCellsVM("cell1");
						mCellsService.startCellsVM("cell1");
					}catch(RemoteException e){
						e.printStackTrace();
					}
					long ms = System.currentTimeMillis() - beginTime;
					Log.e(TAG, "Boot image consumption- " + ms + "(ms).");

					int count = 25;
					while(SystemProperties.get("persist.sys.cell1.init").equals("0") && count > 0){
						try {
							Thread.sleep(1000);
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
						count--;
					}

					try {
						Thread.sleep(1000);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}

				mHandler.sendEmptyMessage(MSG_START_VM_CODE);
			}
		}).start();
	}

	public void btnStop(View v){
		if(!SystemProperties.get("persist.sys.cell1.init").equals("1")){
			return;
		}

		try{
			mCellsService.stopCellsVM("cell1");
		}catch(RemoteException e){
			e.printStackTrace();
		}

		try {
			Thread.sleep(1000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}

		Toast.makeText(SwitchActivity.this,"Closed successfully",Toast.LENGTH_SHORT).show();
	}

	public void btnDown(View v){

	}

	public void p2p(){
		String path = FileUtils.CellsPath("hwcell.img");
		if (path != null) {
			final File file = new File(path);
			if (!file.exists()) {
				Toast.makeText(SwitchActivity.this,"File path not found",Toast.LENGTH_SHORT).show();
				return;
			}
			if (mWifiP2pInfo == null) {
				Toast.makeText(SwitchActivity.this,"no connection",Toast.LENGTH_SHORT).show();
				return;
			}
			String md5 = Md5Util.getMd5(file);
			FileBean fileBean = new FileBean(file.getPath(), file.length(), md5);
			String hostAddress = mWifiP2pInfo.groupOwnerAddress.getHostAddress();
			new SendTask(SwitchActivity.this, fileBean).execute(hostAddress);
		}
	}

	public void up(){
		if(mWeiboDialog != null){
			WeiboDialogUtils.closeDialog(mWeiboDialog);
			mWeiboDialog = null;
		}

		Toast.makeText(SwitchActivity.this,"Packaged successfully",Toast.LENGTH_SHORT).show();

		mHandler.sendEmptyMessage(MSG_P2P_CODE);
	}

	public void up_connect_err(){
		if(mWeiboDialog != null){
			WeiboDialogUtils.closeDialog(mWeiboDialog);
			mWeiboDialog = null;
		}

		Toast.makeText(SwitchActivity.this,"no connection",Toast.LENGTH_SHORT).show();
	}

	public void up_send_err(){
		if(mWeiboDialog != null){
			WeiboDialogUtils.closeDialog(mWeiboDialog);
			mWeiboDialog = null;
		}

		Toast.makeText(SwitchActivity.this,"Failed to send",Toast.LENGTH_SHORT).show();
	}

	public void up_send_succ(){
		if(mWeiboDialog != null){
			WeiboDialogUtils.closeDialog(mWeiboDialog);
			mWeiboDialog = null;
		}

		Toast.makeText(SwitchActivity.this,"Sent successfully",Toast.LENGTH_SHORT).show();
	}

	public void btnUp(View v){
		btnConnect(v);
		mWeiboDialog = WeiboDialogUtils.createLoadingDialog(SwitchActivity.this, SwitchActivity.this.getString(R.string.uping_string));
		new Thread(new Runnable() {
			@Override
			public void run() {
				CellsPrivateServiceManager mCellsInitService = new CellsPrivateServiceManager(SwitchActivity.this,
																																		ServiceManager.getService("CellsPrivateService"));

				long beginTime=System.currentTimeMillis();
				try{
					mCellsInitService.tarCellsVM("cell1");
				}catch(RemoteException e){
					e.printStackTrace();
				}
				long ms = System.currentTimeMillis() - beginTime;
				Log.e(TAG, "Package image consumption - " + ms + "(ms).");
				mHandler.sendEmptyMessage(MSG_UP_CODE_PACK);

				if(ms < 1500){
					try {
						Thread.sleep(2000);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}

				int count = 7;
				while(mWifiP2pInfo == null && count > 0){
					try {
						Thread.sleep(1000);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
					count--;
				}
				if (mWifiP2pInfo == null) {
					mHandler.sendEmptyMessage(MSG_UP_CODE_CONNECT_ERR);
					return;
				}

				String path = FileUtils.SendCellsPath("hwcell.img");
				String hostAddress = mWifiP2pInfo.groupOwnerAddress.getHostAddress();
				int ret = 0;
				try{
					ret = mCellsInitService.sendCellsVM(path, hostAddress);
				}catch(RemoteException e){
					e.printStackTrace();
				}
				if(ret == 0)
					mHandler.sendEmptyMessage(MSG_UP_CODE_SEND_SUCC);
				else
					mHandler.sendEmptyMessage(MSG_UP_CODE_SEND_ERR);
			}
		}).start();
	}
}
