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

// Implementation of common HTTP related functions.

#include "update_engine/common/http_common.h"

#include <cstdlib>

#include <base/macros.h>

namespace chromeos_update_engine {

const char* GetHttpResponseDescription(HttpResponseCode code) {
  static const struct {
    HttpResponseCode code;
    const char* description;
  } http_response_table[] = {
      {kHttpResponseOk, "OK"},
      {kHttpResponseCreated, "Created"},
      {kHttpResponseAccepted, "Accepted"},
      {kHttpResponseNonAuthInfo, "Non-Authoritative Information"},
      {kHttpResponseNoContent, "No Content"},
      {kHttpResponseResetContent, "Reset Content"},
      {kHttpResponsePartialContent, "Partial Content"},
      {kHttpResponseMultipleChoices, "Multiple Choices"},
      {kHttpResponseMovedPermanently, "Moved Permanently"},
      {kHttpResponseFound, "Found"},
      {kHttpResponseSeeOther, "See Other"},
      {kHttpResponseNotModified, "Not Modified"},
      {kHttpResponseUseProxy, "Use Proxy"},
      {kHttpResponseTempRedirect, "Temporary Redirect"},
      {kHttpResponseBadRequest, "Bad Request"},
      {kHttpResponseUnauth, "Unauthorized"},
      {kHttpResponseForbidden, "Forbidden"},
      {kHttpResponseNotFound, "Not Found"},
      {kHttpResponseRequestTimeout, "Request Timeout"},
      {kHttpResponseInternalServerError, "Internal Server Error"},
      {kHttpResponseNotImplemented, "Not Implemented"},
      {kHttpResponseServiceUnavailable, "Service Unavailable"},
      {kHttpResponseVersionNotSupported, "HTTP Version Not Supported"},
  };

  bool is_found = false;
  size_t i;
  for (i = 0; i < arraysize(http_response_table); i++)
    if ((is_found = (http_response_table[i].code == code)))
      break;

  return (is_found ? http_response_table[i].description : "(unsupported)");
}

HttpResponseCode StringToHttpResponseCode(const char* s) {
  return static_cast<HttpResponseCode>(strtoul(s, nullptr, 10));
}

const char* GetHttpContentTypeString(HttpContentType type) {
  static const struct {
    HttpContentType type;
    const char* str;
  } http_content_type_table[] = {
      {kHttpContentTypeTextXml, "text/xml"},
  };

  bool is_found = false;
  size_t i;
  for (i = 0; i < arraysize(http_content_type_table); i++)
    if ((is_found = (http_content_type_table[i].type == type)))
      break;

  return (is_found ? http_content_type_table[i].str : nullptr);
}

}  // namespace chromeos_update_engine
