package com.android.rs.refocus.d1new;

import com.android.rs.refocus.BlurStack;
import com.android.rs.refocus.KernelDataForRenderScript;
import com.android.rs.refocus.LayerInfo;
import com.android.rs.refocus.MediaStoreSaver;
import com.android.rs.refocus.R;
import com.android.rs.refocus.RefocusFilter;
import com.android.rs.refocus.renderscript.ScriptC_layered_filter_fast_d1new;

import android.graphics.Bitmap;
import androidx.renderscript.Allocation;
import androidx.renderscript.RenderScript;
import androidx.renderscript.Script;
import android.util.Log;
/**

/**
 * An accelerated implementation of RefocusFilter using float32 as pixel
 * representation. The corresponding RenderScript class is
 * ScriptC_layered_filter_Fast. Integral image is used for the speedup.
 *
 * Example Usage:
 *
 * {@code RenderScript renderScript = RenderScript.create(context);}
 * {@code RefocusFilterd1new rfFilter = new RefocusFilterd1new(renderScript);}
 * {@code ProgressCallback progress;}
 * {@code Bitmap result = rfFilter.compute(rgbdImage, blurStack, progress);}
 *
 * @author zhl@google.com (Li Zhang)
 */
public class RefocusFilterd1new extends
    RefocusFilter<ScriptC_layered_filter_fast_d1new> {
  private static final String myTAG = "RefocusFilterd1new";
  private static final boolean ENABLE_FAST_FILTER = true;
  private static final float MIN_DISC_RADIUS_FOR_FAST_FILTER = 3;
  boolean useFastFilterForCurrentLayer = false;
  ImageBuffersForRenderScriptd1new buffers;

  public RefocusFilterd1new(RenderScript rs) {
    super(rs);
  }

  @Override
  protected void initializeScriptAndBuffers(Bitmap inputImage,
      LayerInfo focalLayer) {
    scriptC = new ScriptC_layered_filter_fast_d1new(renderScript);

    // Allocates, binds, and initializes buffers that interface between Java
    // and Render Script.
    // + 1 is for the boundary case of using integral image.
    KernelDataForRenderScript.setUseNewRS(true);
    int margin = KernelDataForRenderScript.getMaxKernelRadius() + 1;
    buffers = new ImageBuffersForRenderScriptd1new(inputImage, margin,
        renderScript, scriptC);
    buffers.initializeRenderScript(focalLayer, scriptC);

  }

  @Override
  protected Bitmap extractResultImage() {
    // Extracts the result from .rs file to {@code buffers.outputImage} in Java.
    long startnow;
    long endnow;
    startnow = System.nanoTime();
    scriptC.forEach_PackOutputImage(buffers.outAllocation);
    endnow = System.nanoTime();
    logTiming(myTAG, "PackOutputImage", endnow - startnow);

    startnow = System.nanoTime();
    buffers.outAllocation.copyTo(buffers.outputImage);
    endnow = System.nanoTime();
    logTiming(myTAG, "CopyOutputImage", endnow - startnow);

    return buffers.outputImage;
  }

  /*
   * Utility Method to extract intermediatory result
   */
  private  void extractSharpImage(String name) {

    Bitmap mBitmap = Bitmap.createBitmap(buffers.inputImage.getWidth(),
            buffers.inputImage.getHeight(), Bitmap.Config.ARGB_8888);
    Allocation mAllocation = Allocation.createFromBitmap(renderScript, mBitmap);
    scriptC.forEach_PackSharpImage(mAllocation);

    mAllocation.copyTo(mBitmap);
    MediaStoreSaver.savePNG(mBitmap, "sharpd1new", name, renderScript.getApplicationContext());
  }
  /*
   * Utility Method to extract intermediatory result
   */
  private  void extractFuzzyImage(String name) {

    Bitmap mBitmap = Bitmap.createBitmap(buffers.inputImage.getWidth(),
            buffers.inputImage.getHeight(), Bitmap.Config.ARGB_8888);
    Allocation mAllocation = Allocation.createFromBitmap(renderScript, mBitmap);
    scriptC.forEach_PackFuzzyImage(mAllocation);

    mAllocation.copyTo(mBitmap);
    MediaStoreSaver.savePNG(mBitmap, "fuzzyd1new", name, renderScript.getApplicationContext());
  }

  @Override
  protected void setTargetLayer(LayerInfo layerInfo) {
    scriptC.invoke_SetTargetLayer(layerInfo.frontDepth, layerInfo.backDepth);
  }

  @Override
  protected void setBlendInfo(int dilationRadius) {
    scriptC.invoke_SetBlendInfo(dilationRadius);
  }

  @Override
  protected void setKernelData(int targetLayer, BlurStack blurStack) {
    KernelDataForRenderScriptd1new kernelData =
        new KernelDataForRenderScriptd1new(targetLayer, blurStack, renderScript);

    if (ENABLE_FAST_FILTER
        && kernelData.minDiskRadius > MIN_DISC_RADIUS_FOR_FAST_FILTER) {
      useFastFilterForCurrentLayer = true;
    } else {
      useFastFilterForCurrentLayer = false;
    }
    scriptC.set_g_kernel_stack(kernelData.stackAllocation);
    scriptC.set_galloc_kernel_info(kernelData.infoAllocation);
  }

  @Override
  protected void computeLayerMatteBehindFocalDepth() {
    // Marks active pixels (pixels that are on this target layer);
    // Marks adjacent pixels that are close enough to active pixels;
    long startnow;
    long endnow;

    startnow = System.nanoTime();
    //scriptC.forEach_MarkLayerMask(buffers.inAllocation);
    // Pass sharp meta allocation directly into the kernel
    scriptC.forEach_MarkLayerMaskPassInput(buffers.sharpActualDepthAllocation);
    endnow = System.nanoTime();
    logTiming(myTAG, "MarkLayerMask", endnow - startnow);

    startnow = System.nanoTime();
    //scriptC.forEach_ComputeLayerMatteBehindFocalDepth(buffers.inAllocation);
    // Pass g_sharp_meta into kernel and get updated g_sharp_meta
    scriptC.forEach_ComputeLayerMatteBehindFocalDepthPassInput(buffers.sharpDilatedDepthAllocation, buffers.sharpDilatedDepthAllocation);
    endnow = System.nanoTime();
    logTiming(myTAG, "ComputeLayerMatteBehindFocalDepth", endnow - startnow);
  }

  @Override
  protected void filterLayerBehindFocalDepth() {
    // Filters the target layer and saves the result to {@code g_accum_map} in
    // .rs file.
    long startnow;
    long endnow;

    if (useFastFilterForCurrentLayer) {
      scriptC.invoke_SetUseIntegralImage(1);
      Script.LaunchOptions launchOptions = new Script.LaunchOptions();
      launchOptions.setX(0, 1);
      launchOptions.setY(0, buffers.inputImage.getHeight());

      startnow = System.nanoTime();
      scriptC.forEach_ComputeIntegralImageForLayerBehindFocalDepth(
          buffers.inAllocation, launchOptions);
      endnow = System.nanoTime();
      logTiming(myTAG, "ComputeIntegralImageForLayerBehindFocalDepth", endnow - startnow);
    } else {
      scriptC.invoke_SetUseIntegralImage(0);
    }

    startnow = System.nanoTime();
    //scriptC.forEach_FilterLayerBehindFocalDepth(buffers.inAllocation);
    // Pass g_fuzzy_RGBA into kernel and get g_fuzzy_RGBA as output
    scriptC.forEach_FilterLayerBehindFocalDepthPassInput(buffers.fuzzyRGBAAllocation, buffers.fuzzyRGBAAllocation);
    endnow = System.nanoTime();
    logTiming(myTAG, "FilterLayerBehindFocalDepth", endnow - startnow);

    //extractFuzzyImage("fuzzy_behind");
    //extractSharpImage("sharp_behind");
  }

  @Override
  protected void updateSharpImageUsingFuzzyImage() {
    long startnow;
    long endnow;

    startnow = System.nanoTime();

    //scriptC.forEach_UpdateSharpImageUsingFuzzyImage(buffers.inAllocation);
    // Pass input and output version of UpdateSharpImageUsingFuzzyImage
    scriptC.forEach_UpdateSharpUsingFuzzyPassInput(buffers.sharpDilatedDepthAllocation, buffers.sharpDilatedDepthAllocation);
    endnow = System.nanoTime();
    logTiming(myTAG, "UpdateSharpImageUsingFuzzyImage", endnow - startnow);

    //extractSharpImage("sharp_update");
  }


  @Override
  protected void computeLayerMatteInFrontOfFocalDepth() {
    // Marks active pixels (pixels that are on this target layer);
    // Marks adjacent pixels that are close enough to active pixels;
    long startnow;
    long endnow;

    startnow = System.nanoTime();
    //scriptC.forEach_MarkLayerMask(buffers.inAllocation);
    // Pass sharp meta allocation directly into the kernel
    scriptC.forEach_MarkLayerMaskPassInput(buffers.sharpActualDepthAllocation);
    endnow = System.nanoTime();
    logTiming(myTAG, "MarkLayerMask", endnow - startnow);

    startnow = System.nanoTime();
    //scriptC.forEach_ComputeLayerMatteInFrontOfFocalDepth(buffers.inAllocation);
    // Pass g_sharp_meta and g_fuzzy_RGBA directly into the kernel
    scriptC.forEach_ComputeLayerMatteInFrontOfFocalDepthPassInput(buffers.sharpDilatedDepthAllocation, buffers.sharpDilatedDepthAllocation);
    endnow = System.nanoTime();
    logTiming(myTAG, "ComputeLayerMatteInFrontOfFocalDepth", endnow - startnow);
  }

  @Override
  protected void filterLayerInFrontOfFocalDepth() {
    // Filters the target layer and accumulates the result to {@code
    // g_accum_map} in .rs file.
    long startnow;
    long endnow;
    if (useFastFilterForCurrentLayer) {
      scriptC.invoke_SetUseIntegralImage(1);
      Script.LaunchOptions launchOptions = new Script.LaunchOptions();
      launchOptions.setX(0, 1);
      launchOptions.setY(0, buffers.inputImage.getHeight());

      startnow = System.nanoTime();
      scriptC.forEach_ComputeIntegralImageForLayerInFrontOfFocalDepth(
          buffers.inAllocation, launchOptions);
      endnow = System.nanoTime();
      logTiming(myTAG, "ComputeIntegralImageForLayerInFrontOfFocalDepth", endnow - startnow);
    } else {
      scriptC.invoke_SetUseIntegralImage(0);
    }
    startnow = System.nanoTime();
    //scriptC.forEach_FilterLayerInFrontOfFocalDepth(buffers.inAllocation);
    // Pass g_sharp_dilated_depth into kernel and get g_fuzzy_RGBA as output
    scriptC.forEach_FilterLayerInFrontOfFocalDepthPassInput(buffers.sharpDilatedDepthAllocation, buffers.sharpDilatedDepthAllocation);
    endnow = System.nanoTime();
    logTiming(myTAG, "FilterLayerInFrontOfFocalDepth", endnow - startnow);

    //extractFuzzyImage("fuzzy_front");
    //extractSharpImage("sharp_front");
  }

  @Override
  protected void finalizeFuzzyImageUsingSharpImage() {
    // Blends {@code g_accum_map} and {@code g_focus_map} in .rs file.
    // Saves the result in {@code g_accum_map}.
    long startnow;
    long endnow;
    startnow = System.nanoTime();
    scriptC.forEach_FinalizeFuzzyImageUsingSharpImage(buffers.inAllocation);
    //scriptC.forEach_FinalizeFuzzyImageUsingSharpImagePassInput(buffers.sharpActualDepthAllocation, buffers.fuzzyRGBAAllocation);
    endnow = System.nanoTime();
    logTiming(myTAG, "FinalizeFuzzyImageUsingSharpImage", endnow - startnow);
  }
}
