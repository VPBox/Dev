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

#pragma once

#include "netd_resolv/resolv.h"  // struct android_net_context
#include "stats.pb.h"

// Query dns with raw msg
int resolv_res_nsend(const android_net_context* netContext, const uint8_t* msg, int msgLen,
                     uint8_t* ans, int ansLen, int* rcode, uint32_t flags,
                     android::net::NetworkDnsEventReported* event);
