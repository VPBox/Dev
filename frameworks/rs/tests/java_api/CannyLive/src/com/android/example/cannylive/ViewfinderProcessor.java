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

package com.android.example.cannylive;

import android.content.ContentResolver;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.ImageFormat;
import android.net.Uri;
import android.os.Handler;
import android.os.HandlerThread;
import android.renderscript.Allocation;
import android.renderscript.Element;
import android.renderscript.RenderScript;
import android.renderscript.Script;
import android.renderscript.Type;
import android.util.Log;
import android.util.Size;
import android.view.Surface;

import java.io.FileNotFoundException;
import java.text.DecimalFormat;

/**
 * Renderscript-based Focus peaking viewfinder
 */
public class ViewfinderProcessor {
    private static final String TAG = "ViewfinderProcessor";
    int mCount;
    long mLastTime;
    float mFps;
    RenderScript mRs;
    private Allocation mInputAllocation;
    private Allocation mOutputAllocation;
    private Allocation mBlurAllocation;
    private Allocation mEdgeAllocation;
    private HandlerThread mProcessingThread;
    private Handler mProcessingHandler;
    private ScriptC_canny mScriptCanny;
    public ProcessingTask mProcessingTask;
    public Allocation mHoughOutput;
    public Allocation mHoughSlices;
    private volatile int mMode = 1;
    DecimalFormat df = new DecimalFormat("###.##");

    public ViewfinderProcessor(RenderScript rs, Size dimensions) {
        mRs = rs;
        Type.Builder yuvTypeBuilder = new Type.Builder(rs, Element.YUV(rs));
        yuvTypeBuilder.setX(dimensions.getWidth());
        yuvTypeBuilder.setY(dimensions.getHeight());
        yuvTypeBuilder.setYuvFormat(ImageFormat.YUV_420_888);
        Log.d(TAG, ">>>>>>>>>>>>  " + dimensions.getWidth() + "x" + dimensions.getHeight());
        mInputAllocation = Allocation.createTyped(rs, yuvTypeBuilder.create(),
                Allocation.USAGE_IO_INPUT | Allocation.USAGE_SCRIPT);

        Type.Builder rgbTypeBuilder = new Type.Builder(rs, Element.RGBA_8888(rs));
        rgbTypeBuilder.setX(dimensions.getWidth());
        rgbTypeBuilder.setY(dimensions.getHeight());

        mOutputAllocation = Allocation.createTyped(rs, rgbTypeBuilder.create(),
                Allocation.USAGE_IO_OUTPUT | Allocation.USAGE_SCRIPT);
        Type.Builder buffTypeBuilder = new Type.Builder(rs, Element.U8(rs));
        buffTypeBuilder.setX(dimensions.getWidth());
        buffTypeBuilder.setY(dimensions.getHeight());
        mBlurAllocation = Allocation.createTyped(rs, buffTypeBuilder.create());
        mEdgeAllocation = Allocation.createTyped(rs, buffTypeBuilder.create());

        mProcessingThread = new HandlerThread("ViewfinderProcessor");
        mProcessingThread.start();
        mProcessingHandler = new Handler(mProcessingThread.getLooper());
        mScriptCanny = new ScriptC_canny(rs);
        mScriptCanny.set_blurImage(mBlurAllocation);
        mScriptCanny.set_edgeImage(mEdgeAllocation);
        mProcessingTask = new ProcessingTask(mInputAllocation);

        int NO_OF_SLICES = 8;
        int[] slices = new int[NO_OF_SLICES * 2];
        for (int i = 0; i < NO_OF_SLICES; i++) {
            int s1 = i * 360 / NO_OF_SLICES;
            int s2 = ((1 + i) * 360) / NO_OF_SLICES;
            slices[i * 2] = s1;
            slices[i * 2 + 1] = s2;
        }
        Type.Builder houghSliceBuilder = new Type.Builder(rs, Element.I32_2(rs));
        houghSliceBuilder.setX(NO_OF_SLICES);
        mHoughSlices = Allocation.createTyped(rs, houghSliceBuilder.create(), Allocation.USAGE_SCRIPT);
        mHoughSlices.copyFrom(slices);
        Type.Builder houghOutputBuilder = new Type.Builder(rs, Element.U8(rs));
        houghOutputBuilder.setX(800);
        houghOutputBuilder.setY(360);
        mHoughOutput = Allocation.createTyped(rs, houghOutputBuilder.create());
        mScriptCanny.set_hough_output(mHoughOutput);

    }

