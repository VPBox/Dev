/*
 * Copyright (C) 2008 The Android Open Source Project
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

#define LOG_TAG "res_cache"

#include "resolv_cache.h"

#include <resolv.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include <arpa/inet.h>
#include <arpa/nameser.h>
#include <errno.h>
#include <linux/if.h>
#include <net/if.h>
#include <netdb.h>

#include <android-base/logging.h>
#include <android-base/parseint.h>
#include <android-base/stringprintf.h>
#include <android-base/strings.h>
#include <android-base/thread_annotations.h>
#include <android/multinetwork.h>  // ResNsendFlags

#include <server_configurable_flags/get_flags.h>

#include "res_state_ext.h"
#include "resolv_private.h"

// NOTE: verbose logging MUST NOT be left enabled in production binaries.
// It floods logs at high rate, and can leak privacy-sensitive information.
constexpr bool kDumpData = false;

/* This code implements a small and *simple* DNS resolver cache.
 *
 * It is only used to cache DNS answers for a time defined by the smallest TTL
 * among the answer records in order to reduce DNS traffic. It is not supposed
 * to be a full DNS cache, since we plan to implement that in the future in a
 * dedicated process running on the system.
 *
 * Note that its design is kept simple very intentionally, i.e.:
 *
 *  - it takes raw DNS query packet data as input, and returns raw DNS
 *    answer packet data as output
 *
 *    (this means that two similar queries that encode the DNS name
 *     differently will be treated distinctly).
 *
 *    the smallest TTL value among the answer records are used as the time
 *    to keep an answer in the cache.
 *
 *    this is bad, but we absolutely want to avoid parsing the answer packets
 *    (and should be solved by the later full DNS cache process).
 *
 *  - the implementation is just a (query-data) => (answer-data) hash table
 *    with a trivial least-recently-used expiration policy.
 *
 * Doing this keeps the code simple and avoids to deal with a lot of things
 * that a full DNS cache is expected to do.
 *
 * The API is also very simple:
 *
 *   - the client calls _resolv_cache_get() to obtain a handle to the cache.
 *     this will initialize the cache on first usage. the result can be NULL
 *     if the cache is disabled.
 *
 *   - the client calls _resolv_cache_lookup() before performing a query
 *
 *     if the function returns RESOLV_CACHE_FOUND, a copy of the answer data
 *     has been copied into the client-provided answer buffer.
 *
 *     if the function returns RESOLV_CACHE_NOTFOUND, the client should perform
 *     a request normally, *then* call _resolv_cache_add() to add the received
 *     answer to the cache.
 *
 *     if the function returns RESOLV_CACHE_UNSUPPORTED, the client should
 *     perform a request normally, and *not* call _resolv_cache_add()
 *
 *     note that RESOLV_CACHE_UNSUPPORTED is also returned if the answer buffer
 *     is too short to accomodate the cached result.
 */

/* default number of entries kept in the cache. This value has been
 * determined by browsing through various sites and counting the number
 * of corresponding requests. Keep in mind that our framework is currently
 * performing two requests per name lookup (one for IPv4, the other for IPv6)
 *
 *    www.google.com      4
 *    www.ysearch.com     6
 *    www.amazon.com      8
 *    www.nytimes.com     22
 *    www.espn.com        28
 *    www.msn.com         28
 *    www.lemonde.fr      35
 *
 * (determined in 2009-2-17 from Paris, France, results may vary depending
 *  on location)
 *
 * most high-level websites use lots of media/ad servers with different names
 * but these are generally reused when browsing through the site.
 *
 * As such, a value of 64 should be relatively comfortable at the moment.
 *
 * ******************************************
 * * NOTE - this has changed.
 * * 1) we've added IPv6 support so each dns query results in 2 responses
 * * 2) we've made this a system-wide cache, so the cost is less (it's not
 * *    duplicated in each process) and the need is greater (more processes
 * *    making different requests).
 * * Upping by 2x for IPv6
 * * Upping by another 5x for the centralized nature
 * *****************************************
 */
#define CONFIG_MAX_ENTRIES (64 * 2 * 5)

/** BOUNDED BUFFER FORMATTING **/

/* technical note:
 *
 *   the following debugging routines are used to append data to a bounded
 *   buffer they take two parameters that are:
 *
 *   - p : a pointer to the current cursor position in the buffer
 *         this value is initially set to the buffer's address.
 *
 *   - end : the address of the buffer's limit, i.e. of the first byte
 *           after the buffer. this address should never be touched.
 *
 *           IMPORTANT: it is assumed that end > buffer_address, i.e.
 *                      that the buffer is at least one byte.
 *
 *   the bprint_x() functions return the new value of 'p' after the data
 *   has been appended, and also ensure the following:
 *
 *   - the returned value will never be strictly greater than 'end'
 *
 *   - a return value equal to 'end' means that truncation occurred
 *     (in which case, end[-1] will be set to 0)
 *
 *   - after returning from a bprint_x() function, the content of the buffer
 *     is always 0-terminated, even in the event of truncation.
 *
 *  these conventions allow you to call bprint_x() functions multiple times and
 *  only check for truncation at the end of the sequence, as in:
 *
 *     char  buff[1000], *p = buff, *end = p + sizeof(buff);
 *
 *     p = bprint_c(p, end, '"');
 *     p = bprint_s(p, end, my_string);
 *     p = bprint_c(p, end, '"');
 *
 *     if (p >= end) {
 *        // buffer was too small
 *     }
 *
 *     printf( "%s", buff );
 */

/* Defaults used for initializing res_params */

// If successes * 100 / total_samples is less than this value, the server is considered failing
#define SUCCESS_THRESHOLD 75
// Sample validity in seconds. Set to -1 to disable skipping failing servers.
#define NSSAMPLE_VALIDITY 1800

/* add a char to a bounded buffer */
static char* bprint_c(char* p, char* end, int c) {
    if (p < end) {
        if (p + 1 == end)
            *p++ = 0;
        else {
            *p++ = (char) c;
            *p = 0;
        }
    }
    return p;
}

/* add a sequence of bytes to a bounded buffer */
static char* bprint_b(char* p, char* end, const char* buf, int len) {
    int avail = end - p;

    if (avail <= 0 || len <= 0) return p;

    if (avail > len) avail = len;

    memcpy(p, buf, avail);
    p += avail;

    if (p < end)
        p[0] = 0;
    else
        end[-1] = 0;

    return p;
}

/* add a string to a bounded buffer */
static char* bprint_s(char* p, char* end, const char* str) {
    return bprint_b(p, end, str, strlen(str));
}

/* add a formatted string to a bounded buffer */
static char* bprint(char* p, char* end, const char* format, ...) {
    int avail, n;
    va_list args;

    avail = end - p;

    if (avail <= 0) return p;

    va_start(args, format);
    n = vsnprintf(p, avail, format, args);
    va_end(args);

    /* certain C libraries return -1 in case of truncation */
    if (n < 0 || n > avail) n = avail;

    p += n;
    /* certain C libraries do not zero-terminate in case of truncation */
    if (p == end) p[-1] = 0;

    return p;
}

/* add a hex value to a bounded buffer, up to 8 digits */
static char* bprint_hex(char* p, char* end, unsigned value, int numDigits) {
    char text[sizeof(unsigned) * 2];
    int nn = 0;

    while (numDigits-- > 0) {
        text[nn++] = "0123456789abcdef"[(value >> (numDigits * 4)) & 15];
    }
    return bprint_b(p, end, text, nn);
}

/* add the hexadecimal dump of some memory area to a bounded buffer */
static char* bprint_hexdump(char* p, char* end, const uint8_t* data, int datalen) {
    int lineSize = 16;

    while (datalen > 0) {
        int avail = datalen;
        int nn;

        if (avail > lineSize) avail = lineSize;

        for (nn = 0; nn < avail; nn++) {
            if (nn > 0) p = bprint_c(p, end, ' ');
            p = bprint_hex(p, end, data[nn], 2);
        }
        for (; nn < lineSize; nn++) {
            p = bprint_s(p, end, "   ");
        }
        p = bprint_s(p, end, "  ");

        for (nn = 0; nn < avail; nn++) {
            int c = data[nn];

            if (c < 32 || c > 127) c = '.';

            p = bprint_c(p, end, c);
        }
        p = bprint_c(p, end, '\n');

        data += avail;
        datalen -= avail;
    }
    return p;
}

/* dump the content of a query of packet to the log */
static void dump_bytes(const uint8_t* base, int len) {
    if (!kDumpData) return;

    char buff[1024];
    char *p = buff, *end = p + sizeof(buff);

    p = bprint_hexdump(p, end, base, len);
    LOG(INFO) << __func__ << ": " << buff;
}

static time_t _time_now(void) {
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return tv.tv_sec;
}

