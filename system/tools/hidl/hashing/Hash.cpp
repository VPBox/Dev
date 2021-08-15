/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include <hidl-hash/Hash.h>

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <map>
#include <regex>
#include <sstream>

#include <android-base/logging.h>
#include <openssl/sha.h>

namespace android {

const std::vector<uint8_t> Hash::kEmptyHash = std::vector<uint8_t>(SHA256_DIGEST_LENGTH, 0);

Hash& Hash::getMutableHash(const std::string& path) {
    static std::map<std::string, Hash> hashes;

    auto it = hashes.find(path);

    if (hashes.find(path) == hashes.end()) {
        it = hashes.insert(it, {path, Hash(path)});
    }

    return it->second;
}

const Hash& Hash::getHash(const std::string& path) {
    return getMutableHash(path);
}

void Hash::clearHash(const std::string& path) {
    getMutableHash(path).mHash = kEmptyHash;
}

static std::vector<uint8_t> sha256File(const std::string& path) {
    std::ifstream stream(path);
    std::stringstream fileStream;
    fileStream << stream.rdbuf();
    std::string fileContent = fileStream.str();

    std::vector<uint8_t> ret = std::vector<uint8_t>(SHA256_DIGEST_LENGTH);

    SHA256(reinterpret_cast<const uint8_t*>(fileContent.c_str()), fileContent.size(), ret.data());

    return ret;
}

Hash::Hash(const std::string& path) : mPath(path), mHash(sha256File(path)) {}

std::string Hash::hexString(const std::vector<uint8_t>& hash) {
    std::ostringstream s;
    s << std::hex << std::setfill('0');
    for (uint8_t i : hash) {
        s << std::setw(2) << static_cast<int>(i);
    }
    return s.str();
}

std::string Hash::hexString() const {
    return hexString(mHash);
}

const std::vector<uint8_t>& Hash::raw() const {
    return mHash;
}

const std::string& Hash::getPath() const {
    return mPath;
}

#define HASH "([0-9a-f]+)"
#define FQNAME "([^\\s]+)"
#define SPACES " +"
#define MAYBE_SPACES " *"
#define OPTIONAL_COMMENT "(?:#.*)?"
static const std::regex kHashLine("(?:" MAYBE_SPACES HASH SPACES FQNAME MAYBE_SPACES
                                  ")?" OPTIONAL_COMMENT);

struct HashFile {
    static const HashFile* parse(const std::string& path, std::string* err) {
        static std::map<std::string, HashFile*> hashfiles;
        auto it = hashfiles.find(path);

        if (it == hashfiles.end()) {
            it = hashfiles.insert(it, {path, readHashFile(path, err)});
        }

        return it->second;
    }

    std::vector<std::string> lookup(const std::string& fqName) const {
        auto it = hashes.find(fqName);

        if (it == hashes.end()) {
            return {};
        }

        return it->second;
    }

   private:
    static HashFile* readHashFile(const std::string& path, std::string* err) {
        std::ifstream stream(path);
        if (!stream) {
            return nullptr;
        }

        HashFile* file = new HashFile();
        file->path = path;

        std::string line;
        while (std::getline(stream, line)) {
            std::smatch match;
            bool valid = std::regex_match(line, match, kHashLine);

            if (!valid) {
                *err = "Error reading line from " + path + ": " + line;
                delete file;
                return nullptr;
            }

            CHECK_EQ(match.size(), 3u);

            std::string hash = match.str(1);
            std::string fqName = match.str(2);

            if (hash.size() == 0 && fqName.size() == 0) {
                continue;
            }

            if (hash.size() == 0 || fqName.size() == 0) {
                *err = "Hash or fqName empty on " + path + ": " + line;
                delete file;
                return nullptr;
            }

            file->hashes[fqName].push_back(hash);
        }
        return file;
    }

    std::string path;
    std::map<std::string, std::vector<std::string>> hashes;
};

std::vector<std::string> Hash::lookupHash(const std::string& path, const std::string& interfaceName,
                                          std::string* err, bool* fileExists) {
    *err = "";
    const HashFile* file = HashFile::parse(path, err);

    if (file == nullptr || err->size() > 0) {
        if (fileExists != nullptr) *fileExists = false;
        return {};
    }

    if (fileExists != nullptr) *fileExists = true;

    return file->lookup(interfaceName);
}

}  // namespace android