    public Surface getInputSurface() {
        return mInputAllocation.getSurface();
    }

    public void setOutputSurface(Surface output) {
        mOutputAllocation.setSurface(output);
    }

    public float getmFps() {
        return mFps;
    }

    public void changeEffectMode() {
        mMode++;
    }

    public int getMode() {
        return mMode;
    }

    volatile boolean mStop = false;

    public void close() {

        mStop = true;
    }

    /**
     * Class to process buffer from camera and output to buffer to screen
     */
    class ProcessingTask implements Runnable, Allocation.OnBufferAvailableListener {
        private int mPendingFrames = 0;
        int mode = -1;
        private Allocation mInputAllocation;

        public ProcessingTask(Allocation input) {
            mInputAllocation = input;
            mInputAllocation.setOnBufferAvailableListener(this);
        }

        @Override
        public void onBufferAvailable(Allocation a) {
            if (mStop) {

                return;
            }
            synchronized (this) {
                mPendingFrames++;
                mProcessingHandler.post(this);
            }
        }

        @Override
        public void run() {
            // Find out how many frames have arrived
            int pendingFrames;
            synchronized (this) {
                pendingFrames = mPendingFrames;
                mPendingFrames = 0;

                // Discard extra messages in case processing is slower than frame rate
                mProcessingHandler.removeCallbacks(this);
            }
            if (mInputAllocation == null) return;
            // Get to newest input
            for (int i = 0; i < pendingFrames; i++) {
                mInputAllocation.ioReceive();
            }
            mCount++;
            mScriptCanny.set_gCurrentFrame(mInputAllocation);
            long time = System.currentTimeMillis() - mLastTime;
            if (time > 1000) {
                mLastTime += time;
                mFps = mCount * 1000 / (float) (time);
                mCount = 0;
            }
            // Run processing pass
            mScriptCanny.forEach_getyuv_y(mEdgeAllocation);

            Script.LaunchOptions opt = new Script.LaunchOptions();
            opt.setX(2, mBlurAllocation.getType().getX() - 2);
            opt.setY(2, mBlurAllocation.getType().getY() - 2);
            mScriptCanny.forEach_blur_uchar(mBlurAllocation, opt);

            opt.setX(3, mBlurAllocation.getType().getX() - 3);
            opt.setY(3, mBlurAllocation.getType().getY() - 3);
            mScriptCanny.forEach_edge(mEdgeAllocation, opt);

            opt.setX(4, mBlurAllocation.getType().getX() - 4);
            opt.setY(4, mBlurAllocation.getType().getY() - 4);
            mScriptCanny.forEach_thin(mBlurAllocation, opt);

            opt.setX(5, mBlurAllocation.getType().getX() - 5);
            opt.setY(5, mBlurAllocation.getType().getY() - 5);
            mScriptCanny.forEach_hysteresis(mBlurAllocation, mEdgeAllocation, opt);

            switch (mMode % 6) {
                case 0:
                default:
                    long mt = System.nanoTime();
                    mScriptCanny.forEach_black_uchar(mHoughOutput);
                    mScriptCanny.forEach_hough(mHoughSlices);
                    mRs.finish();
                    mt = System.nanoTime() - mt;
                    Log.v(TAG, " hough = " + df.format(mt * 1E-6) + "ms");
                    mScriptCanny.forEach_hough_map(mOutputAllocation);
                    break;
                case 1:
                    mScriptCanny.forEach_toRGB(mOutputAllocation, opt);
                    break;
                case 2:
                    mScriptCanny.forEach_toRGBfuzz(mOutputAllocation, opt);
                    break;
                case 3:
                    mScriptCanny.forEach_toWhiteRGBfuzz(mOutputAllocation, opt);
                    break;
                case 4:
                    mScriptCanny.forEach_toWhiteRGB(mOutputAllocation, opt);
                    break;
                case 5:
                    mScriptCanny.forEach_toCartoon(mOutputAllocation, opt);
                    break;
            }
            mOutputAllocation.ioSend();
            if (mStop) {
                if (mInputAllocation != null) {
                    mInputAllocation.destroy();
                    mInputAllocation = null;
                }
                return;
            }
        }
    }


