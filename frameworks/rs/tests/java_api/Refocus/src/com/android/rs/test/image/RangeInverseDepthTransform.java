package com.android.rs.refocus.image;

import com.android.rs.refocus.DepthTransform;

/**
 * An implementation of {@code DepthTransform} that uses an inverse
 * 8-bit representation of depth similar to the OpenGL z-buffer storage.
 *
 * @author chernand@google.com (Carlos Hernandez)
 */
public class RangeInverseDepthTransform implements DepthTransform {
  public static final String FORMAT = "RangeInverse";

  private final float near;
  private final float far;

  public RangeInverseDepthTransform(float near, float far) {
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
        (int) ((far - near * far / value) / (far - near) * 255f)));
  }

  @Override
  public float reconstruct(int value) {
    return (far * near) / (far - (far - near) *
        Math.max(0, Math.min(255, value)) / 255f);
  }
}
