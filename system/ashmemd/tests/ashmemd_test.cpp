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

#include <dlfcn.h>

#include <binder/IServiceManager.h>
#include <gtest/gtest.h>
#include <linux/ashmem.h>
#include <sys/mman.h>

#include <android/ashmemd/IAshmemDeviceService.h>

using android::IBinder;
using android::IServiceManager;
using android::String16;
using android::ashmemd::IAshmemDeviceService;
using android::os::ParcelFileDescriptor;

namespace android {
namespace ashmemd {

class AshmemdTest : public ::testing::Test {
  public:
    virtual void SetUp() override {
        sp<IServiceManager> sm = android::defaultServiceManager();
        sp<IBinder> binder = sm->getService(String16("ashmem_device_service"));
        ASSERT_NE(binder, nullptr);

        ashmemService = android::interface_cast<IAshmemDeviceService>(binder);
        ASSERT_NE(ashmemService, nullptr);
    }

    void openFd(ParcelFileDescriptor* fd) {
        auto status = ashmemService->open(fd);
        ASSERT_TRUE(status.isOk());
        ASSERT_GE(fd->get(), 0);
    }

    sp<IAshmemDeviceService> ashmemService;
};

TEST_F(AshmemdTest, OpenFd) {
    ParcelFileDescriptor fd;
    openFd(&fd);
}

TEST_F(AshmemdTest, OpenMultipleFds) {
    ParcelFileDescriptor fd1;
    ParcelFileDescriptor fd2;
    openFd(&fd1);
    openFd(&fd2);
    ASSERT_NE(fd1.get(), fd2.get());
}

TEST_F(AshmemdTest, MmapFd) {
    ParcelFileDescriptor pfd;
    openFd(&pfd);
    int fd = pfd.get();
    size_t testSize = 2097152;

    ASSERT_EQ(ioctl(fd, ASHMEM_SET_NAME, "AshmemdTest"), 0);
    ASSERT_EQ(ioctl(fd, ASHMEM_SET_SIZE, testSize), 0);

    void* data = mmap(NULL, testSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    ASSERT_NE(data, MAP_FAILED) << "Failed to mmap() ashmem fd";
    ASSERT_EQ(munmap(data, testSize), 0) << "Failed to munmap() ashmem fd";
}

TEST(LibAshmemdClientTest, OpenFd) {
    void* handle = dlopen("libashmemd_client.so", RTLD_NOW);
    ASSERT_NE(handle, nullptr) << "Failed to dlopen() libashmemd_client.so: " << dlerror();

    auto function = (int (*)())dlsym(handle, "openAshmemdFd");
    ASSERT_NE(function, nullptr) << "Failed to dlsym() openAshmemdFd() function: " << dlerror();

    int fd = function();
    ASSERT_GE(fd, 0) << "Failed to open /dev/ashmem";
}

}  // namespace ashmemd
}  // namespace android
