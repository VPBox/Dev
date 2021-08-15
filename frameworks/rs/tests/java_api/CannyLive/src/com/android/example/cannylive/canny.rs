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
#pragma rs java_package_name(com.android.example.cannylive)
#pragma rs_fp_relaxed

rs_allocation gCurrentFrame;     // yuv
rs_allocation gCurrentRGBFrame;  // RGB
rs_allocation blurImage;         // uchar
rs_allocation edgeImage;         // uchar

ushort __attribute__((kernel)) black_ushort() { return 0; }
uchar __attribute__((kernel)) black_uchar() { return 0; }
uchar4 __attribute__((kernel)) black_uchar4() { return 0; }

uchar4 __attribute__((kernel)) toRGB(uint32_t x, uint32_t y) {
  ushort v = rsGetElementAt_uchar(edgeImage, x, y);

  if (true) {
    uchar charv = clamp(v * 10, 0, 255);
    uchar4 out = {charv, charv, charv, 255};
    return out;
  }
}
uchar4 __attribute__((kernel)) toWhiteRGB(uint32_t x, uint32_t y) {
  ushort v = rsGetElementAt_uchar(edgeImage, x, y);

  uchar charv = 255 - clamp(v * 10, 0, 255);
  uchar4 out = {charv, charv, charv, 255};
  return out;
}
uchar4 __attribute__((kernel)) toRGBfuzz(uint32_t x, uint32_t y) {
  ushort v = rsGetElementAt_uchar(blurImage, x, y);

  if (true) {
    uchar charv = clamp(v * 10, 0, 255);
    uchar4 out = {charv, charv, charv, 255};
    return out;
  }
}
uchar4 __attribute__((kernel)) toWhiteRGBfuzz(uint32_t x, uint32_t y) {
  ushort v = rsGetElementAt_uchar(blurImage, x, y);

  uchar charv = 255 - clamp(v * 10, 0, 255);
  uchar4 out = {charv, charv, charv, 255};
  return out;
}

uchar4 __attribute__((kernel)) toRGBCartoon(uchar4 in, uint32_t x, uint32_t y) {
  ushort v = rsGetElementAt_uchar(blurImage, x, y);

  return in - (uchar)(v * 10);
}

uchar4 __attribute__((kernel)) toCartoon(uint32_t x, uint32_t y) {
  ushort v = rsGetElementAt_uchar(blurImage, x, y);

  uchar4 yuv;
  yuv.r =
      clamp(rsGetElementAtYuv_uchar_Y(gCurrentFrame, x, y) - v * 10, 0, 255);

  yuv.g = rsGetElementAtYuv_uchar_U(gCurrentFrame, x, y);
  yuv.b = rsGetElementAtYuv_uchar_V(gCurrentFrame, x, y);
  yuv.a = 255;

  int4 rgb;
  rgb.r = yuv.r + yuv.b * 1436 / 1024 - 179;
  rgb.g = yuv.r - yuv.g * 46549 / 131072 + 44 - yuv.b * 93604 / 131072 + 91;
  rgb.b = yuv.r + yuv.g * 1814 / 1024 - 227;
  rgb.a = 255;

  // Write out merged HDR result
  uchar4 out = convert_uchar4(clamp(rgb, 0, 255));
  return out;
}

static ushort getY(rs_allocation a, int x, int y) {
  ushort3 v = convert_ushort3(rsGetElementAt_uchar4(a, x, y).xyz);
  return v.x + v.y + v.z;
}

uchar __attribute__((kernel)) getLum(uchar4 in) {
  return (in.x + in.y + in.z) / 3;
}

uchar __attribute__((kernel)) getyuv_y(uint32_t x, uint32_t y) {
  return rsGetElementAtYuv_uchar_Y(gCurrentFrame, x, y);
}

