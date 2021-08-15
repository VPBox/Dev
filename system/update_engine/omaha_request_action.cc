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

#include "update_engine/omaha_request_action.h"

#include <inttypes.h>

#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <base/bind.h>
#include <base/logging.h>
#include <base/rand_util.h>
#include <base/strings/string_number_conversions.h>
#include <base/strings/string_split.h>
#include <base/strings/string_util.h>
#include <base/strings/stringprintf.h>
#include <base/time/time.h>
#include <brillo/key_value_store.h>
#include <expat.h>
#include <metrics/metrics_library.h>
#include <policy/libpolicy.h>

#include "update_engine/common/action_pipe.h"
#include "update_engine/common/constants.h"
#include "update_engine/common/hardware_interface.h"
#include "update_engine/common/hash_calculator.h"
#include "update_engine/common/platform_constants.h"
#include "update_engine/common/prefs_interface.h"
#include "update_engine/common/utils.h"
#include "update_engine/connection_manager_interface.h"
#include "update_engine/metrics_reporter_interface.h"
#include "update_engine/metrics_utils.h"
#include "update_engine/omaha_request_params.h"
#include "update_engine/p2p_manager.h"
#include "update_engine/payload_state_interface.h"

using base::Time;
using base::TimeDelta;
using chromeos_update_manager::kRollforwardInfinity;
using std::map;
using std::numeric_limits;
using std::string;
using std::vector;

namespace chromeos_update_engine {

// List of custom attributes that we interpret in the Omaha response:
constexpr char kAttrDeadline[] = "deadline";
constexpr char kAttrDisableP2PForDownloading[] = "DisableP2PForDownloading";
constexpr char kAttrDisableP2PForSharing[] = "DisableP2PForSharing";
constexpr char kAttrDisablePayloadBackoff[] = "DisablePayloadBackoff";
constexpr char kAttrVersion[] = "version";
// Deprecated: "IsDelta"
constexpr char kAttrIsDeltaPayload[] = "IsDeltaPayload";
constexpr char kAttrMaxFailureCountPerUrl[] = "MaxFailureCountPerUrl";
constexpr char kAttrMaxDaysToScatter[] = "MaxDaysToScatter";
// Deprecated: "ManifestSignatureRsa"
// Deprecated: "ManifestSize"
constexpr char kAttrMetadataSignatureRsa[] = "MetadataSignatureRsa";
constexpr char kAttrMetadataSize[] = "MetadataSize";
constexpr char kAttrMoreInfo[] = "MoreInfo";
constexpr char kAttrNoUpdate[] = "noupdate";
// Deprecated: "NeedsAdmin"
constexpr char kAttrPollInterval[] = "PollInterval";
constexpr char kAttrPowerwash[] = "Powerwash";
constexpr char kAttrPrompt[] = "Prompt";
constexpr char kAttrPublicKeyRsa[] = "PublicKeyRsa";

// List of attributes that we interpret in the Omaha response:
constexpr char kAttrAppId[] = "appid";
constexpr char kAttrCodeBase[] = "codebase";
constexpr char kAttrCohort[] = "cohort";
constexpr char kAttrCohortHint[] = "cohorthint";
constexpr char kAttrCohortName[] = "cohortname";
constexpr char kAttrElapsedDays[] = "elapsed_days";
constexpr char kAttrElapsedSeconds[] = "elapsed_seconds";
constexpr char kAttrEvent[] = "event";
constexpr char kAttrHashSha256[] = "hash_sha256";
// Deprecated: "hash"; Although we still need to pass it from the server for
// backward compatibility.
constexpr char kAttrName[] = "name";
// Deprecated: "sha256"; Although we still need to pass it from the server for
// backward compatibility.
constexpr char kAttrSize[] = "size";
constexpr char kAttrStatus[] = "status";

// List of values that we interpret in the Omaha response:
constexpr char kValPostInstall[] = "postinstall";
constexpr char kValNoUpdate[] = "noupdate";

constexpr char kOmahaUpdaterVersion[] = "0.1.0.0";

// X-Goog-Update headers.
constexpr char kXGoogleUpdateInteractivity[] = "X-Goog-Update-Interactivity";
constexpr char kXGoogleUpdateAppId[] = "X-Goog-Update-AppId";
constexpr char kXGoogleUpdateUpdater[] = "X-Goog-Update-Updater";

// updatecheck attributes (without the underscore prefix).
constexpr char kAttrEol[] = "eol";
constexpr char kAttrRollback[] = "rollback";
constexpr char kAttrFirmwareVersion[] = "firmware_version";
constexpr char kAttrKernelVersion[] = "kernel_version";

namespace {

// Returns an XML ping element attribute assignment with attribute
// |name| and value |ping_days| if |ping_days| has a value that needs
// to be sent, or an empty string otherwise.
string GetPingAttribute(const string& name, int ping_days) {
  if (ping_days > 0 || ping_days == OmahaRequestAction::kNeverPinged)
    return base::StringPrintf(" %s=\"%d\"", name.c_str(), ping_days);
  return "";
}

// Returns an XML ping element if any of the elapsed days need to be
// sent, or an empty string otherwise.
string GetPingXml(int ping_active_days, int ping_roll_call_days) {
  string ping_active = GetPingAttribute("a", ping_active_days);
  string ping_roll_call = GetPingAttribute("r", ping_roll_call_days);
  if (!ping_active.empty() || !ping_roll_call.empty()) {
    return base::StringPrintf("        <ping active=\"1\"%s%s></ping>\n",
                              ping_active.c_str(),
                              ping_roll_call.c_str());
  }
  return "";
}

// Returns an XML that goes into the body of the <app> element of the Omaha
// request based on the given parameters.
string GetAppBody(const OmahaEvent* event,
                  OmahaRequestParams* params,
                  bool ping_only,
                  bool include_ping,
                  bool skip_updatecheck,
                  int ping_active_days,
                  int ping_roll_call_days,
                  PrefsInterface* prefs) {
  string app_body;
  if (event == nullptr) {
    if (include_ping)
      app_body = GetPingXml(ping_active_days, ping_roll_call_days);
    if (!ping_only) {
      if (!skip_updatecheck) {
        app_body += "        <updatecheck";
        if (!params->target_version_prefix().empty()) {
          app_body += base::StringPrintf(
              " targetversionprefix=\"%s\"",
              XmlEncodeWithDefault(params->target_version_prefix(), "")
                  .c_str());
          // Rollback requires target_version_prefix set.
          if (params->rollback_allowed()) {
            app_body += " rollback_allowed=\"true\"";
          }
        }
        app_body += "></updatecheck>\n";
      }

      // If this is the first update check after a reboot following a previous
      // update, generate an event containing the previous version number. If
      // the previous version preference file doesn't exist the event is still
      // generated with a previous version of 0.0.0.0 -- this is relevant for
      // older clients or new installs. The previous version event is not sent
      // for ping-only requests because they come before the client has
      // rebooted. The previous version event is also not sent if it was already
      // sent for this new version with a previous updatecheck.
      string prev_version;
      if (!prefs->GetString(kPrefsPreviousVersion, &prev_version)) {
        prev_version = "0.0.0.0";
      }
      // We only store a non-empty previous version value after a successful
      // update in the previous boot. After reporting it back to the server,
      // we clear the previous version value so it doesn't get reported again.
      if (!prev_version.empty()) {
        app_body += base::StringPrintf(
            "        <event eventtype=\"%d\" eventresult=\"%d\" "
            "previousversion=\"%s\"></event>\n",
            OmahaEvent::kTypeRebootedAfterUpdate,
            OmahaEvent::kResultSuccess,
            XmlEncodeWithDefault(prev_version, "0.0.0.0").c_str());
        LOG_IF(WARNING, !prefs->SetString(kPrefsPreviousVersion, ""))
            << "Unable to reset the previous version.";
      }
    }
  } else {
    // The error code is an optional attribute so append it only if the result
    // is not success.
    string error_code;
    if (event->result != OmahaEvent::kResultSuccess) {
      error_code = base::StringPrintf(" errorcode=\"%d\"",
                                      static_cast<int>(event->error_code));
    }
    app_body = base::StringPrintf(
        "        <event eventtype=\"%d\" eventresult=\"%d\"%s></event>\n",
        event->type,
        event->result,
        error_code.c_str());
  }

  return app_body;
}

// Returns the cohort* argument to include in the <app> tag for the passed
// |arg_name| and |prefs_key|, if any. The return value is suitable to
// concatenate to the list of arguments and includes a space at the end.
string GetCohortArgXml(PrefsInterface* prefs,
                       const string arg_name,
                       const string prefs_key) {
  // There's nothing wrong with not having a given cohort setting, so we check
  // existence first to avoid the warning log message.
  if (!prefs->Exists(prefs_key))
    return "";
  string cohort_value;
  if (!prefs->GetString(prefs_key, &cohort_value) || cohort_value.empty())
    return "";
  // This is a sanity check to avoid sending a huge XML file back to Ohama due
  // to a compromised stateful partition making the update check fail in low
  // network environments envent after a reboot.
  if (cohort_value.size() > 1024) {
    LOG(WARNING) << "The omaha cohort setting " << arg_name
                 << " has a too big value, which must be an error or an "
                    "attacker trying to inhibit updates.";
    return "";
  }

  string escaped_xml_value;
  if (!XmlEncode(cohort_value, &escaped_xml_value)) {
    LOG(WARNING) << "The omaha cohort setting " << arg_name
                 << " is ASCII-7 invalid, ignoring it.";
    return "";
  }

  return base::StringPrintf(
      "%s=\"%s\" ", arg_name.c_str(), escaped_xml_value.c_str());
}

struct OmahaAppData {
  string id;
  string version;
  string product_components;
};

bool IsValidComponentID(const string& id) {
  for (char c : id) {
    if (!isalnum(c) && c != '-' && c != '_' && c != '.')
      return false;
  }
  return true;
}

// Returns an XML that corresponds to the entire <app> node of the Omaha
// request based on the given parameters.
string GetAppXml(const OmahaEvent* event,
                 OmahaRequestParams* params,
                 const OmahaAppData& app_data,
                 bool ping_only,
                 bool include_ping,
                 bool skip_updatecheck,
                 int ping_active_days,
                 int ping_roll_call_days,
                 int install_date_in_days,
                 SystemState* system_state) {
  string app_body = GetAppBody(event,
                               params,
                               ping_only,
                               include_ping,
                               skip_updatecheck,
                               ping_active_days,
                               ping_roll_call_days,
                               system_state->prefs());
  string app_versions;

  // If we are downgrading to a more stable channel and we are allowed to do
  // powerwash, then pass 0.0.0.0 as the version. This is needed to get the
  // highest-versioned payload on the destination channel.
  if (params->ShouldPowerwash()) {
    LOG(INFO) << "Passing OS version as 0.0.0.0 as we are set to powerwash "
              << "on downgrading to the version in the more stable channel";
    app_versions = "version=\"0.0.0.0\" from_version=\"" +
                   XmlEncodeWithDefault(app_data.version, "0.0.0.0") + "\" ";
  } else {
    app_versions = "version=\"" +
                   XmlEncodeWithDefault(app_data.version, "0.0.0.0") + "\" ";
  }

  string download_channel = params->download_channel();
  string app_channels =
      "track=\"" + XmlEncodeWithDefault(download_channel, "") + "\" ";
  if (params->current_channel() != download_channel) {
    app_channels += "from_track=\"" +
                    XmlEncodeWithDefault(params->current_channel(), "") + "\" ";
  }

  string delta_okay_str = params->delta_okay() ? "true" : "false";

  // If install_date_days is not set (e.g. its value is -1 ), don't
  // include the attribute.
  string install_date_in_days_str = "";
  if (install_date_in_days >= 0) {
    install_date_in_days_str =
        base::StringPrintf("installdate=\"%d\" ", install_date_in_days);
  }

  string app_cohort_args;
  app_cohort_args +=
      GetCohortArgXml(system_state->prefs(), "cohort", kPrefsOmahaCohort);
  app_cohort_args += GetCohortArgXml(
      system_state->prefs(), "cohorthint", kPrefsOmahaCohortHint);
  app_cohort_args += GetCohortArgXml(
      system_state->prefs(), "cohortname", kPrefsOmahaCohortName);

  string fingerprint_arg;
  if (!params->os_build_fingerprint().empty()) {
    fingerprint_arg = "fingerprint=\"" +
                      XmlEncodeWithDefault(params->os_build_fingerprint(), "") +
                      "\" ";
  }

  string buildtype_arg;
  if (!params->os_build_type().empty()) {
    buildtype_arg = "os_build_type=\"" +
                    XmlEncodeWithDefault(params->os_build_type(), "") + "\" ";
  }

  string product_components_args;
  if (!params->ShouldPowerwash() && !app_data.product_components.empty()) {
    brillo::KeyValueStore store;
    if (store.LoadFromString(app_data.product_components)) {
      for (const string& key : store.GetKeys()) {
        if (!IsValidComponentID(key)) {
          LOG(ERROR) << "Invalid component id: " << key;
          continue;
        }
        string version;
        if (!store.GetString(key, &version)) {
          LOG(ERROR) << "Failed to get version for " << key
                     << " in product_components.";
          continue;
        }
        product_components_args +=
            base::StringPrintf("_%s.version=\"%s\" ",
                               key.c_str(),
                               XmlEncodeWithDefault(version, "").c_str());
      }
    } else {
      LOG(ERROR) << "Failed to parse product_components:\n"
                 << app_data.product_components;
    }
  }

  // clang-format off
  string app_xml = "    <app "
      "appid=\"" + XmlEncodeWithDefault(app_data.id, "") + "\" " +
      app_cohort_args +
      app_versions +
      app_channels +
      product_components_args +
      fingerprint_arg +
      buildtype_arg +
      "lang=\"" + XmlEncodeWithDefault(params->app_lang(), "en-US") + "\" " +
      "board=\"" + XmlEncodeWithDefault(params->os_board(), "") + "\" " +
      "hardware_class=\"" + XmlEncodeWithDefault(params->hwid(), "") + "\" " +
      "delta_okay=\"" + delta_okay_str + "\" "
      "fw_version=\"" + XmlEncodeWithDefault(params->fw_version(), "") + "\" " +
      "ec_version=\"" + XmlEncodeWithDefault(params->ec_version(), "") + "\" " +
      install_date_in_days_str +
      ">\n" +
         app_body +
      "    </app>\n";
  // clang-format on
  return app_xml;
}

// Returns an XML that corresponds to the entire <os> node of the Omaha
// request based on the given parameters.
string GetOsXml(OmahaRequestParams* params) {
  string os_xml =
      "    <os "
      "version=\"" +
      XmlEncodeWithDefault(params->os_version(), "") + "\" " + "platform=\"" +
      XmlEncodeWithDefault(params->os_platform(), "") + "\" " + "sp=\"" +
      XmlEncodeWithDefault(params->os_sp(), "") +
      "\">"
      "</os>\n";
  return os_xml;
}

// Returns an XML that corresponds to the entire Omaha request based on the
// given parameters.
string GetRequestXml(const OmahaEvent* event,
                     OmahaRequestParams* params,
                     bool ping_only,
                     bool include_ping,
                     int ping_active_days,
                     int ping_roll_call_days,
                     int install_date_in_days,
                     SystemState* system_state) {
  string os_xml = GetOsXml(params);
  OmahaAppData product_app = {
      .id = params->GetAppId(),
      .version = params->app_version(),
      .product_components = params->product_components()};
  // Skips updatecheck for platform app in case of an install operation.
  string app_xml = GetAppXml(event,
                             params,
                             product_app,
                             ping_only,
                             include_ping,
                             params->is_install(), /* skip_updatecheck */
                             ping_active_days,
                             ping_roll_call_days,
                             install_date_in_days,
                             system_state);
  if (!params->system_app_id().empty()) {
    OmahaAppData system_app = {.id = params->system_app_id(),
                               .version = params->system_version()};
    app_xml += GetAppXml(event,
                         params,
                         system_app,
                         ping_only,
                         include_ping,
                         false, /* skip_updatecheck */
                         ping_active_days,
                         ping_roll_call_days,
                         install_date_in_days,
                         system_state);
  }
  // Create APP ID according to |dlc_module_id| (sticking the current AppID to
  // the DLC module ID with an underscode).
  for (const auto& dlc_module_id : params->dlc_module_ids()) {
    OmahaAppData dlc_module_app = {
        .id = params->GetAppId() + "_" + dlc_module_id,
        .version = params->app_version()};
    app_xml += GetAppXml(event,
                         params,
                         dlc_module_app,
                         ping_only,
                         include_ping,
                         false, /* skip_updatecheck */
                         ping_active_days,
                         ping_roll_call_days,
                         install_date_in_days,
                         system_state);
  }

  string install_source = base::StringPrintf(
      "installsource=\"%s\" ",
      (params->interactive() ? "ondemandupdate" : "scheduler"));

  string updater_version = XmlEncodeWithDefault(
      base::StringPrintf(
          "%s-%s", constants::kOmahaUpdaterID, kOmahaUpdaterVersion),
      "");
  string request_xml =
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
      "<request protocol=\"3.0\" " +
      ("version=\"" + updater_version +
       "\" "
       "updaterversion=\"" +
       updater_version + "\" " + install_source + "ismachine=\"1\">\n") +
      os_xml + app_xml + "</request>\n";

  return request_xml;
}

}  // namespace

// Struct used for holding data obtained when parsing the XML.
struct OmahaParserData {
  explicit OmahaParserData(XML_Parser _xml_parser) : xml_parser(_xml_parser) {}

