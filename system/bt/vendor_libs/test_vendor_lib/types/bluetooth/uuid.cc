/******************************************************************************
 *
 *  Copyright (C) 2017 The Android Open Source Project
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#include "uuid.h"

#include <base/rand_util.h>
#include <base/strings/stringprintf.h>
#include <algorithm>

namespace bluetooth {

static_assert(sizeof(Uuid) == 16, "Uuid must be 16 bytes long!");

using UUID128Bit = Uuid::UUID128Bit;

const Uuid Uuid::kEmpty = Uuid::From128BitBE(UUID128Bit{{0x00}});

namespace {
constexpr Uuid kBase = Uuid::From128BitBE(
    UUID128Bit{{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x80, 0x5f, 0x9b, 0x34, 0xfb}});
}  // namespace

size_t Uuid::GetShortestRepresentationSize() const {
  if (memcmp(uu.data() + kNumBytes32, kBase.uu.data() + kNumBytes32, kNumBytes128 - kNumBytes32) != 0) {
    return kNumBytes128;
  }

  if (uu[0] == 0 && uu[1] == 0) return kNumBytes16;

  return kNumBytes32;
}

bool Uuid::Is16Bit() const {
  return GetShortestRepresentationSize() == kNumBytes16;
}

uint16_t Uuid::As16Bit() const {
  return (((uint16_t)uu[2]) << 8) + uu[3];
}

uint32_t Uuid::As32Bit() const {
  return (((uint32_t)uu[0]) << 24) + (((uint32_t)uu[1]) << 16) + (((uint32_t)uu[2]) << 8) + uu[3];
}

Uuid Uuid::FromString(const std::string& uuid, bool* is_valid) {
  if (is_valid) *is_valid = false;
  Uuid ret = kBase;

  if (uuid.empty()) return ret;

  uint8_t* p = ret.uu.data();
  if (uuid.size() == kString128BitLen) {
    if (uuid[8] != '-' || uuid[13] != '-' || uuid[18] != '-' || uuid[23] != '-') {
      return ret;
    }

    int c;
    int rc = sscanf(uuid.c_str(),
                    "%02hhx%02hhx%02hhx%02hhx-%02hhx%02hhx-%02hhx%02hhx"
                    "-%02hhx%02hhx-%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%n",
                    &p[0], &p[1], &p[2], &p[3], &p[4], &p[5], &p[6], &p[7], &p[8], &p[9], &p[10], &p[11], &p[12],
                    &p[13], &p[14], &p[15], &c);
    if (rc != 16) return ret;
    if (c != kString128BitLen) return ret;

    if (is_valid) *is_valid = true;
  } else if (uuid.size() == 8) {
    int c;
    int rc = sscanf(uuid.c_str(), "%02hhx%02hhx%02hhx%02hhx%n", &p[0], &p[1], &p[2], &p[3], &c);
    if (rc != 4) return ret;
    if (c != 8) return ret;

    if (is_valid) *is_valid = true;
  } else if (uuid.size() == 4) {
    int c;
    int rc = sscanf(uuid.c_str(), "%02hhx%02hhx%n", &p[2], &p[3], &c);
    if (rc != 2) return ret;
    if (c != 4) return ret;

    if (is_valid) *is_valid = true;
  }

  return ret;
}

Uuid Uuid::From16Bit(uint16_t uuid16) {
  Uuid u = kBase;

  u.uu[2] = (uint8_t)((0xFF00 & uuid16) >> 8);
  u.uu[3] = (uint8_t)(0x00FF & uuid16);
  return u;
}

Uuid Uuid::From32Bit(uint32_t uuid32) {
  Uuid u = kBase;

  u.uu[0] = (uint8_t)((0xFF000000 & uuid32) >> 24);
  u.uu[1] = (uint8_t)((0x00FF0000 & uuid32) >> 16);
  u.uu[2] = (uint8_t)((0x0000FF00 & uuid32) >> 8);
  u.uu[3] = (uint8_t)(0x000000FF & uuid32);
  return u;
}

Uuid Uuid::From128BitBE(const uint8_t* uuid) {
  UUID128Bit tmp;
  memcpy(tmp.data(), uuid, kNumBytes128);
  return From128BitBE(tmp);
}

Uuid Uuid::From128BitLE(const UUID128Bit& uuid) {
  Uuid u;
  std::reverse_copy(uuid.data(), uuid.data() + kNumBytes128, u.uu.begin());
  return u;
}

Uuid Uuid::From128BitLE(const uint8_t* uuid) {
  UUID128Bit tmp;
  memcpy(tmp.data(), uuid, kNumBytes128);
  return From128BitLE(tmp);
}

const UUID128Bit Uuid::To128BitLE() const {
  UUID128Bit le;
  std::reverse_copy(uu.data(), uu.data() + kNumBytes128, le.begin());
  return le;
}

const UUID128Bit& Uuid::To128BitBE() const {
  return uu;
}

Uuid Uuid::GetRandom() {
  Uuid uuid;
  base::RandBytes(uuid.uu.data(), uuid.uu.size());
  return uuid;
}

bool Uuid::IsEmpty() const {
  return *this == kEmpty;
}

bool Uuid::operator<(const Uuid& rhs) const {
  return std::lexicographical_compare(uu.begin(), uu.end(), rhs.uu.begin(), rhs.uu.end());
}

bool Uuid::operator==(const Uuid& rhs) const {
  return uu == rhs.uu;
}

bool Uuid::operator!=(const Uuid& rhs) const {
  return uu != rhs.uu;
}

std::string Uuid::ToString() const {
  return base::StringPrintf("%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x", uu[0], uu[1], uu[2],
                            uu[3], uu[4], uu[5], uu[6], uu[7], uu[8], uu[9], uu[10], uu[11], uu[12], uu[13], uu[14],
                            uu[15]);
}
}  // namespace bluetooth