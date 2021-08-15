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

#ifndef MINIKIN_FONT_TEST_UTILS_H
#define MINIKIN_FONT_TEST_UTILS_H

#include <memory>

#include "minikin/FontCollection.h"

#include "PathUtils.h"

namespace minikin {

/**
 * Returns list of FontFamily from installed fonts.
 *
 * This function reads an XML file and makes font families.
 */
std::vector<std::shared_ptr<FontFamily>> getFontFamilies(const std::string& fontDir,
                                                         const std::string& xmlAbsPath);

/**
 * Returns FontCollection from installed fonts.
 *
 * This function reads an XML file and makes font families and collections of them.
 * The XML path and font files are needed to be in the test data directory.
 */
inline std::shared_ptr<FontCollection> buildFontCollectionFromXml(const std::string& xmlPath) {
    return std::make_shared<FontCollection>(
            getFontFamilies(getTestDataDir(), getTestDataDir() + xmlPath));
}

/**
 * Build new FontCollection from single file.
 * The font file needs to be in the test data directory.
 */
std::shared_ptr<FontCollection> buildFontCollection(const std::string& filePath);

/**
 * Build new FontFamily from single file.
 * The font file needs to be in the test data directory.
 */
std::shared_ptr<FontFamily> buildFontFamily(const std::string& filePath);

/**
 * Build new FontFamily from single file with locale.
 */
std::shared_ptr<FontFamily> buildFontFamily(const std::string& filePath, const std::string& lang,
                                            bool isCustomFallback);

/**
 * Build new FontFamily from single file with locale.
 */
inline std::shared_ptr<FontFamily> buildFontFamily(const std::string& filePath,
                                                   const std::string& lang) {
    return buildFontFamily(filePath, lang, false /* isCustomFallback */);
}

}  // namespace minikin
#endif  // MINIKIN_FONT_TEST_UTILS_H
