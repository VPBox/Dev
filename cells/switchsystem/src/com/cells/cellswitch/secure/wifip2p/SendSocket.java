package com.cells.cellswitch.secure.wifip2p;

import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;

import java.io.File;
import java.io.FileInputStream;
import java.io.ObjectOutputStream;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.io.IOException;

import com.cells.cellswitch.secure.wifip2p.FileBean;

/**
 * description: Socket sent by the client
 */

public class SendSocket {

    public static final String TAG = "SendSocket";
    public static final int PORT = 10000;
    private FileBean mFileBean;
    private String mAddress;
    private File mFile;

    private Socket mServerSocket = null;
    private ObjectOutputStream mObjectOutputStream = null;
    private OutputStream mOutputStream = null;
    private FileInputStream mInputStream = null;

    private Handler mHandler = new Handler(Looper.getMainLooper()) {

        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            switch (msg.what) {
                case 10:
                    int progress = (int) msg.obj;
                    if (mlistener != null) {
                        mlistener.onProgressChanged(mFile, progress);
                    }
                    break;
                case 20:
                    if (mlistener != null) {
                        mlistener.onFinished(mFile);
                    }
                    break;
                case 30:
                    if (mlistener != null) {
                        mlistener.onFaliure(mFile);
                    }
                    break;
            }
        }
    };

    public SendSocket(FileBean fileBean, String address, ProgressSendListener listener) {
        mFileBean = fileBean;
        mAddress = address;
        mlistener = listener;
    }

    public void clear(){
        if (mServerSocket != null) {
            try {
                mServerSocket.close();
                mServerSocket = null;
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        if (mObjectOutputStream != null) {
            try {
                mObjectOutputStream.close();
                mObjectOutputStream = null;
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        if (mInputStream != null) {
            try {
                mInputStream.close();
                mInputStream = null;
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        if (mOutputStream != null) {
            try {
                mOutputStream.close();
                mOutputStream = null;
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public void createSendSocket() {
        try {

            mFile = new File(mFileBean.filePath);
            if (!mFile.exists()) {
                Log.e(TAG, "file does not exist.");
                mHandler.sendEmptyMessage(30);
                return;
            }

            mServerSocket = new Socket();
            InetSocketAddress inetSocketAddress = new InetSocketAddress(mAddress, PORT);
            mServerSocket.connect(inetSocketAddress);

            long beginTime=System.currentTimeMillis();

            mOutputStream = mServerSocket.getOutputStream();

            mInputStream = new FileInputStream(mFile);

            mObjectOutputStream = new ObjectOutputStream(mOutputStream);
            mObjectOutputStream.writeObject(mFileBean);

            long size = mFileBean.fileLength;
            long total = 0;
            byte bytes[] = new byte[4096];
            int len;
            while ((len = mInputStream.read(bytes)) != -1) {
                mOutputStream.write(bytes, 0, len);

                total += len;

                Message message = Message.obtain();
                message.what = 10;
                message.obj = (int)((total * 100) / size);
                mHandler.sendMessage(message);
            }

            long ms = System.currentTimeMillis() - beginTime;
            Log.e(TAG, "Send file consumption- " + ms + "(ms).");

            mHandler.sendEmptyMessage(20);

            clear();

            if (mFile.exists()) {
                mFile.delete();
                Log.e(TAG, "Delete transfer file.");
            }

            Log.e(TAG, "File sent successfully.");
        } catch (Exception e) {
            mHandler.sendEmptyMessage(30);

            clear();

            if (mFile.exists()) {
                mFile.delete();
                Log.e(TAG, "Delete transfer file.");
            }

            Log.e(TAG, "File sending abnormal.");
        }
    }

    /**
     * Monitor sending progress
     */
    private ProgressSendListener mlistener;

    public interface ProgressSendListener {

        //When the transfer progress changes
        void onProgressChanged(File file, int progress);

        //When the transfer ends
        void onFinished(File file);

        //When transmission fails
        void onFaliure(File file);
    }
}

