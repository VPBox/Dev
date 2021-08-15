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

#pragma version(1)
#pragma rs java_package_name(com.example.android.rs.sample)
#pragma rs_fp_relaxed

typedef rs_allocation AllocationF32_3;

AllocationF32_3 src;

float3 __attribute__((kernel)) laplacian(uint32_t x, uint32_t y) {
  float3 out = 4 * rsGetElementAt_float3(src, x, y);
  out -= rsGetElementAt_float3(src, x - 1, y);
  out -= rsGetElementAt_float3(src, x + 1, y);
  out -= rsGetElementAt_float3(src, x, y - 1);
  out -= rsGetElementAt_float3(src, x, y + 1);
  return out;
}

rs_allocation mask;       // uchar
AllocationF32_3 laplace;  // float3
AllocationF32_3 dest1;    // float3
AllocationF32_3 dest2;    // float3

float3 __attribute__((kernel)) convert_to_f(uchar4 in) {
  return convert_float3(in.xyz);
}
float3 __attribute__((kernel)) copy(float3 in) { return in; }

float3 __attribute__((kernel)) copyMasked(uchar in, uint32_t x, uint32_t y) {
  return rsGetElementAt_float3((in > 0) ? src : dest1, x, y);
}

uchar4 __attribute__((kernel)) convert_to_uc(float3 in) {
  in = clamp(in, 0.0f, 255.0f);
  return convert_uchar4((float4){in.x, in.y, in.z, 0xFF});
}

uchar4 __attribute__((kernel)) alphaMask(uchar4 in, uint32_t x, uint32_t y) {
  if (rsGetElementAt_uchar(mask, x, y) == 0) {
    return (uchar4){0, 0, 0, 0};
  }

  return in;
}

float3 __attribute__((kernel)) solve1(uchar in, uint32_t x, uint32_t y) {
  if (in > 0) {
    float3 k = rsGetElementAt_float3(dest1, x - 1, y);
    k += rsGetElementAt_float3(dest1, x + 1, y);
    k += rsGetElementAt_float3(dest1, x, y - 1);
    k += rsGetElementAt_float3(dest1, x, y + 1);
    k += rsGetElementAt_float3(laplace, x, y);
    k /= 4;
    return k;
  }
  return rsGetElementAt_float3(dest1, x, y);
}

float3 __attribute__((kernel)) solve2(uchar in, uint32_t x, uint32_t y) {
  if (in > 0) {
    float3 k = rsGetElementAt_float3(dest2, x - 1, y);
    k += rsGetElementAt_float3(dest2, x + 1, y);
    k += rsGetElementAt_float3(dest2, x, y - 1);
    k += rsGetElementAt_float3(dest2, x, y + 1);
    k += rsGetElementAt_float3(laplace, x, y);
    k /= 4;
    return k;
  }
  return rsGetElementAt_float3(dest2, x, y);
}

rs_allocation image;
rs_allocation border;         // float3
rs_allocation border_coords;  // int2
int borderLength;

float3 __attribute__((kernel)) extractBorder(int2 in) {
  return convert_float3(rsGetElementAt_uchar4(image, in.x, in.y).xyz);
}

float __attribute__((kernel)) bordercorrelation(uint32_t x, uint32_t y) {
  float sum = 0;
  for (int i = 0; i < borderLength; i++) {
    int2 coord = rsGetElementAt_int2(border_coords, i);
    float3 orig = convert_float3(
        rsGetElementAt_uchar4(image, coord.x + x, coord.y + y).xyz);
    float3 candidate = rsGetElementAt_float3(border, i).xyz;
    sum += distance(orig, candidate);
  }
  return sum;
}

static inline rs_allocation toFloat3(rs_allocation in) {
  int width = rsAllocationGetDimX(in);
  int height = rsAllocationGetDimY(in);

  rs_allocation tmp = rsCreateAllocation_float3(width, height);
  rsForEach(convert_to_f, in, tmp);
  return tmp;
}

static rs_allocation clone(rs_allocation in) {
  int width = rsAllocationGetDimX(in);
  int height = rsAllocationGetDimY(in);
  rs_allocation tmp = rsCreateAllocation_float3(width, height);
  rsForEach(copy, in, tmp);
  return tmp;
}

void heal(rs_allocation mask_image, rs_allocation src_image,
          rs_allocation dest_image) {
  int width = rsAllocationGetDimX(src_image);
  int height = rsAllocationGetDimY(src_image);
  src = toFloat3(src_image);
  mask = mask_image;
  laplace = rsCreateAllocation_float3(width, height);
  dest1 = toFloat3(dest_image);
  dest2 = clone(dest1);

  int steps = (int)hypot((float)width, (float)height);
  rsDebug("find_region.rs:steps = ", steps);

  rs_script_call_t opts = {0};
  opts.xStart = 1;
  opts.xEnd = width - 1;
  opts.yStart = 1;
  opts.yEnd = height - 1;
  rsForEachWithOptions(laplacian, &opts, laplace);
  rsForEach(copyMasked, mask, dest1);
  for (int i = 0; i < steps; i++) {
    rsForEach(solve1, mask, dest2);
    rsForEach(solve2, mask, dest1);
  }

  rsForEach(convert_to_uc, dest1, dest_image);
  rsForEach(alphaMask, dest_image, dest_image);
}
