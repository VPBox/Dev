/*
 * Copyright 2018 The Android Open Source Project
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

#pragma once

#include <map>
#include <utility>

#include <base/logging.h>

namespace bluetooth {

namespace common {

/**
 * State machine used by Bluetooth native stack.
 */
class StateMachine {
 public:
  enum { kStateInvalid = -1 };

  /**
   * A class to represent the state in the State Machine.
   */
  class State {
    friend class StateMachine;

   public:
    /**
     * Constructor.
     *
     * @param sm the State Machine to use
     * @param state_id the unique State ID. It should be a non-negative number.
     */
    State(StateMachine& sm, int state_id) : sm_(sm), state_id_(state_id) {}

    virtual ~State() = default;

    /**
     * Process an event.
     * TODO: The arguments are wrong - used for backward compatibility.
     * Will be replaced later.
     *
     * @param event the event type
     * @param p_data the event data
     * @return true if the processing was completed, otherwise false
     */
    virtual bool ProcessEvent(uint32_t event, void* p_data) = 0;

    /**
     * Get the State ID.
     *
     * @return the State ID
     */
    int StateId() const { return state_id_; }

   protected:
    /**
     * Called when a state is entered.
     */
    virtual void OnEnter() {}

    /**
     * Called when a state is exited.
     */
    virtual void OnExit() {}

    /**
     * Transition the State Machine to a new state.
     *
     * @param dest_state_id the state ID to transition to. It must be one
     * of the unique state IDs when the corresponding state was created.
     */
    void TransitionTo(int dest_state_id) { sm_.TransitionTo(dest_state_id); }

    /**
     * Transition the State Machine to a new state.
     *
     * @param dest_state the state to transition to. It cannot be nullptr.
     */
    void TransitionTo(StateMachine::State* dest_state) {
      sm_.TransitionTo(dest_state);
    }

   private:
    StateMachine& sm_;
    int state_id_;
  };

  StateMachine()
      : initial_state_(nullptr),
        previous_state_(nullptr),
        current_state_(nullptr) {}
  ~StateMachine() {
    for (auto& kv : states_) delete kv.second;
  }

  /**
   * Start the State Machine operation.
   */
  void Start() { TransitionTo(initial_state_); }

  /**
   * Quit the State Machine operation.
   */
  void Quit() { previous_state_ = current_state_ = nullptr; }

  /**
   * Get the current State ID.
   *
   * @return the current State ID
   */
  int StateId() const {
    if (current_state_ != nullptr) {
      return current_state_->StateId();
    }
    return kStateInvalid;
  }

  /**
   * Get the previous current State ID.
   *
   * @return the previous State ID
   */
  int PreviousStateId() const {
    if (previous_state_ != nullptr) {
      return previous_state_->StateId();
    }
    return kStateInvalid;
  }

  /**
   * Process an event.
   * TODO: The arguments are wrong - used for backward compatibility.
   * Will be replaced later.
   *
   * @param event the event type
   * @param p_data the event data
   * @return true if the processing was completed, otherwise false
   */
  bool ProcessEvent(uint32_t event, void* p_data) {
    if (current_state_ == nullptr) return false;
    return current_state_->ProcessEvent(event, p_data);
  }

  /**
   * Transition the State Machine to a new state.
   *
   * @param dest_state_id the state ID to transition to. It must be one
   * of the unique state IDs when the corresponding state was created.
   */
  void TransitionTo(int dest_state_id) {
    auto it = states_.find(dest_state_id);

    CHECK(it != states_.end()) << "Unknown State ID: " << dest_state_id;
    State* dest_state = it->second;
    TransitionTo(dest_state);
  }

  /**
   * Transition the State Machine to a new state.
   *
   * @param dest_state the state to transition to. It cannot be nullptr.
   */
  void TransitionTo(StateMachine::State* dest_state) {
    if (current_state_ != nullptr) {
      current_state_->OnExit();
    }
    previous_state_ = current_state_;
    current_state_ = dest_state;
    current_state_->OnEnter();
  }

  /**
   * Add a state to the State Machine.
   * The state machine takes ownership on the state - i.e., the state will
   * be deleted by the State Machine itself.
   *
   * @param state the state to add
   */
  void AddState(State* state) {
    states_.insert(std::make_pair(state->StateId(), state));
  }

  /**
   * Set the initial state of the State Machine.
   *
   * @param initial_state the initial state
   */
  void SetInitialState(State* initial_state) { initial_state_ = initial_state; }

 private:
  State* initial_state_;
  State* previous_state_;
  State* current_state_;
  std::map<int, State*> states_;
};

}  // namespace common

}  // namespace bluetooth
