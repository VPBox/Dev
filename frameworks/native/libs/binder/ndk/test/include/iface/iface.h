/*
 * Copyright (C) 2018 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <android/binder_ibinder.h>
#include <utils/RefBase.h>

// warning: it is recommended to use AIDL output instead of this. binder_ibinder_utils.h and some of
// the other niceties make sure that, for instance, binder proxies are always the same. They also
// don't use internal Android APIs like refbase which are used here only for convenience.

class IFoo : public virtual ::android::RefBase {
   public:
    static const char* kSomeInstanceName;
    static const char* kInstanceNameToDieFor;

    static AIBinder_Class* kClass;

    // Takes ownership of IFoo
    binder_status_t addService(const char* instance);
    static ::android::sp<IFoo> getService(const char* instance, AIBinder** outBinder = nullptr);

    enum Call {
        DOFOO = FIRST_CALL_TRANSACTION + 0,
        DIE = FIRST_CALL_TRANSACTION + 1,
    };

    virtual ~IFoo();

    virtual binder_status_t doubleNumber(int32_t in, int32_t* out) = 0;
    virtual binder_status_t die() = 0;

   private:
    // this variable is only when IFoo is local (since this test combines 'IFoo' and 'BnFoo'), not
    // for BpFoo.
    AIBinder_Weak* mWeakBinder = nullptr;
};