uchar __attribute__((kernel)) blur_uchar(uint32_t x, uint32_t y) {
  ushort sum = 0;
  int x1 = x - 2;
  int y1 = y - 2;
  sum += 2 * rsGetElementAt_uchar(edgeImage, x1++, y1);
  sum += 4 * rsGetElementAt_uchar(edgeImage, x1++, y1);
  sum += 5 * rsGetElementAt_uchar(edgeImage, x1++, y1);
  sum += 4 * rsGetElementAt_uchar(edgeImage, x1++, y1);
  sum += 2 * rsGetElementAt_uchar(edgeImage, x1++, y1);
  x1 = x - 2;
  y1++;
  sum += 4 * rsGetElementAt_uchar(edgeImage, x1++, y1);
  sum += 9 * rsGetElementAt_uchar(edgeImage, x1++, y1);
  sum += 12 * rsGetElementAt_uchar(edgeImage, x1++, y1);
  sum += 9 * rsGetElementAt_uchar(edgeImage, x1++, y1);
  sum += 4 * rsGetElementAt_uchar(edgeImage, x1++, y1);

  x1 = x - 2;
  y1++;
  sum += 5 * rsGetElementAt_uchar(edgeImage, x1++, y1);
  sum += 12 * rsGetElementAt_uchar(edgeImage, x1++, y1);
  sum += 15 * rsGetElementAt_uchar(edgeImage, x1++, y1);
  sum += 12 * rsGetElementAt_uchar(edgeImage, x1++, y1);
  sum += 5 * rsGetElementAt_uchar(edgeImage, x1++, y1);

  x1 = x - 2;
  y1++;
  sum += 4 * rsGetElementAt_uchar(edgeImage, x1++, y1);
  sum += 9 * rsGetElementAt_uchar(edgeImage, x1++, y1);
  sum += 12 * rsGetElementAt_uchar(edgeImage, x1++, y1);
  sum += 9 * rsGetElementAt_uchar(edgeImage, x1++, y1);
  sum += 4 * rsGetElementAt_uchar(edgeImage, x1++, y1);

  x1 = x - 2;
  y1++;
  sum += 2 * rsGetElementAt_uchar(edgeImage, x1++, y1);
  sum += 4 * rsGetElementAt_uchar(edgeImage, x1++, y1);
  sum += 5 * rsGetElementAt_uchar(edgeImage, x1++, y1);
  sum += 4 * rsGetElementAt_uchar(edgeImage, x1++, y1);
  sum += 2 * rsGetElementAt_uchar(edgeImage, x1++, y1);

  return (uchar)(sum / 159);
}

ushort __attribute__((kernel)) blurRGB(uint32_t x, uint32_t y) {
  ushort sum = 0;
  int x1 = x - 2;
  int y1 = y - 2;
  sum += 2 * getY(gCurrentRGBFrame, x1++, y1);
  sum += 4 * getY(gCurrentRGBFrame, x1++, y1);
  sum += 5 * getY(gCurrentRGBFrame, x1++, y1);
  sum += 4 * getY(gCurrentRGBFrame, x1++, y1);
  sum += 2 * getY(gCurrentRGBFrame, x1++, y1);
  x1 = x - 2;
  y1++;
  sum += 4 * getY(gCurrentRGBFrame, x1++, y1);
  sum += 9 * getY(gCurrentRGBFrame, x1++, y1);
  sum += 12 * getY(gCurrentRGBFrame, x1++, y1);
  sum += 9 * getY(gCurrentRGBFrame, x1++, y1);
  sum += 4 * getY(gCurrentRGBFrame, x1++, y1);

  x1 = x - 2;
  y1++;
  sum += 5 * getY(gCurrentRGBFrame, x1++, y1);
  sum += 12 * getY(gCurrentRGBFrame, x1++, y1);
  sum += 15 * getY(gCurrentRGBFrame, x1++, y1);
  sum += 12 * getY(gCurrentRGBFrame, x1++, y1);
  sum += 5 * getY(gCurrentRGBFrame, x1++, y1);

  x1 = x - 2;
  y1++;
  sum += 4 * getY(gCurrentRGBFrame, x1++, y1);
  sum += 9 * getY(gCurrentRGBFrame, x1++, y1);
  sum += 12 * getY(gCurrentRGBFrame, x1++, y1);
  sum += 9 * getY(gCurrentRGBFrame, x1++, y1);
  sum += 4 * getY(gCurrentRGBFrame, x1++, y1);

  x1 = x - 2;
  y1++;
  sum += 2 * getY(gCurrentRGBFrame, x1++, y1);
  sum += 4 * getY(gCurrentRGBFrame, x1++, y1);
  sum += 5 * getY(gCurrentRGBFrame, x1++, y1);
  sum += 4 * getY(gCurrentRGBFrame, x1++, y1);
  sum += 2 * getY(gCurrentRGBFrame, x1++, y1);

  return sum / 159;
}

