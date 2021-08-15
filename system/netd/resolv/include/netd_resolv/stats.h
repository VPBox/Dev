/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef NETD_RES_STATS_H
#define NETD_RES_STATS_H

#include <stdbool.h>
#include <stdint.h>
#include <sys/socket.h>
#include <time.h>

#include "params.h"

#define RCODE_INTERNAL_ERROR 254
#define RCODE_TIMEOUT 255

struct res_sample {
    time_t at;      // time in s at which the sample was recorded
    uint16_t rtt;   // round-trip time in ms
    uint8_t rcode;  // the DNS rcode or RCODE_XXX defined above
};

// Resolver reachability statistics and run-time parameters.
struct res_stats {
    // Stats of the last <sample_count> queries.
    res_sample samples[MAXNSSAMPLES];
    // The number of samples stored.
    uint8_t sample_count;
    // The next sample to modify.
    uint8_t sample_next;
};

// Aggregates the reachability statistics for the given server based on on the stored samples.
LIBNETD_RESOLV_PUBLIC void android_net_res_stats_aggregate(res_stats* stats, int* successes,
                                                           int* errors, int* timeouts,
                                                           int* internal_errors, int* rtt_avg,
                                                           time_t* last_sample_time);

LIBNETD_RESOLV_PUBLIC int android_net_res_stats_get_info_for_net(
        unsigned netid, int* nscount, sockaddr_storage servers[MAXNS], int* dcount,
        char domains[MAXDNSRCH][MAXDNSRCHPATH], res_params* params, res_stats stats[MAXNS],
        int* wait_for_pending_req_timeout_count);

// Returns an array of bools indicating which servers are considered good
LIBNETD_RESOLV_PUBLIC int android_net_res_stats_get_usable_servers(const res_params* params,
                                                                   res_stats stats[], int nscount,
                                                                   bool valid_servers[]);

#endif  // NETD_RES_STATS_H
