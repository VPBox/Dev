/*
 * Copyright (C) 2014 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#pragma once

#include "params.h"

#include <netinet/in.h>

typedef union sockaddr_union {
    struct sockaddr sa;
    struct sockaddr_in sin;
    struct sockaddr_in6 sin6;
} sockaddr_union;

/*
 * Passing NETID_UNSET as the netId causes system/netd/resolv/DnsProxyListener.cpp to
 * fill in the appropriate default netId for the query.
 */
#define NETID_UNSET 0u

/*
 * MARK_UNSET represents the default (i.e. unset) value for a socket mark.
 */
#define MARK_UNSET 0u

/*
 * Error code extending EAI_* codes defined in bionic/libc/include/netdb.h.
 * This error code, including EAI_*, returned from android_getaddrinfofornetcontext()
 * and android_gethostbynamefornetcontext() are used for DNS metrics.
 */
#define NETD_RESOLV_TIMEOUT 255  // consistent with RCODE_TIMEOUT

/*
 * A struct to capture context relevant to network operations.
 *
 * Application and DNS netids/marks can differ from one another under certain
 * circumstances, notably when a VPN applies to the given uid's traffic but the
 * VPN network does not have its own DNS servers explicitly provisioned.
 *
 * The introduction of per-UID routing means the uid is also an essential part
 * of the evaluation context. Its proper uninitialized value is
 * NET_CONTEXT_INVALID_UID.
 */
struct android_net_context {
    unsigned app_netid;
    unsigned app_mark;
    unsigned dns_netid;
    unsigned dns_mark;
    uid_t uid;
    unsigned flags;
};

#define NET_CONTEXT_INVALID_UID ((uid_t) -1)
#define NET_CONTEXT_FLAG_USE_LOCAL_NAMESERVERS 0x00000001
#define NET_CONTEXT_FLAG_USE_EDNS 0x00000002

// TODO: investigate having the resolver check permissions itself, either by adding support to
// libbinder_ndk or by converting IPermissionController into a stable AIDL interface.
typedef bool (*check_calling_permission_callback)(const char* permission);
typedef void (*get_network_context_callback)(unsigned netid, uid_t uid,
                                             android_net_context* netcontext);
typedef void (*log_callback)(const char* msg);

/*
 * Some functions needed by the resolver (e.g. checkCallingPermission()) live in
 * libraries with no ABI stability guarantees, such as libbinder.so.
 * As a temporary workaround, we keep these functions in netd and call them via
 * function pointers.
 */
struct ResolverNetdCallbacks {
    check_calling_permission_callback check_calling_permission;
    get_network_context_callback get_network_context;
    log_callback log;
};

LIBNETD_RESOLV_PUBLIC bool resolv_has_nameservers(unsigned netid);

// Set callbacks and bring DnsResolver up.
LIBNETD_RESOLV_PUBLIC bool resolv_init(const ResolverNetdCallbacks* callbacks);
