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
#pragma rs java_package_name(com.android.rs.cppallocations)

// Kernel performs basic vector swizzle
uchar4 __attribute__((kernel)) swizzle_kernel(uchar4 in)
{
    return in.wzyx;
}

// Kernel squares every element in allocation
uint __attribute__((kernel)) square_kernel(ushort in)
{
    uint result = (uint)in * (uint)in;
    return result;
}

// Helper function adding 1/2 to passed in double
static double half_helper(double in)
{
    return (in + 0.5);
}

// Kernel returns first 3 elements of a double4 plus 1/2
double3 __attribute__((kernel)) add_half_kernel(double4 in)
{
    double3 result;
    result.x = half_helper(in.x);
    result.y = half_helper(in.y);
    result.z = half_helper(in.z);
    return result;
}
