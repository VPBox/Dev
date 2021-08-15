#pragma version(1)
#pragma rs java_package_name(com.android.rs.refocus.renderscript)
#pragma rs_fp_relaxed
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

#include "camera_response.rsh"
#include "layer_info_fast.rsh"
#include "luts_for_speedup_f32.rsh"
#include "layered_filter_d1new_helper.rsh"

// Set force CPU variable for kernels:
// UnpackInputImage, ComputeLayerMatteBehindFocalDepth, ComputeIntegralImageForLayerBehindFocalDepth,
// FilterLayerBehindFocalDepth, updateSharpImageUsingFuzzyImage, ComputeLayerMatteInFrontOfFocalDepth,
// FilterLayerInFrontOfFocalDepth
// double cpuDummy;

// Image size of padded images: g_sharp_image and g_fuzzy_image.
// Float4 that stores the image size info: 1. width, 2. height, 3. margin
static int4 g_image_size_i4;

static CameraResponse_t g_camera_response;

// This image buffer is used to store input image initially.
// Then the input image gets updated as each layer is processed from the
// back-most to the focal depth. This image buffer is padded.

// This image buffer is used to save an integral image of g_sharp_image
// modulated by the visibility mask.

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

// The dilation radius that controls how the current layer should be blended
// with previous layers.
static BlendInfo_t g_blend_info;

// The depth level that is in focus.
// int2 that stores the focal layer info: 1. front_depth, 2. back_depth
static int2 g_focal_layer_i2;

// This is an allocation to store kernel info: 1. offset, 2. radius_x, 3. radius_y
rs_allocation galloc_kernel_info;

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
  x += g_image_size_i4.s2;  // margin
  y += g_image_size_i4.s2;  // margin

  const int index = y * g_image_size_i4.s0 + x;  // width

  float4 sharp_RGBA;

  sharp_RGBA.r =
      ApplyLUT_Float(in.r * g_kOneOver255, g_camera_response.lut_remove_crf_float);
  sharp_RGBA.g =
      ApplyLUT_Float(in.g * g_kOneOver255, g_camera_response.lut_remove_crf_float);
  sharp_RGBA.b =
      ApplyLUT_Float(in.b * g_kOneOver255, g_camera_response.lut_remove_crf_float);

  rsSetElementAt_float4(g_sharp_RGBA, sharp_RGBA, index);

  rsSetElementAt_uchar(g_sharp_actual_depth, g_kMaxDepth - (in.a >> g_kDepthScaleShift), index);
}

// Marks active pixels that are on the target layer.
// Initializes the matte of active pixels to be the dilation_radius+1, which is
// equivalent to 1.
// Initializes dilated_depth of active pixels and pixels that are close to
// active pixels to be actual depths.
void __attribute__((kernel)) MarkLayerMask(uchar4 in, uint32_t x, uint32_t y) {
  const int actual_depth = g_kMaxDepth - (in.a >> g_kDepthScaleShift);
  if (!OnTheLayer(actual_depth, g_target_layer_i2)) return;

  // Maps (x,y) to the padded image coordinate system.
  x += g_image_size_i4.s2;  // margin
  y += g_image_size_i4.s2;  // margin

  const int index = y * g_image_size_i4.s0 + x;  // width

  // Marks this pixel as active.
  rsSetElementAt_uchar(g_sharp_active, 1, index);
  rsSetElementAt_uchar(g_sharp_matte, g_blend_info.dilation_radius + 1, index);

  uchar sharp_actual_depth = rsGetElementAt_uchar(g_sharp_actual_depth, index);
  rsSetElementAt_uchar(g_sharp_dilated_depth, sharp_actual_depth, index);

  // Next, tries to figure out whether or not this pixel is on the boundary
  // between active and inactive pixels
  int is_this_pixel_on_boundary = 0;

  sharp_actual_depth = rsGetElementAt_uchar(g_sharp_actual_depth, index - g_image_size_i4.s0);
  is_this_pixel_on_boundary |=
    ValidDepthNotOnTheLayer(sharp_actual_depth, g_target_layer_i2);

  sharp_actual_depth = rsGetElementAt_uchar(g_sharp_actual_depth, index + g_image_size_i4.s0);
  is_this_pixel_on_boundary |=
    ValidDepthNotOnTheLayer(sharp_actual_depth, g_target_layer_i2);

  sharp_actual_depth = rsGetElementAt_uchar(g_sharp_actual_depth, index - 1);
  is_this_pixel_on_boundary |=
    ValidDepthNotOnTheLayer(sharp_actual_depth, g_target_layer_i2);

  sharp_actual_depth = rsGetElementAt_uchar(g_sharp_actual_depth, index + 1);
  is_this_pixel_on_boundary |=
    ValidDepthNotOnTheLayer(sharp_actual_depth, g_target_layer_i2);

  if (!is_this_pixel_on_boundary) {
    return;
  }

  // Marks pixels near the boundary of active pixels to compute matte later.
  const int kernel_center_x = g_blend_info.dilation_radius;
  const int kernel_center_y = g_blend_info.dilation_radius;
  const int kernel_dim_x = 2 * kernel_center_x + 1;

  // index of the current meta data, initailly set to top left sharp meta data
  // Moves sharp_nbr to the top left corner of this pixel.
  int current_meta_index = x - (kernel_center_y * g_image_size_i4.s0 + kernel_center_x);  // width
  const int jump_to_next_pixel = 1;
  const int jump_to_next_row = g_image_size_i4.s0 - kernel_dim_x;	// width
  for (int j = -kernel_center_y; j <= kernel_center_y; ++j) {
    // Initializes dilated_depth as actual_depth.
    // The dilated_depth will then be updated in distance transform later.
    // A valid, non-zero dilated_depth indicates distance transform is
    // needed for the pixel. Otherwise, the distance transform will be
    // skipped.
    for (int i = -kernel_center_x; i <= kernel_center_x; ++i) {
      sharp_actual_depth = rsGetElementAt_uchar(g_sharp_actual_depth, current_meta_index);
      rsSetElementAt_uchar(g_sharp_dilated_depth, sharp_actual_depth, current_meta_index);
      current_meta_index += jump_to_next_pixel;
    }
    current_meta_index += jump_to_next_row;
  }
}

