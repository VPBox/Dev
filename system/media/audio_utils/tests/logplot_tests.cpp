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

#include <iostream>
#include <vector>

#include <audio_utils/LogPlot.h>

// TODO Make more rigorous unit tests.
int main()
{
    static const float data[] = {-61.4, -61.7, -56.2, -54.5, -47.7, -51.1, -49.7, -47.2,
        -47.8, -42.3, -38.9, -40.5, -39.4, -33.9, -26.3, -20.9};
    size_t data_size = sizeof(data) / sizeof(*data);
    std::vector<std::pair<float, bool>> vdata;
    for (size_t i = 0; i < data_size; i++) {
        vdata.emplace_back(data[i], (i + 1) % 10 == 0);
    }

    std::string graphstr = audio_utils_log_plot(vdata.begin(), vdata.end());
    std::cout << graphstr << std::endl;

    return EXIT_SUCCESS;
}
