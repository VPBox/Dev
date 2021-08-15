#ifndef LAYERED_FILTER_FAST_HELPER_RSH
#define LAYERED_FILTER_FAST_HELPER_RSH

// Need to access global allocation here
// Declared in layered_filter_fast_f32.rs
// This allocation has three float values of red, green, blue
rs_allocation g_sharp_RGBA;

rs_allocation g_sharp_actual_depth;
rs_allocation g_sharp_active;
rs_allocation g_sharp_matte;
rs_allocation g_sharp_dilated_depth;

// This allocation contains four floats: red, green, blue, alpha
rs_allocation g_fuzzy_RGBA;
// This allocation contains four floats: red, green, blue, alpha
rs_allocation g_integral_RGBA;
// This allocation contains the kernel stack info: only one float per cell
rs_allocation g_kernel_stack;
// int2 that stores the target layer info: 1. front_depth, 2. back_depth; declared in layered_filter_f32_helper.rsh
static int2 g_target_layer_i2;


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


//Compute layer matte using global allocation
static inline void Alloc_ComputeLayerMatteHelper(uchar sharp_input_actual_depth, int index_sharp_meta,
                                           const int4 image_size_i4,
                                           const int dilation_radius) {
  const int kernel_center_x = dilation_radius;
  const int kernel_center_y = dilation_radius;
  const int kernel_dim_x = 2 * kernel_center_x + 1;

  // Initializes the minimum distance from this pixel to the active pixels on
  // this layer.
  int min_dist = dilation_radius + 1;
  min_dist *= min_dist;  // Prepares for L2 distance.
  int depth = sharp_input_actual_depth;  // Get meta data actual_depth

  int current_meta_index = index_sharp_meta - kernel_center_y * image_size_i4.s0 - kernel_center_x;  // image_size.width
  const int jump_to_next_pixel = 1;
  const int jump_to_next_row = image_size_i4.s0 - kernel_dim_x;  // image_size.width

  for (int j = -kernel_center_y; j <= kernel_center_y; ++j) {
    for (int i = -kernel_center_x; i <= kernel_center_x; ++i) {
      const uchar sharp_active = rsGetElementAt_uchar(g_sharp_active, current_meta_index);
      const uchar sharp_actual_depth = rsGetElementAt_uchar(g_sharp_actual_depth, current_meta_index);
      // L_2 distance:
      int this_dist = i * i + j * j;
      // Whether or not this is an active pixel with a smaller distance.
      int cond = (sharp_active && this_dist < min_dist);
      // Updates min_dist and corresponding depth.
      min_dist = cond ? this_dist : min_dist;
      depth = cond ? sharp_actual_depth : depth;

      current_meta_index += jump_to_next_pixel;
    }
    current_meta_index += jump_to_next_row;
  }

  // Converts min_dist to a matte value.
  rsSetElementAt_uchar(g_sharp_matte, (uchar)(dilation_radius + 1 - (int)(half_sqrt((float)min_dist) + 0.5f)), index_sharp_meta);
  // If sharp->matte > 0, depth must be within the depth range of this layer.
  rsSetElementAt_uchar(g_sharp_dilated_depth, (uchar)depth, index_sharp_meta);
}

