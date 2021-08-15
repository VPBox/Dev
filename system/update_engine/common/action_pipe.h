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

#ifndef UPDATE_ENGINE_COMMON_ACTION_PIPE_H_
#define UPDATE_ENGINE_COMMON_ACTION_PIPE_H_

#include <stdio.h>

#include <map>
#include <memory>
#include <string>

#include <base/logging.h>
#include <base/macros.h>

// The structure of these classes (Action, ActionPipe, ActionProcessor, etc.)
// is based on the KSAction* classes from the Google Update Engine code at
// http://code.google.com/p/update-engine/ . The author of this file sends
// a big thanks to that team for their high quality design, implementation,
// and documentation.

// This class serves as a temporary holding area for an object passed out
// from one Action and into another Action. It's templated so that it may
// contain any type of object that an Action outputs/inputs. Actions
// cannot be bonded (i.e., connected with a pipe) if their output/input
// object types differ (a compiler error will result).
//
// An ActionPipe is generally created with the Bond() method and owned by
// the two Action objects. a shared_ptr is used so that when the last Action
// pointing to an ActionPipe dies, the ActionPipe dies, too.

namespace chromeos_update_engine {

// Used by Actions an InputObjectType or OutputObjectType to specify that
// for that type, no object is taken/given.
class NoneType {};

template <typename T>
class Action;

template <typename ObjectType>
class ActionPipe {
 public:
  virtual ~ActionPipe() {}

  // This should be called by an Action on its input pipe.
  // Returns a reference to the stored object.
  const ObjectType& contents() const { return contents_; }

  // This should be called by an Action on its output pipe.
  // Stores a copy of the passed object in this pipe.
  void set_contents(const ObjectType& contents) { contents_ = contents; }

  // Bonds two Actions together with a new ActionPipe. The ActionPipe is
  // jointly owned by the two Actions and will be automatically destroyed
  // when the last Action is destroyed.
  template <typename FromAction, typename ToAction>
  static void Bond(FromAction* from, ToAction* to) {
    std::shared_ptr<ActionPipe<ObjectType>> pipe(new ActionPipe<ObjectType>);
    from->set_out_pipe(pipe);

    to->set_in_pipe(pipe);  // If you get an error on this line, then
    // it most likely means that the From object's OutputObjectType is
    // different from the To object's InputObjectType.
  }

 private:
  ObjectType contents_;

  // The ctor is private. This is because this class should construct itself
  // via the static Bond() method.
  ActionPipe() {}
  DISALLOW_COPY_AND_ASSIGN(ActionPipe);
};

// Utility function
template <typename FromAction, typename ToAction>
void BondActions(FromAction* from, ToAction* to) {
  static_assert(
      std::is_same<typename FromAction::OutputObjectType,
                   typename ToAction::InputObjectType>::value,
      "FromAction::OutputObjectType doesn't match ToAction::InputObjectType");
  ActionPipe<typename FromAction::OutputObjectType>::Bond(from, to);
}

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_COMMON_ACTION_PIPE_H_
