package com.android.rs.refocus;

import android.graphics.Bitmap;
import androidx.renderscript.Allocation;
import androidx.renderscript.RenderScript;

/**
 * A class that manages the image buffers that interface between Java and Render
 * Script. This class will be specialized for float in f32 package and for byte
 * in u8 package.
 *
 * @author zhl@google.com (Li Zhang)
 */
public class ImageBuffersForRenderScript {
  /**
   * Input and output images and their corresponding Allocation to interface
   * with Render Script. Both input and output images are unpadded images.
   */
  public Bitmap inputImage;
  public Bitmap outputImage;
  public Allocation inAllocation;
  public Allocation outAllocation;

  /**
   * The following three member variables are used in the subclasses that extend
   * this class. Therefore, they are protected.
   */
  public int imageWidthPadded;
  public int imageHeightPadded;
  public int paddedMargin;

  public ImageBuffersForRenderScript(Bitmap inImage, int margin,
      RenderScript renderScript) {
    inputImage = inImage;
    inAllocation = Allocation.createFromBitmap(renderScript, inputImage);

    outputImage = Bitmap.createBitmap(inputImage.getWidth(),
        inputImage.getHeight(), Bitmap.Config.ARGB_8888);
    outAllocation = Allocation.createFromBitmap(renderScript, outputImage);

    paddedMargin = margin;
    imageWidthPadded = inputImage.getWidth() + 2 * margin;
    imageHeightPadded = inputImage.getHeight() + 2 * margin;
  }
}