// Another version of MarkLayerMask kernel that directly passes input allocation to kernels
// Input: g_sharp_actual_depth
// Output: g_sharp_dilated_depth
void __attribute__((kernel)) MarkLayerMaskPassInput(uchar in_sharp_actual_depth, uint32_t x) {
  if (!OnTheLayer(in_sharp_actual_depth, g_target_layer_i2)) return;

  // Marks this pixel as active.
  rsSetElementAt_uchar(g_sharp_active, 1, x);
  rsSetElementAt_uchar(g_sharp_matte, g_blend_info.dilation_radius + 1, x);

  // Next, tries to figure out whether or not this pixel is on the boundary
  // between active and inactive pixels
  int is_this_pixel_on_boundary = 0;

  uchar sharp_actual_depth = rsGetElementAt_uchar(g_sharp_actual_depth, x - g_image_size_i4.s0);
  is_this_pixel_on_boundary |=
    ValidDepthNotOnTheLayer(sharp_actual_depth, g_target_layer_i2);

  sharp_actual_depth = rsGetElementAt_uchar(g_sharp_actual_depth, x + g_image_size_i4.s0);
  is_this_pixel_on_boundary |=
    ValidDepthNotOnTheLayer(sharp_actual_depth, g_target_layer_i2);

  sharp_actual_depth = rsGetElementAt_uchar(g_sharp_actual_depth, x - 1);
  is_this_pixel_on_boundary |=
    ValidDepthNotOnTheLayer(sharp_actual_depth, g_target_layer_i2);

  sharp_actual_depth = rsGetElementAt_uchar(g_sharp_actual_depth, x + 1);
  is_this_pixel_on_boundary |=
    ValidDepthNotOnTheLayer(sharp_actual_depth, g_target_layer_i2);

  if (!is_this_pixel_on_boundary) {
    rsSetElementAt_uchar(g_sharp_dilated_depth, in_sharp_actual_depth, x);
    return;
  }

  // Marks pixels near the boundary of active pixels to compute matte later.
  const int kernel_center_x = g_blend_info.dilation_radius;
  const int kernel_center_y = g_blend_info.dilation_radius;
  const int kernel_dim_x = 2 * kernel_center_x + 1;

  // index of the current meta data, initailly set to top left sharp meta data
  // Moves sharp_nbr to the top left corner of this pixel.
  int current_meta_index = x - (kernel_center_y * g_image_size_i4.s0 + kernel_center_x);  // width
  const int jump_to_next_pixel = 1;
  const int jump_to_next_row = g_image_size_i4.s0 - kernel_dim_x;	// width
  for (int j = -kernel_center_y; j <= kernel_center_y; ++j) {
    // Initializes dilated_depth as actual_depth.
    // The dilated_depth will then be updated in distance transform later.
    // A valid, non-zero dilated_depth indicates distance transform is
    // needed for the pixel. Otherwise, the distance transform will be
    // skipped.
    for (int i = -kernel_center_x; i <= kernel_center_x; ++i) {
      sharp_actual_depth = rsGetElementAt_uchar(g_sharp_actual_depth, current_meta_index);
      rsSetElementAt_uchar(g_sharp_dilated_depth, sharp_actual_depth, current_meta_index);
      current_meta_index += jump_to_next_pixel;
    }
    current_meta_index += jump_to_next_row;
  }
}
// Distance transform in processing layers in pass one from the back-most to
// the sharp depth.
void __attribute__((kernel))
ComputeLayerMatteBehindFocalDepth(uchar4 in, uint32_t x, uint32_t y) {
  // Maps (x,y) to the padded image coordinate system.
  x += g_image_size_i4.s2;  // margin
  y += g_image_size_i4.s2;  // margin

  const int index = y * g_image_size_i4.s0 + x;  // width

  uchar sharp_active = rsGetElementAt_uchar(g_sharp_active, index);
  uchar sharp_dilated_depth = rsGetElementAt_uchar(g_sharp_dilated_depth, index);
  if (sharp_active == 0 && sharp_dilated_depth) {
    // This pixel is not active but within the dilation radius of the active
    // pixels.
    uchar sharp_actual_depth = rsGetElementAt_uchar(g_sharp_actual_depth, index);
    if (NotInFrontOfTheLayer(sharp_actual_depth, g_target_layer_i2)) {
      // We do not need to compute matte for depths in front of this layer,
      // because these pixels will be over-written later and hence we don't need
      // to blend them now.
      Alloc_ComputeLayerMatteHelper(sharp_actual_depth, index, g_image_size_i4,
        g_blend_info.dilation_radius);
      }
  }
}
// Pass input allocation directly to kernel ComputeLayerMatteBehindFocalDepth
// Input: g_sharp_dilated_depth
// Output: g_sharp_dilated_depth
uchar __attribute__((kernel))
ComputeLayerMatteBehindFocalDepthPassInput(uchar in_sharp_dilated_depth, uint32_t x) {
  uchar sharp_active = rsGetElementAt_uchar(g_sharp_active, x);
  if (sharp_active == 0 && in_sharp_dilated_depth) {
    // This pixel is not active but within the dilation radius of the active
    // pixels.
    uchar sharp_actual_depth = rsGetElementAt_uchar(g_sharp_actual_depth, x);
    if (NotInFrontOfTheLayer(sharp_actual_depth, g_target_layer_i2)) {
      // We do not need to compute matte for depths in front of this layer,
      // because these pixels will be over-written later and hence we don't need
      // to blend them now.
      return ComputeLayerMattePassInputHelper(sharp_actual_depth, x, g_image_size_i4,
        g_blend_info.dilation_radius);
    }
  }
  return in_sharp_dilated_depth;
}

