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

#include <ctype.h>

#include "aslr_test.h"

unsigned int get_mmap_rnd_bits(bool compat) {
    std::string path;

    if (compat)
        path = PROCFS_COMPAT_PATH;
    else
        path = PROCFS_PATH;

    std::ifstream bi_file(path);
    if (!bi_file)
        return false;
    std::string str_rec;
    bi_file >> str_rec;

    return stoi(str_rec);
}

bool set_mmap_rnd_bits(unsigned int new_val, bool compat) {
    std::string path;

    if (compat)
        path = "/proc/sys/vm/mmap_rnd_compat_bits";
    else
        path = "/proc/sys/vm/mmap_rnd_bits";

    std::ofstream bo_file(path, std::ios::out);
    if (!bo_file)
        return false;

    std::string str_val = std::to_string(new_val);
    bo_file << str_val << std::flush;
    bo_file.close();

    // check to make sure it was recorded
    std::ifstream bi_file(path);
    if (!bi_file)
        return false;
    std::string str_rec;
    bi_file >> str_rec;
    bi_file.close();
    if (str_val.compare(str_rec) != 0)
        return false;
    return true;
}

std::string scrape_addr(const char *exec_name, const char *lib_match) {
    pid_t pid;
    int fd[2];
    char buff[MAX_ADDR_LEN];
    int len, status;
    if(pipe(fd)) {
        std::cerr << "Error creating pipe:" << strerror(errno) << "\n";
        return std::string();
    }

    if ((pid = fork()) < 0) {
        std::cerr << "Error creating new process: " << strerror(errno) << "\n";
        close(fd[0]);
        close(fd[1]);
        return std::string();
    } else if (pid > 0) {
        // parent
        close(fd[1]);
        wait(&status);
        if (status == -1) {
            std::cerr << "Unable to find starting address of mmapp'd libc. Aborting.\n";
            close(fd[0]);
            return std::string();
        }
        len = read(fd[0], buff, MAX_ADDR_LEN - 1);
        if (len < 0) {
            std::cerr << "Error reading pipe from child: " << strerror(errno) << "\n";
            close(fd[0]);
            return std::string();
        }
        buff[len] = '\0';
        close(fd[0]);
    } else {
        // child, dup 'n' exec
        close(fd[0]);
        if(dup2(fd[1], STDOUT_FILENO) != STDOUT_FILENO) {
            std::cerr << "Error dup'n pipe to STDOUT of child: " << strerror(errno) << "\n";
            close(fd[1]);
            return std::string();
        }
        if(execlp(exec_name, exec_name, lib_match, (char *) NULL)) {
            std::cerr << "Error exec'ing mmap_scraper: " << strerror(errno) << "\n";
            close(fd[1]);
            return std::string();
        }
    }
    return std::string(buff, strlen(buff));
}

unsigned int calc_mmap_entropy(const char *exec_name, const char *lib_match, size_t samp_sz) {
    uint64_t addr, min_addr, max_addr;

    std::unordered_set<uint64_t> addrs = { };

    // get our first value
    std::string addr_str = scrape_addr(exec_name, lib_match);
    if (addr_str.empty()) {
        std::cerr << "empty first address";
        return 0;
    }
    if (!isxdigit(addr_str[0])) {
        std::cerr << "invalid address: " << addr_str;
        return 0;
    }
    addr = min_addr = max_addr = std::stoll(addr_str, 0, 16);
    addrs.insert(addr);
    for (unsigned int i = 0; i < samp_sz - 1; ++i) {
        addr_str = scrape_addr(exec_name, lib_match);
        if (addr_str.empty()) {
            std::cerr << "empty address";
            return 0;
        }
        if (!isxdigit(addr_str[0])) {
            std::cerr << "invalid address: " << addr_str;
            return 0;
        }
        addr = std::stoll(addr_str, 0, 16);
        if (addr < min_addr)
            min_addr = addr;
        if (addr >= max_addr)
            max_addr = addr;
        addrs.insert(addr);
    }
    if (addrs.size() < (samp_sz >> 1)) {
        std::cerr << "> 50% collisions in mmap addresses, entropy appears to be rigged!";
        return 0;
    }
    unsigned int e_bits = (int) (std::ceil(std::log2(max_addr - min_addr)) - std::log2(getpagesize()));
    return e_bits;
}

const char *AslrMmapTest::path;
const char *AslrMmapTest::lib;
unsigned int AslrMmapTest::def, AslrMmapTest::min, AslrMmapTest::max;
bool AslrMmapTest::compat = false, AslrMmapTest::user32 = false;
unsigned int AslrMmapTest::def_cmpt, AslrMmapTest::min_cmpt, AslrMmapTest::max_cmpt;

void AslrMmapTest::SetUpTestCase() {
    /* set up per-arch values */
#if defined(__x86_64__)
    def = 32;
    min = 28;
    max = 32;
    path = SCRAPE_PATH_64;
    lib = SCRAPE_LIB_64;

    compat = true;
    def_cmpt = 16;
    min_cmpt = 8;
    max_cmpt = 16;

#elif defined(__i386__)
    def = 16;
    min = 8;
    max = 16;
    path = SCRAPE_PATH_32;
    lib = SCRAPE_LIB_32;

    if (!access(PROCFS_COMPAT_PATH, F_OK)) {
        // running 32 bit userspace over 64-bit kernel
        user32 = true;
        def_cmpt = 16;
        min_cmpt = 8;
        max_cmpt = 16;
    }

#elif defined(__aarch64__)
    unsigned int pgbits = std::log2(getpagesize());
    def = 24;
    min = 18 - (pgbits - 12);
    max = 24;
    path = SCRAPE_PATH_64;
    lib = SCRAPE_LIB_64;

    compat = true;
    def_cmpt = 16;
    min_cmpt = 11 - (pgbits - 12);
    max_cmpt = 16;

#elif defined(__arm__)
    unsigned int pgbits = std::log2(getpagesize());
    def = 16;
    min = 8;
    max = 16;
    path = SCRAPE_PATH_32;
    lib = SCRAPE_LIB_32;

    if (!access(PROCFS_COMPAT_PATH, F_OK)) {
        // running 32 bit userspace over 64-bit kernel
        user32 = true;
        def_cmpt = 16;
        min_cmpt = 11 - (pgbits - 12);;
        max_cmpt = 16;
    }
#endif
}

void AslrMmapTest::TearDown() {
    if (!user32)
        set_mmap_rnd_bits(def, false);
    if (user32 || compat)
        set_mmap_rnd_bits(def_cmpt, true);
}

/* run tests only if on supported arch */
#if defined(__x86_64__) || defined(__i386__) || defined(__aarch64__) || defined(__arm__)

TEST_F(AslrMmapTest, entropy_min_def) {
    if (user32) {
        // running 32-bit userspace on 64-bit kernel, only compat used.
        return;
    } else {
        EXPECT_GE(def, calc_mmap_entropy(path, lib, 16));
    }
}

TEST_F(AslrMmapTest, entropy_min_cmpt_def) {
    if (compat || user32) {
        EXPECT_GE(def_cmpt, calc_mmap_entropy(SCRAPE_PATH_32, SCRAPE_LIB_32, 16));
    }
}

#endif /* supported arch */
