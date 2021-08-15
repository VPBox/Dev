//
// Copyright (C) 2012 The Android Open Source Project
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

#ifndef UPDATE_ENGINE_OMAHA_REQUEST_ACTION_H_
#define UPDATE_ENGINE_OMAHA_REQUEST_ACTION_H_

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <gtest/gtest_prod.h>  // for FRIEND_TEST

#include <brillo/secure_blob.h>
#include <curl/curl.h>

#include "update_engine/common/action.h"
#include "update_engine/common/http_fetcher.h"
#include "update_engine/omaha_response.h"
#include "update_engine/system_state.h"

// The Omaha Request action makes a request to Omaha and can output
// the response on the output ActionPipe.

namespace policy {
class PolicyProvider;
}

namespace chromeos_update_engine {

// Encodes XML entities in a given string. Input must be ASCII-7 valid. If
// the input is invalid, the default value is used instead.
std::string XmlEncodeWithDefault(const std::string& input,
                                 const std::string& default_value);

// Escapes text so it can be included as character data and attribute
// values. The |input| string must be valid ASCII-7, no UTF-8 supported.
// Returns whether the |input| was valid and escaped properly in |output|.
bool XmlEncode(const std::string& input, std::string* output);

// This struct encapsulates the Omaha event information. For a
// complete list of defined event types and results, see
// http://code.google.com/p/omaha/wiki/ServerProtocol#event
struct OmahaEvent {
  // The Type values correspond to EVENT_TYPE values of Omaha.
  enum Type {
    kTypeUnknown = 0,
    kTypeDownloadComplete = 1,
    kTypeInstallComplete = 2,
    kTypeUpdateComplete = 3,
    kTypeUpdateDownloadStarted = 13,
    kTypeUpdateDownloadFinished = 14,
    // Chromium OS reserved type sent after the first reboot following an update
    // completed.
    kTypeRebootedAfterUpdate = 54,
  };

  // The Result values correspond to EVENT_RESULT values of Omaha.
  enum Result {
    kResultError = 0,
    kResultSuccess = 1,
    kResultUpdateDeferred = 9,  // When we ignore/defer updates due to policy.
  };

  OmahaEvent()
      : type(kTypeUnknown),
        result(kResultError),
        error_code(ErrorCode::kError) {}
  explicit OmahaEvent(Type in_type)
      : type(in_type),
        result(kResultSuccess),
        error_code(ErrorCode::kSuccess) {}
  OmahaEvent(Type in_type, Result in_result, ErrorCode in_error_code)
      : type(in_type), result(in_result), error_code(in_error_code) {}

