/*
 * Copyright (C) 2016 The Android Open Source Project
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

import android.graphics.Bitmap;
import android.graphics.Color;
import java.lang.Math;

public class ImageCompare {

    /**
     * Compute the luma channel of an RGB image, i.e., the Y channel of the
     * equivalent YCbCr image.
     * https://en.wikipedia.org/wiki/YCbCr
     */
    private static double luma(int pixel) {
        final int R = Color.red(pixel);
        final int G = Color.green(pixel);
        final int B = Color.blue(pixel);
        return 0.299 * R + 0.587 * G + 0.114 * B;
    }

    /**
     * Compute peak signal-to-noise ration (PSNR) between two images
     * The greater the value of psnr, the closer the two images are to each
     * other. For identical images, psnr = +infinity.
     * For 8-bit images, a psnr above 50 is commonly acceptable for a lossy
     * conversion.
     *
     * References:
     * http://www.mathworks.com/help/vision/ref/psnr.html
     * https://en.wikipedia.org/wiki/Peak_signal-to-noise_ratio
     */
    public static double psnr(Bitmap bitmap1, Bitmap bitmap2) {
        if (bitmap1.getWidth() != bitmap2.getWidth() ||
            bitmap1.getHeight() != bitmap2.getHeight()) {
            throw new RuntimeException("images were of diffrent size");
        }

        if (bitmap1.sameAs(bitmap2)) {
            android.util.Log.i("RefocusTest",
                               "bitmaps verified to be identical in fast path.");
            return Double.POSITIVE_INFINITY;
        }

        final int width = bitmap1.getWidth();
        final int height = bitmap1.getHeight();
        final int numPixels = width * height;

        double noise = 0;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int pixel1 = bitmap1.getPixel(x, y);
                int pixel2 = bitmap2.getPixel(x, y);
                if (pixel1 != pixel2) {
                    final double Y1 = luma(pixel1);
                    final double Y2 = luma(pixel2);
                    noise += (Y1 - Y2) * (Y1 - Y2);
                }
            }
        }

        final double mse = noise / numPixels;
        final double psnr = 20 * Math.log10(255) - 10 * Math.log10(mse);
        return psnr;
    }
}
