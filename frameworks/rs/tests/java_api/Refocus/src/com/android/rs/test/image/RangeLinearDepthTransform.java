package com.android.rs.refocus.image;

import com.android.rs.refocus.DepthTransform;

/**
 *
 * An implementation of {@code DepthTransform} that uses a linear 8-bit
 * quantization.
 *
 * @author chernand@google.com (Carlos Hernandez)
 */
public class RangeLinearDepthTransform implements DepthTransform {
  public static final String FORMAT = "RangeLinear";

  private final float near;
  private final float far;

  public RangeLinearDepthTransform(float near, float far) {
    this.near = near;
    this.far = far;
  }

  @Override
  public float getNear() {
    return near;
  }

  @Override
  public float getFar() {
    return far;
  }

  @Override
  public String getFormat() {
    return FORMAT;
  }

  @Override
  public int quantize(float value) {
    return Math.max(0, Math.min(255,
        (int) ((value - near) / (far - near) * 255f)));
  }

  @Override
  public float reconstruct(int value) {
    return near + (far - near) * Math.max(0, Math.min(255, value)) / 255f;
  }
}
