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

#include "aslr_test.h"

/* run tests if on supported arch */
#if defined(__x86_64__) || defined(__i386__) || defined(__aarch64__) || defined(__arm__)

/* make sure the default entropy values matches what we expect */
TEST_F(AslrMmapTest, match_default) {
    if (user32) {
        // running 32-bit userspace on 64-bit kernel, only compat used.
        return;
    } else {
        EXPECT_EQ(def, get_mmap_rnd_bits(false));
    }
}

/* make sure the default compat entropy values matches what we expect */
TEST_F(AslrMmapTest, match_compat_default) {
    if (compat || user32)
        EXPECT_EQ(def_cmpt, get_mmap_rnd_bits(true));
}

/* make sure we can't set entropy below a minimum threshold */
TEST_F(AslrMmapTest, match_min) {
    if (user32) {
        // running 32-bit userspace on 64-bit kernel, only compat used.
        return;
    } else {
        EXPECT_FALSE(set_mmap_rnd_bits(min - 1, false));
        EXPECT_TRUE(set_mmap_rnd_bits(min, false));
        EXPECT_EQ(min, get_mmap_rnd_bits(false));
    }
}

/* make sure we can't set compat entropy below a minimum threshold */
TEST_F(AslrMmapTest, match_compat_min) {
    if (compat || user32) {
        EXPECT_FALSE(set_mmap_rnd_bits(min_cmpt - 1, true));
        EXPECT_TRUE(set_mmap_rnd_bits(min_cmpt, true));
        EXPECT_EQ(min_cmpt, get_mmap_rnd_bits(true));
    }
}

/* make sure we can't set entropy above a maximum threshold */
TEST_F(AslrMmapTest, match_max) {
    if (user32) {
        // running 32-bit userspace on 64-bit kernel, only compat used.
        return;
    } else {
        EXPECT_FALSE(set_mmap_rnd_bits(max + 1, false));
        EXPECT_TRUE(set_mmap_rnd_bits(max, false));
        EXPECT_EQ(max, get_mmap_rnd_bits(false));
    }
}

/* make sure we can't set compat entropy above a maximum threshold */
TEST_F(AslrMmapTest, match_compat_max) {
    if (compat || user32) {
        EXPECT_FALSE(set_mmap_rnd_bits(max_cmpt + 1, true));
        EXPECT_TRUE(set_mmap_rnd_bits(max_cmpt, true));
        EXPECT_EQ(max_cmpt, get_mmap_rnd_bits(true));
    }
}

/* make sure observed entropy is what we expect when we set min value */
TEST_F(AslrMmapTest, entropy_min) {
    if (user32) {
        // running 32-bit userspace on 64-bit kernel, only compat used.
        return;
    } else {
        EXPECT_TRUE(set_mmap_rnd_bits(min, false));
        EXPECT_EQ(min, calc_mmap_entropy(path, lib, 16));
    }
}

/* make sure observed compat entropy is what we expect when we set min value */
TEST_F(AslrMmapTest, entropy_cmpt_min) {
    if (compat || user32) {
        EXPECT_TRUE(set_mmap_rnd_bits(min_cmpt, true));
        EXPECT_EQ(min_cmpt, calc_mmap_entropy(SCRAPE_PATH_32, SCRAPE_LIB_32, 16));
    }
}

/* make sure observed entropy is what we expect when we set max value */
TEST_F(AslrMmapTest, entropy_max) {
    if (user32) {
        // running 32-bit userspace on 64-bit kernel, only compat used.
        return;
    } else {
        EXPECT_TRUE(set_mmap_rnd_bits(max, false));
        EXPECT_EQ(max, calc_mmap_entropy(path, lib, 16));
    }
}

/* make sure observed compat entropy is what we expect when we set max value */
TEST_F(AslrMmapTest, entropy_cmpt_max) {
    if (compat || user32) {
        EXPECT_TRUE(set_mmap_rnd_bits(max_cmpt, true));
        EXPECT_EQ(max_cmpt, calc_mmap_entropy(SCRAPE_PATH_32, SCRAPE_LIB_32, 16));
    }
}

/* make sure observed entropy is what we expect for default value */
TEST_F(AslrMmapTest, entropy_def) {
    if (user32) {
        // running 32-bit userspace on 64-bit kernel, only compat used.
        return;
    } else {
        EXPECT_EQ(def, calc_mmap_entropy(path, lib, 16));
    }
}

/* make sure observed entropy is what we expect for default compat value */
TEST_F(AslrMmapTest, entropy_cmpt_def) {
    if (compat || user32) {
        EXPECT_EQ(def_cmpt, calc_mmap_entropy(SCRAPE_PATH_32, SCRAPE_LIB_32, 16));
    }
}

#endif /* supported arch */
