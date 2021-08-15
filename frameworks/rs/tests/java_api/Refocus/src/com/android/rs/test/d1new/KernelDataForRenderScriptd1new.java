package com.android.rs.refocus.d1new;

import com.android.rs.refocus.BlurStack;
import com.android.rs.refocus.KernelDataForRenderScript;

import androidx.renderscript.Allocation;
import androidx.renderscript.Element;
import androidx.renderscript.RenderScript;

import java.util.Arrays;

/**
 * A class that manages the blur kernel matrices of a blending layer that
 * interface between Java and Render Script.
 *
 * @author zhl@google.com (Li Zhang)
 */
public class KernelDataForRenderScriptd1new extends KernelDataForRenderScript {
  /**
   * A stack of blur kernel matrices packed into a continuous memory buffer.
   */
  float[] kernelStack;

  /**
   * The minimum blur disk in the stack.
   */
  float minDiskRadius;

  /**
   * An RenderScript Allocation for kernelStack.
   */
  Allocation stackAllocation;

  /**
   * Initializes {@code kernelStack} and {@code kernelInfo}.
   *
   * @param targetLayer the index of a target layer
   * @param blurStack an instance of {@code BlurStack}
   * @param renderScript an instance of {@code RenderScript}
   */
  public KernelDataForRenderScriptd1new(int targetLayer, BlurStack blurStack,
                                        RenderScript renderScript) {
    super(targetLayer, blurStack, renderScript);
    // stackLength is set in super.

    kernelStack = new float[stackLength];
    int numDepths = blurStack.getNumDepths(targetLayer);
    minDiskRadius = BlurStack.getMaxDiskRadius();
    for (int m = 0; m < numDepths; ++m) {
      int depth = blurStack.getDepth(targetLayer, m);
      float diskRadius = blurStack.getDiskRadius(depth);
      float[] kernelMatrix = getKernel(diskRadius);
      //System.arraycopy(kernelMatrix, 0, kernelStack, kernelInfo.get_offset(m),
        //  kernelMatrix.length);
      System.arraycopy(kernelMatrix, 0, kernelStack, (int)infoArray[4*m],
          kernelMatrix.length);
      minDiskRadius = Math.min(minDiskRadius, diskRadius);
    }

  stackAllocation = Allocation.createSized(renderScript,
      Element.F32(renderScript), kernelStack.length);
  stackAllocation.copyFrom(kernelStack);
  }

  /**
   * Returns the kernel matrix of a depth level.
   *
   * @param diskRadius disk radius of the kernel.
   * @return the kernel matrix of the disk radius.
   */
  public float[] getKernel(float diskRadius) {
    int kernelRadius = computeKernelRadiusFromDiskRadius(diskRadius);
    return generateDiskKernelArray(diskRadius, kernelRadius);
  }

  /**
   * Generates a blur kernel matrix for a blur disk with radius
   * {@code diskRadius}.
   *
   * @param diskRadius radius of blur disk
   * @param kernelRadius radius of blur kernel matrix
   * @return a kernel matrix represented as an array
   */
  private static float[] generateDiskKernelArray(float diskRadius,
      int kernelRadius) {
    int kernelDim = 2 * kernelRadius + 1;
    int kernelLength = kernelDim * kernelDim;

    float[] kernel = new float[kernelLength];
    Arrays.fill(kernel, 0);

    // Generates {@code NUM_SUB_PIXELS*NUM_SUB_PIXELS} sub-pixel shifts {@code
    // (dx,dy)} uniformly within [-0.5,0.5]*[-0.5,0.5].
    // For each shift, tests whether or not the shifted pixel is within the
    // disc and accumulates the count.
    float diskRadius2 = diskRadius * diskRadius;
    float dy0 = -0.5f + 1.0f / (2 * getNumSubPixels());
    float dx0 = -0.5f + 1.0f / (2 * getNumSubPixels());

    float sumKernelValues = 0;
    for (int v = 0; v < getNumSubPixels(); ++v) {
      float dy = dy0 + (float) v / (float) getNumSubPixels();
      for (int u = 0; u < getNumSubPixels(); ++u) {
        float dx = dx0 + (float) u / (float) getNumSubPixels();
        for (int y = 0; y < kernelDim; ++y) {
          float yf = y - kernelRadius + dy;
          for (int x = 0; x < kernelDim; ++x) {
            float xf = x - kernelRadius + dx;
            if (yf * yf + xf * xf <= diskRadius2) {
              kernel[y * kernelDim + x] += 1;
              sumKernelValues += 1;
            }
          }
        }
      }
    }

    // Normalizes kernel elements such that they sum up to 1.
    for (int n = 0; n < kernelLength; ++n) {
      kernel[n] /= sumKernelValues;
    }

    return kernel;
  }

}
