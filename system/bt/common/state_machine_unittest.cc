/******************************************************************************
 *
 *  Copyright 2017 The Android Open Source Project
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#include <gtest/gtest.h>

#include "common/state_machine.h"

using bluetooth::common::StateMachine;

namespace {
static constexpr uint32_t kInvalidEvent = 0xffffffff;
static constexpr uint32_t kEventZero = 0;
static constexpr uint32_t kEventOne = 1;
static constexpr uint32_t kEventTwo = 2;

static char dataZero = 0;
static char dataOne = 1;
static char dataTwo = 2;
}  // namespace

class StateMachineImpl : public StateMachine {
 public:
  enum {
    kStateZero,
    kStateOne,
    kStateTwo,
  };

  class StateZero : public State {
   public:
    StateZero(StateMachine& sm)
        : State(sm, kStateZero),
          on_enter_(false),
          on_exit_(false),
          event_(kInvalidEvent),
          data_(nullptr) {}
    void OnEnter() override {
      on_enter_ = true;
      on_exit_ = false;
    }
    void OnExit() override {
      on_exit_ = true;
      on_enter_ = false;
    }
    bool ProcessEvent(uint32_t event, void* p_data) override {
      event_ = event;
      data_ = p_data;
      TransitionTo(kStateOne);
      return true;
    }

    bool on_enter_;
    bool on_exit_;
    uint32_t event_;
    void* data_;
  };

  class StateOne : public State {
   public:
    StateOne(StateMachine& sm)
        : State(sm, kStateOne),
          on_enter_(false),
          on_exit_(false),
          event_(kInvalidEvent),
          data_(nullptr) {}
    void OnEnter() override {
      on_enter_ = true;
      on_exit_ = false;
    }
    void OnExit() override {
      on_exit_ = true;
      on_enter_ = false;
    }
    bool ProcessEvent(uint32_t event, void* p_data) override {
      event_ = event;
      data_ = p_data;
      TransitionTo(kStateTwo);
      return true;
    }

    bool on_enter_;
    bool on_exit_;
    uint32_t event_;
    void* data_;
  };

  class StateTwo : public State {
   public:
    StateTwo(StateMachine& sm)
        : State(sm, kStateTwo),
          on_enter_(false),
          on_exit_(false),
          event_(kInvalidEvent),
          data_(nullptr) {}
    void OnEnter() override {
      on_enter_ = true;
      on_exit_ = false;
    }
    void OnExit() override {
      on_exit_ = true;
      on_enter_ = false;
    }
    bool ProcessEvent(uint32_t event, void* p_data) override {
      event_ = event;
      data_ = p_data;
      TransitionTo(kStateZero);
      return true;
    }

    bool on_enter_;
    bool on_exit_;
    uint32_t event_;
    void* data_;
  };

  StateMachineImpl() {
    state_zero_ = new StateZero(*this);
    state_one_ = new StateOne(*this);
    state_two_ = new StateTwo(*this);

    AddState(state_zero_);
    AddState(state_one_);
    AddState(state_two_);
    SetInitialState(state_zero_);
  }

  StateZero* state_zero_;
  StateOne* state_one_;
  StateTwo* state_two_;
};

class StateMachineTest : public ::testing::Test {
 protected:
  StateMachineTest() {}

  void SetUp() override { sm_.Start(); }

  void TearDown() override { sm_.Quit(); }

  StateMachineImpl sm_;
};

TEST_F(StateMachineTest, test_initial_state) {
  ASSERT_EQ(sm_.kStateZero, sm_.StateId());
  ASSERT_EQ(sm_.kStateInvalid, sm_.PreviousStateId());
}

TEST_F(StateMachineTest, test_invalid_state) {
  sm_.Quit();
  ASSERT_EQ(sm_.kStateInvalid, sm_.StateId());
  ASSERT_EQ(sm_.kStateInvalid, sm_.PreviousStateId());
  sm_.Start();
  ASSERT_EQ(sm_.kStateZero, sm_.StateId());
  ASSERT_EQ(sm_.kStateInvalid, sm_.PreviousStateId());
}

TEST_F(StateMachineTest, test_transition_to) {
  // Initial state: StateZero
  ASSERT_EQ(sm_.kStateZero, sm_.StateId());
  ASSERT_EQ(sm_.kStateInvalid, sm_.PreviousStateId());
  ASSERT_TRUE(sm_.state_zero_->on_enter_);
  ASSERT_FALSE(sm_.state_zero_->on_exit_);

  // Transition to StateOne
  ASSERT_FALSE(sm_.state_one_->on_enter_);
  ASSERT_FALSE(sm_.state_one_->on_exit_);
  sm_.TransitionTo(sm_.kStateOne);
  ASSERT_EQ(sm_.kStateOne, sm_.StateId());
  ASSERT_EQ(sm_.kStateZero, sm_.PreviousStateId());
  ASSERT_TRUE(sm_.state_zero_->on_exit_);
  ASSERT_TRUE(sm_.state_one_->on_enter_);
  ASSERT_FALSE(sm_.state_one_->on_exit_);

  // Transition to StateTwo
  ASSERT_FALSE(sm_.state_two_->on_enter_);
  ASSERT_FALSE(sm_.state_two_->on_exit_);
  sm_.TransitionTo(sm_.kStateTwo);
  ASSERT_EQ(sm_.kStateTwo, sm_.StateId());
  ASSERT_EQ(sm_.kStateOne, sm_.PreviousStateId());
  ASSERT_TRUE(sm_.state_one_->on_exit_);
  ASSERT_TRUE(sm_.state_two_->on_enter_);
  ASSERT_FALSE(sm_.state_two_->on_exit_);
}

TEST_F(StateMachineTest, test_process_event) {
  // Initial state: StateZero
  ASSERT_EQ(sm_.kStateZero, sm_.StateId());
  ASSERT_EQ(sm_.kStateInvalid, sm_.PreviousStateId());
  ASSERT_TRUE(sm_.state_zero_->on_enter_);
  ASSERT_FALSE(sm_.state_zero_->on_exit_);
  ASSERT_EQ(sm_.state_zero_->event_, kInvalidEvent);
  ASSERT_EQ(sm_.state_zero_->data_, nullptr);

  // Process an event and transition to StateOne
  ASSERT_FALSE(sm_.state_one_->on_enter_);
  ASSERT_FALSE(sm_.state_one_->on_exit_);
  ASSERT_EQ(sm_.state_one_->event_, kInvalidEvent);
  ASSERT_EQ(sm_.state_one_->data_, nullptr);
  ASSERT_TRUE(sm_.ProcessEvent(kEventZero, &dataZero));
  ASSERT_EQ(sm_.kStateOne, sm_.StateId());
  ASSERT_EQ(sm_.kStateZero, sm_.PreviousStateId());
  // Check StateZero
  ASSERT_EQ(sm_.state_zero_->event_, kEventZero);
  ASSERT_EQ(sm_.state_zero_->data_, &dataZero);
  ASSERT_TRUE(sm_.state_zero_->on_exit_);
  // Check StateOne
  ASSERT_TRUE(sm_.state_one_->on_enter_);
  ASSERT_FALSE(sm_.state_one_->on_exit_);
  ASSERT_EQ(sm_.state_one_->event_, kInvalidEvent);
  ASSERT_EQ(sm_.state_one_->data_, nullptr);

  // Process an event and transition to StateTwo
  ASSERT_FALSE(sm_.state_two_->on_enter_);
  ASSERT_FALSE(sm_.state_two_->on_exit_);
  ASSERT_EQ(sm_.state_two_->event_, kInvalidEvent);
  ASSERT_EQ(sm_.state_two_->data_, nullptr);
  ASSERT_TRUE(sm_.ProcessEvent(kEventOne, &dataOne));
  ASSERT_EQ(sm_.kStateTwo, sm_.StateId());
  ASSERT_EQ(sm_.kStateOne, sm_.PreviousStateId());
  // Check StateOne
  ASSERT_EQ(sm_.state_one_->event_, kEventOne);
  ASSERT_EQ(sm_.state_one_->data_, &dataOne);
  ASSERT_TRUE(sm_.state_one_->on_exit_);
  // Check StateTwo
  ASSERT_TRUE(sm_.state_two_->on_enter_);
  ASSERT_FALSE(sm_.state_two_->on_exit_);
  ASSERT_EQ(sm_.state_two_->event_, kInvalidEvent);
  ASSERT_EQ(sm_.state_two_->data_, nullptr);

  // Process an event and transition to StateZero
  // NOTE: StateZero was exited before and has local state
  ASSERT_FALSE(sm_.state_zero_->on_enter_);
  ASSERT_TRUE(sm_.state_zero_->on_exit_);  // NOTE: already exited before
  ASSERT_EQ(sm_.state_zero_->event_, kEventZero);  // NOTE: state from before
  ASSERT_EQ(sm_.state_zero_->data_, &dataZero);    // NOTE: state from before
  ASSERT_TRUE(sm_.ProcessEvent(kEventTwo, &dataTwo));
  ASSERT_EQ(sm_.kStateZero, sm_.StateId());
  ASSERT_EQ(sm_.kStateTwo, sm_.PreviousStateId());
  // Check StateTwo
  ASSERT_EQ(sm_.state_two_->event_, kEventTwo);
  ASSERT_EQ(sm_.state_two_->data_, &dataTwo);
  ASSERT_TRUE(sm_.state_two_->on_exit_);
  // Check StateZero
  ASSERT_TRUE(sm_.state_zero_->on_enter_);
  ASSERT_FALSE(sm_.state_zero_->on_exit_);
  ASSERT_EQ(sm_.state_zero_->event_, kEventZero);  // NOTE: state from before
  ASSERT_EQ(sm_.state_zero_->data_, &dataZero);    // NOTE: state from before
}
