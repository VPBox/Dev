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

/**
  This describes the algorithm that uses the code
  This takes polygon and image to copy from(src_image) and and image to paste to(dest_image)

  void __attribute__((invoke_cpu)) findBestFit(rs_allocation poly,
                    rs_allocation src_image,
                    rs_allocation dest_image) {
    int width = rsAllocationGetDimX(src_image);
    int height = rsAllocationGetDimY(src_image);
    mask  = alloc_uchar(width, height, 0);
    src   = alloc_float3(width height, 0);
    dest1 = alloc_float3(width height, 0);
    dest2 = alloc_float3(width height, 0);
    mask = alloc_uchar(width, height, 0);
    foreach_convert_to_f(src_image, src);
    laplace = alloc_float3(width, height, 0);
    LaunchOptions options;
    options.setX(1, width-1);
    options.setY(1, height-1);
    forEach_laplacian(laplace, options);
    forEach_copyMasked(mask, dest1);
    int steps = (int) sqrt(calcMaskArea());
    for (int i = 0; i < steps; i++) {
            forEach_solve1(mask, dest2);
            forEach_solve2(mask, dest1);
    }
    forEach_convert_to_uc(dest1, dest_image);
    forEach_alphaMask(dest_image, dest_image);
  }


   int calcMaskArea(Allocation mask) {
      int width = rsAllocationGetDimX(mask);
      int height = rsAllocationGetDimY(mask);
      int count = 0;
      int val = rsGetElementAt_uchar(mask,0,0);
      for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
          if (val != rsGetElementAt_uchar(mask, x, y) ) {
              count++;
          }
        }
      }
     return count;
   }

*/

typedef rs_allocation AllocationF32_3;

AllocationF32_3 src;

float3 __attribute__((kernel)) laplacian( uint32_t x, uint32_t y) {
  float3 out = 4 * rsGetElementAt_float3(src, x, y);
  out -= rsGetElementAt_float3(src, x - 1, y);
  out -= rsGetElementAt_float3(src, x + 1, y);
  out -= rsGetElementAt_float3(src, x, y -1 );
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

float3 __attribute__((kernel)) copyMasked(uchar in, uint32_t x, uint32_t y) {
  return  rsGetElementAt_float3((in>0) ? src : dest1, x, y);
}

uchar4 __attribute__((kernel)) convert_to_uc(float3 in) {
  in = clamp(in, 0.0f, 255.0f);
  return convert_uchar4((float4){in.x, in.y, in.z, 0xFF});
}

uchar4 __attribute__((kernel)) alphaMask(uchar4 in, uint32_t x, uint32_t y) {
  if (rsGetElementAt_uchar(mask,x,y) == 0) {
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
  return rsGetElementAt_float3(dest1, x, y);;
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
  return rsGetElementAt_float3(dest2, x, y);;
}

rs_allocation image;
rs_allocation border; // float3
rs_allocation border_coords; //int2
int borderLength;

float3 __attribute__((kernel))extractBorder(int2 in) {
  return convert_float3(rsGetElementAt_uchar4(image, in.x, in.y).xyz);
}

float __attribute__((kernel)) bordercorrelation(uint32_t x, uint32_t y) {
  float sum = 0;
  for(int i = 0 ; i < borderLength; i++) {
    int2  coord = rsGetElementAt_int2(border_coords,i);
    float3 orig = convert_float3(rsGetElementAt_uchar4(image, coord.x + x, coord.y + y).xyz);
    float3 candidate = rsGetElementAt_float3(border, i).xyz;
    sum += distance(orig, candidate);
  }
  return sum;
}
