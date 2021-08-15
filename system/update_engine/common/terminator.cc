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

#include "update_engine/common/terminator.h"

#include <cstdlib>

namespace chromeos_update_engine {

volatile sig_atomic_t Terminator::exit_status_ = 1;  // default exit status
volatile sig_atomic_t Terminator::exit_blocked_ = 0;
volatile sig_atomic_t Terminator::exit_requested_ = 0;

void Terminator::Init() {
  exit_blocked_ = 0;
  exit_requested_ = 0;
  signal(SIGTERM, HandleSignal);
}

void Terminator::Init(int exit_status) {
  exit_status_ = exit_status;
  Init();
}

void Terminator::Exit() {
  exit(exit_status_);
}

void Terminator::HandleSignal(int signum) {
  if (exit_blocked_ == 0) {
    Exit();
  }
  exit_requested_ = 1;
}

ScopedTerminatorExitUnblocker::~ScopedTerminatorExitUnblocker() {
  Terminator::set_exit_blocked(false);
  if (Terminator::exit_requested()) {
    Terminator::Exit();
  }
}

}  // namespace chromeos_update_engine
