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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <audio_utils/limiter.h>

int main(int argc, char **argv)
{
    int i;
    if (argc > 1) {
        for (i = 1; i < argc; i++) {
            float x = atof(argv[i]);
            printf("limit(%g)=%g\n", x, limiter(x));
        }
    } else {
        for (i = 0; i <= 150; i++) {
            float in = (float) ((double) i * 0.01);
            float out = limiter(in);
            printf("%g,%g\n", in, out);
            if (i != 0) {
                out = limiter(-in);
                printf("%g,%g\n", -in, out);
            }
        }
    }
    return EXIT_SUCCESS;
}
