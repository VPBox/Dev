/*
 * Copyright (C) 2018 The Android Open Source Project
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

#ifndef LOG_PLOT_H
#define LOG_PLOT_H

#include <algorithm>
#include <cmath>
#include <iomanip>      // setw
#include <iostream>
#include <map>
#include <sstream>
#include <string>

// TODO Make a class called LogPlot and put this functionality in it.
// Actually maybe this file can be called AsciiPlot or something...
/**
 * \brief Creates a std::string graph representation of equally-spaced time-series data points.
 *
 * \param first     RandomAccessIterator iterator to initial position of sequence.
 *                  Iterator shall point to a pair<float, bool>, where the float is the data value
 *                  and the bool is whether the data value is the start of a new data point in time
 *                  (i.e. a break in time continuity).
 * \param last      RandomAccessIterator iterator to final position of sequence.
 * \return the std::string of the graph.
 *
 */
template <class RandomAccessIterator>
std::string audio_utils_log_plot(RandomAccessIterator first, RandomAccessIterator last)
{
    using T = decltype((*first).first);

    constexpr int HEIGHT = 14;                    // Character height of the plot
    // Leave 20% display space before min and after max data points
    constexpr float RANGE_BUFFER_ROOM = 0.2f;
    // Minimum range of lowest and highest y-axis value to display
    constexpr int RANGE_MIN = 14;
    constexpr unsigned int WIDTH_MAX = 200U;      // Max character width of plot
    const size_t size = last - first;

    if (size <= 0) {
        return "";
    }

    // Find min and max element in the vector.
    const auto result = std::minmax_element(first, last);
    const T minVal = (*result.first).first;
    const T maxVal = (*result.second).first;

    const T range = maxVal - minVal;
    T graphMin, graphMax;
    if (range < RANGE_MIN) {
        T avg = (maxVal + minVal) / 2;
        graphMin = avg - RANGE_MIN / 2;
        graphMax = avg + RANGE_MIN / 2;
    } else {
        graphMin = minVal - range * RANGE_BUFFER_ROOM;
        graphMax = maxVal + range * RANGE_BUFFER_ROOM;
    }

    // Value of one character height increase on the graph
    const T increment = (graphMax - graphMin) / HEIGHT;
    // Something went wrong if we reached this statement..
    if (increment <= 0.0f) {
        return "";
    }

    std::stringstream ss;
    ss << std::fixed << std::setprecision(1);

    // Start storing the graph into string.
    // TODO store everything into a preallocated string rather than use stringstream.
    // This may make the code easier to maintain.
    ss << "\n";
    for (int height = HEIGHT - 1; height >= 0; height--) {
        int spaces = 1;     // Amount of spaces before the data point
        ss << std::setw(9) << graphMin + increment * height;
        ss << std::setw(3) << "-|";
        auto it = size <= WIDTH_MAX ? first : first + size - WIDTH_MAX;
        for (; it < last; ++it) {
            const T power = it->first;
            const bool start = it->second;
            // TODO explicitly do type conversion for parameter passed to round()?
            int px = (int)round((power - graphMin) / increment);
            // The it != last - 1 is a temporary workaround to prevent vertical bar
            // separators after the last data point entry.
            if ((start || px == height) && it != last - 1) {
                ss << std::setw(spaces) << (start ? "|" : "*");
                spaces = 1;
            } else {
                spaces++;
            }
        }
        ss << "\n";
    }
    ss << std::setw(12) << "|";
    ss << std::string(std::min(size - (size_t)1, (size_t)WIDTH_MAX), '_') << "\n\n";

    return ss.str();
}

// determines how many character spaces an integer takes up.
inline int widthOf(int x) {
    int width = 0;
    if (x < 0) {
        ++width;
        x = x == INT_MIN ? INT_MAX : -x;
    }
    // assert (x >= 0)
    do {
        ++width;
        x /= 10;
    } while (x > 0);
    return width;
}

// computes the column width required for a specific histogram value
inline int numberWidth(double number, int leftPadding) {
    // Added values account for whitespaces needed around numbers, and for the
    // dot and decimal digit not accounted for by widthOf
    return std::max(std::max(widthOf(static_cast<int>(number)) + 3, 2), leftPadding + 1);
}

// TODO Make this templated and add comments.
inline std::string audio_utils_plot_histogram(const std::map<double, int> &buckets,
        const char *title = "", const char *label = "", int maxHeight = 10)
{
    if (buckets.empty()) {
        return "";
    }

    auto it = buckets.begin();
    double maxDelta = it->first;
    int maxCount = it->second;
    // Compute maximum values
    while (++it != buckets.end()) {
        if (it->first > maxDelta) {
            maxDelta = it->first;
        }
        if (it->second > maxCount) {
            maxCount = it->second;
        }
    }
    int height = log2(maxCount) + 1; // maxCount > 0, safe to call log2
    const int leftPadding = widthOf(1 << height);
    const int bucketWidth = numberWidth(maxDelta, leftPadding);
    int scalingFactor = 1;
    // scale data if it exceeds maximum height
    if (height > maxHeight) {
        scalingFactor = (height + maxHeight) / maxHeight;
        height /= scalingFactor;
    }
    std::stringstream ss;
    ss << title << "\n " << std::setw(leftPadding) << " ";
    // write histogram label line with bucket values
    for (auto const &x : buckets) {
        const int colWidth = numberWidth(x.first, leftPadding);
        ss << std::setw(colWidth) << x.second;
    }
    // write histogram ascii art
    // underscores and spaces length corresponds to maximum width of histogram
    constexpr int kLen = 200;
    static const std::string underscores(kLen, '_');
    static const std::string spaces(kLen, ' ');
    auto getTail = [](const size_t n, const std::string &s) {
        return s.c_str() + s.size() - std::min(n, s.size());
    };

    ss << "\n ";
    for (int row = height * scalingFactor; row >= 0; row -= scalingFactor) {
        // TODO explain how value is derived from log2 and why it doesn't overflow.
        const int value = 1 << row;
        ss << getTail(leftPadding, spaces);
        for (auto const &x : buckets) {
            const int colWidth = numberWidth(x.first, leftPadding);
            ss << getTail(colWidth - 1, spaces) <<
                (x.second < value ? " " : "|");
        }
        ss << "\n ";
    }
    // print x-axis
    const int columns = static_cast<int>(buckets.size());
    ss << std::setw(leftPadding) << " "
        << getTail((columns + 1) * bucketWidth, underscores) << "\n ";

    // write footer with bucket labels
    ss << std::setw(leftPadding) << " ";
    for (auto const &x : buckets) {
        const int colWidth = numberWidth(x.first, leftPadding);
        ss << std::setw(colWidth) << std::fixed << std::setprecision(1) << x.first;
    }
    ss << getTail(bucketWidth, spaces) << label << "\n";

    return ss.str();
}

#endif // !LOG_PLOT_H
