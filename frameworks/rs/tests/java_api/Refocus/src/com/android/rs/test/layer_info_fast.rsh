#ifndef LAYER_INFO_RSH
#define LAYER_INFO_RSH

// An object that contains the front and back depths values of a layer
typedef struct LayerInfo {
  // Front and back depth values of this layer.
  // front_depth >= back_depth.
  int front_depth;
  int back_depth;
} LayerInfo_t;

// An object that contains parameters used for computing matte for the current
// layer.
typedef struct BlendInfo {
  // The max kernel radius of a layer.
  int dilation_radius;

  // A scale factor =1.0f/(1+dilation_radius), which is used to normalize a
  // distance transform result to be a matte value within [0,1].
  // This data member is only used for PixelFormatF32.
  float matte_normalizer;
} BlendInfo_t;

static inline int ValidDepth(int depth) { return (depth != 0); }

static inline int NotInFrontOfTheLayer(int depth,
                                       const int2 layer_info_f2) {
  return (depth <= layer_info_f2.s0);
}

static inline int OnTheLayer(int depth, const int2 layer_info_i2) {
  //return (layer_info->back_depth <= depth && depth <= layer_info->front_depth);
  return (layer_info_i2.s1 <= depth && depth <= layer_info_i2.s0);
}

static inline int ValidDepthNotInFrontOfTheLayer(
    int depth, const int2 layer_info_f2) {
  //return (depth != 0) & (depth <= layer_info->front_depth);
  return (depth != 0) & (depth <= layer_info_f2.s0);
}

static inline int ValidDepthNotOnTheLayer(int depth,
                                          const int2 layer_info_f2) {
  return (depth != 0) &
         ((depth < layer_info_f2.s1) | (depth > layer_info_f2.s0));
}

#endif
