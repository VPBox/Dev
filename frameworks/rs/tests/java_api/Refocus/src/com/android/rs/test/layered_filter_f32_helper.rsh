#ifndef LAYERED_FILTER_F32_HELPER_RSH
#define LAYERED_FILTER_F32_HELPER_RSH

#include "pixel_format_f32.rsh"
#include "luts_for_speedup_f32.rsh"

// Several supporting functions that are called by kernel functions defined in
// layered_filter_f32.rs for refocus rendering.
// For the following supporting functions, the input arguments have the
// following meaning:
//
// @param x x-coordinate of a pixel in a padded image.
// @param y y-coordinate of a pixel in a padded image.

// A function that visits neighboring pixels around (x,y) and finds the closest
// active pixel to (x,y). Sets the closest distance to sharp->matte and sets the
// depth of the closest pixel to sharp->dilated_depth.
// When this function is called, the initial value of sharp->dilated_depth is
// sharp->actual_depth and must be non-zero. However, sharp->actual_depth may or
// may not be in the depth range of the target layer.
static inline void ComputeLayerMatteHelper(SharpPixelF32_t *sharp, uint32_t x,
                                           uint32_t y,
                                           const ImageSize_t *image_size,
                                           const int dilation_radius) {
  const int kernel_center_x = dilation_radius;
  const int kernel_center_y = dilation_radius;
  const int kernel_dim_x = 2 * kernel_center_x + 1;

  // Initializes the minimum distance from this pixel to the active pixels on
  // this layer.
  int min_dist = dilation_radius + 1;
  min_dist *= min_dist;  // Prepares for L2 distance.
  int depth = sharp->actual_depth;

  const SharpPixelF32_t *sharp_nbr =
      sharp - kernel_center_y * image_size->width - kernel_center_x;
  const int jump_to_next_pixel = 1;
  const int jump_to_next_row = image_size->width - kernel_dim_x;

  for (int j = -kernel_center_y; j <= kernel_center_y; ++j) {
    for (int i = -kernel_center_x; i <= kernel_center_x; ++i) {
      // L_2 distance:
      int this_dist = i * i + j * j;
      // Whether or not this is an active pixel with a smaller distance.
      int cond = (sharp_nbr->active && this_dist < min_dist);
      // Updates min_dist and corresponding depth.
      min_dist = cond ? this_dist : min_dist;
      depth = cond ? sharp_nbr->actual_depth : depth;

      sharp_nbr += jump_to_next_pixel;
    }
    sharp_nbr += jump_to_next_row;
  }

  // Converts min_dist to a matte value.
  sharp->matte = (uchar)(dilation_radius + 1 - (int)half_sqrt((float)min_dist));
  // If sharp->matte > 0, depth must be within the depth range of this layer.
  sharp->dilated_depth = (uchar)depth;
}

// Filters target layer when processing layers in pass one from the back-most to
// the sharp depth.
static inline FuzzyPixelF32_t FilterLayerBehindFocalDepthHelper(
    uint32_t x, uint32_t y, const ImageSize_t *image_size,
    const LayerInfo_t *layer_info, const KernelInfo_t *kernel_info,
    const float *kernel_stack, const SharpPixelF32_t *sharp_image,
    const VisibilityProbability_t *visibility_probability) {
  FuzzyPixelF32_t result = {0.0f, 0.0f, 0.0f, 0.0f};

  // Extracts kernel matrix from kernel stack using kernel_info.
  const int kernel_center_x = kernel_info->radius_x;
  const int kernel_center_y = kernel_info->radius_y;
  const int kernel_dim_x = 2 * kernel_center_x + 1;
  const float *kernel = kernel_stack + kernel_info->offset;

  // Gets the visibility probability lookup table for the target layer depth.
  const float *vis_prob = visibility_probability->lut[layer_info->front_depth];

  // Locates the top left corner of the window surrounding this pixel.
  const SharpPixelF32_t *sharp_nbr = sharp_image +
                                     (y - kernel_center_y) * image_size->width +
                                     (x - kernel_center_x);

  // Filters over the window.
  const int jump_to_next_pixel = 1;
  const int jump_to_next_row = image_size->width - kernel_dim_x;
  for (int j = -kernel_center_y; j <= kernel_center_y; ++j) {
    for (int i = -kernel_center_x; i <= kernel_center_x; ++i) {
      // Filters only using pixels on or behind this layer to avoid color
      // bleeding from layers in front of this layer. Doing visibility test in a
      // soft way using a probability set in a heuristic way.
      const float v_p = vis_prob[sharp_nbr->actual_depth];

      const float weight = v_p * (*kernel);
      result.red += weight * sharp_nbr->red;
      result.green += weight * sharp_nbr->green;
      result.blue += weight * sharp_nbr->blue;
      result.alpha += weight;
      kernel++;
      sharp_nbr += jump_to_next_pixel;
    }
    sharp_nbr += jump_to_next_row;
  }

  // Normalizes and returns the result.
  float factor = 1.0f / result.alpha;
  result.red *= factor;
  result.green *= factor;
  result.blue *= factor;
  result.alpha = 1.0f;
  return result;
}

