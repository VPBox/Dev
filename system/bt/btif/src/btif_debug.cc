/******************************************************************************
 *
 *  Copyright 2015 Google Inc.
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

#include <sys/time.h>
#include <unistd.h>

#include "btif/include/btif_debug.h"
#include "btif/include/btif_debug_btsnoop.h"
#include "internal_include/bt_target.h"

void btif_debug_init(void) {
#if (BTSNOOP_MEM == TRUE)
  btif_debug_btsnoop_init();
#endif
}
