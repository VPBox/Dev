/*
 * Copyright 2017 The Android Open Source Project
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

#define LOG_TAG "C2ComponentInterface_test"

#include <dlfcn.h>
#include <stdio.h>

#include <gtest/gtest.h>
#include <utils/Log.h>

#include <C2Component.h>
#include <C2Config.h>
#include <util/C2InterfaceHelper.h>
#include <C2Param.h>

#if !defined(UNUSED)
#define UNUSED(expr)                                                           \
  do {                                                                         \
      (void)(expr);                                                            \
  } while (0)

#endif //!defined(UNUSED)

namespace android {

template <class T> std::unique_ptr<T> alloc_unique_cstr(const char *cstr) {
    size_t len = strlen(cstr);
    std::unique_ptr<T> ptr = T::AllocUnique(len);
    memcpy(ptr->m.value, cstr, len);
    return ptr;
}

class C2CompIntfTest : public ::testing::Test {
protected:
    C2CompIntfTest() {}
    ~C2CompIntfTest() override {}

    void setComponent(std::shared_ptr<C2ComponentInterface> intf) {
        mIntf = intf;
    }

    void resetResults() {
        mIntf = nullptr;
        mParamResults.clear();
    }

    template <typename T> void testUnsupportedParam();

    template <typename T> void testSupportedParam();

    // testReadOnlyParam() and testWritableParam() are the main functions for testing a parameter.
    // A caller should find out if a tested parameter is read-only or writable before calling them
    // and it must call one of the corresponded them.

    // If a parameter is read-only this is called.
    // Test read-only parameter |preParam|. The test expects failure while config() with |newParam|,
    // and make sure |preParam| stay unchanged.
    template <typename T>
    void testReadOnlyParam(const T &preParam, const T &newParam);

    // If a parameter is writable this is called.
    // Test one filed |writableField| for given writable parameter |param|.
    // |validValues| contains all values obtained from querySupportedValues() for |writableField|.
    // The test checks validity for config() with each value, and make sure values are config-ed
    // by query() them out. |invalidValues| contains some values which are not in |validValues|.
    // The test expects C2_BAD_VALUE while config() with these values,
    // and |param| should stay unchanged.
    template <typename TParam, typename TRealField, typename TField>
    void testWritableParam(TParam *const param, TRealField *const writableField,
                           const std::vector<TField> &validValues,
                           const std::vector<TField> &invalidValues);

    // Test all the defined parameters in C2Param.h.
    void testMain(std::shared_ptr<C2ComponentInterface> intf,
                  const std::string &componentName);

    // Check permission of parameter type |T| for testing interface.
    // This should be called first of the testing per parameter type,
    // therefore different testing process is applied according to the permission type.
    template <typename T>
    void checkParamPermission(
            int *const writable,
            const std::vector<std::shared_ptr<C2ParamDescriptor>> &supportedParams);

private:
    enum ParamPermission : int {
        WRITABLE,
        READONLY,
        UNSUPPORTED,
    };

    struct paramTestInfo {
        std::string name;
        int result;
        paramTestInfo(const char *name_, int result_)
            : name(name_), result(result_) {}
    };

    // queryOnStack() and queryonHeap() both call an interface's query_vb() and
    // check if a component has a parameter whose type is |T|.
    // If a component has, the value should be copied into an argument, that is
    // |p| in queryOnStack() and |heapParams| in queryOnHeap().
    // The return value is c2_status_t (e.g. C2_OK).
    template <typename T> c2_status_t queryOnStack(T *const p);

    template <typename T>
    c2_status_t queryOnHeap(const T &p,
                         std::vector<std::unique_ptr<C2Param>> *const heapParams);

    // Get a value whose type is |T| in a component. The value is copied to |param|.
    // This should be called only if a component has the parameter.
    template <typename T> void getValue(T *const param);

    // Check if the parameter's value in component is equal to |expected| and
    // queryOnStack() and queryOnHeap() are succeeded. When this function called,
    // it should be guaranteed a component has the parameter.
    template <typename T> void queryParamAsExpected(const T &expected);

    // Test if query functions works correctly for supported parameters.
    // "Support" means here a component has the parameter.
    template <typename T> void querySupportedParam();

    // Test query functions works correctly for unsupported parameters.
    // "Unsupport" means here a component doesn't have the parameter.
    template <typename T> void queryUnsupportedParam();

    // Execute an interface's config_vb(). |T| is a single parameter type, not std::vector.
    // config() creates std::vector<C2Param *> {p} and passes it to config_vb().
    template <typename T>
    c2_status_t
    config(T *const p,
           std::vector<std::unique_ptr<C2SettingResult>> *const failures);

    // Test if config works correctly for read-only parameters.
    // Because the failure of config() is assumed, |newParam| doesn't matter.
    template <typename T> void configReadOnlyParam(const T &newParam);

    // Test if config works correctly for writable parameters.
    // This changes the parameter's value to |newParam|.
    // |stConfig| is a return value of config().
    template <typename T> void configWritableParamValidValue(const T &newParam, c2_status_t *stConfig);

    // Test if config works correctly in the case an invalid value |newParam| is tried to write
    // to an writable parameter.
    template <typename T> void configWritableParamInvalidValue(const T &newParam);

    // Create values for testing from |validValueInfos|. The values are returned as arguments.
    // |validValues| : valid values, which can be written for the parameter.
    // |InvalidValues| : invalid values, which cannot be written for the parameter.
    //                   config() should be failed if these values are used as new values.
    // This function should be called only for writable and supported parameters.
    template <typename TField>
    void getTestValues(const C2FieldSupportedValues &validValueInfos,
                       std::vector<TField> *const validValues,
                       std::vector<TField> *const invalidValues);

    // Output the summary of test results. Categorizes parameters with their configuration.
    void outputResults(const std::string &name);

    std::shared_ptr<C2ComponentInterface> mIntf;
    std::vector<paramTestInfo> mParamResults;
    std::string mCurrentParamName;
};

// factory function
// TODO(hiroh): Add factory functions for other types.
template <typename T> std::unique_ptr<T> makeParam() {
    return std::make_unique<T>();
}

template <> std::unique_ptr<C2PortMediaTypeSetting::input> makeParam() {
    // TODO(hiroh): Set more precise length.
    return C2PortMediaTypeSetting::input::AllocUnique(100);
}

#define TRACED_FAILURE(func)                            \
    do {                                                \
        SCOPED_TRACE(mCurrentParamName);             \
        func;                                           \
        if (::testing::Test::HasFatalFailure()) {       \
            return;                                     \
        }                                               \
    } while (false)

template <typename T> c2_status_t C2CompIntfTest::queryOnStack(T *const p) {
    std::vector<C2Param*> stackParams{p};
    return mIntf->query_vb(stackParams, {}, C2_DONT_BLOCK, nullptr);
}

template <typename T>
c2_status_t C2CompIntfTest::queryOnHeap(
        const T &p, std::vector<std::unique_ptr<C2Param>> *const heapParams) {
    uint32_t index = p.index() & ~0x03FE0000;
    if (p.forStream()) {
        index |= ((p.stream() << 17) & 0x01FE0000) | 0x02000000;
    }
    return mIntf->query_vb({}, {index}, C2_DONT_BLOCK, heapParams);
}

template <typename T> void C2CompIntfTest::getValue(T *const param) {
    // When getValue() is called, a component has to have the parameter.
    ASSERT_EQ(C2_OK, queryOnStack(param));
}

template <typename T>
void C2CompIntfTest::queryParamAsExpected(const T &expected) {
    // TODO(hiroh): Don't create param on stack and call queryOnStack for flex params.
    // Note that all the current supported parameters are non-flex params.
    T stack;
    std::unique_ptr<T> pHeap = makeParam<T>();
    std::vector<std::unique_ptr<C2Param>> heapParams;

    ASSERT_EQ(C2_OK, queryOnStack(&stack));

    // |stack| is a parameter value. The parameter size shouldn't be 0.
    EXPECT_NE(0u, stack.size());
    EXPECT_EQ(stack, expected);

    ASSERT_EQ(C2_OK, queryOnHeap(*pHeap, &heapParams));

    // |*heapParams[0]| is a parameter value. The size of |heapParams| has to be one.
    ASSERT_EQ(1u, heapParams.size());
    EXPECT_TRUE(heapParams[0]);
    EXPECT_EQ(*heapParams[0], expected);
}

template <typename T> void C2CompIntfTest::querySupportedParam() {
    std::unique_ptr<T> param = makeParam<T>();
    // The current parameter's value is acquired by getValue(), which should be succeeded.
    getValue(param.get());
    queryParamAsExpected(*param);
}

template <typename T> void C2CompIntfTest::queryUnsupportedParam() {
    // TODO(hiroh): Don't create param on stack and call queryOnStack for flex params.
    // Note that all the current supported parameters are non-flex params.
    T stack;
    std::unique_ptr<T> pHeap = makeParam<T>();
    std::vector<std::unique_ptr<C2Param>> heapParams;
    // If a component doesn't have the parameter, queryOnStack() and queryOnHeap()
    // should return C2_BAD_INDEX.
    ASSERT_EQ(C2_BAD_INDEX, queryOnStack(&stack));
    EXPECT_FALSE(stack);
    ASSERT_EQ(C2_BAD_INDEX, queryOnHeap(*pHeap, &heapParams));
    EXPECT_EQ(0u, heapParams.size());
}

template <typename T>
c2_status_t C2CompIntfTest::config(
        T *const p, std::vector<std::unique_ptr<C2SettingResult>> *const failures) {
    std::vector<C2Param*> params{p};
    return mIntf->config_vb(params, C2_DONT_BLOCK, failures);
}

// Create a new parameter copied from |p|.
template <typename T> std::unique_ptr<T> makeParamFrom(const T &p) {
    std::unique_ptr<T> retP = makeParam<T>();
    EXPECT_TRUE(retP->updateFrom(p));
    EXPECT_TRUE(memcmp(retP.get(), &p, sizeof(T)) == 0);
    return retP;
}

template <typename T>
void C2CompIntfTest::configReadOnlyParam(const T &newParam) {
    std::unique_ptr<T> p = makeParamFrom(newParam);

    std::vector<C2Param*> params{p.get()};
    std::vector<std::unique_ptr<C2SettingResult>> failures;

    // config_vb should be failed because a parameter is read-only.
    ASSERT_EQ(C2_BAD_VALUE, mIntf->config_vb(params, C2_DONT_BLOCK, &failures));
    ASSERT_EQ(1u, failures.size());
    EXPECT_EQ(C2SettingResult::READ_ONLY, failures[0]->failure);
}

template <typename T>
void C2CompIntfTest::configWritableParamValidValue(const T &newParam, c2_status_t *configResult) {
    std::unique_ptr<T> p = makeParamFrom(newParam);

    std::vector<C2Param*> params{p.get()};
    std::vector<std::unique_ptr<C2SettingResult>> failures;
    // In most cases, config_vb return C2_OK and the parameter's value should be changed
    // to |newParam|, which is confirmed in a caller of configWritableParamValueValue().
    // However, this can return ~~~~ and the parameter's values is not changed,
    // because there may be dependent limitations between fields or between parameters.
    // TODO(hiroh): I have to fill the return value. Comments in C2Component.h doesn't mention
    // about the return value when conflict happens. I set C2_BAD_VALUE to it temporarily now.
    c2_status_t stConfig = mIntf->config_vb(params, C2_DONT_BLOCK, &failures);
    if (stConfig == C2_OK) {
        EXPECT_EQ(0u, failures.size());
    } else {
        ASSERT_EQ(C2_BAD_VALUE, stConfig);
        EXPECT_EQ(1u, failures.size());
        EXPECT_EQ(C2SettingResult::CONFLICT, failures[0]->failure);
    }
    *configResult = stConfig;
}

template <typename T>
void C2CompIntfTest::configWritableParamInvalidValue(const T &newParam) {
    std::unique_ptr<T> p = makeParamFrom(newParam);

    std::vector<C2Param*> params{p.get()};
    std::vector<std::unique_ptr<C2SettingResult>> failures;
    // Although a parameter is writable, config_vb should be failed,
    // because a new value is invalid.
    ASSERT_EQ(C2_BAD_VALUE, mIntf->config_vb(params, C2_DONT_BLOCK, &failures));
    ASSERT_EQ(1u, failures.size());
    EXPECT_EQ(C2SettingResult::BAD_VALUE, failures[0]->failure);
}

// There is only used enum type for the field type, that is C2Component::domain_t.
// If another field type is added, it is necessary to add function for that.
template <>
void C2CompIntfTest::getTestValues(
        const C2FieldSupportedValues &validValueInfos,
        std::vector<C2Component::domain_t> *const validValues,
        std::vector<C2Component::domain_t> *const invalidValues) {
    UNUSED(validValueInfos);
    validValues->emplace_back(C2Component::DOMAIN_VIDEO);
    validValues->emplace_back(C2Component::DOMAIN_AUDIO);
    validValues->emplace_back(C2Component::DOMAIN_OTHER);

    // There is no invalid value.
    UNUSED(invalidValues);
}

template <typename TField>
void C2CompIntfTest::getTestValues(
        const C2FieldSupportedValues &validValueInfos,
        std::vector<TField> *const validValues,
        std::vector<TField> *const invalidValues) {
    using TStorage = typename _c2_reduce_enum_to_underlying_type<TField>::type;

    // The supported values are represented by C2Values. C2Value::Primitive needs to
    // be transformed to a primitive value. This function is one to do that.
    auto prim2Value = [](const C2Value::Primitive &prim) -> TField {
        return (TField)prim.ref<TStorage>();
        static_assert(std::is_same<TStorage, int32_t>::value ||
                      std::is_same<TStorage, uint32_t>::value ||
                      std::is_same<TStorage, int64_t>::value ||
                      std::is_same<TStorage, uint64_t>::value ||
                      std::is_same<TStorage, float>::value, "Invalid TField type.");
    };

    // The size of validValueInfos is one.
    const auto &c2FSV = validValueInfos;

    switch (c2FSV.type) {
    case C2FieldSupportedValues::type_t::EMPTY: {
        invalidValues->emplace_back(TField(0));
        // TODO(hiroh) : Should other invalid values be tested?
        break;
    }
    case C2FieldSupportedValues::type_t::RANGE: {
        const auto &range = c2FSV.range;
        auto rmin = prim2Value(range.min);
        auto rmax = prim2Value(range.max);
        auto rstep = prim2Value(range.step);

        ASSERT_LE(rmin, rmax);

        if (rstep != 0) {
            // Increase linear
            for (auto v = rmin; v <= rmax; v = TField(v + rstep)) {
                validValues->emplace_back(v);
            }
            if (rmin > std::numeric_limits<TField>::min()) {
                invalidValues->emplace_back(TField(rmin - 1));
            }
            if (rmax < std::numeric_limits<TField>::max()) {
                invalidValues->emplace_back(TField(rmax + 1));
            }
            const unsigned int N = validValues->size();
            if (N >= 2) {
                if (std::is_same<TField, float>::value) {
                    invalidValues->emplace_back(TField((validValues->at(0) + validValues->at(1)) / 2));
                    invalidValues->emplace_back(TField((validValues->at(N - 2) + validValues->at(N - 1)) / 2));
                } else {
                    if (rstep > 1) {
                        invalidValues->emplace_back(TField(validValues->at(0) + 1));
                        invalidValues->emplace_back(TField(validValues->at(N - 1) - 1));
                    }
                }
            }
        } else {
            // There should be two cases, except linear case.
            // 1. integer geometric case
            // 2. float geometric case

            auto num = prim2Value(range.num);
            auto denom = prim2Value(range.denom);

            // If both range.num and range.denom are 1 and step is 0, we should use
            // VALUES, shouldn't we?
            ASSERT_FALSE(num == 1 && denom == 1);

            // (num / denom) is not less than 1.
            ASSERT_FALSE(denom == 0);
            ASSERT_LE(denom, num);
            for (auto v = rmin; v <= rmax; v = TField(v * num / denom)) {
                validValues->emplace_back(v);
            }

            if (rmin > std::numeric_limits<TField>::min()) {
                invalidValues->emplace_back(TField(rmin - 1));
            }
            if (rmax < std::numeric_limits<TField>::max()) {
                invalidValues->emplace_back(TField(rmax + 1));
            }

            const unsigned int N = validValues->size();
            if (N >= 2) {
                if (std::is_same<TField, float>::value) {
                    invalidValues->emplace_back(TField((validValues->at(0) + validValues->at(1)) / 2));
                    invalidValues->emplace_back(TField((validValues->at(N - 2) + validValues->at(N - 1)) / 2));
                } else {
                    if (validValues->at(1) - validValues->at(0) > 1) {
                        invalidValues->emplace_back(TField(validValues->at(0) + 1));
                    }
                    if (validValues->at(N - 1) - validValues->at(N - 2) > 1) {
                        invalidValues->emplace_back(TField(validValues->at(N - 1) - 1));
                    }
                }
            }
        }
        break;
    }
    case C2FieldSupportedValues::type_t::VALUES: {
        for (const C2Value::Primitive &prim : c2FSV.values) {
            validValues->emplace_back(prim2Value(prim));
        }
        auto minv = *std::min_element(validValues->begin(), validValues->end());
        auto maxv = *std::max_element(validValues->begin(), validValues->end());
        if (minv - 1 > std::numeric_limits<TField>::min()) {
            invalidValues->emplace_back(TField(minv - 1));
        }
        if (maxv + 1 < std::numeric_limits<TField>::max()) {
            invalidValues->emplace_back(TField(maxv + 1));
        }
        break;
    }
    case C2FieldSupportedValues::type_t::FLAGS: {
        // TODO(hiroh) : Implement the case that param.type is FLAGS.
        break;
    }
    }
}

template <typename T>
void C2CompIntfTest::testReadOnlyParam(const T &preParam, const T &newParam) {
    TRACED_FAILURE(configReadOnlyParam(newParam));
    // Parameter value must not be changed
    TRACED_FAILURE(queryParamAsExpected(preParam));
}

template <typename TParam, typename TRealField, typename TField>
void C2CompIntfTest::testWritableParam(
        TParam *const param, TRealField *const writableField,
        const std::vector<TField> &validValues,
        const std::vector<TField> &invalidValues) {
    c2_status_t stConfig;

    // Get the parameter's value in the beginning in order to reset the value at the end.
    TRACED_FAILURE(getValue(param));
    std::unique_ptr<TParam> defaultParam = makeParamFrom(*param);

    // Test valid values
    for (const auto &val : validValues) {
        std::unique_ptr<TParam> preParam = makeParamFrom(*param);

        // Param is try to be changed
        *writableField = val;
        TRACED_FAILURE(configWritableParamValidValue(*param, &stConfig));
        if (stConfig == C2_OK) {
            TRACED_FAILURE(queryParamAsExpected(*param));
        } else {
            // Param is unchanged because a field value conflicts with other field or parameter.
            TRACED_FAILURE(queryParamAsExpected(*preParam));
        }
    }

    // Store the current parameter in order to test |param| is unchanged
    // after trying to write an invalid value.
    std::unique_ptr<TParam> lastValidParam = makeParamFrom(*param);

    // Test invalid values
    for (const auto &val : invalidValues) {
        // Param is changed
        *writableField = val;
        TRACED_FAILURE(configWritableParamInvalidValue(*param));
        TRACED_FAILURE(queryParamAsExpected(*lastValidParam));
    }
    // Reset the parameter by config().
    TRACED_FAILURE(configWritableParamValidValue(*defaultParam, &stConfig));
}

template <typename T> void C2CompIntfTest::testUnsupportedParam() {
    TRACED_FAILURE(queryUnsupportedParam<T>());
}

template <typename T> void C2CompIntfTest::testSupportedParam() {
    TRACED_FAILURE(querySupportedParam<T>());
}

bool isSupportedParam(
        const C2Param &param,
        const std::vector<std::shared_ptr<C2ParamDescriptor>> &sParams) {
    for (const auto &pd : sParams) {
        if (param.type() == pd->index().type()) {
            return true;
        }
    }
    return false;
}

template <typename T>
void C2CompIntfTest::checkParamPermission(
    int *const result,
    const std::vector<std::shared_ptr<C2ParamDescriptor>> &supportedParams) {
    std::unique_ptr<T> param = makeParam<T>();

    if (!isSupportedParam(*param, supportedParams)) {
        // If a parameter isn't supported, it just finish after calling testUnsupportedParam().
        testUnsupportedParam<T>();
        *result = ParamPermission::UNSUPPORTED;
        return;
    }

    testSupportedParam<T>();

    TRACED_FAILURE(getValue(param.get()));
    std::vector<std::unique_ptr<C2SettingResult>> failures;
    // Config does not change the parameter, because param is the present param.
    // This config is executed to find out if a parameter is read-only or writable.
    c2_status_t stStack = config(param.get(), &failures);
    if (stStack == C2_BAD_VALUE) {
        // Read-only
        std::unique_ptr<T> newParam = makeParam<T>();
        testReadOnlyParam(*param, *newParam);
        *result = ParamPermission::READONLY;
    } else {
        // Writable
        EXPECT_EQ(stStack, C2_OK);
        *result = ParamPermission::WRITABLE;
    }
}

void C2CompIntfTest::outputResults(const std::string &name) {
    std::vector<std::string> params[3];
    for (const auto &testInfo : mParamResults) {
        int result = testInfo.result;
        ASSERT_TRUE(0 <= result && result <= 2);
        params[result].emplace_back(testInfo.name);
    }
    const char *resultString[] = {"Writable", "Read-Only", "Unsupported"};
    printf("\n----TEST RESULTS (%s)----\n\n", name.c_str());
    for (int i = 0; i < 3; i++) {
        printf("[ %s ]\n", resultString[i]);
        for (const auto &t : params[i]) {
            printf("%s\n", t.c_str());
        }
        printf("\n");
    }
}

#define TEST_GENERAL_WRITABLE_FIELD(TParam_, field_type_name_, field_name_) \
    do {                                                                \
        std::unique_ptr<TParam_> param = makeParam<TParam_>();          \
        std::vector<C2FieldSupportedValuesQuery> validValueInfos = {    \
            C2FieldSupportedValuesQuery::Current(                       \
                    C2ParamField(param.get(), &field_type_name_::field_name_)) \
        };                                                              \
        ASSERT_EQ(C2_OK,                                                \
                  mIntf->querySupportedValues_vb(validValueInfos, C2_DONT_BLOCK));     \
        ASSERT_EQ(1u, validValueInfos.size());                          \
        std::vector<decltype(param->field_name_)> validValues;          \
        std::vector<decltype(param->field_name_)> invalidValues;        \
        getTestValues(validValueInfos[0].values, &validValues, &invalidValues);   \
        testWritableParam(param.get(), &param->field_name_, validValues,\
                          invalidValues);                               \
    } while (0)

#define TEST_VSSTRUCT_WRITABLE_FIELD(TParam_, field_type_name_)         \
    do {                                                                \
        TEST_GENERAL_WRITABLE_FIELD(TParam_, field_type_name_, width);  \
        TEST_GENERAL_WRITABLE_FIELD(TParam_, field_type_name_, height); \
    } while (0)

#define TEST_U32_WRITABLE_FIELD(TParam_, field_type_name_)              \
  TEST_GENERAL_WRITABLE_FIELD(TParam_, field_type_name_, value)

#define TEST_ENUM_WRITABLE_FIELD(TParam_, field_type_name_)             \
  TEST_GENERAL_WRITABLE_FIELD(TParam_, field_type_name_, value)

// TODO(hiroh): Support parameters based on char[] and uint32_t[].
//#define TEST_STRING_WRITABLE_FIELD(TParam_, field_type_name_)
// TEST_GENERAL_WRITABLE_FIELD(TParam_, field_type_name_, m.value)
//#define TEST_U32ARRAY_WRITABLE_FIELD(Tparam_, field_type_name_)
// TEST_GENERAL_WRITABLE_FIELD(Tparam_, uint32_t[], field_type_name_, values)

#define EACH_TEST(TParam_, field_type_name_, test_name)                 \
    do {                                                                \
      int result = 0;                                                   \
      this->mCurrentParamName = #TParam_;                            \
      checkParamPermission<TParam_>(&result, supportedParams);          \
      if (result == ParamPermission::WRITABLE) {                        \
          test_name(TParam_, field_type_name_);                         \
      }                                                                 \
      mParamResults.emplace_back(#TParam_, result);                      \
  } while (0)

#define EACH_TEST_SELF(type_, test_name) EACH_TEST(type_, type_, test_name)
#define EACH_TEST_INPUT(type_, test_name) EACH_TEST(type_::input, type_, test_name)
#define EACH_TEST_OUTPUT(type_, test_name) EACH_TEST(type_::output, type_, test_name)
void C2CompIntfTest::testMain(std::shared_ptr<C2ComponentInterface> intf,
                              const std::string &componentName) {
    setComponent(intf);

    std::vector<std::shared_ptr<C2ParamDescriptor>> supportedParams;
    ASSERT_EQ(C2_OK, mIntf->querySupportedParams_nb(&supportedParams));

    EACH_TEST_SELF(C2ActualPipelineDelayTuning, TEST_U32_WRITABLE_FIELD);
    EACH_TEST_SELF(C2ComponentAttributesSetting, TEST_U32_WRITABLE_FIELD);
    EACH_TEST_INPUT(C2PortActualDelayTuning, TEST_U32_WRITABLE_FIELD);
    EACH_TEST_OUTPUT(C2PortActualDelayTuning, TEST_U32_WRITABLE_FIELD);
    EACH_TEST_INPUT(C2StreamBufferTypeSetting, TEST_U32_WRITABLE_FIELD);
    EACH_TEST_OUTPUT(C2StreamBufferTypeSetting, TEST_U32_WRITABLE_FIELD);
    EACH_TEST_INPUT(C2PortStreamCountTuning, TEST_U32_WRITABLE_FIELD);
    EACH_TEST_OUTPUT(C2PortStreamCountTuning, TEST_U32_WRITABLE_FIELD);

    EACH_TEST_SELF(C2ComponentDomainSetting, TEST_ENUM_WRITABLE_FIELD);

    // TODO(hiroh): Support parameters based on uint32_t[] and char[].
    // EACH_TEST_INPUT(C2PortMediaTypeSetting, TEST_STRING_WRITABLE_FIELD);
    // EACH_TEST_OUTPUT(C2PortMediaTypeSetting, TEST_STRING_WRITABLE_FIELD);
    // EACH_TEST_INPUT(C2StreamMimeConfig, TEST_STRING_WRITABLE_FIELD);
    // EACH_TEST_OUTPUT(C2StreamMimeConfig, TEST_STRING_WRITABLE_FIELD);

    // EACH_TEST_SELF(C2SupportedParamsInfo, TEST_U32ARRAY_WRITABLE_FIELD);
    // EACH_TEST_SELF(C2RequiredParamsInfo, TEST_U32ARRAY_WRITABLE_FIELD);
    // EACH_TEST_SELF(C2ReadOnlyParamsInfo, TEST_U32ARRAY_WRITABLE_FIELD);
    // EACH_TEST_SELF(C2RequestedInfosInfo, TEST_U32ARRAY_WRITABLE_FIELD);

    EACH_TEST_INPUT(C2StreamPictureSizeInfo, TEST_VSSTRUCT_WRITABLE_FIELD);
    EACH_TEST_OUTPUT(C2StreamPictureSizeInfo, TEST_VSSTRUCT_WRITABLE_FIELD);
    EACH_TEST_INPUT(C2StreamPictureSizeInfo, TEST_VSSTRUCT_WRITABLE_FIELD);
    EACH_TEST_OUTPUT(C2StreamPictureSizeInfo, TEST_VSSTRUCT_WRITABLE_FIELD);
    EACH_TEST_INPUT(C2MaxVideoSizeHintPortSetting, TEST_VSSTRUCT_WRITABLE_FIELD);
    EACH_TEST_OUTPUT(C2MaxVideoSizeHintPortSetting, TEST_VSSTRUCT_WRITABLE_FIELD);

    outputResults(componentName);
    resetResults();
}

TEST_F(C2CompIntfTest, C2V4L2CodecIntf) {

    // Read a shared object library.
    void* compLib = dlopen("system/lib/libv4l2_codec2.so", RTLD_NOW);

    if (!compLib) {
        printf("Cannot open library: %s.\n", dlerror());
        FAIL();
        return;
    }

    typedef C2ComponentStore* create_t();
    create_t* create_store= (create_t*) dlsym(compLib, "create_store");
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        printf("Cannot load symbol create: %s.\n", dlsym_error);
        FAIL();
        return;
    }

    typedef void destroy_t(C2ComponentStore*);
    destroy_t* destroy_store = (destroy_t*) dlsym(compLib, "destroy_store");
    dlsym_error = dlerror();
    if (dlsym_error) {
        printf("Cannot load symbol destroy: %s.\n", dlsym_error);
        FAIL();
        return;
    }

    std::shared_ptr<C2ComponentStore> componentStore(create_store(), destroy_store);
    std::shared_ptr<C2ComponentInterface> componentIntf;
    componentStore->createInterface("v4l2.decoder", &componentIntf);
    auto componentName = "C2V4L2Codec";
    testMain(componentIntf, componentName);
}

} // namespace android
