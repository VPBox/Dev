/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include <android/log.h>
#include <jni.h>
#include <pthread.h>
#include <simpleperf.h>
#include <unistd.h>

#include <atomic>
#include <string>

static void log(const char* msg) {
  __android_log_print(ANDROID_LOG_INFO, "simpleperf", "%s", msg);
}

static std::atomic_bool profile_thread_exited(false);

void* ProfileThreadFunc(void*) {
  pthread_setname_np(pthread_self(), "ProfileThread");
  simpleperf::RecordOptions options;
  options.RecordDwarfCallGraph();
  simpleperf::ProfileSession session;
  log("start recording");
  session.StartRecording(options);
  for (int i = 0; i < 3; i++) {
    sleep(1);
    log("pause recording");
    session.PauseRecording();
    sleep(1);
    log("resume recording");
    session.ResumeRecording();
  }
  sleep(1);
  log("stop recording");
  session.StopRecording();
  log("stop recording successfully");
  profile_thread_exited = true;
  return nullptr;
};

int CallFunction(int a) {
  return a + 1;
}

static std::atomic_int64_t count;

void* BusyThreadFunc(void*) {
  pthread_setname_np(pthread_self(), "BusyThread");
  count = 0;
  volatile int i;
  while (!profile_thread_exited) {
    for (i = 0; i < 1000000; ) {
      i = CallFunction(i);
    }
    timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 1000000;
    nanosleep(&ts, nullptr);
    count++;
  }
  return nullptr;
}

extern "C" JNIEXPORT void JNICALL
Java_simpleperf_demo_cpp_1api_MainActivity_runNativeCode(
    JNIEnv *env,
    jobject jobj) {
  pthread_t profile_thread;
  if (pthread_create(&profile_thread, nullptr, ProfileThreadFunc, nullptr) != 0) {
    log("failed to create profile thread");
    return;
  }
  pthread_t busy_thread;
  if (pthread_create(&busy_thread, nullptr, BusyThreadFunc, nullptr) != 0) {
    log("failed to create busy thread");
  }
}

extern "C" JNIEXPORT jlong JNICALL
Java_simpleperf_demo_cpp_1api_MainActivity_getBusyThreadCount(
    JNIEnv *env,
    jobject jobj) {
  return static_cast<jlong>(count);
}
