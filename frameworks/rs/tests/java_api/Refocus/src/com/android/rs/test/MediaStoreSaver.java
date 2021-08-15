/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.rs.refocus;

import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Build;
import android.os.Environment;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Utility class to save images into android image database
 */
public class MediaStoreSaver {
    public static final String savePNG(Bitmap bitmap,
                                       String folderName,
                                       String imageName,
                                        Context mContext) {
        File folder = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES);
        String folder_path = folder.getAbsolutePath();
        String file_path = folder_path + "/" + folderName;
        File dir = new File(file_path);

        if (!dir.exists()) {
            System.out.println("make directory: " + dir.getAbsolutePath());
            dir.mkdirs();
        }
        //int n = dir.listFiles().length;
        //File file = new File(dir, imageName + n + ".png");
        File file = new File(dir, imageName + ".png");
        try {
            FileOutputStream fOut = new FileOutputStream(file);
            bitmap.compress(Bitmap.CompressFormat.PNG, 0, fOut);
            System.out.println("saved image: " + file.getAbsolutePath());
            fOut.flush();
            fOut.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

        // MediaStorageScan(mContext, file);
        return file.getAbsolutePath();
    }
    /*
     * Refresh image files to view them on computer
     */
    private static void MediaStorageScan(Context context, final File file) {
        final Uri fileUri = Uri.fromFile(file);

        // New way
        if (android.os.Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH) {
            context.sendBroadcast(new Intent("android.hardware.action.NEW_PICTURE", fileUri));
        }

        // Keep compatibility
        context.sendBroadcast(new Intent("com.android.camera.NEW_PICTURE", fileUri));

        // Usual way
        final Intent intent = new Intent(Intent.ACTION_MEDIA_SCANNER_SCAN_FILE);
        intent.setData(fileUri);
        context.sendBroadcast(intent);
    }

}
