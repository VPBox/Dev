# Copyright 2017 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from __future__ import print_function

import os
import sys

"""Shared functions for use in tzdata scripts."""

def GetIanaTarFile(dir_name, file_type):
  matching_files = []
  for filename in os.listdir(dir_name):
    if filename.startswith('tz%s20' % file_type) and filename.endswith('.tar.gz'):
      matching_files.append(filename);

  if len(matching_files) == 0:
    return None
  elif len(matching_files) == 1:
    return '%s/%s' % (dir_name, matching_files[0])
  else:
    print('Multiple tz%s files found unexpectedly %s' % (file_type, matching_files))
    sys.exit(1)

