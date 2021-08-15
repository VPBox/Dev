/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package rs.example.android.com.healingbrush;

import android.graphics.Bitmap;
import android.graphics.Rect;
import android.graphics.RectF;
import android.renderscript.Allocation;
import android.renderscript.Element;
import android.renderscript.RenderScript;
import android.renderscript.Type;
import android.util.Log;

import com.example.android.rs.sample.ScriptC_find_region;

public class FindRegion {
    private static final String TAG = "FindRegion";

    Rect mRoiBounds; // bounding box of the ROI
    float[] mPointsXY; // polygon point in original image coordnates

    int mCutOffsetX; // image coords of the cut  (mPointsXY - mPasteOffX + mCutOffsetX)
    int mCutOffsetY; // image coords of the cut (mPointsXY - mPasteOffY + mCutOffsetY)

    public FindRegion(float[] xy, Bitmap img) {
        mPointsXY = xy;
        int imgWidth = img.getWidth();
        int imgHeight = img.getHeight();
        mRoiBounds = getBoundingRect(xy);

        if (mRoiBounds.height() <= 2 ||
                mRoiBounds.left < 0
                || mRoiBounds.top < 0
                || mRoiBounds.right >= imgWidth
                || mRoiBounds.bottom >= imgHeight) {
            throw new RuntimeException("ROI to close to the edge of the image");
        }

     }

    public int getCutOffsetX() {
        return mCutOffsetX;
    }

    public int getCutOffsetY() {
        return mCutOffsetY;
    }

    public Rect getRoiBounds() {
        return mRoiBounds;
    }

    Allocation allocFloat2(float[] p, RenderScript rs) {
        Type.Builder builderF32_2 = new Type.Builder(rs, Element.F32_2(rs));
        builderF32_2.setX(p.length / 2);
        Allocation ret = Allocation.createTyped(rs, builderF32_2.create());
        ret.copyFrom(p);
        return ret;
    }

    public Rect findMatch(ScriptC_find_region findRegion, RenderScript mRs, Bitmap image) {
        long time = System.nanoTime();

        Allocation border_coords;

        border_coords = allocFloat2(mPointsXY, mRs);
        Allocation aImage = Allocation.createFromBitmap(mRs, image);
        Allocation ret = Allocation.createSized(mRs, Element.I32_2(mRs), 1);

        findRegion.invoke_findRegion(border_coords, aImage, image.getWidth(), image.getHeight(), ret);

        int[] mina = new int[2];
        ret.copyTo(mina);
        mCutOffsetX = mina[0];
        mCutOffsetY = mina[1];
        Log.v(TAG, "New best location = " + mCutOffsetX + ", " + mCutOffsetY);
        Log.v(TAG, "Time to find replacement= " + (System.nanoTime() - time) / 1E6f + "ms");

        return mRoiBounds;
    }

    /**
     * Computes the bounding box of the polygon
     * then pads and sizes to multiple of 8
     *
     * @param xy points of polygon [x1,y1,x2,y2,...]
     * @return rectangle
     */
    private static Rect getBoundingRect(float[] xy) {
        RectF mRect = calcBounds(xy);
        int mWidth = (((int) (8 + mRect.width())) & ~3); // bounding rectangle that is a power of 8
        int mHeight = (((int) (8 + mRect.height())) & ~3);
        int mPasteOffX = (int) mRect.left - 1;
        int mPasteOffY = (int) mRect.top - 1;
        return new Rect(mPasteOffX, mPasteOffY, mPasteOffX + mWidth, mPasteOffY + mHeight);
    }

    private static RectF calcBounds(float[] xy) {
        float minx = xy[0], miny = xy[1];
        float maxx = xy[0], maxy = xy[1];
        for (int i = 0; i < xy.length; i += 2) {
            minx = Math.min(minx, xy[i]);
            maxx = Math.max(maxx, xy[i]);
            miny = Math.min(miny, xy[i + 1]);
            maxy = Math.max(maxy, xy[i + 1]);
        }
        RectF rect = new RectF();
        rect.set(minx, miny, maxx, maxy);
        return rect;
    }



}
