/*
 * Copyright (C) 2016 The Android Open Source Project
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

#include <stdio.h>
#include <stdlib.h>

#include "libfdt.h"
#include "libufdt_sysdeps.h"

#include "util.h"


int find_dtb_header_pos(const char *buf, size_t buf_size) {
  if (buf == NULL || buf_size == 0) {
    return -1;
  }

  int pos = buf_size;
  while (1) {
    pos--;
    if (pos < 0) {
      break;
    }
    uint32_t tag = fdt32_to_cpu(*(fdt32_t *)(buf + pos));
    if (tag == FDT_MAGIC) {
      // Hit!
      break;
    }
  }

  return pos;
}

int find_and_write_dtb(const char *filename,
                       const char *buf, size_t buf_size) {
  int tag_pos = find_dtb_header_pos(buf, buf_size);
  if (tag_pos < 0) {
    goto end;
  }

  // Check FDT header
  const char *fdt_ptr = buf + tag_pos;
  if (fdt_check_header(fdt_ptr) != 0) {
    fprintf(stderr, "Bad DTB header.\n");
    goto end;
  }

  // Check FDT size and actual size
  size_t fdt_size = fdt_totalsize(fdt_ptr);
  size_t fdt_actual_size = buf_size - tag_pos;
  int fdt_size_diff = (int)fdt_actual_size - (int)fdt_size;
  if (fdt_size_diff) {
    fprintf(stderr, "Wrong size: actual size = %d FDT size = %d(%d)\n",
      fdt_actual_size, fdt_size, fdt_size_diff);
  }

  // Print the DT basic information
  int root_node_off = fdt_path_offset(fdt_ptr, "/");
  if (root_node_off < 0) {
    fprintf(stderr, "Can not get the root node.\n");
    goto end;
  }
  printf("Output %s\n", filename);
  const char *model =
    (const char *)fdt_getprop(fdt_ptr, root_node_off, "model", NULL);
  printf("  model=%s\n", model ? model : "(unknown)");
  const char *compatible =
    (const char *)fdt_getprop(fdt_ptr, root_node_off, "compatible", NULL);
  printf("  compatible=%s\n", compatible ? compatible : "(unknown)");

  // Output DTB file
  if (write_fdt_to_file(filename, fdt_ptr) != 0) {
    fprintf(stderr, "Write file error: %s\n", filename);
    goto end;
  }

end:
  return tag_pos;
}

int extract_dtbs(const char *in_filename,
                 const char *out_dtb_filename,
                 const char *out_image_filename) {
  int ret = 1;
  char *buf = NULL;

  size_t buf_size;
  buf = load_file(in_filename, &buf_size);
  if (!buf) {
    fprintf(stderr, "Can not load file: %s\n", in_filename);
    goto end;
  }

  char filename[128];
  int index = 0;
  while (buf_size > 0) {
    int tag_pos;
    if (index > 0) {
      snprintf(filename, sizeof(filename), "%s.%d", out_dtb_filename, index);
      tag_pos = find_and_write_dtb(filename, buf, buf_size);
    } else {
      tag_pos = find_and_write_dtb(out_dtb_filename, buf, buf_size);
    }
    if (tag_pos < 0) {
      break;
    }

    buf_size = tag_pos;
    index++;
  }

  if (index == 0) {
    fprintf(stderr, "Can not find any DTB.\n");
    goto end;
  }

  // Output the rest buffer as image file
  if (out_image_filename != NULL) {
    printf("Output %s\n", out_image_filename);
    if (write_buf_to_file(out_image_filename, buf, buf_size) != 0) {
      fprintf(stderr, "Write file error: %s\n", out_image_filename);
      goto end;
    }
  }

  ret = 0;

end:
  if (buf) dto_free(buf);

  return ret;
}

int main(int argc, char **argv) {
  if (argc < 3) {
    fprintf(stderr, "Usage: %s <image.gz-dtb_file> <out_dtb_name> (<out_image_name>)\n\n", argv[0]);
    fprintf(stderr, " Note: If there are more than one DTB, it outputs DTB files named\n" \
                    "       out_dtb_name, out_dtb_name.1, out_dtb_name.2, etc.\n");
    return 1;
  }

  const char *in_file = argv[1];
  const char *out_dtb_file = argv[2];
  const char *out_image_file = (argc > 3) ? argv[3] : NULL;
  int ret = extract_dtbs(in_file, out_dtb_file, out_image_file);

  return ret;
}
