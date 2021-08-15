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

#include "chre_host/socket_client.h"

#include <inttypes.h>

#include <string.h>
#include <unistd.h>

#include <chrono>

#include <cutils/sockets.h>
#include <sys/socket.h>
#include <utils/RefBase.h>
#include <utils/StrongPointer.h>

#include "chre_host/log.h"

namespace android {
namespace chre {

SocketClient::SocketClient() {
  std::atomic_init(&mSockFd, INVALID_SOCKET);
}

SocketClient::~SocketClient() {
  disconnect();
}

bool SocketClient::connect(const char *socketName,
                           const sp<ICallbacks>& callbacks) {
  return doConnect(socketName, callbacks, false /* connectInBackground */);
}

bool SocketClient::connectInBackground(const char *socketName,
                                       const sp<ICallbacks>& callbacks) {
  return doConnect(socketName, callbacks, true /* connectInBackground */);
}

void SocketClient::disconnect() {
  if (inReceiveThread()) {
    LOGE("disconnect() can't be called from a receive thread callback");
  } else if (receiveThreadRunning()) {
    // Inform the RX thread that we're requesting a shutdown, breaking it out of
    // the retry wait if it's currently blocked there
    {
      std::lock_guard<std::mutex> lock(mShutdownMutex);
      mGracefulShutdown = true;
    }
    mShutdownCond.notify_all();

    // Invalidate the socket (will kick the RX thread out of recv if it's
    // currently blocked there)
    if (mSockFd != INVALID_SOCKET && shutdown(mSockFd, SHUT_RDWR) != 0) {
      LOG_ERROR("Couldn't shut down socket", errno);
    }

    if (mRxThread.joinable()) {
      LOGD("Waiting for RX thread to exit");
      mRxThread.join();
    }
  }
}

bool SocketClient::isConnected() const {
  return (mSockFd != INVALID_SOCKET);
}

bool SocketClient::sendMessage(const void *data, size_t length) {
  bool success = false;

  if (mSockFd == INVALID_SOCKET) {
    LOGW("Tried sending a message, but don't have a valid socket handle");
  } else {
    ssize_t bytesSent = send(mSockFd, data, length, 0);
    if (bytesSent < 0) {
      LOGE("Failed to send %zu bytes of data: %s", length, strerror(errno));
    } else if (bytesSent == 0) {
      LOGW("Failed to send data; remote side disconnected");
    } else if (static_cast<size_t>(bytesSent) != length) {
      LOGW("Truncated packet, tried sending %zu bytes, only %zd went through",
           length, bytesSent);
    } else {
      success = true;
    }
  }

  return success;
}

bool SocketClient::doConnect(const char *socketName,
                             const sp<ICallbacks>& callbacks,
                             bool connectInBackground) {
  bool success = false;
  if (inReceiveThread()) {
    LOGE("Can't attempt to connect from a receive thread callback");
  } else {
    if (receiveThreadRunning()) {
      LOGW("Re-connecting socket with implicit disconnect");
      disconnect();
    }

    size_t socketNameLen = strlcpy(mSocketName, socketName,
                                   sizeof(mSocketName));
    if (socketNameLen >= sizeof(mSocketName)) {
      LOGE("Socket name length parameter is too long (%zu, max %zu)",
           socketNameLen, sizeof(mSocketName));
    } else if (callbacks == nullptr) {
      LOGE("Callbacks parameter must be provided");
    } else if (connectInBackground || tryConnect()) {
      mGracefulShutdown = false;
      mCallbacks = callbacks;
      mRxThread = std::thread([this]() {
        receiveThread();
      });
      success = true;
    }
  }

  return success;
}

bool SocketClient::inReceiveThread() const {
  return (std::this_thread::get_id() == mRxThread.get_id());
}

void SocketClient::receiveThread() {
  constexpr size_t kReceiveBufferSize = 4096;
  uint8_t buffer[kReceiveBufferSize];

  LOGV("Receive thread started");
  while (!mGracefulShutdown && (mSockFd != INVALID_SOCKET || reconnect())) {
    while (!mGracefulShutdown) {
      ssize_t bytesReceived = recv(mSockFd, buffer, sizeof(buffer), 0);
      if (bytesReceived < 0) {
        LOG_ERROR("Exiting RX thread", errno);
        break;
      } else if (bytesReceived == 0) {
        if (!mGracefulShutdown) {
          LOGI("Socket disconnected on remote end");
          mCallbacks->onDisconnected();
        }
        break;
      }

      mCallbacks->onMessageReceived(buffer, bytesReceived);
    }

    if (close(mSockFd) != 0) {
      LOG_ERROR("Couldn't close socket", errno);
    }
    mSockFd = INVALID_SOCKET;
  }

  if (!mGracefulShutdown) {
    mCallbacks->onConnectionAborted();
  }

  mCallbacks.clear();
  LOGV("Exiting receive thread");
}

bool SocketClient::receiveThreadRunning() const {
  return mRxThread.joinable();
}

bool SocketClient::reconnect() {
  constexpr auto kMinDelay = std::chrono::duration<int32_t, std::milli>(250);
  constexpr auto kMaxDelay = std::chrono::minutes(5);
  // Try reconnecting at initial delay this many times before backing off
  constexpr unsigned int kExponentialBackoffDelay =
    std::chrono::seconds(10) / kMinDelay;
  // Give up after this many tries (~2.5 hours)
  constexpr unsigned int kRetryLimit = kExponentialBackoffDelay + 40;
  auto delay = kMinDelay;
  unsigned int retryCount = 0;

  while (retryCount++ < kRetryLimit) {
    {
      std::unique_lock<std::mutex> lock(mShutdownMutex);
      mShutdownCond.wait_for(lock, delay,
                             [this]() { return mGracefulShutdown.load(); });
      if (mGracefulShutdown) {
        break;
      }
    }

    bool suppressErrorLogs = (delay == kMinDelay);
    if (!tryConnect(suppressErrorLogs)) {
      if (!suppressErrorLogs) {
        LOGW("Failed to (re)connect, next try in %" PRId32 " ms",
             delay.count());
      }
      if (retryCount > kExponentialBackoffDelay) {
        delay *= 2;
      }
      if (delay > kMaxDelay) {
        delay = kMaxDelay;
      }
    } else {
      LOGD("Successfully (re)connected");
      mCallbacks->onConnected();
      return true;
    }
  }

  return false;
}

bool SocketClient::tryConnect(bool suppressErrorLogs) {
  bool success = false;

  errno = 0;
  int sockFd = socket(AF_LOCAL, SOCK_SEQPACKET, 0);
  if (sockFd >= 0) {
    // Set the send buffer size to 2MB to allow plenty of room for nanoapp
    // loading
    int sndbuf = 2 * 1024 * 1024;
    int ret = setsockopt(
        sockFd, SOL_SOCKET, SO_SNDBUF, &sndbuf, sizeof(sndbuf));
    if (ret == 0) {
      mSockFd = socket_local_client_connect(
          sockFd, mSocketName, ANDROID_SOCKET_NAMESPACE_RESERVED,
          SOCK_SEQPACKET);
      if (mSockFd != INVALID_SOCKET) {
        success = true;
      } else if (!suppressErrorLogs) {
        LOGE("Couldn't connect client socket to '%s': %s",
             mSocketName, strerror(errno));
      }
    } else if (!suppressErrorLogs) {
      LOGE("Failed to set SO_SNDBUF to %d: %s", sndbuf, strerror(errno));
    }

    if (!success) {
      close(sockFd);
    }
  } else if (!suppressErrorLogs) {
    LOGE("Couldn't create local socket: %s", strerror(errno));
  }

  return success;
}

}  // namespace chre
}  // namespace android
