/******************************************************************************
 *
 *  Copyright 2009-2012 Broadcom Corporation
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

#ifndef BTIF_SOCK_SDP_H
#define BTIF_SOCK_SDP_H

#include <bluetooth/uuid.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

static const bluetooth::Uuid UUID_OBEX_OBJECT_PUSH =
    bluetooth::Uuid::From16Bit(0x1105);
static const bluetooth::Uuid UUID_PBAP_PSE = bluetooth::Uuid::From16Bit(0x112F);
static const bluetooth::Uuid UUID_MAP_MAS = bluetooth::Uuid::From16Bit(0x1132);
static const bluetooth::Uuid UUID_SAP = bluetooth::Uuid::From16Bit(0x112D);
static const bluetooth::Uuid UUID_SPP = bluetooth::Uuid::From16Bit(0x1101);

int add_rfc_sdp_rec(const char* name, bluetooth::Uuid uuid, int scn);
void del_rfc_sdp_rec(int handle);
bool is_reserved_rfc_channel(int channel);
int get_reserved_rfc_channel(const bluetooth::Uuid& uuid);

#endif