// return sharp_dilated_depth
static inline uchar ComputeLayerMattePassInputHelper(uchar sharp_input_actual_depth, int index_sharp_meta,
                                           const int4 image_size_i4,
                                           const int dilation_radius) {
  const int kernel_center_x = dilation_radius;
  const int kernel_center_y = dilation_radius;
  const int kernel_dim_x = 2 * kernel_center_x + 1;

  // Initializes the minimum distance from this pixel to the active pixels on
  // this layer.
  int min_dist = dilation_radius + 1;
  min_dist *= min_dist;  // Prepares for L2 distance.
  int depth = sharp_input_actual_depth;  // Get meta data actual_depth

  int current_meta_index = index_sharp_meta - kernel_center_y * image_size_i4.s0 - kernel_center_x;	// image_size.width
  const int jump_to_next_pixel = 1;
  const int jump_to_next_row = image_size_i4.s0 - kernel_dim_x;	// image_size.width

  for (int j = -kernel_center_y; j <= kernel_center_y; ++j) {
    for (int i = -kernel_center_x; i <= kernel_center_x; ++i) {
      const uchar sharp_active = rsGetElementAt_uchar(g_sharp_active, current_meta_index);
      const uchar sharp_actual_depth = rsGetElementAt_uchar(g_sharp_actual_depth, current_meta_index);
      // L_2 distance:
      int this_dist = i * i + j * j;
      // Whether or not this is an active pixel with a smaller distance.
      int cond = (sharp_active && this_dist < min_dist);
      // Updates min_dist and corresponding depth.
      min_dist = cond ? this_dist : min_dist;
      depth = cond ? sharp_actual_depth : depth;

      current_meta_index += jump_to_next_pixel;
    }
    current_meta_index += jump_to_next_row;
  }
  // Converts min_dist to a matte value.
  rsSetElementAt_uchar(g_sharp_matte, (uchar)(dilation_radius + 1 - (int)(half_sqrt((float)min_dist) + 0.5f)), index_sharp_meta);
  // If sharp->matte > 0, depth must be within the depth range of this layer.
  return (uchar)depth;
}

// Filters target layer when processing layers in pass one from the back-most to
// the sharp depth.
// Use global allocation for filter from back most layer to focus layer
static inline float4 AllocFilterLayerBehindFocalDepthHelper(
    uint32_t x, uint32_t y, const int4 image_size_i4,
    const int2 layer_info_i2, const float4 alloc_kernel_info,
    const VisibilityProbability_t *visibility_probability) {
  float4 result = {0.0f, 0.0f, 0.0f, 0.0f};

  // Extracts kernel matrix from kernel stack using kernel_info.
  const int kernel_center_x = alloc_kernel_info.s1;
  const int kernel_center_y = alloc_kernel_info.s2;
  const int kernel_dim_x = 2 * kernel_center_x + 1;
  int current_kernel_index = alloc_kernel_info.s0;  // kernel_info.offset

  // Gets the visibility probability lookup table for the target layer depth.
  const float *vis_prob = visibility_probability->lut[layer_info_i2.s0]; // layer_info.front_depth

  // Locates the top left corner of the window surrounding this pixel.
  int current_meta_index =  (y - kernel_center_y) * image_size_i4.s0 +  // image_size.width
                                     (x - kernel_center_x);

  // Filters over the window.
  const int jump_to_next_pixel = 1;
  const int jump_to_next_row = image_size_i4.s0 - kernel_dim_x;  // image_size.width

  for (int j = -kernel_center_y; j <= kernel_center_y; ++j) {
    for (int i = -kernel_center_x; i <= kernel_center_x; ++i) {
      // Filters only using pixels on or behind this layer to avoid color
      // bleeding from layers in front of this layer. Doing visibility test in a
      // soft way using a probability set in a heuristic way.
        const uchar sharp_actual_depth = rsGetElementAt_uchar(g_sharp_actual_depth, current_meta_index);
        const float4 sharp_RGBA_nbr = rsGetElementAt_float4(g_sharp_RGBA, current_meta_index);
        const float kernel = rsGetElementAt_float(g_kernel_stack, current_kernel_index);

        const float v_p = vis_prob[sharp_actual_depth];

        const float weight = v_p * (kernel);
        result.r += weight * sharp_RGBA_nbr.r;
        result.g += weight * sharp_RGBA_nbr.g;
        result.b += weight * sharp_RGBA_nbr.b;
        result.a += weight;
        current_kernel_index++;
        current_meta_index += jump_to_next_pixel;
    }
    current_meta_index += jump_to_next_row;
  }

  // Normalizes and returns the result.
  float factor = 1.0f / result.a;
  result.r *= factor;
  result.g *= factor;
  result.b *= factor;
  result.a = 1.0f;
  return result;
}

