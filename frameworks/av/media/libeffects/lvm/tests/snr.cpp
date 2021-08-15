/*
 * Copyright (C) 2019 The Android Open Source Project
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
#include <assert.h>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

template <typename T, typename A = float>
std::pair<A, A> getSignalNoise(FILE *finp, FILE *fref) {
  constexpr size_t framesize = 256;
  std::vector<T> in(framesize);
  std::vector<T> ref(framesize);
  A signal{};
  A noise{};

  for (;;) {
    size_t read_samples_in = fread(&in[0], sizeof(T), framesize, finp);
    const size_t read_samples_ref = fread(&ref[0], sizeof(T), framesize, fref);
    if (read_samples_in != read_samples_ref) {
      printf("file sizes do not match (last %zu %zu)", read_samples_in, read_samples_ref);
      read_samples_in = std::min(read_samples_in, read_samples_ref);
    }
    if (read_samples_in == 0) {
        return { signal, noise };
    }
    for (size_t i = 0; i < read_samples_in; ++i) {
       const A value(ref[i]);
       const A diff(A(in[i]) - value);
       signal += value * value;
       noise += diff * diff;
    }
  }
}

void printUsage() {
  printf("\nUsage: ");
  printf("\n     snr <ref_file> <test_file> [options]\n");
  printf("\nwhere, \n     <ref_file>  is the reference file name");
  printf("\n                  on which will be taken as pure signal");
  printf("\n     <test_file> is test file for snr calculation");
  printf("\n     and options are mentioned below");
  printf("\n");
  printf("\n     -pcm_format:<pcm format of input files>");
  printf("\n           0 - 16 bit pcm");
  printf("\n           1 - 32 bit float");
  printf("\n           default 0");
  printf("\n     -thr:<threshold value>");
  printf("\n           default - negative infinity\n\n");
}

int main(int argc, const char *argv[]) {
  if (argc < 3) {
    printUsage();
    return -1;
  }
  int pcm_format = 0;
  float thr = - std::numeric_limits<float>::infinity();
  FILE *fref = fopen(argv[1], "rb");
  FILE *finp = fopen(argv[2], "rb");
  for (int i = 3; i < argc; i++) {
    if (!strncmp(argv[i], "-pcm_format:", 12)) {
      pcm_format = atoi(argv[i] + 12);
    } else if (!strncmp(argv[i], "-thr:", 5)) {
      thr = atof(argv[i] + 5);
    }
  }
  if (finp == nullptr || fref == nullptr) {
    printf("\nError: missing input/reference files\n");
    return -1;
  }
  int ret = EXIT_SUCCESS;
  auto sn = pcm_format == 0
      ? getSignalNoise<short>(finp, fref)
      : getSignalNoise<float>(finp, fref);
  if (sn.first > 0.f && sn.second > 0.f) {
    float snr = 10.f * log(sn.first / sn.second);
    // compare the measured snr value with threshold
    if (snr < thr) {
      printf("%.6f less than threshold %.6f\n", snr, thr);
      ret = EXIT_FAILURE;
    } else {
      printf("%.6f\n", snr);
    }
  }
  fclose(finp);
  fclose(fref);

  return ret;
}
