/*
**
** Copyright 2017, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#ifndef CONTEXTS_KEYMASTER0_PASSTHROUGH_CONTEXT_H_
#define CONTEXTS_KEYMASTER0_PASSTHROUGH_CONTEXT_H_

#include <hardware/keymaster0.h>

#include <keymaster/contexts/pure_soft_keymaster_context.h>
#include <keymaster/legacy_support/ec_keymaster0_key.h>
#include <keymaster/legacy_support/keymaster0_engine.h>
#include <keymaster/legacy_support/rsa_keymaster0_key.h>

namespace keymaster {

class Keymaster0PassthroughContext : public PureSoftKeymasterContext {
  public:
    explicit Keymaster0PassthroughContext(keymaster0_device_t* dev) : PureSoftKeymasterContext() {
        km0_engine_.reset(new Keymaster0Engine(dev));
        rsa_factory_.reset(new RsaKeymaster0KeyFactory(this, km0_engine_.get()));
        ec_factory_.reset(new EcdsaKeymaster0KeyFactory(this, km0_engine_.get()));
    }

  private:
    UniquePtr<Keymaster0Engine> km0_engine_;
};

}  // namespace keymaster

#endif  // CONTEXTS_KEYMASTER0_PASSTHROUGH_CONTEXT_H_
