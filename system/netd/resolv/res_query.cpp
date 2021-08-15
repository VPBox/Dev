/*	$NetBSD: res_query.c,v 1.7 2006/01/24 17:41:25 christos Exp $	*/

/*
 * Copyright (c) 1988, 1993
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

#define LOG_TAG "res_query"

#include <arpa/inet.h>
#include <arpa/nameser.h>
#include <ctype.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/param.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <android-base/logging.h>

#include "resolv_cache.h"
#include "resolv_private.h"

#if PACKETSZ > 1024
#define MAXPACKET PACKETSZ
#else
#define MAXPACKET 1024
#endif

/*
 * Formulate a normal query, send, and await answer.
 * Returned answer is placed in supplied buffer "answer".
 * Perform preliminary check of answer, returning success only
 * if no error is indicated and the answer count is nonzero.
 * Return the size of the response on success, -1 on error.
 * Error number is left in *herrno.
 *
 * Caller must parse answer and determine whether it answers the question.
 */
int res_nquery(res_state statp, const char* name,  // domain name
               int cl, int type,                   // class and type of query
               u_char* answer,                     // buffer to put answer
               int anslen,                         // size of answer buffer
               int* herrno)                        // legacy and extended h_errno
                                                   // NETD_RESOLV_H_ERRNO_EXT_*
{
    u_char buf[MAXPACKET];
    HEADER* hp = (HEADER*) (void*) answer;
    int n;
    int rcode = NOERROR;
    bool retried = false;

again:
    hp->rcode = NOERROR; /* default */

    LOG(DEBUG) << __func__ << ": (" << cl << ", " << type << ")";

    n = res_nmkquery(statp, QUERY, name, cl, type, NULL, 0, NULL, buf, sizeof(buf));
    if (n > 0 && (statp->options & (RES_USE_EDNS0 | RES_USE_DNSSEC)) != 0U && !retried)
        n = res_nopt(statp, n, buf, sizeof(buf), anslen);
    if (n <= 0) {
        LOG(DEBUG) << __func__ << ": mkquery failed";
        *herrno = NO_RECOVERY;
        return n;
    }
    n = res_nsend(statp, buf, n, answer, anslen, &rcode, 0);
    if (n < 0) {
        /* if the query choked with EDNS0, retry without EDNS0 */
        if ((statp->options & (RES_USE_EDNS0 | RES_USE_DNSSEC)) != 0U &&
            (statp->_flags & RES_F_EDNS0ERR) && !retried) {
            LOG(DEBUG) << __func__ << ": retry without EDNS0";
            retried = true;
            goto again;
        }
        LOG(DEBUG) << __func__ << ": send error";

        // Note that rcodes SERVFAIL, NOTIMP, REFUSED may cause res_nquery() to return a general
        // error code EAI_AGAIN, but mapping the error code from rcode as res_queryN() does for
        // getaddrinfo(). Different rcodes trigger different behaviors:
        //
        // - SERVFAIL, NOTIMP, REFUSED
        //   These result in send_dg() returning 0, causing res_nsend() to try the next
        //   nameserver. After all nameservers failed, res_nsend() returns -ETIMEDOUT, causing
        //   res_nquery() to return EAI_AGAIN here regardless of the rcode from the DNS response.
        //
        // - NXDOMAIN, FORMERR
        //   These rcodes may cause res_nsend() to return successfully (i.e. the result is a
        //   positive integer). In this case, res_nquery() returns error number by referring
        //   the rcode from the DNS response.
        switch (rcode) {
            case RCODE_TIMEOUT:  // Not defined in RFC.
                // DNS metrics monitors DNS query timeout.
                *herrno = NETD_RESOLV_H_ERRNO_EXT_TIMEOUT;  // extended h_errno.
                break;
            default:
                *herrno = TRY_AGAIN;
                break;
        }
        return n;
    }

    if (hp->rcode != NOERROR || ntohs(hp->ancount) == 0) {
        LOG(DEBUG) << __func__ << ": rcode = (" << p_rcode(hp->rcode)
                   << "), counts = an:" << ntohs(hp->ancount) << " ns:" << ntohs(hp->nscount)
                   << " ar:" << ntohs(hp->arcount);

        switch (hp->rcode) {
            case NXDOMAIN:
                *herrno = HOST_NOT_FOUND;
                break;
            case SERVFAIL:
                *herrno = TRY_AGAIN;
                break;
            case NOERROR:
                *herrno = NO_DATA;
                break;
            case FORMERR:
            case NOTIMP:
            case REFUSED:
            default:
                *herrno = NO_RECOVERY;
                break;
        }
        return -1;
    }
    return n;
}

/*
 * Formulate a normal query, send, and retrieve answer in supplied buffer.
 * Return the size of the response on success, -1 on error.
 * If enabled, implement search rules until answer or unrecoverable failure
 * is detected.  Error code, if any, is left in *herrno.
 */
