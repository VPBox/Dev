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

#ifndef IIORAP_IFACE_DEF_H
#define IIORAP_IFACE_DEF_H

// Provide an x-macro that defines the 'IIorap.aidl' interface through repeated
// macro invocation on the member functions and their respective parameters.
//
// Future changes in the AIDL file to add new methods or change parameters should avoid
// a boilerplate-ripple effect in the rest of the codebase.

#define IIORAP_IFACE_DEF(FN_BEGIN, FN, FN_END)                                                     \
FN_BEGIN(::com::google::android::startup::iorap::,IIorap)                                          \
/* name              <see IORAP_BINDER_PARAM_JOIN> */                                              \
FN(setTaskListener, (const ::android::sp<::com::google::android::startop::iorap::,ITaskListener,>&,listener)) /*NOLINT*/ \
FN(onAppLaunchEvent,(const ::com::google::android::startop::iorap::,RequestId,&,request),          \
                    (const ::com::google::android::startop::iorap::,AppLaunchEvent,&,event))       \
FN(onPackageEvent,  (const ::com::google::android::startop::iorap::,RequestId,&,request),          \
                    (const ::com::google::android::startop::iorap::,PackageEvent,&,event))         \
FN(onAppIntentEvent,(const ::com::google::android::startop::iorap::,RequestId,&,request),          \
                    (const ::com::google::android::startop::iorap::,AppIntentEvent,&,event))       \
FN(onSystemServiceEvent,                                                                           \
                    (const ::com::google::android::startop::iorap::,RequestId,&,request),          \
                    (const ::com::google::android::startop::iorap::,SystemServiceEvent,&,event))   \
FN(onSystemServiceUserEvent,                                                                       \
                    (const ::com::google::android::startop::iorap::,RequestId,&,request),          \
                    (const ::com::google::android::startop::iorap::,SystemServiceUserEvent,&,event))\
FN_END()                                                                                           \

// Convenience macros to unpack the 2nd parameter from IIORAP_IFACE_DEF#FN calls.

#define IORAP_BINDER_PARAM_JOIN_ALL(arg) IORAP_BINDER_PARAM_JOIN_ALL_IMPL arg
#define IORAP_BINDER_PARAM_JOIN_ALL_IMPL(type_l, type, type_r, name) type_l type type_r name

#define IORAP_BINDER_PARAM_JOIN_NAMES(arg) IORAP_BINDER_PARAM_JOIN_NAMES_IMPL arg
#define IORAP_BINDER_PARAM_JOIN_NAMES_IMPL(type_l, type, type_r, name) name

#endif //IIORAP_IFACE_DEF_H
