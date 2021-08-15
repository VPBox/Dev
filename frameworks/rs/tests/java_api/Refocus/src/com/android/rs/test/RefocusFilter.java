package com.android.rs.refocus;

import android.graphics.Bitmap;
import androidx.renderscript.RenderScript;
import android.util.Log;
import android.util.Pair;
import java.util.ArrayList;

/**
 * An abstract class that implements refocus filtering using Render Script. The
 * main function is {@code compute}. All other functions and data structures are
 * supporting this main function. Subclasses need to implement individual steps
 * based on pixel representation, e.g., uint8 or float32.
 *
 * @param <ScriptType> pixel representation, which can be float of byte.
 *
 * @author zhl@google.com (Li Zhang)
 */
public abstract class RefocusFilter<ScriptType> {
  //private static final Log.Tag TAG = new Log.Tag("RefocusFilter");
  protected static final String TAG = "RefocusFilter";
  // Render Script context.
  protected RenderScript renderScript;

  // Script functions in .rs file.
  protected ScriptType scriptC;

  // Timing info for each phase and total computation
  public ArrayList<Pair<String,Long>> timings = new ArrayList<Pair<String,Long>>();

  public void logTiming(String implementation, String phase, long time) {
      logTiming(implementation, phase, time, "ns");
  }

  public void logTiming(String implementation, String phase, long time, String unit) {
      timings.add(Pair.create(phase + " (" + unit + ")", Long.valueOf(time)));
      Log.d(implementation, phase + ":" + time + " " + unit);
  }

  /*
   * A constructor that initializes the class.
   *
   * @param rs the Render Script context.
   */
  public RefocusFilter(RenderScript rs) {
    renderScript = rs;
  }

  /*
   * A function that implements refocus filtering using Render Script.
   *
   * @param inputImage an RGBD image. RGB channels of the input image form the
   * color image. The D channel has a range of [1,BlurStack.MAX_DEPTH], where 0
   * is reserved for invalid padded pixels. Depth here refers to inverse depth
   * (i.e., disparity), where larger depths are closer to the camera.
   *
   * @param blurStack an object that has all the parameters for refocus
   * filtering, including: the number of blending layers, the depth levels in
   * each blending layer, focal depth, etc. For details, please refer to the
   * definition of {@code BlurStack}.
   *
   * @return a {@code Bitmap} of the filtering result
   */
  /*
    Commented out for now to define in derived classes
    so that images after each stage could be extracted
   */
  protected Bitmap compute(Bitmap inputImage, BlurStack blurStack) {

    // Initializes {@code scriptC} and allocates required memory buffers
    // (defined in subclasses) that interface between Java and Render Script.
    initializeScriptAndBuffers(inputImage,
        blurStack.getLayerInfo(blurStack.getFocusLayer()));

    // Processes layers from back-most to focal depth (including the focal
    // depth).
    if (!processLayersFromBackToFocus(blurStack)) {
      return null;
    }

    // Processes layers from front-most to focal depth (excluding the focal
    // depth).
    if (!processLayersFromFrontToFocus(blurStack)) {
      return null;
    }

    // Extracts the result from .rs file to Java.
    Bitmap resultImage = extractResultImage();
    renderScript.finish();

    Log.d(TAG, "filterAndBlendAllLayersUsingKernel is finished");
    return resultImage;
  }

  /*
   * Process layers from back-most to focal depth (including the focal depth).
   */
  protected boolean processLayersFromBackToFocus(BlurStack blurStack) {
    for (int targetLayer = blurStack.getNumLayers() - 1;
        targetLayer >= blurStack.getFocusLayer(); --targetLayer) {
      // Sets up target layer info in Render Script.
      LayerInfo layerInfo = blurStack.getLayerInfo(targetLayer);
      setTargetLayer(layerInfo);

      // For a layer that is behind the focal depth, its back depth has the
      // largest blur kernel radius. Uses the kernel radius as dilation radius
      // of this layer.
      int dilationRadius = getKernelRadius(layerInfo.backDepth, blurStack);
      setBlendInfo(dilationRadius);

      // Sends blur kernel matrix data to Render Script.
      setKernelData(targetLayer, blurStack);

      // Marks active pixels (pixels that on this layer).
      // Marks pixels that are close enough (within dilationRadius) to the
      // active pixels.
      // Computes distance transform of the active pixels in their neighborhood
      // and use the distance value as matte for layer blending later.
      computeLayerMatteBehindFocalDepth();

      // Computes filtering for pixels on the target layer and saves the
      // filtering result in a buffer {@code g_fuzzy_image} in .rs file.
      filterLayerBehindFocalDepth();

      // Replaces active pixels in {@code g_sharp_image} with the filtering
      // result saved in {@code g_fuzzy_image}. The replacement is soft,
      // blending {@code g_sharp_image} and {@code g_fuzzy_image} using the
      // computed matte. Uses the blending result as the sharp input image for
      // the next iteration.
      updateSharpImageUsingFuzzyImage();
    }
    return true;
  }

