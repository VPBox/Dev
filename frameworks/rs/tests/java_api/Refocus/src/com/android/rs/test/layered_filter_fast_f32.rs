#pragma version(1)
#pragma rs java_package_name(com.android.rs.refocus.renderscript)

// This is a speedup version of layered_filter_f32.rs using summation table.
// Several kernel functions and global functions for refocus rendering using
// Render Script. These functions are accessible from Java side. The assumptions
// made in this file are (consistent with Java side):
//
// 1. The depth value varies between 1 and number of depth levels. 0 is reserved
// for invalid pixels, e.g., padded pixels around image boundary.
// 2. The depth value represents inverse depth. Larger depths are closer to the
// camera.
// 3. The depth values are grouped into several blending layers.
// 4. The focal layer (i.e., the global variable g_focal_layer) is the depth
// value interval that has no blur.
//
// For the following kernel functions defined in this rs file, the input
// arguments have the following meaning:
//
// @param in an input RGBD pixel with @in.a being quantized inverse depth.
// @param x x-coordinate of @param in in the unpadded input image.
// @param y y-coordinate of @param in in the unpadded input image.

#include "image_and_kernel.rsh"
#include "camera_response.rsh"
#include "layer_info.rsh"
#include "pixel_format_f32.rsh"
#include "luts_for_speedup_f32.rsh"
#include "layered_filter_f32_helper.rsh"

// Image size of padded images: g_sharp_image and g_fuzzy_image.
static ImageSize_t g_image_size;

static CameraResponse_t g_camera_response;

float4 *g_sharp_image_buffer = NULL;
float4 *g_fuzzy_image_buffer = NULL;
float4 *g_integral_image_buffer = NULL;

// This image buffer is used to store input image initially.
// Then the input image gets updated as each layer is processed from the
// back-most to the focal depth. This image buffer is padded.
static SharpPixelF32_t *g_sharp_image = NULL;

// This image buffer is used to save an integral image of g_sharp_image
// modulated by the visibility mask.
static float4 *g_integral_image = NULL;

// whether or not to use integral image in this iteration.
static int g_use_integral_image;

// This image buffer is used to store output image, both for intermediate
// results and final result.
// In the first pass from back-most to focal depth, g_fuzzy_image holds the
// layer filtering result temporarily and then the result is used to update the
// input image after each layer is processed.
// In the second pass from front-most to focal depth, g_fuzzy_image accumulates
// all the layer filtering results and in the end, the accumulation result is
// blended with the input image (which has been updated in the first pass) to
// generate the final result. This image buffer is padded.
static FuzzyPixelF32_t *g_fuzzy_image = NULL;

// The info of current layer that is being processed.
static LayerInfo_t g_target_layer;
// The dilation radius that controls how the current layer should be blended
// with previous layers.
static BlendInfo_t g_blend_info;

// The depth level that is in focus.
static LayerInfo_t g_focal_layer;

// For depth d, let n = d-g_target_layer.back_depth be the relative depth index.
// g_kernel_stack+g_kernel_info[n].offset is the pointer to the kernel matrix
// for this depth level.
// The kernel has a center at
// (g_kernel_info[n].radius_x, g_kernel_info[n].radius_y).
// And the kernel has a size of
// (2*center_x+1, 2*center_y+1) ;
float *g_kernel_stack;
KernelInfo_t *g_kernel_info;

// Precomputed LUTs for speedup.
static VisibilityProbability_t g_visibility_probability;
static SecantOffset_t g_secant_offset;

static const float g_kOneOver255 = 1.0f / 255.0f;
static const float g_kAlmostOne = .998f;

// (1 << g_kDepthScaleShift) must be the same as BlurStack::DEPTH_SCALE.
static const int g_kDepthScaleShift = 2;
// g_kMaxDepth must be the same as BlurStack::MAX_DETPH.
static const int g_kMaxDepth = 256 >> g_kDepthScaleShift;

