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

#ifndef _NETD_PERMISSIONS_H
#define _NETD_PERMISSIONS_H

inline constexpr char PERM_CONNECTIVITY_INTERNAL[] = "android.permission.CONNECTIVITY_INTERNAL";
inline constexpr char PERM_NETWORK_STACK[] = "android.permission.NETWORK_STACK";
inline constexpr char PERM_MAINLINE_NETWORK_STACK[] = "android.permission.MAINLINE_NETWORK_STACK";
inline constexpr char PERM_DUMP[] = "android.permission.DUMP";
inline constexpr char PERM_CONNECTIVITY_USE_RESTRICTED_NETWORKS[] =
        "android.permission.CONNECTIVITY_USE_RESTRICTED_NETWORKS";
inline constexpr char PERM_NETWORK_BYPASS_PRIVATE_DNS[] =
        "android.permission.NETWORK_BYPASS_PRIVATE_DNS";

#endif  // _NETD_PERMISSIONS_H
