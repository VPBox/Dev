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

package com.android.rs.imagejb;

import java.util.ArrayList;

class Result {

    private ArrayList<Float> timeList;

    Result() {
        timeList = new ArrayList<Float>();
    }

    // Add time
    public void add(float time) {
        timeList.add(time);
    }

    // Get total execution time
    public float getTotalTime() {
        float total = 0.0f;
        for (float time : timeList) {
            total += time;
        }
        return total;
    }

    // Get number of iterations
    public int getIterations() {
        return timeList.size();
    }

    // Get the average of an array list
    public float getAvg() {
        return getTotalTime() / getIterations();
    }

    // Get the biased ("full population") standard deviation
    public float getStdevp() {
        float sumSqDiff = 0;
        float avg = getAvg();
        for (float time : timeList) {
            float diff = time - avg;
            sumSqDiff += diff * diff;
        }
        return (float) Math.sqrt(sumSqDiff / getIterations());
    }

    // Get the normalized standard deviation ("standard deviation coefficient")
    public float getStdCoef() {
        return getStdevp() / getAvg();
    }

    // Get all times as float[]
    public float[] getTimes() {
        float[] array = new float[getIterations()];
        for (int i = 0; i < getIterations(); i++) {
            array[i] = timeList.get(i);
        }
        return array;
    }
}
