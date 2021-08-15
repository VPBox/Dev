/*
 * Copyright 2018 The Android Open Source Project
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

#include <stdlib.h>

#include <algorithm>

#include <binder/ProcessState.h>
#include <gtest/gtest.h>
#include <gui/Surface.h>
#include <media/ICrypto.h>
#include <media/MediaCodecBuffer.h>
#include <media/hardware/VideoAPI.h>
#include <media/stagefright/MediaCodec.h>
#include <media/stagefright/MediaCodecConstants.h>
#include <media/stagefright/foundation/ABuffer.h>
#include <media/stagefright/foundation/ALooper.h>
#include <media/stagefright/foundation/AMessage.h>

namespace android {

class MediaCodecSanityTest : public ::testing::Test {
public:
    MediaCodecSanityTest()
        : looper(new ALooper),
          cfg(new AMessage),
          ifmt(new AMessage),
          ofmt(new AMessage) {
        ProcessState::self()->startThreadPool();
        looper->start();
    }

    ~MediaCodecSanityTest() {
        if (codec != nullptr) {
            codec->release();
        }
        looper->stop();
    }

    sp<ALooper> looper;
    sp<MediaCodec> codec;
    sp<AMessage> cfg;
    sp<AMessage> ifmt;
    sp<AMessage> ofmt;
};

const static size_t kLinearBufferSize = 1048576;

// data for a codec input frame
struct FrameData {
    const uint8_t *data;
    size_t size;
    template<size_t N>
    constexpr FrameData(const uint8_t(&data_)[N]) : data(data_), size(N) { }
};

// one yellow frame of 240x180 (albeit 4:4:4)
const uint8_t avcStream_A1[] = { // IDR frame
    0x00, 0x00, 0x00, 0x01, 0x67, 0x64, 0x00, 0x0d, 0xac, 0xd9, 0x41, 0x41, 0xfa, 0x10, 0x00, 0x00,
    0x03, 0x00, 0x10, 0x00, 0x00, 0x03, 0x03, 0x20, 0xf1, 0x42, 0x99, 0x60,

    0x00, 0x00, 0x00, 0x01, 0x68, 0xeb, 0xe3, 0xcb, 0x22, 0xc0,

    0x00, 0x00, 0x01, 0x65, 0x88, 0x84, 0x00, 0x2b, 0xff, 0xfe, 0xd8, 0xe7, 0xf3, 0x2c, 0xa5, 0x60,
    0xca, 0xbb, 0xf1, 0x5c, 0x44, 0x7c, 0x9a, 0xa5, 0xc3, 0xab, 0x2f, 0x77, 0x0a, 0x94, 0x0d, 0x19,
    0x43, 0x3b, 0x4f, 0x25, 0xea, 0x66, 0x00, 0x01, 0x24, 0xcd, 0x35, 0x5f, 0xc2, 0x34, 0x89, 0xd1,
    0xa5, 0x60, 0x09, 0x98, 0x00, 0x01, 0x1b, 0x0e, 0xcb, 0x0d, 0x04, 0x86, 0x94, 0xe2, 0x32, 0x3c,
    0xdd, 0x0f,
};

FrameData avcStream_A[] __unused = { avcStream_A1 };

// AVC stream of 2 yellow frames (240x180)
const uint8_t avcStream_B1[] = { // IDR frame
    0x00, 0x00, 0x00, 0x01, 0x67, 0x64, 0x00, 0x0c, 0xac, 0xd9, 0x41, 0x41, 0xfa, 0x10, 0x00, 0x00,
    0x03, 0x00, 0x10, 0x00, 0x00, 0x03, 0x02, 0x80, 0xf1, 0x42, 0x99, 0x60,

    0x00, 0x00, 0x00, 0x01, 0x68, 0xeb, 0xe3, 0xcb, 0x22, 0xc0,

    0x00, 0x00, 0x01, 0x65, 0x88, 0x84, 0x00, 0x33, 0xff, 0xfe, 0xdf, 0x32, 0xf8, 0x14, 0xd6, 0x25,
    0xd0, 0x74, 0x42, 0x50, 0x84, 0x6f, 0xf4, 0xc2, 0x5c, 0x76, 0x37, 0x17, 0x72, 0xac, 0x52, 0xfc,
    0xd6, 0x1f, 0xd2, 0xd0, 0x60, 0xb2, 0x20, 0x00, 0x10, 0x3d, 0x2a, 0xc0, 0xe4, 0x27, 0xcb, 0xce,
    0xea, 0x25, 0x00, 0x81, 0x00, 0x00, 0x0f, 0x40, 0xbc, 0x81, 0x15, 0xc1, 0x65, 0x20, 0x80, 0x81,
    0x7a, 0x57, 0x51,
};

const uint8_t avcStream_B2[] = { // P frame
    0x00, 0x00, 0x00, 0x01, 0x41, 0x9a, 0x21, 0x6c, 0x42, 0xbf, 0xfe, 0x38, 0x40, 0x00, 0x0d, 0x48,
};

FrameData avcStream_B[] = { avcStream_B1, avcStream_B2 };

class MediaCodecInputBufferSizeTest : public MediaCodecSanityTest,
        public ::testing::WithParamInterface<int32_t> {
};

TEST_P(MediaCodecInputBufferSizeTest, TestAvcDecoder) {
    codec = MediaCodec::CreateByComponentName(looper, "c2.android.avc.decoder");
    cfg->setInt32("width", 320);
    cfg->setInt32("height", 240);
    cfg->setString("mime", MIMETYPE_VIDEO_AVC);

    const int32_t InputSize = GetParam();
    if (InputSize >= 0) {
        cfg->setInt32("max-input-size", InputSize);
    }

    EXPECT_EQ(codec->configure(cfg, nullptr, nullptr, 0), OK);
    EXPECT_EQ(codec->getInputFormat(&ifmt), OK);
    int32_t maxInputSize;
    ASSERT_TRUE(ifmt->findInt32("max-input-size", &maxInputSize));
    if (InputSize > 0) {
        EXPECT_EQ(maxInputSize, InputSize);
    } else {
        EXPECT_GE(maxInputSize, 1 << 20); // 1 MB
    }
    EXPECT_EQ(codec->start(),  OK);
    size_t ix;
    EXPECT_EQ(codec->dequeueInputBuffer(&ix, 1000000), OK);
    sp<MediaCodecBuffer> buf;
    EXPECT_EQ(codec->getInputBuffer(ix, &buf),  OK);
    EXPECT_GE(buf->size(), (size_t)maxInputSize);
    EXPECT_LE(buf->size(), (size_t)maxInputSize + 4096u);
}

TEST_P(MediaCodecInputBufferSizeTest, TestVideoDecoder) {
    codec = MediaCodec::CreateByComponentName(looper, "c2.android.vp8.decoder");
    cfg->setInt32("width", 320);
    cfg->setInt32("height", 240);
    cfg->setString("mime", MIMETYPE_VIDEO_VP8);

    const int32_t InputSize = GetParam();
    if (InputSize >= 0) {
        cfg->setInt32("max-input-size", InputSize);
    }

    EXPECT_EQ(codec->configure(cfg, nullptr, nullptr, 0), OK);
    EXPECT_EQ(codec->getInputFormat(&ifmt), OK);
    int32_t maxInputSize;
    ASSERT_TRUE(ifmt->findInt32("max-input-size", &maxInputSize));
    if (InputSize > 0) {
        EXPECT_EQ(maxInputSize, InputSize);
    } else {
        EXPECT_GE(maxInputSize, 1 << 20); // 1 MB
    }
    EXPECT_EQ(codec->start(),  OK);
    size_t ix;
    EXPECT_EQ(codec->dequeueInputBuffer(&ix, 1000000), OK);
    sp<MediaCodecBuffer> buf;
    EXPECT_EQ(codec->getInputBuffer(ix, &buf),  OK);
    EXPECT_GE(buf->size(), (size_t)maxInputSize);
    EXPECT_LE(buf->size(), (size_t)maxInputSize + 4096u);
}

TEST_P(MediaCodecInputBufferSizeTest, TestAudioDecoder) {
    codec = MediaCodec::CreateByComponentName(looper, "c2.android.aac.decoder");
    cfg->setInt32("sample-rate", 44100);
    cfg->setInt32("channel-count", 2);
    cfg->setString("mime", MIMETYPE_AUDIO_AAC);

    const int32_t InputSize = GetParam();
    if (InputSize >= 0) {
        cfg->setInt32("max-input-size", InputSize);
    }

    EXPECT_EQ(codec->configure(cfg, nullptr, nullptr, 0), OK);
    EXPECT_EQ(codec->getInputFormat(&ifmt), OK);
    int32_t maxInputSize;
    if (InputSize > 0) {
        ASSERT_TRUE(ifmt->findInt32("max-input-size", &maxInputSize));
        EXPECT_EQ(maxInputSize, InputSize);
    } else {
        if (ifmt->findInt32("max-input-size", &maxInputSize)) {
            EXPECT_EQ(maxInputSize, 1 << 19); // 512 KB
        }
        maxInputSize = kLinearBufferSize; // input size is set by channel
    }

    EXPECT_EQ(codec->start(),  OK);
    size_t ix;
    EXPECT_EQ(codec->dequeueInputBuffer(&ix, 1000000), OK);
    sp<MediaCodecBuffer> buf;
    EXPECT_EQ(codec->getInputBuffer(ix, &buf),  OK);
    EXPECT_GE(buf->size(), (size_t)maxInputSize);
    EXPECT_LE(buf->size(), (size_t)maxInputSize + 4096u);
}

INSTANTIATE_TEST_CASE_P(InputSizes, MediaCodecInputBufferSizeTest, ::testing::Values(-1, 1234, 12345678));

TEST_F(MediaCodecSanityTest, TestAvcDecoderHdrStaticInfo) {
    codec = MediaCodec::CreateByComponentName(looper, "c2.android.avc.decoder");
    cfg->setInt32("width", 320);
    cfg->setInt32("height", 240);
    cfg->setString("mime", MIMETYPE_VIDEO_AVC);
    HDRStaticInfo info = { .mID = HDRStaticInfo::kType1, .sType1 = {
        .mR = { .x = 35400, .y = 14600 }, .mG = { .x = 8500,  .y = 39850 },
        .mB = { .x = 6550,  .y =  2300 }, .mW = { .x = 15635, .y = 16450 },
        .mMaxDisplayLuminance = 1000, .mMinDisplayLuminance = 1000,
        .mMaxContentLightLevel = 1000, .mMaxFrameAverageLightLevel = 120 }
    };
    cfg->setBuffer("hdr-static-info", ABuffer::CreateAsCopy(&info, sizeof(info)));

    EXPECT_EQ(codec->configure(cfg, nullptr, nullptr, 0), OK);
    EXPECT_EQ(codec->getOutputFormat(&ofmt), OK);
    sp<ABuffer> oinfo;
    ASSERT_TRUE(ofmt->findBuffer("hdr-static-info", &oinfo));
    ASSERT_EQ(oinfo->size(), sizeof(info));
    EXPECT_EQ(memcmp(oinfo->data(), &info, sizeof(info)),  0);

    EXPECT_EQ(codec->start(),  OK);
    // assume we can submit all input before dequeuing output
    size_t frameIx = 0;
    size_t ix;
    sp<MediaCodecBuffer> buf;
    for (const FrameData &frame : avcStream_B) {
        EXPECT_EQ(codec->dequeueInputBuffer(&ix, 1000000), OK);
        EXPECT_EQ(codec->getInputBuffer(ix, &buf),  OK);
        ASSERT_GE(buf->capacity(), frame.size);
        memcpy(buf->base(), frame.data, frame.size);
        EXPECT_EQ(buf->setRange(0, frame.size), OK);
        bool eos = ++frameIx == NELEM(avcStream_B);
        EXPECT_EQ(codec->queueInputBuffer(ix, 0, frame.size, frameIx * 33333,
                                          eos ? BUFFER_FLAG_END_OF_STREAM : 0),  OK);
    }

    size_t offset, size;
    int64_t ts;
    uint32_t flags;
    bool mInfoFormatChangedOk = true;
    bool mInfoBuffersChangedOk = true;
    while (true) {
        status_t err = codec->dequeueOutputBuffer(&ix, &offset, &size, &ts, &flags, 1000000);
        if (err == INFO_FORMAT_CHANGED && mInfoFormatChangedOk) {
            mInfoFormatChangedOk = false;
        } else if (err == INFO_OUTPUT_BUFFERS_CHANGED && mInfoBuffersChangedOk) {
            mInfoBuffersChangedOk = false;
        } else {
            ASSERT_EQ(err, OK);
            break;
        }
    }
    EXPECT_EQ(codec->getOutputBuffer(ix, &buf), OK);
    EXPECT_EQ(codec->getOutputFormat(ix, &ofmt), OK);
    ASSERT_TRUE(ofmt->findBuffer("hdr-static-info", &oinfo));
    ASSERT_EQ(oinfo->size(), sizeof(info));
    EXPECT_EQ(memcmp(oinfo->data(), &info, sizeof(info)),  0);
}

TEST_F(MediaCodecSanityTest, TestVideoDecoderHdrStaticInfo) {
    codec = MediaCodec::CreateByComponentName(looper, "c2.android.mpeg4.decoder");
    cfg->setInt32("width", 320);
    cfg->setInt32("height", 240);
    cfg->setString("mime", MIMETYPE_VIDEO_MPEG4);
    HDRStaticInfo info = { .mID = HDRStaticInfo::kType1, .sType1 = {
        .mR = { .x = 35400, .y = 14600 }, .mG = { .x = 8500,  .y = 39850 },
        .mB = { .x = 6550,  .y =  2300 }, .mW = { .x = 15635, .y = 16450 },
        .mMaxDisplayLuminance = 1000, .mMinDisplayLuminance = 1000,
        .mMaxContentLightLevel = 1000, .mMaxFrameAverageLightLevel = 120 }
    };
    cfg->setBuffer("hdr-static-info", ABuffer::CreateAsCopy(&info, sizeof(info)));

    EXPECT_EQ(codec->configure(cfg, nullptr, nullptr, 0), OK);
    EXPECT_EQ(codec->getOutputFormat(&ofmt), OK);
    sp<ABuffer> oinfo;
    ASSERT_TRUE(ofmt->findBuffer("hdr-static-info", &oinfo));
    ASSERT_EQ(oinfo->size(), sizeof(info));
    EXPECT_EQ(memcmp(oinfo->data(), &info, sizeof(info)),  0);
}

class MediaCodecByteBufferTest : public MediaCodecSanityTest,
        public ::testing::WithParamInterface<int32_t> {
};

TEST_P(MediaCodecByteBufferTest, TestVideoDecoder420Planar) {
    codec = MediaCodec::CreateByComponentName(looper, "c2.android.avc.decoder");
//    codec = MediaCodec::CreateByComponentName(looper, "OMX.google.h264.decoder");
    cfg->setInt32("width", 320);
    cfg->setInt32("height", 240);
    cfg->setString("mime", MIMETYPE_VIDEO_AVC);
    const int32_t Color = GetParam();
    if (Color >= 0) {
        cfg->setInt32("color-format", Color);
    }
    int32_t xcolor = Color == -1 ? COLOR_FormatYUV420Planar : Color;

    EXPECT_EQ(codec->configure(cfg, nullptr, nullptr, 0), OK);
    EXPECT_EQ(codec->getOutputFormat(&ofmt), OK);
    int32_t ocolor = -1;
    EXPECT_TRUE(ofmt->findInt32("color-format", &ocolor));
    EXPECT_EQ(ocolor, xcolor);

    EXPECT_EQ(codec->start(),  OK);
    // assume we can submit all input before dequeuing output
    size_t frameIx = 0;
    size_t ix;
    sp<MediaCodecBuffer> buf;
    for (const FrameData &frame : avcStream_A) {
        EXPECT_EQ(codec->dequeueInputBuffer(&ix, 1000000), OK);
        EXPECT_EQ(codec->getInputBuffer(ix, &buf),  OK);
        ASSERT_GE(buf->capacity(), frame.size);
        memcpy(buf->base(), frame.data, frame.size);
        EXPECT_EQ(buf->setRange(0, frame.size), OK);
        bool eos = ++frameIx == NELEM(avcStream_A);
        EXPECT_EQ(codec->queueInputBuffer(ix, 0, frame.size, frameIx * 33333,
                                          eos ? BUFFER_FLAG_END_OF_STREAM : 0),  OK);
    }

    size_t offset, size;
    int64_t ts;
    uint32_t flags;
    bool mInfoFormatChangedOk = true;
    bool mInfoBuffersChangedOk = true;
    while (true) {
        status_t err = codec->dequeueOutputBuffer(&ix, &offset, &size, &ts, &flags, 1000000);
        if (err == INFO_FORMAT_CHANGED && mInfoFormatChangedOk) {
            mInfoFormatChangedOk = false;
        } else if (err == INFO_OUTPUT_BUFFERS_CHANGED && mInfoBuffersChangedOk) {
            mInfoBuffersChangedOk = false;
        } else {
            ASSERT_EQ(err, OK);
            break;
        }
    }
    EXPECT_EQ(codec->getOutputBuffer(ix, &buf), OK);
    EXPECT_EQ(codec->getOutputFormat(ix, &ofmt), OK);
    ASSERT_TRUE(ofmt->findInt32("color-format", &ocolor));
    EXPECT_EQ(ocolor, xcolor) << ofmt->debugString(8).c_str() << buf->meta()->debugString(8).c_str();
    // expect an image-data in both format and meta
    sp<ABuffer> imgBuf, imgBuf2;
    ASSERT_TRUE(ofmt->findBuffer("image-data", &imgBuf));
    ASSERT_TRUE(buf->meta()->findBuffer("image-data", &imgBuf2));
    EXPECT_EQ(imgBuf->size(), sizeof(MediaImage2));
    ASSERT_EQ(imgBuf->size(), imgBuf2->size());
    EXPECT_EQ(0, memcmp(imgBuf->data(), imgBuf2->data(), imgBuf->size()));
    MediaImage2 *img = (MediaImage2*)imgBuf->data();
    EXPECT_EQ(img->mType, img->MEDIA_IMAGE_TYPE_YUV);
    EXPECT_EQ(img->mNumPlanes, 3u);
    EXPECT_EQ(img->mWidth, 320u);
    EXPECT_EQ(img->mHeight, 240u);
    EXPECT_EQ(img->mBitDepth, 8u);
    EXPECT_EQ(img->mBitDepthAllocated, 8u);

    // read strides from format
    int32_t stride, vstride;
    ofmt->findInt32("stride", &stride)          || ofmt->findInt32("width", &stride);
    ofmt->findInt32("slice-height", &vstride)   || ofmt->findInt32("height", &vstride);

    EXPECT_EQ(img->mPlane[img->Y].mHorizSubsampling, 1u);
    EXPECT_EQ(img->mPlane[img->Y].mVertSubsampling, 1u);
    EXPECT_EQ(img->mPlane[img->U].mHorizSubsampling, 2u);
    EXPECT_EQ(img->mPlane[img->U].mVertSubsampling, 2u);
    EXPECT_EQ(img->mPlane[img->V].mHorizSubsampling, 2u);
    EXPECT_EQ(img->mPlane[img->V].mVertSubsampling, 2u);

    switch (xcolor) {
        // defined formats
        case COLOR_FormatYUV420Planar:
        case COLOR_FormatYUV420PackedPlanar:
            EXPECT_EQ(img->mPlane[img->Y].mOffset, 0u);
            EXPECT_EQ(img->mPlane[img->Y].mColInc, 1);
            EXPECT_EQ(img->mPlane[img->Y].mRowInc, stride);

            EXPECT_EQ(img->mPlane[img->U].mOffset, (uint32_t)(stride * vstride));
            EXPECT_EQ(img->mPlane[img->U].mColInc, 1);
            EXPECT_EQ(img->mPlane[img->U].mRowInc, stride / 2);

            EXPECT_EQ(img->mPlane[img->V].mOffset, (uint32_t)(stride * vstride * 5 / 4));
            EXPECT_EQ(img->mPlane[img->V].mColInc, 1);
            EXPECT_EQ(img->mPlane[img->V].mRowInc, stride / 2);

            EXPECT_GE(size, (size_t)(stride * vstride * 5 / 4 + stride / 2 * 119 + 160));
            EXPECT_LE(size, (size_t)(stride * vstride * 3 / 2));
            break;

        case COLOR_FormatYUV420SemiPlanar:
        case COLOR_FormatYUV420PackedSemiPlanar:
            EXPECT_EQ(img->mPlane[img->Y].mOffset, 0u);
            EXPECT_EQ(img->mPlane[img->Y].mColInc, 1);
            EXPECT_EQ(img->mPlane[img->Y].mRowInc, stride);

            EXPECT_EQ(img->mPlane[img->U].mOffset, (uint32_t)(stride * vstride));
            EXPECT_EQ(img->mPlane[img->U].mColInc, 2);
            EXPECT_EQ(img->mPlane[img->U].mRowInc, stride);

            EXPECT_EQ(img->mPlane[img->V].mOffset, (uint32_t)(stride * vstride + 1));
            EXPECT_EQ(img->mPlane[img->V].mColInc, 2);
            EXPECT_EQ(img->mPlane[img->V].mRowInc, stride);

            EXPECT_GE(size, (size_t)(stride * vstride + stride * 119 + 320));
            EXPECT_LE(size, (size_t)(stride * vstride * 3 / 2));
            break;

        case COLOR_FormatYUV420Flexible:
            // anything goes, but stride should match Y plane
            EXPECT_EQ(img->mPlane[img->Y].mRowInc, stride);

            EXPECT_GE(size,
                      std::max({
                            img->mPlane[img->Y].mOffset + 239 * img->mPlane[img->Y].mRowInc
                                    + 319 * img->mPlane[img->Y].mColInc + 1,
                            img->mPlane[img->U].mOffset + 119 * img->mPlane[img->U].mRowInc
                                    + 159 * img->mPlane[img->U].mColInc + 1,
                            img->mPlane[img->V].mOffset + 119 * img->mPlane[img->V].mRowInc
                                    + 159 * img->mPlane[img->V].mColInc + 1 }));
            break;

        default:
            break;
    }

    // validate all pixels
#if 0
    fprintf(stderr, "MediaImage { F(%ux%u) @%u+%d+%d @%u+%d+%d @%u+%d+%d }\n",
            img->mWidth, img->mHeight,
            img->mPlane[0].mOffset, img->mPlane[0].mColInc, img->mPlane[0].mRowInc,
            img->mPlane[1].mOffset, img->mPlane[1].mColInc, img->mPlane[1].mRowInc,
            img->mPlane[2].mOffset, img->mPlane[2].mColInc, img->mPlane[2].mRowInc);
#endif
    for (ix = 0; ix < 3; ++ix) {
        const static uint8_t expected[] = { 210, 16, 146 };
        for (uint32_t y = 0; y < img->mHeight / img->mPlane[ix].mVertSubsampling ; ++y) {
            for (uint32_t x = 0; x < img->mWidth / img->mPlane[ix].mHorizSubsampling; ++x) {
                uint8_t val = buf->data()[img->mPlane[ix].mOffset + img->mPlane[ix].mColInc * x
                        + img->mPlane[ix].mRowInc * y];
                ASSERT_EQ(val, expected[ix]) << "incorrect value for plane "
                        << ix << " at x=" << x << ", y=" << y;
            }
        }
    }
}

INSTANTIATE_TEST_CASE_P(InputSizes, MediaCodecByteBufferTest, ::testing::Values(
        -1,
        COLOR_FormatYUV420Planar,
        COLOR_FormatYUV420SemiPlanar,
        COLOR_FormatYUV420PackedPlanar,
        COLOR_FormatYUV420PackedSemiPlanar,
        COLOR_FormatYUV420Flexible));

} // namespace android
