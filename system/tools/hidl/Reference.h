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

#ifndef REFERENCE_H_

#define REFERENCE_H_

#include <android-base/logging.h>
#include <hidl-util/FQName.h>

#include "DocComment.h"
#include "Location.h"

namespace android {

/**
 * Reference placeholder
 */
template <class T>
struct Reference {
    Reference() = default;
    virtual ~Reference() {}

    Reference(const FQName& fqName, const Location& location)
        : mResolved(nullptr), mFqName(fqName), mLocation(location) {}

    Reference(T* type, const Location& location) : mResolved(type), mLocation(location) {
        CHECK(type != nullptr);
    }

    template <class OtherT>
    Reference(const Reference<OtherT>& ref)
        : mResolved(ref.mResolved), mFqName(ref.mFqName), mLocation(ref.mLocation) {}

    template <class OtherT>
    Reference(const Reference<OtherT>& ref, const Location& location)
        : mResolved(ref.mResolved), mFqName(ref.mFqName), mLocation(location) {}

    /* Returns true iff referred type is resolved
       Referred type's field might be not resolved */
    bool isResolved() const { return mResolved != nullptr; }

    T* operator->() { return get(); }
    const T* operator->() const { return get(); }

    /* Returns referenced object.
       If a type is referenced, all typedefs are unwrapped. */
    T* get() {
        CHECK(mResolved != nullptr);
        return mResolved->resolve();
    }
    const T* get() const {
        CHECK(mResolved != nullptr);
        return mResolved->resolve();
    }

    /* Returns exact referenced object.
       If a type is referenced, typedefs are not unwraped. */
    T* shallowGet() {
        CHECK(mResolved != nullptr);
        return mResolved;
    }
    const T* shallowGet() const {
        CHECK(mResolved != nullptr);
        return mResolved;
    }

    void set(T* resolved) {
        CHECK(!isResolved());
        CHECK(resolved != nullptr);
        mResolved = resolved;
    }

    /* Returns true iff this is reference to null:
       not resolved and has not name for lookup */
    bool isEmptyReference() const { return !isResolved() && !hasLookupFqName(); }

    const FQName& getLookupFqName() const {
        CHECK(hasLookupFqName());
        return mFqName;
    }

    bool hasLocation() const { return mLocation.isValid(); }

    const Location& location() const {
        CHECK(hasLocation());
        return mLocation;
    }

   private:
    /* Referred type */
    T* mResolved = nullptr;
    /* Reference name for lookup */
    FQName mFqName;
    /* Reference location is mainly used for printing errors
       and handling forward reference restrictions */
    Location mLocation;

    bool hasLookupFqName() const {
        // Valid only while not resolved to prevent confusion when
        // ref.hasLookupFqName() is false while ref,get()->fqName is valid.
        CHECK(!isResolved());
        return mFqName != FQName();
    }

    template <class OtherT>
    friend struct Reference;
};

template <class T>
struct NamedReference : public Reference<T>, DocCommentable {
    NamedReference(const std::string& name, const Reference<T>& reference, const Location& location)
        : Reference<T>(reference, location), mName(name) {}

    const std::string& name() const { return mName; }

    // TODO(b/64715470) Legacy
    const T& type() const { return *Reference<T>::get(); }

   private:
    const std::string mName;
};

}  // namespace android

#endif  // REFERENCE_H_
