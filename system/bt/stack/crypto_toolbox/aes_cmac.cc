/******************************************************************************
 *
 *  Copyright 2008-2012 Broadcom Corporation
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
 *  This file contains the implementation of the AES128 and AES CMAC algorithm.
 *
 ******************************************************************************/

#include "stack/crypto_toolbox/aes.h"
#include "stack/crypto_toolbox/crypto_toolbox.h"

#include <base/logging.h>
#include <base/strings/string_number_conversions.h>

namespace crypto_toolbox {

namespace {

typedef struct {
  uint8_t* text;
  uint16_t len;
  uint16_t round;
} tCMAC_CB;

tCMAC_CB cmac_cb;

/* Rb for AES-128 as block cipher, LSB as [0] */
Octet16 const_Rb{0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/** utility function to do an biteise exclusive-OR of two bit strings of the
 * length of OCTET16_LEN. Result is stored in first argument.
 */
static void xor_128(Octet16* a, const Octet16& b) {
  CHECK(a);
  uint8_t i, *aa = a->data();
  const uint8_t* bb = b.data();

  for (i = 0; i < OCTET16_LEN; i++) {
    aa[i] = aa[i] ^ bb[i];
  }
}
}  // namespace

/* This function computes AES_128(key, message) */
Octet16 aes_128(const Octet16& key, const Octet16& message) {
  Octet16 key_reversed;
  Octet16 message_reversed;
  Octet16 output;

  std::reverse_copy(key.begin(), key.end(), key_reversed.begin());
  std::reverse_copy(message.begin(), message.end(), message_reversed.begin());

  aes_context ctx;
  aes_set_key(key_reversed.data(), key_reversed.size(), &ctx);
  aes_encrypt(message_reversed.data(), output.data(), &ctx);

  std::reverse(output.begin(), output.end());
  return output;
}

/** utility function to padding the given text to be a 128 bits data. The
 * parameter dest is input and output parameter, it must point to a
 * OCTET16_LEN memory space; where include length bytes valid data. */
static void padding(Octet16* dest, uint8_t length) {
  uint8_t i, *p = dest->data();
  /* original last block */
  for (i = length; i < OCTET16_LEN; i++)
    p[OCTET16_LEN - i - 1] = (i == length) ? 0x80 : 0;
}

/** utility function to left shift one bit for a 128 bits value. */
static void leftshift_onebit(uint8_t* input, uint8_t* output) {
  uint8_t i, overflow = 0, next_overflow = 0;
  DVLOG(2) << __func__;
  /* input[0] is LSB */
  for (i = 0; i < OCTET16_LEN; i++) {
    next_overflow = (input[i] & 0x80) ? 1 : 0;
    output[i] = (input[i] << 1) | overflow;
    overflow = next_overflow;
  }
  return;
}

/** This function is the calculation of block cipher using AES-128. */
static Octet16 cmac_aes_k_calculate(const Octet16& key) {
  Octet16 output;
  Octet16 x{0};  // zero initialized

  DVLOG(2) << __func__;

  uint8_t i = 1;
  while (i <= cmac_cb.round) {
    /* Mi' := Mi (+) X  */
    xor_128((Octet16*)&cmac_cb.text[(cmac_cb.round - i) * OCTET16_LEN], x);

    output = aes_128(key, &cmac_cb.text[(cmac_cb.round - i) * OCTET16_LEN],
                     OCTET16_LEN);
    x = output;
    i++;
  }

  return output;
}

/** This function proceeed to prepare the last block of message Mn depending on
 * the size of the message.
 */
static void cmac_prepare_last_block(const Octet16& k1, const Octet16& k2) {
  //    uint8_t     x[16] = {0};
  bool flag;

  DVLOG(2) << __func__;
  /* last block is a complete block set flag to 1 */
  flag = ((cmac_cb.len % OCTET16_LEN) == 0 && cmac_cb.len != 0) ? true : false;

  DVLOG(2) << "flag=" << flag << " round=" << cmac_cb.round;

  if (flag) { /* last block is complete block */
    xor_128((Octet16*)&cmac_cb.text[0], k1);
  } else /* padding then xor with k2 */
  {
    padding((Octet16*)&cmac_cb.text[0], (uint8_t)(cmac_cb.len % 16));

    xor_128((Octet16*)&cmac_cb.text[0], k2);
  }
}

/** This is the function to generate the two subkeys.
 * |key| is CMAC key, expect SRK when used by SMP.
 */
static void cmac_generate_subkey(const Octet16& key) {
  DVLOG(2) << __func__;

  Octet16 zero{};
  Octet16 p = aes_128(key, zero.data(), OCTET16_LEN);

  Octet16 k1, k2;
  uint8_t* pp = p.data();

  /* If MSB(L) = 0, then K1 = L << 1 */
  if ((pp[OCTET16_LEN - 1] & 0x80) != 0) {
    /* Else K1 = ( L << 1 ) (+) Rb */
    leftshift_onebit(pp, k1.data());
    xor_128(&k1, const_Rb);
  } else {
    leftshift_onebit(pp, k1.data());
  }

  if ((k1[OCTET16_LEN - 1] & 0x80) != 0) {
    /* K2 =  (K1 << 1) (+) Rb */
    leftshift_onebit(k1.data(), k2.data());
    xor_128(&k2, const_Rb);
  } else {
    /* If MSB(K1) = 0, then K2 = K1 << 1 */
    leftshift_onebit(k1.data(), k2.data());
  }

  cmac_prepare_last_block(k1, k2);
}

/** key - CMAC key in little endian order
 *  input - text to be signed in little endian byte order.
 *  length - length of the input in byte.
 */
Octet16 aes_cmac(const Octet16& key, const uint8_t* input, uint16_t length) {
  uint16_t len, diff;
  /* n is number of rounds */
  uint16_t n = (length + OCTET16_LEN - 1) / OCTET16_LEN;

  DVLOG(2) << __func__;

  if (n == 0) n = 1;
  len = n * OCTET16_LEN;

  DVLOG(2) << "AES128_CMAC started, allocate buffer size=" << len;
  /* allocate a memory space of multiple of 16 bytes to hold text  */
  cmac_cb.text = (uint8_t*)alloca(len);
  cmac_cb.round = n;
  diff = len - length;

  if (input != NULL && length > 0) {
    memcpy(&cmac_cb.text[diff], input, (int)length);
    cmac_cb.len = length;
  } else {
    cmac_cb.len = 0;
  }

  /* prepare calculation for subkey s and last block of data */
  cmac_generate_subkey(key);
  /* start calculation */
  Octet16 signature = cmac_aes_k_calculate(key);

  /* clean up */
  memset(&cmac_cb, 0, sizeof(tCMAC_CB));
  // cmac_cb.text is auto-freed by alloca

  return signature;
}

}  // namespace crypto_toolbox
