/*	$NetBSD: resolv.h,v 1.31 2005/12/26 19:01:47 perry Exp $	*/

/*
 * Copyright (c) 1983, 1987, 1989
 *    The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * Copyright (c) 2004 by Internet Systems Consortium, Inc. ("ISC")
 * Portions Copyright (c) 1996-1999 by Internet Software Consortium.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS.  IN NO EVENT SHALL ISC BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 *	@(#)resolv.h	8.1 (Berkeley) 6/2/93
 *	Id: resolv.h,v 1.7.2.11.4.2 2004/06/25 00:41:05 marka Exp
 */

#ifndef NETD_RESOLV_PRIVATE_H
#define NETD_RESOLV_PRIVATE_H

#include <android-base/logging.h>
#include <net/if.h>
#include <resolv.h>
#include <time.h>
#include <string>

#include "netd_resolv/params.h"
#include "netd_resolv/resolv.h"
#include "netd_resolv/stats.h"
#include "resolv_static.h"
#include "stats.pb.h"

// Linux defines MAXHOSTNAMELEN as 64, while the domain name limit in
// RFC 1034 and RFC 1035 is 255 octets.
#ifdef MAXHOSTNAMELEN
#undef MAXHOSTNAMELEN
#endif
#define MAXHOSTNAMELEN 256

/*
 * Global defines and variables for resolver stub.
 */
#define MAXDFLSRCH 3       /* # default domain levels to try */
#define LOCALDOMAINPARTS 2 /* min levels in name that is "local" */

#define RES_TIMEOUT 5000 /* min. milliseconds between retries */
#define MAXRESOLVSORT 10  /* number of net to sort on */
#define RES_MAXNDOTS 15   /* should reflect bit field size */
#define RES_DFLRETRY 2    /* Default #/tries. */
#define RES_MAXTIME 65535 /* Infinity, in milliseconds. */

struct res_state_ext;

struct __res_state {
    unsigned netid;                        /* NetId: cache key and socket mark */
    u_long options;                        /* option flags - see below. */
    int nscount;                           /* number of name srvers */
    struct sockaddr_in nsaddr_list[MAXNS]; /* address of name server */
#define nsaddr nsaddr_list[0]              /* for backward compatibility */
    u_short id;                            /* current message id */
    char* dnsrch[MAXDNSRCH + 1];           /* components of domain to search */
    char defdname[256];                    /* default domain (deprecated) */
    unsigned ndots : 4;                    /* threshold for initial abs. query */
    unsigned nsort : 4;                    /* number of elements in sort_list[] */
    char unused[3];
    struct {
        struct in_addr addr;
        uint32_t mask;
    } sort_list[MAXRESOLVSORT];
    unsigned _mark;       /* If non-0 SET_MARK to _mark on all request sockets */
    int _vcsock;          /* PRIVATE: for res_send VC i/o */
    u_int _flags;         /* PRIVATE: see below */
    u_int _pad;           /* make _u 64 bit aligned */
    union {
        /* On an 32-bit arch this means 512b total. */
        char pad[72 - 4 * sizeof(int) - 2 * sizeof(void*)];
        struct {
            uint16_t nscount;
            uint16_t nstimes[MAXNS]; /* ms. */
            int nssocks[MAXNS];
            struct res_state_ext* ext; /* extention for IPv6 */
        } _ext;
    } _u;
    struct res_static rstatic[1];
    bool use_local_nameserver; /* DNS-over-TLS bypass */
    android::net::NetworkDnsEventReported* event;
};

typedef struct __res_state* res_state;

/* Retrieve a local copy of the stats for the given netid. The buffer must have space for
 * MAXNS __resolver_stats. Returns the revision id of the resolvers used.
 */
int resolv_cache_get_resolver_stats(unsigned netid, res_params* params, res_stats stats[MAXNS]);

/* Add a sample to the shared struct for the given netid and server, provided that the
 * revision_id of the stored servers has not changed.
 */
void _resolv_cache_add_resolver_stats_sample(unsigned netid, int revision_id, int ns,
                                             const res_sample* sample, int max_samples);

// Calculate the round-trip-time from start time t0 and end time t1.
int _res_stats_calculate_rtt(const timespec* t1, const timespec* t0);

// Create a sample for calculating server reachability statistics.
void _res_stats_set_sample(res_sample* sample, time_t now, int rcode, int rtt);

/* End of stats related definitions */

// Flags for res_state->_flags
#define RES_F_VC 0x00000001        // socket is TCP
#define RES_F_EDNS0ERR 0x00000004  // EDNS0 caused errors

/*
 * Resolver options (keep these in synch with res_debug.c, please)
 */
