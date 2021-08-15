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

import android.content.ContentResolver;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.net.Uri;
import android.util.Log;
import com.android.rs.refocus.image.RangeInverseDepthTransform;
import java.io.FileNotFoundException;
import java.io.IOException;

/**
 * An RGBZ image, where Z stands for depth, i.e. a color+depth image.
 * The RGBZ always has a preview image, which represents the latest rendering of the RGBZ.
 * The preview is encoded as the normal jpeg content for client compatibility,
 * while the color channel and depth channels are encoded as XMP data.
 * The class supports lazy initialization where the XMP meta data is loaded only when first
 * accessed.
 */
public class RGBZ {
  public static final String TAG = "RGBZ";

  private Bitmap bitmap;
  private Bitmap preview;
  private Bitmap depthBitmap;
  private DepthTransform depthTransform;
  private DepthImage depthImage;

  /**
   * Creates an RGBZ from a content uri.
   *
   * @param uri The uri name of the RGBZ
   * @throws FileNotFoundException if the RGBZ could not be read
   */
  public RGBZ(Uri uri, ContentResolver contentResolver, Context context) throws IOException {
    preview = BitmapFactory.decodeStream(contentResolver.openInputStream(uri));
    if (preview == null) {
      throw new FileNotFoundException(uri.toString());
    }
    depthImage = DepthImage.createFromXMPMetadata(context, uri);
    depthBitmap = depthImage.getDepthBitmap();
    //MediaStoreSaver.savePNG(depthBitmap, "depthmap", "depthmap", context);
    bitmap = setAlphaChannel(preview, depthBitmap);
    depthTransform = depthImage.getDepthTransform();
  }

  /**
   * Creates an RGBZ from uris to an image and a depthmap.
   *
   * @param uriImage The uri name of the image
   * @param uriDepthmap The uri name of the depthmap
   * @throws FileNotFoundException if the RGBZ could not be read
   */
  public RGBZ(Uri uriImage, Uri uriDepthmap, ContentResolver contentResolver,
              Context context) throws IOException {
    preview = BitmapFactory.decodeStream(contentResolver.openInputStream(uriImage));
    if (preview == null) {
      throw new FileNotFoundException(uriImage.toString());
    }
    depthImage = DepthImage.createFromDepthmap(context, uriDepthmap);
    depthBitmap = depthImage.getDepthBitmap();
    bitmap = setAlphaChannel(preview, depthBitmap);
    depthTransform = depthImage.getDepthTransform();
  }


    public RGBZ(Bitmap image, DepthImage depthImage) {
        preview = image;
        this.depthImage = depthImage;
        depthBitmap = depthImage.getDepthBitmap();
        bitmap = setAlphaChannel(preview, depthBitmap);
        depthTransform = depthImage.getDepthTransform();
    }

    public static RGBZ createFromBitmapDepthmap(Uri uriImage, Uri uriDepthmap,
                                                ContentResolver contentResolver, Context context)
            throws IOException {
        Bitmap image = BitmapFactory.decodeStream(contentResolver.openInputStream(uriImage));
        if (image == null) {
            throw new FileNotFoundException(uriImage.toString());
        }
        DepthImage depthImage = DepthImage.createFromDepthmap(context, uriDepthmap);
        return new RGBZ(image, depthImage);
    }

    public static RGBZ createFromPFMDepthmap(Uri uriImage, Uri uriDepthmap,
                                             ContentResolver contentResolver, Context context)
            throws IOException {
        Bitmap image = BitmapFactory.decodeStream(contentResolver.openInputStream(uriImage));
        if (image == null) {
            throw new FileNotFoundException(uriImage.toString());
        }
        DepthImage depthImage = DepthImage.createFromPFM(context, uriDepthmap);
        MediaStoreSaver.savePNG(depthImage.getDepthBitmap(), "depthmap",
                                "depthmap", context);
        return new RGBZ(image, depthImage);
    }

  /**
   * @return Whether the RGBZ has a depth channel
   */
  public boolean hasDepthmap() {
    return depthTransform != null;
  }

  /**
   * @return The color+depth {@code Bitmap}
   */
  public Bitmap getBitmap() {
    return bitmap;
  }

  /**
   * @return The depthmap component of this RGBZ
   */
  public DepthTransform getDepthTransform() {
    return depthTransform;
  }

  public double getFocusDepth() {
    return this.depthImage.getFocalDistance();
  }

  public double getDepthOfField() {
    return this.depthImage.getDepthOfField();
  }

  public double getBlurInfinity() {
    return this.depthImage.getBlurAtInfinity();
  }

  /**
   * @return the width of this {@code RGBZ}
   */
  public int getWidth() {
    return bitmap.getWidth();
  }

  /**
   * @return the height of this {@code RGBZ}
   */
  public int getHeight() {
    return bitmap.getHeight();
  }

  /**
   * @return the depth value of the given pixel
   */

  public float getDepth(int x, int y) {
    if (!hasDepthmap()) {
      return 0.0f;
    }
    if (x < 0 || x > depthBitmap.getWidth() ||
            y < 0 || y > depthBitmap.getHeight()) {
      Log.e("RGBZ getDepth", "index out of bound");
      return 0;
    }
    return getDepthTransform().reconstruct(Color.blue(depthBitmap.getPixel(x, y)));
  }

  /**
   * Sets the depthmap as the alpha channel of the {@code Bitmap}.
   */
  public Bitmap setAlphaChannel(Bitmap bitmap, Bitmap depthBitmap) {
    if (bitmap == null) {
      return bitmap;
    }
    Bitmap result = bitmap.copy(Bitmap.Config.ARGB_8888, true);
    // set the alpha channel of depthBitmap to alpha of bitmap
    result = setAlphaChannelFromBitmap(depthBitmap, bitmap, result);
    return result;
  }

  private Bitmap setAlphaChannelFromBitmap(Bitmap depth, Bitmap orig, Bitmap dest) {
    int w = orig.getWidth();
    int h = orig.getHeight();
    int[] orig_data = new int[w*h];
    int[] depth_data = new int[w*h];

    orig.getPixels(orig_data, 0, w, 0, 0, w, h);
    depth.getPixels(depth_data, 0, w, 0, 0, w, h);
    for (int i = 0; i < orig_data.length; i++) {
      int v = orig_data[i] & 0x00FFFFFF;
      int temp = (depth_data[i] & 0x000000FF) << 24;
      v = v | temp;
      orig_data[i] = v;
    }
    dest.setPixels(orig_data, 0, w, 0, 0, w, h);
    return dest;
  }
}

