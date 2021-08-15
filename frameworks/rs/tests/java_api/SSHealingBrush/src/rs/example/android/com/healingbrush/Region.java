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
import android.graphics.ColorFilter;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.Rect;
import android.graphics.drawable.Drawable;
import android.renderscript.RenderScript;

import com.example.android.rs.sample.ScriptC_find_region;
import com.example.android.rs.sample.ScriptC_healing;

public class Region {
    private static final String TAG = "Region";
    int mCutOffsetX; // image coords of the cut  (mPointsXY - mPasteOffX + mCutOffsetX)
    int mCutOffsetY; // image coords of the cut (mPointsXY - mPasteOffY + mCutOffsetY)

    int[] mPaste; // contains a copy where to paste

    float[] mPointsXY; // polygon point in original image coordnates
    int numberOfPoints;

    FindRegion mFindRegion;
    private Bitmap mUndoBitmap;

    /**
     * @param xy  A list of xy points that represents the polygon
     * @param img The original polygon
     */
    public Region(float[] xy, Bitmap img) {
        mPointsXY = xy;
        mFindRegion = new FindRegion(mPointsXY, img);
    }

    Drawable getSourceLocation() {
        final Path path = new Path();
        Rect bounds = mFindRegion.getRoiBounds();
        for (int i = 0; i < mPointsXY.length; i += 2) {
            if (i == 0) {
                path.moveTo(mPointsXY[i] - bounds.left + mCutOffsetX,
                        mPointsXY[i + 1] - bounds.top + mCutOffsetY);
            } else {
                path.lineTo(mPointsXY[i] - bounds.left + mCutOffsetX,
                        mPointsXY[i + 1] - bounds.top + mCutOffsetY);
            }
        }
        path.close();
        Drawable d = new Drawable() {
            Paint paint1 = new Paint();
            Paint paint2 = new Paint();

            {
                paint1.setStyle(Paint.Style.STROKE);
                paint2.setStyle(Paint.Style.STROKE);
                paint1.setColor(Color.BLACK);
                paint1.setStrokeWidth(2);
                paint2.setColor(Color.BLUE);
            }

            @Override
            public void draw(Canvas canvas) {
                canvas.drawPath(path, paint1);
                canvas.drawPath(path, paint2);
            }

            @Override
            public void setAlpha(int alpha) {
            }

            @Override
            public void setColorFilter(ColorFilter cf) {
            }

            @Override
            public int getOpacity() {
                return 0;
            }
        };
        return d;
    }

    public Drawable findMatch(ScriptC_find_region findRegion, RenderScript mRs, Bitmap image) {
        Rect mRoiBounds = mFindRegion.findMatch(findRegion, mRs, image);
        int cutOffsetX = mFindRegion.getCutOffsetX();
        int cutOffsetY = mFindRegion.getCutOffsetY();
        final Path path = new Path();
        for (int i = 0; i < mPointsXY.length; i += 2) {
            if (i == 0) {
                path.moveTo(mPointsXY[i] - mRoiBounds.left + cutOffsetX,
                        mPointsXY[i + 1] - mRoiBounds.top + cutOffsetY);
            } else {
                path.lineTo(mPointsXY[i] - mRoiBounds.left + cutOffsetX,
                        mPointsXY[i + 1] - mRoiBounds.top + cutOffsetY);
            }
        }

        path.close();


        Drawable d = new Drawable() {
            Paint paint = new Paint();

            {
                paint.setStyle(Paint.Style.STROKE);
            }

            @Override
            public void draw(Canvas canvas) {
                canvas.drawPath(path, paint);
            }

            @Override
            public void setAlpha(int alpha) {
            }

            @Override
            public void setColorFilter(ColorFilter cf) {
            }

            @Override
            public int getOpacity() {
                return 0;
            }
        };
        return d;
    }

    Bitmap createMutableBitmap(Bitmap image, int x, int y, int width, int height) {
        Bitmap ret = Bitmap.createBitmap(image, x, y, width, height);
        return ret.copy(Bitmap.Config.ARGB_8888, true);
    }

    /**
     * This function only assumes mPointsXY, mPasteOffX, mPasteOffY
     *
     * @param healing
     * @param rs
     * @param image
     */
    public void heal(ScriptC_healing healing, RenderScript rs, Bitmap image, Bitmap output) {
        Healing h = new Healing(
                mFindRegion.getRoiBounds(),
                mPointsXY,
                mFindRegion.getCutOffsetX(),
                mFindRegion.getCutOffsetY());
        h.heal(healing, rs, image, output);
        mUndoBitmap = h.getmUndoBitmap();
    }

    public void undo(Bitmap output) {
        Canvas c = new Canvas(output);
        Rect roi;
        roi = mFindRegion.getRoiBounds();
        c.drawBitmap(mUndoBitmap, roi.left, roi.top, null);
    }
}
