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

#ifndef ASLR_TEST_H
#define ASLR_TEST_H

#include <cmath>
#include <errno.h>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <unordered_set>

#include <gtest/gtest.h>

#define MAX_ADDR_LEN 256

#define PROCFS_PATH "/proc/sys/vm/mmap_rnd_bits"
#define PROCFS_COMPAT_PATH "/proc/sys/vm/mmap_rnd_compat_bits"

#define SCRAPE_PATH_64 "/data/nativetest64/scrape_mmap_addr/scrape_mmap_addr"
#define SCRAPE_PATH_32 "/data/nativetest/scrape_mmap_addr/scrape_mmap_addr"
#define SCRAPE_LIB_64 "/system/bin/linker64"
#define SCRAPE_LIB_32 "/system/bin/linker"

class AslrMmapTest : public ::testing::Test {
  protected:
    static void SetUpTestCase();
    static const char *path;
    static const char *lib;
    static unsigned int def, min, max;
    static bool compat, user32;
    static unsigned int def_cmpt, min_cmpt, max_cmpt;

    void TearDown();
};

/*
 * gets the current mmap_rnd_bits value. requires root.
 */
unsigned int get_mmap_rnd_bits(bool compat);

/*
 * sets the corresponding mmap_rnd_bits variable, returns false if couldn't
 * change. requires root.
 */
bool set_mmap_rnd_bits(unsigned int new_val, bool compat);

/*
 * scrape_addr - get the raw starting address from /proc/child_pid/mmaps
 */
std::string scrape_addr(const char *exec_name, const char *lib_match);

/*
 * forks off sample_size processes and records the starting address of the
 * indicated library as reported by exec_name.  Reports entropy observed among
 * recorded samples.
 */
unsigned int calc_mmap_entropy(const char *exec_name, const char *lib_match, size_t samp_sz);

#endif //ASLR_TEST_H
