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

#ifndef CHRE_PLATFORM_ANDROID_HOST_LINK_H_
#define CHRE_PLATFORM_ANDROID_HOST_LINK_H_

#include <cstddef>

#include "chre_host/socket_server.h"
#include "chre/util/singleton.h"

namespace chre {

//! A convenience typedef to refer to the socket server singleton instance.
typedef chre::Singleton<::android::chre::SocketServer> SocketServerSingleton;

/**
 * Provides a data payload to the Android implementation of the host link
 * protocol to be parsed and handled.
 *
 * @param message a pointer to message data.
 * @param length the size of the data buffer.
 * @return true if the message was parsed successfully, false otherwise.
 */
bool handleMessageFromHost(void *message, size_t length);

}  // namespace chre

#endif  // CHRE_PLATFORM_ANDROID_HOST_LINK_H_
