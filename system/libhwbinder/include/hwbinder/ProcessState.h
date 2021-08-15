/*
 * Copyright (C) 2005 The Android Open Source Project
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

#ifndef ANDROID_HARDWARE_PROCESS_STATE_H
#define ANDROID_HARDWARE_PROCESS_STATE_H

#include <hwbinder/IBinder.h>
#include <utils/KeyedVector.h>
#include <utils/String8.h>
#include <utils/String16.h>

#include <utils/threads.h>

#include <pthread.h>

#define MAX_CONTEXT 6

// ---------------------------------------------------------------------------
namespace android {
namespace hardware {

class IPCThreadState;

class ProcessState : public virtual RefBase
{
public:
    static  sp<ProcessState>    self();
    static  sp<ProcessState>    selfOrNull();
    // Note: don't call self() or selfOrNull() before initWithMmapSize()
    static  sp<ProcessState>    initWithMmapSize(size_t mmapSize); // size in bytes

            void                setContextObject(const sp<IBinder>& object);
            sp<IBinder>         getContextObject(const sp<IBinder>& caller);
            sp<IBinder>         getMgrContextObject(int index);

            void                setContextObject(const sp<IBinder>& object,
                                                 const String16& name);
            sp<IBinder>         getContextObject(const String16& name,
                                                 const sp<IBinder>& caller);

            void                startThreadPool();

    typedef bool (*context_check_func)(const String16& name,
                                       const sp<IBinder>& caller,
                                       void* userData);

            bool                isContextManager(void) const;
            bool                becomeContextManager(
                                    context_check_func checkFunc,
                                    void* userData);

            sp<IBinder>         getStrongProxyForHandle(int32_t handle);
            wp<IBinder>         getWeakProxyForHandle(int32_t handle);
            void                expungeHandle(int32_t handle, IBinder* binder);

            void                spawnPooledThread(bool isMain);

            status_t            setThreadPoolConfiguration(size_t maxThreads, bool callerJoinsPool);
            size_t              getMaxThreads();
            void                giveThreadPoolName();

            ssize_t             getKernelReferences(size_t count, uintptr_t* buf);
                                // This refcount includes:
                                // 1. Strong references to the node by this  and other processes
                                // 2. Temporary strong references held by the kernel during a
                                //    transaction on the node.
                                // It does NOT include local strong references to the node
            ssize_t             getStrongRefCountForNodeByHandle(int32_t handle);
            size_t              getMmapSize();

            enum class CallRestriction {
                // all calls okay
                NONE,
                // log when calls are blocking
                ERROR_IF_NOT_ONEWAY,
                // abort process on blocking calls
                FATAL_IF_NOT_ONEWAY,
            };
            // Sets calling restrictions for all transactions in this process. This must be called
            // before any threads are spawned.
            void setCallRestriction(CallRestriction restriction);

private:
    friend class IPCThreadState;
            explicit            ProcessState(size_t mmap_size);
                                ~ProcessState();

                                ProcessState(const ProcessState& o);
            ProcessState&       operator=(const ProcessState& o);
            String8             makeBinderThreadName();

            struct handle_entry {
                IBinder* binder;
                RefBase::weakref_type* refs;
            };

            handle_entry*       lookupHandleLocked(int32_t handle);

            int                 mDriverFD;
            void*               mVMStart;

            // Protects thread count variable below.
            pthread_mutex_t     mThreadCountLock;
            pthread_cond_t      mThreadCountDecrement;
            // Number of binder threads current executing a command.
            size_t              mExecutingThreadsCount;
            // Maximum number for binder threads allowed for this process.
            size_t              mMaxThreads;
            // Time when thread pool was emptied
            int64_t             mStarvationStartTimeMs;

    mutable Mutex               mLock;  // protects everything below.

            Vector<handle_entry>mHandleToObject;
            handle_entry        mSystemContextMgrHandle[MAX_CONTEXT];

            bool                mManagesContexts;
            context_check_func  mBinderContextCheckFunc;
            void*               mBinderContextUserData;

            KeyedVector<String16, sp<IBinder> >
                                mContexts;


            String8             mRootDir;
            bool                mThreadPoolStarted;
            bool                mSpawnThreadOnStart;
    volatile int32_t            mThreadPoolSeq;
            const size_t        mMmapSize;

            CallRestriction     mCallRestriction;
};

}; // namespace hardware
}; // namespace android

// ---------------------------------------------------------------------------

#endif // ANDROID_HARDWARE_PROCESS_STATE_H