// Distance transform in processing layers in pass two from the front-most to
// the sharp depth.
void __attribute__((kernel))
ComputeLayerMatteInFrontOfFocalDepth(uchar4 in, uint32_t x, uint32_t y) {
  // Maps (x,y) to the padded image coordinate system.
  x += g_image_size_i4.s2;  // margin
  y += g_image_size_i4.s2;  // margin

  const int index = y * g_image_size_i4.s0 + x;  // width

  uchar sharp_active = rsGetElementAt_uchar(g_sharp_active, index);
  uchar sharp_dilated_depth = rsGetElementAt_uchar(g_sharp_dilated_depth, index);
  if (sharp_active == 0 && sharp_dilated_depth) {
    // This pixel is not active but within the dilation radius of the active
    // pixels.
    float4 fuzzy_RGBA = rsGetElementAt_float4(g_fuzzy_RGBA, index);
    if (fuzzy_RGBA.a < g_kAlmostOne) {
      uchar sharp_actual_depth = rsGetElementAt_uchar(g_sharp_actual_depth, index);
      Alloc_ComputeLayerMatteHelper(sharp_actual_depth, index, g_image_size_i4,
        g_blend_info.dilation_radius);
    }
  }
}

// Pass input allocation directly into kernel ComputeLayerMatteInFrontOfFocalDepth
// Input: g_sharp_dilated_depth
// Output: g_sharp_dilated_depth
uchar __attribute__((kernel))
ComputeLayerMatteInFrontOfFocalDepthPassInput(uchar in_sharp_dilated_depth, uint32_t x) {
  uchar sharp_active = rsGetElementAt_uchar(g_sharp_active, x);
  if (sharp_active == 0 && in_sharp_dilated_depth) {
    // This pixel is not active but within the dilation radius of the active
    // pixels.
    float4 fuzzy_RGBA = rsGetElementAt_float4(g_fuzzy_RGBA, x);
    if (fuzzy_RGBA.a < g_kAlmostOne) {
      uchar sharp_actual_depth = rsGetElementAt_uchar(g_sharp_actual_depth, x);
      return ComputeLayerMattePassInputHelper(sharp_actual_depth,x, g_image_size_i4,
                        g_blend_info.dilation_radius);
    }
  }
  return in_sharp_dilated_depth;
}

