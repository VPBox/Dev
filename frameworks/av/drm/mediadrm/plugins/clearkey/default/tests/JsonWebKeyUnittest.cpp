/*
 * Copyright (C) 2014 The Android Open Source Project
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
#include <utils/Log.h>

#include "JsonWebKey.h"

#include "gtest/gtest.h"
#include "Utils.h"

namespace clearkeydrm {
using android::String8;
using android::Vector;

class JsonWebKeyTest : public ::testing::Test {
 protected:
    JsonWebKey* jwk;

    JsonWebKeyTest() {
        jwk = new JsonWebKey;
    }

    virtual ~JsonWebKeyTest() {
        if (jwk)
            delete jwk;
    }
};

void stringFromVector(const Vector<uint8_t>& input,
        String8* converted) {
    converted->clear();
    if (input.isEmpty()) {
        return;
    }

    for (size_t i = 0; i < input.size(); ++i) {
        converted->appendFormat("%c", input.itemAt(i));
    }
}

void verifyKeys(const KeyMap& keys, const String8* clearKeys) {
    if (keys.isEmpty()) {
        return;
    }

    String8 keyString;
    for (size_t i = 0; i < keys.size(); ++i) {
        stringFromVector(keys.valueAt(i), &keyString);
        EXPECT_EQ(keyString, clearKeys[i]);
    }
}

TEST_F(JsonWebKeyTest, NoSymmetricKey) {
    const String8 js(
            "{"
                "[{"
                    "\"kty\":\"rsa\","
                    "\"alg\":\"A128KW1\","
                    "\"kid\":\"Y2xlYXJrZXlrZXlpZDAx\","
                    "\"k\":\"1-GawgguFyGrWKav7AX4VKUg\""
                "}]"
          "}");

    KeyMap keys;
    EXPECT_FALSE(jwk->extractKeysFromJsonWebKeySet(js, &keys));
    EXPECT_TRUE(keys.isEmpty());
}

TEST_F(JsonWebKeyTest, NoKeysTag) {
    const String8 js(
            "{"
                "[{"
                    "\"kty\":\"oct\","
                    "\"alg\":\"A128KW1\","
                    "\"kid\":\"Y2xlYXJrZXlrZXlpZDAx\","
                    "\"k\":\"1-GawgguFyGrWKav7AX4VKUg\""
                "},"
                "{"
                    "\"kty\":\"oct\","
                    "\"alg\":\"A128KW2\","
                    "\"k\":\"R29vZCBkYXkh\","
                    "\"kid\":\"Y2xlYXJrZXlrZXlpZDAy\""
                "}]"
            "}");

    KeyMap keys;
    EXPECT_FALSE(jwk->extractKeysFromJsonWebKeySet(js, &keys));
    EXPECT_TRUE(keys.isEmpty());
}

TEST_F(JsonWebKeyTest, NoKeyId) {
    const String8 js(
            "{"
                "\"keys\":"
                    "[{"
                        "\"kty\":\"oct\""
                        "\"alg\":\"A128KW1\""
                        "\"k\":\"SGVsbG8gRnJpZW5kISE=\""
                    "}"
                    "{"
                        "\"kty\":\"oct\""
                        "\"alg\":\"A128KW2\""
                        "\"k\":\"R29vZCBkYXkh\""
                        "\"kid\":\"Y2xlYXJrZXlrZXlpZDAy\""
                    "}]"
            "}");

    KeyMap keys;
    EXPECT_TRUE(jwk->extractKeysFromJsonWebKeySet(js, &keys));
    EXPECT_TRUE(keys.size() == 1);

    const String8 clearKeys("Good day!");
    verifyKeys(keys, &clearKeys);
}

TEST_F(JsonWebKeyTest, NoKey) {
    const String8 js(
            "{"
                "\"keys\":"
                    "[{"
                        "\"kty\":\"oct\""
                        "\"alg\":\"A128KW1\""
                        "\"kid\":\"`\""
                    "}"
                    "{"
                        "\"kty\":\"oct\""
                        "\"alg\":\"A128KW2\""
                        "\"k\":\"R29vZCBkYXkh\""
                        "\"kid\":\"Y2xlYXJrZXlrZXlpZDAy\""
                    "}]"
            "}");

    KeyMap keys;
    EXPECT_TRUE(jwk->extractKeysFromJsonWebKeySet(js, &keys));
    EXPECT_TRUE(keys.size() == 1);

    const String8 clearKeys("Good day!");
    verifyKeys(keys, &clearKeys);
}

TEST_F(JsonWebKeyTest, MalformedKey) {
    const String8 js(
            "{"
                "\"keys\":"
                    "[{"
                        "\"kty\":\"oct\""
                        "\"alg\":\"A128KW1\""
                        "\"k\":\"GawgguFyGrWKav7AX4V???\""
                        "\"kid\":\"67ef0gd8pvfd0=\""
                    "}"
                    "{"
                        "\"kty\":\"oct\""
                        "\"alg\":\"A128KW1\""
                        "\"k\":\"GawgguFyGrWKav7AX4V???\""
                        "\"kid\":"
                    "}"
                    "{"
                        "\"kty\":\"oct\""
                        "\"alg\":\"A128KW1\""
                        ":\"GawgguFyGrWKav7AX4V???\""
                        "\"kid\":\"67ef0gd8pvfd0=\""
                    "}"
                    "{"
                        "\"kty\":\"oct\""
                        "\"alg\":\"A128KW3\""
                        "\"kid\":\"Y2xlYXJrZXlrZXlpZDAz\""
                        "\"k\":\"R29vZCBkYXkh\""
                    "}]"
            "}");

    KeyMap keys;
    EXPECT_TRUE(jwk->extractKeysFromJsonWebKeySet(js, &keys));
    EXPECT_TRUE(keys.size() == 1);

    const String8 clearKeys("Good day!");
    verifyKeys(keys, &clearKeys);
}

TEST_F(JsonWebKeyTest, EmptyJsonWebKey) {
    const String8 js;
    KeyMap keys;
    EXPECT_FALSE(jwk->extractKeysFromJsonWebKeySet(js, &keys));
    EXPECT_TRUE(keys.isEmpty());
}

TEST_F(JsonWebKeyTest, MalformedJsonWebKey) {
    // Missing begin array '['
    const String8 js(
            "{"
                "\"keys\":"
                    "{"
                        "\"kty\":\"oct\""
                        "\"alg\":\"A128KW1\""
                        "\"k\":\"GawgguFyGrWKav7AX4VKUg\""
                        "\"kid\":\"67ef0gd8pvfd0=\""
                    "}"
            "]"
            "}");

    KeyMap keys;
    EXPECT_FALSE(jwk->extractKeysFromJsonWebKeySet(js, &keys));
    EXPECT_TRUE(keys.isEmpty());
}

TEST_F(JsonWebKeyTest, SameKeyId) {
    const String8 js(
            "{"
                "\"keys\":"
                    "[{"
                        "\"kty\":\"oct\""
                        "\"alg\":\"A128KW1\""
                        "\"kid\":\"Y2xlYXJrZXlrZXlpZDAx\""
                        "\"k\":\"SGVsbG8gRnJpZW5kISE\""
                    "}"
                    "{"
                        "\"kty\":\"oct\""
                        "\"alg\":\"A128KW1\""
                        "\"k\":\"SGVsbG8gRnJpZW5kIQ\""
                        "\"kid\":\"Y2xlYXJrZXlrZXlpZDAx\""
                    "}"
                    "{"
                        "\"kty\":\"oct\""
                        "\"alg\":\"A128KW3\""
                        "\"kid\":\"Y2xlYXJrZXlrZXlpZDAz\""
                        "\"k\":\"R29vZCBkYXkh\""
                    "}]"
            "}");

    KeyMap keys;
    jwk->extractKeysFromJsonWebKeySet(js, &keys);
    EXPECT_TRUE(keys.size() == 2);

    const String8 clearKeys[] =
            { String8("Hello Friend!"), String8("Good day!") };
    verifyKeys(keys, clearKeys);
}

TEST_F(JsonWebKeyTest, ExtractWellFormedKeys) {
    const String8 js(
            "{"
                "\"keys\":"
                    "[{"
                        "\"kty\":\"oct\""
                        "\"alg\":\"A128KW1\""
                    "}"
                    "{"
                        "\"kty\":\"oct\""
                        "\"alg\":\"A128KW2\""
                        "\"k\":\"SGVsbG8gRnJpZW5kIQ\""
                        "\"kid\":\"Y2xlYXJrZXlrZXlpZDAy\""
                    "}"
                    "{"
                        "\"kty\":\"oct\""
                        "\"alg\":\"A128KW3\""
                        "\"kid\":\"Y2xlYXJrZXlrZXlpZDAz\""
                        "\"k\":\"R29vZCBkYXkh\""
                    "}]"
            "}");

    KeyMap keys;
    jwk->extractKeysFromJsonWebKeySet(js, &keys);
    EXPECT_TRUE(keys.size() == 2);

    const String8 clearKeys[] =
            { String8("Hello Friend!"), String8("Good day!") };
    verifyKeys(keys, clearKeys);
}

TEST_F(JsonWebKeyTest, ExtractKeys) {
    const String8 js(
            "{"
                "\"keys\":"
                    "[{"
                        "\"kid\":\"Y2xlYXJrZXlrZXlpZDAx\""
                        "\"k\":\"SGVsbG8gRnJpZW5kICE-Pw\""
                        "\"kty\":\"oct\""
                        "\"alg\":\"A128KW1\""
                    "}"
                    "{"
                        "\"kty\":\"oct\""
                        "\"alg\":\"A128KW2\""
                        "\"k\":\"SGVsbG8gRnJpZW5kICE_\""
                        "\"kid\":\"Y2xlYXJrZXlrZXlpZDAy\""
                    "}"
                    "{"
                        "\"kty\":\"rsa\""
                        "\"alg\":\"A128KW-rsa\""
                        "\"k\":\"R29vZCBkYXkh\""
                        "\"kid\":\"rsa-67ef0gd8pvfd0=\""
                    "}"
                    "{"
                        "\"alg\":\"A128KW3\""
                        "\"kid\":\"Y2xlYXJrZXlrZXlpZDAz\""
                        "\"k\":\"SGVsbG8gPz4-IEZyaWVuZCA_Pg\""
                        "\"kty\":\"oct\""
                    "}]"
            "}");

    KeyMap keys;
    jwk->extractKeysFromJsonWebKeySet(js, &keys);
    EXPECT_TRUE(keys.size() == 3);

    const String8 clearKeys[] =
            { String8("Hello Friend !>?"), String8("Hello Friend !?"),
              String8("Hello ?>> Friend ?>") };
    verifyKeys(keys, clearKeys);
}

}  // namespace clearkeydrm
