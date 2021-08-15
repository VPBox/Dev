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
#pragma rs java_package_name(com.android.rs.cppmultiplersfiles)

float4 gColor = {0.299f, 0.587f, 0.114f, 1.f};

/* RenderScript kernel that just sets the colour of the screen and does some
 * simple operations so it is not completely empty
 * (and can therefore be debugged).
 */
uchar4 __attribute__((kernel)) first_kernel(uchar4 in)
{
    float4 temp = rsUnpackColor8888(in);
    temp = gColor;
    uchar4 result = rsPackColorTo8888(temp);
    return result;
}
