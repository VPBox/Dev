package com.android.rs.refocus;

/**
 * A struct containing all the data needed to apply a depth-of-field effect.
 *
 * @author chernand@google.com (Carlos Hernandez)
 */
public class DepthOfFieldOptions {
  public final RGBZ rgbz;
  public float focalDepth;
  public float blurInfinity;
  // The depth of field specifies the depth range in focus (i.e., zero blur) as
  // a ratio of the focal depth. Its range is [0, 1). The depth of field range
  // in depth units is computed as
  // [(1 - depthOfField) * focalDepth,(1 + depthOfField) * focalDepth].
  public float depthOfField;

  /**
   * Creates a {@code DepthOfFieldOptions} from an {@code RGBZ}.
   *
   * @param rgbz the {@code RGBZ} to render
   */
  public DepthOfFieldOptions(RGBZ rgbz) {
    this.focalDepth = (float)rgbz.getFocusDepth();
    this.depthOfField = (float)rgbz.getDepthOfField();
    this.blurInfinity = (float)rgbz.getBlurInfinity();
    this.rgbz = rgbz;
  }

  public void setFocusPoint(float x, float y) {
    this.focalDepth = rgbz.getDepth((int)(x * rgbz.getWidth()), (int)(y * rgbz.getHeight()));
    //this.blurInfinity = lensController.blurInfinityFromAverageBlur(this.focalDepth, this.depthOfField, averageBlur);
    //System.out.println("new focal depth: " + this.focalDepth);
  }

  public void setBokeh(float bokeh) {
    this.blurInfinity = bokeh * 200;
  }

  public void setDepthOfField(float depthOfField) {
    this.depthOfField = depthOfField;
  }
}
