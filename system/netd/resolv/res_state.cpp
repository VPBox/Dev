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

#define LOG_TAG "res_state"

#include <arpa/inet.h>
#include <arpa/nameser.h>
#include <netdb.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h> /* for gettid() */

#include <android-base/logging.h>

#include "resolv_cache.h"
#include "resolv_private.h"

typedef struct {
    // TODO: Have one __res_state per network so we don't have to repopulate frequently.
    struct __res_state _nres[1];
    struct res_static _rstatic[1];
} _res_thread;

static _res_thread* res_thread_alloc(void) {
    _res_thread* rt = (_res_thread*) calloc(1, sizeof(*rt));

    if (rt) {
        memset(rt->_rstatic, 0, sizeof rt->_rstatic);
    }
    return rt;
}

static void res_static_done(struct res_static* rs) {
    /* fortunately, there is nothing to do here, since the
     * points in h_addr_ptrs and host_aliases should all
     * point to 'hostbuf'
     */
    if (rs->hostf) { /* should not happen in theory, but just be safe */
        fclose(rs->hostf);
        rs->hostf = NULL;
    }
    free(rs->servent.s_aliases);
}

static void res_thread_free(void* _rt) {
    _res_thread* rt = (_res_thread*) _rt;

    LOG(VERBOSE) << __func__ << ": rt=" << rt << " for thread=" << gettid();

    res_static_done(rt->_rstatic);
    res_ndestroy(rt->_nres);
    free(rt);
}

static pthread_key_t _res_key;

__attribute__((constructor)) static void __res_key_init() {
    pthread_key_create(&_res_key, res_thread_free);
}

static _res_thread* res_thread_get(void) {
    _res_thread* rt = (_res_thread*) pthread_getspecific(_res_key);
    if (rt != NULL) {
        return rt;
    }

    /* It is the first time this function is called in this thread,
     * we need to create a new thread-specific DNS resolver state. */
    rt = res_thread_alloc();
    if (rt == NULL) {
        return NULL;
    }
    pthread_setspecific(_res_key, rt);

    /* Reset the state, note that res_ninit() can now properly reset
     * an existing state without leaking memory.
     */
    LOG(VERBOSE) << __func__ << ": tid=" << gettid() << ", rt=" << rt
                 << " setting DNS state (options=" << rt->_nres->options << ")";
    if (res_ninit(rt->_nres) < 0) {
        /* This should not happen */
        LOG(VERBOSE) << __func__ << ": tid=" << gettid() << " rt=" << rt
                     << ", res_ninit() returned < 0";
        res_thread_free(rt);
        pthread_setspecific(_res_key, NULL);
        return NULL;
    }
    return rt;
}

struct __res_state _nres;

res_state res_get_state(void) {
    _res_thread* rt = res_thread_get();

    return rt ? rt->_nres : NULL;
}

res_static* res_get_static(void) {
    _res_thread* rt = res_thread_get();

    return rt ? rt->_rstatic : NULL;
}
