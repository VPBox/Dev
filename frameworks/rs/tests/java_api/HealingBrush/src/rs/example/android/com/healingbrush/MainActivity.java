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

import android.app.Activity;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.provider.OpenableColumns;
import androidx.renderscript.RenderScript;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ImageView;
import android.widget.Toast;

import com.example.android.rs.sample.ScriptC_find_region;
import com.example.android.rs.sample.ScriptC_healing;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.InputStream;

public class MainActivity extends Activity {
    private static final String TAG = "MainActivity";
    ImageView mImgView;
    DrawView mDrawView;
    Matrix mMatrix = new Matrix();
    Matrix mInverseMatrix = new Matrix();
    Bitmap mDisplayedImage;
    Bitmap mImage2;
    RenderScript mRs;
    ScriptC_healing mHealingScript;
    ScriptC_find_region mFindRegion;
    private float mZoom = 0.8f;
    float mYOffset = 0;
    float mXOffset = 0;
    RunScript mRunScript = null;
    private String mImagePath;
    private String mImageName;
    private Region mLastRegion;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mImgView = findViewById(R.id.imageview);
        mDrawView = findViewById(R.id.overlay);
        mDrawView.setImageView(mImgView);
        mRs = RenderScript.create(this.getBaseContext());
        mHealingScript = new ScriptC_healing(mRs);
        mFindRegion = new ScriptC_find_region(mRs);

        mImgView.setOnTouchListener(new View.OnTouchListener() {
            float[] imgPoint = new float[2];
            float[] imgMoveList = new float[100];
            boolean mPanZoomDown = false;

            float mCenterDownX;
            float mCenterDownY;
            float mDistDown;
            float mDownXOffset;
            float mDownYOffset;
            float mDownZoom;
            boolean inMultiTouch = false;

            @Override
            public boolean onTouch(View v, MotionEvent event) {
                int action = event.getAction();
                float x = event.getX();
                float y = event.getY();
                imgPoint[0] = x;
                imgPoint[1] = y;
                int sw = mImgView.getWidth();
                int sh = mImgView.getHeight();
                int iw = mImgView.getDrawable().getIntrinsicWidth();
                int ih = mImgView.getDrawable().getIntrinsicHeight();
                switch (action) {
                    case MotionEvent.ACTION_DOWN:
                        Log.v(TAG, "ACTION_DOWN " + event.getPointerCount());

                        break;
                    case MotionEvent.ACTION_UP:
                        Log.v(TAG, "ACTION_UP " + event.getPointerCount());

                        break;
                    case MotionEvent.ACTION_MOVE:
                        Log.v(TAG, "ACTION_MOVE " + event.getPointerCount());
                        break;
                }
                if (event.getPointerCount() > 1) {
                    inMultiTouch = true;
                }
                if (event.getPointerCount() == 2) {
                    float x1 = event.getX(0);
                    float y1 = event.getY(0);
                    float x2 = event.getX(1);
                    float y2 = event.getY(1);
                    if (mPanZoomDown) {
                        float dx = (x1 + x2) / 2 - mCenterDownX;
                        float dy = (y1 + y2) / 2 - mCenterDownY;
                        float zoom = (float) Math.hypot(x1 - x2, y1 - y2);
                        mZoom = zoom * mDownZoom / mDistDown;

                        float scale = mZoom * Math.min(sw / (float) iw, sh / (float) ih);
                        mXOffset = mDownXOffset + 2 * (dx / (sw - scale * iw));
                        mYOffset = mDownYOffset + 2 * (dy / (sh - scale * ih));
                        if (Math.abs(mXOffset) > 1) {
                            mXOffset = Math.signum(mXOffset);
                        }
                        if (Math.abs(mYOffset) > 1) {
                            mYOffset = Math.signum(mYOffset);
                        }
                    } else {
                        mDrawView.undo();
                        mPanZoomDown = true;
                        mCenterDownX = (x1 + x2) / 2;
                        mCenterDownY = (y1 + y2) / 2;
                        mDistDown = (float) Math.hypot(x1 - x2, y1 - y2);
                        mDownXOffset = mXOffset;
                        mDownYOffset = mYOffset;
                        mDownZoom = mZoom;
                    }
                } else {
                    if (mPanZoomDown) {
                        mPanZoomDown = false;
                    }
                }
                if (!mPanZoomDown) {
                    switch (action) {
                        case MotionEvent.ACTION_DOWN:
                            mInverseMatrix.mapPoints(imgPoint);
                            mDrawView.clearDrawables();
                            mDrawView.downPoint(imgPoint);
                            mDrawView.invalidate();

                            break;
                        case MotionEvent.ACTION_UP:
                            if (inMultiTouch && event.getPointerCount() == 1) {
                                inMultiTouch = false;
                            } else {
                                mInverseMatrix.mapPoints(imgPoint);
                                mDrawView.upPoint(imgPoint);
                                mDrawView.invalidate();
                            }

                            break;
                        case MotionEvent.ACTION_MOVE:

                            int size = event.getHistorySize();
                            size = Math.min(size, imgMoveList.length / 2);
                            for (int i = 0; i < size; i++) {
                                imgMoveList[i * 2] = event.getHistoricalX(size - i - 1);
                                imgMoveList[i * 2 + 1] = event.getHistoricalY(size - i - 1);
                            }
                            mInverseMatrix.mapPoints(imgMoveList, 0, imgMoveList, 0, size);
                            if (!inMultiTouch) {
                                mDrawView.movePoint(imgMoveList, size);
                                mDrawView.invalidate();
                            }
                            break;
                    }
                }
                updateMatrix();

                return true;
            }
        });


