/*
 * Copyright 2014 The Android Open Source Project
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

//#define LOG_NDEBUG 0
#define LOG_TAG "C2SampleComponent_test"

#include <gtest/gtest.h>

#define __C2_GENERATE_GLOBAL_VARS__
#include <C2Component.h>
#include <C2Config.h>
#include <C2Debug.h>
#include <C2Enum.h>

#include <unordered_map>

class C2SampleComponentTest : public ::testing::Test {
};

void PrintTo(const C2FieldDescriptor &fd, ::std::ostream *os) {
    using FD=C2FieldDescriptor;
    switch (fd.type()) {
    case FD::INT32: *os << "i32"; break;
    case FD::INT64: *os << "i64"; break;
    case FD::UINT32: *os << "u32"; break;
    case FD::UINT64: *os << "u64"; break;
    case FD::FLOAT: *os << "float"; break;
    case FD::STRING: *os << "char"; break;
    case FD::BLOB: *os << "u8"; break;
    default:
        if (fd.type() & FD::STRUCT_FLAG) {
            *os << "struct-" << (fd.type() & ~FD::STRUCT_FLAG);
        } else {
            *os << "type-" << fd.type();
        }
    }
    *os << " " << fd.name();
    if (fd.extent() > 1) {
        *os << "[" << fd.extent() << "]";
    } else if (fd.extent() == 0) {
        *os << "[]";
    }
    *os << " (" << fd._mFieldId << "*" << fd.extent() << ")";
}

C2ENUM(
    MetadataType, int32_t,
    kInvalid = -1,
    kNone = 0,
    kGralloc,
    kNativeHandle,
    kANativeWindow,
    kCamera,
)

enum {
    kParamIndexVideoConfig = 0x1234,
};

struct C2VideoConfigStruct {
    int32_t width;
    uint32_t height;
    MetadataType metadataType;
    int32_t supportedFormats[];

    C2VideoConfigStruct() {}

    DEFINE_AND_DESCRIBE_FLEX_C2STRUCT(VideoConfig, supportedFormats)
    C2FIELD(width, "width")
    C2FIELD(height, "height")
    C2FIELD(metadataType, "metadata-type")
    C2FIELD(supportedFormats, "formats")
};

typedef C2PortParam<C2Tuning, C2VideoConfigStruct> C2VideoConfigPortTuning;

class MyComponentInstance : public C2ComponentInterface {
public:
    virtual C2String getName() const override {
        /// \todo this seems too specific
        return "sample.interface";
    };

    virtual c2_node_id_t getId() const override {
        /// \todo how are these shared?
        return 0;
    }

    virtual c2_status_t config_vb(
            const std::vector<C2Param*> &params,
            c2_blocking_t mayBlock,
            std::vector<std::unique_ptr<C2SettingResult>>* const failures) override {
        (void)params;
        (void)failures;
        (void)mayBlock;
        return C2_OMITTED;
    }

    virtual c2_status_t createTunnel_sm(c2_node_id_t targetComponent) override {
        (void)targetComponent;
        return C2_OMITTED;
    }

    virtual c2_status_t query_vb(
            const std::vector<C2Param*> &stackParams,
            const std::vector<C2Param::Index> &heapParamIndices,
            c2_blocking_t mayBlock,
            std::vector<std::unique_ptr<C2Param>>* const heapParams) const override {
        for (C2Param* const param : stackParams) {
            (void)mayBlock;
            if (!*param) { // param is already invalid - remember it
                continue;
            }

            // note: this does not handle stream params (should use index...)
            if (!mMyParams.count(param->index())) {
                continue; // not my param
            }

            C2Param & myParam = mMyParams.find(param->index())->second;
            if (myParam.size() != param->size()) { // incorrect size
                param->invalidate();
                continue;
            }

            param->updateFrom(myParam);
        }

        for (const C2Param::Index index : heapParamIndices) {
            if (mMyParams.count(index)) {
                C2Param & myParam = mMyParams.find(index)->second;
                std::unique_ptr<C2Param> paramCopy(C2Param::Copy(myParam));
                heapParams->push_back(std::move(paramCopy));
            }
        }

        return C2_OK;
    }

    std::unordered_map<uint32_t, C2Param &> mMyParams;

    C2ComponentDomainSetting mDomainInfo;

    MyComponentInstance() {
        mMyParams.insert({mDomainInfo.index(), mDomainInfo});
    }

    virtual c2_status_t releaseTunnel_sm(c2_node_id_t targetComponent) override {
        (void)targetComponent;
        return C2_OMITTED;
    }

    class MyParamReflector : public C2ParamReflector {
        const MyComponentInstance *instance;

    public:
        MyParamReflector(const MyComponentInstance *i) : instance(i) { }

        virtual std::unique_ptr<C2StructDescriptor> describe(C2Param::CoreIndex paramIndex) const override {
            switch (paramIndex.typeIndex()) {
            case decltype(instance->mDomainInfo)::CORE_INDEX:
            default:
                return std::unique_ptr<C2StructDescriptor>(new C2StructDescriptor{
                    instance->mDomainInfo.type(),
                    decltype(instance->mDomainInfo)::FieldList(),
                });
            }
            return nullptr;
        }
    };

    virtual c2_status_t querySupportedValues_vb(
            std::vector<C2FieldSupportedValuesQuery> &fields,
            c2_blocking_t mayBlock) const override {
        (void)mayBlock;
        for (C2FieldSupportedValuesQuery &query : fields) {
            if (query.field() == C2ParamField(&mDomainInfo, &C2ComponentDomainSetting::value)) {
                query.values = C2FieldSupportedValues(
                    false /* flag */,
                    &mDomainInfo.value
                    //,
                    //{(int32_t)C2Component::DOMAIN_VIDEO}
                );
                query.status = C2_OK;
            } else {
                query.status = C2_BAD_INDEX;
            }
        }
        return C2_OK;
    }

    std::shared_ptr<C2ParamReflector> getParamReflector() const {
        return std::shared_ptr<C2ParamReflector>(new MyParamReflector(this));
    }

    virtual c2_status_t querySupportedParams_nb(
            std::vector<std::shared_ptr<C2ParamDescriptor>> * const params) const override {
        params->push_back(std::make_shared<C2ParamDescriptor>(
                true /* required */, "_domain", &mDomainInfo));
        params->push_back(std::shared_ptr<C2ParamDescriptor>(
                new C2ParamDescriptor(true /* required */, "_domain", &mDomainInfo)));
        return C2_OK;
    }

    virtual ~MyComponentInstance() override = default;
};

