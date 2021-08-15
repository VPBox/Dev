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

#include "chre_host/socket_server.h"

#include <poll.h>

#include <cassert>
#include <cinttypes>
#include <csignal>
#include <cstdlib>
#include <map>
#include <mutex>

#include <cutils/sockets.h>

#include "chre_host/log.h"

namespace android {
namespace chre {

std::atomic<bool> SocketServer::sSignalReceived(false);

namespace {

void maskAllSignals() {
  sigset_t signalMask;
  sigfillset(&signalMask);
  if (sigprocmask(SIG_SETMASK, &signalMask, NULL) != 0) {
    LOG_ERROR("Couldn't mask all signals", errno);
  }
}

void maskAllSignalsExceptIntAndTerm() {
  sigset_t signalMask;
  sigfillset(&signalMask);
  sigdelset(&signalMask, SIGINT);
  sigdelset(&signalMask, SIGTERM);
  if (sigprocmask(SIG_SETMASK, &signalMask, NULL) != 0) {
    LOG_ERROR("Couldn't mask all signals except INT/TERM", errno);
  }
}

}  // anonymous namespace

SocketServer::SocketServer() {
  // Initialize the socket fds field for all inactive client slots to -1, so
  // poll skips over it, and we don't attempt to send on it
  for (size_t i = 1; i <= kMaxActiveClients; i++) {
    mPollFds[i].fd = -1;
    mPollFds[i].events = POLLIN;
  }
}

void SocketServer::run(const char *socketName, bool allowSocketCreation,
                       ClientMessageCallback clientMessageCallback) {
  mClientMessageCallback = clientMessageCallback;

  mSockFd = android_get_control_socket(socketName);
  if (mSockFd == INVALID_SOCKET && allowSocketCreation) {
    LOGI("Didn't inherit socket, creating...");
    mSockFd = socket_local_server(socketName,
                                  ANDROID_SOCKET_NAMESPACE_RESERVED,
                                  SOCK_SEQPACKET);
  }

  if (mSockFd == INVALID_SOCKET) {
    LOGE("Couldn't get/create socket");
  } else {
    int ret = listen(mSockFd, kMaxPendingConnectionRequests);
    if (ret < 0) {
      LOG_ERROR("Couldn't listen on socket", errno);
    } else {
      serviceSocket();
    }

    {
      std::lock_guard<std::mutex> lock(mClientsMutex);
      for (const auto& pair : mClients) {
        int clientSocket = pair.first;
        if (close(clientSocket) != 0) {
          LOGI("Couldn't close client %" PRIu16 "'s socket: %s",
               pair.second.clientId, strerror(errno));
        }
      }
      mClients.clear();
    }
    close(mSockFd);
  }
}

void SocketServer::sendToAllClients(const void *data, size_t length) {
  std::lock_guard<std::mutex> lock(mClientsMutex);

  int deliveredCount = 0;
  for (const auto& pair : mClients) {
    int clientSocket = pair.first;
    uint16_t clientId = pair.second.clientId;
    if (sendToClientSocket(data, length, clientSocket, clientId)) {
      deliveredCount++;
    } else if (errno == EINTR) {
      // Exit early if we were interrupted - we should only get this for
      // SIGINT/SIGTERM, so we should exit quickly
      break;
    }
  }

  if (deliveredCount == 0) {
    LOGW("Got message but didn't deliver to any clients");
  }
}

bool SocketServer::sendToClientById(const void *data, size_t length,
                                    uint16_t clientId) {
  std::lock_guard<std::mutex> lock(mClientsMutex);

  bool sent = false;
  for (const auto& pair : mClients) {
    uint16_t thisClientId = pair.second.clientId;
    if (thisClientId == clientId) {
      int clientSocket = pair.first;
      sent = sendToClientSocket(data, length, clientSocket, thisClientId);
      break;
    }
  }

  return sent;
}

void SocketServer::acceptClientConnection() {
  int clientSocket = accept(mSockFd, NULL, NULL);
  if (clientSocket < 0) {
    LOG_ERROR("Couldn't accept client connection", errno);
  } else if (mClients.size() >= kMaxActiveClients) {
    LOGW("Rejecting client request - maximum number of clients reached");
    close(clientSocket);
  } else {
    ClientData clientData;
    clientData.clientId = mNextClientId++;

    // We currently don't handle wraparound - if we're getting this many
    // connects/disconnects, then something is wrong.
    // TODO: can handle this properly by iterating over the existing clients to
    // avoid a conflict.
    if (clientData.clientId == 0) {
      LOGE("Couldn't allocate client ID");
      std::exit(-1);
    }

    bool slotFound = false;
    for (size_t i = 1; i <= kMaxActiveClients; i++) {
      if (mPollFds[i].fd < 0) {
        mPollFds[i].fd = clientSocket;
        slotFound = true;
        break;
      }
    }

    if (!slotFound) {
      LOGE("Couldn't find slot for client!");
      assert(slotFound);
      close(clientSocket);
    } else {
      {
        std::lock_guard<std::mutex> lock(mClientsMutex);
        mClients[clientSocket] = clientData;
      }
      LOGI("Accepted new client connection (count %zu), assigned client ID %"
           PRIu16, mClients.size(), clientData.clientId);
    }
  }
}

void SocketServer::handleClientData(int clientSocket) {
  const ClientData& clientData = mClients[clientSocket];
  uint16_t clientId = clientData.clientId;

  ssize_t packetSize = recv(
      clientSocket, mRecvBuffer.data(), mRecvBuffer.size(), MSG_DONTWAIT);
  if (packetSize < 0) {
    LOGE("Couldn't get packet from client %" PRIu16 ": %s", clientId,
         strerror(errno));
  } else if (packetSize == 0) {
    LOGI("Client %" PRIu16 " disconnected", clientId);
    disconnectClient(clientSocket);
  } else {
    LOGV("Got %zd byte packet from client %" PRIu16, packetSize, clientId);
    mClientMessageCallback(clientId, mRecvBuffer.data(), packetSize);
  }
}

void SocketServer::disconnectClient(int clientSocket) {
  {
    std::lock_guard<std::mutex> lock(mClientsMutex);
    mClients.erase(clientSocket);
  }
  close(clientSocket);

  bool removed = false;
  for (size_t i = 1; i <= kMaxActiveClients; i++) {
    if (mPollFds[i].fd == clientSocket) {
      mPollFds[i].fd = -1;
      removed = true;
      break;
    }
  }

  if (!removed) {
    LOGE("Out of sync");
    assert(removed);
  }
}

bool SocketServer::sendToClientSocket(const void *data, size_t length,
                                      int clientSocket, uint16_t clientId) {
  errno = 0;
  ssize_t bytesSent = send(clientSocket, data, length, 0);
  if (bytesSent < 0) {
    LOGE("Error sending packet of size %zu to client %" PRIu16 ": %s",
         length, clientId, strerror(errno));
  } else if (bytesSent == 0) {
    LOGW("Client %" PRIu16 " disconnected before message could be delivered",
         clientId);
  } else {
    LOGV("Delivered message of size %zu bytes to client %" PRIu16, length,
         clientId);
  }

  return (bytesSent > 0);
}

void SocketServer::serviceSocket() {
  constexpr size_t kListenIndex = 0;
  static_assert(kListenIndex == 0, "Code assumes that the first index is "
                "always the listen socket");

  mPollFds[kListenIndex].fd = mSockFd;
  mPollFds[kListenIndex].events = POLLIN;

  // Signal mask used with ppoll() so we gracefully handle SIGINT and SIGTERM,
  // and ignore other signals
  sigset_t signalMask;
  sigfillset(&signalMask);
  sigdelset(&signalMask, SIGINT);
  sigdelset(&signalMask, SIGTERM);

  // Masking signals here ensure that after this point, we won't handle INT/TERM
  // until after we call into ppoll()
  maskAllSignals();
  std::signal(SIGINT, signalHandler);
  std::signal(SIGTERM, signalHandler);

  LOGI("Ready to accept connections");
  while (!sSignalReceived) {
    int ret = ppoll(mPollFds, 1 + kMaxActiveClients, nullptr, &signalMask);
    maskAllSignalsExceptIntAndTerm();
    if (ret == -1) {
      LOGI("Exiting poll loop: %s", strerror(errno));
      break;
    }

    if (mPollFds[kListenIndex].revents & POLLIN) {
      acceptClientConnection();
    }

    for (size_t i = 1; i <= kMaxActiveClients; i++) {
      if (mPollFds[i].fd < 0) {
        continue;
      }

      if (mPollFds[i].revents & POLLIN) {
        handleClientData(mPollFds[i].fd);
      }
    }

    // Mask all signals to ensure that sSignalReceived can't become true between
    // checking it in the while condition and calling into ppoll()
    maskAllSignals();
  }
}

void SocketServer::signalHandler(int signal) {
  LOGD("Caught signal %d", signal);
  sSignalReceived = true;
}

}  // namespace chre
}  // namespace android
