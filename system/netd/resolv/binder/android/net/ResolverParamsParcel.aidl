/*
 * Copyright (C) 2019 The Android Open Source Project
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

package android.net;

/**
 * Configuration for a resolver parameters.
 *
 * {@hide}
 */
parcelable ResolverParamsParcel {
    /**
     * The network ID of the network for which information should be configured.
     */
    int netId;

    /**
     * Sample lifetime in seconds.
     */
    int sampleValiditySeconds;

    /**
     * Use to judge if the server is considered broken.
     */
    int successThreshold;

    /**
     * Min. # samples.
     */
    int minSamples;

    /**
     * Max # samples.
     */
    int maxSamples;

    /**
     * Retransmission interval in milliseconds.
     */
    int baseTimeoutMsec;

    /**
     * Number of retries.
     */
    int retryCount;

    /**
     * The DNS servers to configure for the network.
     */
    @utf8InCpp String[] servers;

    /**
     * The search domains to configure.
     */
    @utf8InCpp String[] domains;

    /**
     * The TLS subject name to require for all servers, or empty if there is none.
     */
    @utf8InCpp String   tlsName;

    /**
     * The DNS servers to configure for strict mode Private DNS.
     */
    @utf8InCpp String[] tlsServers;

    /**
     * An array containing TLS public key fingerprints (pins) of which each server must match
     * at least one, or empty if there are no pinned keys.
     */
    // DEPRECATED: remove tlsFingerprints in new code
    @utf8InCpp String[] tlsFingerprints;
}
