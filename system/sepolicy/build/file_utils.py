# Copyright 2018 - The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""File-related utilities."""


import os
import shutil
import tempfile


def make_parent_dirs(file_path):
    """Creates parent directories for the file_path."""
    if os.path.exists(file_path):
        return

    parent_dir = os.path.dirname(file_path)
    if parent_dir and not os.path.exists(parent_dir):
        os.makedirs(parent_dir)


def filter_out(pattern_files, input_file):
    """"Removes lines in input_file that match any line in pattern_files."""

    # Prepares patterns.
    patterns = []
    for f in pattern_files:
        patterns.extend(open(f).readlines())

    # Copy lines that are not in the pattern.
    tmp_output = tempfile.NamedTemporaryFile()
    with open(input_file, 'r') as in_file:
        tmp_output.writelines(line for line in in_file.readlines()
                              if line not in patterns)
        tmp_output.flush()

    # Replaces the input_file.
    shutil.copyfile(tmp_output.name, input_file)
