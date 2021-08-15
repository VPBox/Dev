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

#ifndef UIMG_DL_VER_MAJOR
#define UIMG_DL_VER_MAJOR 1
#endif
#ifndef UIMG_DL_VER_MINOR
#define UIMG_DL_VER_MINOR 0
#endif
#ifndef UIMG_DL_VER_MAINT
#define UIMG_DL_VER_MAINT 0
#endif

#define __TOSTR(_x) #_x
#define _TOSTR(_x) __TOSTR(_x)

typedef struct note_type{
  int sizename;
  int sizedesc;
  int type;
  char name[24];
  int desc[3];
} note_type;

#ifdef __llvm__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#endif

const note_type uimg_dl_ver __attribute__ ((section (".note.qti.uimg.dl.ver")))
                            __attribute__ ((visibility ("default"))) = {
  24,
  12,
  0,
  "uimg.dl.ver." _TOSTR(UIMG_DL_VER_MAJOR) "." _TOSTR(UIMG_DL_VER_MINOR) "." _TOSTR(UIMG_DL_VER_MAINT),
  {UIMG_DL_VER_MAJOR, UIMG_DL_VER_MINOR, UIMG_DL_VER_MAINT}
};

#ifdef __llvm__
#pragma clang diagnostic pop
#endif


