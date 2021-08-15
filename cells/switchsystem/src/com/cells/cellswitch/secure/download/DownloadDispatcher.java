package com.cells.cellswitch.secure.download;

import android.support.annotation.NonNull;
import android.util.Log;

import java.io.IOException;
import java.util.ArrayDeque;
import java.util.Deque;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.SynchronousQueue;
import java.util.concurrent.ThreadFactory;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.Response;

public class DownloadDispatcher {
    private static final String TAG = "DownloadDispatcher";
    private static volatile DownloadDispatcher sDownloadDispatcher;
    private static final int CPU_COUNT = Runtime.getRuntime().availableProcessors();
    private static final int THREAD_SIZE = Math.max(3, Math.min(CPU_COUNT - 1, 5));
  
    private static final int CORE_POOL_SIZE = THREAD_SIZE;
   
    private ExecutorService mExecutorService;
    //private final Deque<DownloadTask> readyTasks = new ArrayDeque<>();
    private final Deque<DownloadTask> runningTasks = new ArrayDeque<>();
    //private final Deque<DownloadTask> stopTasks = new ArrayDeque<>();


    private DownloadDispatcher() {
    }

    public static DownloadDispatcher getInstance() {
        if (sDownloadDispatcher == null) {
            synchronized (DownloadDispatcher.class) {
                if (sDownloadDispatcher == null) {
                    sDownloadDispatcher = new DownloadDispatcher();
                }
            }
        }
        return sDownloadDispatcher;
    }

   
    public synchronized ExecutorService executorService() {
        if (mExecutorService == null) {
            mExecutorService = new ThreadPoolExecutor(CORE_POOL_SIZE, Integer.MAX_VALUE, 60, TimeUnit.SECONDS,
                    new SynchronousQueue<Runnable>(), new ThreadFactory() {
                @Override
                public Thread newThread(@NonNull Runnable r) {
                    Thread thread = new Thread(r);
                    thread.setDaemon(false);
                    return thread;
                }
            });
        }
        return mExecutorService;
    }


 
    public void startDownload(final String name, final String url, final DownloadCallback callBack) {
        Call call = OkHttpManager.getInstance().asyncCall(url);
        call.enqueue(new Callback() {
            @Override
            public void onFailure(@NonNull Call call, @NonNull IOException e) {
                callBack.onFailure(e);
            }

            @Override
            public void onResponse(@NonNull Call call, @NonNull Response response) {
           
                long contentLength = response.body().contentLength();
                Log.i(TAG, "contentLength=" + contentLength);
                if (contentLength <= -1) {
                    return;
                }
                DownloadTask downloadTask = new DownloadTask(name, url, THREAD_SIZE, contentLength, callBack);
                downloadTask.init();
                runningTasks.add(downloadTask);
            }
        });
    }


    public void stopDownLoad(String url) {
     
        for (DownloadTask runningTask : runningTasks) {
            if (runningTask.getUrl().equals(url)) {
                runningTask.stopDownload();
            }
        }
    }

 
    public void recyclerTask(DownloadTask downLoadTask) {
        runningTasks.remove(downLoadTask);
      
    }


}
