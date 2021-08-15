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

#include <errno.h>
#include <stdio.h>
#include <string>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <new>

#include <audio_utils/fifo.h>
#include <cutils/ashmem.h>

#define FRAME_COUNT 2048
#define FRAME_SIZE sizeof(int16_t)
#define BUFFER_SIZE (FRAME_COUNT * FRAME_SIZE)

int main(int argc __unused, char **argv __unused)
{
    // TODO Add error checking for ashmem_create_region and mmap

    const int frontFd = ashmem_create_region("front", sizeof(audio_utils_fifo_index));
    printf("frontFd=%d\n", frontFd);

    const int rearFd = ashmem_create_region("rear", sizeof(audio_utils_fifo_index));
    printf("rearFd=%d\n", rearFd);

    const int dataFd = ashmem_create_region("buffer", BUFFER_SIZE);
    printf("dataFd=%d\n", dataFd);

    // next two index constructors must execute exactly once, so we do it in the parent

    audio_utils_fifo_index *frontIndex = (audio_utils_fifo_index *) mmap(NULL,
            sizeof(audio_utils_fifo_index), PROT_READ | PROT_WRITE, MAP_SHARED, frontFd, (off_t) 0);
    printf("parent frontIndex=%p\n", frontIndex);
    (void) new(frontIndex) audio_utils_fifo_index();

    audio_utils_fifo_index *rearIndex = (audio_utils_fifo_index *) mmap(NULL,
            sizeof(audio_utils_fifo_index), PROT_READ | PROT_WRITE, MAP_SHARED, rearFd, (off_t) 0);
    printf("parent rearIndex=%p\n", rearIndex);
    (void) new(rearIndex) audio_utils_fifo_index();

    int16_t *data = (int16_t *) mmap(NULL, sizeof(audio_utils_fifo_index), PROT_READ | PROT_WRITE,
            MAP_SHARED, dataFd, (off_t) 0);
    printf("parent data=%p\n", data);
    memset(data, 0, BUFFER_SIZE);

    const int pageSize = getpagesize();
    printf("page size=%d\n", pageSize);

    // create writer

    printf("fork writer:\n");
    const pid_t pidWriter = fork();
    // TODO check if pidWriter < 0
    if (!pidWriter) {

        // Child inherits the parent's read/write mapping of front index.
        // To confirm that there are no attempts to write to the front index,
        // unmap it and then re-map it as read-only.
        int ok = munmap(frontIndex, sizeof(audio_utils_fifo_index));
        printf("writer unmap front ok=%d\n", ok);
        ok = ashmem_set_prot_region(frontFd, PROT_READ);
        printf("writer prot read front ok=%d\n", ok);
        // The pagesize * 4 offset confirms that we don't assume identical mapping in both processes
        frontIndex = (audio_utils_fifo_index *) mmap((char *) frontIndex + pageSize * 4,
                sizeof(audio_utils_fifo_index), PROT_READ, MAP_SHARED | MAP_FIXED, frontFd,
                (off_t) 0);
        printf("writer frontIndex=%p\n", frontIndex);

        // Retain our read/write mapping of rear index and data
        audio_utils_fifo fifo(FRAME_COUNT, FRAME_SIZE, data, *rearIndex, frontIndex);
        audio_utils_fifo_writer writer(fifo);

        sleep(2);

        for (int16_t value = 1; value <= 20; value++) {
            printf("writing %d\n", value);
            const ssize_t actual = writer.write(&value, 1);
            if (actual != 1) {
                printf("wrote unexpected actual = %zd\n", actual);
                break;
            }
            // TODO needs a lot of work
            switch (value) {
            case 10:
                sleep(2);
                break;
            case 14:
                sleep(4);
                break;
            default:
                usleep(500000);
                break;
            }
        }

        (void) close(frontFd);
        (void) close(rearFd);
        (void) close(dataFd);

        return EXIT_SUCCESS;
    }

    // The sleep(2) above and sleep(1) here ensure that the order is:
    //  a. writer initializes
    //  b. reader initializes
    //  c. reader starts the read loop
    //  d. writer starts the write loop
    // Actually, as long as (a) precedes (d) and (b) precedes (c), the order does not matter.
    // TODO test all valid sequences.
    sleep(1);

    // create reader

    printf("fork reader:\n");
    const pid_t pidReader = fork();
    // TODO check if pidReader < 0
    if (!pidReader) {

        // Child inherits the parent's read/write mapping of rear index.
        // To confirm that there are no attempts to write to the rear index,
        // unmap it and then re-map it as read-only.
        int ok = munmap(rearIndex, sizeof(audio_utils_fifo_index));
        printf("reader unmap rear ok=%d\n", ok);
        ok = ashmem_set_prot_region(rearFd, PROT_READ);
        printf("reader prot read rear ok=%d\n", ok);
        // The pagesize * 4 offset confirms that we don't assume identical mapping in both processes
        rearIndex = (audio_utils_fifo_index *) mmap((char *) rearIndex + pageSize * 4,
                sizeof(audio_utils_fifo_index), PROT_READ, MAP_SHARED | MAP_FIXED, rearFd,
                (off_t) 0);
        printf("reader rearIndex=%p\n", rearIndex);

        // Similarly for the data
        ok = munmap(data, BUFFER_SIZE);
        printf("reader unmap data ok=%d\n", ok);
        ok = ashmem_set_prot_region(dataFd, PROT_READ);
        printf("reader prot read data ok=%d\n", ok);
        // The pagesize * 8 offset confirms that we don't assume identical mapping in both processes
        data = (int16_t *) mmap((char *) data + pageSize * 8, BUFFER_SIZE, PROT_READ,
                MAP_SHARED | MAP_FIXED, dataFd, (off_t) 0);
        printf("reader data=%p\n", data);

        // Retain our read/write mapping of front index
        audio_utils_fifo fifo(FRAME_COUNT, FRAME_SIZE, data, *rearIndex, frontIndex);
        audio_utils_fifo_reader reader(fifo);

        for (;;) {
            int16_t value;
            struct timespec timeout = {
                .tv_sec = 1,
                .tv_nsec = 0
            };
            const ssize_t actual = reader.read(&value, 1, &timeout);
            switch (actual) {
            case 0:
                break;
            case 1:
                printf("read %d\n", value);
                if (value == 20) {
                    goto out;
                }
                break;
            case -ETIMEDOUT:
                printf("read timed out\n");
                break;
            default:
                printf("read unexpected actual = %zd\n", actual);
                goto out;
            }
        }
out:

        (void) close(frontFd);
        (void) close(rearFd);
        (void) close(dataFd);

        return EXIT_SUCCESS;
    }

    int status;
    pid_t pid = waitpid(pidWriter, &status, 0);
    if (pid == pidWriter) {
        printf("writer exited with status %d\n", status);
    } else {
        printf("waitpid on writer = %d\n", pid);
    }
    pid = waitpid(pidReader, &status, 0);
    if (pid == pidReader) {
        printf("reader exited with status %d\n", status);
    } else {
        printf("waitpid on reader = %d\n", pid);
    }

    // next two index destructors must execute exactly once, so we do it in the parent
    frontIndex->~audio_utils_fifo_index();
    rearIndex->~audio_utils_fifo_index();

    int ok = munmap(frontIndex, sizeof(audio_utils_fifo_index));
    printf("parent unmap front ok=%d\n", ok);
    ok = munmap(rearIndex, sizeof(audio_utils_fifo_index));
    printf("parent unmap rear ok=%d\n", ok);
    ok = munmap(data, BUFFER_SIZE);
    printf("parent unmap data ok=%d\n", ok);

    (void) close(frontFd);
    (void) close(rearFd);
    (void) close(dataFd);

    return EXIT_SUCCESS;
}