ushort __attribute__((kernel)) blur(uint32_t x, uint32_t y) {
  ushort sum = 0;
  int x1 = x - 2;
  int y1 = y - 2;
  sum += 2 * rsGetElementAtYuv_uchar_Y(gCurrentFrame, x1++, y1);
  sum += 4 * rsGetElementAtYuv_uchar_Y(gCurrentFrame, x1++, y1);
  sum += 5 * rsGetElementAtYuv_uchar_Y(gCurrentFrame, x1++, y1);
  sum += 4 * rsGetElementAtYuv_uchar_Y(gCurrentFrame, x1++, y1);
  sum += 2 * rsGetElementAtYuv_uchar_Y(gCurrentFrame, x1++, y1);
  x1 = x - 2;
  y1++;
  sum += 4 * rsGetElementAtYuv_uchar_Y(gCurrentFrame, x1++, y1);
  sum += 9 * rsGetElementAtYuv_uchar_Y(gCurrentFrame, x1++, y1);
  sum += 12 * rsGetElementAtYuv_uchar_Y(gCurrentFrame, x1++, y1);
  sum += 9 * rsGetElementAtYuv_uchar_Y(gCurrentFrame, x1++, y1);
  sum += 4 * rsGetElementAtYuv_uchar_Y(gCurrentFrame, x1++, y1);

  x1 = x - 2;
  y1++;
  sum += 5 * rsGetElementAtYuv_uchar_Y(gCurrentFrame, x1++, y1);
  sum += 12 * rsGetElementAtYuv_uchar_Y(gCurrentFrame, x1++, y1);
  sum += 15 * rsGetElementAtYuv_uchar_Y(gCurrentFrame, x1++, y1);
  sum += 12 * rsGetElementAtYuv_uchar_Y(gCurrentFrame, x1++, y1);
  sum += 5 * rsGetElementAtYuv_uchar_Y(gCurrentFrame, x1++, y1);

  x1 = x - 2;
  y1++;
  sum += 4 * rsGetElementAtYuv_uchar_Y(gCurrentFrame, x1++, y1);
  sum += 9 * rsGetElementAtYuv_uchar_Y(gCurrentFrame, x1++, y1);
  sum += 12 * rsGetElementAtYuv_uchar_Y(gCurrentFrame, x1++, y1);
  sum += 9 * rsGetElementAtYuv_uchar_Y(gCurrentFrame, x1++, y1);
  sum += 4 * rsGetElementAtYuv_uchar_Y(gCurrentFrame, x1++, y1);

  x1 = x - 2;
  y1++;
  sum += 2 * rsGetElementAtYuv_uchar_Y(gCurrentFrame, x1++, y1);
  sum += 4 * rsGetElementAtYuv_uchar_Y(gCurrentFrame, x1++, y1);
  sum += 5 * rsGetElementAtYuv_uchar_Y(gCurrentFrame, x1++, y1);
  sum += 4 * rsGetElementAtYuv_uchar_Y(gCurrentFrame, x1++, y1);
  sum += 2 * rsGetElementAtYuv_uchar_Y(gCurrentFrame, x1++, y1);

  return sum / 159;
}

uchar __attribute__((kernel)) edge(uint32_t x, uint32_t y) {
  int sum_h = 0;
  int x1 = x - 1;
  int y1 = y - 1;
  sum_h += -rsGetElementAt_uchar(blurImage, x1, y1);
  x += 2;
  sum_h += rsGetElementAt_uchar(blurImage, x1, y1);
  x1 = x - 1;
  y1++;

  sum_h += -2 * rsGetElementAt_uchar(blurImage, x1, y1);
  x1 += 2;
  sum_h += 2 * rsGetElementAt_uchar(blurImage, x1, y1);
  x1 = x - 1;
  y1++;

  sum_h += -rsGetElementAt_uchar(blurImage, x1, y1);
  x1 += 2;
  sum_h += rsGetElementAt_uchar(blurImage, x1, y1);
  int sum_v = 0;

  x1 = x - 1;
  y1 = y - 1;
  sum_v += -rsGetElementAt_uchar(blurImage, x1++, y1);
  sum_v += -2 * rsGetElementAt_uchar(blurImage, x1++, y1);
  sum_v += -rsGetElementAt_uchar(blurImage, x1++, y1);

  x1 = x - 1;
  y1 += 2;
  sum_v += rsGetElementAt_uchar(blurImage, x1++, y1);
  sum_v += 2 * rsGetElementAt_uchar(blurImage, x1++, y1);
  sum_v += rsGetElementAt_uchar(blurImage, x1++, y1);
  int v = (int)native_hypot((float)sum_v, (float)sum_h);
  v = v & (~3);
  int dir = (int)(4 * native_atan2pi((float)sum_v, (float)sum_h) + 8.5f);

  return (uchar)(v + (dir & 0x3));
}

