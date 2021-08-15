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
 *
 * Simple memory eater. Runs as a daemon. Prints the child PID to
 * std so you can easily kill it later.
 * Usage: memeater <size in MB>
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    pid_t pid;
    int fd;
    int numMb = argc > 1 ? atoi(argv[1]) : 1;

    if (argc < 2) {
        printf("Usage: memeater <num MB to allocate>\n");
        exit(1);
    }

    switch (fork()) {
        case -1:
            perror(argv[0]);
            exit(1);
            break;
        case 0: /* child */
            chdir("/");
            umask(0);
            setpgrp();
            setsid();
            /* fork again to fully detach from controlling terminal. */
            switch (pid = fork()) {
                case -1:
                    perror("failed to fork");
                    break;
                case 0: /* second child */
                    /* redirect to /dev/null */
                    close(0);
                    open("/dev/null", 0);
                    for (fd = 3; fd < 256; fd++) {
                        close(fd);
                    }

                    printf("Allocating %d MB\n", numMb);
                    fflush(stdout);
                    /* allocate memory and fill it */
                    while (numMb > 0) {
                        // Allocate 500MB at a time at most
                        int mbToAllocate = numMb > 500 ? 500 : numMb;
                        int bytesToAllocate = mbToAllocate * 1024 * 1024;
                        char *p = malloc(bytesToAllocate);
                        if (p == NULL) {
                            printf("Failed to allocate memory\n");
                            exit(1);
                        }
                        for (int j = 0; j < bytesToAllocate; j++) {
                            p[j] = j & 0xFF;
                        }
                        printf("Allocated %d MB %p\n", mbToAllocate, p);
                        fflush(stdout);
                        numMb -= mbToAllocate;
                    }

                    close(1);
                    if (open("/dev/null", O_WRONLY) < 0) {
                        perror("/dev/null");
                        exit(1);
                    }
                    close(2);
                    dup(1);

                    /* Sit around doing nothing */
                    while (1) {
                        usleep(1000000);
                    }
                  default:
                      /* so caller can easily kill it later. */
                      printf("%d\n", pid);
                      exit(0);
                      break;
                }
                break;
          default:
              exit(0);
              break;
    }
  return 0;
}