// Copies an input (unpadded) RGBD image into g_sharp_image, which has been
// padded with margin. Initialize other fields.
void __attribute__((kernel))
UnpackInputImage(uchar4 in, uint32_t x, uint32_t y) {
  // Maps (x,y) to the padded image coordinate system.
  x += g_image_size.margin;
  y += g_image_size.margin;

  const int index = y * g_image_size.width + x;

  SharpPixelF32_t *sharp = g_sharp_image + index;
  sharp->red = in.r * g_kOneOver255;
  sharp->green = in.g * g_kOneOver255;
  sharp->blue = in.b * g_kOneOver255;

  sharp->red =
      ApplyLUT_Float(sharp->red, g_camera_response.lut_remove_crf_float);
  sharp->green =
      ApplyLUT_Float(sharp->green, g_camera_response.lut_remove_crf_float);
  sharp->blue =
      ApplyLUT_Float(sharp->blue, g_camera_response.lut_remove_crf_float);

  sharp->actual_depth = g_kMaxDepth - (in.a >> g_kDepthScaleShift);

  sharp->active = 0;

  sharp->matte = 0;

  sharp->dilated_depth = 0;
}

// Marks active pixels that are on the target layer.
// Initializes the matte of active pixels to be the dilation_radius+1, which is
// equivalent to 1.
// Initializes dilated_depth of active pixels and pixels that are close to
// active pixels to be actual depths.
void __attribute__((kernel)) MarkLayerMask(uchar4 in, uint32_t x, uint32_t y) {
  const int actual_depth = g_kMaxDepth - (in.a >> g_kDepthScaleShift);
  if (!OnTheLayer(actual_depth, &g_target_layer)) return;

  // Maps (x,y) to the padded image coordinate system.
  x += g_image_size.margin;
  y += g_image_size.margin;

  const int index = y * g_image_size.width + x;  // index of this pixel
  SharpPixelF32_t *sharp = g_sharp_image + index;

  // Marks this pixel as active.
  sharp->active = 1;
  sharp->matte = g_blend_info.dilation_radius + 1;
  sharp->dilated_depth = sharp->actual_depth;

  // Next, tries to figure out whether or not this pixel is on the boundary
  // between active and inactive pixels
  int is_this_pixel_on_boundary = 0;
  SharpPixelF32_t *sharp_nbr = NULL;

  // Top
  sharp_nbr = sharp - g_image_size.width;
  is_this_pixel_on_boundary |=
      ValidDepthNotOnTheLayer(sharp_nbr->actual_depth, &g_target_layer);

  // Bottom
  sharp_nbr = sharp + g_image_size.width;
  is_this_pixel_on_boundary |=
      ValidDepthNotOnTheLayer(sharp_nbr->actual_depth, &g_target_layer);

  // Left
  sharp_nbr = sharp - 1;
  is_this_pixel_on_boundary |=
      ValidDepthNotOnTheLayer(sharp_nbr->actual_depth, &g_target_layer);

  // Right
  sharp_nbr = sharp + 1;
  is_this_pixel_on_boundary |=
      ValidDepthNotOnTheLayer(sharp_nbr->actual_depth, &g_target_layer);

  if (!is_this_pixel_on_boundary) {
    return;
  }

  // Marks pixels near the boundary of active pixels to compute matte later.
  const int kernel_center_x = g_blend_info.dilation_radius;
  const int kernel_center_y = g_blend_info.dilation_radius;
  const int kernel_dim_x = 2 * kernel_center_x + 1;

  // Moves sharp_nbr to the top left corner of this pixel.
  sharp_nbr = sharp - (kernel_center_y * g_image_size.width + kernel_center_x);

  // Visits every pixel in the window of radius (center_x,center_y)
  // surrounding this pixel.
  const int jump_to_next_pixel = 1;
  const int jump_to_next_row = g_image_size.width - kernel_dim_x;
  for (int j = -kernel_center_y; j <= kernel_center_y; ++j) {
    for (int i = -kernel_center_x; i <= kernel_center_x; ++i) {
      // Initializes dilated_depth as actual_depth.
      // The dilated_depth will then be updated in distance transform later.
      // A valid, non-zero dilated_depth indicates distance transform is
      // needed for the pixel. Otherwise, the distance transform will be
      // skipped.
      sharp_nbr->dilated_depth = sharp_nbr->actual_depth;

      sharp_nbr += jump_to_next_pixel;
    }
    sharp_nbr += jump_to_next_row;
  }
}

