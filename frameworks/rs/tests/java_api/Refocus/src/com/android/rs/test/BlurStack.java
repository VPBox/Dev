package com.android.rs.refocus;

import android.util.Log;
import java.util.ArrayList;

/**
 * An object that contains all the parameters that are needed in refocusing
 * filtering function, including the range of depth levels, the disc blur radius
 * of each depth level, how the depth levels are grouped into layers, which
 * layer is in focus.
 *
 *  <b> Here by "depth", we mean inverse depth. Pixels with larger depth values
 * are closer to the camera.
 *
 *  For a layer n, its depth interval is (@code [layerInfo[n].backDepth,
 * layerInfo[n].frontDepth]), where (@code backDepth<=frontDepth).
 *
 *  The layers are ordered from near to far; note that near layers have larger
 * depth values.
 *
 *  (@code focusLayer) is the index of the layer that is in focus, that is, has
 * zero blur.
 *
 * @author zhl@google.com (Li Zhang)
 */

public class BlurStack {
  //private static final Log.Tag TAG = new Log.Tag("BlurStack");
  private static final String TAG = "BlurStack";
  /**
   * The cap for disc radius of blur kernels.
   */
  private static final float MAX_DISC_RADIUS = 25.0f;

  /**
   * The minimum of the interval that is used to group depth levels into
   * blending layers based on the corresponding blur disk radius.
   */
  private static final float MIN_DISK_RADIUS_STEP_SIZE = 2.0f;

  /**
   * The starting index of depth quantization level. Must be positive as zero is
   * reserved for invalid depth.
   */
  private static final int MIN_DEPTH = 1;

  /**
   * The ending index of depth quantization level. It must be a power of 2.
   */
  private static final int MAX_DEPTH = 64;

  /**
   * The scale to apply to 8-bit depthmaps.
   */
  private static final int DEPTH_SCALE = 256 / MAX_DEPTH;

  /**
   * For each depth value {@code d} within [MIN_DEPTH,MAX_DEPTH], its blur disc
   * radius is saved in {@code diskRadius[d-MIN_DEPTH]}. Hence the length
   * {@code diskRadius} is {@code MAX_DEPTH-MIN_DEPTH+1}.
   */
  private float[] diskRadiusArray;

  /**
   * A set of non-overlapping layers that covers all the depth levels. The
   * layers are ordered from front (closer to the camera) to back (farther away
   * from the camera).
   */
  private LayerInfo[] layerInfo;

  /**
   * The layer in which the focal depth belongs to. <b> For this layer, we
   * assume that it is a single depth layer. That is, the front depth and back
   * depth both equal to focal depth.
   */
  private int focusLayer;

  public static float getMaxDiskRadius() {
    return MAX_DISC_RADIUS;
  }

  /**
   * Returns the blur disk radius of a depth level.
   *
   * @param depth depth level
   * @return the blur disk radius of the depth level
   */
  public float getDiskRadius(int depth) {
    return diskRadiusArray[depth - MIN_DEPTH];
  }

  public int getNumLayers() {
    return layerInfo.length;
  }

  public LayerInfo getLayerInfo(int layer) {
    return layerInfo[layer];
  }

  /**
   * Returns the number of depths in a given layer.
   *
   * @param layer layer index
   * @return the number of depth levels in the layer
   */
  public int getNumDepths(int layer) {
    return layerInfo[layer].frontDepth - layerInfo[layer].backDepth + 1;
  }

  public int getFocusLayer() {
    return focusLayer;
  }

  /**
   * Returns the depth given the layer and the relative depth in the layer.
   *
   * @param layer the layer index
   * @param relativeDepthInLayer the relative depth index relative to the back
   *        depth of a layer
   * @return the depth
   */
  public int getDepth(int layer, int relativeDepthInLayer) {
    return layerInfo[layer].backDepth + relativeDepthInLayer;
  }

