/******************************************************************************
 *
 *  Copyright 2013 Google, Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#define LOG_TAG "bt_snoop_net"

#include <base/logging.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/prctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <mutex>

#include "osi/include/log.h"
#include "osi/include/osi.h"

static void safe_close_(int* fd);
static void* listen_fn_(void* context);

static const char* LISTEN_THREAD_NAME_ = "btsnoop_net_listen";
static const int LOCALHOST_ = 0x7F000001;
static const int LISTEN_PORT_ = 8872;

static pthread_t listen_thread_;
static bool listen_thread_valid_ = false;
static std::mutex client_socket_mutex_;
static int listen_socket_ = -1;
static int client_socket_ = -1;

void btsnoop_net_open() {
#if (BT_NET_DEBUG != TRUE)
  return;  // Disable using network sockets for security reasons
#endif

  listen_thread_valid_ =
      (pthread_create(&listen_thread_, NULL, listen_fn_, NULL) == 0);
  if (!listen_thread_valid_)
    LOG_ERROR(LOG_TAG, "%s pthread_create failed: %s", __func__,
              strerror(errno));
}

void btsnoop_net_close() {
#if (BT_NET_DEBUG != TRUE)
  return;  // Disable using network sockets for security reasons
#endif

  if (listen_thread_valid_) {
    shutdown(listen_socket_, SHUT_RDWR);
    pthread_join(listen_thread_, NULL);
    safe_close_(&client_socket_);
    listen_thread_valid_ = false;
  }
}

void btsnoop_net_write(const void* data, size_t length) {
#if (BT_NET_DEBUG != TRUE)
  return;  // Disable using network sockets for security reasons
#endif

  std::lock_guard<std::mutex> lock(client_socket_mutex_);
  if (client_socket_ != -1) {
    ssize_t ret;
    OSI_NO_INTR(ret = send(client_socket_, data, length, 0));

    if (ret == -1 && errno == ECONNRESET) {
      safe_close_(&client_socket_);
    }
  }
}

static void* listen_fn_(UNUSED_ATTR void* context) {
  int enable = 1;

  prctl(PR_SET_NAME, (unsigned long)LISTEN_THREAD_NAME_, 0, 0, 0);

  listen_socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (listen_socket_ == -1) {
    LOG_ERROR(LOG_TAG, "%s socket creation failed: %s", __func__,
              strerror(errno));
    goto cleanup;
  }

  if (setsockopt(listen_socket_, SOL_SOCKET, SO_REUSEADDR, &enable,
                 sizeof(enable)) == -1) {
    LOG_ERROR(LOG_TAG, "%s unable to set SO_REUSEADDR: %s", __func__,
              strerror(errno));
    goto cleanup;
  }

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(LOCALHOST_);
  addr.sin_port = htons(LISTEN_PORT_);
  if (bind(listen_socket_, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    LOG_ERROR(LOG_TAG, "%s unable to bind listen socket: %s", __func__,
              strerror(errno));
    goto cleanup;
  }

  if (listen(listen_socket_, 10) == -1) {
    LOG_ERROR(LOG_TAG, "%s unable to listen: %s", __func__, strerror(errno));
    goto cleanup;
  }

  for (;;) {
    int client_socket;
    OSI_NO_INTR(client_socket = accept(listen_socket_, NULL, NULL));
    if (client_socket == -1) {
      if (errno == EINVAL || errno == EBADF) {
        break;
      }
      LOG_WARN(LOG_TAG, "%s error accepting socket: %s", __func__,
               strerror(errno));
      continue;
    }

    /* When a new client connects, we have to send the btsnoop file header. This
     * allows a decoder to treat the session as a new, valid btsnoop file. */
    std::lock_guard<std::mutex> lock(client_socket_mutex_);
    safe_close_(&client_socket_);
    client_socket_ = client_socket;

    OSI_NO_INTR(send(client_socket_, "btsnoop\0\0\0\0\1\0\0\x3\xea", 16, 0));
  }

cleanup:
  safe_close_(&listen_socket_);
  return NULL;
}

static void safe_close_(int* fd) {
  CHECK(fd != NULL);
  if (*fd != -1) {
    close(*fd);
    *fd = -1;
  }
}
