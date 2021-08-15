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

#include <keymaster/km_openssl/software_random_source.h>
#include <openssl/rand.h>

namespace keymaster {

keymaster_error_t SoftwareRandomSource::GenerateRandom(uint8_t* buffer, size_t length) const {
    if (RAND_bytes(buffer, length) != 1)
        return KM_ERROR_UNKNOWN_ERROR;
    return KM_ERROR_OK;
}

}