// Computes integral image for target layer in processing layers in pass one
// from the back-most to the sharp depth.
void __attribute__((kernel))
ComputeIntegralImageForLayerBehindFocalDepth(uchar4 in, uint32_t x,
                                             uint32_t y) {
  // Maps (x,y) to the padded image coordinate system.
  // Kernel invocation should make sure x = 0.
  y += g_image_size_i4.s2;  // margin

  const int index = y * g_image_size_i4.s0;  // width
  float4 sharp_RGBA;
  float4 integral_RGBA;
  uchar sharp_actual_depth;

  int current_index = index;
  int last_index = index + g_image_size_i4.s0;  // width

  // Gets the visibility probability lookup table for the target layer depth.
  const float *vis_prob =
    g_visibility_probability.lut[g_target_layer_i2.s0];

  float4 prev_integral_value = 0;
  for(; current_index != last_index; current_index++) {
    sharp_RGBA = rsGetElementAt_float4(g_sharp_RGBA, current_index);
    sharp_actual_depth = rsGetElementAt_uchar(g_sharp_actual_depth, current_index);

    const float weight = vis_prob[sharp_actual_depth];

    float4 this_value = {weight * sharp_RGBA.r, weight * sharp_RGBA.g,
                         weight * sharp_RGBA.b, weight};
    prev_integral_value = prev_integral_value + this_value;

    // update pointer image
    integral_RGBA = prev_integral_value;
    rsSetElementAt_float4(g_integral_RGBA, integral_RGBA, current_index);
  }
}

void __attribute__((kernel))
ComputeIntegralImageForLayerInFrontOfFocalDepth(uchar4 in, uint32_t x,
                                                uint32_t y) {
  // Maps (x,y) to the padded image coordinate system.
  // Kernel invocation should make sure x = 0.
  y += g_image_size_i4.s2;  // margin

  const int index = y * g_image_size_i4.s0;  // width

  float4 sharp_RGBA;
  float4 integral_RGBA;
  uchar sharp_actual_depth;

  int current_index = index;
  int last_index = index + g_image_size_i4.s0;  // width
  float4 prev_integral_value = 0;
  for(; current_index != last_index; current_index++) {
    sharp_RGBA = rsGetElementAt_float4(g_sharp_RGBA, current_index);
    sharp_actual_depth = rsGetElementAt_uchar(g_sharp_actual_depth, current_index);

    const float weight = ValidDepth(sharp_actual_depth);
    float4 this_value = {weight * sharp_RGBA.r, weight * sharp_RGBA.g,
                         weight * sharp_RGBA.b, weight};
    prev_integral_value = prev_integral_value + this_value;

    // Update image
    integral_RGBA = prev_integral_value;
    rsSetElementAt_float4(g_integral_RGBA, integral_RGBA, current_index);
  }
}

// Filters target layer in processing layers in pass one from the back-most to
// the sharp depth.
void __attribute__((kernel))
FilterLayerBehindFocalDepth(uchar4 in, uint32_t x, uint32_t y) {
  // Maps (x,y) to the padded image coordinate system.
  x += g_image_size_i4.s2;  // margin
  y += g_image_size_i4.s2;  // margin

  const int index = y * g_image_size_i4.s0 + x;  // width

  uchar matte = rsGetElementAt_uchar(g_sharp_matte, index);
  if (matte == 0) {
    return;
  }

  // At this point, sharp->dilated_depth must be within the depth range of
  // this target layer. Hence, the kernel_info below is a valid pointer.
  uchar sharp_dilated_depth = rsGetElementAt_uchar(g_sharp_dilated_depth, index);
  const float4 alloc_kernel_info = rsGetElementAt_float4(galloc_kernel_info, sharp_dilated_depth - g_target_layer_i2.s1);

  float4 result;
  // Saves the filtering result in g_fuzzy_image temporarily.
  if (g_use_integral_image) {
    result = AllocFilterLayerUsingRowwiseIntegralImage(
        x, y, g_image_size_i4, alloc_kernel_info, &g_secant_offset);
  } else {
    result = AllocFilterLayerBehindFocalDepthHelper(
        x, y, g_image_size_i4, g_target_layer_i2,
        alloc_kernel_info, &g_visibility_probability);
  }
  rsSetElementAt_float4(g_fuzzy_RGBA, result, index);
  // Once the kernel invoke is completed, uses the g_fuzzy_image to update
  // g_sharp_image.
}

