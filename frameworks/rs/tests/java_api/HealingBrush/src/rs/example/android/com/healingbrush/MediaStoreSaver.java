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

package rs.example.android.com.healingbrush;

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

public class MediaStoreSaver {
    public static final byte TYPE_PNG = 2;
    public static final byte TYPE_JPG = 3;

    public static final String save(Bitmap bitmap,
                                    String folderName,
                                    String imageName,
                                    Context mContext,
                                    byte imageType) {
        File folder = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES);
        String folder_path = folder.getAbsolutePath();
        String file_path = folder_path + "/" + folderName;
        File dir = new File(file_path);

        if (!dir.exists()) {
            dir.mkdirs();
        }
        File file = null;
        try {
            String suffix = ".png";
            Bitmap.CompressFormat format = Bitmap.CompressFormat.PNG;
            if (imageType == TYPE_JPG) {
                suffix = ".jpg";
                format = Bitmap.CompressFormat.JPEG;
            }

            file = File.createTempFile(imageName, suffix, dir);
            FileOutputStream fOut = new FileOutputStream(file);
            bitmap.compress(format, 100, fOut);
            System.out.println("saved image: " + file.getAbsolutePath());
            fOut.flush();
            fOut.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

        MediaStorageScan(mContext, file);
        return file.getAbsolutePath();
    }

    /*
     * Refresh image files to view them on computer
     */
    private static void MediaStorageScan(Context context, final File file) {
        final Uri fileUri = Uri.fromFile(file);

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH) {
            context.sendBroadcast(new Intent("android.hardware.action.NEW_PICTURE", fileUri));
        }

        context.sendBroadcast(new Intent("com.android.camera.NEW_PICTURE", fileUri));

        final Intent intent = new Intent(Intent.ACTION_MEDIA_SCANNER_SCAN_FILE);
        intent.setData(fileUri);
        context.sendBroadcast(intent);
    }

}