int res_nsearch(res_state statp, const char* name, /* domain name */
                int cl, int type,                  /* class and type of query */
                u_char* answer,                    /* buffer to put answer */
                int anslen,                        /* size of answer */
                int* herrno)                       /* legacy and extended
                                                      h_errno NETD_RESOLV_H_ERRNO_EXT_* */
{
    const char *cp, *const *domain;
    HEADER* hp = (HEADER*) (void*) answer;
    u_int dots;
    int trailing_dot, ret, saved_herrno;
    int got_nodata = 0, got_servfail = 0, root_on_list = 0;
    int tried_as_is = 0;
    int searched = 0;

    errno = 0;
    *herrno = HOST_NOT_FOUND; /* True if we never query. */

    dots = 0;
    for (cp = name; *cp != '\0'; cp++) dots += (*cp == '.');
    trailing_dot = 0;
    if (cp > name && *--cp == '.') trailing_dot++;

    /*
     * If there are enough dots in the name, let's just give it a
     * try 'as is'. The threshold can be set with the "ndots" option.
     * Also, query 'as is', if there is a trailing dot in the name.
     */
    saved_herrno = -1;
    if (dots >= statp->ndots || trailing_dot) {
        ret = res_nquerydomain(statp, name, NULL, cl, type, answer, anslen, herrno);
        if (ret > 0 || trailing_dot) return ret;
        saved_herrno = *herrno;
        tried_as_is++;
    }

    /*
     * We do at least one level of search if
     *	- there is no dot and RES_DEFNAME is set, or
     *	- there is at least one dot, there is no trailing dot,
     *	  and RES_DNSRCH is set.
     */
    if ((!dots && (statp->options & RES_DEFNAMES) != 0U) ||
        (dots && !trailing_dot && (statp->options & RES_DNSRCH) != 0U)) {
        int done = 0;

        /* Unfortunately we need to load network-specific info
         * (dns servers, search domains) before
         * the domain stuff is tried.  Will have a better
         * fix after thread pools are used as this will
         * be loaded once for the thread instead of each
         * time a query is tried.
         */
        _resolv_populate_res_for_net(statp);

        for (domain = (const char* const*) statp->dnsrch; *domain && !done; domain++) {
            searched = 1;

            if (domain[0][0] == '\0' || (domain[0][0] == '.' && domain[0][1] == '\0'))
                root_on_list++;

            ret = res_nquerydomain(statp, name, *domain, cl, type, answer, anslen, herrno);
            if (ret > 0) return ret;

            /*
             * If no server present, give up.
             * If name isn't found in this domain,
             * keep trying higher domains in the search list
             * (if that's enabled).
             * On a NO_DATA error, keep trying, otherwise
             * a wildcard entry of another type could keep us
             * from finding this entry higher in the domain.
             * If we get some other error (negative answer or
             * server failure), then stop searching up,
             * but try the input name below in case it's
             * fully-qualified.
             */
            if (errno == ECONNREFUSED) {
                *herrno = TRY_AGAIN;
                return -1;
            }

            switch (*herrno) {
                case NO_DATA:
                    got_nodata++;
                    break;
                case HOST_NOT_FOUND:
                    /* keep trying */
                    break;
                case TRY_AGAIN:
                    if (hp->rcode == SERVFAIL) {
                        /* try next search element, if any */
                        got_servfail++;
                        break;
                    }
                    [[fallthrough]];
                default:
                    /* anything else implies that we're done */
                    done++;
            }

            /* if we got here for some reason other than DNSRCH,
             * we only wanted one iteration of the loop, so stop.
             */
            if ((statp->options & RES_DNSRCH) == 0U) done++;
        }
    }

    /*
     * If the query has not already been tried as is then try it
     * unless RES_NOTLDQUERY is set and there were no dots.
     */
    if ((dots || !searched || (statp->options & RES_NOTLDQUERY) == 0U) &&
        !(tried_as_is || root_on_list)) {
        ret = res_nquerydomain(statp, name, NULL, cl, type, answer, anslen, herrno);
        if (ret > 0) return ret;
    }

    /* if we got here, we didn't satisfy the search.
     * if we did an initial full query, return that query's H_ERRNO
     * (note that we wouldn't be here if that query had succeeded).
     * else if we ever got a nodata, send that back as the reason.
     * else send back meaningless H_ERRNO, that being the one from
     * the last DNSRCH we did.
     */
    if (saved_herrno != -1)
        *herrno = saved_herrno;
    else if (got_nodata)
        *herrno = NO_DATA;
    else if (got_servfail)
        *herrno = TRY_AGAIN;
    return -1;
}

/*
 * Perform a call on res_query on the concatenation of name and domain,
 * removing a trailing dot from name if domain is NULL.
 */
int res_nquerydomain(res_state statp, const char* name, const char* domain, int cl,
                     int type,       /* class and type of query */
                     u_char* answer, /* buffer to put answer */
                     int anslen,     /* size of answer */
                     int* herrno)    /* legacy and extended h_errno NETD_RESOLV_H_ERRNO_EXT_* */
{
    char nbuf[MAXDNAME];
    const char* longname = nbuf;
    int n, d;

    if (domain == NULL) {
        LOG(DEBUG) << __func__ << ": (null, " << cl << ", " << type << ")";
        /*
         * Check for trailing '.';
         * copy without '.' if present.
         */
        n = strlen(name);
        if (n >= MAXDNAME) {
            *herrno = NO_RECOVERY;
            return -1;
        }
        n--;
        if (n >= 0 && name[n] == '.') {
            strncpy(nbuf, name, (size_t) n);
            nbuf[n] = '\0';
        } else
            longname = name;
    } else {
        LOG(DEBUG) << __func__ << ": (" << cl << ", " << type << ")";
        n = strlen(name);
        d = strlen(domain);
        if (n + d + 1 >= MAXDNAME) {
            *herrno = NO_RECOVERY;
            return -1;
        }
        snprintf(nbuf, sizeof(nbuf), "%s.%s", name, domain);
    }
    return res_nquery(statp, longname, cl, type, answer, anslen, herrno);
}
