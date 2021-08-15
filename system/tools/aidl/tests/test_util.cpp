/*
 * Copyright (C) 2015, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <algorithm>
#include <iostream>
#include <iterator>

#include <android-base/strings.h>

#include "os.h"
#include "tests/test_util.h"

using android::base::Split;
using android::base::Join;
using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::distance;

namespace android {
namespace aidl {
namespace test {

string CanonicalNameToPath(const char* package_class, const char* extension) {
  string rel_path{package_class};
  for (char& c : rel_path) {
    if (c == '.') {
      c = OS_PATH_SEPARATOR;
    }
  }
  rel_path += extension;
  return rel_path;
}

void SplitPackageClass(const string& package_class,
                       string* rel_path,
                       string* package,
                       string* class_name) {
  *package = string{package_class, 0, package_class.rfind('.')};
  *class_name = string{package_class, package_class.rfind('.') + 1};
  *rel_path = CanonicalNameToPath(package_class.c_str(), ".aidl");
}

void PrintDiff(const string& a, const string& b) {
  const int LEFT = 1;
  const int UP = 2;
  const int UP_LEFT = 4;

  auto a_lines = Split(a, "\n");
  auto b_lines = Split(b, "\n");

  struct diff_table_entry {
    size_t longest_common_subsequence_length;
    int propagation_directions;
  };

  diff_table_entry table[a_lines.size() + 1][b_lines.size() + 1];

  for (size_t i = 0; i < a_lines.size() + 1; ++i) {
    for (size_t j = 0; j < b_lines.size() + 1; ++j) {
      if (i == 0 || j == 0) {
        int directions = 0;

        if (i) {
          directions |= UP;
        }

        if (j) {
          directions |= LEFT;
        }

        table[i][j].longest_common_subsequence_length = 0;
        table[i][j].propagation_directions = directions;
      } else if (a_lines[i-1] == b_lines[j-1]) {
        table[i][j].longest_common_subsequence_length =
            table[i-1][j-1].longest_common_subsequence_length + 1;
        table[i][j].propagation_directions = UP_LEFT;
      } else {
        size_t length_up = table[i-1][j].longest_common_subsequence_length;
        size_t length_left = table[i][j-1].longest_common_subsequence_length;
        int directions = 0;
        size_t length;

        if (length_up >= length_left) {
          directions |= UP;
          length = length_up;
        }

        if (length_left >= length_up) {
          directions |= LEFT;
          length = length_left;
        }

        table[i][j].longest_common_subsequence_length = length;
        table[i][j].propagation_directions = directions;
      }
    }
  }

  size_t i = a_lines.size();
  size_t j = b_lines.size();
  vector<string> output;

  while (table[i][j].propagation_directions) {
    if (table[i][j].propagation_directions & UP_LEFT) {
      output.push_back(" " + a_lines[i-1]);
      i--;
      j--;
    } else if (table[i][j].propagation_directions & UP) {
      output.push_back("-" + a_lines[i-1]);
      i--;
    } else {
      output.push_back("+" + b_lines[j-1]);
      j--;
    }
  }

  int print_mask = 0;
  bool printed_last = false;
  size_t line_number = 0;

  for (auto it = output.crbegin(), frontier = output.crbegin();
       it != output.crend(); ++it) {
    while (frontier != output.crend() && distance(it, frontier) <= 3) {
      print_mask <<= 1;
      print_mask &= 0x7f;

      if ((*frontier)[0] != ' ') {
        print_mask |= 1;
      }

      frontier++;
    }

    if ((*it)[0] != '-') {
      line_number++;
    }

    if (print_mask) {
      if (!printed_last) {
        cout << "Line: " << line_number << endl;
      }

      cout << *it << endl;
      printed_last = true;
    } else {
      printed_last = false;
    }
  }
}

}  // namespace test
}  // namespace android
}  // namespace aidl
