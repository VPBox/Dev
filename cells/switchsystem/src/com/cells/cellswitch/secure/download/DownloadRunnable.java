package com.cells.cellswitch.secure.download;

import android.os.Environment;
import android.util.Log;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.RandomAccessFile;

import okhttp3.Response;

public class DownloadRunnable implements Runnable {
    private static final String TAG = "DownloadRunnable";
    private static final int STATUS_DOWNLOADING = 1;
    private static final int STATUS_STOP = 2;
    
    private int mStatus = STATUS_DOWNLOADING;
    
    private String url;
    
    private String name;
    
    private int threadId;
    
    private long start;
   
    private long end;
   
    private long mProgress;
   
    private long mCurrentLength;
    private DownloadCallback downloadCallback;

    public DownloadRunnable(String name, String url, long currentLength, int threadId, long start, long end, DownloadCallback downloadCallback) {
        this.name = name;
        this.url = url;
        this.mCurrentLength = currentLength;
        this.threadId = threadId;
        this.start = start;
        this.end = end;
        this.downloadCallback = downloadCallback;
    }

    @Override
    public void run() {
        InputStream inputStream = null;
        RandomAccessFile randomAccessFile = null;
        try {
            Response response = OkHttpManager.getInstance().syncResponse(url, start, end);
            Log.i(TAG, "fileName=" + name + " contentLength=" + response.body().contentLength()
                    + " start=" + start + "end=" + end + " threadId=" + threadId);
            inputStream = response.body().byteStream();
        
            File file = new File(Environment.getExternalStorageDirectory().getAbsolutePath(), name);
            randomAccessFile = new RandomAccessFile(file, "rwd");
          
            randomAccessFile.seek(start);
            int length;
            byte[] bytes = new byte[10 * 1024];
            while ((length = inputStream.read(bytes)) != -1) {
                if (mStatus == STATUS_STOP) {
                    downloadCallback.onPause(length, mCurrentLength);
                    break;
                }
             
                randomAccessFile.write(bytes, 0, length);
             
                mProgress = mProgress + length;
             
                downloadCallback.onProgress(length, mCurrentLength);
            }
            downloadCallback.onSuccess(file);
        } catch (IOException e) {
            e.printStackTrace();
            downloadCallback.onFailure(e);
        } finally {
            Utils.close(inputStream);
            Utils.close(randomAccessFile);
         
        }
    }

    public void stop() {
        mStatus = STATUS_STOP;
    }
}
