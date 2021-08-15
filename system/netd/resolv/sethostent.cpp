/*	$NetBSD: sethostent.c,v 1.20 2014/03/17 13:24:23 christos Exp $	*/

/*
 * Copyright (c) 1985, 1993
 *	The Regents of the University of California.  All rights reserved.
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

#include <arpa/inet.h>
#include <arpa/nameser.h>
#include <assert.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <resolv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>

#include "hostent.h"
#include "resolv_private.h"

#define ALIGNBYTES (sizeof(uintptr_t) - 1)
#define ALIGN(p) (((uintptr_t)(p) + ALIGNBYTES) & ~ALIGNBYTES)

static void sethostent_r(FILE** hf) {
    if (!*hf)
        *hf = fopen(_PATH_HOSTS, "re");
    else
        rewind(*hf);
}

static void endhostent_r(FILE** hf) {
    if (*hf) {
        (void) fclose(*hf);
        *hf = NULL;
    }
}

// TODO: Consider returning a boolean result as files_getaddrinfo() does because the error code
// does not currently return to netd.
int _hf_gethtbyname2(const char* name, int af, getnamaddr* info) {
    struct hostent *hp, hent;
    char *buf, *ptr;
    size_t len, num, i;
    char* aliases[MAXALIASES];
    char* addr_ptrs[MAXADDRS];

    FILE* hf = NULL;
    sethostent_r(&hf);
    if (hf == NULL) {
        // TODO: Consider converting to a private extended EAI_* error code.
        // Currently, the EAI_* value has no corresponding error code for invalid argument socket
        // length. In order to not rely on errno, convert the original error code pair, EAI_SYSTEM
        // and EINVAL, to EAI_FAIL.
        return EAI_FAIL;
    }

    if ((ptr = buf = (char*) malloc(len = info->buflen)) == NULL) {
        return EAI_MEMORY;
    }

    hent.h_name = NULL;
    hent.h_addrtype = 0;
    hent.h_length = 0;

    size_t anum = 0;
    for (num = 0; num < MAXADDRS; /**/) {
        info->hp->h_addrtype = af;
        info->hp->h_length = 0;

        int he;
        hp = netbsd_gethostent_r(hf, info->hp, info->buf, info->buflen, &he);
        if (hp == NULL) {
            if (he == NETDB_INTERNAL && errno == ENOSPC) {
                goto nospc;  // glibc compatibility.
            }
            break;
        }

        if (strcasecmp(hp->h_name, name) != 0) {
            char** cp;
            for (cp = hp->h_aliases; *cp != NULL; cp++)
                if (strcasecmp(*cp, name) == 0) break;
            // NOTE: does not increment num
            if (*cp == NULL) continue;
        }

        if (num == 0) {
            hent.h_addrtype = hp->h_addrtype;
            hent.h_length = hp->h_length;

            HENT_SCOPY(hent.h_name, hp->h_name, ptr, len);
            for (anum = 0; hp->h_aliases[anum]; anum++) {
                if (anum >= MAXALIASES) goto nospc;
                HENT_SCOPY(aliases[anum], hp->h_aliases[anum], ptr, len);
            }
            ptr = (char*) ALIGN(ptr);
            if ((size_t)(ptr - buf) >= info->buflen) goto nospc;
        }

        if (num >= MAXADDRS) goto nospc;
        HENT_COPY(addr_ptrs[num], hp->h_addr_list[0], hp->h_length, ptr, len);

        num++;
    }
    endhostent_r(&hf);

    if (num == 0) {
        free(buf);
        // TODO: Perhaps convert HOST_NOT_FOUND to EAI_NONAME instead.
        // The original return error number is h_errno HOST_NOT_FOUND which was converted to
        // EAI_NODATA.
        return EAI_NODATA;
    }

    hp = info->hp;
    ptr = info->buf;
    len = info->buflen;

    hp->h_addrtype = hent.h_addrtype;
    hp->h_length = hent.h_length;

    HENT_ARRAY(hp->h_aliases, anum, ptr, len);
    HENT_ARRAY(hp->h_addr_list, num, ptr, len);

    for (i = 0; i < num; i++) {
        HENT_COPY(hp->h_addr_list[i], addr_ptrs[i], hp->h_length, ptr, len);

        // reserve space for mapping IPv4 address to IPv6 address in place
        if (hp->h_addrtype == AF_INET) {
            HENT_COPY(ptr, NAT64_PAD, sizeof(NAT64_PAD), ptr, len);
        }
    }
    hp->h_addr_list[num] = NULL;

    HENT_SCOPY(hp->h_name, hent.h_name, ptr, len);

    for (i = 0; i < anum; i++) {
        HENT_SCOPY(hp->h_aliases[i], aliases[i], ptr, len);
    }
    hp->h_aliases[anum] = NULL;

    free(buf);
    return 0;
nospc:
    free(buf);
    return EAI_MEMORY;
}

// TODO: Consider returning a boolean result as files_getaddrinfo() does because the error code
// does not currently return to netd.
int _hf_gethtbyaddr(const unsigned char* uaddr, int len, int af, getnamaddr* info) {
    info->hp->h_length = len;
    info->hp->h_addrtype = af;

    FILE* hf = NULL;
    sethostent_r(&hf);
    if (hf == NULL) {
        // TODO: Consider converting to a private extended EAI_* error code.
        // Currently, the EAI_* value has no corresponding error code for invalid argument socket
        // length. In order to not rely on errno, convert the original error code pair, EAI_SYSTEM
        // and EINVAL, to EAI_FAIL.
        return EAI_FAIL;
    }
    struct hostent* hp;
    int he;
    while ((hp = netbsd_gethostent_r(hf, info->hp, info->buf, info->buflen, &he)) != NULL)
        if (!memcmp(hp->h_addr_list[0], uaddr, (size_t) hp->h_length)) break;
    endhostent_r(&hf);

    if (hp == NULL) {
        if (errno == ENOSPC) return EAI_MEMORY;  // glibc compatibility.
        // TODO: Perhaps convert HOST_NOT_FOUND to EAI_NONAME instead.
        // The original return error number is h_errno HOST_NOT_FOUND which was converted to
        // EAI_NODATA.
        return EAI_NODATA;
    }
    return 0;
}