    public static void reProcessImage(Context context, String urlName, int type) {

        ContentResolver cr = context.getContentResolver();
        try {
            Uri uri = Uri.parse(urlName);
            Bitmap b = BitmapFactory.decodeStream(cr.openInputStream(uri));
            processImage(b, context, type);

            MediaStoreSaver.insertImage(cr, b, "canny", "canny filtered image");
        } catch (FileNotFoundException e) {
            Log.v(TAG, "S>> Could not open file ");
        }

    }

    public static void processImage(Bitmap image, Context context, int mMode) {
        RenderScript mRs = RenderScript.create(context);
        int width = image.getWidth();
        int height = image.getHeight();
        Allocation img_alloc, blur_alloc, edge_alloc;
        long time = System.nanoTime();
        img_alloc = Allocation.createFromBitmap(mRs, image);

        Type.Builder buffTypeBuilder = new Type.Builder(mRs, Element.U8(mRs));
        buffTypeBuilder.setX(width).setY(height);
        blur_alloc = Allocation.createTyped(mRs, buffTypeBuilder.create());
        edge_alloc = Allocation.createTyped(mRs, buffTypeBuilder.create());


        ScriptC_canny canny_script = new ScriptC_canny(mRs);
        canny_script.set_blurImage(blur_alloc);
        canny_script.set_edgeImage(edge_alloc);
        canny_script.forEach_getLum(img_alloc, edge_alloc);

        Script.LaunchOptions opt = new Script.LaunchOptions();
        opt.setX(2, blur_alloc.getType().getX() - 2);
        opt.setY(2, blur_alloc.getType().getY() - 2);
        canny_script.forEach_blur_uchar(blur_alloc, opt);

        opt.setX(3, blur_alloc.getType().getX() - 3);
        opt.setY(3, blur_alloc.getType().getY() - 3);
        canny_script.forEach_edge(edge_alloc, opt);

        opt.setX(4, blur_alloc.getType().getX() - 4);
        opt.setY(4, blur_alloc.getType().getY() - 4);
        canny_script.forEach_thin(blur_alloc, opt);

        opt.setX(5, blur_alloc.getType().getX() - 5);
        opt.setY(5, blur_alloc.getType().getY() - 5);

        canny_script.forEach_hysteresis(blur_alloc, edge_alloc, opt);
        switch (mMode % 6) {
            case 0:
            case 1:
            default:
                canny_script.forEach_toRGB(img_alloc, opt);
                break;
            case 2:
                canny_script.forEach_toRGBfuzz(img_alloc, opt);
                break;
            case 3:
                canny_script.forEach_toWhiteRGBfuzz(img_alloc, opt);
                break;
            case 4:
                canny_script.forEach_toWhiteRGB(img_alloc, opt);
                break;
            case 5:
                canny_script.forEach_toRGBCartoon(img_alloc, img_alloc, opt);
                break;
        }
        img_alloc.copyTo(image);
        time = System.nanoTime() - time;
        DecimalFormat df = new DecimalFormat("###.#");
        String ts = df.format(time * 1E-6) + "ms";
        Log.v(TAG, "processed a " + width + "x" + height + " in " + ts);
    }

}
