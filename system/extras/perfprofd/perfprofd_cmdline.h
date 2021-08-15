/*
 *
 * Copyright 2015, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SYSTEM_EXTRAS_PERFPROFD_PERFPROFD_CMDLINE_H_
#define SYSTEM_EXTRAS_PERFPROFD_PERFPROFD_CMDLINE_H_

// Semaphore file that indicates that the user is opting in
#define SEMAPHORE_FILENAME "perf_profile_collection_enabled.txt"

// File containing a list of sequence numbers corresponding to profiles
// that have been processed/uploaded. Written by the GmsCore uploader,
// within the GmsCore files directory.
#define PROCESSED_FILENAME "perfprofd_processed.txt"

// File containing a list of sequence numbers corresponding to profiles
// that have been created by the perfprofd but not yet uploaded. Written
// by perfprofd within the destination directory; consumed by GmsCore.
#define PRODUCED_FILENAME "perfprofd_produced.txt"

struct Config;

// Main routine for perfprofd daemon
int perfprofd_main(int argc, char **argv, Config* config);

#endif  // SYSTEM_EXTRAS_PERFPROFD_PERFPROFD_CMDLINE_H_
