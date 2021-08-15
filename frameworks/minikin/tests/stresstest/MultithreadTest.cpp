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

#include "minikin/Layout.h"

#include <condition_variable>
#include <mutex>
#include <random>
#include <thread>

#include <cutils/log.h>
#include <gtest/gtest.h>

#include "minikin/FontCollection.h"
#include "minikin/Macros.h"
#include "minikin/MinikinPaint.h"

#include "FontTestUtils.h"
#include "MinikinInternal.h"
#include "PathUtils.h"

namespace minikin {

constexpr int LAYOUT_COUNT_PER_COLLECTION = 500;
constexpr int COLLECTION_COUNT_PER_THREAD = 15;
constexpr int NUM_THREADS = 10;

std::mutex gMutex;
std::condition_variable gCv;
bool gReady GUARDED_BY(gMutex) = false;

static std::vector<uint16_t> generateTestText(std::mt19937* mt, int lettersInWord,
                                              int wordsInText) {
    std::uniform_int_distribution<uint16_t> dist('A', 'Z');

    std::vector<uint16_t> text;
    text.reserve((lettersInWord + 1) * wordsInText - 1);
    for (int i = 0; i < wordsInText; ++i) {
        if (i != 0) {
            text.emplace_back(' ');
        }
        for (int j = 0; j < lettersInWord; ++j) {
            text.emplace_back(dist(*mt));
        }
    }
    return text;
}

static void thread_main(int tid) {
    {
        // Wait until all threads are created.
        std::unique_lock<std::mutex> lock(gMutex);
        gCv.wait(lock, [] { return gReady; });
    }

    std::mt19937 mt(tid);

    for (int i = 0; i < COLLECTION_COUNT_PER_THREAD; ++i) {
        MinikinPaint paint(buildFontCollection("Ascii.ttf"));
        paint.size = 10.0f;  // Make 1em = 10px

        for (int j = 0; j < LAYOUT_COUNT_PER_COLLECTION; ++j) {
            // Generates 10 of 3-letter words so that the word sometimes hit the cache.
            std::vector<uint16_t> text = generateTestText(&mt, 3, 10);
            Layout layout(text, Range(0, text.size()), Bidi::LTR, paint, StartHyphenEdit::NO_EDIT,
                          EndHyphenEdit::NO_EDIT);
            for (size_t k = 0; k < text.size(); ++k) {
                // All characters in Ascii.ttf has 1.0em horizontal advance.
                LOG_ALWAYS_FATAL_IF(layout.getCharAdvance(k) != 10.0f,
                                    "Memory corruption detected.");
            }
        }
    }
}

TEST(MultithreadTest, ThreadSafeStressTest) {
    std::vector<std::thread> threads;

    {
        std::unique_lock<std::mutex> lock(gMutex);
        threads.reserve(NUM_THREADS);
        for (int i = 0; i < NUM_THREADS; ++i) {
            threads.emplace_back(&thread_main, i);
        }
        gReady = true;
    }
    gCv.notify_all();

    for (auto& thread : threads) {
        thread.join();
    }
}

}  // namespace minikin