        new AsyncLoad().execute();
    }

    void updateMatrix() {
        int sw = mImgView.getWidth();
        int sh = mImgView.getHeight();
        int iw = mImgView.getDrawable().getIntrinsicWidth();
        int ih = mImgView.getDrawable().getIntrinsicHeight();


        mMatrix.reset();
        float scale = mZoom * Math.min(sw / (float) iw, sh / (float) ih);
        mMatrix.postTranslate((1 + mXOffset) * (sw - iw * scale) / 2,
                (1 + mYOffset) * (sh - ih * scale) / 2);
        mMatrix.preScale(scale, scale);
        boolean ret = mMatrix.invert(mInverseMatrix);
        if (!ret) {
            Log.e(TAG, "Fail to invert");
        }
        mImgView.setImageMatrix(mMatrix);
        mImgView.invalidate();
        mDrawView.invalidate();
     }

    void getScreenCoord(float[] point) {
        Matrix matrix = mImgView.getImageMatrix();
    }
    class AsyncLoad extends AsyncTask<Void, Void, Void> {

        protected Void doInBackground(Void... regions) {
            Intent intent = getIntent();

            if (intent != null) {
                File folder = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS);
                mImagePath = folder.getPath();

                String s = intent.getType();
                if (s != null && s.indexOf("image/") != -1) {
                    Uri data = (Uri) intent.getParcelableExtra(Intent.EXTRA_STREAM);
                    mImageName = "edit"+data.getLastPathSegment(); // TODO the wrong way to do this

                    if (data != null) {
                        InputStream input = null;
                        try {
                            input = getContentResolver().openInputStream(data);
                            mDisplayedImage = BitmapFactory.decodeStream(input);

                            return null;
                        } catch (FileNotFoundException e) {
                            e.printStackTrace();
                        }

                    }
                }
            }

            getLocalImage();
            return null;
        }

        @Override
        protected void onPostExecute(Void s) {
            mImgView.setImageBitmap(mDisplayedImage);
            Log.v(TAG, "BITMAP SIZE = " + mDisplayedImage.getWidth() + "," +
                    mDisplayedImage.getHeight());

            updateMatrix();
        }
    }
    void getLocalImage() {

        File folder;
        folder = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS);
        mImagePath = folder.getPath();
        File[] files = folder.listFiles();
        if (files != null) {
            Log.v(TAG, "files" + files.length);
            for (int i = 0; i < files.length; i++) {
                Log.v(TAG, "[" + i + "]=" + files[i].getAbsolutePath());
                if (files[i].getName().toLowerCase().endsWith(".jpg")) {
                    mDisplayedImage = BitmapFactory.decodeFile(files[i].getAbsolutePath());
                    mImagePath = files[i].getParentFile().getAbsolutePath();
                    mImageName = files[i].getName();
                    return;
                }
            }
        }

        mDisplayedImage = BitmapFactory.decodeResource(this.getResources(), R.drawable.bugs);
        mImageName = "bugs";
    }

    public void heal(View v) {
        mLastRegion = mDrawView.getRegion(mDisplayedImage);
        if (mRunScript == null) {
            mRunScript = new RunScript();
            mRunScript.execute(mLastRegion);
        }
    }

    public void undo(View v) {
        if (mImage2 != null) {
            mLastRegion.undo(mImage2);
            mDrawView.invalidate();
        }
    }

    public void save(View v) {
        String name = mImageName;
        if (name.indexOf(".") > 0) {
            name = name.substring(0, name.lastIndexOf(".")) + "_e";
        }
        MediaStoreSaver.save(mImage2,
                mImagePath,
                name,
                this,
                MediaStoreSaver.TYPE_JPG);
        Toast.makeText(this, "Saved " + name, Toast.LENGTH_SHORT).show();
    }

    class RunScript extends AsyncTask<Region, String, String> {
        Drawable d;

        protected String doInBackground(Region... regions) {
            d = regions[0].findMatch(mFindRegion, mRs, mDisplayedImage);

            if (mImage2 == null) {
                mImage2 = mDisplayedImage.copy(Bitmap.Config.ARGB_8888, true);

            }
            regions[0].heal(mHealingScript, mRs, mImage2, mImage2);

            return "";
        }

        @Override
        protected void onPostExecute(String s) {
            super.onPostExecute(s);
            mDrawView.addDrawable(d);
            mDrawView.invalidate();
            mImgView.setImageBitmap(mImage2);
            mRunScript = null;
        }
    }

}