#define RES_INIT 0x00000001           /* address initialized */
#define RES_DEBUG 0x00000002          /* print debug messages */
#define RES_AAONLY 0x00000004         /* authoritative answers only (!IMPL)*/
#define RES_USEVC 0x00000008          /* use virtual circuit */
#define RES_PRIMARY 0x00000010        /* query primary server only (!IMPL) */
#define RES_IGNTC 0x00000020          /* ignore trucation errors */
#define RES_RECURSE 0x00000040        /* recursion desired */
#define RES_DEFNAMES 0x00000080       /* use default domain name */
#define RES_STAYOPEN 0x00000100       /* Keep TCP socket open */
#define RES_DNSRCH 0x00000200         /* search up local domain tree */
#define RES_INSECURE1 0x00000400      /* type 1 security disabled */
#define RES_INSECURE2 0x00000800      /* type 2 security disabled */
#define RES_USE_INET6 0x00002000      /* use/map IPv6 in gethostbyname() */
#define RES_ROTATE 0x00004000         /* rotate ns list after each query */
#define RES_NOCHECKNAME 0x00008000    /* do not check names for sanity. */
#define RES_KEEPTSIG 0x00010000       /* do not strip TSIG records */
#define RES_BLAST 0x00020000          /* blast all recursive servers */
#define RES_NOTLDQUERY 0x00100000     /* don't unqualified name as a tld */
#define RES_USE_DNSSEC 0x00200000     /* use DNSSEC using OK bit in OPT */
/* #define RES_DEBUG2	0x00400000 */ /* nslookup internal */
/* KAME extensions: use higher bit to avoid conflict with ISC use */
#define RES_USE_DNAME 0x10000000  /* use DNAME */
#define RES_USE_EDNS0 0x40000000  /* use EDNS0 if configured */
#define RES_NO_NIBBLE2 0x80000000 /* disable alternate nibble lookup */

#define RES_DEFAULT (RES_RECURSE | RES_DEFNAMES | RES_DNSRCH | RES_NO_NIBBLE2)


/*
 * Error code extending h_errno codes defined in bionic/libc/include/netdb.h.
 *
 * This error code, including legacy h_errno, is returned from res_nquery(), res_nsearch(),
 * res_nquerydomain(), res_queryN(), res_searchN() and res_querydomainN() for DNS metrics.
 *
 * TODO: Consider mapping legacy and extended h_errno into a unified resolver error code mapping.
 */
#define NETD_RESOLV_H_ERRNO_EXT_TIMEOUT RCODE_TIMEOUT

extern const char* const _res_opcodes[];

/* Things involving an internal (static) resolver context. */
struct __res_state* res_get_state(void);

int res_hnok(const char*);
int res_ownok(const char*);
int res_mailok(const char*);
int res_dnok(const char*);
int dn_skipname(const u_char*, const u_char*);
void putlong(uint32_t, u_char*);
void putshort(uint16_t, u_char*);

// Thread-unsafe functions returning pointers to static buffers :-(
// TODO: switch all res_debug to std::string
const char* p_class(int);
const char* p_type(int);
const char* p_rcode(int);
const char* p_section(int, int);

int res_nameinquery(const char*, int, int, const u_char*, const u_char*);
int res_queriesmatch(const u_char*, const u_char*, const u_char*, const u_char*);
/* Things involving a resolver context. */
int res_ninit(res_state);
void res_pquery(const u_char*, int);

int res_nquery(res_state, const char*, int, int, u_char*, int, int*);
int res_nsearch(res_state, const char*, int, int, u_char*, int, int*);
int res_nquerydomain(res_state, const char*, const char*, int, int, u_char*, int, int*);
int res_nmkquery(res_state, int, const char*, int, int, const u_char*, int, const u_char*, u_char*,
                 int);
int res_nsend(res_state, const u_char*, int, u_char*, int, int*, uint32_t);
void res_nclose(res_state);
int res_nopt(res_state, int, u_char*, int, int);
int res_vinit(res_state, int);
void res_ndestroy(res_state);
void res_setservers(res_state, const sockaddr_union*, int);
int res_getservers(res_state, sockaddr_union*, int);

struct android_net_context; /* forward */
void res_setnetcontext(res_state, const struct android_net_context*,
                       android::net::NetworkDnsEventReported* event);

int getaddrinfo_numeric(const char* hostname, const char* servname, addrinfo hints,
                        addrinfo** result);

// Helper function for converting h_errno to the error codes visible to netd
int herrnoToAiErrno(int herrno);

// switch resolver log severity
android::base::LogSeverity logSeverityStrToEnum(const std::string& logSeverityStr);

template <typename Dest>
Dest saturate_cast(int64_t x) {
    using DestLimits = std::numeric_limits<Dest>;
    if (x > DestLimits::max()) return DestLimits::max();
    if (x < DestLimits::min()) return DestLimits::min();
    return static_cast<Dest>(x);
}

android::net::NsType getQueryType(const uint8_t* msg, size_t msgLen);

android::net::IpVersion ipFamilyToIPVersion(int ipFamily);

#endif  // NETD_RESOLV_PRIVATE_H
