package com.android.rs.refocus;

/**
 * An object that defines a blending layer for refocus rendering, which groups
 * several consecutive depth levels.
 */
public class LayerInfo {
  public int frontDepth;
  public int backDepth;

  LayerInfo(int depth) {
    frontDepth = depth;
    backDepth = depth;
  }

  LayerInfo(int front, int back) {
    frontDepth = front;
    backDepth = back;
  }
}
