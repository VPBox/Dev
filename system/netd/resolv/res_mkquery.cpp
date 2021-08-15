/*	$NetBSD: res_mkquery.c,v 1.6 2006/01/24 17:40:32 christos Exp $	*/

/*
 * Copyright (c) 1985, 1993
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
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 * 	This product includes software developed by the University of
 * 	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
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
 * Portions Copyright (c) 1993 by Digital Equipment Corporation.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies, and that
 * the name of Digital Equipment Corporation not be used in advertising or
 * publicity pertaining to distribution of the document or software without
 * specific, written prior permission.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND DIGITAL EQUIPMENT CORP. DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS.   IN NO EVENT SHALL DIGITAL EQUIPMENT
 * CORPORATION BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
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

#define LOG_TAG "res_mkquery"

#include <algorithm>  // std::min()

#include <arpa/nameser.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include <android-base/logging.h>

#include "resolv_private.h"

// Queries will be padded to a multiple of this length when EDNS0 is active.
constexpr uint16_t kEdns0Padding = 128;

extern const char* const _res_opcodes[] = {
        "QUERY",  "IQUERY", "CQUERYM", "CQUERYU", /* experimental */
        "NOTIFY",                                 /* experimental */
        "UPDATE", "6",      "7",       "8",        "9",       "10",
        "11",     "12",     "13",      "ZONEINIT", "ZONEREF",
};

/*
 * Form all types of queries.
 * Returns the size of the result or -1.
 */
int res_nmkquery(res_state statp, int op,    /* opcode of query */
                 const char* dname,          /* domain name */
                 int cl, int type,           /* class and type of query */
                 const u_char* data,         /* resource record data */
                 int datalen,                /* length of data */
                 const u_char* /*newrr_in*/, /* new rr for modify or append */
                 u_char* buf,                /* buffer to put query */
                 int buflen)                 /* size of buffer */
{
    HEADER* hp;
    u_char *cp, *ep;
    int n;
    u_char *dnptrs[20], **dpp, **lastdnptr;

    LOG(DEBUG) << __func__ << ": (" << _res_opcodes[op] << ", " << p_class(cl) << ", "
               << p_type(type) << ")";

    /*
     * Initialize header fields.
     */
    if ((buf == NULL) || (buflen < HFIXEDSZ)) return (-1);
    memset(buf, 0, HFIXEDSZ);
    hp = (HEADER*) (void*) buf;
    hp->id = htons(arc4random_uniform(65536));
    hp->opcode = op;
    hp->rd = (statp->options & RES_RECURSE) != 0U;
    hp->ad = (statp->options & RES_USE_DNSSEC) != 0U;
    hp->rcode = NOERROR;
    cp = buf + HFIXEDSZ;
    ep = buf + buflen;
    dpp = dnptrs;
    *dpp++ = buf;
    *dpp++ = NULL;
    lastdnptr = dnptrs + sizeof dnptrs / sizeof dnptrs[0];
    /*
     * perform opcode specific processing
     */
    switch (op) {
        case QUERY:
            [[fallthrough]];
        case NS_NOTIFY_OP:
            if (ep - cp < QFIXEDSZ) return (-1);
            if ((n = dn_comp(dname, cp, ep - cp - QFIXEDSZ, dnptrs, lastdnptr)) < 0) return (-1);
            cp += n;
            *reinterpret_cast<uint16_t*>(cp) = htons(type);
            cp += INT16SZ;
            *reinterpret_cast<uint16_t*>(cp) = htons(cl);
            cp += INT16SZ;
            hp->qdcount = htons(1);
            if (op == QUERY || data == NULL) break;
            /*
             * Make an additional record for completion domain.
             */
            if ((ep - cp) < RRFIXEDSZ) return (-1);
            n = dn_comp((const char*) data, cp, ep - cp - RRFIXEDSZ, dnptrs, lastdnptr);
            if (n < 0) return (-1);
            cp += n;
            *reinterpret_cast<uint16_t*>(cp) = htons(ns_t_null);
            cp += INT16SZ;
            *reinterpret_cast<uint16_t*>(cp) = htons(cl);
            cp += INT16SZ;
            *reinterpret_cast<uint32_t*>(cp) = htonl(0);
            cp += INT32SZ;
            *reinterpret_cast<uint16_t*>(cp) = htons(0);
            cp += INT16SZ;
            hp->arcount = htons(1);
            break;

        case IQUERY:
            /*
             * Initialize answer section
             */
            if (ep - cp < 1 + RRFIXEDSZ + datalen) return (-1);
            *cp++ = '\0'; /* no domain name */
            *reinterpret_cast<uint16_t*>(cp) = htons(type);
            cp += INT16SZ;
            *reinterpret_cast<uint16_t*>(cp) = htons(cl);
            cp += INT16SZ;
            *reinterpret_cast<uint32_t*>(cp) = htonl(0);
            cp += INT32SZ;
            *reinterpret_cast<uint16_t*>(cp) = htons(datalen);
            cp += INT16SZ;
            if (datalen) {
                memcpy(cp, data, (size_t) datalen);
                cp += datalen;
            }
            hp->ancount = htons(1);
            break;

        default:
            return (-1);
    }
    return (cp - buf);
}

int res_nopt(res_state statp, int n0, /* current offset in buffer */
             u_char* buf,             /* buffer to put query */
             int buflen,              /* size of buffer */
             int anslen)              /* UDP answer buffer size */
{
    HEADER* hp;
    u_char *cp, *ep;
    u_int16_t flags = 0;

    LOG(DEBUG) << __func__;

    hp = (HEADER*) (void*) buf;
    cp = buf + n0;
    ep = buf + buflen;

    if ((ep - cp) < 1 + RRFIXEDSZ) return (-1);

    *cp++ = 0; /* "." */

    // Attach OPT pseudo-RR, as documented in RFC2671 (EDNS0).
    *reinterpret_cast<uint16_t*>(cp) = htons(ns_t_opt); /* TYPE */
    cp += INT16SZ;
    if (anslen > 0xffff) anslen = 0xffff;
    *reinterpret_cast<uint16_t*>(cp) = htons(anslen); /* CLASS = UDP payload size */
    cp += INT16SZ;
    *cp++ = NOERROR; /* extended RCODE */
    *cp++ = 0;       /* EDNS version */
    if (statp->options & RES_USE_DNSSEC) {
        LOG(DEBUG) << __func__ << ": ENDS0 DNSSEC";
        flags |= NS_OPT_DNSSEC_OK;
    }
    *reinterpret_cast<uint16_t*>(cp) = htons(flags);
    cp += INT16SZ;

    // EDNS0 padding
    const uint16_t minlen = static_cast<uint16_t>(cp - buf) + 3 * INT16SZ;
    const uint16_t extra = minlen % kEdns0Padding;
    uint16_t padlen = (kEdns0Padding - extra) % kEdns0Padding;
    if (minlen > buflen) {
        return -1;
    }
    padlen = std::min(padlen, static_cast<uint16_t>(buflen - minlen));
    *reinterpret_cast<uint16_t*>(cp) = htons(padlen + 2 * INT16SZ); /* RDLEN */
    cp += INT16SZ;
    *reinterpret_cast<uint16_t*>(cp) = htons(NS_OPT_PADDING); /* OPTION-CODE */
    cp += INT16SZ;
    *reinterpret_cast<uint16_t*>(cp) = htons(padlen); /* OPTION-LENGTH */
    cp += INT16SZ;
    memset(cp, 0, padlen);
    cp += padlen;

    hp->arcount = htons(ntohs(hp->arcount) + 1);
    return (cp - buf);
}