/* reminder: the general format of a DNS packet is the following:
 *
 *    HEADER  (12 bytes)
 *    QUESTION  (variable)
 *    ANSWER (variable)
 *    AUTHORITY (variable)
 *    ADDITIONNAL (variable)
 *
 * the HEADER is made of:
 *
 *   ID     : 16 : 16-bit unique query identification field
 *
 *   QR     :  1 : set to 0 for queries, and 1 for responses
 *   Opcode :  4 : set to 0 for queries
 *   AA     :  1 : set to 0 for queries
 *   TC     :  1 : truncation flag, will be set to 0 in queries
 *   RD     :  1 : recursion desired
 *
 *   RA     :  1 : recursion available (0 in queries)
 *   Z      :  3 : three reserved zero bits
 *   RCODE  :  4 : response code (always 0=NOERROR in queries)
 *
 *   QDCount: 16 : question count
 *   ANCount: 16 : Answer count (0 in queries)
 *   NSCount: 16: Authority Record count (0 in queries)
 *   ARCount: 16: Additionnal Record count (0 in queries)
 *
 * the QUESTION is made of QDCount Question Record (QRs)
 * the ANSWER is made of ANCount RRs
 * the AUTHORITY is made of NSCount RRs
 * the ADDITIONNAL is made of ARCount RRs
 *
 * Each Question Record (QR) is made of:
 *
 *   QNAME   : variable : Query DNS NAME
 *   TYPE    : 16       : type of query (A=1, PTR=12, MX=15, AAAA=28, ALL=255)
 *   CLASS   : 16       : class of query (IN=1)
 *
 * Each Resource Record (RR) is made of:
 *
 *   NAME    : variable : DNS NAME
 *   TYPE    : 16       : type of query (A=1, PTR=12, MX=15, AAAA=28, ALL=255)
 *   CLASS   : 16       : class of query (IN=1)
 *   TTL     : 32       : seconds to cache this RR (0=none)
 *   RDLENGTH: 16       : size of RDDATA in bytes
 *   RDDATA  : variable : RR data (depends on TYPE)
 *
 * Each QNAME contains a domain name encoded as a sequence of 'labels'
 * terminated by a zero. Each label has the following format:
 *
 *    LEN  : 8     : lenght of label (MUST be < 64)
 *    NAME : 8*LEN : label length (must exclude dots)
 *
 * A value of 0 in the encoding is interpreted as the 'root' domain and
 * terminates the encoding. So 'www.android.com' will be encoded as:
 *
 *   <3>www<7>android<3>com<0>
 *
 * Where <n> represents the byte with value 'n'
 *
 * Each NAME reflects the QNAME of the question, but has a slightly more
 * complex encoding in order to provide message compression. This is achieved
 * by using a 2-byte pointer, with format:
 *
 *    TYPE   : 2  : 0b11 to indicate a pointer, 0b01 and 0b10 are reserved
 *    OFFSET : 14 : offset to another part of the DNS packet
 *
 * The offset is relative to the start of the DNS packet and must point
 * A pointer terminates the encoding.
 *
 * The NAME can be encoded in one of the following formats:
 *
 *   - a sequence of simple labels terminated by 0 (like QNAMEs)
 *   - a single pointer
 *   - a sequence of simple labels terminated by a pointer
 *
 * A pointer shall always point to either a pointer of a sequence of
 * labels (which can themselves be terminated by either a 0 or a pointer)
 *
 * The expanded length of a given domain name should not exceed 255 bytes.
 *
 * NOTE: we don't parse the answer packets, so don't need to deal with NAME
 *       records, only QNAMEs.
 */

#define DNS_HEADER_SIZE 12

#define DNS_TYPE_A "\00\01"     /* big-endian decimal 1 */
#define DNS_TYPE_PTR "\00\014"  /* big-endian decimal 12 */
#define DNS_TYPE_MX "\00\017"   /* big-endian decimal 15 */
#define DNS_TYPE_AAAA "\00\034" /* big-endian decimal 28 */
#define DNS_TYPE_ALL "\00\0377" /* big-endian decimal 255 */

#define DNS_CLASS_IN "\00\01" /* big-endian decimal 1 */

typedef struct {
    const uint8_t* base;
    const uint8_t* end;
    const uint8_t* cursor;
} DnsPacket;

static void _dnsPacket_init(DnsPacket* packet, const uint8_t* buff, int bufflen) {
    packet->base = buff;
    packet->end = buff + bufflen;
    packet->cursor = buff;
}

static void _dnsPacket_rewind(DnsPacket* packet) {
    packet->cursor = packet->base;
}

static void _dnsPacket_skip(DnsPacket* packet, int count) {
    const uint8_t* p = packet->cursor + count;

    if (p > packet->end) p = packet->end;

    packet->cursor = p;
}

static int _dnsPacket_readInt16(DnsPacket* packet) {
    const uint8_t* p = packet->cursor;

    if (p + 2 > packet->end) return -1;

    packet->cursor = p + 2;
    return (p[0] << 8) | p[1];
}

/** QUERY CHECKING **/

/* check bytes in a dns packet. returns 1 on success, 0 on failure.
 * the cursor is only advanced in the case of success
 */
static int _dnsPacket_checkBytes(DnsPacket* packet, int numBytes, const void* bytes) {
    const uint8_t* p = packet->cursor;

    if (p + numBytes > packet->end) return 0;

    if (memcmp(p, bytes, numBytes) != 0) return 0;

    packet->cursor = p + numBytes;
    return 1;
}

/* parse and skip a given QNAME stored in a query packet,
 * from the current cursor position. returns 1 on success,
 * or 0 for malformed data.
 */
static int _dnsPacket_checkQName(DnsPacket* packet) {
    const uint8_t* p = packet->cursor;
    const uint8_t* end = packet->end;

    for (;;) {
        int c;

        if (p >= end) break;

        c = *p++;

        if (c == 0) {
            packet->cursor = p;
            return 1;
        }

        /* we don't expect label compression in QNAMEs */
        if (c >= 64) break;

        p += c;
        /* we rely on the bound check at the start
         * of the loop here */
    }
    /* malformed data */
    LOG(INFO) << __func__ << ": malformed QNAME";
    return 0;
}

/* parse and skip a given QR stored in a packet.
 * returns 1 on success, and 0 on failure
 */
static int _dnsPacket_checkQR(DnsPacket* packet) {
    if (!_dnsPacket_checkQName(packet)) return 0;

    /* TYPE must be one of the things we support */
    if (!_dnsPacket_checkBytes(packet, 2, DNS_TYPE_A) &&
        !_dnsPacket_checkBytes(packet, 2, DNS_TYPE_PTR) &&
        !_dnsPacket_checkBytes(packet, 2, DNS_TYPE_MX) &&
        !_dnsPacket_checkBytes(packet, 2, DNS_TYPE_AAAA) &&
        !_dnsPacket_checkBytes(packet, 2, DNS_TYPE_ALL)) {
        LOG(INFO) << __func__ << ": unsupported TYPE";
        return 0;
    }
    /* CLASS must be IN */
    if (!_dnsPacket_checkBytes(packet, 2, DNS_CLASS_IN)) {
        LOG(INFO) << __func__ << ": unsupported CLASS";
        return 0;
    }

    return 1;
}

/* check the header of a DNS Query packet, return 1 if it is one
 * type of query we can cache, or 0 otherwise
 */
static int _dnsPacket_checkQuery(DnsPacket* packet) {
    const uint8_t* p = packet->base;
    int qdCount, anCount, dnCount, arCount;

    if (p + DNS_HEADER_SIZE > packet->end) {
        LOG(INFO) << __func__ << ": query packet too small";
        return 0;
    }

    /* QR must be set to 0, opcode must be 0 and AA must be 0 */
    /* RA, Z, and RCODE must be 0 */
    if ((p[2] & 0xFC) != 0 || (p[3] & 0xCF) != 0) {
        LOG(INFO) << __func__ << ": query packet flags unsupported";
        return 0;
    }

    /* Note that we ignore the TC, RD, CD, and AD bits here for the
     * following reasons:
     *
     * - there is no point for a query packet sent to a server
     *   to have the TC bit set, but the implementation might
     *   set the bit in the query buffer for its own needs
     *   between a _resolv_cache_lookup and a
     *   _resolv_cache_add. We should not freak out if this
     *   is the case.
     *
     * - we consider that the result from a query might depend on
     *   the RD, AD, and CD bits, so these bits
     *   should be used to differentiate cached result.
     *
     *   this implies that these bits are checked when hashing or
     *   comparing query packets, but not TC
     */

    /* ANCOUNT, DNCOUNT and ARCOUNT must be 0 */
    qdCount = (p[4] << 8) | p[5];
    anCount = (p[6] << 8) | p[7];
    dnCount = (p[8] << 8) | p[9];
    arCount = (p[10] << 8) | p[11];

    if (anCount != 0 || dnCount != 0 || arCount > 1) {
        LOG(INFO) << __func__ << ": query packet contains non-query records";
        return 0;
    }

    if (qdCount == 0) {
        LOG(INFO) << __func__ << ": query packet doesn't contain query record";
        return 0;
    }

    /* Check QDCOUNT QRs */
    packet->cursor = p + DNS_HEADER_SIZE;

    for (; qdCount > 0; qdCount--)
        if (!_dnsPacket_checkQR(packet)) return 0;

    return 1;
}

