//
// Copyright (C) 2009 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// This file contains general definitions used in implementing, testing and
// emulating communication over HTTP.

#ifndef UPDATE_ENGINE_COMMON_HTTP_COMMON_H_
#define UPDATE_ENGINE_COMMON_HTTP_COMMON_H_

namespace chromeos_update_engine {

// Enumeration type for HTTP response codes.
enum HttpResponseCode {
  kHttpResponseUndefined = 0,
  kHttpResponseOk = 200,
  kHttpResponseCreated = 201,
  kHttpResponseAccepted = 202,
  kHttpResponseNonAuthInfo = 203,
  kHttpResponseNoContent = 204,
  kHttpResponseResetContent = 205,
  kHttpResponsePartialContent = 206,
  kHttpResponseMultipleChoices = 300,
  kHttpResponseMovedPermanently = 301,
  kHttpResponseFound = 302,
  kHttpResponseSeeOther = 303,
  kHttpResponseNotModified = 304,
  kHttpResponseUseProxy = 305,
  kHttpResponseTempRedirect = 307,
  kHttpResponseBadRequest = 400,
  kHttpResponseUnauth = 401,
  kHttpResponseForbidden = 403,
  kHttpResponseNotFound = 404,
  kHttpResponseRequestTimeout = 408,
  kHttpResponseReqRangeNotSat = 416,
  kHttpResponseInternalServerError = 500,
  kHttpResponseNotImplemented = 501,
  kHttpResponseServiceUnavailable = 503,
  kHttpResponseVersionNotSupported = 505,
};

// Returns a standard HTTP status line string for a given response code.
const char* GetHttpResponseDescription(HttpResponseCode code);

// Converts a string beginning with an HTTP error code into numerical value.
HttpResponseCode StringToHttpResponseCode(const char* s);

// Enumeration type for HTTP Content-Type.
enum HttpContentType {
  kHttpContentTypeUnspecified = 0,
  kHttpContentTypeTextXml,
};

// Returns a standard HTTP Content-Type string.
const char* GetHttpContentTypeString(HttpContentType type);

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_COMMON_HTTP_COMMON_H_
