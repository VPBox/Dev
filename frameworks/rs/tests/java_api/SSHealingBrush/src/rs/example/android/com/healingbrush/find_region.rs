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
rs_allocation tmp;

static rs_allocation createVectorAllocation(rs_data_type dt, int vecSize,
                                            int gDimX, int gDimY, int gDimZ) {
  rs_element element;
  rs_type type;

  if (vecSize == 1)
    element = rsCreateElement(dt);
  else
    element = rsCreateVectorElement(dt, vecSize);

  if (gDimY == 0) {
    type = rsCreateType(element, gDimX);
  } else {
    type = rsCreateType(element, gDimX, gDimY, gDimZ);
  }
  tmp = rsCreateAllocation(type);
  return tmp;
}

rs_allocation mImage;
int2 mImgOffset;
int2 mSearchOffset;           // offset to the region we will search
rs_allocation mBorderValues;  // float3
rs_allocation mBorderCoords;  // int2

int mBorderLength;
int2 __attribute__((kernel)) toInt_ss(int2 in) { return in - mImgOffset; }

float3 __attribute__((kernel)) extractBorder_ss(uint2 in) {
  return convert_float3(rsGetElementAt_uchar4(mImage, in.x, in.y).xyz);
}

static float4 gCalcBounds(rs_allocation xy) {
  int len = rsAllocationGetDimX(xy);
  float2 min_xy = rsGetElementAt_float2(xy, 0);
  float2 max_xy = min_xy;
  for (int i = 1; i < len; i++) {
    float2 v = rsGetElementAt_float2(xy, i);
    min_xy = min(min_xy, v);
    max_xy = max(max_xy, v);
  }

  return (float4){min_xy.x, min_xy.y, max_xy.x, max_xy.y};
}

uint2 __attribute__((kernel)) toInt(float2 in) {
  return (uint2){(int)in.x, (int)in.y};
}

static int4 padRegionRect_ss(float4 rec) {
  int width = rec.z - rec.x;
  int height = rec.w - rec.y;
  int mWidth =
      (((int)(8 + width)) & ~3);  // bounding rectangle that is a power of 8 big
  int mHeight = (((int)(8 + height)) & ~3);
  int4 out = {(int)(rec.x - 1), (int)(rec.y - 1), mWidth + (int)(rec.x - 1),
              mHeight + (int)(rec.y - 1)};
  return out;
}

static int4 calcSearchRange_ss(int4 roiBounds, int imgWidth, int imgHeight,
                               int pad) {
  int2 size = roiBounds.zw - roiBounds.xy;
  int2 minr = max((int2){0, 0}, roiBounds.xy - size * (1 + pad));
  int2 maxr = min((int2){imgWidth, imgHeight}, roiBounds.zw + size * (1 + pad));
  return (int4){minr.x, minr.y, maxr.x, maxr.y};
}

float __attribute__((kernel)) bordercorrelation_ss(uint32_t x, uint32_t y) {
  float sum = 0.f;
  int2 delta = {x, y};
  delta += mSearchOffset;

  for (int i = 0; i < mBorderLength; i++) {
    int2 coord = rsGetElementAt_int2(mBorderCoords, i) + delta;
    float3 orig =
        convert_float3(rsGetElementAt_uchar4(mImage, coord.x, coord.y).xyz);
    float3 candidate = rsGetElementAt_float3(mBorderValues, i).xyz;
    sum += distance(orig, candidate);
  }
  return sum;
}

static int2 gfindMin_ss(rs_allocation fit, int4 regionInSearch) {
  int w = rsAllocationGetDimX(fit);
  int h = rsAllocationGetDimY(fit);
  float minFit = rsGetElementAt_float(fit, 0, 0);
  int2 fit_pos = {0, 0};

  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      int2 p = {x, y};
      p = (p - regionInSearch.xy) *
          (p - regionInSearch.zw);  // positive if outside of exclusion zone
      if (!(p.x < 0 && p.y < 0)) {
        float v = rsGetElementAt_float(fit, x, y);
        if (v < minFit) {
          minFit = v;
          fit_pos.x = x;
          fit_pos.y = y;
        }
      }
    }
  }

  return fit_pos;
}

static int width(int4 rec) { return rec.z - rec.x; }
static int height(int4 rec) { return rec.w - rec.y; }

/**
 *  Entry point to frind region
 */
void findRegion(rs_allocation pointsXY, rs_allocation image, int imgWidth,
                int imgHeight, rs_allocation ret) {
  rs_allocation fit;

  mImage = image;

  mBorderLength = rsAllocationGetDimX(pointsXY);

  float4 bounds = gCalcBounds(pointsXY);

  int4 roiBounds = padRegionRect_ss(bounds);

  int4 searchRange = calcSearchRange_ss(roiBounds, imgWidth, imgHeight, 2);

  int4 regionInSearch = roiBounds - searchRange.xyxy;

  mSearchOffset = searchRange.xy;
  mImgOffset = roiBounds.xy;

  mBorderCoords =
      createVectorAllocation(RS_TYPE_SIGNED_32, 2, mBorderLength, 0, 0);

  rsForEach(toInt, pointsXY, mBorderCoords);

  mBorderValues =
      createVectorAllocation(RS_TYPE_FLOAT_32, 3, mBorderLength, 0, 0);

  rsForEach(extractBorder_ss, mBorderCoords, mBorderValues);
  rsForEach(toInt_ss, mBorderCoords, mBorderCoords);

  int fit_width = width(searchRange) - width(roiBounds);
  int fit_height = height(searchRange) - height(roiBounds);

  fit = createVectorAllocation(RS_TYPE_FLOAT_32, 1, fit_width, fit_height, 0);

  int4 noZone = regionInSearch;
  noZone.xy -= convert_int2(bounds.zw - bounds.xy);
  noZone.zw += convert_int2(bounds.zw - bounds.xy);

  rsForEach(bordercorrelation_ss, fit);

  int2 fit_pos = gfindMin_ss(fit, noZone);

  rsSetElementAt_int2(ret, fit_pos + searchRange.xy, 0);
}
