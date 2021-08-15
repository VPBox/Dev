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

#pragma version(1)

#pragma rs java_package_name(com.android.rs.rsov.test)

float4 RS_KERNEL invert(float4 a) {
    return 1.0f - a;
}

float4 RS_KERNEL scalebytwo(float4 a) {
    return 2.0f * a;
}

float4 RS_KERNEL scalebythree(float4 a) {
    return 3.0f * a;
}
