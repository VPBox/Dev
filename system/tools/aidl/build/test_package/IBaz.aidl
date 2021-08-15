/*
 * Copyright (C) 2018 The Android Open Source Project
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

package test_package;

import some_package.IBar;

interface IBaz {
    oneway void CanYouDealWithThisBar(in IBar bar);
    void MethodAddedInVersion2();

    ParcelFileDescriptor readPFD();
    void writePFD(in ParcelFileDescriptor fd);
    void readWritePFD(inout ParcelFileDescriptor fd);

    // TODO(b/112664205) uncomment these when we have the support for array type in
    // the ndk backend
    //ParcelFileDescriptor[] readPFDArray();
    //void writePFDArray(in ParcelFileDescriptor[] fds);
    //void readWritePFDArray(inout ParcelFileDescriptor[] fds);
}
