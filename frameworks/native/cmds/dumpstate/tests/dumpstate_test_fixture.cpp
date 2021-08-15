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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define LOG_TAG "dumpstate"
#include <cutils/log.h>

void PrintDefaultOutput() {
    fprintf(stdout, "stdout\n");
    fflush(stdout);
    fprintf(stderr, "stderr\n");
    fflush(stderr);
}

/*
 * Binary used to on RunCommand tests.
 *
 * Usage:
 *
 * - Unless stated otherwise this command:
 *
 *   1.Prints `stdout\n` on `stdout` and flushes it.
 *   2.Prints `stderr\n` on `stderr` and flushes it.
 *   3.Exit with status 0.
 *
 * - If 1st argument is '--pid', it first prints its pid on `stdout`.
 *
 * - If 1st argument is '--uid', it first prints its uid on `stdout`.
 *
 * - If 1st argument is '--crash', it uses ALOGF to crash and returns 666.
 *
 * - With argument '--exit' 'CODE', returns CODE;
 *
 * - With argument '--sleep 'TIME':
 *
 *   1.Prints `stdout line1\n` on `stdout` and `sleeping TIME s\n` on `stderr`
 *   2.Sleeps for TIME s
 *   3.Prints `stdout line2\n` on `stdout` and `woke up\n` on `stderr`
 */
int main(int argc, char* const argv[]) {
    if (argc == 2) {
        if (strcmp(argv[1], "--crash") == 0) {
            PrintDefaultOutput();
            LOG_FATAL("D'OH\n");
            return 666;
        }
    }
    if (argc == 3) {
        if (strcmp(argv[1], "--exit") == 0) {
            PrintDefaultOutput();
            return atoi(argv[2]);
        }
    }

    if (argc > 1) {
        int index = 1;

        // First check arguments that can shift the index.
        if (strcmp(argv[1], "--pid") == 0) {
            index++;
            fprintf(stdout, "%d\n", getpid());
            fflush(stdout);
        } else if (strcmp(argv[1], "--uid") == 0) {
            index++;
            fprintf(stdout, "%d\n", getuid());
            fflush(stdout);
        }

        // Then the "common" arguments, if any.
        if (argc > index + 1) {
            if (strcmp(argv[index], "--sleep") == 0) {
                int napTime = atoi(argv[index + 1]);
                fprintf(stdout, "stdout line1\n");
                fflush(stdout);
                fprintf(stderr, "sleeping for %ds\n", napTime);
                fflush(stderr);
                sleep(napTime);
                fprintf(stdout, "stdout line2\n");
                fflush(stdout);
                fprintf(stderr, "woke up\n");
                fflush(stderr);
                return 0;
            }
        }
    }

    PrintDefaultOutput();
    return 0;
}
