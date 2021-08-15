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

#ifndef _RESOLVER_STATS_H_
#define _RESOLVER_STATS_H_

#include <time.h>

namespace android {
namespace net {

struct ResolverStats {
    // Offsets into the per-server resolver stats as encoded in vector<int32_t> stats of
    // getResolverInfo() of Netd's binder interface. The stats are based on data reported by
    // android_net_res_stats_get_info_for_net(), the usability is calculated by applying
    // android_net_res_stats_get_usable_servers() to this data.
    enum ResolverStatsOffsets {
        STATS_SUCCESSES = 0,     // # successes counted for this server
        STATS_ERRORS,            // # errors
        STATS_TIMEOUTS,          // # timeouts
        STATS_INTERNAL_ERRORS,   // # internal errors
        STATS_RTT_AVG,           // average round-trip-time
        STATS_LAST_SAMPLE_TIME,  // time in s when the last sample was recorded
        STATS_USABLE,            // whether the server is considered usable
        STATS_COUNT              // total count of integers in the per-server data
    };

    int successes{-1};
    int errors{-1};
    int timeouts{-1};
    int internal_errors{-1};
    int rtt_avg{-1};
    time_t last_sample_time{0};
    bool usable{false};

    // Serialize the resolver stats to the end of |out|.
    void encode(std::vector<int32_t>* out) const;

    // Read the serialized resolverstats starting at |in[ofs]|.
    ssize_t decode(const std::vector<int32_t>& in, ssize_t ofs);

    // Serialize the contents of |stats| and append them to the end of |out|. Multiple arrays
    // can be written to the same output vector in sequence, however, the corresponding call
    // to decodeAll() will return the combined contents in one vector.
    static void encodeAll(const std::vector<ResolverStats>& stats, std::vector<int32_t>* out);

    // Decodes the serialized ResolverStats from |in| and appends them to stats.
    static bool decodeAll(const std::vector<int32_t>& in, std::vector<ResolverStats>* stats);
};

inline void ResolverStats::encode(std::vector<int32_t>* out) const {
    size_t ofs = out->size();
    out->resize(ofs + STATS_COUNT);
    int32_t* cur = &(*out)[ofs];
    cur[STATS_SUCCESSES] = successes;
    cur[STATS_ERRORS] = errors;
    cur[STATS_TIMEOUTS] = timeouts;
    cur[STATS_INTERNAL_ERRORS] = internal_errors;
    cur[STATS_RTT_AVG] = rtt_avg;
    cur[STATS_LAST_SAMPLE_TIME] = last_sample_time;
    cur[STATS_USABLE] = usable;
}

// Read the serialized resolverstats starting at |in[ofs]|.
inline ssize_t ResolverStats::decode(const std::vector<int32_t>& in, ssize_t ofs) {
    if (ofs < 0 || static_cast<size_t>(ofs) + STATS_COUNT > in.size()) {
        return -1;
    }
    const int32_t* cur = &in[ofs];
    successes = cur[STATS_SUCCESSES];
    errors = cur[STATS_ERRORS];
    timeouts = cur[STATS_TIMEOUTS];
    internal_errors = cur[STATS_INTERNAL_ERRORS];
    rtt_avg = cur[STATS_RTT_AVG];
    last_sample_time = cur[STATS_LAST_SAMPLE_TIME];
    usable = cur[STATS_USABLE];
    return ofs + STATS_COUNT;
}

inline void ResolverStats::encodeAll(const std::vector<ResolverStats>& stats,
                                     std::vector<int32_t>* out) {
    for (const auto& s : stats) {
        s.encode(out);
    }
}

// TODO: Replace with a better representation, e.g. a Parcelable.
inline bool ResolverStats::decodeAll(const std::vector<int32_t>& in,
                                     std::vector<ResolverStats>* stats) {
    ssize_t size = in.size();
    if (size % STATS_COUNT) {
        return false;
    }
    stats->resize(size / STATS_COUNT);
    ssize_t ofs = 0;
    for (auto& s : *stats) {
        ofs = s.decode(in, ofs);
        if (ofs < 0) {
            return false;
        }
    }
    return true;
}

}  // namespace net
}  // namespace android

#endif /* _RESOLVER_STATS_H_ */
