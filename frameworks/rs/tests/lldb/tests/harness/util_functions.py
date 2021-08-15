# Copyright (C) 2016 The Android Open Source Project
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

'''This file contains utility functions used by both the test suite and the
single test executor.'''

from __future__ import absolute_import

import os
import importlib
import sys


def load_py_module(path):
    '''Load a python file from disk.

    Args:
        path: String path to python file.

    Returns:
        python module if success, None otherwise.
    '''
    assert isinstance(path, str)
    try:
        if not os.path.exists(path):
            print('Path does not exist: ' + path)
            return None
        path = os.path.abspath(path)
        module_dir, module_file = os.path.split(path)
        module_name, _ = os.path.splitext(module_file)
        # adjust sys.path, runtime counterpart of PYTHONPATH, to temporarily
        # include the folder containing the user configuration module
        sys.path.append(module_dir)
        module_obj = importlib.import_module(module_name)
        sys.path.pop(0)
        return module_obj
    except ImportError as err:
        print(str(err))
        print("Looking in directory ")
        print(module_dir)
        return None
