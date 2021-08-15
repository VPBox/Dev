/*
 * Copyright (C) 2017 The Android Open Source Project
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

#ifndef SIMPLE_PERF_INPLACE_SAMPLER_LIB_H_
#define SIMPLE_PERF_INPLACE_SAMPLER_LIB_H_

enum InplaceSamplerMessageType {
  START_PROFILING,
  START_PROFILING_REPLY,
  THREAD_INFO,
  MAP_INFO,
  SAMPLE_INFO,
  END_PROFILING,
  END_PROFILING_REPLY,
};

// Type: START_PROFILING
// Direction: simpleperf to inplace_sampler
// Data:
//   char options[];  // ended by '\0'
//
// options[] contains space separated options like below:
//   freq=4000 # sample at 4000/s.
//   signal=14  # use signal 14 to raise sample recording.
//   tids=1432,1433  # take samples of thread 1432,1433.


// Type: START_PROFILING_REPLY
// Direction: inplace_sampler to simpleperf
// Data:
//   char reply[]; // ended by '\0'
// reply[] contains a string, which is either 'ok' or 'error'

// Type: THREAD_INFO
// Direction: inplace_sampler to simpleperf
// Data:
//  uint64_t time;
//  uint32_t tid;
//  char thread_name[];  // ended by '\0'

// Type: MAP_INFO
// Direction: inplace_sampler to simpleperf
// Data:
//  uint64_t time;
//  uint64_t start;
//  uint64_t len;
//  uint64_t pgoff;
//  char[] dso;  // ended by '\0'

// Type: SAMPLE_INFO
// Direction: inplace_sampler to simpleperf
// Data:
//  uint64_t time;
//  uint32_t tid;
//  uint32_t period;
//  uint32_t ip_nr;
//  uint64_t ip[ip_nr];

// Type: END_PROFILING
// Direction: simpleperf to inplace_sampler
// Data:
//   None.

// Type: END_PROFILING_REPLY
// Direction: inplace_sampler to simpleperf
// Data:
//   uint64_t used_cpu_time;
//   uint64_t lost_samples;


#endif  // SIMPLE_PERF_INPLACE_SAMPLER_LIB_H_
