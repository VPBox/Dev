#ifndef IMAGE_AND_KERNEL_RSH
#define IMAGE_AND_KERNEL_RSH

// Image size
typedef struct ImageSize {
  // width-by-height is the dimension of a padded image with a margin.
  int width;
  int height;
  // The dimension of the original unpadded image is
  // (width-2*margin)-by-(height-2*margin).
  int margin;
} ImageSize_t;

static void SetImageSize(int w, int h, int m, ImageSize_t *image_size) {
  image_size->width = w;
  image_size->height = h;
  image_size->margin = m;
}

// Auxiliary information that is needed to extract a kernel matrix from a buffer
// of a stack of kernel matrices.
typedef struct KernelInfo {
  // The starting position of a kernel matrix in a buffer of a stack of kernel
  // matrices.
  int offset;

  // The matrix is of size (2*radius_y+1)-by-(2*radius_x+1).
  int radius_x;
  int radius_y;
} KernelInfo_t;

#endif