  /**
   * Creates an instance of BlurStack using depth range, focal depth, desired
   * amount of blur at infinity, and the number of blending layers.
   *
   * @param depthTransform an object that translates between floating depth and
   *        quantized depth.
   * @param focusDepth3D focus depth in 3D
   * @param depthOfField the range of depth values around focus depth 3D that
   *        has zero blur.
   * @param blurInfinity the desired amount of blur, represented as blur radius
   *        at infinity
   * @param numBlendingLayers the number of blending layers that group all the
   *        depth levels
   * @return an instance of {@code BlurStack}
   */
  public static BlurStack createFromDepthTransform(
      final DepthTransform depthTransform, float focusDepth3D,
      float depthOfField, float blurInfinity, int numBlendingLayers) {
    BlurStack blurStack = new BlurStack();
    // Finds the front and back depth levels for the focus layer.
    if (depthOfField < 0) {
      depthOfField = -depthOfField;
      Log.e(TAG, "Negative depth of field");
    }
    int frontFocalDepth = openglDepthToStackDepth(
        depthTransform.quantize(focusDepth3D * (1 - depthOfField)));
    int backFocalDepth = openglDepthToStackDepth(
        depthTransform.quantize(focusDepth3D * (1 + depthOfField)));
    // Computes blur disk radius for all the depth levels.
    blurStack.computeDiskRadius(depthTransform, frontFocalDepth, backFocalDepth,
        blurInfinity);

    if (numBlendingLayers >= MAX_DEPTH) {
      blurStack.generateOneLayerForEachDepth(frontFocalDepth, backFocalDepth);
    } else {
      // Sets the max variation of blur disk radius in a blending layer.
      float diskRadiusInterval = (blurStack.getDiskRadius(MIN_DEPTH)
          + blurStack.getDiskRadius(MAX_DEPTH)) / numBlendingLayers;
      diskRadiusInterval =
          Math.max(diskRadiusInterval, MIN_DISK_RADIUS_STEP_SIZE);
      // Computes {@code layerInfo, focusLayer}, assuming {@code diskRadius}
      // have been computed.
      blurStack.groupDepthLevelsIntoLayers(frontFocalDepth, backFocalDepth,
          diskRadiusInterval);
    }
    return blurStack;
  }

  @Override
  public String toString() {
    String s = "disparity range: " + MAX_DEPTH + ", " + MIN_DEPTH + "\n";
    s += "focus disparity: " + layerInfo[focusLayer].frontDepth + ", "
        + layerInfo[focusLayer].backDepth + "\n";
    s += "num of layers: " + getNumLayers() + "\n";
    s += "focus layer: " + focusLayer + "\n";

    for (int n = 0; n < layerInfo.length; ++n) {
      int front = layerInfo[n].frontDepth;
      int back = layerInfo[n].backDepth;
      s += "\nlayer " + n + " num of disparities " + (front - back + 1) + "\n";

      for (int d = front; d >= back; --d) {
        s += "layer " + n + " disparity " + d + " disk radius "
            + getDiskRadius(d) + "\n";
      }
    }

    return s;
  }

  /**
   * OpenGL depth is from 0(near) to 255(far). The depth in BlurStack is from
   * 1(far) to MAX_DEPTH(near). Converts from openglDepth to stackDepth.
   *
   * @param openglDepth openGL depth.
   * @return stackDepth stack depth.
   */
  private static int openglDepthToStackDepth(int openglDepth) {
    return MAX_DEPTH - (openglDepth / DEPTH_SCALE);
  }

  /**
   * OpenGL depth is from 0(near) to 255(far). The depth in BlurStack is from
   * 1(far) to MAX_DEPTH(near). Converts from stackDepth to openglDepth.
   *
   * @param stackDepth stack depth.
   * @return openglDepth openGL depth.
   */
  private static int stackDepthToOpenglDepth(int stackDepth) {
    return (MAX_DEPTH - stackDepth) * DEPTH_SCALE;
  }

  /**
   * A private constructor that forces users to use {@code createFromDepthRange}
   * to construct an instance of BlurStack.
   */
  private BlurStack() {}

