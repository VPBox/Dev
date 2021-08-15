/*
 * Copyright (C) 2015 The Android Open Source Project
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

#define LOG_TAG "Minikin"

#include "FreeTypeMinikinFontForTest.h"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>

#include <ft2build.h>
#include <log/log.h>
#include FT_OUTLINE_H

#include "minikin/MinikinExtent.h"
#include "minikin/MinikinFont.h"
#include "minikin/MinikinPaint.h"
#include "minikin/MinikinRect.h"

namespace minikin {
namespace {

static int uniqueId = 0;

constexpr FT_Int32 LOAD_FLAG =
        FT_LOAD_NO_HINTING | FT_LOAD_NO_BITMAP | FT_LOAD_IGNORE_GLOBAL_ADVANCE_WIDTH;

constexpr float FTPosToFloat(FT_Pos x) {
    return x / 64.0;
}

constexpr FT_F26Dot6 FTFloatToF26Dot6(float x) {
    return static_cast<FT_F26Dot6>(x * 64);
}

void loadGlyphOrDie(uint32_t glyphId, float size, FT_Face face) {
    const FT_F26Dot6 scale = FTFloatToF26Dot6(size);
    LOG_ALWAYS_FATAL_IF(FT_Set_Char_Size(face, scale, scale, 72 /* dpi */, 72 /* dpi */),
                        "Failed to set character size.");
    LOG_ALWAYS_FATAL_IF(FT_Load_Glyph(face, glyphId, LOAD_FLAG), "Failed to load glyph");
    LOG_ALWAYS_FATAL_IF(face->glyph->format != FT_GLYPH_FORMAT_OUTLINE,
                        "Only outline font is supported.");
}

}  // namespace

FreeTypeMinikinFontForTest::FreeTypeMinikinFontForTest(const std::string& font_path, int index)
        : MinikinFont(uniqueId++), mFontPath(font_path), mFontIndex(index) {
    int fd = open(font_path.c_str(), O_RDONLY);
    LOG_ALWAYS_FATAL_IF(fd == -1, "Open failed: %s", font_path.c_str());
    struct stat st = {};
    LOG_ALWAYS_FATAL_IF(fstat(fd, &st) != 0);
    mFontSize = st.st_size;
    mFontData = mmap(NULL, mFontSize, PROT_READ, MAP_SHARED, fd, 0);
    LOG_ALWAYS_FATAL_IF(mFontData == nullptr);
    close(fd);

    LOG_ALWAYS_FATAL_IF(FT_Init_FreeType(&mFtLibrary), "Failed to initialize FreeType");

    FT_Open_Args args;
    args.flags = FT_OPEN_MEMORY;
    args.memory_base = static_cast<const FT_Byte*>(mFontData);
    args.memory_size = mFontSize;
    LOG_ALWAYS_FATAL_IF(FT_Open_Face(mFtLibrary, &args, index, &mFtFace), "Failed to open FT_Face");
}

FreeTypeMinikinFontForTest::~FreeTypeMinikinFontForTest() {
    FT_Done_Face(mFtFace);
    FT_Done_FreeType(mFtLibrary);
    munmap(mFontData, mFontSize);
}

float FreeTypeMinikinFontForTest::GetHorizontalAdvance(uint32_t glyphId, const MinikinPaint& paint,
                                                       const FontFakery& /* fakery */) const {
    loadGlyphOrDie(glyphId, paint.size, mFtFace);
    return FTPosToFloat(mFtFace->glyph->advance.x);
}

void FreeTypeMinikinFontForTest::GetBounds(MinikinRect* bounds, uint32_t glyphId,
                                           const MinikinPaint& paint,
                                           const FontFakery& /* fakery */) const {
    loadGlyphOrDie(glyphId, paint.size, mFtFace);

    FT_BBox bbox;
    FT_Outline_Get_CBox(&mFtFace->glyph->outline, &bbox);

    bounds->mLeft = FTPosToFloat(bbox.xMin);
    bounds->mTop = FTPosToFloat(bbox.yMax);
    bounds->mRight = FTPosToFloat(bbox.xMax);
    bounds->mBottom = FTPosToFloat(bbox.yMin);
}

void FreeTypeMinikinFontForTest::GetFontExtent(MinikinExtent* extent, const MinikinPaint& paint,
                                               const FontFakery& /* fakery */) const {
    float upem = mFtFace->units_per_EM;
    extent->ascent = -static_cast<float>(mFtFace->ascender) * paint.size / upem;
    extent->descent = -static_cast<float>(mFtFace->descender) * paint.size / upem;
}

}  // namespace minikin