  // Pointer to the expat XML_Parser object.
  XML_Parser xml_parser;

  // This is the state of the parser as it's processing the XML.
  bool failed = false;
  bool entity_decl = false;
  string current_path;

  // These are the values extracted from the XML.
  string updatecheck_poll_interval;
  map<string, string> updatecheck_attrs;
  string daystart_elapsed_days;
  string daystart_elapsed_seconds;

  struct App {
    string id;
    vector<string> url_codebase;
    string manifest_version;
    map<string, string> action_postinstall_attrs;
    string updatecheck_status;
    string cohort;
    string cohorthint;
    string cohortname;
    bool cohort_set = false;
    bool cohorthint_set = false;
    bool cohortname_set = false;

    struct Package {
      string name;
      string size;
      string hash;
    };
    vector<Package> packages;
  };
  vector<App> apps;
};

namespace {

// Callback function invoked by expat.
void ParserHandlerStart(void* user_data,
                        const XML_Char* element,
                        const XML_Char** attr) {
  OmahaParserData* data = reinterpret_cast<OmahaParserData*>(user_data);

  if (data->failed)
    return;

  data->current_path += string("/") + element;

  map<string, string> attrs;
  if (attr != nullptr) {
    for (int n = 0; attr[n] != nullptr && attr[n + 1] != nullptr; n += 2) {
      string key = attr[n];
      string value = attr[n + 1];
      attrs[key] = value;
    }
  }

  if (data->current_path == "/response/app") {
    OmahaParserData::App app;
    if (attrs.find(kAttrAppId) != attrs.end()) {
      app.id = attrs[kAttrAppId];
    }
    if (attrs.find(kAttrCohort) != attrs.end()) {
      app.cohort_set = true;
      app.cohort = attrs[kAttrCohort];
    }
    if (attrs.find(kAttrCohortHint) != attrs.end()) {
      app.cohorthint_set = true;
      app.cohorthint = attrs[kAttrCohortHint];
    }
    if (attrs.find(kAttrCohortName) != attrs.end()) {
      app.cohortname_set = true;
      app.cohortname = attrs[kAttrCohortName];
    }
    data->apps.push_back(std::move(app));
  } else if (data->current_path == "/response/app/updatecheck") {
    if (!data->apps.empty())
      data->apps.back().updatecheck_status = attrs[kAttrStatus];
    if (data->updatecheck_poll_interval.empty())
      data->updatecheck_poll_interval = attrs[kAttrPollInterval];
    // Omaha sends arbitrary key-value pairs as extra attributes starting with
    // an underscore.
    for (const auto& attr : attrs) {
      if (!attr.first.empty() && attr.first[0] == '_')
        data->updatecheck_attrs[attr.first.substr(1)] = attr.second;
    }
  } else if (data->current_path == "/response/daystart") {
    // Get the install-date.
    data->daystart_elapsed_days = attrs[kAttrElapsedDays];
    data->daystart_elapsed_seconds = attrs[kAttrElapsedSeconds];
  } else if (data->current_path == "/response/app/updatecheck/urls/url") {
    // Look at all <url> elements.
    if (!data->apps.empty())
      data->apps.back().url_codebase.push_back(attrs[kAttrCodeBase]);
  } else if (data->current_path ==
             "/response/app/updatecheck/manifest/packages/package") {
    // Look at all <package> elements.
    if (!data->apps.empty())
      data->apps.back().packages.push_back({.name = attrs[kAttrName],
                                            .size = attrs[kAttrSize],
                                            .hash = attrs[kAttrHashSha256]});
  } else if (data->current_path == "/response/app/updatecheck/manifest") {
    // Get the version.
    if (!data->apps.empty())
      data->apps.back().manifest_version = attrs[kAttrVersion];
  } else if (data->current_path ==
             "/response/app/updatecheck/manifest/actions/action") {
    // We only care about the postinstall action.
    if (attrs[kAttrEvent] == kValPostInstall && !data->apps.empty()) {
      data->apps.back().action_postinstall_attrs = std::move(attrs);
    }
  }
}

// Callback function invoked by expat.
void ParserHandlerEnd(void* user_data, const XML_Char* element) {
  OmahaParserData* data = reinterpret_cast<OmahaParserData*>(user_data);
  if (data->failed)
    return;

  const string path_suffix = string("/") + element;

  if (!base::EndsWith(
          data->current_path, path_suffix, base::CompareCase::SENSITIVE)) {
    LOG(ERROR) << "Unexpected end element '" << element
               << "' with current_path='" << data->current_path << "'";
    data->failed = true;
    return;
  }
  data->current_path.resize(data->current_path.size() - path_suffix.size());
}

// Callback function invoked by expat.
//
// This is called for entity declarations. Since Omaha is guaranteed
// to never return any XML with entities our course of action is to
// just stop parsing. This avoids potential resource exhaustion
// problems AKA the "billion laughs". CVE-2013-0340.
void ParserHandlerEntityDecl(void* user_data,
                             const XML_Char* entity_name,
                             int is_parameter_entity,
                             const XML_Char* value,
                             int value_length,
                             const XML_Char* base,
                             const XML_Char* system_id,
                             const XML_Char* public_id,
                             const XML_Char* notation_name) {
  OmahaParserData* data = reinterpret_cast<OmahaParserData*>(user_data);

  LOG(ERROR) << "XML entities are not supported. Aborting parsing.";
  data->failed = true;
  data->entity_decl = true;
  XML_StopParser(data->xml_parser, false);
}

}  // namespace

bool XmlEncode(const string& input, string* output) {
  if (std::find_if(input.begin(), input.end(), [](const char c) {
        return c & 0x80;
      }) != input.end()) {
    LOG(WARNING) << "Invalid ASCII-7 string passed to the XML encoder:";
    utils::HexDumpString(input);
    return false;
  }
  output->clear();
  // We need at least input.size() space in the output, but the code below will
  // handle it if we need more.
  output->reserve(input.size());
  for (char c : input) {
    switch (c) {
      case '\"':
        output->append("&quot;");
        break;
      case '\'':
        output->append("&apos;");
        break;
      case '&':
        output->append("&amp;");
        break;
      case '<':
        output->append("&lt;");
        break;
      case '>':
        output->append("&gt;");
        break;
      default:
        output->push_back(c);
    }
  }
  return true;
}

string XmlEncodeWithDefault(const string& input, const string& default_value) {
  string output;
  if (XmlEncode(input, &output))
    return output;
  return default_value;
}

OmahaRequestAction::OmahaRequestAction(
    SystemState* system_state,
    OmahaEvent* event,
    std::unique_ptr<HttpFetcher> http_fetcher,
    bool ping_only)
    : system_state_(system_state),
      params_(system_state->request_params()),
      event_(event),
      http_fetcher_(std::move(http_fetcher)),
      policy_provider_(std::make_unique<policy::PolicyProvider>()),
      ping_only_(ping_only),
      ping_active_days_(0),
      ping_roll_call_days_(0) {
  policy_provider_->Reload();
}

OmahaRequestAction::~OmahaRequestAction() {}

// Calculates the value to use for the ping days parameter.
int OmahaRequestAction::CalculatePingDays(const string& key) {
  int days = kNeverPinged;
  int64_t last_ping = 0;
  if (system_state_->prefs()->GetInt64(key, &last_ping) && last_ping >= 0) {
    days = (Time::Now() - Time::FromInternalValue(last_ping)).InDays();
    if (days < 0) {
      // If |days| is negative, then the system clock must have jumped
      // back in time since the ping was sent. Mark the value so that
      // it doesn't get sent to the server but we still update the
      // last ping daystart preference. This way the next ping time
      // will be correct, hopefully.
      days = kPingTimeJump;
      LOG(WARNING)
          << "System clock jumped back in time. Resetting ping daystarts.";
    }
  }
  return days;
}

void OmahaRequestAction::InitPingDays() {
  // We send pings only along with update checks, not with events.
  if (IsEvent()) {
    return;
  }
  // TODO(petkov): Figure a way to distinguish active use pings
  // vs. roll call pings. Currently, the two pings are identical. A
  // fix needs to change this code as well as UpdateLastPingDays and ShouldPing.
  ping_active_days_ = CalculatePingDays(kPrefsLastActivePingDay);
  ping_roll_call_days_ = CalculatePingDays(kPrefsLastRollCallPingDay);
}

bool OmahaRequestAction::ShouldPing() const {
  if (ping_active_days_ == OmahaRequestAction::kNeverPinged &&
      ping_roll_call_days_ == OmahaRequestAction::kNeverPinged) {
    int powerwash_count = system_state_->hardware()->GetPowerwashCount();
    if (powerwash_count > 0) {
      LOG(INFO) << "Not sending ping with a=-1 r=-1 to omaha because "
                << "powerwash_count is " << powerwash_count;
      return false;
    }
    if (system_state_->hardware()->GetFirstActiveOmahaPingSent()) {
      LOG(INFO) << "Not sending ping with a=-1 r=-1 to omaha because "
                << "the first_active_omaha_ping_sent is true";
      return false;
    }
    return true;
  }
  return ping_active_days_ > 0 || ping_roll_call_days_ > 0;
}

// static
int OmahaRequestAction::GetInstallDate(SystemState* system_state) {
  PrefsInterface* prefs = system_state->prefs();
  if (prefs == nullptr)
    return -1;

  // If we have the value stored on disk, just return it.
  int64_t stored_value;
  if (prefs->GetInt64(kPrefsInstallDateDays, &stored_value)) {
    // Convert and sanity-check.
    int install_date_days = static_cast<int>(stored_value);
    if (install_date_days >= 0)
      return install_date_days;
    LOG(ERROR) << "Dropping stored Omaha InstallData since its value num_days="
               << install_date_days << " looks suspicious.";
    prefs->Delete(kPrefsInstallDateDays);
  }

  // Otherwise, if OOBE is not complete then do nothing and wait for
  // ParseResponse() to call ParseInstallDate() and then
  // PersistInstallDate() to set the kPrefsInstallDateDays state
  // variable. Once that is done, we'll then report back in future
  // Omaha requests.  This works exactly because OOBE triggers an
  // update check.
  //
  // However, if OOBE is complete and the kPrefsInstallDateDays state
  // variable is not set, there are two possibilities
  //
  //   1. The update check in OOBE failed so we never got a response
  //      from Omaha (no network etc.); or
  //
  //   2. OOBE was done on an older version that didn't write to the
  //      kPrefsInstallDateDays state variable.
  //
  // In both cases, we approximate the install date by simply
  // inspecting the timestamp of when OOBE happened.

  Time time_of_oobe;
  if (!system_state->hardware()->IsOOBEEnabled() ||
      !system_state->hardware()->IsOOBEComplete(&time_of_oobe)) {
    LOG(INFO) << "Not generating Omaha InstallData as we have "
              << "no prefs file and OOBE is not complete or not enabled.";
    return -1;
  }

  int num_days;
  if (!utils::ConvertToOmahaInstallDate(time_of_oobe, &num_days)) {
    LOG(ERROR) << "Not generating Omaha InstallData from time of OOBE "
               << "as its value '" << utils::ToString(time_of_oobe)
               << "' looks suspicious.";
    return -1;
  }

  // Persist this to disk, for future use.
  if (!OmahaRequestAction::PersistInstallDate(
          system_state, num_days, kProvisionedFromOOBEMarker))
    return -1;

  LOG(INFO) << "Set the Omaha InstallDate from OOBE time-stamp to " << num_days
            << " days";

  return num_days;
}

void OmahaRequestAction::PerformAction() {
  http_fetcher_->set_delegate(this);
  InitPingDays();
  if (ping_only_ && !ShouldPing()) {
    processor_->ActionComplete(this, ErrorCode::kSuccess);
    return;
  }

  string request_post(GetRequestXml(event_.get(),
                                    params_,
                                    ping_only_,
                                    ShouldPing(),  // include_ping
                                    ping_active_days_,
                                    ping_roll_call_days_,
                                    GetInstallDate(system_state_),
                                    system_state_));

  // Set X-Goog-Update headers.
  http_fetcher_->SetHeader(kXGoogleUpdateInteractivity,
                           params_->interactive() ? "fg" : "bg");
  http_fetcher_->SetHeader(kXGoogleUpdateAppId, params_->GetAppId());
  http_fetcher_->SetHeader(
      kXGoogleUpdateUpdater,
      base::StringPrintf(
          "%s-%s", constants::kOmahaUpdaterID, kOmahaUpdaterVersion));

  http_fetcher_->SetPostData(
      request_post.data(), request_post.size(), kHttpContentTypeTextXml);
  LOG(INFO) << "Posting an Omaha request to " << params_->update_url();
  LOG(INFO) << "Request: " << request_post;
  http_fetcher_->BeginTransfer(params_->update_url());
}

void OmahaRequestAction::TerminateProcessing() {
  http_fetcher_->TerminateTransfer();
}

// We just store the response in the buffer. Once we've received all bytes,
// we'll look in the buffer and decide what to do.
bool OmahaRequestAction::ReceivedBytes(HttpFetcher* fetcher,
                                       const void* bytes,
                                       size_t length) {
  const uint8_t* byte_ptr = reinterpret_cast<const uint8_t*>(bytes);
  response_buffer_.insert(response_buffer_.end(), byte_ptr, byte_ptr + length);
  return true;
}

namespace {

// Parses a 64 bit base-10 int from a string and returns it. Returns 0
// on error. If the string contains "0", that's indistinguishable from
// error.
off_t ParseInt(const string& str) {
  off_t ret = 0;
  int rc = sscanf(str.c_str(), "%" PRIi64, &ret);  // NOLINT(runtime/printf)
  if (rc < 1) {
    // failure
    return 0;
  }
  return ret;
}

// Parses |str| and returns |true| if, and only if, its value is "true".
bool ParseBool(const string& str) {
  return str == "true";
}

// Update the last ping day preferences based on the server daystart
// response. Returns true on success, false otherwise.
bool UpdateLastPingDays(OmahaParserData* parser_data, PrefsInterface* prefs) {
  int64_t elapsed_seconds = 0;
  TEST_AND_RETURN_FALSE(base::StringToInt64(
      parser_data->daystart_elapsed_seconds, &elapsed_seconds));
  TEST_AND_RETURN_FALSE(elapsed_seconds >= 0);

  // Remember the local time that matches the server's last midnight
  // time.
  Time daystart = Time::Now() - TimeDelta::FromSeconds(elapsed_seconds);
  prefs->SetInt64(kPrefsLastActivePingDay, daystart.ToInternalValue());
  prefs->SetInt64(kPrefsLastRollCallPingDay, daystart.ToInternalValue());
  return true;
}

// Parses the package node in the given XML document and populates
// |output_object| if valid. Returns true if we should continue the parsing.
// False otherwise, in which case it sets any error code using |completer|.
bool ParsePackage(OmahaParserData::App* app,
                  OmahaResponse* output_object,
                  ScopedActionCompleter* completer) {
  if (app->updatecheck_status.empty() ||
      app->updatecheck_status == kValNoUpdate) {
    if (!app->packages.empty()) {
      LOG(ERROR) << "No update in this <app> but <package> is not empty.";
      completer->set_code(ErrorCode::kOmahaResponseInvalid);
      return false;
    }
    return true;
  }
  if (app->packages.empty()) {
    LOG(ERROR) << "Omaha Response has no packages";
    completer->set_code(ErrorCode::kOmahaResponseInvalid);
    return false;
  }
  if (app->url_codebase.empty()) {
    LOG(ERROR) << "No Omaha Response URLs";
    completer->set_code(ErrorCode::kOmahaResponseInvalid);
    return false;
  }
  LOG(INFO) << "Found " << app->url_codebase.size() << " url(s)";
  vector<string> metadata_sizes =
      base::SplitString(app->action_postinstall_attrs[kAttrMetadataSize],
                        ":",
                        base::TRIM_WHITESPACE,
                        base::SPLIT_WANT_ALL);
  vector<string> metadata_signatures = base::SplitString(
      app->action_postinstall_attrs[kAttrMetadataSignatureRsa],
      ":",
      base::TRIM_WHITESPACE,
      base::SPLIT_WANT_ALL);
  vector<string> is_delta_payloads =
      base::SplitString(app->action_postinstall_attrs[kAttrIsDeltaPayload],
                        ":",
                        base::TRIM_WHITESPACE,
                        base::SPLIT_WANT_ALL);
  for (size_t i = 0; i < app->packages.size(); i++) {
    const auto& package = app->packages[i];
    if (package.name.empty()) {
      LOG(ERROR) << "Omaha Response has empty package name";
      completer->set_code(ErrorCode::kOmahaResponseInvalid);
      return false;
    }
    LOG(INFO) << "Found package " << package.name;

    OmahaResponse::Package out_package;
    for (const string& codebase : app->url_codebase) {
      if (codebase.empty()) {
        LOG(ERROR) << "Omaha Response URL has empty codebase";
        completer->set_code(ErrorCode::kOmahaResponseInvalid);
        return false;
      }
      out_package.payload_urls.push_back(codebase + package.name);
    }
    // Parse the payload size.
    base::StringToUint64(package.size, &out_package.size);
    if (out_package.size <= 0) {
      LOG(ERROR) << "Omaha Response has invalid payload size: " << package.size;
      completer->set_code(ErrorCode::kOmahaResponseInvalid);
      return false;
    }
    LOG(INFO) << "Payload size = " << out_package.size << " bytes";

    if (i < metadata_sizes.size())
      base::StringToUint64(metadata_sizes[i], &out_package.metadata_size);
    LOG(INFO) << "Payload metadata size = " << out_package.metadata_size
              << " bytes";

    if (i < metadata_signatures.size())
      out_package.metadata_signature = metadata_signatures[i];
    LOG(INFO) << "Payload metadata signature = "
              << out_package.metadata_signature;

    out_package.hash = package.hash;
    if (out_package.hash.empty()) {
      LOG(ERROR) << "Omaha Response has empty hash_sha256 value";
      completer->set_code(ErrorCode::kOmahaResponseInvalid);
      return false;
    }
    LOG(INFO) << "Payload hash = " << out_package.hash;

    if (i < is_delta_payloads.size())
      out_package.is_delta = ParseBool(is_delta_payloads[i]);
    LOG(INFO) << "Payload is delta = " << utils::ToString(out_package.is_delta);

    output_object->packages.push_back(std::move(out_package));
  }

  return true;
}

// Parses the 2 key version strings kernel_version and firmware_version. If the
// field is not present, or cannot be parsed the values default to 0xffff.
void ParseRollbackVersions(OmahaParserData* parser_data,
                           OmahaResponse* output_object) {
  utils::ParseRollbackKeyVersion(
      parser_data->updatecheck_attrs[kAttrFirmwareVersion],
      &output_object->rollback_key_version.firmware_key,
      &output_object->rollback_key_version.firmware);
  utils::ParseRollbackKeyVersion(
      parser_data->updatecheck_attrs[kAttrKernelVersion],
      &output_object->rollback_key_version.kernel_key,
      &output_object->rollback_key_version.kernel);
}

}  // namespace

bool OmahaRequestAction::ParseResponse(OmahaParserData* parser_data,
                                       OmahaResponse* output_object,
                                       ScopedActionCompleter* completer) {
  if (parser_data->apps.empty()) {
    completer->set_code(ErrorCode::kOmahaResponseInvalid);
    return false;
  }
  LOG(INFO) << "Found " << parser_data->apps.size() << " <app>.";

  // chromium-os:37289: The PollInterval is not supported by Omaha server
  // currently.  But still keeping this existing code in case we ever decide to
  // slow down the request rate from the server-side. Note that the PollInterval
  // is not persisted, so it has to be sent by the server on every response to
  // guarantee that the scheduler uses this value (otherwise, if the device got
  // rebooted after the last server-indicated value, it'll revert to the default
  // value). Also kDefaultMaxUpdateChecks value for the scattering logic is
  // based on the assumption that we perform an update check every hour so that
  // the max value of 8 will roughly be equivalent to one work day. If we decide
  // to use PollInterval permanently, we should update the
  // max_update_checks_allowed to take PollInterval into account.  Note: The
  // parsing for PollInterval happens even before parsing of the status because
  // we may want to specify the PollInterval even when there's no update.
  base::StringToInt(parser_data->updatecheck_poll_interval,
                    &output_object->poll_interval);

  // Check for the "elapsed_days" attribute in the "daystart"
  // element. This is the number of days since Jan 1 2007, 0:00
  // PST. If we don't have a persisted value of the Omaha InstallDate,
  // we'll use it to calculate it and then persist it.
  if (ParseInstallDate(parser_data, output_object) &&
      !HasInstallDate(system_state_)) {
    // Since output_object->install_date_days is never negative, the
    // elapsed_days -> install-date calculation is reduced to simply
    // rounding down to the nearest number divisible by 7.
    int remainder = output_object->install_date_days % 7;
    int install_date_days_rounded =
        output_object->install_date_days - remainder;
    if (PersistInstallDate(system_state_,
                           install_date_days_rounded,
                           kProvisionedFromOmahaResponse)) {
      LOG(INFO) << "Set the Omaha InstallDate from Omaha Response to "
                << install_date_days_rounded << " days";
    }
  }

  // We persist the cohorts sent by omaha even if the status is "noupdate".
  for (const auto& app : parser_data->apps) {
    if (app.id == params_->GetAppId()) {
      if (app.cohort_set)
        PersistCohortData(kPrefsOmahaCohort, app.cohort);
      if (app.cohorthint_set)
        PersistCohortData(kPrefsOmahaCohortHint, app.cohorthint);
      if (app.cohortname_set)
        PersistCohortData(kPrefsOmahaCohortName, app.cohortname);
      break;
    }
  }

  // Parse the updatecheck attributes.
  PersistEolStatus(parser_data->updatecheck_attrs);
  // Rollback-related updatecheck attributes.
  // Defaults to false if attribute is not present.
  output_object->is_rollback =
      ParseBool(parser_data->updatecheck_attrs[kAttrRollback]);

  // Parses the rollback versions of the current image. If the fields do not
  // exist they default to 0xffff for the 4 key versions.
  ParseRollbackVersions(parser_data, output_object);

  if (!ParseStatus(parser_data, output_object, completer))
    return false;

  if (!ParseParams(parser_data, output_object, completer))
    return false;

  // Package has to be parsed after Params now because ParseParams need to make
  // sure that postinstall action exists.
  for (auto& app : parser_data->apps)
    if (!ParsePackage(&app, output_object, completer))
      return false;

  return true;
}

bool OmahaRequestAction::ParseStatus(OmahaParserData* parser_data,
                                     OmahaResponse* output_object,
                                     ScopedActionCompleter* completer) {
  output_object->update_exists = false;
  for (const auto& app : parser_data->apps) {
    const string& status = app.updatecheck_status;
    if (status == kValNoUpdate) {
      // Don't update if any app has status="noupdate".
      LOG(INFO) << "No update for <app> " << app.id;
      output_object->update_exists = false;
      break;
    } else if (status == "ok") {
      auto const& attr_no_update =
          app.action_postinstall_attrs.find(kAttrNoUpdate);
      if (attr_no_update != app.action_postinstall_attrs.end() &&
          attr_no_update->second == "true") {
        // noupdate="true" in postinstall attributes means it's an update to
        // self, only update if there's at least one app really have update.
        LOG(INFO) << "Update to self for <app> " << app.id;
      } else {
        LOG(INFO) << "Update for <app> " << app.id;
        output_object->update_exists = true;
      }
    } else if (status.empty() && params_->is_install() &&
               params_->GetAppId() == app.id) {
      // Skips the platform app for install operation.
      LOG(INFO) << "No payload (and ignore) for <app> " << app.id;
    } else {
      LOG(ERROR) << "Unknown Omaha response status: " << status;
      completer->set_code(ErrorCode::kOmahaResponseInvalid);
      return false;
    }
  }
  if (!output_object->update_exists) {
    SetOutputObject(*output_object);
    completer->set_code(ErrorCode::kSuccess);
  }

  return output_object->update_exists;
}

bool OmahaRequestAction::ParseParams(OmahaParserData* parser_data,
                                     OmahaResponse* output_object,
                                     ScopedActionCompleter* completer) {
  map<string, string> attrs;
  for (auto& app : parser_data->apps) {
    if (app.id == params_->GetAppId()) {
      // this is the app (potentially the only app)
      output_object->version = app.manifest_version;
    } else if (!params_->system_app_id().empty() &&
               app.id == params_->system_app_id()) {
      // this is the system app (this check is intentionally skipped if there is
      // no system_app_id set)
      output_object->system_version = app.manifest_version;
    } else if (params_->is_install() &&
               app.manifest_version != params_->app_version()) {
      LOG(WARNING) << "An app has a different version (" << app.manifest_version
                   << ") that is different than platform app version ("
                   << params_->app_version() << ")";
    }
    if (!app.action_postinstall_attrs.empty() && attrs.empty()) {
      attrs = app.action_postinstall_attrs;
    }
  }
  if (params_->is_install()) {
    LOG(INFO) << "Use request version for Install operation.";
    output_object->version = params_->app_version();
  }
  if (output_object->version.empty()) {
    LOG(ERROR) << "Omaha Response does not have version in manifest!";
    completer->set_code(ErrorCode::kOmahaResponseInvalid);
    return false;
  }

  LOG(INFO) << "Received omaha response to update to version "
            << output_object->version;

  if (attrs.empty()) {
    LOG(ERROR) << "Omaha Response has no postinstall event action";
    completer->set_code(ErrorCode::kOmahaResponseInvalid);
    return false;
  }

  // Get the optional properties one by one.
  output_object->more_info_url = attrs[kAttrMoreInfo];
  output_object->prompt = ParseBool(attrs[kAttrPrompt]);
  output_object->deadline = attrs[kAttrDeadline];
  output_object->max_days_to_scatter = ParseInt(attrs[kAttrMaxDaysToScatter]);
  output_object->disable_p2p_for_downloading =
      ParseBool(attrs[kAttrDisableP2PForDownloading]);
  output_object->disable_p2p_for_sharing =
      ParseBool(attrs[kAttrDisableP2PForSharing]);
  output_object->public_key_rsa = attrs[kAttrPublicKeyRsa];

  string max = attrs[kAttrMaxFailureCountPerUrl];
  if (!base::StringToUint(max, &output_object->max_failure_count_per_url))
    output_object->max_failure_count_per_url = kDefaultMaxFailureCountPerUrl;

  output_object->disable_payload_backoff =
      ParseBool(attrs[kAttrDisablePayloadBackoff]);
  output_object->powerwash_required = ParseBool(attrs[kAttrPowerwash]);

  return true;
}

// If the transfer was successful, this uses expat to parse the response
// and fill in the appropriate fields of the output object. Also, notifies
// the processor that we're done.
void OmahaRequestAction::TransferComplete(HttpFetcher* fetcher,
                                          bool successful) {
  ScopedActionCompleter completer(processor_, this);
  string current_response(response_buffer_.begin(), response_buffer_.end());
  LOG(INFO) << "Omaha request response: " << current_response;

  PayloadStateInterface* const payload_state = system_state_->payload_state();

  // Set the max kernel key version based on whether rollback is allowed.
  SetMaxKernelKeyVersionForRollback();

  // Events are best effort transactions -- assume they always succeed.
  if (IsEvent()) {
    CHECK(!HasOutputPipe()) << "No output pipe allowed for event requests.";
    completer.set_code(ErrorCode::kSuccess);
    return;
  }

  if (!successful) {
    LOG(ERROR) << "Omaha request network transfer failed.";
    int code = GetHTTPResponseCode();
    // Makes sure we send sane error values.
    if (code < 0 || code >= 1000) {
      code = 999;
    }
    completer.set_code(static_cast<ErrorCode>(
        static_cast<int>(ErrorCode::kOmahaRequestHTTPResponseBase) + code));
    return;
  }

  XML_Parser parser = XML_ParserCreate(nullptr);
  OmahaParserData parser_data(parser);
  XML_SetUserData(parser, &parser_data);
  XML_SetElementHandler(parser, ParserHandlerStart, ParserHandlerEnd);
  XML_SetEntityDeclHandler(parser, ParserHandlerEntityDecl);
  XML_Status res =
      XML_Parse(parser,
                reinterpret_cast<const char*>(response_buffer_.data()),
                response_buffer_.size(),
                XML_TRUE);

  if (res != XML_STATUS_OK || parser_data.failed) {
    LOG(ERROR) << "Omaha response not valid XML: "
               << XML_ErrorString(XML_GetErrorCode(parser)) << " at line "
               << XML_GetCurrentLineNumber(parser) << " col "
               << XML_GetCurrentColumnNumber(parser);
    XML_ParserFree(parser);
    ErrorCode error_code = ErrorCode::kOmahaRequestXMLParseError;
    if (response_buffer_.empty()) {
      error_code = ErrorCode::kOmahaRequestEmptyResponseError;
    } else if (parser_data.entity_decl) {
      error_code = ErrorCode::kOmahaRequestXMLHasEntityDecl;
    }
    completer.set_code(error_code);
    return;
  }
  XML_ParserFree(parser);

  // Update the last ping day preferences based on the server daystart response
  // even if we didn't send a ping. Omaha always includes the daystart in the
  // response, but log the error if it didn't.
  LOG_IF(ERROR, !UpdateLastPingDays(&parser_data, system_state_->prefs()))
      << "Failed to update the last ping day preferences!";

  // Sets first_active_omaha_ping_sent to true (vpd in CrOS). We only do this if
  // we have got a response from omaha and if its value has never been set to
  // true before. Failure of this function should be ignored. There should be no
  // need to check if a=-1 has been sent because older devices have already sent
  // their a=-1 in the past and we have to set first_active_omaha_ping_sent for
  // future checks.
  if (!system_state_->hardware()->GetFirstActiveOmahaPingSent()) {
    if (!system_state_->hardware()->SetFirstActiveOmahaPingSent()) {
      system_state_->metrics_reporter()->ReportInternalErrorCode(
          ErrorCode::kFirstActiveOmahaPingSentPersistenceError);
    }
  }

  if (!HasOutputPipe()) {
    // Just set success to whether or not the http transfer succeeded,
    // which must be true at this point in the code.
    completer.set_code(ErrorCode::kSuccess);
    return;
  }

  OmahaResponse output_object;
  if (!ParseResponse(&parser_data, &output_object, &completer))
    return;
  output_object.update_exists = true;
  SetOutputObject(output_object);

  LoadOrPersistUpdateFirstSeenAtPref();

  ErrorCode error = ErrorCode::kSuccess;
  if (ShouldIgnoreUpdate(output_object, &error)) {
    // No need to change output_object.update_exists here, since the value
    // has been output to the pipe.
    completer.set_code(error);
    return;
  }

  // If Omaha says to disable p2p, respect that
  if (output_object.disable_p2p_for_downloading) {
    LOG(INFO) << "Forcibly disabling use of p2p for downloading as "
              << "requested by Omaha.";
    payload_state->SetUsingP2PForDownloading(false);
  }
  if (output_object.disable_p2p_for_sharing) {
    LOG(INFO) << "Forcibly disabling use of p2p for sharing as "
              << "requested by Omaha.";
    payload_state->SetUsingP2PForSharing(false);
  }

  // Update the payload state with the current response. The payload state
  // will automatically reset all stale state if this response is different
  // from what's stored already. We are updating the payload state as late
  // as possible in this method so that if a new release gets pushed and then
  // got pulled back due to some issues, we don't want to clear our internal
  // state unnecessarily.
  payload_state->SetResponse(output_object);

  // It could be we've already exceeded the deadline for when p2p is
  // allowed or that we've tried too many times with p2p. Check that.
  if (payload_state->GetUsingP2PForDownloading()) {
    payload_state->P2PNewAttempt();
    if (!payload_state->P2PAttemptAllowed()) {
      LOG(INFO) << "Forcibly disabling use of p2p for downloading because "
                << "of previous failures when using p2p.";
      payload_state->SetUsingP2PForDownloading(false);
    }
  }

  // From here on, we'll complete stuff in CompleteProcessing() so
  // disable |completer| since we'll create a new one in that
  // function.
  completer.set_should_complete(false);

  // If we're allowed to use p2p for downloading we do not pay
  // attention to wall-clock-based waiting if the URL is indeed
  // available via p2p. Therefore, check if the file is available via
  // p2p before deferring...
  if (payload_state->GetUsingP2PForDownloading()) {
    LookupPayloadViaP2P(output_object);
  } else {
    CompleteProcessing();
  }
}

void OmahaRequestAction::CompleteProcessing() {
  ScopedActionCompleter completer(processor_, this);
  OmahaResponse& output_object = const_cast<OmahaResponse&>(GetOutputObject());
  PayloadStateInterface* payload_state = system_state_->payload_state();

  if (ShouldDeferDownload(&output_object)) {
    output_object.update_exists = false;
    LOG(INFO) << "Ignoring Omaha updates as updates are deferred by policy.";
    completer.set_code(ErrorCode::kOmahaUpdateDeferredPerPolicy);
    return;
  }

  if (payload_state->ShouldBackoffDownload()) {
    output_object.update_exists = false;
    LOG(INFO) << "Ignoring Omaha updates in order to backoff our retry "
              << "attempts";
    completer.set_code(ErrorCode::kOmahaUpdateDeferredForBackoff);
    return;
  }
  completer.set_code(ErrorCode::kSuccess);
}

void OmahaRequestAction::OnLookupPayloadViaP2PCompleted(const string& url) {
  LOG(INFO) << "Lookup complete, p2p-client returned URL '" << url << "'";
  if (!url.empty()) {
    system_state_->payload_state()->SetP2PUrl(url);
  } else {
    LOG(INFO) << "Forcibly disabling use of p2p for downloading "
              << "because no suitable peer could be found.";
    system_state_->payload_state()->SetUsingP2PForDownloading(false);
  }
  CompleteProcessing();
}

void OmahaRequestAction::LookupPayloadViaP2P(const OmahaResponse& response) {
  // If the device is in the middle of an update, the state variables
  // kPrefsUpdateStateNextDataOffset, kPrefsUpdateStateNextDataLength
  // tracks the offset and length of the operation currently in
  // progress. The offset is based from the end of the manifest which
  // is kPrefsManifestMetadataSize bytes long.
  //
  // To make forward progress and avoid deadlocks, we need to find a
  // peer that has at least the entire operation we're currently
  // working on. Otherwise we may end up in a situation where two
  // devices bounce back and forth downloading from each other,
  // neither making any forward progress until one of them decides to
  // stop using p2p (via kMaxP2PAttempts and kMaxP2PAttemptTimeSeconds
  // safe-guards). See http://crbug.com/297170 for an example)
  size_t minimum_size = 0;
  int64_t manifest_metadata_size = 0;
  int64_t manifest_signature_size = 0;
  int64_t next_data_offset = 0;
  int64_t next_data_length = 0;
  if (system_state_ &&
      system_state_->prefs()->GetInt64(kPrefsManifestMetadataSize,
                                       &manifest_metadata_size) &&
      manifest_metadata_size != -1 &&
      system_state_->prefs()->GetInt64(kPrefsManifestSignatureSize,
                                       &manifest_signature_size) &&
      manifest_signature_size != -1 &&
      system_state_->prefs()->GetInt64(kPrefsUpdateStateNextDataOffset,
                                       &next_data_offset) &&
      next_data_offset != -1 &&
      system_state_->prefs()->GetInt64(kPrefsUpdateStateNextDataLength,
                                       &next_data_length)) {
    minimum_size = manifest_metadata_size + manifest_signature_size +
                   next_data_offset + next_data_length;
  }

  // TODO(senj): Fix P2P for multiple package.
  brillo::Blob raw_hash;
  if (!base::HexStringToBytes(response.packages[0].hash, &raw_hash))
    return;
  string file_id =
      utils::CalculateP2PFileId(raw_hash, response.packages[0].size);
  if (system_state_->p2p_manager()) {
    LOG(INFO) << "Checking if payload is available via p2p, file_id=" << file_id
              << " minimum_size=" << minimum_size;
    system_state_->p2p_manager()->LookupUrlForFile(
        file_id,
        minimum_size,
        TimeDelta::FromSeconds(kMaxP2PNetworkWaitTimeSeconds),
        base::Bind(&OmahaRequestAction::OnLookupPayloadViaP2PCompleted,
                   base::Unretained(this)));
  }
}

bool OmahaRequestAction::ShouldDeferDownload(OmahaResponse* output_object) {
  if (params_->interactive()) {
    LOG(INFO) << "Not deferring download because update is interactive.";
    return false;
  }

  // If we're using p2p to download _and_ we have a p2p URL, we never
  // defer the download. This is because the download will always
  // happen from a peer on the LAN and we've been waiting in line for
  // our turn.
  const PayloadStateInterface* payload_state = system_state_->payload_state();
  if (payload_state->GetUsingP2PForDownloading() &&
      !payload_state->GetP2PUrl().empty()) {
    LOG(INFO) << "Download not deferred because download "
              << "will happen from a local peer (via p2p).";
    return false;
  }

  // We should defer the downloads only if we've first satisfied the
  // wall-clock-based-waiting period and then the update-check-based waiting
  // period, if required.
  if (!params_->wall_clock_based_wait_enabled()) {
    LOG(INFO) << "Wall-clock-based waiting period is not enabled,"
              << " so no deferring needed.";
    return false;
  }

  switch (IsWallClockBasedWaitingSatisfied(output_object)) {
    case kWallClockWaitNotSatisfied:
      // We haven't even satisfied the first condition, passing the
      // wall-clock-based waiting period, so we should defer the downloads
      // until that happens.
      LOG(INFO) << "wall-clock-based-wait not satisfied.";
      return true;

    case kWallClockWaitDoneButUpdateCheckWaitRequired:
      LOG(INFO) << "wall-clock-based-wait satisfied and "
                << "update-check-based-wait required.";
      return !IsUpdateCheckCountBasedWaitingSatisfied();

    case kWallClockWaitDoneAndUpdateCheckWaitNotRequired:
      // Wall-clock-based waiting period is satisfied, and it's determined
      // that we do not need the update-check-based wait. so no need to
      // defer downloads.
      LOG(INFO) << "wall-clock-based-wait satisfied and "
                << "update-check-based-wait is not required.";
      return false;

    default:
      // Returning false for this default case so we err on the
      // side of downloading updates than deferring in case of any bugs.
      NOTREACHED();
      return false;
  }
}

OmahaRequestAction::WallClockWaitResult
OmahaRequestAction::IsWallClockBasedWaitingSatisfied(
    OmahaResponse* output_object) {
  Time update_first_seen_at = LoadOrPersistUpdateFirstSeenAtPref();
  if (update_first_seen_at == base::Time()) {
    LOG(INFO) << "Not scattering as UpdateFirstSeenAt value cannot be read or "
                 "persisted";
    return kWallClockWaitDoneAndUpdateCheckWaitNotRequired;
  }

  TimeDelta elapsed_time =
      system_state_->clock()->GetWallclockTime() - update_first_seen_at;
  TimeDelta max_scatter_period =
      TimeDelta::FromDays(output_object->max_days_to_scatter);
  int64_t staging_wait_time_in_days = 0;
  // Use staging and its default max value if staging is on.
  if (system_state_->prefs()->GetInt64(kPrefsWallClockStagingWaitPeriod,
                                       &staging_wait_time_in_days) &&
      staging_wait_time_in_days > 0)
    max_scatter_period = TimeDelta::FromDays(kMaxWaitTimeStagingInDays);

  LOG(INFO) << "Waiting Period = "
            << utils::FormatSecs(params_->waiting_period().InSeconds())
            << ", Time Elapsed = "
            << utils::FormatSecs(elapsed_time.InSeconds())
            << ", MaxDaysToScatter = " << max_scatter_period.InDays();

  if (!output_object->deadline.empty()) {
    // The deadline is set for all rules which serve a delta update from a
    // previous FSI, which means this update will be applied mostly in OOBE
    // cases. For these cases, we shouldn't scatter so as to finish the OOBE
    // quickly.
    LOG(INFO) << "Not scattering as deadline flag is set";
    return kWallClockWaitDoneAndUpdateCheckWaitNotRequired;
  }

  if (max_scatter_period.InDays() == 0) {
    // This means the Omaha rule creator decides that this rule
    // should not be scattered irrespective of the policy.
    LOG(INFO) << "Not scattering as MaxDaysToScatter in rule is 0.";
    return kWallClockWaitDoneAndUpdateCheckWaitNotRequired;
  }

  if (elapsed_time > max_scatter_period) {
    // This means we've waited more than the upperbound wait in the rule
    // from the time we first saw a valid update available to us.
    // This will prevent update starvation.
    LOG(INFO) << "Not scattering as we're past the MaxDaysToScatter limit.";
    return kWallClockWaitDoneAndUpdateCheckWaitNotRequired;
  }

  // This means we are required to participate in scattering.
  // See if our turn has arrived now.
  TimeDelta remaining_wait_time = params_->waiting_period() - elapsed_time;
  if (remaining_wait_time.InSeconds() <= 0) {
    // Yes, it's our turn now.
    LOG(INFO) << "Successfully passed the wall-clock-based-wait.";

    // But we can't download until the update-check-count-based wait is also
    // satisfied, so mark it as required now if update checks are enabled.
    return params_->update_check_count_wait_enabled()
               ? kWallClockWaitDoneButUpdateCheckWaitRequired
               : kWallClockWaitDoneAndUpdateCheckWaitNotRequired;
  }

  // Not our turn yet, so we have to wait until our turn to
  // help scatter the downloads across all clients of the enterprise.
  LOG(INFO) << "Update deferred for another "
            << utils::FormatSecs(remaining_wait_time.InSeconds())
            << " per policy.";
  return kWallClockWaitNotSatisfied;
}

bool OmahaRequestAction::IsUpdateCheckCountBasedWaitingSatisfied() {
  int64_t update_check_count_value;

  if (system_state_->prefs()->Exists(kPrefsUpdateCheckCount)) {
    if (!system_state_->prefs()->GetInt64(kPrefsUpdateCheckCount,
                                          &update_check_count_value)) {
      // We are unable to read the update check count from file for some reason.
      // So let's proceed anyway so as to not stall the update.
      LOG(ERROR) << "Unable to read update check count. "
                 << "Skipping update-check-count-based-wait.";
      return true;
    }
  } else {
    // This file does not exist. This means we haven't started our update
    // check count down yet, so this is the right time to start the count down.
    update_check_count_value =
        base::RandInt(params_->min_update_checks_needed(),
                      params_->max_update_checks_allowed());

    LOG(INFO) << "Randomly picked update check count value = "
              << update_check_count_value;

    // Write out the initial value of update_check_count_value.
    if (!system_state_->prefs()->SetInt64(kPrefsUpdateCheckCount,
                                          update_check_count_value)) {
      // We weren't able to write the update check count file for some reason.
      // So let's proceed anyway so as to not stall the update.
      LOG(ERROR) << "Unable to write update check count. "
                 << "Skipping update-check-count-based-wait.";
      return true;
    }
  }

  if (update_check_count_value == 0) {
    LOG(INFO) << "Successfully passed the update-check-based-wait.";
    return true;
  }

  if (update_check_count_value < 0 ||
      update_check_count_value > params_->max_update_checks_allowed()) {
    // We err on the side of skipping scattering logic instead of stalling
    // a machine from receiving any updates in case of any unexpected state.
    LOG(ERROR) << "Invalid value for update check count detected. "
               << "Skipping update-check-count-based-wait.";
    return true;
  }

  // Legal value, we need to wait for more update checks to happen
  // until this becomes 0.
  LOG(INFO) << "Deferring Omaha updates for another "
            << update_check_count_value << " update checks per policy";
  return false;
}

// static
bool OmahaRequestAction::ParseInstallDate(OmahaParserData* parser_data,
                                          OmahaResponse* output_object) {
  int64_t elapsed_days = 0;
  if (!base::StringToInt64(parser_data->daystart_elapsed_days, &elapsed_days))
    return false;

  if (elapsed_days < 0)
    return false;

  output_object->install_date_days = elapsed_days;
  return true;
}

// static
bool OmahaRequestAction::HasInstallDate(SystemState* system_state) {
  PrefsInterface* prefs = system_state->prefs();
  if (prefs == nullptr)
    return false;

  return prefs->Exists(kPrefsInstallDateDays);
}

// static
bool OmahaRequestAction::PersistInstallDate(
    SystemState* system_state,
    int install_date_days,
    InstallDateProvisioningSource source) {
  TEST_AND_RETURN_FALSE(install_date_days >= 0);

  PrefsInterface* prefs = system_state->prefs();
  if (prefs == nullptr)
    return false;

  if (!prefs->SetInt64(kPrefsInstallDateDays, install_date_days))
    return false;

  system_state->metrics_reporter()->ReportInstallDateProvisioningSource(
      static_cast<int>(source),  // Sample.
      kProvisionedMax);          // Maximum.
  return true;
}

bool OmahaRequestAction::PersistCohortData(const string& prefs_key,
                                           const string& new_value) {
  if (new_value.empty() && system_state_->prefs()->Exists(prefs_key)) {
    LOG(INFO) << "Removing stored " << prefs_key << " value.";
    return system_state_->prefs()->Delete(prefs_key);
  } else if (!new_value.empty()) {
    LOG(INFO) << "Storing new setting " << prefs_key << " as " << new_value;
    return system_state_->prefs()->SetString(prefs_key, new_value);
  }
  return true;
}

bool OmahaRequestAction::PersistEolStatus(const map<string, string>& attrs) {
  auto eol_attr = attrs.find(kAttrEol);
  if (eol_attr != attrs.end()) {
    return system_state_->prefs()->SetString(kPrefsOmahaEolStatus,
                                             eol_attr->second);
  } else if (system_state_->prefs()->Exists(kPrefsOmahaEolStatus)) {
    return system_state_->prefs()->Delete(kPrefsOmahaEolStatus);
  }
  return true;
}

void OmahaRequestAction::ActionCompleted(ErrorCode code) {
  // We only want to report this on "update check".
  if (ping_only_ || event_ != nullptr)
    return;

  metrics::CheckResult result = metrics::CheckResult::kUnset;
  metrics::CheckReaction reaction = metrics::CheckReaction::kUnset;
  metrics::DownloadErrorCode download_error_code =
      metrics::DownloadErrorCode::kUnset;

  // Regular update attempt.
  switch (code) {
    case ErrorCode::kSuccess:
      // OK, we parsed the response successfully but that does
      // necessarily mean that an update is available.
      if (HasOutputPipe()) {
        const OmahaResponse& response = GetOutputObject();
        if (response.update_exists) {
          result = metrics::CheckResult::kUpdateAvailable;
          reaction = metrics::CheckReaction::kUpdating;
        } else {
          result = metrics::CheckResult::kNoUpdateAvailable;
        }
      } else {
        result = metrics::CheckResult::kNoUpdateAvailable;
      }
      break;

    case ErrorCode::kOmahaUpdateIgnoredPerPolicy:
    case ErrorCode::kOmahaUpdateIgnoredOverCellular:
      result = metrics::CheckResult::kUpdateAvailable;
      reaction = metrics::CheckReaction::kIgnored;
      break;

    case ErrorCode::kOmahaUpdateDeferredPerPolicy:
      result = metrics::CheckResult::kUpdateAvailable;
      reaction = metrics::CheckReaction::kDeferring;
      break;

    case ErrorCode::kOmahaUpdateDeferredForBackoff:
      result = metrics::CheckResult::kUpdateAvailable;
      reaction = metrics::CheckReaction::kBackingOff;
      break;

    default:
      // We report two flavors of errors, "Download errors" and "Parsing
      // error". Try to convert to the former and if that doesn't work
      // we know it's the latter.
      metrics::DownloadErrorCode tmp_error =
          metrics_utils::GetDownloadErrorCode(code);
      if (tmp_error != metrics::DownloadErrorCode::kInputMalformed) {
        result = metrics::CheckResult::kDownloadError;
        download_error_code = tmp_error;
      } else {
        result = metrics::CheckResult::kParsingError;
      }
      break;
  }

  system_state_->metrics_reporter()->ReportUpdateCheckMetrics(
      system_state_, result, reaction, download_error_code);
}

bool OmahaRequestAction::ShouldIgnoreUpdate(const OmahaResponse& response,
                                            ErrorCode* error) const {
  // Note: policy decision to not update to a version we rolled back from.
  string rollback_version =
      system_state_->payload_state()->GetRollbackVersion();
  if (!rollback_version.empty()) {
    LOG(INFO) << "Detected previous rollback from version " << rollback_version;
    if (rollback_version == response.version) {
      LOG(INFO) << "Received version that we rolled back from. Ignoring.";
      *error = ErrorCode::kOmahaUpdateIgnoredPerPolicy;
      return true;
    }
  }

  if (system_state_->hardware()->IsOOBEEnabled() &&
      !system_state_->hardware()->IsOOBEComplete(nullptr) &&
      (response.deadline.empty() ||
       system_state_->payload_state()->GetRollbackHappened()) &&
      params_->app_version() != "ForcedUpdate") {
    LOG(INFO) << "Ignoring a non-critical Omaha update before OOBE completion.";
    *error = ErrorCode::kNonCriticalUpdateInOOBE;
    return true;
  }

  if (!IsUpdateAllowedOverCurrentConnection(error, response)) {
    LOG(INFO) << "Update is not allowed over current connection.";
    return true;
  }

  // Note: We could technically delete the UpdateFirstSeenAt state when we
  // return true. If we do, it'll mean a device has to restart the
  // UpdateFirstSeenAt and thus help scattering take effect when the AU is
  // turned on again. On the other hand, it also increases the chance of update
  // starvation if an admin turns AU on/off more frequently. We choose to err on
  // the side of preventing starvation at the cost of not applying scattering in
  // those cases.
  return false;
}

bool OmahaRequestAction::IsUpdateAllowedOverCellularByPrefs(
    const OmahaResponse& response) const {
  PrefsInterface* prefs = system_state_->prefs();

  if (!prefs) {
    LOG(INFO) << "Disabling updates over cellular as the preferences are "
                 "not available.";
    return false;
  }

  bool is_allowed;

  if (prefs->Exists(kPrefsUpdateOverCellularPermission) &&
      prefs->GetBoolean(kPrefsUpdateOverCellularPermission, &is_allowed) &&
      is_allowed) {
    LOG(INFO) << "Allowing updates over cellular as permission preference is "
                 "set to true.";
    return true;
  }

  if (!prefs->Exists(kPrefsUpdateOverCellularTargetVersion) ||
      !prefs->Exists(kPrefsUpdateOverCellularTargetSize)) {
    LOG(INFO) << "Disabling updates over cellular as permission preference is "
                 "set to false or does not exist while target does not exist.";
    return false;
  }

  std::string target_version;
  int64_t target_size;

  if (!prefs->GetString(kPrefsUpdateOverCellularTargetVersion,
                        &target_version) ||
      !prefs->GetInt64(kPrefsUpdateOverCellularTargetSize, &target_size)) {
    LOG(INFO) << "Disabling updates over cellular as the target version or "
                 "size is not accessible.";
    return false;
  }

  uint64_t total_packages_size = 0;
  for (const auto& package : response.packages) {
    total_packages_size += package.size;
  }
  if (target_version == response.version &&
      static_cast<uint64_t>(target_size) == total_packages_size) {
    LOG(INFO) << "Allowing updates over cellular as the target matches the"
                 "omaha response.";
    return true;
  } else {
    LOG(INFO) << "Disabling updates over cellular as the target does not"
                 "match the omaha response.";
    return false;
  }
}

bool OmahaRequestAction::IsUpdateAllowedOverCurrentConnection(
    ErrorCode* error, const OmahaResponse& response) const {
  ConnectionType type;
  ConnectionTethering tethering;
  ConnectionManagerInterface* connection_manager =
      system_state_->connection_manager();
  if (!connection_manager->GetConnectionProperties(&type, &tethering)) {
    LOG(INFO) << "We could not determine our connection type. "
              << "Defaulting to allow updates.";
    return true;
  }

  bool is_allowed = connection_manager->IsUpdateAllowedOver(type, tethering);
  bool is_device_policy_set =
      connection_manager->IsAllowedConnectionTypesForUpdateSet();
  // Treats tethered connection as if it is cellular connection.
  bool is_over_cellular = type == ConnectionType::kCellular ||
                          tethering == ConnectionTethering::kConfirmed;

  if (!is_over_cellular) {
    // There's no need to further check user preferences as we are not over
    // cellular connection.
    if (!is_allowed)
      *error = ErrorCode::kOmahaUpdateIgnoredPerPolicy;
  } else if (is_device_policy_set) {
    // There's no need to further check user preferences as the device policy
    // is set regarding updates over cellular.
    if (!is_allowed)
      *error = ErrorCode::kOmahaUpdateIgnoredPerPolicy;
  } else {
    // Deivce policy is not set, so user preferences overwrite whether to
    // allow updates over cellular.
    is_allowed = IsUpdateAllowedOverCellularByPrefs(response);
    if (!is_allowed)
      *error = ErrorCode::kOmahaUpdateIgnoredOverCellular;
  }

  LOG(INFO) << "We are connected via "
            << connection_utils::StringForConnectionType(type)
            << ", Updates allowed: " << (is_allowed ? "Yes" : "No");
  return is_allowed;
}

bool OmahaRequestAction::IsRollbackEnabled() const {
  if (policy_provider_->IsConsumerDevice()) {
    LOG(INFO) << "Rollback is not enabled for consumer devices.";
    return false;
  }

  if (!policy_provider_->device_policy_is_loaded()) {
    LOG(INFO) << "No device policy is loaded. Assuming rollback enabled.";
    return true;
  }

  int allowed_milestones;
  if (!policy_provider_->GetDevicePolicy().GetRollbackAllowedMilestones(
          &allowed_milestones)) {
    LOG(INFO) << "RollbackAllowedMilestones policy can't be read. "
                 "Defaulting to rollback enabled.";
    return true;
  }

  LOG(INFO) << "Rollback allows " << allowed_milestones << " milestones.";
  return allowed_milestones > 0;
}

void OmahaRequestAction::SetMaxKernelKeyVersionForRollback() const {
  int max_kernel_rollforward;
  int min_kernel_version = system_state_->hardware()->GetMinKernelKeyVersion();
  if (IsRollbackEnabled()) {
    // If rollback is enabled, set the max kernel key version to the current
    // kernel key version. This has the effect of freezing kernel key roll
    // forwards.
    //
    // TODO(zentaro): This behavior is temporary, and ensures that no kernel
    // key roll forward happens until the server side components of rollback
    // are implemented. Future changes will allow the Omaha server to return
    // the kernel key version from max_rollback_versions in the past. At that
    // point the max kernel key version will be set to that value, creating a
    // sliding window of versions that can be rolled back to.
    LOG(INFO) << "Rollback is enabled. Setting kernel_max_rollforward to "
              << min_kernel_version;
    max_kernel_rollforward = min_kernel_version;
  } else {
    // For devices that are not rollback enabled (ie. consumer devices), the
    // max kernel key version is set to 0xfffffffe, which is logically
    // infinity. This maintains the previous behavior that that kernel key
    // versions roll forward each time they are incremented.
    LOG(INFO) << "Rollback is disabled. Setting kernel_max_rollforward to "
              << kRollforwardInfinity;
    max_kernel_rollforward = kRollforwardInfinity;
  }

  bool max_rollforward_set =
      system_state_->hardware()->SetMaxKernelKeyRollforward(
          max_kernel_rollforward);
  if (!max_rollforward_set) {
    LOG(ERROR) << "Failed to set kernel_max_rollforward";
  }
  // Report metrics
  system_state_->metrics_reporter()->ReportKeyVersionMetrics(
      min_kernel_version, max_kernel_rollforward, max_rollforward_set);
}

base::Time OmahaRequestAction::LoadOrPersistUpdateFirstSeenAtPref() const {
  Time update_first_seen_at;
  int64_t update_first_seen_at_int;
  if (system_state_->prefs()->Exists(kPrefsUpdateFirstSeenAt)) {
    if (system_state_->prefs()->GetInt64(kPrefsUpdateFirstSeenAt,
                                         &update_first_seen_at_int)) {
      // Note: This timestamp could be that of ANY update we saw in the past
      // (not necessarily this particular update we're considering to apply)
      // but never got to apply because of some reason (e.g. stop AU policy,
      // updates being pulled out from Omaha, changes in target version prefix,
      // new update being rolled out, etc.). But for the purposes of scattering
      // it doesn't matter which update the timestamp corresponds to. i.e.
      // the clock starts ticking the first time we see an update and we're
      // ready to apply when the random wait period is satisfied relative to
      // that first seen timestamp.
      update_first_seen_at = Time::FromInternalValue(update_first_seen_at_int);
      LOG(INFO) << "Using persisted value of UpdateFirstSeenAt: "
                << utils::ToString(update_first_seen_at);
    } else {
      // This seems like an unexpected error where the persisted value exists
      // but it's not readable for some reason.
      LOG(INFO) << "UpdateFirstSeenAt value cannot be read";
      return base::Time();
    }
  } else {
    update_first_seen_at = system_state_->clock()->GetWallclockTime();
    update_first_seen_at_int = update_first_seen_at.ToInternalValue();
    if (system_state_->prefs()->SetInt64(kPrefsUpdateFirstSeenAt,
                                         update_first_seen_at_int)) {
      LOG(INFO) << "Persisted the new value for UpdateFirstSeenAt: "
                << utils::ToString(update_first_seen_at);
    } else {
      // This seems like an unexpected error where the value cannot be
      // persisted for some reason.
      LOG(INFO) << "UpdateFirstSeenAt value "
                << utils::ToString(update_first_seen_at)
                << " cannot be persisted";
      return base::Time();
    }
  }
  return update_first_seen_at;
}

}  // namespace chromeos_update_engine
