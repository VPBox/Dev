/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include "fec_private.h"

struct process_info {
    int id;
    fec_handle *f;
    uint8_t *buf;
    size_t count;
    uint64_t offset;
    read_func func;
    ssize_t rc;
    size_t errors;
};

/* thread function  */
static void * __process(void *cookie)
{
    process_info *p = static_cast<process_info *>(cookie);

    debug("thread %d: [%" PRIu64 ", %" PRIu64 ")", p->id, p->offset,
        p->offset + p->count);

    p->rc = p->func(p->f, p->buf, p->count, p->offset, &p->errors);
    return p;
}

/* launches a maximum number of threads to process a read */
ssize_t process(fec_handle *f, uint8_t *buf, size_t count, uint64_t offset,
        read_func func)
{
    check(f);
    check(buf)
    check(func);

    if (count == 0) {
        return 0;
    }

    int threads = sysconf(_SC_NPROCESSORS_ONLN);

    if (threads < WORK_MIN_THREADS) {
        threads = WORK_MIN_THREADS;
    } else if (threads > WORK_MAX_THREADS) {
        threads = WORK_MAX_THREADS;
    }

    uint64_t start = (offset / FEC_BLOCKSIZE) * FEC_BLOCKSIZE;
    size_t blocks = fec_div_round_up(count, FEC_BLOCKSIZE);

    size_t count_per_thread = fec_div_round_up(blocks, threads) * FEC_BLOCKSIZE;
    size_t max_threads = fec_div_round_up(count, count_per_thread);

    if ((size_t)threads > max_threads) {
        threads = (int)max_threads;
    }

    size_t left = count;
    uint64_t pos = offset;
    uint64_t end = start + count_per_thread;

    debug("%d threads, %zu bytes per thread (total %zu)", threads,
        count_per_thread, count);

    std::vector<pthread_t> handles;
    process_info info[threads];
    ssize_t rc = 0;

    /* start threads to process queue */
    for (int i = 0; i < threads; ++i) {
        check(left > 0);

        info[i].id = i;
        info[i].f = f;
        info[i].buf = &buf[pos - offset];
        info[i].count = (size_t)(end - pos);
        info[i].offset = pos;
        info[i].func = func;
        info[i].rc = -1;
        info[i].errors = 0;

        if (info[i].count > left) {
            info[i].count = left;
        }

        pthread_t thread;

        if (pthread_create(&thread, NULL, __process, &info[i]) != 0) {
            error("failed to create thread: %s", strerror(errno));
            rc = -1;
        } else {
            handles.push_back(thread);
        }

        pos = end;
        end  += count_per_thread;
        left -= info[i].count;
    }

    check(left == 0);

    ssize_t nread = 0;

    /* wait for all threads to complete */
    for (auto thread : handles) {
        process_info *p = NULL;

        if (pthread_join(thread, (void **)&p) != 0) {
            error("failed to join thread: %s", strerror(errno));
            rc = -1;
        } else if (!p || p->rc == -1) {
            rc = -1;
        } else {
            nread += p->rc;
            f->errors += p->errors;
        }
    }

    if (rc == -1) {
        errno = EIO;
        return -1;
    }

    return nread;
}
