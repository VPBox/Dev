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
#pragma rs java_package_name(com.example.android.rs.vr.engine)
#pragma rs_fp_relaxed

int brick_dimx;
int brick_dimy;
int brick_dimz;
rs_allocation volume;
rs_allocation opacity;
int z_offset;
// output a single bit per pixel volume based on opacity

uint __attribute__((kernel)) pack_chunk(uint32_t x) {

    int brick = x / (32 * 32);
    int bx = brick % brick_dimx;
    int yz = brick / brick_dimx;
    int by = yz % brick_dimy;
    int bz = yz / brick_dimy;

    int in_brick = x % (32 * 32);
    int in_br_y = in_brick % 32;
    int in_br_z = in_brick / 32;
    int pz = (bz << 5) | in_br_z;


     int py = (by << 5) | in_br_y;

    uint out = 0;

    if (pz >= rsAllocationGetDimZ(volume)) {
         return out;
    }
    if (py >= rsAllocationGetDimY(volume)) {
              return out;
    }
    for (int in_br_x = 0; in_br_x < 32; in_br_x++) {
        int px = (bx << 5) | in_br_x;


        int intensity = 0xFFFF & rsGetElementAt_short(volume, px, py, pz);
        uchar op = rsGetElementAt_uchar(opacity, intensity);
        uint v = (op > 0) ? 1 : 0;
        out |= v << in_br_x;
    }
    return out;
}
rs_allocation bricks; // input bricks

uint __attribute__((kernel)) dilate(uint in, uint32_t x) {

    int BRICK_SIZE = 32 * 32;
    int brick = x / (BRICK_SIZE);
    int bx = brick % brick_dimx;
    int yz = brick / brick_dimx;
    int by = yz % brick_dimy;
    int bz = yz / brick_dimy;

    int in_brick = x % (BRICK_SIZE);
    int in_br_y = in_brick % 32;
    int in_br_z = in_brick / 32;
    uint slice;
    uint out = in;
    out |= in >> 1;
    out |= in << 1;
    int base_brick = bx + (by + brick_dimy * bz) * brick_dimx;
    int base_offset = (in_br_z) * 32 + (in_br_y);

      int slice_pos = base_brick * BRICK_SIZE + base_offset;

      // +/- in x
      if (bx > 0) {
        slice =   0x80000000 & rsGetElementAt_uint(bricks, slice_pos - BRICK_SIZE);
        out |= slice >> 31;
      }
      if (bx < brick_dimx - 1) {
        slice =  1  & rsGetElementAt_uint(bricks, slice_pos + BRICK_SIZE);
         out |= slice << 31;
      }

      // - in Y
      int off_neg_y = -1; // simple case -1 slice;
      if (in_br_y == 0) { // att the edge in brick go to y-1 brick
        if (by > 0) { // edge of screen
          off_neg_y = 31 - BRICK_SIZE * brick_dimx;
        } else {// edge of volume
          off_neg_y = 0;
        }
      }

      slice = rsGetElementAt_uint(bricks, slice_pos + off_neg_y);
      out |= slice;
      // + in Y
      int off_pos_y = 1;
      if (in_br_y == 31) {
        if (by < brick_dimy - 1) {
          off_pos_y = BRICK_SIZE * brick_dimx - 31;
        } else {
          off_pos_y = 0;
        }
      }
      slice = rsGetElementAt_uint(bricks, slice_pos + off_pos_y);
      out |= slice;
      int off_neg_z = -32;
      if (in_br_z == 0) {
        if (bz > 0) { // edge of screen
          off_neg_z = 31*32 - brick_dimx * brick_dimy* BRICK_SIZE;
        } else {
          off_neg_z = 0;
        }
      }
      slice = rsGetElementAt_uint(bricks, slice_pos + off_neg_z);
      out |= slice;
      int off_pos_z = 32;
      if (in_br_z == 31) {
        if (bz < brick_dimz - 1) {
          off_pos_z = brick_dimx * brick_dimy * BRICK_SIZE - 31*32;
        } else {
          off_pos_z = 0;
        }
      }
      slice = rsGetElementAt_uint(bricks, slice_pos + off_pos_z);
      out |= slice;

    return out;



}
int z;

void __attribute__((kernel)) copy(short in, uint32_t x, uint32_t y) {
    rsSetElementAt_short(volume, in, x, y, z);
}
