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

#include <sstream>
#include <sys/stat.h>

#include "Hwc2TestVirtualDisplay.h"

#define DIR_NAME "images"

Hwc2TestVirtualDisplay::Hwc2TestVirtualDisplay(
        Hwc2TestCoverage coverage)
    : mDisplayDimension(coverage)
{
    mDisplayDimension.setDependent(&mOutputBuffer);
    mDisplayDimension.setDependent(&mExpectedBuffer);
}

std::string Hwc2TestVirtualDisplay::dump() const
{
    std::stringstream dmp;

    dmp << "virtual display: \n";

    mDisplayDimension.dump();

    return dmp.str();
}

int Hwc2TestVirtualDisplay::getOutputBuffer(buffer_handle_t* outHandle,
        android::base::unique_fd* outAcquireFence)
{
    int32_t acquireFence;
    int ret = mOutputBuffer.getOutputBuffer(outHandle, &acquireFence);
    outAcquireFence->reset(acquireFence);
    return ret;
}

void Hwc2TestVirtualDisplay::reset()
{
    return mDisplayDimension.reset();
}

bool Hwc2TestVirtualDisplay::advance()
{
    return mDisplayDimension.advance();
}

UnsignedArea Hwc2TestVirtualDisplay::getDisplayDimension() const
{
    return mDisplayDimension.get();
}

int Hwc2TestVirtualDisplay::verifyOutputBuffer(const Hwc2TestLayers* testLayers,
        const std::vector<hwc2_layer_t>* allLayers,
        const std::set<hwc2_layer_t>* clearLayers)
{
    int ret = mExpectedBuffer.generateExpectedBuffer(testLayers, allLayers,
            clearLayers);
    if (ret)
        return ret;

    ComparatorResult::get().CompareBuffers(mOutputBuffer.graphicBuffer(),
        mExpectedBuffer.graphicBuffer());

    return 0;
}

int Hwc2TestVirtualDisplay::writeBuffersToFile(std::string name)
{
    std::ostringstream expectedPath;
    std::ostringstream resultPath;
    int ret = mkdir(DIR_NAME, DEFFILEMODE);
    if (ret && errno != EEXIST)
        return ret;

    expectedPath << DIR_NAME << "/expected-" << name << ".png";
    resultPath << DIR_NAME << "/result-" << name << ".png";

    if (!mExpectedBuffer.writeBufferToFile(expectedPath.str()) ||
            !mOutputBuffer.writeBufferToFile(resultPath.str()))
        return -1;

    return 0;
}
