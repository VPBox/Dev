/*
 * Copyright (C) 2014 The Android Open Source Project
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

#include "UidRanges.h"

#include "NetdConstants.h"

#include <inttypes.h>
#include <limits.h>
#include <stdlib.h>

#include <android-base/stringprintf.h>
#include <log/log.h>

using android::base::StringAppendF;

namespace android {
namespace net {

namespace {

bool compUidRangeParcel(const UidRangeParcel& lhs, const UidRangeParcel& rhs) {
    return lhs.start != rhs.start ? (lhs.start < rhs.start) : (lhs.stop < rhs.stop);
};

UidRangeParcel makeUidRangeParcel(int start, int stop) {
    UidRangeParcel res;
    res.start = start;
    res.stop = stop;

    return res;
}

uint32_t length(const UidRangeParcel& t) {
    if (t.start == -1 || t.stop == -1) {
        return 0;
    }
    return static_cast<uint32_t>(t.stop - t.start + 1);
}

}  // namespace

bool UidRanges::hasUid(uid_t uid) const {
    if (uid > (unsigned) INT32_MAX) {
        ALOGW("UID larger than 32 bits: %" PRIu64, static_cast<uint64_t>(uid));
        return false;
    }
    const int32_t intUid = static_cast<int32_t>(uid);

    auto iter = std::lower_bound(mRanges.begin(), mRanges.end(), makeUidRangeParcel(intUid, intUid),
                                 compUidRangeParcel);
    return (iter != mRanges.end() && iter->start == intUid) ||
           (iter != mRanges.begin() && (--iter)->stop >= intUid);
}

const std::vector<UidRangeParcel>& UidRanges::getRanges() const {
    return mRanges;
}

bool UidRanges::parseFrom(int argc, char* argv[]) {
    mRanges.clear();
    for (int i = 0; i < argc; ++i) {
        if (!*argv[i]) {
            // The UID string is empty.
            return false;
        }
        char* endPtr;
        uid_t uidStart = strtoul(argv[i], &endPtr, 0);
        uid_t uidEnd;
        if (!*endPtr) {
            // Found a single UID. The range contains just the one UID.
            uidEnd = uidStart;
        } else if (*endPtr == '-') {
            if (!*++endPtr) {
                // Unexpected end of string.
                return false;
            }
            uidEnd = strtoul(endPtr, &endPtr, 0);
            if (*endPtr) {
                // Illegal trailing chars.
                return false;
            }
            if (uidEnd < uidStart) {
                // Invalid order.
                return false;
            }
        } else {
            // Not a single uid, not a range. Found some other illegal char.
            return false;
        }
        if (uidStart == INVALID_UID || uidEnd == INVALID_UID) {
            // Invalid UIDs.
            return false;
        }
        mRanges.push_back(makeUidRangeParcel(uidStart, uidEnd));
    }
    std::sort(mRanges.begin(), mRanges.end(), compUidRangeParcel);
    return true;
}

UidRanges::UidRanges(const std::vector<UidRangeParcel>& ranges) {
    mRanges = ranges;
    std::sort(mRanges.begin(), mRanges.end(), compUidRangeParcel);
}

void UidRanges::add(const UidRanges& other) {
    auto middle = mRanges.insert(mRanges.end(), other.mRanges.begin(), other.mRanges.end());
    std::inplace_merge(mRanges.begin(), middle, mRanges.end(), compUidRangeParcel);
}

void UidRanges::remove(const UidRanges& other) {
    auto end = std::set_difference(mRanges.begin(), mRanges.end(), other.mRanges.begin(),
                                   other.mRanges.end(), mRanges.begin(), compUidRangeParcel);
    mRanges.erase(end, mRanges.end());
}

std::string UidRanges::toString() const {
    std::string s("UidRanges{ ");
    for (const auto &range : mRanges) {
        if (length(range) == 0) {
            StringAppendF(&s, "<BAD: %u-%u> ", range.start, range.stop);
        } else if (length(range) == 1) {
            StringAppendF(&s, "%u ", range.start);
        } else {
            StringAppendF(&s, "%u-%u ", range.start, range.stop);
        }
    }
    StringAppendF(&s, "}");
    return s;
}

}  // namespace net
}  // namespace android
