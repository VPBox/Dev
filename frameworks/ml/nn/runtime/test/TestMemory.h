/*
 * Copyright (C) 2018 The Android Open Source Project
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

#ifndef FRAMEWORKS_ML_NN_RUNTIME_TEST_TEST_MEMORY_H
#define FRAMEWORKS_ML_NN_RUNTIME_TEST_TEST_MEMORY_H

#include <stdio.h>

namespace {

typedef float Matrix3x4[3][4];

// Check that the values are the same. This works only if dealing with integer
// value, otherwise we should accept values that are similar if not exact.
int CompareMatrices(const Matrix3x4& expected, const Matrix3x4& actual) {
    int errors = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            if (expected[i][j] != actual[i][j]) {
                printf("expected[%d][%d] != actual[%d][%d], %f != %f\n", i, j, i, j,
                       static_cast<double>(expected[i][j]), static_cast<double>(actual[i][j]));
                errors++;
            }
        }
    }
    return errors;
}

const Matrix3x4 matrix1 = {{1.f, 2.f, 3.f, 4.f}, {5.f, 6.f, 7.f, 8.f}, {9.f, 10.f, 11.f, 12.f}};
const Matrix3x4 matrix2 = {{100.f, 200.f, 300.f, 400.f},
                           {500.f, 600.f, 700.f, 800.f},
                           {900.f, 1000.f, 1100.f, 1200.f}};
const Matrix3x4 matrix3 = {{20.f, 30.f, 40.f, 50.f},
                           {21.f, 22.f, 23.f, 24.f},
                           {31.f, 32.f, 33.f, 34.f}};
const Matrix3x4 expected3 = {{121.f, 232.f, 343.f, 454.f},
                             {526.f, 628.f, 730.f, 832.f},
                             {940.f, 1042.f, 1144.f, 1246.f}};

}  // anonymous namespace

#endif  // FRAMEWORKS_ML_NN_RUNTIME_TEST_TEST_MEMORY_H
