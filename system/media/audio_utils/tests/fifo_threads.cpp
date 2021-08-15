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
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <audio_utils/fifo.h>
extern "C" {
#include "getch.h"
}

struct Context {
    audio_utils_fifo_writer *mInputWriter;
    audio_utils_fifo_reader *mInputReader;
    audio_utils_fifo_writer *mTransferWriter;
    audio_utils_fifo_reader *mTransferReader;
    audio_utils_fifo_writer *mOutputWriter;
    audio_utils_fifo_reader *mOutputReader;
};

void *input_routine(void *arg)
{
    Context *context = (Context *) arg;
    for (;;) {
        struct timespec timeout;
        timeout.tv_sec = 30;
        timeout.tv_nsec = 0;
        char buffer[4];
        ssize_t actual = context->mInputReader->read(buffer, sizeof(buffer), &timeout);
        // TODO this test is unreadable
        if (actual > 0) {
            if ((size_t) actual > sizeof(buffer)) {
                printf("input.read actual = %d\n", (int) actual);
                abort();
            }
            ssize_t actual2 = context->mTransferWriter->write(buffer, actual, &timeout);
            if (actual2 != actual) {
                printf("transfer.write(%d) = %d\n", (int) actual, (int) actual2);
            }
            //sleep(10);
        } else if (actual == -ETIMEDOUT) {
            (void) write(1, "t", 1);
        } else {
            printf("input.read actual = %d\n", (int) actual);
        }
    }
    return NULL;
}

volatile bool outputPaused = false;

void *output_routine(void *arg)
{
    Context *context = (Context *) arg;
    for (;;) {
        if (outputPaused) {
            sleep(1);
            continue;
        }
        struct timespec timeout;
        timeout.tv_sec = 60;
        timeout.tv_nsec = 0;
        char buffer[4];
        ssize_t actual = context->mTransferReader->read(buffer, sizeof(buffer), &timeout);
        if (actual > 0) {
            if ((size_t) actual > sizeof(buffer)) {
                printf("transfer.read actual = %d\n", (int) actual);
                abort();
            }
            ssize_t actual2 = context->mOutputWriter->write(buffer, actual, NULL /*timeout*/);
            if (actual2 != actual) {
                printf("output.write(%d) = %d\n", (int) actual, (int) actual2);
            }
        } else if (actual == -ETIMEDOUT) {
            (void) write(1, "T", 1);
        } else {
            printf("transfer.read actual = %d\n", (int) actual);
        }
    }
    return NULL;
}

int main(int argc, char **argv)
{
    set_conio_terminal_mode();
    argc = argc + 0;
    argv = &argv[0];

    char inputBuffer[16];
    audio_utils_fifo inputFifo(sizeof(inputBuffer) /*frameCount*/, 1 /*frameSize*/, inputBuffer,
            true /*throttlesWriter*/);
    audio_utils_fifo_writer inputWriter(inputFifo);
    audio_utils_fifo_reader inputReader(inputFifo, true /*throttlesWriter*/);
    //inputWriter.setHysteresis(sizeof(inputBuffer) * 1/4, sizeof(inputBuffer) * 3/4);

    char transferBuffer[64];
    audio_utils_fifo transferFifo(sizeof(transferBuffer) /*frameCount*/, 1 /*frameSize*/,
            transferBuffer, true /*throttlesWriter*/);
    audio_utils_fifo_writer transferWriter(transferFifo);
    audio_utils_fifo_reader transferReader(transferFifo, true /*throttlesWriter*/);
    transferReader.setHysteresis(sizeof(transferBuffer) * 3/4, sizeof(transferBuffer) * 1/4);
    //transferWriter.setEffective(8);

    char outputBuffer[64];
    audio_utils_fifo outputFifo(sizeof(outputBuffer) /*frameCount*/, 1 /*frameSize*/, outputBuffer,
            true /*throttlesWriter*/);
    audio_utils_fifo_writer outputWriter(outputFifo);
    audio_utils_fifo_reader outputReader(outputFifo, true /*readerThrottlesWriter*/);

    Context context;
    context.mInputWriter = &inputWriter;
    context.mInputReader = &inputReader;
    context.mTransferWriter = &transferWriter;
    context.mTransferReader = &transferReader;
    context.mOutputWriter = &outputWriter;
    context.mOutputReader = &outputReader;

    pthread_t input_thread;
    int ok = pthread_create(&input_thread, (const pthread_attr_t *) NULL, input_routine,
            (void *) &context);
    pthread_t output_thread;
    ok = pthread_create(&output_thread, (const pthread_attr_t *) NULL, output_routine,
            (void *) &context);
    ok = ok + 0;

    for (;;) {
        char buffer[4];
        ssize_t actual = outputReader.read(buffer, sizeof(buffer), NULL /*timeout*/);
        if (actual > 0) {
            printf("%.*s", (int) actual, buffer);
            fflush(stdout);
        } else if (actual != 0) {
            printf("outputReader.read actual = %d\n", (int) actual);
        }
        if (kbhit()) {
            int ch = getch();
            if (ch <= 0 || ch == '\003' /*control-C*/) {
                break;
            }
            if (ch == 'p')
                outputPaused = true;
            else if (ch == 'p')
                outputPaused = false;
            buffer[0] = ch;
            actual = inputWriter.write(buffer, 1, NULL /*timeout*/);
            if (actual != 1) {
                printf("inputWriter.write actual = %d\n", (int) actual);
            }
        }
    }
    reset_terminal_mode();
}
