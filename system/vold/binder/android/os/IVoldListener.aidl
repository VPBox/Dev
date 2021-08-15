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

package android.os;

/** {@hide} */
oneway interface IVoldListener {
    void onDiskCreated(@utf8InCpp String diskId, int flags);
    void onDiskScanned(@utf8InCpp String diskId);
    void onDiskMetadataChanged(@utf8InCpp String diskId,
            long sizeBytes, @utf8InCpp String label, @utf8InCpp String sysPath);
    void onDiskDestroyed(@utf8InCpp String diskId);

    void onVolumeCreated(@utf8InCpp String volId,
            int type, @utf8InCpp String diskId, @utf8InCpp String partGuid);
    void onVolumeStateChanged(@utf8InCpp String volId, int state);
    void onVolumeMetadataChanged(@utf8InCpp String volId,
            @utf8InCpp String fsType, @utf8InCpp String fsUuid, @utf8InCpp String fsLabel);
    void onVolumePathChanged(@utf8InCpp String volId,
            @utf8InCpp String path);
    void onVolumeInternalPathChanged(@utf8InCpp String volId,
            @utf8InCpp String internalPath);
    void onVolumeDestroyed(@utf8InCpp String volId);
}
