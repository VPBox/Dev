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
import android.renderscript.Allocation;
import android.renderscript.Element;
import android.renderscript.RenderScript;
import android.renderscript.Script;
import android.renderscript.Type;
import android.util.Log;

import com.example.android.rs.sample.ScriptC_healing;

public class Healing {
    private static final String TAG = "Healing";
    Rect mRoiBounds; // bounding box of the ROI
    float[] mPointsXY; // polygon point in original image coordnates
    int mCutOffsetX; // image coords of the cut  (mPointsXY - mPasteOffX + mCutOffsetX)
    int mCutOffsetY; // image coords of the cut (mPointsXY - mPasteOffY + mCutOffsetY)
    Bitmap mUndoBitmap;

    public Healing(Rect roiBounds, float[] pointsXY, int cutOffsetX, int cutOffsetY) {
        mRoiBounds = roiBounds;
        mPointsXY = pointsXY;
        mCutOffsetX = cutOffsetX;
        mCutOffsetY = cutOffsetY;
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

    /**
     * This function only assumes mPointsXY, mPasteOffX, mPasteOffY
     *
     * @param healing
     * @param rs
     * @param image
     */
    public void heal(ScriptC_healing healing, RenderScript rs, Bitmap image, Bitmap output) {
        long time = System.nanoTime();

        Type.Builder floatImage = new Type.Builder(rs, Element.F32_3(rs));
        floatImage.setX(mRoiBounds.width());
        floatImage.setY(mRoiBounds.height());


        Bitmap mask_bitmap = buildMask(mRoiBounds, mPointsXY);
        Bitmap dest_bitmap = createMutableBitmap(image, mRoiBounds.left, mRoiBounds.top,
                mRoiBounds.width(), mRoiBounds.height());
        Allocation dest_alloc = Allocation.createFromBitmap(rs, dest_bitmap);
        Bitmap src_bitmap = createMutableBitmap(image, mCutOffsetX, mCutOffsetY,
                mRoiBounds.width(), mRoiBounds.height());
        Allocation src_alloc = Allocation.createFromBitmap(rs, src_bitmap);
        Allocation mask_alloc = Allocation.createFromBitmap(rs, mask_bitmap);

        healing.invoke_heal(mask_alloc, src_alloc, dest_alloc);

        dest_alloc.copyTo(dest_bitmap);

        dest_bitmap.setHasAlpha(true);

        // build the undo
        mUndoBitmap = Bitmap.createBitmap(mRoiBounds.width(), mRoiBounds.height(),
                Bitmap.Config.ARGB_8888);
        Canvas undoCanvas = new Canvas(mUndoBitmap);
        Rect undoRect = new Rect(0, 0, mRoiBounds.width(), mRoiBounds.height());
        undoCanvas.drawBitmap(output, mRoiBounds, undoRect, null);

        Canvas c = new Canvas(output);
        c.drawBitmap(image, 0, 0, null);
        c.drawBitmap(dest_bitmap, mRoiBounds.left, mRoiBounds.top, null);
        Log.v(TAG, " time ss to smart paste = " + (System.nanoTime() - time) / 1E6f + "ms");
        heal_orig(healing, rs, image, output);

    }


    /**
     * This function only assumes mPointsXY, mPasteOffX, mPasteOffY
     *
     * @param healing
     * @param rs
     * @param image
     */
    public void heal_orig(ScriptC_healing healing, RenderScript rs, Bitmap image, Bitmap output) {
        long time = System.nanoTime();
        Type.Builder floatImage = new Type.Builder(rs, Element.F32_3(rs));
        floatImage.setX(mRoiBounds.width());
        floatImage.setY(mRoiBounds.height());

        Bitmap maskBitmap = buildMask(mRoiBounds, mPointsXY);

        Allocation dest1 = Allocation.createTyped(rs, floatImage.create());
        Allocation dest2 = Allocation.createTyped(rs, floatImage.create());
        healing.set_dest1(dest1);
        healing.set_dest2(dest2);

        Bitmap destBitmap = createMutableBitmap(image, mRoiBounds.left, mRoiBounds.top,
                mRoiBounds.width(), mRoiBounds.height());
        Allocation dest_uc4 = Allocation.createFromBitmap(rs, destBitmap);
        healing.forEach_convert_to_f(dest_uc4, dest1);

        Bitmap src = createMutableBitmap(image, mCutOffsetX, mCutOffsetY,
                mRoiBounds.width(), mRoiBounds.height());
        Allocation src_f3 = Allocation.createTyped(rs, floatImage.create());
        Allocation src_uc4 = Allocation.createFromBitmap(rs, src);
        healing.forEach_convert_to_f(src_uc4, src_f3);
        healing.set_src(src_f3);

        Allocation mask = Allocation.createFromBitmap(rs, maskBitmap);
        healing.set_mask(mask);

        Allocation laplace_f3 = Allocation.createTyped(rs, floatImage.create());
        healing.set_laplace(laplace_f3);

        Script.LaunchOptions options = new Script.LaunchOptions();
        options.setX(1, mRoiBounds.width() - 1);
        options.setY(1, mRoiBounds.height() - 1);
        healing.forEach_laplacian(laplace_f3, options);
        healing.forEach_copyMasked(mask, dest1);

        int steps = (int) Math.hypot(mRoiBounds.width(), mRoiBounds.height()); // match RS Single source
        Log.v(TAG, "Healing_orig  :steps = " + steps);
        for (int i = 0; i < steps; i++) {
            healing.forEach_solve1(mask, dest2);
            healing.forEach_solve2(mask, dest1);
        }

        healing.forEach_convert_to_uc(dest1, dest_uc4);
        rs.finish();

        healing.forEach_alphaMask(dest_uc4, dest_uc4);
        rs.finish();

        dest_uc4.copyTo(destBitmap);
        rs.finish();
        destBitmap.setHasAlpha(true);
        rs.finish();
        // build the undo
        mUndoBitmap = Bitmap.createBitmap(mRoiBounds.width(), mRoiBounds.height(),
                Bitmap.Config.ARGB_8888);
        Canvas undoCanvas = new Canvas(mUndoBitmap);
        Rect undoRect = new Rect(0, 0, mRoiBounds.width(), mRoiBounds.height());
        undoCanvas.drawBitmap(output, mRoiBounds, undoRect, null);

        Canvas c = new Canvas(output);
        c.drawBitmap(image, 0, 0, null);
        c.drawBitmap(destBitmap, mRoiBounds.left, mRoiBounds.top, null);
        Log.v(TAG, " time to smart paste = " + (System.nanoTime() - time) / 1E6f + "ms");
    }

    Bitmap createMutableBitmap(Bitmap image, int x, int y, int width, int height) {
        Bitmap ret = Bitmap.createBitmap(image, x, y, width, height);
        return ret.copy(Bitmap.Config.ARGB_8888, true);
    }

    private static int calcMaskArea(Allocation mask) {
        int w = mask.getType().getX();
        int h = mask.getType().getY();
        byte[] data = new byte[w * h];
        mask.copyTo(data);
        int count = 0;
        int val = data[0];
        for (int i = 0; i < data.length; i++) {
            if (data[i] != val) {
                count++;
            }
        }
        return count;
    }

    Bitmap getmUndoBitmap() {
        return mUndoBitmap;
    }
}