/** QUERY DEBUGGING **/
static char* dnsPacket_bprintQName(DnsPacket* packet, char* bp, char* bend) {
    const uint8_t* p = packet->cursor;
    const uint8_t* end = packet->end;
    int first = 1;

    for (;;) {
        int c;

        if (p >= end) break;

        c = *p++;

        if (c == 0) {
            packet->cursor = p;
            return bp;
        }

        /* we don't expect label compression in QNAMEs */
        if (c >= 64) break;

        if (first)
            first = 0;
        else
            bp = bprint_c(bp, bend, '.');

        bp = bprint_b(bp, bend, (const char*) p, c);

        p += c;
        /* we rely on the bound check at the start
         * of the loop here */
    }
    /* malformed data */
    bp = bprint_s(bp, bend, "<MALFORMED>");
    return bp;
}

static char* dnsPacket_bprintQR(DnsPacket* packet, char* p, char* end) {
#define QQ(x) \
    { DNS_TYPE_##x, #x }
    static const struct {
        const char* typeBytes;
        const char* typeString;
    } qTypes[] = {QQ(A), QQ(PTR), QQ(MX), QQ(AAAA), QQ(ALL), {NULL, NULL}};
    int nn;
    const char* typeString = NULL;

    /* dump QNAME */
    p = dnsPacket_bprintQName(packet, p, end);

    /* dump TYPE */
    p = bprint_s(p, end, " (");

    for (nn = 0; qTypes[nn].typeBytes != NULL; nn++) {
        if (_dnsPacket_checkBytes(packet, 2, qTypes[nn].typeBytes)) {
            typeString = qTypes[nn].typeString;
            break;
        }
    }

    if (typeString != NULL)
        p = bprint_s(p, end, typeString);
    else {
        int typeCode = _dnsPacket_readInt16(packet);
        p = bprint(p, end, "UNKNOWN-%d", typeCode);
    }

    p = bprint_c(p, end, ')');

    /* skip CLASS */
    _dnsPacket_skip(packet, 2);
    return p;
}

/* this function assumes the packet has already been checked */
static char* dnsPacket_bprintQuery(DnsPacket* packet, char* p, char* end) {
    int qdCount;

    if (packet->base[2] & 0x1) {
        p = bprint_s(p, end, "RECURSIVE ");
    }

    _dnsPacket_skip(packet, 4);
    qdCount = _dnsPacket_readInt16(packet);
    _dnsPacket_skip(packet, 6);

    for (; qdCount > 0; qdCount--) {
        p = dnsPacket_bprintQR(packet, p, end);
    }
    return p;
}

/** QUERY HASHING SUPPORT
 **
 ** THE FOLLOWING CODE ASSUMES THAT THE INPUT PACKET HAS ALREADY
 ** BEEN SUCCESFULLY CHECKED.
 **/

/* use 32-bit FNV hash function */
#define FNV_MULT 16777619U
#define FNV_BASIS 2166136261U

static unsigned _dnsPacket_hashBytes(DnsPacket* packet, int numBytes, unsigned hash) {
    const uint8_t* p = packet->cursor;
    const uint8_t* end = packet->end;

    while (numBytes > 0 && p < end) {
        hash = hash * FNV_MULT ^ *p++;
    }
    packet->cursor = p;
    return hash;
}

static unsigned _dnsPacket_hashQName(DnsPacket* packet, unsigned hash) {
    const uint8_t* p = packet->cursor;
    const uint8_t* end = packet->end;

    for (;;) {
        int c;

        if (p >= end) { /* should not happen */
            LOG(INFO) << __func__ << ": INTERNAL_ERROR: read-overflow";
            break;
        }

        c = *p++;

        if (c == 0) break;

        if (c >= 64) {
            LOG(INFO) << __func__ << ": INTERNAL_ERROR: malformed domain";
            break;
        }
        if (p + c >= end) {
            LOG(INFO) << __func__ << ": INTERNAL_ERROR: simple label read-overflow";
            break;
        }
        while (c > 0) {
            hash = hash * FNV_MULT ^ *p++;
            c -= 1;
        }
    }
    packet->cursor = p;
    return hash;
}

static unsigned _dnsPacket_hashQR(DnsPacket* packet, unsigned hash) {
    hash = _dnsPacket_hashQName(packet, hash);
    hash = _dnsPacket_hashBytes(packet, 4, hash); /* TYPE and CLASS */
    return hash;
}

static unsigned _dnsPacket_hashRR(DnsPacket* packet, unsigned hash) {
    int rdlength;
    hash = _dnsPacket_hashQR(packet, hash);
    hash = _dnsPacket_hashBytes(packet, 4, hash); /* TTL */
    rdlength = _dnsPacket_readInt16(packet);
    hash = _dnsPacket_hashBytes(packet, rdlength, hash); /* RDATA */
    return hash;
}

static unsigned _dnsPacket_hashQuery(DnsPacket* packet) {
    unsigned hash = FNV_BASIS;
    int count, arcount;
    _dnsPacket_rewind(packet);

    /* ignore the ID */
    _dnsPacket_skip(packet, 2);

    /* we ignore the TC bit for reasons explained in
     * _dnsPacket_checkQuery().
     *
     * however we hash the RD bit to differentiate
     * between answers for recursive and non-recursive
     * queries.
     */
    hash = hash * FNV_MULT ^ (packet->base[2] & 1);

    /* mark the first header byte as processed */
    _dnsPacket_skip(packet, 1);

    /* process the second header byte */
    hash = _dnsPacket_hashBytes(packet, 1, hash);

    /* read QDCOUNT */
    count = _dnsPacket_readInt16(packet);

    /* assume: ANcount and NScount are 0 */
    _dnsPacket_skip(packet, 4);

    /* read ARCOUNT */
    arcount = _dnsPacket_readInt16(packet);

    /* hash QDCOUNT QRs */
    for (; count > 0; count--) hash = _dnsPacket_hashQR(packet, hash);

    /* hash ARCOUNT RRs */
    for (; arcount > 0; arcount--) hash = _dnsPacket_hashRR(packet, hash);

    return hash;
}

/** QUERY COMPARISON
 **
 ** THE FOLLOWING CODE ASSUMES THAT THE INPUT PACKETS HAVE ALREADY
 ** BEEN SUCCESSFULLY CHECKED.
 **/

static int _dnsPacket_isEqualDomainName(DnsPacket* pack1, DnsPacket* pack2) {
    const uint8_t* p1 = pack1->cursor;
    const uint8_t* end1 = pack1->end;
    const uint8_t* p2 = pack2->cursor;
    const uint8_t* end2 = pack2->end;

    for (;;) {
        int c1, c2;

        if (p1 >= end1 || p2 >= end2) {
            LOG(INFO) << __func__ << ": INTERNAL_ERROR: read-overflow";
            break;
        }
        c1 = *p1++;
        c2 = *p2++;
        if (c1 != c2) break;

        if (c1 == 0) {
            pack1->cursor = p1;
            pack2->cursor = p2;
            return 1;
        }
        if (c1 >= 64) {
            LOG(INFO) << __func__ << ": INTERNAL_ERROR: malformed domain";
            break;
        }
        if ((p1 + c1 > end1) || (p2 + c1 > end2)) {
            LOG(INFO) << __func__ << ": INTERNAL_ERROR: simple label read-overflow";
            break;
        }
        if (memcmp(p1, p2, c1) != 0) break;
        p1 += c1;
        p2 += c1;
        /* we rely on the bound checks at the start of the loop */
    }
    /* not the same, or one is malformed */
    LOG(INFO) << __func__ << ": different DN";
    return 0;
}

static int _dnsPacket_isEqualBytes(DnsPacket* pack1, DnsPacket* pack2, int numBytes) {
    const uint8_t* p1 = pack1->cursor;
    const uint8_t* p2 = pack2->cursor;

    if (p1 + numBytes > pack1->end || p2 + numBytes > pack2->end) return 0;

    if (memcmp(p1, p2, numBytes) != 0) return 0;

    pack1->cursor += numBytes;
    pack2->cursor += numBytes;
    return 1;
}

static int _dnsPacket_isEqualQR(DnsPacket* pack1, DnsPacket* pack2) {
    /* compare domain name encoding + TYPE + CLASS */
    if (!_dnsPacket_isEqualDomainName(pack1, pack2) ||
        !_dnsPacket_isEqualBytes(pack1, pack2, 2 + 2))
        return 0;

    return 1;
}

static int _dnsPacket_isEqualRR(DnsPacket* pack1, DnsPacket* pack2) {
    int rdlength1, rdlength2;
    /* compare query + TTL */
    if (!_dnsPacket_isEqualQR(pack1, pack2) || !_dnsPacket_isEqualBytes(pack1, pack2, 4)) return 0;

    /* compare RDATA */
    rdlength1 = _dnsPacket_readInt16(pack1);
    rdlength2 = _dnsPacket_readInt16(pack2);
    if (rdlength1 != rdlength2 || !_dnsPacket_isEqualBytes(pack1, pack2, rdlength1)) return 0;

    return 1;
}

static int _dnsPacket_isEqualQuery(DnsPacket* pack1, DnsPacket* pack2) {
    int count1, count2, arcount1, arcount2;

    /* compare the headers, ignore most fields */
    _dnsPacket_rewind(pack1);
    _dnsPacket_rewind(pack2);

    /* compare RD, ignore TC, see comment in _dnsPacket_checkQuery */
    if ((pack1->base[2] & 1) != (pack2->base[2] & 1)) {
        LOG(INFO) << __func__ << ": different RD";
        return 0;
    }

    if (pack1->base[3] != pack2->base[3]) {
        LOG(INFO) << __func__ << ": different CD or AD";
        return 0;
    }

    /* mark ID and header bytes as compared */
    _dnsPacket_skip(pack1, 4);
    _dnsPacket_skip(pack2, 4);

    /* compare QDCOUNT */
    count1 = _dnsPacket_readInt16(pack1);
    count2 = _dnsPacket_readInt16(pack2);
    if (count1 != count2 || count1 < 0) {
        LOG(INFO) << __func__ << ": different QDCOUNT";
        return 0;
    }

    /* assume: ANcount and NScount are 0 */
    _dnsPacket_skip(pack1, 4);
    _dnsPacket_skip(pack2, 4);

    /* compare ARCOUNT */
    arcount1 = _dnsPacket_readInt16(pack1);
    arcount2 = _dnsPacket_readInt16(pack2);
    if (arcount1 != arcount2 || arcount1 < 0) {
        LOG(INFO) << __func__ << ": different ARCOUNT";
        return 0;
    }

    /* compare the QDCOUNT QRs */
    for (; count1 > 0; count1--) {
        if (!_dnsPacket_isEqualQR(pack1, pack2)) {
            LOG(INFO) << __func__ << ": different QR";
            return 0;
        }
    }

    /* compare the ARCOUNT RRs */
    for (; arcount1 > 0; arcount1--) {
        if (!_dnsPacket_isEqualRR(pack1, pack2)) {
            LOG(INFO) << __func__ << ": different additional RR";
            return 0;
        }
    }
    return 1;
}

/* cache entry. for simplicity, 'hash' and 'hlink' are inlined in this
 * structure though they are conceptually part of the hash table.
 *
 * similarly, mru_next and mru_prev are part of the global MRU list
 */
typedef struct Entry {
    unsigned int hash;   /* hash value */
    struct Entry* hlink; /* next in collision chain */
    struct Entry* mru_prev;
    struct Entry* mru_next;

    const uint8_t* query;
    int querylen;
    const uint8_t* answer;
    int answerlen;
    time_t expires; /* time_t when the entry isn't valid any more */
    int id;         /* for debugging purpose */
} Entry;

/*
 * Find the TTL for a negative DNS result.  This is defined as the minimum
 * of the SOA records TTL and the MINIMUM-TTL field (RFC-2308).
 *
 * Return 0 if not found.
 */
static u_long answer_getNegativeTTL(ns_msg handle) {
    int n, nscount;
    u_long result = 0;
    ns_rr rr;

    nscount = ns_msg_count(handle, ns_s_ns);
    for (n = 0; n < nscount; n++) {
        if ((ns_parserr(&handle, ns_s_ns, n, &rr) == 0) && (ns_rr_type(rr) == ns_t_soa)) {
            const u_char* rdata = ns_rr_rdata(rr);          // find the data
            const u_char* edata = rdata + ns_rr_rdlen(rr);  // add the len to find the end
            int len;
            u_long ttl, rec_result = ns_rr_ttl(rr);

            // find the MINIMUM-TTL field from the blob of binary data for this record
            // skip the server name
            len = dn_skipname(rdata, edata);
            if (len == -1) continue;  // error skipping
            rdata += len;

            // skip the admin name
            len = dn_skipname(rdata, edata);
            if (len == -1) continue;  // error skipping
            rdata += len;

            if (edata - rdata != 5 * NS_INT32SZ) continue;
            // skip: serial number + refresh interval + retry interval + expiry
            rdata += NS_INT32SZ * 4;
            // finally read the MINIMUM TTL
            ttl = ntohl(*reinterpret_cast<const uint32_t*>(rdata));
            if (ttl < rec_result) {
                rec_result = ttl;
            }
            // Now that the record is read successfully, apply the new min TTL
            if (n == 0 || rec_result < result) {
                result = rec_result;
            }
        }
    }
    return result;
}

/*
 * Parse the answer records and find the appropriate
 * smallest TTL among the records.  This might be from
 * the answer records if found or from the SOA record
 * if it's a negative result.
 *
 * The returned TTL is the number of seconds to
 * keep the answer in the cache.
 *
 * In case of parse error zero (0) is returned which
 * indicates that the answer shall not be cached.
 */
static u_long answer_getTTL(const void* answer, int answerlen) {
    ns_msg handle;
    int ancount, n;
    u_long result, ttl;
    ns_rr rr;

    result = 0;
    if (ns_initparse((const uint8_t*) answer, answerlen, &handle) >= 0) {
        // get number of answer records
        ancount = ns_msg_count(handle, ns_s_an);

        if (ancount == 0) {
            // a response with no answers?  Cache this negative result.
            result = answer_getNegativeTTL(handle);
        } else {
            for (n = 0; n < ancount; n++) {
                if (ns_parserr(&handle, ns_s_an, n, &rr) == 0) {
                    ttl = ns_rr_ttl(rr);
                    if (n == 0 || ttl < result) {
                        result = ttl;
                    }
                } else {
                    PLOG(INFO) << __func__ << ": ns_parserr failed ancount no = " << n;
                }
            }
        }
    } else {
        PLOG(INFO) << __func__ << ": ns_initparse failed";
    }

    LOG(INFO) << __func__ << ": TTL = " << result;
    return result;
}

static void entry_free(Entry* e) {
    /* everything is allocated in a single memory block */
    if (e) {
        free(e);
    }
}

static void entry_mru_remove(Entry* e) {
    e->mru_prev->mru_next = e->mru_next;
    e->mru_next->mru_prev = e->mru_prev;
}

static void entry_mru_add(Entry* e, Entry* list) {
    Entry* first = list->mru_next;

    e->mru_next = first;
    e->mru_prev = list;

    list->mru_next = e;
    first->mru_prev = e;
}

/* compute the hash of a given entry, this is a hash of most
 * data in the query (key) */
static unsigned entry_hash(const Entry* e) {
    DnsPacket pack[1];

    _dnsPacket_init(pack, e->query, e->querylen);
    return _dnsPacket_hashQuery(pack);
}

/* initialize an Entry as a search key, this also checks the input query packet
 * returns 1 on success, or 0 in case of unsupported/malformed data */
static int entry_init_key(Entry* e, const void* query, int querylen) {
    DnsPacket pack[1];

    memset(e, 0, sizeof(*e));

    e->query = (const uint8_t*) query;
    e->querylen = querylen;
    e->hash = entry_hash(e);

    _dnsPacket_init(pack, e->query, e->querylen);

    return _dnsPacket_checkQuery(pack);
}

/* allocate a new entry as a cache node */
static Entry* entry_alloc(const Entry* init, const void* answer, int answerlen) {
    Entry* e;
    int size;

    size = sizeof(*e) + init->querylen + answerlen;
    e = (Entry*) calloc(size, 1);
    if (e == NULL) return e;

    e->hash = init->hash;
    e->query = (const uint8_t*) (e + 1);
    e->querylen = init->querylen;

    memcpy((char*) e->query, init->query, e->querylen);

    e->answer = e->query + e->querylen;
    e->answerlen = answerlen;

    memcpy((char*) e->answer, answer, e->answerlen);

    return e;
}

static int entry_equals(const Entry* e1, const Entry* e2) {
    DnsPacket pack1[1], pack2[1];

    if (e1->querylen != e2->querylen) {
        return 0;
    }
    _dnsPacket_init(pack1, e1->query, e1->querylen);
    _dnsPacket_init(pack2, e2->query, e2->querylen);

    return _dnsPacket_isEqualQuery(pack1, pack2);
}

/* We use a simple hash table with external collision lists
 * for simplicity, the hash-table fields 'hash' and 'hlink' are
 * inlined in the Entry structure.
 */

/* Maximum time for a thread to wait for an pending request */
constexpr int PENDING_REQUEST_TIMEOUT = 20;

typedef struct resolv_cache {
    int max_entries;
    int num_entries;
    Entry mru_list;
    int last_id;
    Entry* entries;
    struct pending_req_info {
        unsigned int hash;
        struct pending_req_info* next;
    } pending_requests;
} Cache;

struct resolv_cache_info {
    unsigned netid;
    Cache* cache;
    struct resolv_cache_info* next;
    int nscount;
    char* nameservers[MAXNS];
    struct addrinfo* nsaddrinfo[MAXNS];
    int revision_id;  // # times the nameservers have been replaced
    res_params params;
    struct res_stats nsstats[MAXNS];
    char defdname[MAXDNSRCHPATH];
    int dnsrch_offset[MAXDNSRCH + 1];  // offsets into defdname
    int wait_for_pending_req_timeout_count;
    // Map format: ReturnCode:rate_denom
    std::unordered_map<int, uint32_t> dns_event_subsampling_map;
};

// A helper class for the Clang Thread Safety Analysis to deal with
// std::unique_lock.
class SCOPED_CAPABILITY ScopedAssumeLocked {
  public:
    ScopedAssumeLocked(std::mutex& mutex) ACQUIRE(mutex) {}
    ~ScopedAssumeLocked() RELEASE() {}
};

// lock protecting everything in the resolve_cache_info structs (next ptr, etc)
static std::mutex cache_mutex;
static std::condition_variable cv;

/* gets cache associated with a network, or NULL if none exists */
static resolv_cache* find_named_cache_locked(unsigned netid) REQUIRES(cache_mutex);
static int resolv_create_cache_for_net_locked(unsigned netid) REQUIRES(cache_mutex);

static void cache_flush_pending_requests_locked(struct resolv_cache* cache) {
    resolv_cache::pending_req_info *ri, *tmp;
    if (!cache) return;

    ri = cache->pending_requests.next;

    while (ri) {
        tmp = ri;
        ri = ri->next;
        free(tmp);
    }

    cache->pending_requests.next = NULL;
    cv.notify_all();
}

// Return true - if there is a pending request in |cache| matching |key|.
// Return false - if no pending request is found matching the key. Optionally
//                link a new one if parameter append_if_not_found is true.
static bool cache_has_pending_request_locked(resolv_cache* cache, const Entry* key,
                                             bool append_if_not_found) {
    if (!cache || !key) return false;

    resolv_cache::pending_req_info* ri = cache->pending_requests.next;
    resolv_cache::pending_req_info* prev = &cache->pending_requests;
    while (ri) {
        if (ri->hash == key->hash) {
            return true;
        }
        prev = ri;
        ri = ri->next;
    }

    if (append_if_not_found) {
        ri = (resolv_cache::pending_req_info*)calloc(1, sizeof(resolv_cache::pending_req_info));
        if (ri) {
            ri->hash = key->hash;
            prev->next = ri;
        }
    }
    return false;
}

// Notify all threads that the cache entry |key| has become available
static void _cache_notify_waiting_tid_locked(struct resolv_cache* cache, const Entry* key) {
    if (!cache || !key) return;

    resolv_cache::pending_req_info* ri = cache->pending_requests.next;
    resolv_cache::pending_req_info* prev = &cache->pending_requests;
    while (ri) {
        if (ri->hash == key->hash) {
            // remove item from list and destroy
            prev->next = ri->next;
            free(ri);
            cv.notify_all();
            return;
        }
        prev = ri;
        ri = ri->next;
    }
}

void _resolv_cache_query_failed(unsigned netid, const void* query, int querylen, uint32_t flags) {
    // We should not notify with these flags.
    if (flags & (ANDROID_RESOLV_NO_CACHE_STORE | ANDROID_RESOLV_NO_CACHE_LOOKUP)) {
        return;
    }
    Entry key[1];
    Cache* cache;

    if (!entry_init_key(key, query, querylen)) return;

    std::lock_guard guard(cache_mutex);

    cache = find_named_cache_locked(netid);

    if (cache) {
        _cache_notify_waiting_tid_locked(cache, key);
    }
}

static void cache_flush_locked(Cache* cache) {
    int nn;

    for (nn = 0; nn < cache->max_entries; nn++) {
        Entry** pnode = (Entry**) &cache->entries[nn];

        while (*pnode != NULL) {
            Entry* node = *pnode;
            *pnode = node->hlink;
            entry_free(node);
        }
    }

    // flush pending request
    cache_flush_pending_requests_locked(cache);

    cache->mru_list.mru_next = cache->mru_list.mru_prev = &cache->mru_list;
    cache->num_entries = 0;
    cache->last_id = 0;

    LOG(INFO) << __func__ << ": *** DNS CACHE FLUSHED ***";
}

static resolv_cache* resolv_cache_create() {
    struct resolv_cache* cache;

    cache = (struct resolv_cache*) calloc(sizeof(*cache), 1);
    if (cache) {
        cache->max_entries = CONFIG_MAX_ENTRIES;
        cache->entries = (Entry*) calloc(sizeof(*cache->entries), cache->max_entries);
        if (cache->entries) {
            cache->mru_list.mru_prev = cache->mru_list.mru_next = &cache->mru_list;
            LOG(INFO) << __func__ << ": cache created";
        } else {
            free(cache);
            cache = NULL;
        }
    }
    return cache;
}

static void dump_query(const uint8_t* query, int querylen) {
    if (!WOULD_LOG(VERBOSE)) return;

    char temp[256], *p = temp, *end = p + sizeof(temp);
    DnsPacket pack[1];

    _dnsPacket_init(pack, query, querylen);
    p = dnsPacket_bprintQuery(pack, p, end);
    LOG(VERBOSE) << __func__ << ": " << temp;
}

static void cache_dump_mru(Cache* cache) {
    char temp[512], *p = temp, *end = p + sizeof(temp);
    Entry* e;

    p = bprint(temp, end, "MRU LIST (%2d): ", cache->num_entries);
    for (e = cache->mru_list.mru_next; e != &cache->mru_list; e = e->mru_next)
        p = bprint(p, end, " %d", e->id);

    LOG(INFO) << __func__ << ": " << temp;
}

/* This function tries to find a key within the hash table
 * In case of success, it will return a *pointer* to the hashed key.
 * In case of failure, it will return a *pointer* to NULL
 *
 * So, the caller must check '*result' to check for success/failure.
 *
 * The main idea is that the result can later be used directly in
 * calls to _resolv_cache_add or _resolv_cache_remove as the 'lookup'
 * parameter. This makes the code simpler and avoids re-searching
 * for the key position in the htable.
 *
 * The result of a lookup_p is only valid until you alter the hash
 * table.
 */
static Entry** _cache_lookup_p(Cache* cache, Entry* key) {
    int index = key->hash % cache->max_entries;
    Entry** pnode = (Entry**) &cache->entries[index];

    while (*pnode != NULL) {
        Entry* node = *pnode;

        if (node == NULL) break;

        if (node->hash == key->hash && entry_equals(node, key)) break;

        pnode = &node->hlink;
    }
    return pnode;
}

/* Add a new entry to the hash table. 'lookup' must be the
 * result of an immediate previous failed _lookup_p() call
 * (i.e. with *lookup == NULL), and 'e' is the pointer to the
 * newly created entry
 */
static void _cache_add_p(Cache* cache, Entry** lookup, Entry* e) {
    *lookup = e;
    e->id = ++cache->last_id;
    entry_mru_add(e, &cache->mru_list);
    cache->num_entries += 1;

    LOG(INFO) << __func__ << ": entry " << e->id << " added (count=" << cache->num_entries << ")";
}

/* Remove an existing entry from the hash table,
 * 'lookup' must be the result of an immediate previous
 * and succesful _lookup_p() call.
 */
static void _cache_remove_p(Cache* cache, Entry** lookup) {
    Entry* e = *lookup;

    LOG(INFO) << __func__ << ": entry " << e->id << " removed (count=" << cache->num_entries - 1
              << ")";

    entry_mru_remove(e);
    *lookup = e->hlink;
    entry_free(e);
    cache->num_entries -= 1;
}

/* Remove the oldest entry from the hash table.
 */
static void _cache_remove_oldest(Cache* cache) {
    Entry* oldest = cache->mru_list.mru_prev;
    Entry** lookup = _cache_lookup_p(cache, oldest);

    if (*lookup == NULL) { /* should not happen */
        LOG(INFO) << __func__ << ": OLDEST NOT IN HTABLE ?";
        return;
    }
    LOG(INFO) << __func__ << ": Cache full - removing oldest";
    dump_query(oldest->query, oldest->querylen);
    _cache_remove_p(cache, lookup);
}

/* Remove all expired entries from the hash table.
 */
static void _cache_remove_expired(Cache* cache) {
    Entry* e;
    time_t now = _time_now();

    for (e = cache->mru_list.mru_next; e != &cache->mru_list;) {
        // Entry is old, remove
        if (now >= e->expires) {
            Entry** lookup = _cache_lookup_p(cache, e);
            if (*lookup == NULL) { /* should not happen */
                LOG(INFO) << __func__ << ": ENTRY NOT IN HTABLE ?";
                return;
            }
            e = e->mru_next;
            _cache_remove_p(cache, lookup);
        } else {
            e = e->mru_next;
        }
    }
}

// gets a resolv_cache_info associated with a network, or NULL if not found
static resolv_cache_info* find_cache_info_locked(unsigned netid) REQUIRES(cache_mutex);

ResolvCacheStatus _resolv_cache_lookup(unsigned netid, const void* query, int querylen,
                                       void* answer, int answersize, int* answerlen,
                                       uint32_t flags) {
    // Skip cache lookup, return RESOLV_CACHE_NOTFOUND directly so that it is
    // possible to cache the answer of this query.
    // If ANDROID_RESOLV_NO_CACHE_STORE is set, return RESOLV_CACHE_SKIP to skip possible cache
    // storing.
    if (flags & ANDROID_RESOLV_NO_CACHE_LOOKUP) {
        return flags & ANDROID_RESOLV_NO_CACHE_STORE ? RESOLV_CACHE_SKIP : RESOLV_CACHE_NOTFOUND;
    }
    Entry key;
    Entry** lookup;
    Entry* e;
    time_t now;
    Cache* cache;

    LOG(INFO) << __func__ << ": lookup";
    dump_query((u_char*) query, querylen);

    /* we don't cache malformed queries */
    if (!entry_init_key(&key, query, querylen)) {
        LOG(INFO) << __func__ << ": unsupported query";
        return RESOLV_CACHE_UNSUPPORTED;
    }
    /* lookup cache */
    std::unique_lock lock(cache_mutex);
    ScopedAssumeLocked assume_lock(cache_mutex);
    cache = find_named_cache_locked(netid);
    if (cache == NULL) {
        return RESOLV_CACHE_UNSUPPORTED;
    }

    /* see the description of _lookup_p to understand this.
     * the function always return a non-NULL pointer.
     */
    lookup = _cache_lookup_p(cache, &key);
    e = *lookup;

    if (e == NULL) {
        LOG(INFO) << __func__ << ": NOT IN CACHE";
        // If it is no-cache-store mode, we won't wait for possible query.
        if (flags & ANDROID_RESOLV_NO_CACHE_STORE) {
            return RESOLV_CACHE_SKIP;
        }

        if (!cache_has_pending_request_locked(cache, &key, true)) {
            return RESOLV_CACHE_NOTFOUND;

        } else {
            LOG(INFO) << __func__ << ": Waiting for previous request";
            // wait until (1) timeout OR
            //            (2) cv is notified AND no pending request matching the |key|
            // (cv notifier should delete pending request before sending notification.)
            bool ret = cv.wait_for(lock, std::chrono::seconds(PENDING_REQUEST_TIMEOUT),
                                   [netid, &cache, &key]() REQUIRES(cache_mutex) {
                                       // Must update cache as it could have been deleted
                                       cache = find_named_cache_locked(netid);
                                       return !cache_has_pending_request_locked(cache, &key, false);
                                   });
            if (!cache) {
                return RESOLV_CACHE_NOTFOUND;
            }
            if (ret == false) {
                resolv_cache_info* info = find_cache_info_locked(netid);
                if (info != NULL) {
                    info->wait_for_pending_req_timeout_count++;
                }
            }
            lookup = _cache_lookup_p(cache, &key);
            e = *lookup;
            if (e == NULL) {
                return RESOLV_CACHE_NOTFOUND;
            }
        }
    }

    now = _time_now();

    /* remove stale entries here */
    if (now >= e->expires) {
        LOG(INFO) << __func__ << ": NOT IN CACHE (STALE ENTRY " << *lookup << "DISCARDED)";
        dump_query(e->query, e->querylen);
        _cache_remove_p(cache, lookup);
        return RESOLV_CACHE_NOTFOUND;
    }

    *answerlen = e->answerlen;
    if (e->answerlen > answersize) {
        /* NOTE: we return UNSUPPORTED if the answer buffer is too short */
        LOG(INFO) << __func__ << ": ANSWER TOO LONG";
        return RESOLV_CACHE_UNSUPPORTED;
    }

    memcpy(answer, e->answer, e->answerlen);

    /* bump up this entry to the top of the MRU list */
    if (e != cache->mru_list.mru_next) {
        entry_mru_remove(e);
        entry_mru_add(e, &cache->mru_list);
    }

    LOG(INFO) << __func__ << ": FOUND IN CACHE entry=" << e;
    return RESOLV_CACHE_FOUND;
}

void _resolv_cache_add(unsigned netid, const void* query, int querylen, const void* answer,
                       int answerlen) {
    Entry key[1];
    Entry* e;
    Entry** lookup;
    u_long ttl;
    Cache* cache = NULL;

    /* don't assume that the query has already been cached
     */
    if (!entry_init_key(key, query, querylen)) {
        LOG(INFO) << __func__ << ": passed invalid query?";
        return;
    }

    std::lock_guard guard(cache_mutex);

    cache = find_named_cache_locked(netid);
    if (cache == NULL) {
        return;
    }

    LOG(INFO) << __func__ << ": query:";
    dump_query((u_char*)query, querylen);
    res_pquery((u_char*)answer, answerlen);
    if (kDumpData) {
        LOG(INFO) << __func__ << ": answer:";
        dump_bytes((u_char*)answer, answerlen);
    }

    lookup = _cache_lookup_p(cache, key);
    e = *lookup;

    // Should only happen on ANDROID_RESOLV_NO_CACHE_LOOKUP
    if (e != NULL) {
        LOG(INFO) << __func__ << ": ALREADY IN CACHE (" << e << ") ? IGNORING ADD";
        _cache_notify_waiting_tid_locked(cache, key);
        return;
    }

    if (cache->num_entries >= cache->max_entries) {
        _cache_remove_expired(cache);
        if (cache->num_entries >= cache->max_entries) {
            _cache_remove_oldest(cache);
        }
        // TODO: It looks useless, remove below code after having test to prove it.
        lookup = _cache_lookup_p(cache, key);
        e = *lookup;
        if (e != NULL) {
            LOG(INFO) << __func__ << ": ALREADY IN CACHE (" << e << ") ? IGNORING ADD";
            _cache_notify_waiting_tid_locked(cache, key);
            return;
        }
    }

    ttl = answer_getTTL(answer, answerlen);
    if (ttl > 0) {
        e = entry_alloc(key, answer, answerlen);
        if (e != NULL) {
            e->expires = ttl + _time_now();
            _cache_add_p(cache, lookup, e);
        }
    }

    cache_dump_mru(cache);
    _cache_notify_waiting_tid_locked(cache, key);
}

// Head of the list of caches.
static struct resolv_cache_info res_cache_list GUARDED_BY(cache_mutex);

// insert resolv_cache_info into the list of resolv_cache_infos
static void insert_cache_info_locked(resolv_cache_info* cache_info);
// creates a resolv_cache_info
static resolv_cache_info* create_cache_info();
// empty the nameservers set for the named cache
static void free_nameservers_locked(resolv_cache_info* cache_info);
// return 1 if the provided list of name servers differs from the list of name servers
// currently attached to the provided cache_info
static int resolv_is_nameservers_equal_locked(resolv_cache_info* cache_info, const char** servers,
                                              int numservers);
// clears the stats samples contained withing the given cache_info
static void res_cache_clear_stats_locked(resolv_cache_info* cache_info);

// public API for netd to query if name server is set on specific netid
bool resolv_has_nameservers(unsigned netid) {
    std::lock_guard guard(cache_mutex);
    resolv_cache_info* info = find_cache_info_locked(netid);
    return (info != nullptr) && (info->nscount > 0);
}

namespace {

// Map format: ReturnCode:rate_denom
// if the ReturnCode is not associated with any rate_denom, use default
// Sampling rate varies by return code; events to log are chosen randomly, with a
// probability proportional to the sampling rate.
constexpr const char DEFAULT_SUBSAMPLING_MAP[] = "default:1 0:100 7:10";

std::unordered_map<int, uint32_t> resolv_get_dns_event_subsampling_map() {
    using android::base::ParseInt;
    using android::base::ParseUint;
    using android::base::Split;
    using server_configurable_flags::GetServerConfigurableFlag;
    std::unordered_map<int, uint32_t> sampling_rate_map{};
    std::vector<std::string> subsampling_vector =
            Split(GetServerConfigurableFlag("netd_native", "dns_event_subsample_map",
                                            DEFAULT_SUBSAMPLING_MAP),
                  " ");
    for (const auto& pair : subsampling_vector) {
        std::vector<std::string> rate_denom = Split(pair, ":");
        int return_code;
        uint32_t denom;
        if (rate_denom.size() != 2) {
            LOG(ERROR) << __func__ << ": invalid subsampling_pair = " << pair;
            continue;
        }
        if (rate_denom[0] == "default") {
            return_code = DNSEVENT_SUBSAMPLING_MAP_DEFAULT_KEY;
        } else if (!ParseInt(rate_denom[0], &return_code)) {
            LOG(ERROR) << __func__ << ": parse subsampling_pair failed = " << pair;
            continue;
        }
        if (!ParseUint(rate_denom[1], &denom)) {
            LOG(ERROR) << __func__ << ": parse subsampling_pair failed = " << pair;
            continue;
        }
        sampling_rate_map[return_code] = denom;
    }
    return sampling_rate_map;
}

}  // namespace

static int resolv_create_cache_for_net_locked(unsigned netid) {
    resolv_cache* cache = find_named_cache_locked(netid);
    // Should not happen
    if (cache) {
        LOG(ERROR) << __func__ << ": Cache is already created, netId: " << netid;
        return -EEXIST;
    }

    resolv_cache_info* cache_info = create_cache_info();
    if (!cache_info) return -ENOMEM;
    cache = resolv_cache_create();
    if (!cache) {
        free(cache_info);
        return -ENOMEM;
    }
    cache_info->cache = cache;
    cache_info->netid = netid;
    cache_info->dns_event_subsampling_map = resolv_get_dns_event_subsampling_map();
    insert_cache_info_locked(cache_info);

    return 0;
}

int resolv_create_cache_for_net(unsigned netid) {
    std::lock_guard guard(cache_mutex);
    return resolv_create_cache_for_net_locked(netid);
}

void resolv_delete_cache_for_net(unsigned netid) {
    std::lock_guard guard(cache_mutex);

    struct resolv_cache_info* prev_cache_info = &res_cache_list;

    while (prev_cache_info->next) {
        struct resolv_cache_info* cache_info = prev_cache_info->next;

        if (cache_info->netid == netid) {
            prev_cache_info->next = cache_info->next;
            cache_flush_locked(cache_info->cache);
            free(cache_info->cache->entries);
            free(cache_info->cache);
            free_nameservers_locked(cache_info);
            free(cache_info);
            break;
        }

        prev_cache_info = prev_cache_info->next;
    }
}

std::vector<unsigned> resolv_list_caches() {
    std::lock_guard guard(cache_mutex);
    struct resolv_cache_info* cache_info = res_cache_list.next;
    std::vector<unsigned> result;
    while (cache_info) {
        result.push_back(cache_info->netid);
        cache_info = cache_info->next;
    }
    return result;
}

static resolv_cache_info* create_cache_info() {
    return (struct resolv_cache_info*) calloc(sizeof(struct resolv_cache_info), 1);
}

// TODO: convert this to a simple and efficient C++ container.
static void insert_cache_info_locked(struct resolv_cache_info* cache_info) {
    struct resolv_cache_info* last;
    for (last = &res_cache_list; last->next; last = last->next) {}
    last->next = cache_info;
}

static resolv_cache* find_named_cache_locked(unsigned netid) {
    resolv_cache_info* info = find_cache_info_locked(netid);
    if (info != NULL) return info->cache;
    return NULL;
}

static resolv_cache_info* find_cache_info_locked(unsigned netid) {
    struct resolv_cache_info* cache_info = res_cache_list.next;

    while (cache_info) {
        if (cache_info->netid == netid) {
            break;
        }

        cache_info = cache_info->next;
    }
    return cache_info;
}

static void resolv_set_default_params(res_params* params) {
    params->sample_validity = NSSAMPLE_VALIDITY;
    params->success_threshold = SUCCESS_THRESHOLD;
    params->min_samples = 0;
    params->max_samples = 0;
    params->base_timeout_msec = 0;  // 0 = legacy algorithm
    params->retry_count = 0;
}

static void resolv_set_experiment_params(res_params* params) {
    using android::base::ParseInt;
    using server_configurable_flags::GetServerConfigurableFlag;

    if (params->retry_count == 0) {
        params->retry_count = RES_DFLRETRY;
        ParseInt(GetServerConfigurableFlag("netd_native", "retry_count", ""), &params->retry_count);
    }

    if (params->base_timeout_msec == 0) {
        params->base_timeout_msec = RES_TIMEOUT;
        ParseInt(GetServerConfigurableFlag("netd_native", "retransmission_time_interval", ""),
                 &params->base_timeout_msec);
    }
}

int resolv_set_nameservers_for_net(unsigned netid, const char** servers, const int numservers,
                                   const char* domains, const res_params* params) {
    char* cp;
    int* offset;
    struct addrinfo* nsaddrinfo[MAXNS];

    if (numservers > MAXNS) {
        LOG(ERROR) << __func__ << ": numservers=" << numservers << ", MAXNS=" << MAXNS;
        return E2BIG;
    }

    // Parse the addresses before actually locking or changing any state, in case there is an error.
    // As a side effect this also reduces the time the lock is kept.
    char sbuf[NI_MAXSERV];
    snprintf(sbuf, sizeof(sbuf), "%u", NAMESERVER_PORT);
    for (int i = 0; i < numservers; i++) {
        // The addrinfo structures allocated here are freed in free_nameservers_locked().
        const addrinfo hints = {
                .ai_family = AF_UNSPEC, .ai_socktype = SOCK_DGRAM, .ai_flags = AI_NUMERICHOST};
        int rt = getaddrinfo_numeric(servers[i], sbuf, hints, &nsaddrinfo[i]);
        if (rt != 0) {
            for (int j = 0; j < i; j++) {
                freeaddrinfo(nsaddrinfo[j]);
            }
            LOG(INFO) << __func__ << ": getaddrinfo_numeric(" << servers[i]
                      << ") = " << gai_strerror(rt);
            return EINVAL;
        }
    }

    std::lock_guard guard(cache_mutex);

    resolv_cache_info* cache_info = find_cache_info_locked(netid);

    if (cache_info == NULL) return ENONET;

    uint8_t old_max_samples = cache_info->params.max_samples;
    if (params != NULL) {
        cache_info->params = *params;
    } else {
        resolv_set_default_params(&cache_info->params);
    }
    resolv_set_experiment_params(&cache_info->params);
    if (!resolv_is_nameservers_equal_locked(cache_info, servers, numservers)) {
        // free current before adding new
        free_nameservers_locked(cache_info);
        for (int i = 0; i < numservers; i++) {
            cache_info->nsaddrinfo[i] = nsaddrinfo[i];
            cache_info->nameservers[i] = strdup(servers[i]);
            LOG(INFO) << __func__ << ": netid = " << netid << ", addr = " << servers[i];
        }
        cache_info->nscount = numservers;

        // Clear the NS statistics because the mapping to nameservers might have changed.
        res_cache_clear_stats_locked(cache_info);

        // increment the revision id to ensure that sample state is not written back if the
        // servers change; in theory it would suffice to do so only if the servers or
        // max_samples actually change, in practice the overhead of checking is higher than the
        // cost, and overflows are unlikely
        ++cache_info->revision_id;
    } else {
        if (cache_info->params.max_samples != old_max_samples) {
            // If the maximum number of samples changes, the overhead of keeping the most recent
            // samples around is not considered worth the effort, so they are cleared instead.
            // All other parameters do not affect shared state: Changing these parameters does
            // not invalidate the samples, as they only affect aggregation and the conditions
            // under which servers are considered usable.
            res_cache_clear_stats_locked(cache_info);
            ++cache_info->revision_id;
        }
        for (int j = 0; j < numservers; j++) {
            freeaddrinfo(nsaddrinfo[j]);
        }
    }

    // Always update the search paths, since determining whether they actually changed is
    // complex due to the zero-padding, and probably not worth the effort. Cache-flushing
    // however is not necessary, since the stored cache entries do contain the domain, not
    // just the host name.
    strlcpy(cache_info->defdname, domains, sizeof(cache_info->defdname));
    if ((cp = strchr(cache_info->defdname, '\n')) != NULL) *cp = '\0';
    LOG(INFO) << __func__ << ": domains=\"" << cache_info->defdname << "\"";

    cp = cache_info->defdname;
    offset = cache_info->dnsrch_offset;
    while (offset < cache_info->dnsrch_offset + MAXDNSRCH) {
        while (*cp == ' ' || *cp == '\t') /* skip leading white space */
            cp++;
        if (*cp == '\0') /* stop if nothing more to do */
            break;
        *offset++ = cp - cache_info->defdname; /* record this search domain */
        while (*cp) {                          /* zero-terminate it */
            if (*cp == ' ' || *cp == '\t') {
                *cp++ = '\0';
                break;
            }
            cp++;
        }
    }
    *offset = -1; /* cache_info->dnsrch_offset has MAXDNSRCH+1 items */

    return 0;
}

static int resolv_is_nameservers_equal_locked(resolv_cache_info* cache_info, const char** servers,
                                              int numservers) {
    if (cache_info->nscount != numservers) {
        return 0;
    }

    // Compare each name server against current name servers.
    // TODO: this is incorrect if the list of current or previous nameservers
    // contains duplicates. This does not really matter because the framework
    // filters out duplicates, but we should probably fix it. It's also
    // insensitive to the order of the nameservers; we should probably fix that
    // too.
    for (int i = 0; i < numservers; i++) {
        for (int j = 0;; j++) {
            if (j >= numservers) {
                return 0;
            }
            if (strcmp(cache_info->nameservers[i], servers[j]) == 0) {
                break;
            }
        }
    }

    return 1;
}

static void free_nameservers_locked(resolv_cache_info* cache_info) {
    int i;
    for (i = 0; i < cache_info->nscount; i++) {
        free(cache_info->nameservers[i]);
        cache_info->nameservers[i] = NULL;
        if (cache_info->nsaddrinfo[i] != NULL) {
            freeaddrinfo(cache_info->nsaddrinfo[i]);
            cache_info->nsaddrinfo[i] = NULL;
        }
        cache_info->nsstats[i].sample_count = cache_info->nsstats[i].sample_next = 0;
    }
    cache_info->nscount = 0;
    res_cache_clear_stats_locked(cache_info);
    ++cache_info->revision_id;
}

void _resolv_populate_res_for_net(res_state statp) {
    if (statp == NULL) {
        return;
    }
    LOG(INFO) << __func__ << ": netid=" << statp->netid;

    std::lock_guard guard(cache_mutex);
    resolv_cache_info* info = find_cache_info_locked(statp->netid);
    if (info != NULL) {
        int nserv;
        struct addrinfo* ai;
        for (nserv = 0; nserv < MAXNS; nserv++) {
            ai = info->nsaddrinfo[nserv];
            if (ai == NULL) {
                break;
            }

            if ((size_t) ai->ai_addrlen <= sizeof(statp->_u._ext.ext->nsaddrs[0])) {
                if (statp->_u._ext.ext != NULL) {
                    memcpy(&statp->_u._ext.ext->nsaddrs[nserv], ai->ai_addr, ai->ai_addrlen);
                    statp->nsaddr_list[nserv].sin_family = AF_UNSPEC;
                } else {
                    if ((size_t) ai->ai_addrlen <= sizeof(statp->nsaddr_list[0])) {
                        memcpy(&statp->nsaddr_list[nserv], ai->ai_addr, ai->ai_addrlen);
                    } else {
                        statp->nsaddr_list[nserv].sin_family = AF_UNSPEC;
                    }
                }
            } else {
                LOG(INFO) << __func__ << ": found too long addrlen";
            }
        }
        statp->nscount = nserv;
        // now do search domains.  Note that we cache the offsets as this code runs alot
        // but the setting/offset-computer only runs when set/changed
        // WARNING: Don't use str*cpy() here, this string contains zeroes.
        memcpy(statp->defdname, info->defdname, sizeof(statp->defdname));
        char** pp = statp->dnsrch;
        int* p = info->dnsrch_offset;
        while (pp < statp->dnsrch + MAXDNSRCH && *p != -1) {
            *pp++ = &statp->defdname[0] + *p++;
        }
    }
}

/* Resolver reachability statistics. */

static void _res_cache_add_stats_sample_locked(res_stats* stats, const res_sample* sample,
                                               int max_samples) {
    // Note: This function expects max_samples > 0, otherwise a (harmless) modification of the
    // allocated but supposedly unused memory for samples[0] will happen
    LOG(INFO) << __func__ << ": adding sample to stats, next = " << unsigned(stats->sample_next)
              << ", count = " << unsigned(stats->sample_count);
    stats->samples[stats->sample_next] = *sample;
    if (stats->sample_count < max_samples) {
        ++stats->sample_count;
    }
    if (++stats->sample_next >= max_samples) {
        stats->sample_next = 0;
    }
}

static void res_cache_clear_stats_locked(resolv_cache_info* cache_info) {
    if (cache_info) {
        for (int i = 0; i < MAXNS; ++i) {
            cache_info->nsstats->sample_count = cache_info->nsstats->sample_next = 0;
        }
    }
}

int android_net_res_stats_get_info_for_net(unsigned netid, int* nscount,
                                           struct sockaddr_storage servers[MAXNS], int* dcount,
                                           char domains[MAXDNSRCH][MAXDNSRCHPATH],
                                           res_params* params, struct res_stats stats[MAXNS],
                                           int* wait_for_pending_req_timeout_count) {
    int revision_id = -1;
    std::lock_guard guard(cache_mutex);

    resolv_cache_info* info = find_cache_info_locked(netid);
    if (info) {
        if (info->nscount > MAXNS) {
            LOG(INFO) << __func__ << ": nscount " << info->nscount << " > MAXNS " << MAXNS;
            errno = EFAULT;
            return -1;
        }
        int i;
        for (i = 0; i < info->nscount; i++) {
            // Verify that the following assumptions are held, failure indicates corruption:
            //  - getaddrinfo() may never return a sockaddr > sockaddr_storage
            //  - all addresses are valid
            //  - there is only one address per addrinfo thanks to numeric resolution
            int addrlen = info->nsaddrinfo[i]->ai_addrlen;
            if (addrlen < (int) sizeof(struct sockaddr) || addrlen > (int) sizeof(servers[0])) {
                LOG(INFO) << __func__ << ": nsaddrinfo[" << i << "].ai_addrlen == " << addrlen;
                errno = EMSGSIZE;
                return -1;
            }
            if (info->nsaddrinfo[i]->ai_addr == NULL) {
                LOG(INFO) << __func__ << ": nsaddrinfo[" << i << "].ai_addr == NULL";
                errno = ENOENT;
                return -1;
            }
            if (info->nsaddrinfo[i]->ai_next != NULL) {
                LOG(INFO) << __func__ << ": nsaddrinfo[" << i << "].ai_next != NULL";
                errno = ENOTUNIQ;
                return -1;
            }
        }
        *nscount = info->nscount;
        for (i = 0; i < info->nscount; i++) {
            memcpy(&servers[i], info->nsaddrinfo[i]->ai_addr, info->nsaddrinfo[i]->ai_addrlen);
            stats[i] = info->nsstats[i];
        }
        for (i = 0; i < MAXDNSRCH; i++) {
            const char* cur_domain = info->defdname + info->dnsrch_offset[i];
            // dnsrch_offset[i] can either be -1 or point to an empty string to indicate the end
            // of the search offsets. Checking for < 0 is not strictly necessary, but safer.
            // TODO: Pass in a search domain array instead of a string to
            // resolv_set_nameservers_for_net() and make this double check unnecessary.
            if (info->dnsrch_offset[i] < 0 ||
                ((size_t) info->dnsrch_offset[i]) >= sizeof(info->defdname) || !cur_domain[0]) {
                break;
            }
            strlcpy(domains[i], cur_domain, MAXDNSRCHPATH);
        }
        *dcount = i;
        *params = info->params;
        revision_id = info->revision_id;
        *wait_for_pending_req_timeout_count = info->wait_for_pending_req_timeout_count;
    }

    return revision_id;
}

std::vector<std::string> resolv_cache_dump_subsampling_map(unsigned netid) {
    using android::base::StringPrintf;
    std::lock_guard guard(cache_mutex);
    resolv_cache_info* cache_info = find_cache_info_locked(netid);
    if (cache_info == nullptr) return {};
    std::vector<std::string> result;
    for (const auto& pair : cache_info->dns_event_subsampling_map) {
        result.push_back(StringPrintf("%s:%d",
                                      (pair.first == DNSEVENT_SUBSAMPLING_MAP_DEFAULT_KEY)
                                              ? "default"
                                              : std::to_string(pair.first).c_str(),
                                      pair.second));
    }
    return result;
}

// Decides whether an event should be sampled using a random number generator and
// a sampling factor derived from the netid and the return code.
//
// Returns the subsampling rate if the event should be sampled, or 0 if it should be discarded.
uint32_t resolv_cache_get_subsampling_denom(unsigned netid, int return_code) {
    std::lock_guard guard(cache_mutex);
    resolv_cache_info* cache_info = find_cache_info_locked(netid);
    if (cache_info == nullptr) return 0;  // Don't log anything at all.
    const auto& subsampling_map = cache_info->dns_event_subsampling_map;
    auto search_returnCode = subsampling_map.find(return_code);
    uint32_t denom;
    if (search_returnCode != subsampling_map.end()) {
        denom = search_returnCode->second;
    } else {
        auto search_default = subsampling_map.find(DNSEVENT_SUBSAMPLING_MAP_DEFAULT_KEY);
        denom = (search_default == subsampling_map.end()) ? 0 : search_default->second;
    }
    return denom;
}

int resolv_cache_get_resolver_stats(unsigned netid, res_params* params, res_stats stats[MAXNS]) {
    std::lock_guard guard(cache_mutex);
    resolv_cache_info* info = find_cache_info_locked(netid);
    if (info) {
        memcpy(stats, info->nsstats, sizeof(info->nsstats));
        *params = info->params;
        return info->revision_id;
    }

    return -1;
}

void _resolv_cache_add_resolver_stats_sample(unsigned netid, int revision_id, int ns,
                                             const res_sample* sample, int max_samples) {
    if (max_samples <= 0) return;

    std::lock_guard guard(cache_mutex);
    resolv_cache_info* info = find_cache_info_locked(netid);

    if (info && info->revision_id == revision_id) {
        _res_cache_add_stats_sample_locked(&info->nsstats[ns], sample, max_samples);
    }
}