  /**
   * Quantizes the depth range into MAX_DEPTH levels in inverse depth space, and
   * for each level, computes the blur disk radius.
   *
   * @param depthTransform an object that translates between floating depth and
   *        quantized depth.
   * @param frontFocalDepth front focal depth level
   * @param backFocalDepth back focal depth level
   * @param blurInfinity the amount of desired blur represented as the blur
   *        radius at infinity
   */
  private void computeDiskRadius(final DepthTransform depthTransform,
      int frontFocalDepth, int backFocalDepth, float blurInfinity) {
    int numLevels = MAX_DEPTH - MIN_DEPTH + 1;
    diskRadiusArray = new float[numLevels];
    float frontFocalDepth3D =
        depthTransform.reconstruct(stackDepthToOpenglDepth(frontFocalDepth));
    float backFocalDepth3D =
        depthTransform.reconstruct(stackDepthToOpenglDepth(backFocalDepth));

    // Computes the blur disk radius for each depth level.
    for (int depth = MIN_DEPTH; depth <= MAX_DEPTH; ++depth) {
      float depth3D =
          depthTransform.reconstruct(stackDepthToOpenglDepth(depth));
      float radius = 0;
      if (depth3D < frontFocalDepth3D) {
        radius = blurInfinity * (frontFocalDepth3D - depth3D) / depth3D;
      } else if (depth3D > backFocalDepth3D) {
        radius = blurInfinity * (depth3D - backFocalDepth3D) / depth3D;
      }
      diskRadiusArray[depth - MIN_DEPTH] = Math.min(radius, MAX_DISC_RADIUS);
    }
  }

  /**
   * Sets up {@code focusLayer} such that each layer contains only a single
   * depth, except that the focal layer contains frontFocalDepth and
   * backFocalDepth.
   *
   * <b> This function computes {@code layerInfo, focusLayer}.
   *
   * @param frontFocalDepth the front depth of focal layer.
   * @param backFocalDepth the back depth of focal layer.
   */
  private void generateOneLayerForEachDepth(int frontFocalDepth,
      int backFocalDepth) {
    int numLayers =
        MAX_DEPTH - MIN_DEPTH + 1 - (frontFocalDepth - backFocalDepth);
    layerInfo = new LayerInfo[numLayers];

    // Pushes single depth layers in front of the focal layer to layerInfo.
    int layer = 0;
    for (int depth = MAX_DEPTH; depth > frontFocalDepth; --depth, ++layer) {
      layerInfo[layer] = new LayerInfo(depth);
    }

    // Pushes focal layer to layerInfo.
    focusLayer = layer;
    layerInfo[layer] = new LayerInfo(frontFocalDepth, backFocalDepth);
    ++layer;

    // Pushes single depth layers behind the focal layer to layerInfo.
    for (int depth = backFocalDepth - 1; depth >= MIN_DEPTH; --depth, ++layer) {
      layerInfo[layer] = new LayerInfo(depth);
    }
  }

  /**
   * Sets up {@code focusLayer} such that within each layer, the blur radius
   * variation due to depth difference is no larger than
   * {@code diskRadiusInterval}.
   *
   * <b> This function computes {@code layerInfo, focusLayer}, assuming that
   * {@code diskRadius} have been properly initialized.
   *
   * @param frontFocalDepth the front depth of focal layer.
   * @param backFocalDepth the back depth of focal layer.
   * @diskRadiusInterval the max allowed blur disk radius difference within each
   *                     layer.
   */
  private void groupDepthLevelsIntoLayers(int frontFocalDepth,
      int backFocalDepth, float diskRadiusInterval) {
    // Groups depth levels behind the focal depth into several layers.
    // The blur radius difference in each layer is no larger than
    // diskRadiusInterval.
    ArrayList<LayerInfo> layerInfoBehindFocus =
        groupDepthLevelsBehindFocus(backFocalDepth, diskRadiusInterval);

    // Groups depth levels in front of the focal depth into several layers.
    // The blur radius difference in each layer is no larger than {@code
    // diskRadiusInterval}.
    ArrayList<LayerInfo> layerInfoInFrontOfFocus =
        groupDepthLevelsInFrontOfFocus(frontFocalDepth, diskRadiusInterval);

    // Merges the two groups of layers into one stack of layers, plus the focus
    // depth layer.
    int numLayers =
        layerInfoInFrontOfFocus.size() + 1 + layerInfoBehindFocus.size();
    layerInfo = new LayerInfo[numLayers];
    focusLayer = layerInfoInFrontOfFocus.size();

    // The merged layers is ordered from the front-most layer to the back-most
    // layer.
    for (int n = 0; n < numLayers; ++n) {
      if (n < layerInfoInFrontOfFocus.size()) {
        // Finds the corresponding layer index m in layerInfoInFrontOfFocus,
        // which is ordered from focal depth to front-most.
        int m = (layerInfoInFrontOfFocus.size() - 1) - n;
        layerInfo[n] = layerInfoInFrontOfFocus.get(m);
      } else if (n == layerInfoInFrontOfFocus.size()) {
        layerInfo[n] = new LayerInfo(frontFocalDepth, backFocalDepth);
      } else {
        // Finds the corresponding layer index m in layerInfoBehindFocus, which
        // is ordered from focal depth to back-most.
        int m = n - (layerInfoInFrontOfFocus.size() + 1);
        layerInfo[n] = layerInfoBehindFocus.get(m);
      }
    }
  }