// Directly pass allocation to kernel FilterLayerBehindFocalDepth
// Input: g_fuzzy_RGBA
// Output: g_fuzzy_RGBA
float4 __attribute__((kernel))
FilterLayerBehindFocalDepthPassInput(float4 in_fuzzy_RGBA, uint32_t x) {
  uchar sharp_matte = rsGetElementAt_uchar(g_sharp_matte, x);
  if (sharp_matte == 0) {
    return in_fuzzy_RGBA;
  }

  // At this point, sharp->dilated_depth must be within the depth range of
  // this target layer. Hence, the kernel_info below is a valid pointer.
  uchar sharp_dilated_depth = rsGetElementAt_uchar(g_sharp_dilated_depth, x);
  const float4 alloc_kernel_info = rsGetElementAt_float4(galloc_kernel_info, sharp_dilated_depth - g_target_layer_i2.s1);

  float4 result;
  // Saves the filtering result in g_fuzzy_image temporarily.
  if (g_use_integral_image) {
    result = AllocFilterLayerUsingRowwiseIntegralImage(
        x, 0, g_image_size_i4, alloc_kernel_info, &g_secant_offset);
  } else {
    result = AllocFilterLayerBehindFocalDepthHelper(
        x, 0, g_image_size_i4, g_target_layer_i2,
        alloc_kernel_info, &g_visibility_probability);
  }
  return result;
  // Once the kernel invoke is completed, uses the g_fuzzy_image to update
  // g_sharp_image.
}

// Filters target layer in processing layers in pass two from the front-most
// to the focus depth.
void __attribute__((kernel))
FilterLayerInFrontOfFocalDepth(uchar4 in, uint32_t x, uint32_t y) {
  // Maps (x,y) to the padded image coordinate system.
  x += g_image_size_i4.s2;  // margin
  y += g_image_size_i4.s2;  // margin

  const int index = y * g_image_size_i4.s0 + x;  // width
  uchar depth = rsGetElementAt_uchar(g_sharp_dilated_depth, index);

  if (depth == 0) {
    return;
  }

  // At this point, this pixel must be either active or close enough to an
  // active pixel.
  // For such a pixel, its matte value can still be zero if its distance
  // transform to the active pixels is larger than dilation_radius.
  uchar matte = rsGetElementAt_uchar(g_sharp_matte, index);
  // Resets fields of this pixel to prepare for the next iteration.

  // Does not reset sharp->actual_depth, which will be used in future
  // iterations.
  rsSetElementAt_uchar(g_sharp_active, 0, index);
  rsSetElementAt_uchar(g_sharp_matte, 0, index);
  rsSetElementAt_uchar(g_sharp_dilated_depth, 0, index);

  if (matte == 0) {
    return;
  }

  // At this point, sharp->dilated_depth must be within the depth range of
  // this target layer. Hence kernel_info below is a valid pointer.
  const float4 alloc_kernel_info = rsGetElementAt_float4(galloc_kernel_info, depth - g_target_layer_i2.s1);

  float4 result;

  if (g_use_integral_image) {
    result = AllocFilterLayerUsingRowwiseIntegralImage(
        x, y, g_image_size_i4, alloc_kernel_info, &g_secant_offset);
  } else {
    result = AllocFilterLayerInFrontOfFocalDepthHelper(
        x, y, g_image_size_i4, alloc_kernel_info);
  }

  // Because matte !=0 here, fuzzy->a < g_kAlmostOne must be true.
  // Otherwise, ComputeLayerMatteHelper won't be called in
  // ComputeLayerMatteInFrontOfFocalDepth, which results in a zero matte.

  // Accumulates the filtering result to fuzzy.
  float4 fuzzy_RGBA = rsGetElementAt_float4(g_fuzzy_RGBA, index);
  const float capacity = 1.0f - fuzzy_RGBA.a;
  const float factor = capacity * (float)matte * g_blend_info.matte_normalizer;
  fuzzy_RGBA.r += factor * result.r;
  fuzzy_RGBA.g += factor * result.g;
  fuzzy_RGBA.b += factor * result.b;
  fuzzy_RGBA.a += factor;
  rsSetElementAt_float4(g_fuzzy_RGBA, fuzzy_RGBA, index);
}