template<typename E, bool S=std::is_enum<E>::value>
struct getter {
    int32_t get(const C2FieldSupportedValues::Primitive &p, int32_t*) {
        return p.i32;
    }
    int64_t get(const C2FieldSupportedValues::Primitive &p, int64_t*) {
        return p.i64;
    }
    uint32_t get(const C2FieldSupportedValues::Primitive &p, uint32_t*) {
        return p.u32;
    }
    uint64_t get(const C2FieldSupportedValues::Primitive &p, uint64_t*) {
        return p.u64;
    }
    float get(const C2FieldSupportedValues::Primitive &p, float*) {
        return p.fp;
    }
};

template<typename E>
struct getter<E, true> {
     typename std::underlying_type<E>::type get(const C2FieldSupportedValues::Primitive &p, E*) {
         using u=typename std::underlying_type<E>::type;
         return getter<u>().get(p, (u*)0);
     }
};

template<typename T, bool E=std::is_enum<T>::value>
struct lax_underlying_type {
    typedef typename std::underlying_type<T>::type type;
};

template<typename T>
struct lax_underlying_type<T, false> {
    typedef T type;
};

template<typename E>
typename lax_underlying_type<E>::type get(
        const C2FieldSupportedValues::Primitive &p, E*) {
    return getter<E>().get(p, (E*)0);
}

template<typename T>
void dumpFSV(const C2FieldSupportedValues &sv, T*t) {
    using namespace std;
    cout << (std::is_enum<T>::value ? (std::is_signed<typename lax_underlying_type<T>::type>::value ? "i" : "u")
             : std::is_integral<T>::value ? std::is_signed<T>::value ? "i" : "u" : "f")
         << (8 * sizeof(T));
    if (sv.type == sv.RANGE) {
        cout << ".range(" << get(sv.range.min, t);
        if (get(sv.range.step, t) != std::is_integral<T>::value) {
            cout << ":" << get(sv.range.step, t);
        }
        if (get(sv.range.num, t) != 1 || get(sv.range.denom, t) != 1) {
            cout << ":" << get(sv.range.num, t) << "/" << get(sv.range.denom, t);
        }
        cout << get(sv.range.max, t) << ")";
    }
    if (sv.values.size()) {
        cout << (sv.type == sv.FLAGS ? ".flags(" : ".list(");
        const char *sep = "";
        for (const C2FieldSupportedValues::Primitive &p : sv.values) {
            cout << sep << get(p, t);
            sep = ",";
        }
        cout << ")";
    }
    cout << endl;
}

void dumpType(C2Param::Type type) {
    using namespace std;
    cout << (type.isVendor() ? "Vendor" : "C2");
    if (type.forInput()) {
        cout << "Input";
    } else if (type.forOutput()) {
        cout << "Output";
    } else if (type.forPort() && !type.forStream()) {
        cout << "Port";
    }
    if (type.forStream()) {
        cout << "Stream";
    }

    if (type.isFlexible()) {
        cout << "Flex";
    }

    cout << type.typeIndex();

    switch (type.kind()) {
    case C2Param::INFO: cout << "Info"; break;
    case C2Param::SETTING: cout << "Setting"; break;
    case C2Param::TUNING: cout << "Tuning"; break;
    case C2Param::STRUCT: cout << "Struct"; break;
    default: cout << "Kind" << (int32_t)type.kind(); break;
    }
}

