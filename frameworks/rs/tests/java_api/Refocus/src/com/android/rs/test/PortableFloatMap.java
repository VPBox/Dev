package com.android.rs.refocus;

import android.graphics.Bitmap;
import com.android.rs.refocus.DepthTransform;
import com.android.rs.refocus.image.RangeLinearDepthTransform;
import java.io.InputStream;
import java.io.IOException;
import java.lang.RuntimeException;
import java.lang.StringBuffer;
import java.nio.ByteBuffer;
import java.nio.FloatBuffer;
import java.nio.ByteOrder;

public class PortableFloatMap {

    private int mWidth;
    private int mHeight;
    private float mScale;
    private boolean mLittleEndian;
    private ByteOrder mByteOrder;

    private int mBytesPerPixel;
    private ByteBuffer mData;

    private static String readString(InputStream in)
            throws IOException {
        StringBuffer buffer = new StringBuffer();
        int c;
        while ((c = in.read()) != -1 && c != 0xa && c != 0x20) {
            buffer.append((char)c);
        }
        return buffer.toString();
    }

    public PortableFloatMap(InputStream in)
            throws IOException {
        String formatLine = readString(in);

        if (formatLine.compareTo("Pf") != 0) {
            // Currently "PF" (RGB32F) is not supported.
            // Only "Pf" (Greyscale32F) is supported.
            throw new RuntimeException("Unexpected format line: " + formatLine);
        }

        mBytesPerPixel = 4;

        String widthLine = readString(in);

        mWidth = Integer.parseInt(widthLine);

        String heightLine = readString(in);

        mHeight = Integer.parseInt(heightLine);

        final int bytesPerRow = mBytesPerPixel * mWidth;

        mData = ByteBuffer.allocate(bytesPerRow * mHeight);

        String scaleLine = readString(in);

        mScale = Float.parseFloat(scaleLine);

        mData.order(mScale < 0 ? ByteOrder.LITTLE_ENDIAN : ByteOrder.BIG_ENDIAN);

        byte[] row = new byte[mBytesPerPixel * mWidth];

        for (int y = 0; y < mHeight; y++) {
            int nRead = 0;
            int offset = 0;
            while (offset < bytesPerRow) {
                nRead = in.read(row, offset, bytesPerRow - offset);
                if (nRead < 0) {
                    throw new RuntimeException("Unexpected End of File.");
                }
                offset += nRead;
            }
            mData.put(row);
        }
        mData.rewind();
    }

    final float[] getPixelArray() {
        FloatBuffer floatBuffer = mData.asFloatBuffer();
        if (floatBuffer.hasArray()) {
            return floatBuffer.array();
        }

        float[] array = new float[mWidth * mHeight];
        floatBuffer.get(array);

        return array;
    }

    int getWidth() { return mWidth; }

    int getHeight() { return mHeight; }
}
