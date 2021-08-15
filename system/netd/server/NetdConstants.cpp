/*
 * Copyright (C) 2012 The Android Open Source Project
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

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <net/if.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define LOG_TAG "Netd"

#include <android-base/stringprintf.h>
#include <cutils/sockets.h>
#include <log/log.h>
#include <logwrap/logwrap.h>

#include "Controllers.h"
#include "NetdConstants.h"
#include "IptablesRestoreController.h"

int execIptablesRestoreWithOutput(IptablesTarget target, const std::string& commands,
                                  std::string *output) {
    return android::net::gCtls->iptablesRestoreCtrl.execute(target, commands, output);
}

int execIptablesRestore(IptablesTarget target, const std::string& commands) {
    return execIptablesRestoreWithOutput(target, commands, nullptr);
}

int execIptablesRestoreCommand(IptablesTarget target, const std::string& table,
                               const std::string& command, std::string *output) {
    std::string fullCmd = android::base::StringPrintf("*%s\n%s\nCOMMIT\n", table.c_str(),
                                                      command.c_str());
    return execIptablesRestoreWithOutput(target, fullCmd, output);
}

/*
 * Check an interface name for plausibility. This should e.g. help against
 * directory traversal.
 */
bool isIfaceName(const std::string& name) {
    size_t i;
    if ((name.empty()) || (name.size() > IFNAMSIZ)) {
        return false;
    }

    /* First character must be alphanumeric */
    if (!isalnum(name[0])) {
        return false;
    }

    for (i = 1; i < name.size(); i++) {
        if (!isalnum(name[i]) && (name[i] != '_') && (name[i] != '-') && (name[i] != ':')) {
            return false;
        }
    }

    return true;
}

int parsePrefix(const char *prefix, uint8_t *family, void *address, int size, uint8_t *prefixlen) {
    if (!prefix || !family || !address || !prefixlen) {
        return -EFAULT;
    }

    // Find the '/' separating address from prefix length.
    const char *slash = strchr(prefix, '/');
    const char *prefixlenString = slash + 1;
    if (!slash || !*prefixlenString)
        return -EINVAL;

    // Convert the prefix length to a uint8_t.
    char *endptr;
    unsigned templen;
    templen = strtoul(prefixlenString, &endptr, 10);
    if (*endptr || templen > 255) {
        return -EINVAL;
    }
    *prefixlen = templen;

    // Copy the address part of the prefix to a local buffer. We have to copy
    // because inet_pton and getaddrinfo operate on null-terminated address
    // strings, but prefix is const and has '/' after the address.
    std::string addressString(prefix, slash - prefix);

    // Parse the address.
    addrinfo *res;
    addrinfo hints = {
        .ai_flags = AI_NUMERICHOST,
    };
    int ret = getaddrinfo(addressString.c_str(), nullptr, &hints, &res);
    if (ret || !res) {
        return -EINVAL;  // getaddrinfo return values are not errno values.
    }

    // Convert the address string to raw address bytes.
    void *rawAddress;
    int rawLength;
    switch (res[0].ai_family) {
        case AF_INET: {
            if (*prefixlen > 32) {
                return -EINVAL;
            }
            sockaddr_in *sin = (sockaddr_in *) res[0].ai_addr;
            rawAddress = &sin->sin_addr;
            rawLength = 4;
            break;
        }
        case AF_INET6: {
            if (*prefixlen > 128) {
                return -EINVAL;
            }
            sockaddr_in6 *sin6 = (sockaddr_in6 *) res[0].ai_addr;
            rawAddress = &sin6->sin6_addr;
            rawLength = 16;
            break;
        }
        default: {
            freeaddrinfo(res);
            return -EAFNOSUPPORT;
        }
    }

    if (rawLength > size) {
        freeaddrinfo(res);
        return -ENOSPC;
    }

    *family = res[0].ai_family;
    memcpy(address, rawAddress, rawLength);
    freeaddrinfo(res);

    return rawLength;
}

void blockSigpipe() {
    sigset_t mask;

    sigemptyset(&mask);
    sigaddset(&mask, SIGPIPE);
    if (sigprocmask(SIG_BLOCK, &mask, nullptr) != 0)
        ALOGW("WARNING: SIGPIPE not blocked\n");
}

void setCloseOnExec(const char *sock) {
    int fd = android_get_control_socket(sock);
    int flags = fcntl(fd, F_GETFD, 0);
    if (flags == -1) {
        ALOGE("Can't get fd flags for control socket %s", sock);
        flags = 0;
    }
    flags |= FD_CLOEXEC;
    if (fcntl(fd, F_SETFD, flags) == -1) {
        ALOGE("Can't set control socket %s to FD_CLOEXEC", sock);
    }
}
