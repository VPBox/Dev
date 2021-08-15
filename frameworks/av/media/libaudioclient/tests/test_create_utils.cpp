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

#include "test_create_utils.h"

namespace android {

int readLine(FILE *inputFile, char *line, int size) {
    int ret = 0;
    while (true) {
        char *str = fgets(line, size, inputFile);
        if (str == nullptr) {
            ret = -1;
            break;
        }
        if (feof(inputFile) != 0 || ferror(inputFile) != 0) {
            ret = -1;
            break;
        }
        if (strlen(str) != 0 && str[0] != COMMENT_CHAR) {
            break;
        }
    }
    return ret;
}

bool checkVersion(FILE *inputFile, const char *version)
{
    char line[MAX_INPUT_FILE_LINE_LENGTH];
    char versionKey[MAX_INPUT_FILE_LINE_LENGTH];
    char versionValue[MAX_INPUT_FILE_LINE_LENGTH];

    if (readLine(inputFile, line, MAX_INPUT_FILE_LINE_LENGTH) != 0) {
        fprintf(stderr, "Missing version in input file\n");
        return false;
    }

    if (sscanf(line, " %s %s", versionKey, versionValue) != 2) {
        fprintf(stderr, "Malformed version in input file\n");
        return false;
    }
    if (strcmp(versionKey, VERSION_KEY) != 0) {
        fprintf(stderr, "Malformed version in input file\n");
        return false;
    }
    if (strcmp(versionValue, version) != 0) {
        fprintf(stderr, "Wrong input file version %s expecting %s\n", versionValue, version);
        return false;
    }
    return true;
}

void callback(int event __unused, void* user __unused, void *info __unused)
{
}

int main(int argc, char **argv, test_func_t testFunc)
{
    FILE *inputFile = nullptr;
    int outputFileFd = STDOUT_FILENO;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    int ret = 0;

    if (argc > 5) {
        fprintf(stderr, "Usage: %s [-i input_params.txt] [-o output_params.txt]\n", argv[0]);
        return 1;
    }

    argv++;
    while (*argv) {
        if (strcmp(*argv, "-i") == 0) {
            argv++;
            if (*argv) {
                inputFile = fopen(*argv, "r");
                if (inputFile == nullptr) {
                    ret = 1;
                }
            } else {
                ret = 1;
            }
        }
        if (strcmp(*argv, "-o") == 0) {
            argv++;
            if (*argv) {
                outputFileFd = open(*argv, O_WRONLY|O_CREAT, mode);
                if (outputFileFd < 0) {
                    ret = 1;
                }
            } else {
                ret = 1;
            }
            argv++;
        }
        if (*argv) {
            argv++;
        }
    }

    if (ret != 0) {
        return ret;
    }

    ret = testFunc(inputFile, outputFileFd);

    if (inputFile) {
        fclose(inputFile);
    }
    if (outputFileFd >= 0 && outputFileFd != STDOUT_FILENO) {
        close(outputFileFd);
    }

    return ret;
}

}; // namespace android

