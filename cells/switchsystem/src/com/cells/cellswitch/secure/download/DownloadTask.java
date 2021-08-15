package com.cells.cellswitch.secure.download;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

public class DownloadTask {
    private static final String TAG = "DownloadTask";
    
    private String url;
    
    private String name;
    
    private long mContentLength;
    
    private int mThreadSize;
    
    private volatile int mSuccessNumber;
   
    private long mTotalProgress;
    private List<DownloadRunnable> mDownloadRunnables;
    private DownloadCallback mDownloadCallback;


    public DownloadTask(String name, String url, int threadSize, long contentLength, DownloadCallback callBack) {
        this.name = name;
        this.url = url;
        this.mThreadSize = threadSize;
        this.mContentLength = contentLength;
        this.mDownloadRunnables = new ArrayList<>();
        this.mDownloadCallback = callBack;
    }

    public void init() {
        for (int i = 0; i < mThreadSize; i++) {
          
          
            long threadSize = mContentLength / mThreadSize;
      
            long start = i * threadSize;
         
            long end = start + threadSize - 1;
            if (i == mThreadSize - 1) {
                end = mContentLength - 1;
            }
            DownloadRunnable downloadRunnable = new DownloadRunnable(name, url, mContentLength, i, start, end, new DownloadCallback() {
                @Override
                public void onFailure(Exception e) {
                   
                    mDownloadCallback.onFailure(e);
                    stopDownload();
                }

                @Override
                public void onSuccess(File file) {
                    mSuccessNumber = mSuccessNumber + 1;
                    if (mSuccessNumber == mThreadSize) {
                        mDownloadCallback.onSuccess(file);
                        DownloadDispatcher.getInstance().recyclerTask(DownloadTask.this);
                       
                    }
                }

                @Override
                public void onProgress(long progress, long currentLength) {
                  
                    synchronized (DownloadTask.this) {
                        mTotalProgress = mTotalProgress + progress;
                        //Log.i(TAG, "mTotalProgress==" + mTotalProgress);
                        mDownloadCallback.onProgress(mTotalProgress, currentLength);
                    }
                }

                @Override
                public void onPause(long progress, long currentLength) {
                    mDownloadCallback.onPause(progress,currentLength);
                }
            });
           
            DownloadDispatcher.getInstance().executorService().execute(downloadRunnable);
            mDownloadRunnables.add(downloadRunnable);
        }
    }


    public void stopDownload() {
        for (DownloadRunnable runnable : mDownloadRunnables) {
            runnable.stop();
        }
    }

    public String getUrl() {
        return url;
    }
}
