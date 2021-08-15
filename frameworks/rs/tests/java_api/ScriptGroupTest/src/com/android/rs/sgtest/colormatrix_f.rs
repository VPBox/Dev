/*
 * Copyright (C) 2012 The Android Open Source Project
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

#include "ip.rsh"
// #pragma rs_fp_relaxed

static rs_matrix4x4 Mat = {
       {
1.0f, 0, 0, 0,
0.2f, 0.9f, 0.2f, 0,
0, 0, 1.0f, 0,
0, 0, 0, 1.0f,
       }
};

/*
void init() {
     rsMatrixLoadIdentity(&Mat);
}
*/

void setMatrix(rs_matrix4x4 m) {
    Mat = m;
}

float4 RS_KERNEL colormatrix(float4 in) {
    return rsMatrixMultiply(&Mat, in);
}

