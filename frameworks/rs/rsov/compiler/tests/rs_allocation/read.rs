// Copyright (C) 2016 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma version(1)
#pragma rs java_package_name(rs2spirv)
#pragma rs_fp_relaxed

uchar c1;
rs_allocation alloc;

uchar4 __attribute__((kernel)) k1(uchar4 in) {
  uchar4 res = rsGetElementAt_uchar4(alloc, in.r, in.g);
  res.b += c1;
  return res;
}
