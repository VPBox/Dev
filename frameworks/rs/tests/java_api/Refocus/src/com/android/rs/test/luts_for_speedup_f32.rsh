#ifndef LUTS_FOR_SPEEDUP_F32_RSH
#define LUTS_FOR_SPEEDUP_F32_RSH

// Defines several LUTs for speed up for uint8 pixel type.

// Depth Range: [1,g_kMaxDepthValue]; 0 is reserved for invalid pixels.
// g_kMaxDepthValue must be the same as BlurStack::MAX_DEPTH.
static const int g_kMaxDepthValue = 64;

// A lookup table that heuristically computes the probability that a layer with
// depth y is visible in the presence of a pixel with depth x.
// VisibilityProbability.lut[y][x] = 1.0f/(max(x-y,0)+1).
// The value range of both x and y is [0,g_kMaxDepthValue].
// The value range of VisibilityProbability.lut[y][x] is [0,1].
typedef struct VisibilityProbability {
  float lut[g_kMaxDepthValue + 1][g_kMaxDepthValue + 1];
} VisibilityProbability_t;

// Assumes a <= b.
// When a = b, RemoveInterval(x,a,b) always returns x.
// When a < b,
//   if x <= a,     RemoveInterval(x,a,b) = x;
//   if a < x <= b, RemoveInterval(x,a,b) = a;
//   if b < x,      RemoveInterval(x,a,b) = a + (x-b).
static inline int RemoveInterval(int x, int a, int b) {
  return (x <= a) ? (x) : (a + max(x - b, 0));
}

static inline void InitializeVisibilityProbability(
    VisibilityProbability_t* visibility_probability, int front_focal_depth,
    int back_focal_depth) {
  // We expect back_focal_depth <= front_focal_depth.
  for (int y = 0; y <= g_kMaxDepthValue; y++) {
    for (int x = 0; x <= g_kMaxDepthValue; x++) {
      if (y == 0 || x == 0) {
        visibility_probability->lut[y][x] = 0;
      } else if (y <= back_focal_depth) {
        const int z = RemoveInterval(x, back_focal_depth, front_focal_depth);
        visibility_probability->lut[y][x] = 1.0f / (max(z - y, 0) + 1);
      } else if (y <= front_focal_depth) {
        // For a depth value within the focal layer, only depth values in front
        // of the focal layer are considered as its potential occluders. That
        // is, a depth value x that has a larger value (closer to the camera)
        // than y but is still within the focal layer is _not_ considered as an
        // occluder to y.
        visibility_probability->lut[y][x] =
            1.0f / (max(x - front_focal_depth, 0) + 1);
      } else {
        // For depths > front_focal_depth, visibility check is not used in
        // filtering. So always sets it to 1.
        visibility_probability->lut[y][x] = 1;
      }
    }
  }
}

// g_kMaxKernelRadius must match the value returned by
// KernelDataForRenderScript.computeKernelRadiusFromDiskRadius(
// BlurStack::MaxDiscRadius).
static const int g_kMaxKernelRadius = 25 + 1;

// A lookup table that pre-computes x coordinate of the intersection between a
// circle of radius r and a horizontal line of height y.
// lut[r][y+g_kMaxKernelRadius] = x;
// The value range of r is [0,g_kMaxKernelRadius].
// The value range of y is [-g_kMaxKernelRadius,g_kMaxKernelRadius].
typedef struct SecantOffset {
  int lut[g_kMaxKernelRadius + 1][2 * g_kMaxKernelRadius + 1];
} SecantOffset_t;

static inline void InitializeRadiusOffset(SecantOffset_t* secant_offset) {
  for (int r = 0; r <= g_kMaxKernelRadius; ++r) {
    for (int y = -g_kMaxKernelRadius; y <= g_kMaxKernelRadius; ++y) {
      float tmp = max(0, r * r - y * y);
      secant_offset->lut[r][y + g_kMaxKernelRadius] =
          (int)round(half_sqrt(tmp));
    }
  }
}

#endif