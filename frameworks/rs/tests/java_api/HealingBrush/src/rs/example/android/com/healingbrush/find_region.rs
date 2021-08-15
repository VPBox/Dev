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

rs_allocation image;
rs_allocation border_values; // float3
rs_allocation border_coords; //int2

int borderLength;
/**
  This describes the algorithm that uses the code
  This takes a float2 polygon and an image and searches a best fit polygon in the image

  int2  __attribute__((invoke_cpu)) findBestFit(rs_allocation poly, rs_allocation image) {
    int search_range_width;
    int search_range_height;
    borderLength = rsAllocationGetDimX(poly);
    int width = rsAllocationGetDimX(image);
    int height = rsAllocationGetDimY(image);

    border_coords = alloc_int2(borderLength, 0, 0);
    foreach(toInt, poly , border_coords);
    border_values = alloc_float3(borderLength, 0, 0);
    forEach_extractBorder(border_coords , border_values);

    fit = alloc_float(search_range_width, search_range_width, 0);
    borderLength = size;

    forEach_bordercorrelation(image);
    int2 fit_pos = gfindMin(fit, cx, cy, cwidth, cheight);
    return fit_pos;
 }

void findRegion(rs_allocation coord, rs_allocation image) {
  borderLength = rsAllocationGetDimX(coord);
  int width = rsAllocationGetDimX(image);
  int height = rsAllocationGetDimY(image);
}

static int4 padRegionRect(float4 rec) {
  int width = rec.z;
  int height = rec.w;
  int  mWidth = (((int) (8 + width)) & ~3); // bounding rectangle that is a power of 8 big
  int  mHeight = (((int) (8 + height)) & ~3);
  int4 out = {(int)(rec.x - 1 ),(int)(rec.y - 1), width, height};
  return out;
}


*/


static float4 gcalcBounds(rs_allocation xy) {
  int len = rsAllocationGetDimX(xy);
  float2 min_xy = rsGetElementAt_float(xy, 0);
  float2 max_xy = min_xy;
  for (int i = 0; i < len; i += 2) {
    float2 v = rsGetElementAt_float2(xy, i);
    min_xy = min(min_xy, v);
    max_xy = max(max_xy, v);
  }
  max_xy-=min_xy;
  float4 out = {min_xy.x, min_xy.y, max_xy.x, max_xy.y};
  return out;
}

void calcBounds(rs_allocation xy, rs_allocation rect) {
  float4 r = gcalcBounds(xy);
  rsSetElementAt_float2(rect, r.xy, 0);
  rsSetElementAt_float2(rect, r.zw, 1);
}


float3 __attribute__((kernel))extractBorder(int2 in) {
   return convert_float3(rsGetElementAt_uchar4(image, in.x, in.y).xyz);
}

int2 __attribute__((kernel)) toInt(float2 in) {
  int2 out = {(int)in.x, (int) in.y};
  return out;
}


static int2 gfindMin(rs_allocation fit, int cx, int cy, int cwidth, int cheight) {
  int w = rsAllocationGetDimX(fit);
  int h = rsAllocationGetDimY(fit);
  float minFit = rsGetElementAt_float(fit, 0, 0);
  int2 fit_pos = {0, 0};
  int reg_minx = cx;
  int reg_miny = cy;
  int mWidth =  cwidth;
  int mHeight =  cheight;
  int reg_width = mWidth;
  int reg_height = mHeight;
  int reg_maxx = reg_minx + mWidth;
  int reg_maxy = reg_miny + mHeight;
  for (int y = 0; y < h - mHeight; y++) {

    for (int x = 0; x < w - mWidth; x++) {
      if (!(x > reg_maxx || x + reg_width < reg_minx || y > reg_maxy
          || y + reg_height < reg_miny)) {
        continue;
      }
      float v = rsGetElementAt_float(fit, x, y);
      if (v < minFit) {
        minFit = v;
        fit_pos.x = x;
        fit_pos.y = y;
      }
    }
  }

  return fit_pos;
}

void findMin(rs_allocation fit, rs_allocation fit_max,int cx, int cy, int cwidth, int cheight) {
  int2 fit_pos = gfindMin(fit, cx, cy, cwidth, cheight);
  rsSetElementAt_int2(fit_max, fit_pos, 0);
}

int imagePosX;
int imagePosY;

float __attribute__((kernel)) bordercorrelation(uint32_t x, uint32_t y) {
  float sum = 0;
  int dx = x-imagePosX;
  int dy = y-imagePosY;
  for(int i = 0 ; i < borderLength; i++) {
    int2  coord = rsGetElementAt_int2(border_coords, i);
    float3 orig = convert_float3(rsGetElementAt_uchar4(image, coord.x + dx, coord.y + dy).xyz);
    float3 candidate = rsGetElementAt_float3(border_values, i).xyz;
    sum += distance(orig, candidate);
  }
  return sum;
}