uchar __attribute__((kernel)) thin(uint32_t x, uint32_t y) {
  int value = rsGetElementAt_uchar(edgeImage, x, y);

  int dir = value & 0x3;
  int dx[4] = {1, 1, 0, 1};
  int dy[4] = {0, 1, 1, -1};
  if (value < rsGetElementAt_uchar(edgeImage, x + dx[dir], y + dy[dir])) {
    return 0;
  }
  if (value < rsGetElementAt_uchar(edgeImage, x - dx[dir], y - dy[dir])) {
    return 0;
  }
  return value >> 2;
}

static const short dark_line = 4;
static const short bright_line = 16;
#define q_add(v)                 \
  {                              \
    if (q_size < 64) {           \
      queue[q_start & 0x3F] = v; \
      q_start++;                 \
      q_size++;                  \
    }                            \
  }
#define q_peak() (queue[q_peek_pos & 0x3F])
#define q_pop() (q_size--, q_tmp = queue[q_end], q_end++, q_tmp)

uchar __attribute__((kernel)) hysteresis(uchar in, uint32_t x, uint32_t y) {
  short3 queue[64];
  int q_start = 0;
  int q_end = 0;
  int q_size = 0;
  short3 q_tmp;
  int q_peek_pos = 0;
  int dx[] = {-1, 0, 1, -1, 1, -1, 0, 1};
  int dy[] = {-1, -1, -1, 0, 0, 1, 1, 1};
  if (in < dark_line) {
    return 0;
  }
  if (in >= bright_line) {
    return in;
  }
  short3 p = {0, 0, 1};
  q_add(p);

  while (q_peek_pos != q_start) {
    if (q_size > 60) {
      return 0;
    }
    p = q_peak();
    q_peek_pos++;
    short cdx = p[0];
    short cdy = p[1];

    int p_dist = p.z;
    short v;
    while (queue[q_end & 0x3F].z < p_dist - 1) {
      q_pop();
    }
    for (int i = 0; i < 8; i++) {
      int tx = cdx + dx[i];
      int ty = cdy + dy[i];
      bool skip = false;
      for (int k = q_end; k < q_start; k++) {
        short3 tq = queue[k & 0x3F];
        if (tq.x == tx && tq.y == ty) {
          skip = true;
          break;
        }
      }
      if (!(tx + x >= -5 && tx <= 5 && ty >= -5 && ty <= 5)) {
        skip = true;
      }
      if (skip) continue;

      v = rsGetElementAt_uchar(edgeImage, tx + x, ty + y);
      if (v >= bright_line) {
        return in;
      }
      if (v >= dark_line) {
        short3 tmp = {tx, ty, p_dist + 1};
        q_add(tmp);
      }
      q_peek_pos = (q_peek_pos + 1) & 0x3F;
    }
  }
  return 0;
}

rs_allocation hough_output;  // width * height

