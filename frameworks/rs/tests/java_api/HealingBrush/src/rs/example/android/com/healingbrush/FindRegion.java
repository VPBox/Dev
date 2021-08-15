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
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.Rect;
import android.graphics.RectF;
import androidx.renderscript.Allocation;
import androidx.renderscript.Element;
import androidx.renderscript.RenderScript;
import androidx.renderscript.Script;
import androidx.renderscript.Type;
import android.util.Log;

import com.example.android.rs.sample.ScriptC_find_region;

public class FindRegion {
    private static final String TAG = "FindRegion";

    Rect mRoiBounds; // bounding box of the ROI
    int[] mPaste; // contains a copy where to paste
    float[] mPointsXY; // polygon point in original image coordnates
    Rect mSearchRange; // range to search in (original image coordinates
    Bitmap mMaskBitmap;

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

        mMaskBitmap = buildMask(mRoiBounds, mPointsXY);
        mPaste = new int[this.mRoiBounds.width() * this.mRoiBounds.height()];
        img.getPixels(mPaste, 0, mRoiBounds.width(),
                mRoiBounds.left, mRoiBounds.top, mRoiBounds.width(), mRoiBounds.height());

        mSearchRange = calcSearchRange(mRoiBounds, imgWidth, imgHeight);
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


    public Rect findMatch(ScriptC_find_region findRegion, RenderScript mRs, Bitmap image) {
        long time = System.nanoTime();
        Allocation border_coords;
        Allocation border_values;

        Type.Builder builderI32_2 = new Type.Builder(mRs, Element.I32_2(mRs));
        builderI32_2.setX(mPointsXY.length / 2);
        border_coords = Allocation.createTyped(mRs, builderI32_2.create());

        Allocation border_coords_float;
        border_coords_float = Allocation.createSized(mRs, Element.F32_2(mRs), mPointsXY.length / 2);
        border_coords_float.copyFrom(mPointsXY);
        findRegion.forEach_toInt(border_coords_float, border_coords);
        findRegion.set_border_coords(border_coords);

        findRegion.set_image(Allocation.createFromBitmap(mRs, image));

        Type.Builder builderF32_3 = new Type.Builder(mRs, Element.F32_3(mRs));
        builderF32_3.setX(mPointsXY.length / 2);
        border_values = Allocation.createTyped(mRs, builderF32_3.create());
        findRegion.set_border_values(border_values);
        findRegion.forEach_extractBorder(border_coords, border_values);

        Type.Builder builderF32 = new Type.Builder(mRs, Element.F32(mRs));
        builderF32.setX(mSearchRange.width());
        builderF32.setY(mSearchRange.height());

        Allocation fit = Allocation.createTyped(mRs, builderF32.create());
        findRegion.set_borderLength(mPointsXY.length / 2);
        int noSearch_x = mRoiBounds.left - mSearchRange.left;
        int noSearch_y = mRoiBounds.top - mSearchRange.top;
        findRegion.set_imagePosX(noSearch_x);
        findRegion.set_imagePosY(noSearch_y);
        Script.LaunchOptions options = new Script.LaunchOptions();
        options.setX(0, mSearchRange.width() - mRoiBounds.width());
        options.setY(0, mSearchRange.height() - mRoiBounds.height());
        findRegion.forEach_bordercorrelation(fit, options);


        Log.v(TAG, "noSearch " + noSearch_x + ", " + noSearch_y);
        Log.v(TAG, "noSearch " + mRoiBounds.width() + ", " + mRoiBounds.height());

        Allocation fit_max = Allocation.createSized(mRs, Element.I32_2(mRs), 1);

        findRegion.invoke_findMin(fit, fit_max, noSearch_x, noSearch_y,
                mRoiBounds.width(), mRoiBounds.height());
        int[] mina = new int[2];
        fit_max.copyTo(mina);

        mCutOffsetX = mina[0] + mSearchRange.left;
        mCutOffsetY = mina[1] + mSearchRange.top;

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

    private static Bitmap buildMask(Rect rec, float[] xy) {
        Bitmap bitmap = Bitmap.createBitmap(rec.width(), rec.height(), Bitmap.Config.ALPHA_8);

        Canvas c = new Canvas(bitmap);
        Paint paint = new Paint();
        paint.setStyle(Paint.Style.FILL);
        paint.setColor(Color.BLACK);
        Path path = new Path();
        for (int i = 0; i < xy.length; i += 2) {
            if (i == 0) {
                path.moveTo(xy[i] - rec.left, xy[i + 1] - rec.top);
            } else {
                path.lineTo(xy[i] - rec.left, xy[i + 1] - rec.top);
            }
        }
        path.close();
        c.drawPath(path, paint);
        return bitmap;
    }

    private static Rect calcSearchRange(Rect mRoiBounds, int imgWidth, int imgHeight) {
        int xmin = Math.max(0, (int) (mRoiBounds.left - mRoiBounds.width() * 2));
        int ymin = Math.max(0, (int) (mRoiBounds.top - mRoiBounds.height() * 2));
        int xmax = (int) (mRoiBounds.right + mRoiBounds.width() * 2);
        int ymax = (int) (mRoiBounds.bottom + mRoiBounds.height() * 2);
        xmax = Math.min(imgWidth, xmax);
        ymax = Math.min(imgHeight, ymax);
        xmax = Math.max(0, xmax);
        ymax = Math.max(0, ymax);
        return new Rect(xmin, ymin, xmax, ymax);
    }
}
