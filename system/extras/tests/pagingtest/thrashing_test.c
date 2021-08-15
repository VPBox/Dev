#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/time.h>

#include "pagingtest.h"

#define LINESIZE 32

int thrashing_test(int test_runs, bool cache) {
    int fds[4] = {-1, -1, -1, -1};
    char tmpnames[4][17] = { "thrashing1XXXXXX", "thrashing2XXXXXX", "thrashing3XXXXXX", "thrashing4XXXXXX" };
    volatile char *bufs[4] = {0};
    unsigned long long k;
    int ret = -1;
    struct timeval begin_time, end_time, elapsed_time, total_time;
    unsigned long long filesize;
    long num_pages;
    long pagesize;

    timerclear(&total_time);

    num_pages = sysconf(_SC_PHYS_PAGES);
    pagesize = sysconf(_SC_PAGE_SIZE);
    if (num_pages < 0) {
        fprintf(stderr, "failed to get the number of pages\n");
        return -1;
    }

    filesize = num_pages * pagesize / (ARRAY_SIZE(fds) - 1);

    for (size_t i = 0; i < ARRAY_SIZE(fds); i++) {
        fds[i] = create_tmp_file(tmpnames[i], filesize);
        if (fds[i] < 0) {
            goto err_fd;
        }
    }

    for (size_t i = 0; i < ARRAY_SIZE(fds); i++) {
        bufs[i] = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fds[i], 0);
        if (bufs[i] == ((void *)-1)) {
            fprintf(stderr, "Failed to mmap file: %s\n", strerror(errno));
            goto err;
        }
        if (!cache) {
            //madvise and fadvise as random to prevent prefetching
            ret = madvise((void *)bufs[i], filesize, MADV_RANDOM) ||
                    posix_fadvise(fds[i], 0, filesize, POSIX_FADV_RANDOM);
            if (ret) {
                goto err;
            }
        }
    }

    for (int i = 0; i < test_runs; i++) {
        for (size_t j = 0; j < ARRAY_SIZE(fds); j++) {
            gettimeofday(&begin_time, NULL);
            for (k = 0; k < filesize; k += pagesize) {
                bufs[j][k];
            }
            gettimeofday(&end_time, NULL);

            timersub(&end_time, &begin_time, &elapsed_time);
            timeradd(&total_time, &elapsed_time, &total_time);
        }
    }

    printf("%scached thrashing: %llu MB/s\n", cache ? "" : "un",
             (filesize * ARRAY_SIZE(fds) * test_runs * USEC_PER_SEC) /
             (1024 * 1024 * (total_time.tv_sec * USEC_PER_SEC + total_time.tv_usec)));

    ret = 0;

err:
    for (size_t i = 0; i < ARRAY_SIZE(bufs) && bufs[i] != NULL; i++) {
        munmap((void *)bufs[i], filesize);
    }
err_fd:
    for (size_t i = 0; i < ARRAY_SIZE(fds) && fds[i] >= 0; i++) {
        close(fds[i]);
    }
    return ret;
}
