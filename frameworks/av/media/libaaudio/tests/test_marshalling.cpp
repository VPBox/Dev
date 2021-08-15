/*
 * Copyright (C) 2016 The Android Open Source Project
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

// Unit tests for AAudio Marshalling of RingBuffer information.

#include <stdlib.h>
#include <math.h>

#include <android-base/unique_fd.h>
#include <binder/Parcel.h>
#include <binder/Parcelable.h>
#include <cutils/ashmem.h>
#include <gtest/gtest.h>
#include <sys/mman.h>

#include <aaudio/AAudio.h>
#include <binding/AudioEndpointParcelable.h>

using android::base::unique_fd;
using namespace android;
using namespace aaudio;

// Test adding one value.
TEST(test_marshalling, aaudio_one_read_write) {
    Parcel parcel;
    size_t pos = parcel.dataPosition();
    const int arbitraryValue = 235;
    parcel.writeInt32(arbitraryValue);
    parcel.setDataPosition(pos);
    int32_t y;
    parcel.readInt32(&y);
    EXPECT_EQ(arbitraryValue, y);
}

// Test SharedMemoryParcel.
TEST(test_marshalling, aaudio_shared_memory) {
    SharedMemoryParcelable sharedMemoryA;
    SharedMemoryParcelable sharedMemoryB;
    const size_t memSizeBytes = 840;
    unique_fd fd(ashmem_create_region("TestMarshalling", memSizeBytes));
    ASSERT_LE(0, fd);
    sharedMemoryA.setup(fd, memSizeBytes);
    void *region1;
    EXPECT_EQ(AAUDIO_OK, sharedMemoryA.resolve(0, 16, &region1)); // fits in region
    EXPECT_NE(AAUDIO_OK, sharedMemoryA.resolve(-2, 16, &region1)); // offset is negative
    EXPECT_NE(AAUDIO_OK, sharedMemoryA.resolve(0, memSizeBytes + 8, &region1)); // size too big
    EXPECT_NE(AAUDIO_OK, sharedMemoryA.resolve(memSizeBytes - 8, 16, &region1)); // goes past the end
    int32_t *buffer1 = (int32_t *)region1;
    buffer1[0] = 98735; // arbitrary value

    Parcel parcel;
    size_t pos = parcel.dataPosition();
    sharedMemoryA.writeToParcel(&parcel);

    parcel.setDataPosition(pos);
    sharedMemoryB.readFromParcel(&parcel);
    EXPECT_EQ(sharedMemoryA.getSizeInBytes(), sharedMemoryB.getSizeInBytes());

    // should see same value at two different addresses
    void *region2;
    EXPECT_EQ(AAUDIO_OK, sharedMemoryB.resolve(0, 16, &region2));
    int32_t *buffer2 = (int32_t *)region2;
    EXPECT_NE(buffer1, buffer2);
    EXPECT_EQ(buffer1[0], buffer2[0]);
}

// Test SharedRegionParcel.
TEST(test_marshalling, aaudio_shared_region) {
    SharedMemoryParcelable sharedMemories[2];
    SharedRegionParcelable sharedRegionA;
    SharedRegionParcelable sharedRegionB;
    const size_t memSizeBytes = 840;
    unique_fd fd(ashmem_create_region("TestMarshalling", memSizeBytes));
    ASSERT_LE(0, fd);
    sharedMemories[0].setup(fd, memSizeBytes);
    int32_t regionOffset1 = 32;
    int32_t regionSize1 = 16;
    sharedRegionA.setup(0, regionOffset1, regionSize1);

    void *region1;
    EXPECT_EQ(AAUDIO_OK, sharedRegionA.resolve(sharedMemories, &region1));
    int32_t *buffer1 = (int32_t *)region1;
    buffer1[0] = 336677; // arbitrary value

    Parcel parcel;
    size_t pos = parcel.dataPosition();
    sharedRegionA.writeToParcel(&parcel);

    parcel.setDataPosition(pos);
    sharedRegionB.readFromParcel(&parcel);

    // should see same value
    void *region2;
    EXPECT_EQ(AAUDIO_OK, sharedRegionB.resolve(sharedMemories, &region2));
    int32_t *buffer2 = (int32_t *)region2;
    EXPECT_EQ(buffer1[0], buffer2[0]);
}

// Test RingBufferParcelable.
TEST(test_marshalling, aaudio_ring_buffer_parcelable) {
    SharedMemoryParcelable sharedMemories[2];
    RingBufferParcelable ringBufferA;
    RingBufferParcelable ringBufferB;

    const size_t bytesPerFrame = 8;
    const size_t framesPerBurst = 32;
    const size_t dataSizeBytes = 2048;
    const int32_t counterSizeBytes = sizeof(int64_t);
    const size_t memSizeBytes = dataSizeBytes + (2 * counterSizeBytes);

    unique_fd fd(ashmem_create_region("TestMarshalling Z", memSizeBytes));
    ASSERT_LE(0, fd);
    sharedMemories[0].setup(fd, memSizeBytes);

    int32_t sharedMemoryIndex = 0;
    // arrange indices and data in the shared memory
    int32_t readOffset = 0;
    int32_t writeOffset = readOffset + counterSizeBytes;
    int32_t dataOffset = writeOffset + counterSizeBytes;
    ringBufferA.setupMemory(sharedMemoryIndex, dataOffset, dataSizeBytes,
        readOffset, writeOffset, counterSizeBytes);
    ringBufferA.setFramesPerBurst(framesPerBurst);
    ringBufferA.setBytesPerFrame(bytesPerFrame);
    ringBufferA.setCapacityInFrames(dataSizeBytes / bytesPerFrame);

    // setup A
    RingBufferDescriptor descriptorA;
    EXPECT_EQ(AAUDIO_OK, ringBufferA.resolve(sharedMemories, &descriptorA));
    descriptorA.dataAddress[0] = 95;
    descriptorA.dataAddress[1] = 57;
    descriptorA.readCounterAddress[0] = 17;
    descriptorA.writeCounterAddress[0] = 39;

    // write A to parcel
    Parcel parcel;
    size_t pos = parcel.dataPosition();
    ringBufferA.writeToParcel(&parcel);

    // read B from parcel
    parcel.setDataPosition(pos);
    ringBufferB.readFromParcel(&parcel);

    RingBufferDescriptor descriptorB;
    EXPECT_EQ(AAUDIO_OK, ringBufferB.resolve(sharedMemories, &descriptorB));

    // A and B should match
    EXPECT_EQ(descriptorA.dataAddress[0], descriptorB.dataAddress[0]);
    EXPECT_EQ(descriptorA.dataAddress[1], descriptorB.dataAddress[1]);
    EXPECT_EQ(descriptorA.readCounterAddress[0], descriptorB.readCounterAddress[0]);
    EXPECT_EQ(descriptorA.writeCounterAddress[0], descriptorB.writeCounterAddress[0]);

    EXPECT_EQ(ringBufferA.getFramesPerBurst(), ringBufferB.getFramesPerBurst());
    EXPECT_EQ(ringBufferA.getBytesPerFrame(), ringBufferB.getBytesPerFrame());
    EXPECT_EQ(ringBufferA.getCapacityInFrames(), ringBufferB.getCapacityInFrames());
}