void dumpType(C2Param::CoreIndex type) {
    using namespace std;
    cout << (type.isVendor() ? "Vendor" : "C2");
    if (type.isFlexible()) {
        cout << "Flex";
    }

    cout << type.typeIndex() << "Struct";
}

void dumpType(C2FieldDescriptor::type_t type) {
    using namespace std;
    switch (type) {
    case C2FieldDescriptor::BLOB: cout << "blob "; break;
    case C2FieldDescriptor::FLOAT: cout << "float "; break;
    case C2FieldDescriptor::INT32: cout << "int32_t "; break;
    case C2FieldDescriptor::INT64: cout << "int64_t "; break;
    case C2FieldDescriptor::UINT32: cout << "uint32_t "; break;
    case C2FieldDescriptor::UINT64: cout << "uint64_t "; break;
    case C2FieldDescriptor::STRING: cout << "char "; break;
    default:
        cout << "struct ";
        dumpType((C2Param::Type)type);
        break;
    }
}

void dumpStruct(const C2StructDescriptor &sd) {
    using namespace std;
    cout << "struct ";
    dumpType(sd.coreIndex());
    cout << " {" << endl;
    //C2FieldDescriptor &f;
    for (const C2FieldDescriptor &f : sd) {
        PrintTo(f, &cout);
        cout << endl;

        if (f.namedValues().size()) {
            cout << ".named(";
            const char *sep = "";
            for (const C2FieldDescriptor::NamedValueType &p : f.namedValues()) {
                cout << sep << p.first << "=";
                switch (f.type()) {
                case C2FieldDescriptor::INT32: cout << get(p.second, (int32_t *)0); break;
                case C2FieldDescriptor::INT64: cout << get(p.second, (int64_t *)0); break;
                case C2FieldDescriptor::UINT32: cout << get(p.second, (uint32_t *)0); break;
                case C2FieldDescriptor::UINT64: cout << get(p.second, (uint64_t *)0); break;
                case C2FieldDescriptor::FLOAT: cout << get(p.second, (float *)0); break;
                default: cout << "???"; break;
                }
                sep = ",";
            }
            cout << ")";
        }
    }

    cout << "};" << endl;
}

void dumpDesc(const C2ParamDescriptor &pd) {
    using namespace std;
    if (pd.isRequired()) {
        cout << "required ";
    }
    if (pd.isPersistent()) {
        cout << "persistent ";
    }
    cout << "struct ";
    dumpType(C2Param::Type(pd.index().type()));
    cout << " " << pd.name() << ";" << endl;
}

TEST_F(C2SampleComponentTest, ReflectorTest) {
    C2ComponentDomainSetting domainInfo;
    std::shared_ptr<MyComponentInstance> myComp(new MyComponentInstance);
    std::shared_ptr<C2ComponentInterface> comp = myComp;

    std::unique_ptr<C2StructDescriptor> desc{
        myComp->getParamReflector()->describe(C2ComponentDomainSetting::CORE_INDEX)};
    dumpStruct(*desc);

    std::vector<C2FieldSupportedValuesQuery> query = {
        { C2ParamField(&domainInfo, &C2ComponentDomainSetting::value),
          C2FieldSupportedValuesQuery::CURRENT },
        C2FieldSupportedValuesQuery(C2ParamField(&domainInfo, &C2ComponentDomainSetting::value),
          C2FieldSupportedValuesQuery::CURRENT),
        C2FieldSupportedValuesQuery::Current(C2ParamField(&domainInfo, &C2ComponentDomainSetting::value)),
    };

    EXPECT_EQ(C2_OK, comp->querySupportedValues_vb(query, C2_DONT_BLOCK));

    for (const C2FieldSupportedValuesQuery &q : query) {
        dumpFSV(q.values, &domainInfo.value);
    }
}

TEST_F(C2SampleComponentTest, FieldSupportedValuesTest) {
    typedef C2GlobalParam<C2Info, C2Uint32Value, 0> Uint32TestInfo;
    Uint32TestInfo t;
    std::vector<C2FieldSupportedValues> values;
    values.push_back(C2FieldSupportedValues(0, 10, 1));  // min, max, step
    values.push_back(C2FieldSupportedValues(1, 64, 2, 1));  // min, max, num, den
    values.push_back(C2FieldSupportedValues(false, {1, 2, 3}));  // flags, std::initializer_list
    uint32_t val[] = {1, 3, 5, 7};
    std::vector<uint32_t> v(std::begin(val), std::end(val));
    values.push_back(C2FieldSupportedValues(false, v));  // flags, std::vector

    for (const C2FieldSupportedValues &sv : values) {
        dumpFSV(sv, &t.value);
    }
}

