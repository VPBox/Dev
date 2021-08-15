#ifndef _CHRE_APP_SUPPORT_CC_
#define _CHRE_APP_SUPPORT_CC_

/*
Copyright (c) 2017, The Linux Foundation. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <chre.h>

#ifdef __cplusplus
extern "C" {
#endif

struct AppInfo {
  uint64_t appId;
  uint32_t appVer;
  bool (*init)(void); 
  void (*end)(void);
  void (*handle)(uint32_t, uint16_t, const void*);
};

extern void hexChreAddApp(const struct AppInfo* info);
extern void hexChreRemoveApp(const struct AppInfo* info);

#if !defined(NANOAPP_ID) || !defined(NANOAPP_VERSION)
#error NANOAPP_ID and NANOAPP_VERSION must be defined in the build environment
#endif

static const struct AppInfo mAppInfo = {
  .appId = NANOAPP_ID,
  .appVer = NANOAPP_VERSION,
  .init = (nanoappStart),
  .end = (nanoappEnd),
  .handle = (nanoappHandleEvent)
};

static void __appInit (void) __attribute__((constructor));
  static void __appInit (void) {
    hexChreAddApp(&(mAppInfo));
}

static void __appEnd (void) __attribute__((destructor));
    static void __appEnd (void) {
      hexChreRemoveApp(&(mAppInfo));
}

#ifdef __cplusplus
}
#endif

#endif