// Filters target layer when processing layers in pass one from the back-most to
// the sharp depth.
static inline FuzzyPixelF32_t FilterLayerUsingRowwiseIntegralImage(
    uint32_t x, uint32_t y, const ImageSize_t *image_size,
    const KernelInfo_t *kernel_info, const float4 *integral_image,
    const SecantOffset_t *secant_offset) {
  float4 result = {0.0f, 0.0f, 0.0f, 0.0f};

  // Extracts kernel matrix from kernel stack using kernel_info.
  const int kernel_radius_y = kernel_info->radius_y;
  const int *radius_lut =
      secant_offset->lut[kernel_radius_y] + g_kMaxKernelRadius;

  // Locates the top left corner of the window surrounding this pixel.
  const float4 *integral_pixel =
      integral_image + (y - kernel_radius_y) * image_size->width + x;

  // Filters over the window.
  const int jump_to_next_row = image_size->width;
  for (int j = -kernel_radius_y; j <= kernel_radius_y; ++j) {
    const int kernel_radius_x_at_y = radius_lut[j];
    result += integral_pixel[kernel_radius_x_at_y] -
              integral_pixel[-kernel_radius_x_at_y - 1];
    integral_pixel += jump_to_next_row;
  }

  // Normalizes and returns the result.
  FuzzyPixelF32_t fuzzy;
  float factor = 1.0f / result.a;
  fuzzy.red = result.r * factor;
  fuzzy.green = result.g * factor;
  fuzzy.blue = result.b * factor;
  fuzzy.alpha = 1.0f;
  return fuzzy;
}

// Filters the target layer when processing layers in pass two from the
// front-most to the focus depth.
static inline FuzzyPixelF32_t FilterLayerInFrontOfFocalDepthHelper(
    uint32_t x, uint32_t y, const ImageSize_t *image_size,
    const KernelInfo_t *kernel_info, const float *kernel_stack,
    const SharpPixelF32_t *sharp_image) {
  FuzzyPixelF32_t result = {0.f, 0.f, 0.f, 0.f};

  // Extracts kernel matrix from kernel stack using kernel_info.
  const int kernel_center_x = kernel_info->radius_x;
  const int kernel_center_y = kernel_info->radius_y;
  const int kernel_dim_x = 2 * kernel_center_x + 1;
  const float *kernel = kernel_stack + kernel_info->offset;

  // Locates the top left corner of the window surrounding this pixel.
  const int index =
      (y - kernel_center_y) * image_size->width + (x - kernel_center_x);
  const SharpPixelF32_t *sharp_nbr = sharp_image + index;

  // Filters over the window.
  const int jump_to_next_pixel = 1;
  const int jump_to_next_row = image_size->width - kernel_dim_x;
  for (int j = -kernel_center_y; j <= kernel_center_y; ++j) {
    for (int i = -kernel_center_x; i <= kernel_center_x; ++i) {
      // Filters only using pixels with valid depth values (i.e. excluding
      // padded pixels).
      const float flag = ValidDepth(sharp_nbr->actual_depth);

      const float weight = flag * (*kernel);
      result.red += weight * sharp_nbr->red;
      result.green += weight * sharp_nbr->green;
      result.blue += weight * sharp_nbr->blue;
      result.alpha += weight;
      kernel++;
      sharp_nbr += jump_to_next_pixel;
    }
    sharp_nbr += jump_to_next_row;
  }

  // Normalizes and returns the result
  float fac = 1.0f / result.alpha;
  result.red *= fac;
  result.green *= fac;
  result.blue *= fac;
  result.alpha = 1.0f;
  return result;
}

static void ResetSharpImage(SharpPixelF32_t *sharp_image, int num_pixels) {
  const SharpPixelF32_t *last_p = sharp_image + num_pixels;
  for (SharpPixelF32_t *p = sharp_image; p != last_p; ++p) {
    p->red = 0;
    p->green = 0;
    p->blue = 0;
    p->active = 0;
    p->dilated_depth = 0;
    p->actual_depth = 0;
    p->matte = 0;
  }
}

static void ResetIntegralImage(float4 *integral_image, int num_pixels) {
  const float4 *last_p = integral_image + num_pixels;
  for (float4 *p = integral_image; p != last_p; ++p) {
    *p = 0;
  }
}

static void ResetFuzzyImage(FuzzyPixelF32_t *fuzzy_image, int num_pixels) {
  const FuzzyPixelF32_t *last_p = fuzzy_image + num_pixels;
  for (FuzzyPixelF32_t *p = fuzzy_image; p != last_p; ++p) {
    p->red = 0;
    p->green = 0;
    p->blue = 0;
    p->alpha = 0;
  }
}

#endif
