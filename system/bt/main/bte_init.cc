/******************************************************************************
 *
 *  Copyright 2000-2012 Broadcom Corporation
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

/******************************************************************************
 *
 *  This module contains the routines that initialize the stack components.
 *  It must be called before the BTU task is started.
 *
 ******************************************************************************/

#include <string.h>
#include "bt_target.h"

#include "bte.h"

/* Include initialization functions definitions */
#include "port_api.h"

#if (BNEP_INCLUDED == TRUE)
#include "bnep_api.h"
#endif

#include "gap_api.h"

#if (PAN_INCLUDED == TRUE)
#include "pan_api.h"
#endif

#include "avrc_api.h"
#include "a2dp_api.h"

#if (HID_HOST_INCLUDED == TRUE)
#include "hidh_api.h"
#endif

#include "gatt_api.h"
#include "smp_api.h"

/*****************************************************************************
 *                          F U N C T I O N S                                *
 *****************************************************************************/

/*****************************************************************************
 *
 * Function         BTE_InitStack
 *
 * Description      Initialize control block memory for each component.
 *
 *                  Note: The core stack components must be called
 *                      before creating the BTU Task.  The rest of the
 *                      components can be initialized at a later time if desired
 *                      as long as the component's init function is called
 *                      before accessing any of its functions.
 *
 * Returns          void
 *
 *****************************************************************************/
void BTE_InitStack(void) {
  /* Initialize the optional stack components */
  RFCOMM_Init();

/**************************
 * BNEP and its profiles **
 **************************/
#if (BNEP_INCLUDED == TRUE)
  BNEP_Init();

#if (PAN_INCLUDED == TRUE)
  PAN_Init();
#endif /* PAN */
#endif /* BNEP Included */

/**************************
 * AVDT and its profiles **
 **************************/
  A2DP_Init();

  AVRC_Init();

  /***********
   * Others **
   ***********/
  GAP_Init();

#if (HID_HOST_INCLUDED == TRUE)
  HID_HostInit();
#endif
}