static inline float2 cos_sin(int n) {
  float2 cs[] = {{1.0f,0.0f},
                                  {0.9999619f,0.008726535f},{0.9998477f,0.017452406f},{0.99965733f,0.026176948f},
                                  {0.99939084f,0.034899496f},{0.99904823f,0.043619387f},{0.9986295f,0.052335955f},
                                  {0.9981348f,0.06104854f},{0.9975641f,0.06975647f},{0.9969173f,0.0784591f},
                                  {0.9961947f,0.087155744f},{0.9953962f,0.09584575f},{0.9945219f,0.104528464f},
                                  {0.9935719f,0.11320321f},{0.99254614f,0.12186934f},{0.9914449f,0.13052619f},
                                  {0.99026805f,0.1391731f},{0.9890159f,0.14780942f},{0.98768836f,0.15643446f},
                                  {0.9862856f,0.1650476f},{0.9848077f,0.17364818f},{0.9832549f,0.18223552f},
                                  {0.98162717f,0.190809f},{0.9799247f,0.19936794f},{0.9781476f,0.20791169f},
                                  {0.976296f,0.21643962f},{0.97437006f,0.22495106f},{0.9723699f,0.23344536f},
                                  {0.9702957f,0.2419219f},{0.96814764f,0.25038f},{0.9659258f,0.25881904f},
                                  {0.96363044f,0.26723838f},{0.9612617f,0.27563736f},{0.95881975f,0.28401536f},
                                  {0.9563047f,0.2923717f},{0.95371693f,0.3007058f},{0.95105654f,0.309017f},
                                  {0.94832367f,0.31730467f},{0.94551855f,0.32556817f},{0.9426415f,0.33380687f},
                                  {0.9396926f,0.34202015f},{0.9366722f,0.3502074f},{0.9335804f,0.35836795f},
                                  {0.9304176f,0.3665012f},{0.92718387f,0.37460658f},{0.9238795f,0.38268343f},
                                  {0.92050487f,0.39073113f},{0.9170601f,0.39874908f},{0.9135454f,0.40673664f},
                                  {0.9099613f,0.41469324f},{0.9063078f,0.42261827f},{0.90258527f,0.4305111f},
                                  {0.89879405f,0.43837115f},{0.89493436f,0.4461978f},{0.8910065f,0.4539905f},
                                  {0.8870108f,0.4617486f},{0.88294756f,0.46947157f},{0.87881714f,0.47715876f},
                                  {0.8746197f,0.4848096f},{0.8703557f,0.49242356f},{0.8660254f,0.5f},
                                  {0.8616292f,0.5075384f},{0.8571673f,0.5150381f},{0.85264015f,0.52249855f},
                                  {0.8480481f,0.52991927f},{0.8433914f,0.53729963f},{0.83867055f,0.54463905f},
                                  {0.83388585f,0.551937f},{0.82903755f,0.5591929f},{0.8241262f,0.56640625f},
                                  {0.81915206f,0.57357645f},{0.8141155f,0.58070296f},{0.809017f,0.58778524f},
                                  {0.80385685f,0.59482276f},{0.7986355f,0.60181504f},{0.7933533f,0.6087614f},
                                  {0.7880108f,0.6156615f},{0.78260815f,0.62251467f},{0.777146f,0.6293204f},
                                  {0.77162457f,0.63607824f},{0.76604444f,0.64278764f},{0.76040596f,0.64944804f},
                                  {0.7547096f,0.656059f},{0.7489557f,0.66262007f},{0.7431448f,0.6691306f},
                                  {0.7372773f,0.6755902f},{0.7313537f,0.6819984f},{0.7253744f,0.68835455f},
                                  {0.7193398f,0.6946584f},{0.71325046f,0.70090926f},{0.70710677f,0.70710677f},
                                  {0.70090926f,0.71325046f},{0.6946584f,0.7193398f},{0.68835455f,0.7253744f},
                                  {0.6819984f,0.7313537f},{0.6755902f,0.7372773f},{0.6691306f,0.7431448f},
                                  {0.66262007f,0.7489557f},{0.656059f,0.7547096f},{0.64944804f,0.76040596f},
                                  {0.64278764f,0.76604444f},{0.63607824f,0.77162457f},{0.6293204f,0.777146f},
                                  {0.62251467f,0.78260815f},{0.6156615f,0.7880108f},{0.6087614f,0.7933533f},
                                  {0.60181504f,0.7986355f},{0.59482276f,0.80385685f},{0.58778524f,0.809017f},
                                  {0.58070296f,0.8141155f},{0.57357645f,0.81915206f},{0.56640625f,0.8241262f},
                                  {0.5591929f,0.82903755f},{0.551937f,0.83388585f},{0.54463905f,0.83867055f},
                                  {0.53729963f,0.8433914f},{0.52991927f,0.8480481f},{0.52249855f,0.85264015f},
                                  {0.5150381f,0.8571673f},{0.5075384f,0.8616292f},{0.5f,0.8660254f},
                                  {0.49242356f,0.8703557f},{0.4848096f,0.8746197f},{0.47715876f,0.87881714f},
                                  {0.46947157f,0.88294756f},{0.4617486f,0.8870108f},{0.4539905f,0.8910065f},
                                  {0.4461978f,0.89493436f},{0.43837115f,0.89879405f},{0.4305111f,0.90258527f},
                                  {0.42261827f,0.9063078f},{0.41469324f,0.9099613f},{0.40673664f,0.9135454f},
                                  {0.39874908f,0.9170601f},{0.39073113f,0.92050487f},{0.38268343f,0.9238795f},
                                  {0.37460658f,0.92718387f},{0.3665012f,0.9304176f},{0.35836795f,0.9335804f},
                                  {0.3502074f,0.9366722f},{0.34202015f,0.9396926f},{0.33380687f,0.9426415f},
                                  {0.32556817f,0.94551855f},{0.31730467f,0.94832367f},{0.309017f,0.95105654f},
                                  {0.3007058f,0.95371693f},{0.2923717f,0.9563047f},{0.28401536f,0.95881975f},
                                  {0.27563736f,0.9612617f},{0.26723838f,0.96363044f},{0.25881904f,0.9659258f},
                                  {0.25038f,0.96814764f},{0.2419219f,0.9702957f},{0.23344536f,0.9723699f},
                                  {0.22495106f,0.97437006f},{0.21643962f,0.976296f},{0.20791169f,0.9781476f},
                                  {0.19936794f,0.9799247f},{0.190809f,0.98162717f},{0.18223552f,0.9832549f},
                                  {0.17364818f,0.9848077f},{0.1650476f,0.9862856f},{0.15643446f,0.98768836f},
                                  {0.14780942f,0.9890159f},{0.1391731f,0.99026805f},{0.13052619f,0.9914449f},
                                  {0.12186934f,0.99254614f},{0.11320321f,0.9935719f},{0.104528464f,0.9945219f},
                                  {0.09584575f,0.9953962f},{0.087155744f,0.9961947f},{0.0784591f,0.9969173f},
                                  {0.06975647f,0.9975641f},{0.06104854f,0.9981348f},{0.052335955f,0.9986295f},
                                  {0.043619387f,0.99904823f},{0.034899496f,0.99939084f},{0.026176948f,0.99965733f},
                                  {0.017452406f,0.9998477f},{0.008726535f,0.9999619f},{6.123234E-17f,1.0f},
                                  {-0.008726535f,0.9999619f},{-0.017452406f,0.9998477f},{-0.026176948f,0.99965733f},
                                  {-0.034899496f,0.99939084f},{-0.043619387f,0.99904823f},{-0.052335955f,0.9986295f},
                                  {-0.06104854f,0.9981348f},{-0.06975647f,0.9975641f},{-0.0784591f,0.9969173f},
                                  {-0.087155744f,0.9961947f},{-0.09584575f,0.9953962f},{-0.104528464f,0.9945219f},
                                  {-0.11320321f,0.9935719f},{-0.12186934f,0.99254614f},{-0.13052619f,0.9914449f},
                                  {-0.1391731f,0.99026805f},{-0.14780942f,0.9890159f},{-0.15643446f,0.98768836f},
                                  {-0.1650476f,0.9862856f},{-0.17364818f,0.9848077f},{-0.18223552f,0.9832549f},
                                  {-0.190809f,0.98162717f},{-0.19936794f,0.9799247f},{-0.20791169f,0.9781476f},
                                  {-0.21643962f,0.976296f},{-0.22495106f,0.97437006f},{-0.23344536f,0.9723699f},
                                  {-0.2419219f,0.9702957f},{-0.25038f,0.96814764f},{-0.25881904f,0.9659258f},
                                  {-0.26723838f,0.96363044f},{-0.27563736f,0.9612617f},{-0.28401536f,0.95881975f},
                                  {-0.2923717f,0.9563047f},{-0.3007058f,0.95371693f},{-0.309017f,0.95105654f},
                                  {-0.31730467f,0.94832367f},{-0.32556817f,0.94551855f},{-0.33380687f,0.9426415f},
                                  {-0.34202015f,0.9396926f},{-0.3502074f,0.9366722f},{-0.35836795f,0.9335804f},
                                  {-0.3665012f,0.9304176f},{-0.37460658f,0.92718387f},{-0.38268343f,0.9238795f},
                                  {-0.39073113f,0.92050487f},{-0.39874908f,0.9170601f},{-0.40673664f,0.9135454f},
                                  {-0.41469324f,0.9099613f},{-0.42261827f,0.9063078f},{-0.4305111f,0.90258527f},
                                  {-0.43837115f,0.89879405f},{-0.4461978f,0.89493436f},{-0.4539905f,0.8910065f},
                                  {-0.4617486f,0.8870108f},{-0.46947157f,0.88294756f},{-0.47715876f,0.87881714f},
                                  {-0.4848096f,0.8746197f},{-0.49242356f,0.8703557f},{-0.5f,0.8660254f},
                                  {-0.5075384f,0.8616292f},{-0.5150381f,0.8571673f},{-0.52249855f,0.85264015f},
                                  {-0.52991927f,0.8480481f},{-0.53729963f,0.8433914f},{-0.54463905f,0.83867055f},
                                  {-0.551937f,0.83388585f},{-0.5591929f,0.82903755f},{-0.56640625f,0.8241262f},
                                  {-0.57357645f,0.81915206f},{-0.58070296f,0.8141155f},{-0.58778524f,0.809017f},
                                  {-0.59482276f,0.80385685f},{-0.60181504f,0.7986355f},{-0.6087614f,0.7933533f},
                                  {-0.6156615f,0.7880108f},{-0.62251467f,0.78260815f},{-0.6293204f,0.777146f},
                                  {-0.63607824f,0.77162457f},{-0.64278764f,0.76604444f},{-0.64944804f,0.76040596f},
                                  {-0.656059f,0.7547096f},{-0.66262007f,0.7489557f},{-0.6691306f,0.7431448f},
                                  {-0.6755902f,0.7372773f},{-0.6819984f,0.7313537f},{-0.68835455f,0.7253744f},
                                  {-0.6946584f,0.7193398f},{-0.70090926f,0.71325046f},{-0.70710677f,0.70710677f},
                                  {-0.71325046f,0.70090926f},{-0.7193398f,0.6946584f},{-0.7253744f,0.68835455f},
                                  {-0.7313537f,0.6819984f},{-0.7372773f,0.6755902f},{-0.7431448f,0.6691306f},
                                  {-0.7489557f,0.66262007f},{-0.7547096f,0.656059f},{-0.76040596f,0.64944804f},
                                  {-0.76604444f,0.64278764f},{-0.77162457f,0.63607824f},{-0.777146f,0.6293204f},
                                  {-0.78260815f,0.62251467f},{-0.7880108f,0.6156615f},{-0.7933533f,0.6087614f},
                                  {-0.7986355f,0.60181504f},{-0.80385685f,0.59482276f},{-0.809017f,0.58778524f},
                                  {-0.8141155f,0.58070296f},{-0.81915206f,0.57357645f},{-0.8241262f,0.56640625f},
                                  {-0.82903755f,0.5591929f},{-0.83388585f,0.551937f},{-0.83867055f,0.54463905f},
                                  {-0.8433914f,0.53729963f},{-0.8480481f,0.52991927f},{-0.85264015f,0.52249855f},
                                  {-0.8571673f,0.5150381f},{-0.8616292f,0.5075384f},{-0.8660254f,0.5f},
                                  {-0.8703557f,0.49242356f},{-0.8746197f,0.4848096f},{-0.87881714f,0.47715876f},
                                  {-0.88294756f,0.46947157f},{-0.8870108f,0.4617486f},{-0.8910065f,0.4539905f},
                                  {-0.89493436f,0.4461978f},{-0.89879405f,0.43837115f},{-0.90258527f,0.4305111f},
                                  {-0.9063078f,0.42261827f},{-0.9099613f,0.41469324f},{-0.9135454f,0.40673664f},
                                  {-0.9170601f,0.39874908f},{-0.92050487f,0.39073113f},{-0.9238795f,0.38268343f},
                                  {-0.92718387f,0.37460658f},{-0.9304176f,0.3665012f},{-0.9335804f,0.35836795f},
                                  {-0.9366722f,0.3502074f},{-0.9396926f,0.34202015f},{-0.9426415f,0.33380687f},
                                  {-0.94551855f,0.32556817f},{-0.94832367f,0.31730467f},{-0.95105654f,0.309017f},
                                  {-0.95371693f,0.3007058f},{-0.9563047f,0.2923717f},{-0.95881975f,0.28401536f},
                                  {-0.9612617f,0.27563736f},{-0.96363044f,0.26723838f},{-0.9659258f,0.25881904f},
                                  {-0.96814764f,0.25038f},{-0.9702957f,0.2419219f},{-0.9723699f,0.23344536f},
                                  {-0.97437006f,0.22495106f},{-0.976296f,0.21643962f},{-0.9781476f,0.20791169f},
                                  {-0.9799247f,0.19936794f},{-0.98162717f,0.190809f},{-0.9832549f,0.18223552f},
                                  {-0.9848077f,0.17364818f},{-0.9862856f,0.1650476f},{-0.98768836f,0.15643446f},
                                  {-0.9890159f,0.14780942f},{-0.99026805f,0.1391731f},{-0.9914449f,0.13052619f},
                                  {-0.99254614f,0.12186934f},{-0.9935719f,0.11320321f},{-0.9945219f,0.104528464f},
                                  {-0.9953962f,0.09584575f},{-0.9961947f,0.087155744f},{-0.9969173f,0.0784591f},
                                  {-0.9975641f,0.06975647f},{-0.9981348f,0.06104854f},{-0.9986295f,0.052335955f},
                                  {-0.99904823f,0.043619387f},{-0.99939084f,0.034899496f},{-0.99965733f,0.026176948f},
                                  {-0.9998477f,0.017452406f},{-0.9999619f,0.008726535f},};
  return cs[n];
}

