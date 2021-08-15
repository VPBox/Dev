/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include "apexd_session.h"

#include "apexd_utils.h"
#include "status_or.h"
#include "string_log.h"

#include "session_state.pb.h"

#include <android-base/logging.h>
#include <dirent.h>
#include <sys/stat.h>

#include <filesystem>
#include <fstream>
#include <optional>

using apex::proto::SessionState;

namespace android {
namespace apex {

namespace {

static constexpr const char* kStateFileName = "state";

std::string getSessionDir(int session_id) {
  return kApexSessionsDir + "/" + std::to_string(session_id);
}

std::string getSessionStateFilePath(int session_id) {
  return getSessionDir(session_id) + "/" + kStateFileName;
}

StatusOr<std::string> createSessionDirIfNeeded(int session_id) {
  // create /data/sessions
  auto res = createDirIfNeeded(kApexSessionsDir, 0700);
  if (!res.Ok()) {
    return StatusOr<std::string>(res.ErrorMessage());
  }
  // create /data/sessions/session_id
  std::string sessionDir = getSessionDir(session_id);
  res = createDirIfNeeded(sessionDir, 0700);
  if (!res.Ok()) {
    return StatusOr<std::string>(res.ErrorMessage());
  }

  return StatusOr<std::string>(sessionDir);
}

Status deleteSessionDir(int session_id) {
  std::string session_dir = getSessionDir(session_id);
  LOG(DEBUG) << "Deleting " << session_dir;
  auto path = std::filesystem::path(session_dir);
  std::error_code error_code;
  std::filesystem::remove_all(path, error_code);
  if (error_code) {
    return Status::Fail(StringLog() << "Failed to delete " << session_dir
                                    << " : " << error_code);
  }
  return Status::Success();
}

}  // namespace

ApexSession::ApexSession(const SessionState& state) : state_(state) {}

StatusOr<ApexSession> ApexSession::CreateSession(int session_id) {
  SessionState state;
  // Create session directory
  auto sessionPath = createSessionDirIfNeeded(session_id);
  if (!sessionPath.Ok()) {
    return StatusOr<ApexSession>::MakeError(sessionPath.ErrorMessage());
  }
  state.set_id(session_id);
  ApexSession session(state);

  return StatusOr<ApexSession>(std::move(session));
}
StatusOr<ApexSession> ApexSession::GetSessionFromFile(const std::string& path) {
  SessionState state;
  std::fstream stateFile(path, std::ios::in | std::ios::binary);
  if (!stateFile) {
    return StatusOr<ApexSession>::MakeError("Failed to open " + path);
  }

  if (!state.ParseFromIstream(&stateFile)) {
    return StatusOr<ApexSession>::MakeError("Failed to parse " + path);
  }

  return StatusOr<ApexSession>(ApexSession(state));
}

StatusOr<ApexSession> ApexSession::GetSession(int session_id) {
  auto path = getSessionStateFilePath(session_id);

  return GetSessionFromFile(path);
}

std::vector<ApexSession> ApexSession::GetSessions() {
  std::vector<ApexSession> sessions;

  StatusOr<std::vector<std::string>> sessionPaths = ReadDir(
      kApexSessionsDir, [](const std::filesystem::directory_entry& entry) {
        std::error_code ec;
        return entry.is_directory(ec);
      });

  if (!sessionPaths.Ok()) {
    return sessions;
  }

  for (const std::string& sessionDirPath : *sessionPaths) {
    // Try to read session state
    auto session = GetSessionFromFile(sessionDirPath + "/" + kStateFileName);
    if (!session.Ok()) {
      LOG(WARNING) << session.ErrorMessage();
      continue;
    }
    sessions.push_back(std::move(*session));
  }

  return sessions;
}

std::vector<ApexSession> ApexSession::GetSessionsInState(
    SessionState::State state) {
  auto sessions = GetSessions();
  sessions.erase(
      std::remove_if(sessions.begin(), sessions.end(),
                     [&](const ApexSession &s) { return s.GetState() != state; }),
      sessions.end());

  return sessions;
}

StatusOr<std::optional<ApexSession>> ApexSession::GetActiveSession() {
  auto sessions = GetSessions();
  std::optional<ApexSession> ret = std::nullopt;
  for (const ApexSession& session : sessions) {
    if (!session.IsFinalized()) {
      if (ret) {
        return StatusOr<std::optional<ApexSession>>::MakeError(
            "More than one active session");
      }
      ret.emplace(session);
    }
  }
  return StatusOr<std::optional<ApexSession>>(std::move(ret));
}

SessionState::State ApexSession::GetState() const { return state_.state(); }

int ApexSession::GetId() const { return state_.id(); }

std::string ApexSession::GetBuildFingerprint() const {
  return state_.expected_build_fingerprint();
}

bool ApexSession::IsFinalized() const {
  switch (GetState()) {
    case SessionState::SUCCESS:
      [[fallthrough]];
    case SessionState::ACTIVATION_FAILED:
      [[fallthrough]];
    case SessionState::ROLLED_BACK:
      [[fallthrough]];
    case SessionState::ROLLBACK_FAILED:
      return true;
    default:
      return false;
  }
}

const google::protobuf::RepeatedField<int> ApexSession::GetChildSessionIds()
    const {
  return state_.child_session_ids();
}

void ApexSession::SetChildSessionIds(
    const std::vector<int>& child_session_ids) {
  *(state_.mutable_child_session_ids()) = {child_session_ids.begin(),
                                           child_session_ids.end()};
}

void ApexSession::SetBuildFingerprint(const std::string& fingerprint) {
  *(state_.mutable_expected_build_fingerprint()) = fingerprint;
}

Status ApexSession::UpdateStateAndCommit(
    const SessionState::State& session_state) {
  state_.set_state(session_state);

  auto stateFilePath = getSessionStateFilePath(state_.id());

  std::fstream stateFile(stateFilePath,
                         std::ios::out | std::ios::trunc | std::ios::binary);
  if (!state_.SerializeToOstream(&stateFile)) {
    return Status::Fail("Failed to write state file " + stateFilePath);
  }

  return Status::Success();
}

Status ApexSession::DeleteSession() const { return deleteSessionDir(GetId()); }

std::ostream& operator<<(std::ostream& out, const ApexSession& session) {
  return out << "[id = " << session.GetId()
             << "; state = " << SessionState::State_Name(session.GetState())
             << "]";
}

}  // namespace apex
}  // namespace android
