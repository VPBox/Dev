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

package android.net;

/**
 * Configuration details for a network interface.
 *
 * {@hide}
 */
parcelable InterfaceConfigurationParcel {
    @utf8InCpp String ifName;
    @utf8InCpp String hwAddr;
    @utf8InCpp String ipv4Addr;
    int prefixLength;
    /**
    * Interface flags, String versions of IFF_* defined in netd/if.h
    */
    @utf8InCpp String[] flags;
}
