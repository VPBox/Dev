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

#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <poll.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <memory>

#include <android-base/logging.h>
#include <cutils/sockets.h>
#include <libminijail.h>

#include <nvram/core/nvram_manager.h>
#include <nvram/messages/nvram_messages.h>

// This is defined in fake_nvram_storage.h
void InitStorage(int data_dir_fd);

namespace {

// Minijail parameters.
constexpr char kNvramUser[] = "nvram";
constexpr char kNvramGroup[] = "nvram";
constexpr char kNvramSeccompPolicyPath[] =
    "/system/usr/share/policy/fake-nvram-seccomp.policy";

// Name of the control socket served by this daemon.
constexpr char kNvramControlSocketName[] = "nvram";

// The default data directory.
constexpr char kNvramDataDirectory[] = "/data/misc/fake-nvram/";

// Connection backlog on control socket.
constexpr int kControlSocketBacklog = 20;

// Maximum number of client sockets supported.
constexpr int kMaxClientSockets = 32;

// Size of the NVRAM message buffer for reading and writing serialized NVRAM
// command messages from and to the control socket.
constexpr int kNvramMessageBufferSize = 4096;

// Variables holding command-line flags.
const char* g_data_directory_path = kNvramDataDirectory;
const char* g_control_socket_name = kNvramControlSocketName;

// Parses the command line. Returns true if successful.
bool ParseCommandLine(int argc, char** argv) {
  while (true) {
    static const struct option options[] = {
        {"data_directory", required_argument, nullptr, 'd'},
        {"control_socket", required_argument, nullptr, 's'},
    };

    int option_index = 0;
    int c = getopt_long(argc, argv, "", options, &option_index);
    if (c == -1) {
      break;
    }

    switch (c) {
      case 'd':
        g_data_directory_path = optarg;
        break;
      case 's':
        g_control_socket_name = optarg;
        break;
      default:
        return false;
    }
  }

  return true;
}

// Sets up a restricted environment using minijail and enters it.
bool InitMinijail() {
  std::unique_ptr<struct minijail, void (*)(struct minijail*)> minijail(
      minijail_new(), &minijail_destroy);
  if (minijail_change_user(minijail.get(), kNvramUser) ||
      minijail_change_group(minijail.get(), kNvramGroup)) {
    return false;
  }
  minijail_use_seccomp_filter(minijail.get());
  minijail_no_new_privs(minijail.get());
  minijail_parse_seccomp_filters(minijail.get(), kNvramSeccompPolicyPath);
  minijail_enter(minijail.get());
  return true;
}

// Reads a single command from |socket|, decodes the command, executes it on
// |nvram_manager|, encodes the response, and writes the reply back to |socket|.
// Returns true on success, false on errors (in which case the caller is
// expected the close the |socket|).
bool ProcessCommand(int socket, nvram::NvramManager* nvram_manager) {
  uint8_t command_buffer[kNvramMessageBufferSize];
  ssize_t bytes_read =
      TEMP_FAILURE_RETRY(read(socket, command_buffer, sizeof(command_buffer)));
  if (bytes_read == 0) {
    return false;
  }

  if (bytes_read < 0) {
    PLOG(ERROR) << "Failed to read command from client socket";
    return false;
  }

  nvram::Request request;
  if (!nvram::Decode(command_buffer, bytes_read, &request)) {
    LOG(WARNING) << "Failed to decode command request!";
    return false;
  }

  nvram::Response response;
  nvram_manager->Dispatch(request, &response);
  size_t response_size = sizeof(command_buffer);
  if (!nvram::Encode(response, command_buffer, &response_size)) {
    LOG(WARNING) << "Failed to encode command response!";
    return false;
  }

  if (TEMP_FAILURE_RETRY(write(socket, command_buffer, response_size)) < 0) {
    PLOG(ERROR) << "Failed to write response to client socket";
    return false;
  }

  return true;
}

// Listens for incoming connections or data, accepts connections and processes
// data as needed.
int ProcessMessages(int control_socket_fd, nvram::NvramManager* nvram_manager) {
  struct pollfd poll_fds[kMaxClientSockets];
  memset(poll_fds, 0, sizeof(poll_fds));
  poll_fds[0].fd = control_socket_fd;
  poll_fds[0].events = POLLIN;
  poll_fds[0].revents = 0;
  nfds_t poll_fds_count = 1;
  while (TEMP_FAILURE_RETRY(poll(poll_fds, poll_fds_count, -1)) >= 0) {
    if (poll_fds[0].revents & POLLIN) {
      // Accept a new connection.
      int client_socket = accept(control_socket_fd, NULL, 0);
      if (client_socket < 0) {
        PLOG(ERROR) << "Error accepting connection";
        return errno;
      }

      // Add |client_socket| to |poll_fds|.
      if (poll_fds_count < kMaxClientSockets) {
        poll_fds[poll_fds_count].fd = client_socket;
        poll_fds[poll_fds_count].events = POLLIN;
        poll_fds[poll_fds_count].revents = 0;
        ++poll_fds_count;
      } else {
        LOG(WARNING) << "Too many open client sockets, rejecting connection.";
        // No need to handle EINTR specially here as bionic filters it out.
        if (close(client_socket)) {
          PLOG(ERROR) << "Failed to close connection socket after error";
        }
      }
    }

    // Walk the connection fds backwards. This way, we can remove fds by
    // replacing the slot with the last array element, which we have processed
    // already.
    for (int i = poll_fds_count - 1; i > 0; --i) {
      if (poll_fds[i].revents & POLLIN) {
        if (!ProcessCommand(poll_fds[i].fd, nvram_manager)) {
          // No need to handle EINTR specially here as bionic filters it out.
          if (close(poll_fds[i].fd)) {
            PLOG(ERROR) << "Failed to close connection socket after error";
          }
          --poll_fds_count;
          poll_fds[i] = poll_fds[poll_fds_count];
        }
      }
      poll_fds[i].revents = 0;
    }
  }

  // poll error.
  PLOG(ERROR) << "Failed to poll control socket";
  return errno;
};

}  // namespace

int main(int argc, char** argv) {
  if (!ParseCommandLine(argc, argv)) {
    return EINVAL;
  }

  int control_socket_fd = android_get_control_socket(g_control_socket_name);
  if (control_socket_fd < 0) {
    LOG(ERROR) << "Failed to get control socket.";
    return EINVAL;
  }

  if (listen(control_socket_fd, kControlSocketBacklog)) {
    PLOG(ERROR) << "Failed to listen on control socket";
    return errno;
  }

  if (!InitMinijail()) {
    LOG(ERROR) << "Failed to drop privileges.";
    return -1;
  }

  int data_dir_fd =
      TEMP_FAILURE_RETRY(open(g_data_directory_path, O_RDONLY | O_DIRECTORY));
  if (data_dir_fd < 0) {
    PLOG(ERROR) << "Failed to open data directory";
    return errno;
  }

  InitStorage(data_dir_fd);

  nvram::NvramManager nvram_manager;
  return ProcessMessages(control_socket_fd, &nvram_manager);
}
