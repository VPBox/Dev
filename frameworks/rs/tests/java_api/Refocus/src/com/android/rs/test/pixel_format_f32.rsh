#ifndef PIXEL_FORMAT_F32_RSH
#define PIXEL_FORMAT_F32_RSH

// FocusPixel is the pixel format in the input image, which serves the
// following purposes in the refocus filter pipeline.
//
// 1. Initially, it stores the input color (red, green, blue) and the depth
// value (actual_depth). The actual_depth field is never changed once set.
//
// 2. When processing each layer, we use its field (active) to mark
// whether or not a pixel is on the current target layer. For pixels that are
// active or not active but "close enough" to an active pixel, a matte value is
// computed for blending color between layers. "Close enough" means L1 distance
// is no larger than dilation_radius defined in LayerInfo_t in "layer_info.rsh".
//
// 3. After each layer is processed from back-most layer to focal depth layer,
// the color (red, green, blue) of FocusPixel is updated and ready to be used in
// layer processing for the next iteration.
//
// 4. After each layer is processed from front-most layer to focal depth layer,
// the color (red, green, blue) of FocusPixel is *not* changed.
//
// 5. After each layer is processed in either of the two passes, the fields
// active, matte, and dilated_depth are reset. However, the field actual_depth
// field remains untouched once set initially.
typedef struct SharpPixelF32 {
  float red;
  float green;
  float blue;

  uchar actual_depth;

  // active = 1: the pixel is on the target layer;
  // active = 0: otherwise.
  uchar active;

  // matte is between 0 and dilation_radius+1, where dilation_radius is defined
  // in LayerInfo below.
  // matte/(dilation_radius+1) is the actual value used in layer blending.
  uchar matte;

  // For active pixels, dilated_depth is the same as actual depth;
  // For inactive pixels with non-zero matte, dilated_depth is the depth of the
  // closest active pixels;
  // For other pixels, 0 (invalid depth).
  uchar dilated_depth;

} SharpPixelF32_t;

typedef struct FuzzyPixelF32 {
  float red;
  float green;
  float blue;
  float alpha;
} FuzzyPixelF32_t;

#endif
