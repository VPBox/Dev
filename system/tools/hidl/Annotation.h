/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef ANNOTATION_H_

#define ANNOTATION_H_

#include <android-base/macros.h>
#include <map>
#include <string>
#include <vector>

#include "ConstantExpression.h"

namespace android {

struct Formatter;

struct AnnotationParam {
    virtual ~AnnotationParam() {}

    const std::string& getName() const;

    virtual std::vector<std::string> getValues() const = 0;
    virtual std::string getSingleValue() const = 0;

    /* Returns unquoted version of getSingleValue */
    std::string getSingleString() const;

    /* Returns value interpretted as a boolean */
    bool getSingleBool() const;

    std::vector<ConstantExpression*> getConstantExpressions();
    virtual std::vector<const ConstantExpression*> getConstantExpressions() const;

   protected:
    const std::string mName;

    AnnotationParam(const std::string& name);
};

struct StringAnnotationParam : AnnotationParam {
    StringAnnotationParam(const std::string& name, std::vector<std::string>* values);

    std::vector<std::string> getValues() const override;
    std::string getSingleValue() const override;

   private:
    std::vector<std::string>* const mValues;
};

struct ConstantExpressionAnnotationParam : AnnotationParam {
    ConstantExpressionAnnotationParam(const std::string& name,
                                      std::vector<ConstantExpression*>* values);

    std::vector<std::string> getValues() const override;
    std::string getSingleValue() const override;

    std::vector<const ConstantExpression*> getConstantExpressions() const override;

   private:
    std::vector<ConstantExpression*>* const mValues;
};

using AnnotationParamVector = std::vector<AnnotationParam*>;

struct Annotation {
    Annotation(const char *name, AnnotationParamVector *params);

    std::string name() const;
    const AnnotationParamVector &params() const;
    const AnnotationParam *getParam(const std::string &name) const;

    std::vector<ConstantExpression*> getConstantExpressions();
    std::vector<const ConstantExpression*> getConstantExpressions() const;

    void dump(Formatter &out) const;

   private:
    std::string mName;
    AnnotationParamVector *mParams;

    DISALLOW_COPY_AND_ASSIGN(Annotation);
};

}  // namespace android

#endif  // ANNOTATION_H_