// Distance transform in processing layers in pass one from the back-most to
// the sharp depth.
void __attribute__((kernel))
ComputeLayerMatteBehindFocalDepth(uchar4 in, uint32_t x, uint32_t y) {
  // Maps (x,y) to the padded image coordinate system.
  x += g_image_size.margin;
  y += g_image_size.margin;

  const int index = y * g_image_size.width + x;
  SharpPixelF32_t *sharp = g_sharp_image + index;

  if (sharp->active == 0 && sharp->dilated_depth) {
    // This pixel is not active but within the dilation radius of the active
    // pixels.
    if (NotInFrontOfTheLayer(sharp->actual_depth, &g_target_layer)) {
      // We do not need to compute matte for depths in front of this layer,
      // because these pixels will be over-written later and hence we don't need
      // to blend them now.
      ComputeLayerMatteHelper(sharp, x, y, &g_image_size,
                              g_blend_info.dilation_radius);
    }
  }
}

// Distance transform in processing layers in pass two from the front-most to
// the sharp depth.
void __attribute__((kernel))
ComputeLayerMatteInFrontOfFocalDepth(uchar4 in, uint32_t x, uint32_t y) {
  // Maps (x,y) to the padded image coordinate system.
  x += g_image_size.margin;
  y += g_image_size.margin;

  const int index = y * g_image_size.width + x;
  SharpPixelF32_t *sharp = g_sharp_image + index;

  if (sharp->active == 0 && sharp->dilated_depth) {
    // This pixel is not active but within the dilation radius of the active
    // pixels.
    FuzzyPixelF32_t *fuzzy = g_fuzzy_image + index;
    if (fuzzy->alpha < g_kAlmostOne) {
      // This pixel has not been completely covered by layers in front of the
      // current layer.
      ComputeLayerMatteHelper(sharp, x, y, &g_image_size,
                              g_blend_info.dilation_radius);
    }
  }
}

// Computes integral image for target layer in processing layers in pass one
// from the back-most to the sharp depth.
void __attribute__((kernel))
ComputeIntegralImageForLayerBehindFocalDepth(uchar4 in, uint32_t x,
                                             uint32_t y) {
  // Maps (x,y) to the padded image coordinate system.
  // Kernel invocation should make sure x = 0.
  y += g_image_size.margin;

  // Gets the visibility probability lookup table for the target layer depth.
  const float *vis_prob =
      g_visibility_probability.lut[g_target_layer.front_depth];

  const int index = y * g_image_size.width;
  const SharpPixelF32_t *sharp = g_sharp_image + index;
  const SharpPixelF32_t *last_sharp = sharp + g_image_size.width;
  float4 *integral = g_integral_image + index;
  float4 prev_integral_value = 0;
  for (; sharp != last_sharp; sharp++, integral++) {
    const float weight = vis_prob[sharp->actual_depth];
    float4 this_value = {weight * sharp->red, weight * sharp->green,
                         weight * sharp->blue, weight};
    *integral = prev_integral_value + this_value;
    prev_integral_value = *integral;
  }
}

