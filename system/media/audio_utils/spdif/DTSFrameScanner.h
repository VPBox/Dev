/*
 * Copyright 2015, The Android Open Source Project
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

#ifndef ANDROID_AUDIO_DTS_FRAME_SCANNER_H
#define ANDROID_AUDIO_DTS_FRAME_SCANNER_H

#include <stdint.h>
#include <audio_utils/spdif/FrameScanner.h>

namespace android {

#define DTS_NUM_SAMPLE_RATE_TABLE_ENTRIES      16
#define DTS_PCM_FRAMES_PER_BLOCK               32
#define DTS_MAX_BLOCKS_PER_SYNC_FRAME_BLOCK   128

class DTSFrameScanner : public FrameScanner
{
public:
    DTSFrameScanner();
    virtual ~DTSFrameScanner();

    virtual int getMaxChannels()   const { return 5 + 1; }

    virtual int getMaxSampleFramesPerSyncFrame() const {
        return  DTS_MAX_BLOCKS_PER_SYNC_FRAME_BLOCK * DTS_PCM_FRAMES_PER_BLOCK;
    }

    virtual int getSampleFramesPerSyncFrame() const {
        return mSampleFramesPerSyncFrame;
    }

    virtual bool isFirstInBurst() { return true; }
    virtual bool isLastInBurst() { return true; }
    virtual void resetBurst()  { }

protected:

    int mSampleFramesPerSyncFrame;

    virtual bool parseHeader();

    static const uint8_t kSyncBytes[];
    static const int32_t kDTSSampleRateTable[];

};

}  // namespace android
#endif  // ANDROID_AUDIO_DTS_FRAME_SCANNER_H