uchar4 __attribute__((kernel)) hough_map(uint32_t x, uint32_t y) {
  int w = rsAllocationGetDimX(hough_output);
  int h = rsAllocationGetDimY(hough_output);
  int ix = clamp((int)x, 0, w - 1);
  int iy = clamp((int)y, 0, h - 1);
  int v = rsGetElementAt_uchar(hough_output, ix, iy);
  uchar4 out = {clamp(v * 5, 0, 255), clamp(v, 0, 255), clamp(v, 0, 255), 255};
  return out;
}

/* input is the range of angles to cover */
void __attribute__((kernel)) hough(int2 in) {
  int max_pos = rsAllocationGetDimX(hough_output);
  int pos_shift = max_pos >> 1;
  int start = in.x;
  int end = in.y;
  int width = rsAllocationGetDimX(edgeImage);
  int height = rsAllocationGetDimY(edgeImage);
  float half_height = height >> 1;
  float half_width = width >> 1;
  for (int iy = 0; iy < height; iy++) {
    for (int ix = 0; ix < width; ix++) {
      if (rsGetElementAt_uchar(edgeImage, ix, iy) > 20) {
        float2 pos = {ix - half_width, iy - half_height};
        for (int ang = start; ang < end; ang++) {
          int r = pos_shift + (int)dot(pos, cos_sin(ang));
          if (r >= 0 && r < max_pos) {
            ushort tmp = rsGetElementAt_uchar(hough_output, r, ang);
            rsSetElementAt_uchar(hough_output, tmp + 1, r, ang);
          }
        }
      }
    }
  }
}

uchar __attribute__((kernel)) hough_thin(uchar in, int x, int y) {
  short tmp = rsGetElementAt_ushort(hough_output, x - 1, y - 1);
  if (tmp > in) return 0;
  tmp = rsGetElementAt_uchar(hough_output, x + 1, y + 1);
  if (tmp > in) return 0;

  tmp = rsGetElementAt_uchar(hough_output, x, y - 1);
  if (tmp > in) return 0;
  tmp = rsGetElementAt_uchar(hough_output, x + 1, y - 1);
  if (tmp > in) return 0;

  tmp = rsGetElementAt_uchar(hough_output, x - 1, y);
  if (tmp > in) return 0;

  tmp = rsGetElementAt_uchar(hough_output, x + 1, y);
  if (tmp > in) return 0;

  tmp = rsGetElementAt_uchar(hough_output, x - 1, y + 1);
  if (tmp > in) return 0;
  tmp = rsGetElementAt_uchar(hough_output, x, y + 1);
  if (tmp > in) return 0;
  return in;
}
