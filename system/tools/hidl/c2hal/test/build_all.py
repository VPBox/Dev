#!/usr/bin/env python3
#
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



from os import listdir
from os.path import isfile, join as path_join
from subprocess import call
import argparse

def main():
    """this python program tries to build all hardware interfaces from a directory"""

    args = parseArgs()

    path = args.path
    is_open_gl = args.g

    success, failure = genFiles(path, is_open_gl)

    print("Success: ", ", ".join(success))
    print("Failure: ", ", ".join(failure))

    ratio = len(success) / (len(success) + len(failure))

    print("%% success = %.2f" % (100 * ratio))

def parseArgs():
    parser = argparse.ArgumentParser()
    parser.add_argument("path", help="location of headers to parse", type=str)
    parser.add_argument("-g", help="enable opengl specific parsing", action="store_true")

    return parser.parse_args()

def genFiles(path, is_open_gl):
    success = []
    failure = []

    for header in sorted(headers(path)):
        fname = header[:-2]

        command = ["c2hal",
                   "-r", "android.hardware:hardware/interfaces",
                   "-p", "android.hardware." + fname + "@1.0"]

        if is_open_gl:
            command += ["-g"]

        command += [path_join(path, header)]

        res = call(command)

        if res == 0:
            success += [header]
        else:
            failure += [header]

    return success, failure

def headers(path):
    """all .h files in a directory"""
    for item in listdir(path):
        if not isfile(path_join(path, item)):
            continue

        if not item.endswith(".h"):
            continue

        yield item



if __name__ == "__main__":
    main()