// Pass input allocation directly to kernel FilterLayerInFrontOfFocalDepth
// Input: g_sharp_dilated_depth
// Output: g_sharp_dilated_depth
uchar __attribute__((kernel))
FilterLayerInFrontOfFocalDepthPassInput(uchar in_sharp_dilated_depth, uint32_t x) {
  if (in_sharp_dilated_depth == 0) {
    return 0;
  }
  // At this point, this pixel must be either active or close enough to an
  // active pixel.
  // For such a pixel, its matte value can still be zero if its distance
  // transform to the active pixels is larger than dilation_radius.
  uchar matte = rsGetElementAt_uchar(g_sharp_matte, x);
  // Resets fields of this pixel to prepare for the next iteration.

  // Does not reset sharp->actual_depth, which will be used in future
  // iterations.
  rsSetElementAt_uchar(g_sharp_active, 0, x);
  rsSetElementAt_uchar(g_sharp_matte, 0, x);

  if (matte == 0) {
    return 0;
  }

  // At this point, sharp->dilated_depth must be within the depth range of
  // this target layer. Hence kernel_info below is a valid pointer.
  const float4 alloc_kernel_info = rsGetElementAt_float4(galloc_kernel_info, in_sharp_dilated_depth - g_target_layer_i2.s1);

  float4 result;

  if (g_use_integral_image) {
    result = AllocFilterLayerUsingRowwiseIntegralImage(
        x, 0, g_image_size_i4, alloc_kernel_info, &g_secant_offset);
  } else {
    result = AllocFilterLayerInFrontOfFocalDepthHelper(
        x, 0, g_image_size_i4, alloc_kernel_info);
  }

  // Because matte !=0 here, fuzzy->a < g_kAlmostOne must be true.
  // Otherwise, ComputeLayerMatteHelper won't be called in
  // ComputeLayerMatteInFrontOfFocalDepth, which results in a zero matte.

  // Accumulates the filtering result to fuzzy.
  float4 fuzzy_RGBA = rsGetElementAt_float4(g_fuzzy_RGBA, x);
  const float capacity = 1.0f - fuzzy_RGBA.a;
  const float factor = capacity * (float)matte * g_blend_info.matte_normalizer;
  fuzzy_RGBA.r += factor * result.r;
  fuzzy_RGBA.g += factor * result.g;
  fuzzy_RGBA.b += factor * result.b;
  fuzzy_RGBA.a += factor;
  rsSetElementAt_float4(g_fuzzy_RGBA, fuzzy_RGBA, x);
  return 0;
}

// Replaces active pixels in g_sharp_image with the filtering result saved in
// g_fuzzy_image. Does the replacement in a soft way by blending g_sharp_image
// and g_fuzzy_image using the matte field.
void __attribute__((kernel))
UpdateSharpImageUsingFuzzyImage(uchar4 in, uint32_t x, uint32_t y) {
  // Maps (x,y) to the padded image coordinate system.
  x += g_image_size_i4.s2;  // margin
  y += g_image_size_i4.s2;  // margin

  const int index = y * g_image_size_i4.s0 + x;  // width

  uchar depth = rsGetElementAt_uchar(g_sharp_dilated_depth, index);

  if (depth == 0) {
    return;
  }

  // At this point, this pixel must be either active or close enough to an
  // active pixel.
  // For such a pixel, its matte value can still be zero if its distance
  // transform to the active pixels is larger than dilation_radius.

  // Resets fields of this pixel to prepare for future layer processing (pass
  // two).

  uchar matte = rsGetElementAt_uchar(g_sharp_matte, index);
  rsSetElementAt_uchar(g_sharp_active, 0, index);
  rsSetElementAt_uchar(g_sharp_matte, 0, index);
  rsSetElementAt_uchar(g_sharp_dilated_depth, 0, index);
  // Does not reset sharp->actual depth, which will be used in future
  // layer processing.

  if (matte == 0) {
    return;
  }

  float factor = (float)matte * g_blend_info.matte_normalizer;
  // The following blending amounts to:
  // sharp = (1 - factor) * sharp + factor * fuzzy.
  float4 sharp_RGBA = rsGetElementAt_float4(g_sharp_RGBA, index);
  float4 fuzzy_RGBA = rsGetElementAt_float4(g_fuzzy_RGBA, index);
  sharp_RGBA.r += factor * (fuzzy_RGBA.r - sharp_RGBA.r);
  sharp_RGBA.g += factor * (fuzzy_RGBA.g - sharp_RGBA.g);
  sharp_RGBA.b += factor * (fuzzy_RGBA.b - sharp_RGBA.b);
  fuzzy_RGBA.r = 0;
  fuzzy_RGBA.g = 0;
  fuzzy_RGBA.b = 0;
  fuzzy_RGBA.a = 0;
  rsSetElementAt_float4(g_sharp_RGBA, sharp_RGBA, index);
  rsSetElementAt_float4(g_fuzzy_RGBA, fuzzy_RGBA, index);
}

