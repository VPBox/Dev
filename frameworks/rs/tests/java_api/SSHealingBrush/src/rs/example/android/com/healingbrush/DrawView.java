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

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.RectF;
import android.graphics.drawable.Drawable;
import android.util.AttributeSet;
import android.view.View;
import android.widget.ImageView;

import java.util.ArrayList;
import java.util.Arrays;

public class DrawView extends View {
    private static final String TAG = "DrawView";
    private ImageView mImageView;
    Path mPoints_backup = new Path();
    float[] path = new float[200];
    Path mPoints = new Path();
    int len;
    Paint mPaint1;
    Paint mPaint2;
    private boolean mDone;
    ArrayList<Drawable> drawList = new ArrayList<Drawable>();

    private void setup(Context context) {
        mPaint1 = new Paint();
        mPaint2 = new Paint();
        mPaint1.setStyle(Paint.Style.STROKE);
        mPaint1.setColor(Color.BLACK);
        mPaint1.setStrokeWidth(2);
        mPaint2.setStyle(Paint.Style.STROKE);
        mPaint2.setColor(Color.YELLOW);
    }

    public DrawView(Context context) {
        super(context);
        setup(context);
    }

    public DrawView(Context context, AttributeSet attrs) {
        super(context, attrs);
        setup(context);
    }

    public DrawView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        setup(context);
    }

    /**
     * Assumes imageView is using matrix mode
     *
     * @param imageView
     */
    public void setImageView(ImageView imageView) {
        mImageView = imageView;
    }

    RectF rec = new RectF();

    @Override
    protected void onDraw(Canvas canvas) {
        Matrix m = mImageView.getImageMatrix();
        Drawable d = mImageView.getDrawable();
        canvas.concat(m);

        for (Drawable elem : drawList) {
            elem.draw(canvas);
        }
        canvas.drawPath(mPoints, mPaint1);
        canvas.drawPath(mPoints, mPaint2);
    }

    public Region getRegion(Bitmap img) {
        Region ret = new Region(Arrays.copyOf(path, len), img);

        invalidate();
        return ret;
    }

    public void downPoint(float[] imgPoint) {
        path[0] = imgPoint[0];
        path[1] = imgPoint[1];
        len = 2;
        mPoints_backup.reset();
        mPoints_backup.addPath(mPoints);
        mPoints.reset();
        mPoints.moveTo(imgPoint[0], imgPoint[1]);
    }

    public void undo() {
        mPoints.reset();
        mPoints.addPath(mPoints_backup);
    }

    public void movePoint(float[] imgMoveList, int size) {
        if (len + size * 2 >= path.length) {
            path = Arrays.copyOf(path, 2 * (len + size * 2));
        }
        for (int i = size * 2 - 2; i >= 0; i -= 2) {
            mPoints.lineTo(imgMoveList[i], imgMoveList[i + 1]);
            path[len] = imgMoveList[i];
            path[len + 1] = imgMoveList[i + 1];
            len += 2;
        }
    }

    public void upPoint(float[] imgPoint) {
        if (len + 2 >= path.length) {
            path = Arrays.copyOf(path, 2 * (len + 2));
        }
        path[len] = imgPoint[0];
        path[len + 1] = imgPoint[1];
        len += 2;
        mPoints.lineTo(imgPoint[0], imgPoint[1]);
        mPoints.close();
        mDone = true;
    }

    public void addDrawable(Drawable d) {
        drawList.add(d);
    }

    public void clearDrawables() {
        drawList.clear();
    }
}
