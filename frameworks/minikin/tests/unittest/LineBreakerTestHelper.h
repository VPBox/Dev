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

#include <gtest/gtest.h>

#include "minikin/Hyphenator.h"
#include "minikin/LineBreaker.h"

#include "LocaleListCache.h"
#include "MinikinInternal.h"
#include "UnicodeUtils.h"

namespace minikin {
namespace line_breaker_test_helper {

class RectangleLineWidth : public LineWidth {
public:
    RectangleLineWidth(float width) : mWidth(width) {}
    virtual ~RectangleLineWidth() {}

    float getAt(size_t) const override { return mWidth; }
    float getMin() const override { return mWidth; }

private:
    float mWidth;
};

// The run implemenataion for returning the same width for all characters.
class ConstantRun : public Run {
public:
    ConstantRun(const Range& range, const std::string& lang, float width, float ascent,
                float descent)
            : Run(range),
              mPaint(nullptr /* font collection */),
              mWidth(width),
              mAscent(ascent),
              mDescent(descent) {
        mLocaleListId = LocaleListCache::getId(lang);
    }

    virtual bool isRtl() const override { return false; }
    virtual bool canBreak() const override { return true; }
    virtual uint32_t getLocaleListId() const { return mLocaleListId; }

    virtual void getMetrics(const U16StringPiece&, std::vector<float>* advances, LayoutPieces*,
                            LayoutPieces*) const {
        std::fill(advances->begin() + mRange.getStart(), advances->begin() + mRange.getEnd(),
                  mWidth);
    }

    virtual std::pair<float, MinikinRect> getBounds(const U16StringPiece& /* text */,
                                                    const Range& /* range */,
                                                    const LayoutPieces& /* pieces */) const {
        return std::make_pair(mWidth, MinikinRect());
    }

    virtual MinikinExtent getExtent(const U16StringPiece& /* text */, const Range& /* range */,
                                    const LayoutPieces& /* pieces */) const override {
        return {mAscent, mDescent};
    }

    virtual const MinikinPaint* getPaint() const { return &mPaint; }

    virtual float measureHyphenPiece(const U16StringPiece&, const Range& range,
                                     StartHyphenEdit start, EndHyphenEdit end,
                                     LayoutPieces*) const {
        uint32_t extraCharForHyphen = 0;
        if (isInsertion(start)) {
            extraCharForHyphen++;
        }
        if (isInsertion(end)) {
            extraCharForHyphen++;
        }
        return mWidth * (range.getLength() + extraCharForHyphen);
    }

    virtual void appendLayout(const U16StringPiece&, const Range&, const Range&,
                              const LayoutPieces&, const MinikinPaint&, uint32_t, StartHyphenEdit,
                              EndHyphenEdit, Layout*) const {}

private:
    MinikinPaint mPaint;
    uint32_t mLocaleListId;
    float mWidth;
    float mAscent;
    float mDescent;
};

struct LineBreakExpectation {
    std::string mLineContent;
    float mWidth;
    StartHyphenEdit mStartEdit;
    EndHyphenEdit mEndEdit;
    float mAscent;
    float mDescent;
};

static bool sameLineBreak(const std::vector<LineBreakExpectation>& expected,
                          const LineBreakResult& actual) {
    if (expected.size() != actual.breakPoints.size()) {
        return false;
    }

    uint32_t breakOffset = 0;
    for (uint32_t i = 0; i < expected.size(); ++i) {
        std::vector<uint16_t> u16Str = utf8ToUtf16(expected[i].mLineContent);

        // The expected string contains auto inserted hyphen. Remove it for computing offset.
        uint32_t lineLength = u16Str.size();
        if (isInsertion(expected[i].mStartEdit)) {
            if (u16Str[0] != '-') {
                return false;
            }
            --lineLength;
        }
        if (isInsertion(expected[i].mEndEdit)) {
            if (u16Str.back() != '-') {
                return false;
            }
            --lineLength;
        }
        breakOffset += lineLength;

        if (breakOffset != static_cast<uint32_t>(actual.breakPoints[i])) {
            return false;
        }
        if (expected[i].mWidth != actual.widths[i]) {
            return false;
        }
        HyphenEdit edit = static_cast<HyphenEdit>(actual.flags[i] & 0xFF);
        if (expected[i].mStartEdit != startHyphenEdit(edit)) {
            return false;
        }
        if (expected[i].mEndEdit != endHyphenEdit(edit)) {
            return false;
        }
        if (expected[i].mAscent != actual.ascents[i]) {
            return false;
        }
        if (expected[i].mDescent != actual.descents[i]) {
            return false;
        }
    }
    return true;
}

// Make debug string.
static std::string toString(const std::vector<LineBreakExpectation>& lines) {
    std::string out;
    for (uint32_t i = 0; i < lines.size(); ++i) {
        const LineBreakExpectation& line = lines[i];

        char lineMsg[128] = {};
        snprintf(lineMsg, sizeof(lineMsg),
                 "Line %2d, Width: %5.1f, Hyphen(%hhu, %hhu), Extent(%5.1f, %5.1f), Text: \"%s\"\n",
                 i, line.mWidth, line.mStartEdit, line.mEndEdit, line.mAscent, line.mDescent,
                 line.mLineContent.c_str());
        out += lineMsg;
    }
    return out;
}

// Make debug string.
static std::string toString(const U16StringPiece& textBuf, const LineBreakResult& lines) {
    std::string out;
    for (uint32_t i = 0; i < lines.breakPoints.size(); ++i) {
        const Range textRange(i == 0 ? 0 : lines.breakPoints[i - 1], lines.breakPoints[i]);
        const HyphenEdit edit = static_cast<HyphenEdit>(lines.flags[i] & 0xFF);

        const StartHyphenEdit startEdit = startHyphenEdit(edit);
        const EndHyphenEdit endEdit = endHyphenEdit(edit);
        std::string hyphenatedStr = utf16ToUtf8(textBuf.substr(textRange));

        if (isInsertion(startEdit)) {
            hyphenatedStr.insert(0, "-");
        }
        if (isInsertion(endEdit)) {
            hyphenatedStr.push_back('-');
        }
        char lineMsg[128] = {};
        snprintf(lineMsg, sizeof(lineMsg),
                 "Line %2d, Width: %5.1f, Hyphen(%hhu, %hhu), Extent(%5.1f, %5.1f), Text: \"%s\"\n",
                 i, lines.widths[i], startEdit, endEdit, lines.ascents[i], lines.descents[i],
                 hyphenatedStr.c_str());
        out += lineMsg;
    }
    return out;
}

}  // namespace line_breaker_test_helper
}  // namespace minikin
