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
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import com.android.rs.refocus.image.RangeInverseDepthTransform;
import com.android.rs.refocus.image.RangeLinearDepthTransform;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;

public class DepthImage {
    private final String mFormat;
    private final double mFar;
    private final double mNear;
    private final Bitmap mDepthBitmap;
    private final double mBlurAtInfinity;
    private final double mFocalDistance;
    private final double mDepthOfField;
    private final double mFocalPointX;
    private final double mFocalPointY;
    private final DepthTransform mDepthTransform;

    public DepthImage(String format, double far, double near,
                      Bitmap depthBitmap, double blurAtInfinity,
                      double focalDistance, double depthOfField,
                      double focalPointX, double focalPointY,
                      DepthTransform depthTransform) {
        mFormat = format;
        mFar = far;
        mNear = near;
        mDepthBitmap = depthBitmap;
        mBlurAtInfinity = blurAtInfinity;
        mFocalDistance = focalDistance;
        mDepthOfField = depthOfField;
        mFocalPointX = focalPointX;
        mFocalPointY = focalPointY;
        mDepthTransform = depthTransform;
    }

    public static DepthImage createFromXMPMetadata(Context context, Uri image)
            throws IOException {
        InputStream input = context.getContentResolver().openInputStream(image);
        XmpDepthDecode decode = new XmpDepthDecode(input);
        return new DepthImage(decode.getFormat(), decode.getFar(),
                              decode.getNear(), decode.getDepthBitmap(),
                              decode.getBlurAtInfinity(),
                              decode.getFocalDistance(),
                              decode.getDepthOfField(),
                              decode.getFocalPointX(),
                              decode.getFocalPointY(),
                              decode.getDepthTransform());
    }

    public static DepthImage createFromDepthmap(Context context, Uri uriDepthmap)
            throws IOException {
        Bitmap bitmap = BitmapFactory.decodeStream(context.getContentResolver().openInputStream(uriDepthmap));
        if (bitmap == null) {
            throw new FileNotFoundException(uriDepthmap.toString());
        }

        double near = 12.0;
        double far = 120.0;
        DepthTransform transform = new RangeInverseDepthTransform((float)near, (float)far);
        return new DepthImage(RangeInverseDepthTransform.FORMAT,
                              far,
                              near,
                              bitmap, // depthmap
                              5.0,    // blur at ininity
                              15.0,   // focal distance
                              0.1,    // depth of field
                              0.5,    // x of focal point
                              0.5,    // y of focla point
                              transform);
    }

    private static class MinMax {
        public float min;
        public float max;

        public MinMax(float min, float max) {
            this.min = min;
            this.max = max;
        }

        public static MinMax create(float min, float max) {
            return new MinMax(min, max);
        }
    }

    private static MinMax findMinAndMax(final float[] array) {
        float min = array[0];
        float max = min;
        for (int i = 1; i < array.length; i++) {
            final float x = array[i];
            if (x < min) {
                min = x;
            } else if (x > max) {
                max = x;
            }
        }
        return MinMax.create(min, max);
    }

    public static DepthImage createFromPFM(Context context, Uri uriPFM)
            throws IOException {
        PortableFloatMap pfm = new PortableFloatMap(context.getContentResolver().openInputStream(uriPFM));

        final float[] floatPixels = pfm.getPixelArray();
        int[] intPixels = new int[floatPixels.length];
        final MinMax minMax = findMinAndMax(floatPixels);
        final float near = minMax.min;
        final float far  = minMax.max;
        DepthTransform transform = new RangeInverseDepthTransform(near, far);

/*
        android.util.Log.v("DepthImage", "near = " + String.format("%g", near));
        android.util.Log.v("DepthImage", "far  = " + String.format("%g", far));
*/
        int width = pfm.getWidth();
        int height = pfm.getHeight();

        for (int i = 0; i < intPixels.length; i++) {
            int value = transform.quantize(floatPixels[i]) & 0xFF;
            intPixels[i] = value | (value << 8) | (value << 16) | (0xFF << 24);
/*
            if (i >= intPixels.length - width) {
                android.util.Log.v("DepthImage", "float pixel " + i + ":" + String.format("%g", floatPixels[i]));
                android.util.Log.v("DepthImage", "int pixel   " + i + ":" + String.format("0x%02X", intPixels[i]));
            }
*/
        }

        Bitmap bitmap = Bitmap.createBitmap(intPixels, width, height, Bitmap.Config.ARGB_8888);

        // MediaStoreSaver.savePNG(bitmap, "depthmap", "balls", context);

        return new DepthImage(RangeInverseDepthTransform.FORMAT,
                              far,
                              near,
                              bitmap, // depthmap
                              400.0,  // blur at ininity
                              //15.0,  // blur at ininity
                              19.6,   // focal distance
                              //near+(far-near)*0.2,   // focal distance
                              //5.0,    // flocal distance
                              0.005,    // depth of field
                              0.4,    // x of focal point
                              0.4,    // y of focal point
                              transform);
    }

    public Bitmap getDepthBitmap() {
        return mDepthBitmap;
    }

    public DepthTransform getDepthTransform() { return mDepthTransform; }

    public String getFormat() {
        return mFormat;
    }

    public double getFar() {
        return mFar;
    }

    public double getNear() {
        return mNear;
    }

    public double getBlurAtInfinity() {
        return mBlurAtInfinity;
    }

    public double getFocalDistance() {
        return mFocalDistance;
    }

    public double getDepthOfField() {return mDepthOfField; }

    public double getFocalPointX() {
        return mFocalPointX;
    }

    public double getFocalPointY() {
        return mFocalPointY;
    }
}

