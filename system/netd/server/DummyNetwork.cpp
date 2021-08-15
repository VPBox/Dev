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

#include "DummyNetwork.h"

#include "RouteController.h"

#define LOG_TAG "Netd"
#include "log/log.h"
#include "errno.h"

namespace android {
namespace net {

const char* DummyNetwork::INTERFACE_NAME = "dummy0";

DummyNetwork::DummyNetwork(unsigned netId) : Network(netId) {
    mInterfaces.insert(INTERFACE_NAME);
}

DummyNetwork::~DummyNetwork() {
}

Network::Type DummyNetwork::getType() const {
    return DUMMY;
}

int DummyNetwork::addInterface(const std::string& /* interface */) {
    return -EINVAL;
}

int DummyNetwork::removeInterface(const std::string& /* interface */) {
    return -EINVAL;
}

}  // namespace net
}  // namespace android
