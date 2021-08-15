// Copyright (C) 2011 The Android Open Source Project
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

static uchar4 twice(uchar4 in) {
  return in * 2;
}

static uchar getConstant() {
  return 37;
}

void NOP() {
  uchar3 x = {1, 2, 3};
  (void) convert_float3(x);
}

uchar4 __attribute__((kernel)) setImageAlpha(uchar4 in) {
  uchar4 out;
  out.rgba = twice(convert_uchar4((convert_uint4(in.rgba) * 37) >> (uint4)8));
  out.a = getConstant();
  NOP();
  return out;
}
