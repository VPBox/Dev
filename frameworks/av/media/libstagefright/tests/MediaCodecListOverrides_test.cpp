/*
 * Copyright 2015 The Android Open Source Project
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

// #define LOG_NDEBUG 0
#define LOG_TAG "MediaCodecListOverrides_test"
#include <utils/Log.h>

#include <gtest/gtest.h>

#include "MediaCodecListOverrides.h"

#include <media/MediaCodecInfo.h>
#include <media/stagefright/foundation/AMessage.h>
#include <media/stagefright/MediaCodecList.h>

#include <vector>

namespace android {

static const char kTestOverridesStr[] =
"<MediaCodecs>\n"
"    <Settings>\n"
"        <Setting name=\"supports-multiple-secure-codecs\" value=\"false\" />\n"
"        <Setting name=\"supports-secure-with-non-secure-codec\" value=\"true\" />\n"
"    </Settings>\n"
"    <Encoders>\n"
"        <MediaCodec name=\"OMX.qcom.video.encoder.avc\" type=\"video/avc\" update=\"true\" >\n"
"            <Limit name=\"max-supported-instances\" value=\"4\" />\n"
"        </MediaCodec>\n"
"        <MediaCodec name=\"OMX.qcom.video.encoder.mpeg4\" type=\"video/mp4v-es\" update=\"true\" >\n"
"            <Limit name=\"max-supported-instances\" value=\"4\" />\n"
"        </MediaCodec>\n"
"    </Encoders>\n"
"    <Decoders>\n"
"        <MediaCodec name=\"OMX.qcom.video.decoder.avc.secure\" type=\"video/avc\" update=\"true\" >\n"
"            <Limit name=\"max-supported-instances\" value=\"1\" />\n"
"        </MediaCodec>\n"
"        <MediaCodec name=\"OMX.qcom.video.decoder.h263\" type=\"video/3gpp\" update=\"true\" >\n"
"            <Limit name=\"max-supported-instances\" value=\"4\" />\n"
"        </MediaCodec>\n"
"        <MediaCodec name=\"OMX.qcom.video.decoder.mpeg2\" type=\"video/mpeg2\" update=\"true\" >\n"
"            <Limit name=\"max-supported-instances\" value=\"3\" />\n"
"        </MediaCodec>\n"
"        <MediaCodec name=\"OMX.qcom.video.decoder.mpeg4\" type=\"video/mp4v-es\" update=\"true\" >\n"
"            <Limit name=\"max-supported-instances\" value=\"3\" />\n"
"        </MediaCodec>\n"
"    </Decoders>\n"
"</MediaCodecs>\n";

class MediaCodecListOverridesTest : public ::testing::Test {
public:
    MediaCodecListOverridesTest() {}

    void addMaxInstancesSetting(
            const AString &key,
            const AString &value,
            KeyedVector<AString, CodecSettings> *results) {
        CodecSettings settings;
        settings.add("max-supported-instances", value);
        results->add(key, settings);
    }

    void verifyProfileResults(const KeyedVector<AString, CodecSettings> &results) {
        EXPECT_LT(0u, results.size());
        for (size_t i = 0; i < results.size(); ++i) {
            const AString &key = results.keyAt(i);
            const CodecSettings &settings = results.valueAt(i);
            EXPECT_EQ(1u, settings.size());
            EXPECT_TRUE(settings.keyAt(0) == "max-supported-instances");
            const AString &valueS = settings.valueAt(0);
            int32_t value = strtol(valueS.c_str(), NULL, 10);
            EXPECT_LT(0, value);
            ALOGV("profileCodecs results %s %s", key.c_str(), valueS.c_str());
        }
    }

    void exportTestResultsToXML(const char *fileName) {
        CodecSettings gR;
        gR.add("supports-multiple-secure-codecs", "false");
        gR.add("supports-secure-with-non-secure-codec", "true");
        KeyedVector<AString, CodecSettings> eR;
        addMaxInstancesSetting("OMX.qcom.video.encoder.avc video/avc", "4", &eR);
        addMaxInstancesSetting("OMX.qcom.video.encoder.mpeg4 video/mp4v-es", "4", &eR);
        KeyedVector<AString, CodecSettings> dR;
        addMaxInstancesSetting("OMX.qcom.video.decoder.avc.secure video/avc", "1", &dR);
        addMaxInstancesSetting("OMX.qcom.video.decoder.h263 video/3gpp", "4", &dR);
        addMaxInstancesSetting("OMX.qcom.video.decoder.mpeg2 video/mpeg2", "3", &dR);
        addMaxInstancesSetting("OMX.qcom.video.decoder.mpeg4 video/mp4v-es", "3", &dR);

        exportResultsToXML(fileName, gR, eR, dR);
    }
};

TEST_F(MediaCodecListOverridesTest, splitString) {
    AString s = "abc123";
    AString delimiter = " ";
    AString s1;
    AString s2;
    EXPECT_FALSE(splitString(s, delimiter, &s1, &s2));
    s = "abc 123";
    EXPECT_TRUE(splitString(s, delimiter, &s1, &s2));
    EXPECT_TRUE(s1 == "abc");
    EXPECT_TRUE(s2 == "123");
}

// TODO: the codec component never returns OMX_EventCmdComplete in unit test.
TEST_F(MediaCodecListOverridesTest, DISABLED_profileCodecs) {
    sp<IMediaCodecList> list = MediaCodecList::getInstance();
    std::vector<sp<MediaCodecInfo>> infos;
    for (size_t i = 0; i < list->countCodecs(); ++i) {
        infos.push_back(list->getCodecInfo(i));
    }
    CodecSettings global_results;
    KeyedVector<AString, CodecSettings> encoder_results;
    KeyedVector<AString, CodecSettings> decoder_results;
    profileCodecs(
            infos, &global_results, &encoder_results, &decoder_results, true /* forceToMeasure */);
    verifyProfileResults(encoder_results);
    verifyProfileResults(decoder_results);
}

TEST_F(MediaCodecListOverridesTest, exportTestResultsToXML) {
    const char *fileName = "/sdcard/mediacodec_list_overrides_test.xml";
    remove(fileName);

    exportTestResultsToXML(fileName);

    // verify
    AString overrides;
    FILE *f = fopen(fileName, "rb");
    ASSERT_TRUE(f != NULL);
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char *buf = (char *)malloc(size);
    EXPECT_EQ((size_t)1, fread(buf, size, 1, f));
    overrides.setTo(buf, size);
    fclose(f);
    free(buf);

    AString expected;
    expected.append(getProfilingVersionString());
    expected.append("\n");
    expected.append(kTestOverridesStr);
    EXPECT_TRUE(overrides == expected);

    remove(fileName);
}

} // namespace android
