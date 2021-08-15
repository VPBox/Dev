//
// Copyright (C) 2014 The Android Open Source Project
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

#ifndef UPDATE_ENGINE_PAYLOAD_CONSUMER_PAYLOAD_VERIFIER_H_
#define UPDATE_ENGINE_PAYLOAD_CONSUMER_PAYLOAD_VERIFIER_H_

#include <string>

#include <base/macros.h>
#include <brillo/secure_blob.h>

#include "update_engine/update_metadata.pb.h"

// This class encapsulates methods used for payload signature verification.
// See payload_generator/payload_signer.h for payload signing.

namespace chromeos_update_engine {

class PayloadVerifier {
 public:
  // Interprets |signature_proto| as a protocol buffer containing the Signatures
  // message and decrypts each signature data using the |pem_public_key|.
  // |pem_public_key| should be a PEM format RSA public key data.
  // Pads the 32 bytes |sha256_hash_data| to 256 or 512 bytes according to the
  // PKCS#1 v1.5 standard; and returns whether *any* of the decrypted hashes
  // matches the padded hash data. In case of any error parsing the signatures
  // or the public key, returns false.
  static bool VerifySignature(const std::string& signature_proto,
                              const std::string& pem_public_key,
                              const brillo::Blob& sha256_hash_data);

  // Decrypts |sig_data| with the given |pem_public_key| and populates
  // |out_hash_data| with the decoded raw hash. |pem_public_key| should be a PEM
  // format RSA public key data. Returns true if successful, false otherwise.
  static bool GetRawHashFromSignature(const brillo::Blob& sig_data,
                                      const std::string& pem_public_key,
                                      brillo::Blob* out_hash_data);

  // Pads a SHA256 hash so that it may be encrypted/signed with RSA2048 or
  // RSA4096 using the PKCS#1 v1.5 scheme.
  // hash should be a pointer to vector of exactly 256 bits. |rsa_size| must be
  // one of 256 or 512 bytes. The vector will be modified in place and will
  // result in having a length of 2048 or 4096 bits, depending on the rsa size.
  // Returns true on success, false otherwise.
  static bool PadRSASHA256Hash(brillo::Blob* hash, size_t rsa_size);

 private:
  // This should never be constructed
  DISALLOW_IMPLICIT_CONSTRUCTORS(PayloadVerifier);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_PAYLOAD_CONSUMER_PAYLOAD_VERIFIER_H_