// Filter target layer use global allocation using integral image
static inline float4 AllocFilterLayerUsingRowwiseIntegralImage(
    uint32_t x, uint32_t y, const int4 image_size_i4,
    const float4 alloc_kernel_info, const SecantOffset_t *secant_offset) {
    //const SecantOffset_t *secant_offset) {
  float4 result = {0.0f, 0.0f, 0.0f, 0.0f};

  // Extracts kernel matrix from kernel stack using kernel_info.
  const int kernel_radius_y = alloc_kernel_info.s2;  // kernel_info->radius_y
  const int *radius_lut =
      secant_offset->lut[kernel_radius_y] + g_kMaxKernelRadius;

  // Locates the top left corner index of the window surrounding this pixel.
  int current_index_integral = (y - kernel_radius_y) * image_size_i4.s0 + x;  // image_size.width

  // Filters over the window.
  const int jump_to_next_row = image_size_i4.s0;  // image_size.width
  for (int j = -kernel_radius_y; j <= kernel_radius_y; ++j) {
    const int kernel_radius_x_at_y = radius_lut[j];
    result += rsGetElementAt_float4(g_integral_RGBA, current_index_integral + kernel_radius_x_at_y) -
      rsGetElementAt_float4(g_integral_RGBA, current_index_integral - kernel_radius_x_at_y - 1);
    current_index_integral += jump_to_next_row;
  }

  // Normalizes and returns the result.
  float factor = 1.0f / result.a;
  result.r = result.r * factor;
  result.g = result.g * factor;
  result.b = result.b * factor;
  result.a = 1.0f;
  return result;
}

// Use global allocation to filter layer from the
// front most layer to focus layer.
static inline float4 AllocFilterLayerInFrontOfFocalDepthHelper(
    uint32_t x, uint32_t y, const int4 image_size_i4,
    const float4 alloc_kernel_info) {
  float4 result = {0.f, 0.f, 0.f, 0.f};

  // Extracts kernel matrix from kernel stack using kernel_info.
  const int kernel_center_x = alloc_kernel_info.s1;  // kernel_info->radius_x
  const int kernel_center_y = alloc_kernel_info.s2;  // kernel_info->radius_y
  const int kernel_dim_x = 2 * kernel_center_x + 1;

  int current_kernel_index = alloc_kernel_info.s0;  // kernel_info->offset

  // Locates the top left corner of the window surrounding this pixel.
  const int index =
      (y - kernel_center_y) * image_size_i4.s0 + (x - kernel_center_x);  // image_size.width
  int current_index = index;
  // Filters over the window.
  const int jump_to_next_pixel = 1;
  const int jump_to_next_row = image_size_i4.s0 - kernel_dim_x;  // image_size.width
  for (int j = -kernel_center_y; j <= kernel_center_y; ++j) {
    for (int i = -kernel_center_x; i <= kernel_center_x; ++i) {
      // Filters only using pixels with valid depth values (i.e. excluding
      // padded pixels).
      const float4 sharp_RGBA_nbr = rsGetElementAt_float4(g_sharp_RGBA, current_index);
      const uchar sharp_meta_actual_depth = rsGetElementAt_uchar(g_sharp_actual_depth, current_index);
      const float kernel = rsGetElementAt_float(g_kernel_stack, current_kernel_index);

      const float flag = ValidDepth(sharp_meta_actual_depth);
      const float weight = flag * (kernel);
      result.r += weight * sharp_RGBA_nbr.r;
      result.g += weight * sharp_RGBA_nbr.g;
      result.b += weight * sharp_RGBA_nbr.b;
      result.a += weight;
      current_kernel_index++;
      current_index += jump_to_next_pixel;
    }
    current_index += jump_to_next_row;
  }

  // Normalizes and returns the result
  float fac = 1.0f / result.a;
  result.r *= fac;
  result.g *= fac;
  result.b *= fac;
  result.a = 1.0f;
  return result;
}

#endif
