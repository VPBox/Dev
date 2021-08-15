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

#include "Network.h"

#define LOG_TAG "Netd"
#include "log/log.h"

#include <android-base/strings.h>
#include <sstream>

namespace android {
namespace net {

Network::~Network() {
    if (!mInterfaces.empty()) {
        ALOGE("deleting network with netId %u without clearing its interfaces", mNetId);
    }
}

unsigned Network::getNetId() const {
    return mNetId;
}

bool Network::hasInterface(const std::string& interface) const {
    return mInterfaces.find(interface) != mInterfaces.end();
}

const std::set<std::string>& Network::getInterfaces() const {
    return mInterfaces;
}

int Network::clearInterfaces() {
    while (!mInterfaces.empty()) {
        // Make a copy of the string, so removeInterface() doesn't lose its parameter when it
        // removes the string from the set.
        std::string interface = *mInterfaces.begin();
        if (int ret = removeInterface(interface)) {
            return ret;
        }
    }
    return 0;
}

std::string Network::toString() const {
    const char kSeparator[] = " ";
    std::stringstream repr;

    repr << mNetId;

    repr << kSeparator;
    switch (getType()) {
        case DUMMY:
            repr << "DUMMY";
            break;
        case LOCAL:
            repr << "LOCAL";
            break;
        case PHYSICAL:
            repr << "PHYSICAL";
            break;
        case VIRTUAL:
            repr << "VIRTUAL";
            break;
        default:
            repr << "unknown";
    }

    if (mInterfaces.size() > 0) {
        repr << kSeparator << android::base::Join(mInterfaces, ",");
    }

    return repr.str();
}


Network::Network(unsigned netId) : mNetId(netId) {
}

}  // namespace net
}  // namespace android
