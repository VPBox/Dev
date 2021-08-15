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

#include <libxml/tree.h>
#include <log/log.h>
#include <unistd.h>

#include "minikin/FontCollection.h"
#include "minikin/FontFamily.h"
#include "minikin/LocaleList.h"

#include "FontTestUtils.h"
#include "FreeTypeMinikinFontForTest.h"
#include "LocaleListCache.h"
#include "MinikinInternal.h"

namespace minikin {

namespace {
std::string xmlTrim(const std::string& in) {
    if (in.empty()) {
        return in;
    }
    const char XML_SPACES[] = "\u0020\u000D\u000A\u0009";
    const size_t start = in.find_first_not_of(XML_SPACES);  // inclusive
    const size_t end = in.find_last_not_of(XML_SPACES);     // inclusive
    MINIKIN_ASSERT(start != std::string::npos, "Not a valid file name \"%s\"", in.c_str());
    MINIKIN_ASSERT(end != std::string::npos, "Not a valid file name \"%s\"", in.c_str());
    return in.substr(start, end - start + 1 /* +1 since end is inclusive */);
}

}  // namespace

std::vector<std::shared_ptr<FontFamily>> getFontFamilies(const std::string& fontDir,
                                                         const std::string& xmlPath) {
    xmlDoc* doc = xmlReadFile(xmlPath.c_str(), NULL, 0);
    xmlNode* familySet = xmlDocGetRootElement(doc);

    std::vector<std::shared_ptr<FontFamily>> families;
    for (xmlNode* familyNode = familySet->children; familyNode; familyNode = familyNode->next) {
        if (xmlStrcmp(familyNode->name, (const xmlChar*)"family") != 0) {
            continue;
        }

        xmlChar* variantXmlch = xmlGetProp(familyNode, (const xmlChar*)"variant");
        FamilyVariant variant = FamilyVariant::DEFAULT;
        if (variantXmlch) {
            if (xmlStrcmp(variantXmlch, (const xmlChar*)"elegant") == 0) {
                variant = FamilyVariant::ELEGANT;
            } else if (xmlStrcmp(variantXmlch, (const xmlChar*)"compact") == 0) {
                variant = FamilyVariant::COMPACT;
            }
        }

        std::vector<Font> fonts;
        for (xmlNode* fontNode = familyNode->children; fontNode; fontNode = fontNode->next) {
            if (xmlStrcmp(fontNode->name, (const xmlChar*)"font") != 0) {
                continue;
            }

            uint16_t weight = atoi((const char*)(xmlGetProp(fontNode, (const xmlChar*)"weight")));
            FontStyle::Slant italic = static_cast<FontStyle::Slant>(
                    xmlStrcmp(xmlGetProp(fontNode, (const xmlChar*)"style"),
                              (const xmlChar*)"italic") == 0);
            xmlChar* index = xmlGetProp(familyNode, (const xmlChar*)"index");

            xmlChar* fontFileName = xmlNodeListGetString(doc, fontNode->xmlChildrenNode, 1);
            const std::string fontPath = xmlTrim(fontDir + std::string((const char*)fontFileName));
            xmlFree(fontFileName);

            // TODO: Support font variation axis.

            if (access(fontPath.c_str(), R_OK) != 0) {
                ALOGW("%s is not found.", fontPath.c_str());
                continue;
            }

            FontStyle style(weight, italic);
            if (index == nullptr) {
                std::shared_ptr<MinikinFont> minikinFont =
                        std::make_shared<FreeTypeMinikinFontForTest>(fontPath);
                fonts.push_back(Font::Builder(minikinFont).setStyle(style).build());
            } else {
                std::shared_ptr<MinikinFont> minikinFont =
                        std::make_shared<FreeTypeMinikinFontForTest>(fontPath,
                                                                     atoi((const char*)index));
                fonts.push_back(Font::Builder(minikinFont).setStyle(style).build());
            }
        }

        xmlChar* lang = xmlGetProp(familyNode, (const xmlChar*)"lang");
        std::shared_ptr<FontFamily> family;
        if (lang == nullptr) {
            family = std::make_shared<FontFamily>(variant, std::move(fonts));
        } else {
            uint32_t langId = registerLocaleList(std::string((const char*)lang, xmlStrlen(lang)));
            family = std::make_shared<FontFamily>(langId, variant, std::move(fonts),
                                                  false /* isCustomFallback */);
        }
        families.push_back(family);
    }
    xmlFreeDoc(doc);
    return families;
}

std::shared_ptr<FontCollection> buildFontCollection(const std::string& filePath) {
    return std::make_shared<FontCollection>(buildFontFamily(filePath));
}

std::shared_ptr<FontFamily> buildFontFamily(const std::string& filePath) {
    auto font = std::make_shared<FreeTypeMinikinFontForTest>(getTestFontPath(filePath));
    std::vector<Font> fonts;
    fonts.push_back(Font::Builder(font).build());
    return std::make_shared<FontFamily>(std::move(fonts));
}

std::shared_ptr<FontFamily> buildFontFamily(const std::string& filePath, const std::string& lang,
                                            bool isCustomFallback) {
    auto font = std::make_shared<FreeTypeMinikinFontForTest>(getTestFontPath(filePath));
    std::vector<Font> fonts;
    fonts.push_back(Font::Builder(font).build());
    return std::make_shared<FontFamily>(LocaleListCache::getId(lang), FamilyVariant::DEFAULT,
                                        std::move(fonts), isCustomFallback);
}

}  // namespace minikin
