package com.android.rs.refocus;


/**
 * Interface defining a depth transform that translates real depth values
 * into an 8-bit quantized representation.
 *
 * @author chernand@google.com (Carlos Hernandez)
 */
public interface DepthTransform {
    /**
     * @return The near depth value
     */
    public float getNear();

    /**
     * @return The far depth value
     */
    public float getFar();

    /**
     * @return The format of the transform
     */
    public String getFormat();

    /**
     * @return the quantized value that corresponds to the given depth value
     */
    public int quantize(float depth);

    /**
     * @return the depth value that corresponds to the given quantized value
     */
    public float reconstruct(int value);
}
