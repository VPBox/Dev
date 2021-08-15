/*
 * Copyright (C) 2008 The Android Open Source Project
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

#ifndef _LOOP_H
#define _LOOP_H

#include <linux/loop.h>
#include <unistd.h>
#include <string>

class Loop {
  public:
    static const int LOOP_MAX = 4096;

  public:
    static int create(const std::string& file, std::string& out_device);
    static int destroyByDevice(const char* loopDevice);
    static int destroyAll();
    static int createImageFile(const char* file, unsigned long numSectors);
    static int resizeImageFile(const char* file, unsigned long numSectors);
};

#endif
