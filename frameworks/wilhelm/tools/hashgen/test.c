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

// Test the minimal perfect hash generator function

#include <stdio.h>
#include <stdlib.h>
#include <SLES/OpenSLES.h>
#include "MPH.h"

extern int IID_to_MPH(const SLInterfaceID iid);
extern const struct SLInterfaceID_ SL_IID_array[MPH_MAX];

int main(int argc, char **argv)
{
    int i;
    for (i = 0; i < MPH_MAX; i++) {
        const struct SLInterfaceID_ *original = &SL_IID_array[i];
        // test the address-based lookup
        int MPH = IID_to_MPH(original);
        if (MPH != i) {
            fprintf(stderr, "error: IID_to_MPH(SL_IID_array) = %d != %d\n", MPH, i);
            return EXIT_FAILURE;
        }
        // test the content-based lookup
        const struct SLInterfaceID_ copy = *original;
        MPH = IID_to_MPH(&copy);
        if (MPH != i) {
            fprintf(stderr, "error: IID_to_MPH(copy) = %d != %d\n", MPH, i);
            return EXIT_FAILURE;
        }
    }
    printf("OK\n");
    return EXIT_SUCCESS;
}
