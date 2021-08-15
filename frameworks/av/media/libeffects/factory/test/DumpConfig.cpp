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

#include <media/EffectsFactoryApi.h>
#include <unistd.h>
#include "EffectsXmlConfigLoader.h"
#include "EffectsConfigLoader.h"

int main(int argc, char* argv[]) {
    const char* path = nullptr;
    bool legacyFormat;

    if (argc == 2 && strcmp(argv[1], "--legacy") == 0) {
        legacyFormat = true;
        fprintf(stderr, "Dumping legacy effect config file\n");
    } else if ((argc == 2 || argc == 3) && strcmp(argv[1], "--xml") == 0) {
        legacyFormat = false;
        if (argc == 3) {
            fprintf(stderr, "Dumping XML effect config file: %s\n", path);
        } else {
            fprintf(stderr, "Dumping default XML effect config file.\n");
        }
    } else {
        fprintf(stderr, "Invalid arguments.\n"
                        "Usage: %s [--legacy|--xml [FILE]]\n", argv[0]);
        return 1;
    }

    if (!legacyFormat) {
        ssize_t ret = EffectLoadXmlEffectConfig(path);
        if (ret < 0) {
            fprintf(stderr, "loadXmlEffectConfig failed, see logcat for detail.\n");
            return 2;
        }
        if (ret > 0) {
            fprintf(stderr, "Partially failed to load config. Skipped %zu elements, "
                    "see logcat for detail.\n", (size_t)ret);
        }
    }

    if (legacyFormat) {
        auto ret = EffectLoadEffectConfig();
        if (ret < 0) {
            fprintf(stderr, "loadEffectConfig failed, see logcat for detail.\n");
            return 3;
        }
        fprintf(stderr, "legacy loadEffectConfig has probably succeed, see logcat to make sure.\n");
    }

    if (EffectDumpEffects(STDOUT_FILENO) != 0) {
        fprintf(stderr, "Effect dump failed, see logcat for detail.\n");
        return 4;
    }
}
