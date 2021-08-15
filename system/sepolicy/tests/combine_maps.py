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

"""Tool to combine SEPolicy mapping file.

Say, x, y, z are platform SEPolicy versions such that x > y > z. Then given two
mapping files from x to y (top) and y to z (bottom), it's possible to construct
a mapping file from x to z. We do the following to combine two maps.
1. Add all new types declarations from top to bottom.
2. Say, a new type "bar" in top is mapped like this "foo_V_v<-bar", then we map
"bar" to whatever "foo" is mapped to in the bottom map. We do this for all new
types in the top map.

More generally, we can correctly construct x->z from x->y' and y"->z as long as
y">y'.

This file contains the implementation of combining two mapping files.
"""
import argparse
import re
from mini_parser import MiniCilParser

def Combine(top, bottom):
    bottom.types.update(top.types)

    for top_ta in top.typeattributesets:
        top_type_set = top.typeattributesets[top_ta]
        if len(top_type_set) == 1:
            continue

        m = re.match(r"(\w+)_\d+_\d+", top_ta)
        # Typeattributes in V.v.cil have _V_v suffix, but not in V.v.ignore.cil
        bottom_type = m.group(1) if m else top_ta

        for bottom_ta in bottom.rTypeattributesets[bottom_type]:
            bottom.typeattributesets[bottom_ta].update(top_type_set)

    return bottom

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-t", "--top-map", dest="top_map",
                        required=True, help="top map file")
    parser.add_argument("-b", "--bottom-map", dest="bottom_map",
                        required=True, help="bottom map file")
    parser.add_argument("-o", "--output-file", dest="output_file",
                        required=True, help="output map file")
    args = parser.parse_args()

    top_map_cil = MiniCilParser(args.top_map)
    bottom_map_cil = MiniCilParser(args.bottom_map)
    result = Combine(top_map_cil, bottom_map_cil)

    with open(args.output_file, "w") as output:
        output.write(result.unparse())
