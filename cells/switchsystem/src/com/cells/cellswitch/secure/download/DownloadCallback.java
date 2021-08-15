package com.cells.cellswitch.secure.download;

import java.io.File;

public interface DownloadCallback {
    /**
     * 
     *
     * @param file
     */
    void onSuccess(File file);

    /**
     * 
     *
     * @param e
     */
    void onFailure(Exception e);

    /**
     * 
     *
     * @param progress
     */
    void onProgress(long progress, long currentLength);

    /**
     * 
     *
     * @param progress
     * @param currentLength
     */
    void onPause(long progress, long currentLength);
}