// Passes allocation for g_sharp_meta from java code
// so that one rsGetElement is eliminated.
// Still requires set and get for g_fuzzy_RGBA and g_sharp_RGBA
// Input: g_sharp_dilated_depth
// Output: g_sharp_dilated_depth
uchar __attribute__((kernel))
UpdateSharpUsingFuzzyPassInput(uchar in_sharp_dilated_depth, uint32_t x) {
  // Variable in is the uchar4 for sharp_meta
  if (in_sharp_dilated_depth == 0) {
    return 0;
  }

  // At this point, this pixel must be either active or close enough to an
  // active pixel.
  // For such a pixel, its matte value can still be zero if its distance
  // transform to the active pixels is larger than dilation_radius.

  // Resets fields of this pixel to prepare for future layer processing (pass
  // two).

  // Index of g_sharp_meta should align with g_fuzzy_RGBA and g_sharp_RGBA
  uchar matte = rsGetElementAt_uchar(g_sharp_matte, x);
  rsSetElementAt_uchar(g_sharp_active, 0, x);
  rsSetElementAt_uchar(g_sharp_matte, 0, x);
  // Does not reset sharp->actual depth, which will be used in future
  // layer processing.

  if (matte == 0) {
    return 0;
  }

  float factor = (float)matte * g_blend_info.matte_normalizer;
  // The following blending amounts to:
  // sharp = (1 - factor) * sharp + factor * fuzzy.
  float4 sharp_RGBA = rsGetElementAt_float4(g_sharp_RGBA, x);
  float4 fuzzy_RGBA = rsGetElementAt_float4(g_fuzzy_RGBA, x);
  sharp_RGBA.r += factor * (fuzzy_RGBA.r - sharp_RGBA.r);
  sharp_RGBA.g += factor * (fuzzy_RGBA.g - sharp_RGBA.g);
  sharp_RGBA.b += factor * (fuzzy_RGBA.b - sharp_RGBA.b);
  fuzzy_RGBA.r = 0;
  fuzzy_RGBA.g = 0;
  fuzzy_RGBA.b = 0;
  fuzzy_RGBA.a = 0;
  rsSetElementAt_float4(g_sharp_RGBA, sharp_RGBA, x);
  rsSetElementAt_float4(g_fuzzy_RGBA, fuzzy_RGBA, x);
  return 0;
}

// Fills in the pixels on or behind the focal depth in g_fuzzy_image using
// pixels in g_sharp_image. Does the filling in a soft way by blending using the
// matte field.
void __attribute__((kernel))
FinalizeFuzzyImageUsingSharpImage(uchar4 in, uint32_t x, uint32_t y) {
  // Maps (x,y) to the padded image coordinate system.
  x += g_image_size_i4.s2;  // margin
  y += g_image_size_i4.s2;  // margin

  const int index = y * g_image_size_i4.s0 + x;  // width

  float4 sharp_RGBA = rsGetElementAt_float4(g_sharp_RGBA, index);
  float4 fuzzy_RGBA = rsGetElementAt_float4(g_fuzzy_RGBA, index);
  uchar sharp_actual_depth = rsGetElementAt_uchar(g_sharp_actual_depth, index);

  if (sharp_actual_depth && sharp_actual_depth <= g_focal_layer_i2.s0) {  // g_focal_layer.front_depth
    float capacity = 1.0f - fuzzy_RGBA.a;
    fuzzy_RGBA.r += sharp_RGBA.r * capacity;
    fuzzy_RGBA.g += sharp_RGBA.g * capacity;
    fuzzy_RGBA.b += sharp_RGBA.b * capacity;
    fuzzy_RGBA.a = 1.0f;
    rsSetElementAt_float4(g_fuzzy_RGBA, fuzzy_RGBA, index);
  }
}
// Pass allocation directly to kernel FinalizeFuzzyImageUsingSharpImage
// Input: g_sharp_actual_depth
// Output: g_fuzzy_RGBA
float4 __attribute__((kernel))
FinalizeFuzzyImageUsingSharpImagePassInput(uchar in_sharp_actual_depth, uint32_t x) {
  float4 sharp_RGBA = rsGetElementAt_float4(g_sharp_RGBA, x);
  float4 fuzzy_RGBA = rsGetElementAt_float4(g_fuzzy_RGBA, x);

  if (in_sharp_actual_depth && in_sharp_actual_depth <= g_focal_layer_i2.s0) {  // g_focal_layer.front_depth
    float capacity = 1.0f - fuzzy_RGBA.a;
    fuzzy_RGBA.r += sharp_RGBA.r * capacity;
    fuzzy_RGBA.g += sharp_RGBA.g * capacity;
    fuzzy_RGBA.b += sharp_RGBA.b * capacity;
    fuzzy_RGBA.a = 1.0f;
    return fuzzy_RGBA;
  }
  return rsGetElementAt_float4(g_fuzzy_RGBA, x);
}

