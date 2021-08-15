/*
 * Copyright (C) 2018 The Android Open Source Project
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

#include "stack/include/bt_types.h"

/* Functions below implement cryptographic toolbox, as described in BT Spec
 * Ver 5.0 | Vol 3, Part H CRYPTOGRAPHIC TOOLBOX. Please see the spec for
 * description.
 *
 * Example of usage is avaliable in cryptographic_toolbox_test.cc */

extern Octet16 smp_calculate_f4(uint8_t* u, uint8_t* v, const Octet16& x,
                                uint8_t z);
extern uint32_t smp_calculate_g2(uint8_t* u, uint8_t* v, const Octet16& x,
                                 const Octet16& y);
extern void smp_calculate_f5(uint8_t* w, const Octet16& n1, const Octet16& n2,
                             uint8_t* a1, uint8_t* a2, Octet16* mac_key,
                             Octet16* ltk);
extern Octet16 smp_calculate_f6(const Octet16& w, const Octet16& n1,
                                const Octet16& n2, const Octet16& r,
                                uint8_t* iocap, uint8_t* a1, uint8_t* a2);
extern Octet16 smp_calculate_h6(const Octet16& w, std::array<uint8_t, 4> keyid);
extern Octet16 smp_calculate_h7(const Octet16& salt, const Octet16& w);
extern Octet16 smp_calculate_ltk_to_link_key(const Octet16& ltk, bool use_h7);
extern Octet16 smp_calculate_link_key_to_ltk(const Octet16& link_key,
                                             bool use_h7);