  /*
   * Processes layers from front-most to focal depth (excluding the focal depth)
   */
  protected boolean processLayersFromFrontToFocus(BlurStack blurStack) {
    // At this point, the input image {@code g_sharp_image} has been updated by
    // the first pass from back-most layer to focus layer {@code
    // processLayersFromBackToFocus}.
    for (int targetLayer = 0; targetLayer < blurStack.getFocusLayer();
        ++targetLayer) {
      // Sets up target layer info in Render Script.
      LayerInfo layerInfo = blurStack.getLayerInfo(targetLayer);
      setTargetLayer(layerInfo);

      // For a layer that is in front of the focal depth, its front depth has
      // the largest blur kernel radius. Uses the kernel radius as dilation
      // radius of this layer.
      int dilationRadius = getKernelRadius(layerInfo.frontDepth, blurStack);
      setBlendInfo(dilationRadius);

      // Sends blur kernel matrix data to Render Script.
      setKernelData(targetLayer, blurStack);

      // Marks active pixels (pixels that on this layer).
      // Marks pixels that are close enough (within dilationRadius) to the
      // active pixels.
      // Computes distance transform of the active pixels in their neighborhood
      // and use the distance value as matte for layer blending later.
      computeLayerMatteInFrontOfFocalDepth();

      // Computes filtering for pixels on the target layer and accumulates the
      // filtering result to an buffer {@code g_fuzzy_image} in .rs file.
      // The accumulating operation is soft, using the computed matte values.
      filterLayerInFrontOfFocalDepth();
    }

    // Fills in the pixels on or behind the focal depth in {@code g_fuzzy_image}
    // using pixels in {@code g_sharp_image}. Does the filling in a soft way by
    // blending using the matte. Uses the blending result (saved in {@code
    // g_fuzzy_image}) as the final output image.
    finalizeFuzzyImageUsingSharpImage();
    return true;
  }

  private static int getKernelRadius(int depth, BlurStack blurStack) {
    int kernelRadius = KernelDataForRenderScript
        .computeKernelRadiusFromDiskRadius(blurStack.getDiskRadius(depth));
    return kernelRadius;
  }

  // ///////////////////////////////////////////////////////////////////////////
  //
  // The following abstract functions must be implemented in a subclass.
  //
  // ///////////////////////////////////////////////////////////////////////////

  // Initializes the member {@code scriptC} and allocate memory buffers (defined
  // in a subclass) that interface between Java and .rs file.
  protected abstract void initializeScriptAndBuffers(Bitmap inputImage,
      LayerInfo focalLayer);

  // Extracts the result image from memory buffer.
  protected abstract Bitmap extractResultImage();

  // Sets target layer info in .rs file.
  protected abstract void setTargetLayer(LayerInfo layerInfo);

  // Sets dilation radius in .rs file for blending target layer.
  protected abstract void setBlendInfo(int dilationRadius);

  /*
   * A function that prepares the blur kernels for the target layer and passes
   * them to the Render Script. Each depth value in the layer has a kernel. The
   * kernels are packed in a memory buffer. Auxiliary information for parsing
   * the memory buffer is also prepared and passed to Render Script.
   *
   * @param targetLayer the index of a target layer
   *
   * @param blurStack a BlurStack object that has the layer structure of the
   * refocus filter task in Java
   */
  protected abstract void setKernelData(int targetLayer, BlurStack blurStack);

  protected abstract void computeLayerMatteBehindFocalDepth();

  protected abstract void filterLayerBehindFocalDepth();

  protected abstract void updateSharpImageUsingFuzzyImage();

  protected abstract void computeLayerMatteInFrontOfFocalDepth();

  protected abstract void filterLayerInFrontOfFocalDepth();

  protected abstract void finalizeFuzzyImageUsingSharpImage();
}