void __attribute__((kernel))
ComputeIntegralImageForLayerInFrontOfFocalDepth(uchar4 in, uint32_t x,
                                                uint32_t y) {
  // Maps (x,y) to the padded image coordinate system.
  // Kernel invocation should make sure x = 0.
  y += g_image_size.margin;

  const int index = y * g_image_size.width;
  const SharpPixelF32_t *sharp = g_sharp_image + index;
  const SharpPixelF32_t *last_sharp = sharp + g_image_size.width;
  float4 *integral = g_integral_image + index;
  float4 prev_integral_value = 0;
  for (; sharp != last_sharp; sharp++, integral++) {
    const float weight = ValidDepth(sharp->actual_depth);
    float4 this_value = {weight * sharp->red, weight * sharp->green,
                         weight * sharp->blue, weight};
    *integral = prev_integral_value + this_value;
    prev_integral_value = *integral;
  }
}

// Filters target layer in processing layers in pass one from the back-most to
// the sharp depth.
void __attribute__((kernel))
FilterLayerBehindFocalDepth(uchar4 in, uint32_t x, uint32_t y) {
  // Maps (x,y) to the padded image coordinate system.
  x += g_image_size.margin;
  y += g_image_size.margin;

  const int index = y * g_image_size.width + x;
  SharpPixelF32_t *sharp = g_sharp_image + index;

  if (sharp->matte == 0) {
    return;
  }

  // At this point, sharp->dilated_depth must be within the depth range of
  // this target layer. Hence, the kernel_info below is a valid pointer.
  const KernelInfo_t *kernel_info =
      g_kernel_info + sharp->dilated_depth - g_target_layer.back_depth;

  // Saves the filtering result in g_fuzzy_image temporarily.
  if (g_use_integral_image) {
    g_fuzzy_image[index] = FilterLayerUsingRowwiseIntegralImage(
        x, y, &g_image_size, kernel_info, g_integral_image, &g_secant_offset);
  } else {
    g_fuzzy_image[index] = FilterLayerBehindFocalDepthHelper(
        x, y, &g_image_size, &g_target_layer, kernel_info, g_kernel_stack,
        g_sharp_image, &g_visibility_probability);
  }
  // Once the kernel invoke is completed, uses the g_fuzzy_image to update
  // g_sharp_image.
}

// Filters target layer in processing layers in pass two from the front-most
// to the focus depth.
void __attribute__((kernel))
FilterLayerInFrontOfFocalDepth(uchar4 in, uint32_t x, uint32_t y) {
  // Maps (x,y) to the padded image coordinate system.
  x += g_image_size.margin;
  y += g_image_size.margin;

  const int index = y * g_image_size.width + x;
  SharpPixelF32_t *sharp = g_sharp_image + index;

  const int depth = sharp->dilated_depth;
  if (depth == 0) {
    return;
  }

  // At this point, this pixel must be either active or close enough to an
  // active pixel.
  // For such a pixel, its matte value can still be zero if its distance
  // transform to the active pixels is larger than dilation_radius.
  int matte = sharp->matte;

  // Resets fields of this pixel to prepare for the next iteration.
  sharp->active = 0;
  sharp->dilated_depth = 0;
  sharp->matte = 0;
  // Does not reset sharp->actual_depth, which will be used in future
  // iterations.

  if (matte == 0) {
    return;
  }

  // At this point, sharp->dilated_depth must be within the depth range of
  // this target layer. Hence kernel_info below is a valid pointer.
  const KernelInfo_t *kernel_info =
      g_kernel_info + depth - g_target_layer.back_depth;

  FuzzyPixelF32_t result;
  if (g_use_integral_image) {
    result = FilterLayerUsingRowwiseIntegralImage(
        x, y, &g_image_size, kernel_info, g_integral_image, &g_secant_offset);
  } else {
    result = FilterLayerInFrontOfFocalDepthHelper(
        x, y, &g_image_size, kernel_info, g_kernel_stack, g_sharp_image);
  }

  FuzzyPixelF32_t *fuzzy = g_fuzzy_image + index;
  // Because matte !=0 here, fuzzy->a < g_kAlmostOne must be true.
  // Otherwise, ComputeLayerMatteHelper won't be called in
  // ComputeLayerMatteInFrontOfFocalDepth, which results in a zero matte.

  // Accumulates the filtering result to fuzzy.
  const float capacity = 1.0f - fuzzy->alpha;
  const float factor = capacity * (float)matte * g_blend_info.matte_normalizer;
  fuzzy->red += factor * result.red;
  fuzzy->green += factor * result.green;
  fuzzy->blue += factor * result.blue;
  fuzzy->alpha += factor;
}

