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

#define __C2_GENERATE_GLOBAL_VARS__

#include <set>

#include <gtest/gtest.h>

#include <C2ParamDef.h>

#include <media/stagefright/foundation/ABuffer.h>
#include <media/stagefright/foundation/hexdump.h>
#include <ReflectedParamUpdater.h>

namespace android {

namespace {

enum {
    kParamIndexTestStart = 0x1000,
    kParamIndexInt,
    kParamIndexString,
    kParamIndexComposite,
    kParamIndexFlexString,

    kParamIndexLong = C2Param::TYPE_INDEX_VENDOR_START,
};

typedef C2GlobalParam<C2Info, C2Int32Value, kParamIndexInt> C2IntInfo;
typedef C2GlobalParam<C2Info, C2Int64Value, kParamIndexLong> C2LongInfo;

struct C2FixedSizeStringStruct {
    char value[12];

    DEFINE_AND_DESCRIBE_BASE_C2STRUCT(FixedSizeString)
    C2FIELD(value, "value")
};
typedef C2GlobalParam<C2Info, C2FixedSizeStringStruct, kParamIndexString> C2StringInfo;

struct C2CompositeStruct {
    int32_t i32;
    uint64_t u64;
    char str[12];
    uint8_t blob[8];
    uint8_t flexBlob[];

    C2CompositeStruct() = default;

    DEFINE_AND_DESCRIBE_BASE_FLEX_C2STRUCT(Composite, flexBlob)
    C2FIELD(i32, "i32")
    C2FIELD(u64, "u64")
    C2FIELD(str, "str")
    C2FIELD(blob, "blob")
    C2FIELD(flexBlob, "flex-blob")
};
static_assert(C2CompositeStruct::FLEX_SIZE == 1, "");
static_assert(_C2FlexHelper<C2CompositeStruct>::FLEX_SIZE == 1, "");
typedef C2GlobalParam<C2Info, C2CompositeStruct, kParamIndexComposite> C2CompositeInfo;

typedef C2GlobalParam<C2Info, C2StringValue, kParamIndexFlexString> C2FlexStringInfo;

#define SUPPORTED_TYPES   \
    C2IntInfo,            \
    C2LongInfo,           \
    C2StringInfo,         \
    C2CompositeInfo,      \
    C2FlexStringInfo

template<typename... TYPES> struct describe_impl;
template<typename T, typename... TYPES> struct describe_impl<T, TYPES...> {
    static std::unique_ptr<C2StructDescriptor> describe(C2Param::CoreIndex index) {
        if (index == T::CORE_INDEX) {
            return std::make_unique<C2StructDescriptor>(T::CORE_INDEX, T::FieldList());
        } else {
            return describe_impl<TYPES...>::describe(index);
        }
    }
};

template<> struct describe_impl<> {
    static std::unique_ptr<C2StructDescriptor> describe(C2Param::CoreIndex) {
        return nullptr;
    }
};

template<typename T> const char *GetName()        { return nullptr; }
template<> const char *GetName<C2IntInfo>()       { return "int"; }
template<> const char *GetName<C2LongInfo>()      { return "long"; }
template<> const char *GetName<C2StringInfo>()    { return "string"; }
template<> const char *GetName<C2CompositeInfo>() { return "composite"; }
template<> const char *GetName<C2FlexStringInfo>() { return "flex-string"; }

template<typename... TYPES> struct fill_descriptors_impl;
template<typename T, typename... TYPES> struct fill_descriptors_impl<T, TYPES...> {
    static void fill(std::vector<std::shared_ptr<C2ParamDescriptor>> *vec) {
        fill_descriptors_impl<TYPES...>::fill(vec);
        vec->push_back(std::make_shared<C2ParamDescriptor>(
                T::PARAM_TYPE, C2ParamDescriptor::IS_PERSISTENT, GetName<T>()));
    }
};

template<> struct fill_descriptors_impl<> {
    static void fill(std::vector<std::shared_ptr<C2ParamDescriptor>> *) {}
};

template<typename T> T *CastParam(const std::unique_ptr<C2Param> &param) {
    return (T *)param.get();
}

class ParamReflector : public C2ParamReflector {
public:
    ParamReflector() = default;
    ~ParamReflector() override = default;

    std::unique_ptr<C2StructDescriptor> describe(C2Param::CoreIndex paramIndex) const override {
        return describe_impl<SUPPORTED_TYPES>::describe(paramIndex);
    }
};

}  // namespace

class ReflectedParamUpdaterTest : public ::testing::Test {
public:
    ReflectedParamUpdaterTest() : mReflector(new ParamReflector) {
        fill_descriptors_impl<SUPPORTED_TYPES>::fill(&mDescriptors);
    }

