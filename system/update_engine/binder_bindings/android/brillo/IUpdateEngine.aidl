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

package android.brillo;

import android.brillo.IUpdateEngineStatusCallback;
import android.brillo.ParcelableUpdateEngineStatus;

interface IUpdateEngine {
  void SetUpdateAttemptFlags(in int flags);
  boolean AttemptUpdate(in String app_version, in String omaha_url, in int flags);
  void AttemptRollback(in boolean powerwash);
  boolean CanRollback();
  void ResetStatus();
  ParcelableUpdateEngineStatus GetStatus();
  void RebootIfNeeded();
  void SetChannel(in String target_channel, in boolean powewash);
  String GetChannel(in boolean get_current_channel);
  void SetCohortHint(in String cohort_hint);
  String GetCohortHint();
  void SetP2PUpdatePermission(in boolean enabled);
  boolean GetP2PUpdatePermission();
  void SetUpdateOverCellularPermission(in boolean enabled);
  void SetUpdateOverCellularTarget(in String target_version,
                                   in long target_size);
  boolean GetUpdateOverCellularPermission();
  long GetDurationSinceUpdate();
  String GetPrevVersion();
  String GetRollbackPartition();
  void RegisterStatusCallback(in IUpdateEngineStatusCallback callback);
  int GetLastAttemptError();
  int GetEolStatus();
}