// Replaces active pixels in g_sharp_image with the filtering result saved in
// g_fuzzy_image. Does the replacement in a soft way by blending g_sharp_image
// and g_fuzzy_image using the matte field.
void __attribute__((kernel))
UpdateSharpImageUsingFuzzyImage(uchar4 in, uint32_t x, uint32_t y) {
  // Maps (x,y) to the padded image coordinate system.
  x += g_image_size.margin;
  y += g_image_size.margin;

  const int index = y * g_image_size.width + x;
  SharpPixelF32_t *sharp = g_sharp_image + index;

  const int depth = sharp->dilated_depth;

  if (depth == 0) {
    return;
  }

  // At this point, this pixel must be either active or close enough to an
  // active pixel.
  // For such a pixel, its matte value can still be zero if its distance
  // transform to the active pixels is larger than dilation_radius.
  const int matte = sharp->matte;

  // Resets fields of this pixel to prepare for future layer processing (pass
  // two).
  sharp->active = 0;
  sharp->dilated_depth = 0;
  sharp->matte = 0;
  // Does not reset sharp->actual depth, which will be used in future
  // layer processing.

  if (matte == 0) {
    return;
  }

  FuzzyPixelF32_t *fuzzy = g_fuzzy_image + index;
  float factor = (float)matte * g_blend_info.matte_normalizer;
  // The following blending amounts to:
  // sharp = (1 - factor) * sharp + factor * fuzzy.
  sharp->red += factor * (fuzzy->red - sharp->red);
  sharp->green += factor * (fuzzy->green - sharp->green);
  sharp->blue += factor * (fuzzy->blue - sharp->blue);

  fuzzy->red = 0;
  fuzzy->green = 0;
  fuzzy->blue = 0;
  fuzzy->alpha = 0;
}

// Fills in the pixels on or behind the focal depth in g_fuzzy_image using
// pixels in g_sharp_image. Does the filling in a soft way by blending using the
// matte field.
void __attribute__((kernel))
FinalizeFuzzyImageUsingSharpImage(uchar4 in, uint32_t x, uint32_t y) {
  // Maps (x,y) to the padded image coordinate system.
  x += g_image_size.margin;
  y += g_image_size.margin;

  const int index = y * g_image_size.width + x;
  const SharpPixelF32_t *sharp = g_sharp_image + index;
  FuzzyPixelF32_t *fuzzy = g_fuzzy_image + index;

  // Tests whether this pixel has a valid depth behind the focal depth.
  if (sharp->actual_depth && sharp->actual_depth <= g_focal_layer.front_depth) {
    // The alpha channel for sharp is 1.
    // fuzzy is a result of accumulation and hence is pre-multiplied with alpha.
    // The following blending amounts to:
    // fuzzy = fuzzy + (1 - fuzzy->a) * sharp.
    float capacity = 1.0f - fuzzy->alpha;
    fuzzy->red += sharp->red * capacity;
    fuzzy->green += sharp->green * capacity;
    fuzzy->blue += sharp->blue * capacity;
    fuzzy->alpha = 1.0f;
  }
}

