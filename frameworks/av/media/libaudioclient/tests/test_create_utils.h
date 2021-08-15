/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT_FILE_LINE_LENGTH 512
#define MAX_OUTPUT_FILE_LINE_LENGTH 512

#define COMMENT_CHAR '#'
#define VERSION_KEY "version"

namespace android {

int readLine(FILE *inputFile, char *line, int size);

bool checkVersion(FILE *inputFile, const char *version);

void callback(int event, void* user, void *info);

typedef int (*test_func_t)(FILE *inputFile, int outputFileFd);

int main(int argc, char **argv, test_func_t testFunc);

}; // namespace android