  Type type;
  Result result;
  ErrorCode error_code;
};

class NoneType;
class OmahaRequestAction;
class OmahaRequestParams;
class PrefsInterface;

// This struct is declared in the .cc file.
struct OmahaParserData;

template <>
class ActionTraits<OmahaRequestAction> {
 public:
  // Takes parameters on the input pipe.
  typedef NoneType InputObjectType;
  // On UpdateCheck success, puts the Omaha response on output. Event
  // requests do not have an output pipe.
  typedef OmahaResponse OutputObjectType;
};

class OmahaRequestAction : public Action<OmahaRequestAction>,
                           public HttpFetcherDelegate {
 public:
  static const int kNeverPinged = -1;
  static const int kPingTimeJump = -2;
  // We choose this value of 10 as a heuristic for a work day in trying
  // each URL, assuming we check roughly every 45 mins. This is a good time to
  // wait - neither too long nor too little - so we don't give up the preferred
  // URLs that appear earlier in list too quickly before moving on to the
  // fallback ones.
  static const int kDefaultMaxFailureCountPerUrl = 10;

  // If staging is enabled, set the maximum wait time to 28 days, since that is
  // the predetermined wait time for staging.
  static const int kMaxWaitTimeStagingInDays = 28;

  // These are the possible outcome upon checking whether we satisfied
  // the wall-clock-based-wait.
  enum WallClockWaitResult {
    kWallClockWaitNotSatisfied,
    kWallClockWaitDoneButUpdateCheckWaitRequired,
    kWallClockWaitDoneAndUpdateCheckWaitNotRequired,
  };

  // The ctor takes in all the parameters that will be used for making
  // the request to Omaha. For some of them we have constants that
  // should be used.
  //
  // Takes ownership of the passed in HttpFetcher. Useful for testing.
  //
  // Takes ownership of the passed in OmahaEvent. If |event| is null,
  // this is an UpdateCheck request, otherwise it's an Event request.
  // Event requests always succeed.
  //
  // A good calling pattern is:
  // OmahaRequestAction(..., new OmahaEvent(...), new WhateverHttpFetcher);
  // or
  // OmahaRequestAction(..., nullptr, new WhateverHttpFetcher);
  OmahaRequestAction(SystemState* system_state,
                     OmahaEvent* event,
                     std::unique_ptr<HttpFetcher> http_fetcher,
                     bool ping_only);
  ~OmahaRequestAction() override;
  typedef ActionTraits<OmahaRequestAction>::InputObjectType InputObjectType;
  typedef ActionTraits<OmahaRequestAction>::OutputObjectType OutputObjectType;
  void PerformAction() override;
  void TerminateProcessing() override;
  void ActionCompleted(ErrorCode code) override;

  int GetHTTPResponseCode() { return http_fetcher_->http_response_code(); }

  // Debugging/logging
  static std::string StaticType() { return "OmahaRequestAction"; }
  std::string Type() const override { return StaticType(); }

  // Delegate methods (see http_fetcher.h)
  bool ReceivedBytes(HttpFetcher* fetcher,
                     const void* bytes,
                     size_t length) override;

  void TransferComplete(HttpFetcher* fetcher, bool successful) override;

  // Returns true if this is an Event request, false if it's an UpdateCheck.
  bool IsEvent() const { return event_.get() != nullptr; }

 private:
  friend class OmahaRequestActionTest;
  friend class OmahaRequestActionTestProcessorDelegate;
  FRIEND_TEST(OmahaRequestActionTest, GetInstallDateWhenNoPrefsNorOOBE);
  FRIEND_TEST(OmahaRequestActionTest,
              GetInstallDateWhenOOBECompletedWithInvalidDate);
  FRIEND_TEST(OmahaRequestActionTest,
              GetInstallDateWhenOOBECompletedWithValidDate);
  FRIEND_TEST(OmahaRequestActionTest,
              GetInstallDateWhenOOBECompletedDateChanges);

  // Enumeration used in PersistInstallDate().
  enum InstallDateProvisioningSource {
    kProvisionedFromOmahaResponse,
    kProvisionedFromOOBEMarker,

    // kProvisionedMax is the count of the number of enums above. Add
    // any new enums above this line only.
    kProvisionedMax
  };

  // Gets the install date, expressed as the number of PST8PDT
  // calendar weeks since January 1st 2007, times seven. Returns -1 if
  // unknown. See http://crbug.com/336838 for details about this value.
  static int GetInstallDate(SystemState* system_state);

  // Parses the Omaha Response in |doc| and sets the
  // |install_date_days| field of |output_object| to the value of the
  // elapsed_days attribute of the daystart element. Returns True if
  // the value was set, False if it wasn't found.
  static bool ParseInstallDate(OmahaParserData* parser_data,
                               OmahaResponse* output_object);

  // Returns True if the kPrefsInstallDateDays state variable is set,
  // False otherwise.
  static bool HasInstallDate(SystemState* system_state);

  // Writes |install_date_days| into the kPrefsInstallDateDays state
  // variable and emits an UMA stat for the |source| used. Returns
  // True if the value was written, False if an error occurred.
  static bool PersistInstallDate(SystemState* system_state,
                                 int install_date_days,
                                 InstallDateProvisioningSource source);

  // Persist the new cohort* value received in the XML file in the |prefs_key|
  // preference file. If the |new_value| is empty, the currently stored value
  // will be deleted. Don't call this function with an empty |new_value| if the
  // value was not set in the XML, since that would delete the stored value.
  bool PersistCohortData(const std::string& prefs_key,
                         const std::string& new_value);

  // Parse and persist the end-of-life status flag sent back in the updatecheck
  // tag attributes. The flag will be validated and stored in the Prefs.
  bool PersistEolStatus(const std::map<std::string, std::string>& attrs);

  // If this is an update check request, initializes
  // |ping_active_days_| and |ping_roll_call_days_| to values that may
  // be sent as pings to Omaha.
  void InitPingDays();

  // Based on the persistent preference store values, calculates the
  // number of days since the last ping sent for |key|.
  int CalculatePingDays(const std::string& key);

  // Returns whether we have "active_days" or "roll_call_days" ping values to
  // send to Omaha and thus we should include them in the response.
  bool ShouldPing() const;

  // Returns true if the download of a new update should be deferred.
  // False if the update can be downloaded.
  bool ShouldDeferDownload(OmahaResponse* output_object);

  // Returns true if the basic wall-clock-based waiting period has been
  // satisfied based on the scattering policy setting. False otherwise.
  // If true, it also indicates whether the additional update-check-count-based
  // waiting period also needs to be satisfied before the download can begin.
  WallClockWaitResult IsWallClockBasedWaitingSatisfied(
      OmahaResponse* output_object);

  // Returns true if the update-check-count-based waiting period has been
  // satisfied. False otherwise.
  bool IsUpdateCheckCountBasedWaitingSatisfied();

  // Parses the response from Omaha that's available in |doc| using the other
  // helper methods below and populates the |output_object| with the relevant
  // values. Returns true if we should continue the parsing.  False otherwise,
  // in which case it sets any error code using |completer|.
  bool ParseResponse(OmahaParserData* parser_data,
                     OmahaResponse* output_object,
                     ScopedActionCompleter* completer);

  // Parses the status property in the given update_check_node and populates
  // |output_object| if valid. Returns true if we should continue the parsing.
  // False otherwise, in which case it sets any error code using |completer|.
  bool ParseStatus(OmahaParserData* parser_data,
                   OmahaResponse* output_object,
                   ScopedActionCompleter* completer);

  // Parses the URL nodes in the given XML document and populates
  // |output_object| if valid. Returns true if we should continue the parsing.
  // False otherwise, in which case it sets any error code using |completer|.
  bool ParseUrls(OmahaParserData* parser_data,
                 OmahaResponse* output_object,
                 ScopedActionCompleter* completer);

  // Parses the other parameters in the given XML document and populates
  // |output_object| if valid. Returns true if we should continue the parsing.
  // False otherwise, in which case it sets any error code using |completer|.
  bool ParseParams(OmahaParserData* parser_data,
                   OmahaResponse* output_object,
                   ScopedActionCompleter* completer);

  // Called by TransferComplete() to complete processing, either
  // asynchronously after looking up resources via p2p or directly.
  void CompleteProcessing();

  // Helper to asynchronously look up payload on the LAN.
  void LookupPayloadViaP2P(const OmahaResponse& response);

  // Callback used by LookupPayloadViaP2P().
  void OnLookupPayloadViaP2PCompleted(const std::string& url);

  // Returns true if the current update should be ignored.
  bool ShouldIgnoreUpdate(const OmahaResponse& response,
                          ErrorCode* error) const;

  // Return true if updates are allowed by user preferences.
  bool IsUpdateAllowedOverCellularByPrefs(const OmahaResponse& response) const;

  // Returns true if updates are allowed over the current type of connection.
  // False otherwise.
  bool IsUpdateAllowedOverCurrentConnection(
      ErrorCode* error, const OmahaResponse& response) const;

  // Returns true if rollback is enabled. Always returns false for consumer
  // devices.
  bool IsRollbackEnabled() const;

  // Sets the appropriate max kernel key version based on whether rollback is
  // enabled.
  void SetMaxKernelKeyVersionForRollback() const;

  // Reads and returns the kPrefsUpdateFirstSeenAt pref if the pref currently
  // exists. Otherwise saves the current wallclock time to the
  // kPrefsUpdateFirstSeenAt pref and returns it as a base::Time object.
  base::Time LoadOrPersistUpdateFirstSeenAtPref() const;

  // Global system context.
  SystemState* system_state_;

  // Contains state that is relevant in the processing of the Omaha request.
  OmahaRequestParams* params_;

  // Pointer to the OmahaEvent info. This is an UpdateCheck request if null.
  std::unique_ptr<OmahaEvent> event_;

  // pointer to the HttpFetcher that does the http work
  std::unique_ptr<HttpFetcher> http_fetcher_;

  // Used for fetching information about the device policy.
  std::unique_ptr<policy::PolicyProvider> policy_provider_;

  // If true, only include the <ping> element in the request.
  bool ping_only_;

  // Stores the response from the omaha server
  brillo::Blob response_buffer_;

  // Initialized by InitPingDays to values that may be sent to Omaha
  // as part of a ping message. Note that only positive values and -1
  // are sent to Omaha.
  int ping_active_days_;
  int ping_roll_call_days_;

  DISALLOW_COPY_AND_ASSIGN(OmahaRequestAction);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_OMAHA_REQUEST_ACTION_H_
