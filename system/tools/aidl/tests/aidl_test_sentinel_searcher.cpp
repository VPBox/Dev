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

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <errno.h>
#include <time.h>
#include <unistd.h>

using std::cerr;
using std::cout;
using std::endl;
using std::getline;
using std::ifstream;
using std::string;
using std::vector;

namespace {

bool ReadLines(const string& input_file_path, vector<string>* lines) {
  ifstream watched_file(input_file_path);
  if (!watched_file.is_open()) {
    cerr << "Unable to open input file: " << input_file_path << endl;
    return false;
  }

  string line;
  while (getline(watched_file, line)) {
    lines->push_back(line);
  }
  watched_file.close();
  return true;
}

bool HasSentinel(const vector<string>& lines, const string& sentinel) {
  for (const auto& line : lines) {
    if (line.find(sentinel) != string::npos) {
      return true;
    }
  }
  return false;
}

}  // namespace

int main(int argc, const char* argv[]) {
  if (argc != 5) {
    cerr << "Invalid usage." << endl;
    cerr << argv[0]
         << " <timeout in seconds>"
         << " <input file path>"
         << " <success sentinel>"
         << " <failure sentinel>" << endl;
    return -EINVAL;
  }
  const string timeout_as_str = argv[1];
  const string input_file_path = argv[2];
  const string success_sentinel = argv[3];
  const string failure_sentinel = argv[4];

  const int timeout_seconds = atoi(timeout_as_str.c_str());
  if (timeout_seconds <= 0) {
    cerr << "Invalid timeout value (in seconds): " << timeout_as_str << endl;
    return -EINVAL;
  }

  int exit_code = 1;
  const time_t start_time = time(nullptr);
  vector<string> lines;
  while (true) {
    sleep(1);
    if (time(nullptr) - start_time > timeout_seconds) {
      cerr << "Timed out waiting for success/failure sentinel." << endl;
      break;
    }
    // Ignore errors when reading lines.  The file may not immediately exist
    // because it takes the Java process some time to create it.
    lines.clear();
    ReadLines(input_file_path, &lines);

    if (HasSentinel(lines, success_sentinel)) {
      exit_code = 0;
      break;
    }
    if (HasSentinel(lines, failure_sentinel)) {
      break;
    }
  }

  cout << "Found output:" << endl;
  for (const auto& line : lines) {
    cout << "  " << line << endl;
  }
  return exit_code;
}
