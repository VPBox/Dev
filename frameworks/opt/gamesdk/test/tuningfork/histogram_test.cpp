/*
 * Copyright 2019 The Android Open Source Project
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

#include "tuningfork/histogram.h"

#include "gtest/gtest.h"

namespace histogram_test {

using namespace tuningfork;

const char kEmptyHistogramJson[] = "{\"pmax\":[],\"cnts\":[]}";
const char kEmpty0To10Json[] =
    "{\"pmax\":[0.00,1.00,2.00,3.00,4.00,5.00,6.00,7.00,8.00,9.00,10.00,99999],"
    "\"cnts\":[0,0,0,0,0,0,0,0,0,0,0,0]}";
const char kAdd1AutoJson[] =
    "{\"pmax\":[0.60,0.70,0.80,0.90,1.00,1.10,1.20,1.30,1.40,99999],"
    "\"cnts\":[0,0,0,0,0,1,0,0,0,0]}";
const char kAdd10To10Json[] =
    "{\"pmax\":[0.00,1.00,2.00,3.00,4.00,5.00,6.00,7.00,8.00,9.00,10.00,99999],"
    "\"cnts\":[0,0,1,0,0,0,0,0,0,0,0,0]}";

TEST(HistogramTest, DefaultEmpty) {
    Histogram h;
    EXPECT_EQ(h.Count(), 0) << "Initialized Histogram not empty";
    EXPECT_EQ(h.ToJSON(), kEmptyHistogramJson) << "Empty histogram bad";
}

TEST(HistogramTest, Empty0To10) {
    Histogram h(0, 10, 10);
    EXPECT_EQ(h.Count(), 0) << "Initialized Histogram not empty";
    EXPECT_EQ(h.ToJSON(), kEmpty0To10Json) << "Empty 0-10 histogram bad";
}

TEST(HistogramTest, AddOneToAutoSizing) {
    Histogram h(0, 0, 8);
    EXPECT_EQ(h.Count(), 0) << "Initialized auto-sizing Histogram not empty";
    h.Add(1.0);
    EXPECT_EQ(h.Count(), 0) << "Add 0 should not be counted";
    EXPECT_EQ(h.ToJSON(), kEmptyHistogramJson) << "Empty histogram bad";
    h.CalcBucketsFromSamples();
    EXPECT_EQ(h.Count(), 1) << "Add 1 was not counted";
    EXPECT_EQ(h.ToJSON(), kAdd1AutoJson) << "Add 1 auto-sizing bad";
}

TEST(HistogramTest, AddOneTo0To10) {
    Histogram h(0, 10, 10);
    EXPECT_EQ(h.Count(), 0) << "Initialized Histogram not empty";
    h.Add(1.0);
    EXPECT_EQ(h.Count(), 1) << "Add 1 was not counted";
    EXPECT_EQ(h.ToJSON(), kAdd10To10Json) << "Add 1 0-10 histogram bad";
    h.Clear();
    EXPECT_EQ(h.ToJSON(), kEmpty0To10Json) << "Clear 0-10 histogram bad";
}

} // namespace histogram_test
