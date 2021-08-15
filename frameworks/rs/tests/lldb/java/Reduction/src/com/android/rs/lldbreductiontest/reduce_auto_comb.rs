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

#pragma rs java_package_name(com.android.rs.lldbreductiontest)
#pragma version(1)
#pragma rs reduce(find_min_user_type_auto_comb)                                \
    initializer(find_min_user_type_init) accumulator(find_min_user_type_accum) \
        outconverter(find_min_user_type_outc)                                  \

#include "reduce_common.rsh"
