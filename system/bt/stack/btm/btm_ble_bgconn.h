/******************************************************************************
 *
 *  Copyright 2018 The Android Open Source Project
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

#include "types/raw_address.h"

/** Adds the device into white list. Returns false if white list is full and
 * device can't be added, true otherwise. */
extern bool BTM_WhiteListAdd(const RawAddress& address);

/** Removes the device from white list */
extern void BTM_WhiteListRemove(const RawAddress& address);

/** Clear the whitelist, end any pending whitelist connections */
extern void BTM_WhiteListClear();

/* Use fast scan window/interval for LE connection establishment.
 * This does not send any requests to controller, instead it changes the
 * parameters that will be used after next add/remove request.
 * Returns true, if the change is scheduled, false otherwise. */
extern bool BTM_SetLeConnectionModeToFast();

/* Use slow scan window/interval for LE connection establishment.
 * This does not send any requests to controller, instead it changes the
 * parameters that will be used after next add/remove request */
extern void BTM_SetLeConnectionModeToSlow();