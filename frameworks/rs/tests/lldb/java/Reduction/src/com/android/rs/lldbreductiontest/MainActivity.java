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

/* This application is a simple scheduler for testing the lldb debugger
 * implementation for general reduction kernels.
 *
 * It launches one of two simple reductions in a loop
 */

package com.android.rs.lldbreductiontest;

import android.app.Activity;
import android.content.Context;
import android.content.res.Resources;
import android.os.Handler;
import android.os.Bundle;
import android.util.Log;
import android.renderscript.*;

import java.lang.Float;
import java.lang.Math;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Random;

public class MainActivity extends Activity {
  static private int idxOffset = 10;
  static private int mX = 128;
  static private int mY = 2;
  static private int mZ = 2;
  static private float mMultiplier = 2.f;
  private RenderScript mRS;
  private ScriptC_reduce mScript;
  private ScriptC_reduce_auto_comb mScript_auto_comb;

  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    initRS();

    int loopDelayMillis = 1000;
    Handler loopHandler = new Handler();
    loopHandler.postDelayed(new Runnable() {
      @Override
      public void run() {
        runRS();
        loopHandler.postDelayed(this, loopDelayMillis);
      }
    }, loopDelayMillis);
  }

  private float findMinUserTypeAutoComb(
      RenderScript rs, ScriptC_reduce_auto_comb s, Allocation alloc) {
    s.set_a_startval(mX);
    s.set_b_startval(mY);
    s.set_multiplier(mMultiplier);

    return s.reduce_find_min_user_type_auto_comb(alloc).get();
  }

  private float findMinUserType(RenderScript rs, ScriptC_reduce s, Allocation alloc) {
    s.set_a_startval(mX);
    s.set_b_startval(mY);
    s.set_multiplier(mMultiplier);

    return s.reduce_find_min_user_type(alloc).get();
  }

  private float findMinUserType1DAutoComb(RenderScript rs, ScriptC_reduce_auto_comb s, int xCount) {
    ScriptField_MinUserType minUserType = new ScriptField_MinUserType(rs, xCount);
    for (int i = 0; i < xCount; i++) {
      ScriptField_MinUserType.Item val = new ScriptField_MinUserType.Item();
      val.a = i + idxOffset;
      val.b = i + idxOffset;
      minUserType.set(val, i, true);
    }

    Allocation alloc = minUserType.getAllocation();

    return findMinUserTypeAutoComb(rs, s, alloc);
  }

  private float findMinUserType1D(RenderScript rs, ScriptC_reduce s, int xCount) {
    ScriptField_MinUserType minUserType = new ScriptField_MinUserType(rs, xCount);
    for (int i = 0; i < xCount; i++) {
      ScriptField_MinUserType.Item val = new ScriptField_MinUserType.Item();
      val.a = i + idxOffset;
      val.b = i + idxOffset;
      minUserType.set(val, i, true);
    }

    Allocation alloc = minUserType.getAllocation();

    return findMinUserType(rs, s, alloc);
  }

  public void initRS() {
    mRS = RenderScript.create(this, RenderScript.ContextType.NORMAL,
        RenderScript.CREATE_FLAG_LOW_LATENCY | RenderScript.CREATE_FLAG_WAIT_FOR_ATTACH);
    mScript = new ScriptC_reduce(mRS);
    mScript_auto_comb = new ScriptC_reduce_auto_comb(mRS);
  }

  public void runRS() {
    findMinUserType1D(mRS, mScript, mX);
    findMinUserType1DAutoComb(mRS, mScript_auto_comb, mX);
  }

  public void onDestroy() {
    mRS.finish();
    mRS.destroy();
  }
}