// Copies g_fuzzy_image to output color image, excluding the padded margin.
// (x, y) is the pixel coordinate in the output image.
uchar4 __attribute__((kernel)) PackOutputImage(uint32_t x, uint32_t y) {
  // Maps (x,y) to the padded image coordinate system.
  x += g_image_size.margin;
  y += g_image_size.margin;

  // Returns the pixel at (x,y) after applying CRF.
  const int index = y * g_image_size.width + x;
  const FuzzyPixelF32_t *fuzzy = g_fuzzy_image + index;

  // Applies Camera Response using LUT.
  float4 result;
  result.r = ApplyLUT_Float(fuzzy->red, g_camera_response.lut_apply_crf_float);
  result.g =
      ApplyLUT_Float(fuzzy->green, g_camera_response.lut_apply_crf_float);
  result.b = ApplyLUT_Float(fuzzy->blue, g_camera_response.lut_apply_crf_float);
  result.a = fuzzy->alpha;

  return rsPackColorTo8888(result);
}

// Copies g_sharp_image to output color image, excluding the padded margin.
// (x, y) is the pixel coordinate in the output image.
// This kernel extracts intermediate images for testing purpose.
uchar4 __attribute__((kernel)) PackSharpImage(uint32_t x, uint32_t y) {
  // Maps (x,y) to the padded image coordinate system.
  x += g_image_size.margin;
  y += g_image_size.margin;

  // Returns the pixel at (x,y) after applying CRF.
  const int index = y * g_image_size.width + x;
  const SharpPixelF32_t *sharp = g_sharp_image + index;

  // Applies Camera Response using LUT.
  float4 result;
  result.r = ApplyLUT_Float(sharp->red, g_camera_response.lut_apply_crf_float);
  result.g =
      ApplyLUT_Float(sharp->green, g_camera_response.lut_apply_crf_float);
  result.b = ApplyLUT_Float(sharp->blue, g_camera_response.lut_apply_crf_float);

  return rsPackColorTo8888(result);
}

// Copies g_fuzzy_image to output color image, excluding the padded margin.
// (x, y) is the pixel coordinate in the output image.
// This kernel extracts intermediate images for testing purpose.
uchar4 __attribute__((kernel)) PackFuzzyImage(uint32_t x, uint32_t y) {
  // Maps (x,y) to the padded image coordinate system.
  x += g_image_size.margin;
  y += g_image_size.margin;

  // Returns the pixel at (x,y) after applying CRF.
  const int index = y * g_image_size.width + x;
  const FuzzyPixelF32_t *fuzzy = g_fuzzy_image + index;

  // Applies Camera Response using LUT.
  float4 result;
  result.r = ApplyLUT_Float(fuzzy->red, g_camera_response.lut_apply_crf_float);
  result.g =
      ApplyLUT_Float(fuzzy->green, g_camera_response.lut_apply_crf_float);
  result.b = ApplyLUT_Float(fuzzy->blue, g_camera_response.lut_apply_crf_float);
  result.a = fuzzy->alpha;

  return rsPackColorTo8888(result);
}

void SetTargetLayer(int front, int back) {
  g_target_layer.front_depth = front;
  g_target_layer.back_depth = back;
}

void SetBlendInfo(int radius) {
  g_blend_info.dilation_radius = radius;
  g_blend_info.matte_normalizer = 1.0f / (radius + 1);
}

void SetUseIntegralImage(int use_integral_image) {
  g_use_integral_image = use_integral_image;
}

void InitializeF32(int width, int height, int margin, int front, int back) {
  SetImageSize(width, height, margin, &g_image_size);
  g_focal_layer.front_depth = front;
  g_focal_layer.back_depth = back;

  g_sharp_image = (SharpPixelF32_t *)g_sharp_image_buffer;
  g_integral_image = g_integral_image_buffer;
  g_fuzzy_image = (FuzzyPixelF32_t *)g_fuzzy_image_buffer;

  const int num_pixels = width * height;
  ResetSharpImage(g_sharp_image, num_pixels);
  ResetIntegralImage(g_integral_image, num_pixels);
  ResetFuzzyImage(g_fuzzy_image, num_pixels);

  InitializeRadiusOffset(&g_secant_offset);
  InitializeVisibilityProbability(&g_visibility_probability, front, back);
  InitializeDefaultCameraResponse(&g_camera_response);
}
