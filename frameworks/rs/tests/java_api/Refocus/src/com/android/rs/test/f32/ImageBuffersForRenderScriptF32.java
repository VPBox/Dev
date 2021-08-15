package com.android.rs.refocus.f32;

import android.graphics.Bitmap;
import androidx.renderscript.Allocation;
import androidx.renderscript.Element;
import androidx.renderscript.RenderScript;
import android.util.Log;

import com.android.rs.refocus.renderscript.ScriptC_layered_filter_fast_f32;

import com.android.rs.refocus.LayerInfo;
import com.android.rs.refocus.ImageBuffersForRenderScript;

/**
 * A class that manages the image buffers that interface between Java and Render
 * Script. These buffers are specialized for float32 pixel representation.
 *
 * @author zhl@google.com (Li Zhang)
 */
public class ImageBuffersForRenderScriptF32 extends
        ImageBuffersForRenderScript {
  /**
   * SharpImage, FuzzyImage, and integralImage that are bound with memory in
   * Render Script for layered filtering.
   */
  private static final String myTAG = "RefocusFilterF32";
  public Allocation sharpImageAllocation;
  public Allocation fuzzyImageAllocation;
  public Allocation integralImageAllocation;

  /**
   * A constructor that allocates memory buffers in Java and binds the buffers
   * with the global pointers in the Render Script.
   *
   * @param image an input (padded) RGBD image
   * @param renderScript a RenderScript object that manages the {@code Context}
   *        in Java
   * @param scriptC a RenderScript object that manages the filtering kernel
   *        functions in .rs file
   */
  public ImageBuffersForRenderScriptF32(Bitmap image, int margin,
      RenderScript renderScript, ScriptC_layered_filter_fast_f32 scriptC) {
    super(image, margin, renderScript);

    sharpImageAllocation = Allocation.createSized(
        renderScript, Element.F32_4(renderScript),
        imageWidthPadded * imageHeightPadded);

    fuzzyImageAllocation = Allocation.createSized(
        renderScript, Element.F32_4(renderScript),
        imageWidthPadded * imageHeightPadded);

    integralImageAllocation = Allocation.createSized(renderScript,
        Element.F32_4(renderScript), imageWidthPadded * imageHeightPadded);

    scriptC.bind_g_sharp_image_buffer(sharpImageAllocation);
    scriptC.bind_g_fuzzy_image_buffer(fuzzyImageAllocation);
    scriptC.bind_g_integral_image_buffer(integralImageAllocation);
  }

  /**
   * A function that passes global parameters from Java to Render Script and
   * sets up the input image.
   *
   * @param focalLayer a layer for the depth value interval that has zero blur.
   * @param scriptC a RenderScript object that manages filtering kernels and
   *        global variables in .rs file
   */
  public void initializeRenderScript(LayerInfo focalLayer,
      ScriptC_layered_filter_fast_f32 scriptC) {
    long startnow;
    long endnow;

    startnow = System.nanoTime();
    scriptC.invoke_InitializeF32(imageWidthPadded, imageHeightPadded,
        paddedMargin, focalLayer.frontDepth, focalLayer.backDepth);
    endnow = System.nanoTime();
    Log.d(myTAG, "Initialize: " + (endnow - startnow) + " ns");
    // At this point, {@code inAllocation} contains input RGBD image in Java.
    // {@code g_sharp_image} is a global pointer that points the focus image in
    // Render Script. The following function copies {@code inAllocation} in
    // Java to {@code g_sharp_image) in Render Script.
    startnow = System.nanoTime();
    scriptC.forEach_UnpackInputImage(inAllocation);
    endnow = System.nanoTime();
    Log.d(myTAG, "UnpackInputImage: " + (endnow - startnow) + " ns");
  }
}
