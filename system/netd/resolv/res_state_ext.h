/*	$NetBSD: res_private.h,v 1.1.1.1 2004/05/20 17:18:54 christos Exp $	*/

#ifndef NETD_RES_STATE_EXT_H
#define NETD_RES_STATE_EXT_H

#include "resolv_private.h"

// TODO: consider inlining into res_state
struct res_state_ext {
    sockaddr_union nsaddrs[MAXNS];
    struct sort_list {
        int af;
        union {
            struct in_addr ina;
            struct in6_addr in6a;
        } addr, mask;
    } sort_list[MAXRESOLVSORT];
    char nsuffix[64];
    char nsuffix2[64];
};

#endif  // NETD_RES_STATE_EXT_H
