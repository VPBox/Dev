/*
* Copyright (C) 2016 The Android Open Source Project
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

package com.android.rs.kernelvariables;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.ImageFormat;
import android.os.Bundle;
import android.widget.ImageView;
import android.renderscript.*;

public class MainActivity extends Activity {
    private Bitmap mBitmapIn;
    private Bitmap mBitmapOut;
    private ImageView mImageView;

    private RenderScript mRS;
    private Allocation mInAllocation;
    private Allocation mOutAllocation;
    private ScriptC_simple mScript;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.main_layout);

        mBitmapIn = Bitmap.createBitmap(500, 500, Bitmap.Config.ARGB_8888);
        mBitmapOut = Bitmap.createBitmap(mBitmapIn.getWidth(),
                    mBitmapIn.getHeight(), mBitmapIn.getConfig());

        mImageView = findViewById(R.id.imageView);
        mImageView.setImageBitmap(mBitmapOut);

        createScript();
        updateImage();
    }

    private void createScript() {
        mRS = RenderScript.create(this,
            RenderScript.ContextType.NORMAL,
            RenderScript.CREATE_FLAG_LOW_LATENCY |
            RenderScript.CREATE_FLAG_WAIT_FOR_ATTACH);

        mInAllocation = Allocation.createFromBitmap(mRS, mBitmapIn);
        mOutAllocation = Allocation.createFromBitmap(mRS, mBitmapOut);

        mScript = new ScriptC_simple(mRS);
    }

    private void updateImage() {
        int[] buffer_int = {1, 2, 3, 4};
        Allocation int_allocation = Allocation.createSized(mRS, Element.I32(mRS), 4);
        int_allocation.copyFrom(buffer_int);
        mScript.set_allocation_1D_global(int_allocation);

        int[] buffer_int2 = {5, 6, 7, 8};

        Type.Builder typeI32Builder2D = new Type.Builder(mRS, Element.I32(mRS));
        typeI32Builder2D.setX(2);
        typeI32Builder2D.setY(2);

        Allocation int_allocation2 = Allocation.createTyped(mRS, typeI32Builder2D.create());
        int_allocation2.copyFrom(buffer_int2);
        mScript.set_allocation_1D_global2(int_allocation2);

        mScript.set_allocation_2D_global(mInAllocation);
        mScript.set_allocation_2D_global2(mOutAllocation);

        int[] buffer_int3 = new int[64];

        for (int i=0; i<4*4*4; ++i)
            buffer_int3[i] = 9 + i;

        Type.Builder typeI32Builder3D = new Type.Builder(mRS, Element.I32(mRS));
        typeI32Builder3D.setX(4);
        typeI32Builder3D.setY(4);
        typeI32Builder3D.setZ(4);

        Allocation int_allocation3 = Allocation.createTyped(mRS, typeI32Builder3D.create());
        int_allocation3.copyFrom(buffer_int3);
        mScript.set_allocation_3D_global(int_allocation3);

        Type.Builder yuvTypeBuilder = new Type.Builder(mRS, Element.YUV(mRS));
        yuvTypeBuilder.setX(4);
        yuvTypeBuilder.setY(4);
        yuvTypeBuilder.setYuvFormat(ImageFormat.YV12);
        Allocation yuv_allocation = Allocation.createTyped(mRS, yuvTypeBuilder.create());
        mScript.set_allocation_YUV_2D_global(yuv_allocation);

        mScript.set_sampler_global(Sampler.CLAMP_LINEAR(mRS));

        mScript.forEach_kernel(mInAllocation, mOutAllocation);
        mOutAllocation.copyTo(mBitmapOut);
    }
}