    std::shared_ptr<C2ParamReflector> mReflector;
    std::vector<std::shared_ptr<C2ParamDescriptor>> mDescriptors;
};

TEST_F(ReflectedParamUpdaterTest, SingleValueTest) {
    ReflectedParamUpdater updater;

    ReflectedParamUpdater::Dict msg;
    msg.emplace("int.value", int32_t(12));
    msg.emplace("vendor.long.value", int64_t(34));

    updater.addParamDesc(mReflector, mDescriptors);

    std::vector<C2Param::Index> indices;
    updater.getParamIndicesFromMessage(msg, &indices);
    EXPECT_EQ(1, std::count_if(indices.begin(), indices.end(),
            [](const auto &value) { return (uint32_t)value == C2IntInfo::PARAM_TYPE; }));
    EXPECT_EQ(1, std::count_if(indices.begin(), indices.end(),
            [](const auto &value) { return (uint32_t)value == C2LongInfo::PARAM_TYPE; }));
    EXPECT_EQ(0, std::count_if(indices.begin(), indices.end(),
            [](const auto &value) { return (uint32_t)value == C2StringInfo::PARAM_TYPE; }));
    EXPECT_EQ(0, std::count_if(indices.begin(), indices.end(),
            [](const auto &value) { return (uint32_t)value == C2CompositeInfo::PARAM_TYPE; }));
    EXPECT_EQ(0, std::count_if(indices.begin(), indices.end(),
            [](const auto &value) { return (uint32_t)value == C2FlexStringInfo::PARAM_TYPE; }));

    std::vector<std::unique_ptr<C2Param>> params;
    params.emplace_back(new C2IntInfo);
    params.emplace_back(new C2LongInfo);
    EXPECT_EQ(0, CastParam<C2IntInfo>(params[0])->value);
    EXPECT_EQ(0, CastParam<C2LongInfo>(params[1])->value);

    updater.updateParamsFromMessage(msg, &params);
    EXPECT_EQ(12, CastParam<C2IntInfo>(params[0])->value);
    EXPECT_EQ(34, CastParam<C2LongInfo>(params[1])->value);

    C2Value c2Value;
    int32_t int32Value = 0;
    int64_t int64Value = 0;
    msg = updater.getParams(params);
    ASSERT_EQ(1u, msg.count("int.value"));
    EXPECT_EQ(true, msg["int.value"].find(&c2Value));
    EXPECT_EQ(true, c2Value.get(&int32Value));
    EXPECT_EQ(12, int32Value);

    ASSERT_EQ(1u, msg.count("vendor.long.value"));
    EXPECT_EQ(true, msg["vendor.long.value"].find(&c2Value));
    EXPECT_EQ(true, c2Value.get(&int64Value));
    EXPECT_EQ(34, int64Value);
}

TEST_F(ReflectedParamUpdaterTest, StringTest) {
    ReflectedParamUpdater updater;

    ReflectedParamUpdater::Dict msg;
    msg.emplace("string.value", AString("56"));
    msg.emplace("flex-string.value", AString("Some string"));
    updater.addParamDesc(mReflector, mDescriptors);

    std::vector<C2Param::Index> indices;
    updater.getParamIndicesFromMessage(msg, &indices);
    EXPECT_EQ(0, std::count_if(indices.begin(), indices.end(),
            [](const auto &value) { return (uint32_t)value == C2IntInfo::PARAM_TYPE; }));
    EXPECT_EQ(0, std::count_if(indices.begin(), indices.end(),
            [](const auto &value) { return (uint32_t)value == C2LongInfo::PARAM_TYPE; }));
    EXPECT_EQ(1, std::count_if(indices.begin(), indices.end(),
            [](const auto &value) { return (uint32_t)value == C2StringInfo::PARAM_TYPE; }));
    EXPECT_EQ(0, std::count_if(indices.begin(), indices.end(),
            [](const auto &value) { return (uint32_t)value == C2CompositeInfo::PARAM_TYPE; }));
    EXPECT_EQ(1, std::count_if(indices.begin(), indices.end(),
            [](const auto &value) { return (uint32_t)value == C2FlexStringInfo::PARAM_TYPE; }));

    std::vector<std::unique_ptr<C2Param>> params;
    params.emplace_back(new C2StringInfo);
    EXPECT_EQ(0, CastParam<C2StringInfo>(params[0])->value[0]);
    params.emplace_back(C2FlexStringInfo::AllocUnique(0));
    EXPECT_EQ(0u, CastParam<C2FlexStringInfo>(params[1])->flexCount());
    char *flexStringData = &CastParam<C2FlexStringInfo>(params[1])->m.value[0];

    updater.updateParamsFromMessage(msg, &params);
    EXPECT_STREQ("56", CastParam<C2StringInfo>(params[0])->value);
    EXPECT_EQ(12u, CastParam<C2FlexStringInfo>(params[0])->flexCount());
    EXPECT_STREQ("Some string", CastParam<C2FlexStringInfo>(params[1])->m.value);
    EXPECT_NE(flexStringData, &CastParam<C2FlexStringInfo>(params[1])->m.value[0]);
    flexStringData = &CastParam<C2FlexStringInfo>(params[1])->m.value[0];

    // verify truncation and in-place update
    msg["string.value"] = ReflectedParamUpdater::Value(AString("1234567890ABCDE"));
    msg["flex-string.value"] = ReflectedParamUpdater::Value(AString("abc"));
    updater.updateParamsFromMessage(msg, &params);
    EXPECT_STREQ("1234567890A", CastParam<C2StringInfo>(params[0])->value);
    EXPECT_EQ(4u, CastParam<C2FlexStringInfo>(params[1])->flexCount());
    EXPECT_STREQ("abc", CastParam<C2FlexStringInfo>(params[1])->m.value);
    EXPECT_EQ(flexStringData, &CastParam<C2FlexStringInfo>(params[1])->m.value[0]);

    AString strValue;
    msg = updater.getParams(params);
    ASSERT_EQ(1u, msg.count("string.value"));
    EXPECT_EQ(true, msg["string.value"].find(&strValue));
    EXPECT_STREQ("1234567890A", strValue.c_str());

    ASSERT_EQ(1u, msg.count("flex-string.value"));
    EXPECT_EQ(true, msg["flex-string.value"].find(&strValue));
    EXPECT_STREQ("abc", strValue.c_str());
}

TEST_F(ReflectedParamUpdaterTest, CompositeTest) {
    ReflectedParamUpdater updater;

    ReflectedParamUpdater::Dict msg;
    msg.emplace("composite.i32", int32_t(78));
    msg.emplace("composite.u64", int64_t(910));
    msg.emplace("composite.str", AString("1112"));
    msg.emplace("composite.blob", ABuffer::CreateAsCopy("buffer08", 8));
    msg.emplace("composite.flex-blob", ABuffer::CreateAsCopy("flex-buffer-14", 14));

    updater.addParamDesc(mReflector, mDescriptors);

    std::vector<C2Param::Index> indices;
    updater.getParamIndicesFromMessage(msg, &indices);
    EXPECT_EQ(0, std::count_if(indices.begin(), indices.end(),
            [](const auto &value) { return (uint32_t)value == C2IntInfo::PARAM_TYPE; }));
    EXPECT_EQ(0, std::count_if(indices.begin(), indices.end(),
            [](const auto &value) { return (uint32_t)value == C2LongInfo::PARAM_TYPE; }));
    EXPECT_EQ(0, std::count_if(indices.begin(), indices.end(),
            [](const auto &value) { return (uint32_t)value == C2StringInfo::PARAM_TYPE; }));
    EXPECT_EQ(1, std::count_if(indices.begin(), indices.end(),
            [](const auto &value) { return (uint32_t)value == C2CompositeInfo::PARAM_TYPE; }));

    std::vector<std::unique_ptr<C2Param>> params;
    params.emplace_back(C2CompositeInfo::AllocUnique(0));
    EXPECT_EQ(0, CastParam<C2CompositeInfo>(params[0])->m.i32);
    EXPECT_EQ(0u, CastParam<C2CompositeInfo>(params[0])->m.u64);
    EXPECT_EQ(0, CastParam<C2CompositeInfo>(params[0])->m.str[0]);
    EXPECT_EQ(0, memcmp("\0\0\0\0\0\0\0\0", CastParam<C2CompositeInfo>(params[0])->m.blob, 8));
    EXPECT_EQ(0u, CastParam<C2CompositeInfo>(params[0])->flexCount());
    uint8_t *flexBlobData = &CastParam<C2CompositeInfo>(params[0])->m.flexBlob[0];

    updater.updateParamsFromMessage(msg, &params);
    EXPECT_EQ(78, CastParam<C2CompositeInfo>(params[0])->m.i32);
    EXPECT_EQ(910u, CastParam<C2CompositeInfo>(params[0])->m.u64);
    EXPECT_STREQ("1112", CastParam<C2CompositeInfo>(params[0])->m.str);
    EXPECT_EQ(0, memcmp("buffer08", CastParam<C2CompositeInfo>(params[0])->m.blob, 8));
    AString hex;
    hexdump(CastParam<C2CompositeInfo>(params[0])->m.blob, 8, 0, &hex);
    printf("%s\n", hex.c_str());
    ASSERT_EQ(14u, CastParam<C2CompositeInfo>(params[0])->flexCount());
    EXPECT_EQ(0, memcmp("flex-buffer-14", CastParam<C2CompositeInfo>(params[0])->m.flexBlob, 14));
    EXPECT_NE(flexBlobData, &CastParam<C2CompositeInfo>(params[0])->m.flexBlob[0]);
    flexBlobData = &CastParam<C2CompositeInfo>(params[0])->m.flexBlob[0];

    // test setting and zero extending shorter blob than allowed
    msg.clear();
    msg.emplace("composite.blob", ABuffer::CreateAsCopy("buf05", 5));
    updater.updateParamsFromMessage(msg, &params);
    EXPECT_EQ(0, memcmp("buf05\0\0\0", CastParam<C2CompositeInfo>(params[0])->m.blob, 8));
    ASSERT_EQ(14u, CastParam<C2CompositeInfo>(params[0])->flexCount());
    EXPECT_EQ(0, memcmp("flex-buffer-14", CastParam<C2CompositeInfo>(params[0])->m.flexBlob, 14));
    EXPECT_EQ(flexBlobData, &CastParam<C2CompositeInfo>(params[0])->m.flexBlob[0]);

    // test setting and trimming larger blob than allowed
    msg.clear();
    msg.emplace("composite.blob", ABuffer::CreateAsCopy("ReallyLongBuffer", 16));
    updater.updateParamsFromMessage(msg, &params);
    EXPECT_EQ(0, memcmp("ReallyLo", CastParam<C2CompositeInfo>(params[0])->m.blob, 8));
    ASSERT_EQ(14u, CastParam<C2CompositeInfo>(params[0])->flexCount());
    EXPECT_EQ(0, memcmp("flex-buffer-14", CastParam<C2CompositeInfo>(params[0])->m.flexBlob, 14));
    EXPECT_EQ(flexBlobData, &CastParam<C2CompositeInfo>(params[0])->m.flexBlob[0]);

    // test trimming flex blob in-place
    msg.clear();
    msg.emplace("composite.flex-blob", ABuffer::CreateAsCopy("buf05", 5));
    updater.updateParamsFromMessage(msg, &params);
    ASSERT_EQ(5u, CastParam<C2CompositeInfo>(params[0])->flexCount());
    EXPECT_EQ(0, memcmp("buf05", CastParam<C2CompositeInfo>(params[0])->m.flexBlob, 5));
    EXPECT_EQ(flexBlobData, &CastParam<C2CompositeInfo>(params[0])->m.flexBlob[0]);
}

TEST_F(ReflectedParamUpdaterTest, CompositePartialTest) {
    ReflectedParamUpdater updater;

    ReflectedParamUpdater::Dict msg;
    msg.emplace("composite.i32", C2Value(1314));
    msg.emplace("composite.str", AString("1516"));

    updater.addParamDesc(mReflector, mDescriptors);

    std::vector<C2Param::Index> indices;
    updater.getParamIndicesFromMessage(msg, &indices);
    EXPECT_EQ(0, std::count_if(indices.begin(), indices.end(),
            [](const auto &value) { return (uint32_t)value == C2IntInfo::PARAM_TYPE; }));
    EXPECT_EQ(0, std::count_if(indices.begin(), indices.end(),
            [](const auto &value) { return (uint32_t)value == C2LongInfo::PARAM_TYPE; }));
    EXPECT_EQ(0, std::count_if(indices.begin(), indices.end(),
            [](const auto &value) { return (uint32_t)value == C2StringInfo::PARAM_TYPE; }));
    EXPECT_EQ(1, std::count_if(indices.begin(), indices.end(),
            [](const auto &value) { return (uint32_t)value == C2CompositeInfo::PARAM_TYPE; }));

    std::vector<std::unique_ptr<C2Param>> params;
    params.emplace_back(C2CompositeInfo::AllocUnique(12u));
    EXPECT_EQ(0, CastParam<C2CompositeInfo>(params[0])->m.i32);
    EXPECT_EQ(0u, CastParam<C2CompositeInfo>(params[0])->m.u64);
    EXPECT_EQ(0, CastParam<C2CompositeInfo>(params[0])->m.str[0]);

    updater.updateParamsFromMessage(msg, &params);
    EXPECT_EQ(1314, CastParam<C2CompositeInfo>(params[0])->m.i32);
    EXPECT_EQ(0u, CastParam<C2CompositeInfo>(params[0])->m.u64);
    EXPECT_STREQ("1516", CastParam<C2CompositeInfo>(params[0])->m.str);
}

} // namespace android
