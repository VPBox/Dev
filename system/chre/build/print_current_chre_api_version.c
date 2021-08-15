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

#include <chre/version.h>
#include <stdio.h>

int main(int argc, char **argv) {
  fprintf(stdout, "DEFAULT_CHRE_API_VERSION_MAJOR = 0x%02x\n",
          CHRE_EXTRACT_MAJOR_VERSION(CHRE_API_VERSION));
  fprintf(stdout, "DEFAULT_CHRE_API_VERSION_MINOR = 0x%02x\n",
          CHRE_EXTRACT_MINOR_VERSION(CHRE_API_VERSION));
  return 0;
}
