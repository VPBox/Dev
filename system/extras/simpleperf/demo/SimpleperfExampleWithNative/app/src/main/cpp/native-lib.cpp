#include <jni.h>

#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <string>

#define noinline __attribute__((__noinline__))

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_simpleperf_simpleperfexamplewithnative_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

static void ThrowErrnoException(JNIEnv* env, const char* function_name, int err) {
    jclass cls = env->FindClass("android/system/ErrnoException");
    if (cls == nullptr) {
        return;
    }
    jmethodID cid = env->GetMethodID(cls, "<init>", "(Ljava/lang/String;I)V");
    if (cid == nullptr) {
        return;
    }
    jstring msg = env->NewStringUTF(function_name);
    if (msg == nullptr) {
        return;
    }
    jthrowable obj = (jthrowable)env->NewObject(cls, cid, msg, err);
    if (obj == nullptr) {
        return;
    }
    env->Throw(obj);
}

int CallFunction(int a) {
    return a + atoi("1");
}

static void* BusyLoopThread(void*) {
    volatile int i = 0;
    while (true) {
        i = CallFunction(i);
    }
    return nullptr;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_simpleperf_simpleperfexamplewithnative_MainActivity_createBusyThreadFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    pthread_t thread;
    int ret = pthread_create(&thread, nullptr, BusyLoopThread, nullptr);
    if (ret) {
        ThrowErrnoException(env, "pthread_create", ret);
        return;
    }
}

static inline uint64_t GetSystemClock() {
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000000000ULL + ts.tv_nsec;
}

constexpr int LOOP_COUNT = 100000000;
static uint64_t noinline RunFunction() {
    uint64_t start_time_in_ns = GetSystemClock();
    for (volatile int i = 0; i < LOOP_COUNT; ++i) {
    }
    return GetSystemClock() - start_time_in_ns;
}

static uint64_t noinline SleepFunction(unsigned long long sleep_time_in_ns) {
    uint64_t start_time_in_ns = GetSystemClock();
    struct timespec req;
    req.tv_sec = sleep_time_in_ns / 1000000000;
    req.tv_nsec = sleep_time_in_ns % 1000000000;
    nanosleep(&req, nullptr);
    return GetSystemClock() - start_time_in_ns;
}

static void* SleepThread(void*) {
    pthread_setname_np(pthread_self(), "SleepThread");
    uint64_t total_sleep_time_in_ns = 0;
    uint64_t total_run_time_in_ns = 0;
    while (true) {
        total_run_time_in_ns += RunFunction();
        if (total_sleep_time_in_ns < total_run_time_in_ns) {
            total_sleep_time_in_ns += SleepFunction(total_run_time_in_ns - total_sleep_time_in_ns);
        }
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_simpleperf_simpleperfexamplewithnative_SleepActivity_createSleepThreadFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    pthread_t thread;
    int ret = pthread_create(&thread, nullptr, SleepThread, nullptr);
    if (ret) {
        ThrowErrnoException(env, "pthread_create", ret);
        return;
    }
}

extern "C"
JNIEXPORT int JNICALL
Java_com_example_simpleperf_simpleperfexamplewithnative_MixActivity_callFunction(
        JNIEnv *env,
        jobject /* this */,
        int a) {
    return CallFunction(a);
}