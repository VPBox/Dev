package com.android.rs.refocus.d1new;

import android.graphics.Bitmap;
import androidx.renderscript.Allocation;
import androidx.renderscript.Element;
import androidx.renderscript.RenderScript;
import android.util.Log;

import com.android.rs.refocus.renderscript.ScriptC_layered_filter_fast_d1new;

import com.android.rs.refocus.LayerInfo;
import com.android.rs.refocus.ImageBuffersForRenderScript;

/**
 * A class that manages the image buffers that interface between Java and Render
 * Script. These buffers are specialized for float32 pixel representation.
 *
 * @author zhl@google.com (Li Zhang)
 */
public class ImageBuffersForRenderScriptd1new extends
        ImageBuffersForRenderScript {
  /**
   * SharpImage, FuzzyImage, and integralImage that are bound with memory in
   * Render Script for layered filtering.
   * Global allocation for images and meta data that are bound with memory in Render Script
   *
   */
  private static final String myTAG = "RefocusFilterd1new";
  public Allocation sharpRGBAAllocation;
  public Allocation fuzzyRGBAAllocation;
  public Allocation integralRGBAAllocation;

  public Allocation sharpActualDepthAllocation;
  public Allocation sharpDilatedDepthAllocation;
  public Allocation sharpActiveAllocation;
  public Allocation sharpMatteAllocation;

  /**
   * A constructor that allocates memory buffers in Java and binds the buffers
   * with the global pointers in the Render Script.
   *
   * @param image an input (padded) RGBD image
   * @param renderScript a RenderScript object that manages the {@code Context}
   * in Java
   * @param scriptC a RenderScript object that manages the filtering kernel
   *                functions in .rs file
   */
  public ImageBuffersForRenderScriptd1new(Bitmap image, int margin,
                                          RenderScript renderScript, ScriptC_layered_filter_fast_d1new scriptC) {
    super(image, margin, renderScript);
    sharpRGBAAllocation = Allocation.createSized(
            renderScript, Element.F32_4(renderScript),
            imageWidthPadded * imageHeightPadded);
    sharpActualDepthAllocation = Allocation.createSized(
            renderScript, Element.U8(renderScript),
            imageWidthPadded * imageHeightPadded);
    sharpDilatedDepthAllocation = Allocation.createSized(
            renderScript, Element.U8(renderScript),
            imageWidthPadded * imageHeightPadded);
    sharpActiveAllocation = Allocation.createSized(
            renderScript, Element.U8(renderScript),
            imageWidthPadded * imageHeightPadded);
    sharpMatteAllocation = Allocation.createSized(
            renderScript, Element.U8(renderScript),
            imageWidthPadded * imageHeightPadded);
    fuzzyRGBAAllocation = Allocation.createSized(
            renderScript, Element.F32_4(renderScript),
            imageWidthPadded * imageHeightPadded);
    integralRGBAAllocation = Allocation.createSized(
            renderScript, Element.F32_4(renderScript),
            imageWidthPadded * imageHeightPadded);

    scriptC.set_g_sharp_RGBA(sharpRGBAAllocation);
    scriptC.set_g_fuzzy_RGBA(fuzzyRGBAAllocation);
    scriptC.set_g_integral_RGBA(integralRGBAAllocation);

    scriptC.set_g_sharp_actual_depth(sharpActualDepthAllocation);
    scriptC.set_g_sharp_active(sharpActiveAllocation);
    scriptC.set_g_sharp_matte(sharpMatteAllocation);
    scriptC.set_g_sharp_dilated_depth(sharpDilatedDepthAllocation);

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
                                     ScriptC_layered_filter_fast_d1new scriptC) {
    long startnow;
    long endnow;
    startnow = System.nanoTime();
    scriptC.invoke_InitializeFast(imageWidthPadded, imageHeightPadded,
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
