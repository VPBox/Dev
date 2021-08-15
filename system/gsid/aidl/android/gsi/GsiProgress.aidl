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

package android.gsi;

/** {@hide} */
parcelable GsiProgress {
    /* String containing which step of the installation is in progress. */
    @utf8InCpp String step;
    /* Status code (see constants in IGsiService.aidl) */
    int status;
    /* Number of bytes processed */
    long bytes_processed;
    /* Total number of bytes to be processed */
    long total_bytes;
}
