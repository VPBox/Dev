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

#ifndef CHRE_CORE_REQUEST_MULTIPLEXER_IMPL_H_
#define CHRE_CORE_REQUEST_MULTIPLEXER_IMPL_H_

#include "chre/core/request_multiplexer.h"
#include "chre/platform/assert.h"

namespace chre {

template<typename RequestType>
bool RequestMultiplexer<RequestType>::addRequest(const RequestType& request,
                                                 size_t *index,
                                                 bool *maximalRequestChanged) {
  CHRE_ASSERT(index);
  CHRE_ASSERT(maximalRequestChanged);

  bool requestStored = mRequests.push_back(request);
  if (requestStored) {
    *index = (mRequests.size() - 1);
    *maximalRequestChanged = mCurrentMaximalRequest.mergeWith(request);
  }

  return requestStored;
}

template<typename RequestType>
void RequestMultiplexer<RequestType>::updateRequest(
    size_t index, const RequestType& request, bool *maximalRequestChanged) {
  CHRE_ASSERT(maximalRequestChanged);
  CHRE_ASSERT(index < mRequests.size());

  if (index < mRequests.size()) {
    mRequests[index] = request;
    updateMaximalRequest(maximalRequestChanged);
  }
}

template<typename RequestType>
void RequestMultiplexer<RequestType>::removeRequest(
    size_t index, bool *maximalRequestChanged) {
  CHRE_ASSERT(maximalRequestChanged);
  CHRE_ASSERT(index < mRequests.size());

  if (index < mRequests.size()) {
    mRequests.erase(index);
    updateMaximalRequest(maximalRequestChanged);
  }
}

template<typename RequestType>
void RequestMultiplexer<RequestType>::removeAllRequests(
    bool *maximalRequestChanged) {
  CHRE_ASSERT(maximalRequestChanged);

  mRequests.clear();
  updateMaximalRequest(maximalRequestChanged);
}

template<typename RequestType>
const DynamicVector<RequestType>&
    RequestMultiplexer<RequestType>::getRequests() const {
  return mRequests;
}

template<typename RequestType>
const RequestType& RequestMultiplexer<RequestType>::getCurrentMaximalRequest()
    const {
  return mCurrentMaximalRequest;
}

template<typename RequestType>
void RequestMultiplexer<RequestType>::updateMaximalRequest(
    bool *maximalRequestChanged) {
  RequestType maximalRequest;
  for (size_t i = 0; i < mRequests.size(); i++) {
    maximalRequest.mergeWith(mRequests[i]);
  }

  *maximalRequestChanged = !mCurrentMaximalRequest.isEquivalentTo(
      maximalRequest);
  if (*maximalRequestChanged) {
    mCurrentMaximalRequest = maximalRequest;
  }
}

}  // namespace chre

#endif  // CHRE_CORE_REQUEST_MULTIPLEXER_IMPL_H_