  /**
   * Groups depth levels behind the focal depth into several layers. The blur
   * radius difference in each layer is no larger than
   * {@code diskRadiusInterval}.
   *
   * @param backFocalDepth the back depth of focal layer.
   * @param diskRadiusInterval max disk radius variation in each layer
   * @return layerInfo layering of depth levels behind the focal depth
   */
  private ArrayList<LayerInfo> groupDepthLevelsBehindFocus(int backFocalDepth,
      float diskRadiusInterval) {
    // Initializes the layerInfo array with maximum capacity needed.
    ArrayList<LayerInfo> layerInfo =
        new ArrayList<LayerInfo>(diskRadiusArray.length);

    if (backFocalDepth == MIN_DEPTH) {
      return layerInfo;
    }

    // At this point, focusDepth > minDepth.
    // Moves to the first depth behind the focus depth and initializes a layer.
    int d = backFocalDepth - 1;
    layerInfo.add(new LayerInfo(d));
    // Sets up the max radius threshold for the layer.
    float radiusThreshold = getDiskRadius(d) + diskRadiusInterval;

    // Expands the layer to include depth levels so long as the blur disk
    // radius within the layer is not larger than radiusThreshold.
    // Stops the expansion when current depth is already the minDepth.
    while (d > MIN_DEPTH) {
      // Moves to the next depth.
      d--;
      if (getDiskRadius(d) <= radiusThreshold) {
        // Expands the current layer by lowering its back depth.
        int numLayers = layerInfo.size();
        layerInfo.get(numLayers - 1).backDepth = d;
      } else {
        // Generates a new single-depth layer.
        // Expands it in the next iteration if necessary.
        layerInfo.add(new LayerInfo(d));
        radiusThreshold = getDiskRadius(d) + diskRadiusInterval;
      }
    }
    return layerInfo;
  }

  /**
   * Groups depth levels in front of the focal depth into several layers. The
   * blur radius difference in each layer is no larger than
   * {@code diskRadiusInterval}.
   *
   * @param frontFocalDepth the back depth of focal layer.
   * @param diskRadiusInterval max disk radius variation in each layer
   * @return layerInfo layering of depth levels behind the focal depth
   */
  private ArrayList<LayerInfo> groupDepthLevelsInFrontOfFocus(
      int frontFocalDepth, float diskRadiusInterval) {
    // Initializes the layerInfo array with maximum capacity needed.
    ArrayList<LayerInfo> layerInfo =
        new ArrayList<LayerInfo>(diskRadiusArray.length);

    if (frontFocalDepth == MAX_DEPTH) {
      return layerInfo;
    }

    // At this point, focusDepth < maxDepth.
    // Moves to the first depth in front of the focus depth and initializes a
    // layer.
    int d = frontFocalDepth + 1;
    layerInfo.add(new LayerInfo(d));
    // Sets up the max radius threshold for the layer.
    float radiusThreshold = getDiskRadius(d) + diskRadiusInterval;

    // Expands the layer to include depth levels so long as the blur disk
    // radius within the layer is not larger than radiusThreshold.
    // Stops the expansion when current depth is already the maxDepth.
    while (d < MAX_DEPTH) {
      // Moves to the next depth.
      d++;
      if (getDiskRadius(d) <= radiusThreshold) {
        // Expands the current layer by increasing its front depth.
        int numLayers = layerInfo.size();
        layerInfo.get(numLayers - 1).frontDepth = d;
      } else {
        // Generates a new single-depth layer.
        // Expands it in the next iteration if necessary.
        layerInfo.add(new LayerInfo(d));
        radiusThreshold = getDiskRadius(d) + diskRadiusInterval;
      }
    }
    return layerInfo;
  }
}
