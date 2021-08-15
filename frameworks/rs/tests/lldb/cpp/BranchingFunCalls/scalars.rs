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
#pragma rs java_package_name(com.android.rs.cppbranchingfuncalls)

static bool is_neg(int a)
{
    if(a < 0)
        return true;
    else
        return false;
}

static bool is_pos(int a)
{
    if(a > 0)
        return true;
    else
        return false;
}

static void set_i(int * a, int b)
{
    int tmp = b;
    *a = tmp;
}

static void modify_f(float * f)
{
    *f *= 0.5f;
}

static void modify_i(int * i)
{
    int j = *i;
    int cutoff = 2 << 6;
    if(j > cutoff)
        j = cutoff;
    if(is_neg(j))
        set_i(i, 0);
    else if(is_pos(j))
        set_i(i, j);
    else
        set_i(i, cutoff);
}

int __attribute__((kernel)) simple_kernel(int in)
{
    int i = in;
    float f = (float) i;
    modify_f(&f);
    modify_i(&i);
    int ret = (int) f;
    return in * ret;
}

int glob = 123;

void addToGlobal(int arg)
{
    glob += arg;
}
