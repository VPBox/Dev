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
parcelable GsiInstallParams {
    /**
     * The directory to install GSI images under. This must be either an empty
     * string (which will use the default /data/gsi), "/data/gsi", or a mount
     * under /mnt/media_rw. It may end in a trailing slash.
     */
    @utf8InCpp String installDir;

    /* The size of the on-disk GSI image. */
    long gsiSize;

    /* The desired size of the userdata partition. */
    long userdataSize;

    /* If false, a userdata image is only created if one does not already
     * exist. If the size is zero, a default size of 8GiB is used. If there is
     * an existing image smaller than the desired size, it may be resized
     * automatically.
     */
    boolean wipeUserdata;
}