// Copies g_fuzzy_image to output color image, excluding the padded margin.
// (x, y) is the pixel coordinate in the output image.
uchar4 __attribute__((kernel)) PackOutputImage(uint32_t x, uint32_t y) {
  // Maps (x,y) to the padded image coordinate system.
  x += g_image_size_i4.s2;  // margin
  y += g_image_size_i4.s2;  // margin

  // Returns the pixel at (x,y) after applying CRF.
  const int index = y * g_image_size_i4.s0 + x;  // width

  const float4 fuzzy_RGBA = rsGetElementAt_float4(g_fuzzy_RGBA, index);
  float4 result;

  result.r = ApplyLUT_Float(fuzzy_RGBA.r, g_camera_response.lut_apply_crf_float);
  result.g = ApplyLUT_Float(fuzzy_RGBA.g, g_camera_response.lut_apply_crf_float);
  result.b = ApplyLUT_Float(fuzzy_RGBA.b, g_camera_response.lut_apply_crf_float);
  result.a = fuzzy_RGBA.a;

  return rsPackColorTo8888(result);
}

// Copies g_fuzzy_image to output color image, excluding the padded margin.
// (x, y) is the pixel coordinate in the output image.
uchar4 __attribute__((kernel)) PackSharpImage(uint32_t x, uint32_t y) {
  // Maps (x,y) to the padded image coordinate system.
  x += g_image_size_i4.s2;  // margin
  y += g_image_size_i4.s2;  // margin

  // Returns the pixel at (x,y) after applying CRF.
  const int index = y * g_image_size_i4.s0 + x;  // width

  const float4 sharp_RGBA = rsGetElementAt_float4(g_sharp_RGBA, index);
  float4 result;

  result.r = ApplyLUT_Float(sharp_RGBA.r, g_camera_response.lut_apply_crf_float);
  result.g = ApplyLUT_Float(sharp_RGBA.g, g_camera_response.lut_apply_crf_float);
  result.b = ApplyLUT_Float(sharp_RGBA.b, g_camera_response.lut_apply_crf_float);

  return rsPackColorTo8888(result);
}

// Copies g_fuzzy_image to output color image, excluding the padded margin.
// (x, y) is the pixel coordinate in the output image.
uchar4 __attribute__((kernel)) PackFuzzyImage(uint32_t x, uint32_t y) {
  // Maps (x,y) to the padded image coordinate system.
  x += g_image_size_i4.s2;  // margin
  y += g_image_size_i4.s2;  // margin

  // Returns the pixel at (x,y) after applying CRF.
  const int index = y * g_image_size_i4.s0 + x;  // width

  const float4 fuzzy_RGBA = rsGetElementAt_float4(g_fuzzy_RGBA, index);
  float4 result;

  result.r = ApplyLUT_Float(fuzzy_RGBA.r, g_camera_response.lut_apply_crf_float);
  result.g = ApplyLUT_Float(fuzzy_RGBA.g, g_camera_response.lut_apply_crf_float);
  result.b = ApplyLUT_Float(fuzzy_RGBA.b, g_camera_response.lut_apply_crf_float);
  result.a = fuzzy_RGBA.a;

  return rsPackColorTo8888(result);
}

void SetTargetLayer(int front, int back) {
  g_target_layer_i2.s0 = front;
  g_target_layer_i2.s1 = back;
}

void SetBlendInfo(int radius) {
  g_blend_info.dilation_radius = radius;
  g_blend_info.matte_normalizer = 1.0f / (radius + 1);
}

void SetUseIntegralImage(int use_integral_image) {
  g_use_integral_image = use_integral_image;
}

void InitializeFast(int width, int height, int margin, int front, int back) {
  // Initialize image size
  g_image_size_i4.s0 = width;
  g_image_size_i4.s1 = height;
  g_image_size_i4.s2 = margin;

  // Initialize focal layer
  g_focal_layer_i2.s0 = front;
  g_focal_layer_i2.s1 = back;

  const int num_pixels = width * height;

  for(int i = 0; i < num_pixels; i++) {
    float4 zerof4 = {0.0f, 0.0f, 0.0f, 0.0f};
    rsSetElementAt_float4(g_sharp_RGBA, zerof4, i);
    rsSetElementAt_float4(g_fuzzy_RGBA, zerof4, i);
    rsSetElementAt_float4(g_integral_RGBA, zerof4, i);

    rsSetElementAt_uchar(g_sharp_actual_depth, 0, i);
    rsSetElementAt_uchar(g_sharp_active, 0, i);
    rsSetElementAt_uchar(g_sharp_matte, 0, i);
    rsSetElementAt_uchar(g_sharp_dilated_depth, 0, i);
  }

  InitializeDefaultCameraResponse(&g_camera_response);
  InitializeRadiusOffset(&g_secant_offset);
  InitializeVisibilityProbability(&g_visibility_probability, front, back);
}
