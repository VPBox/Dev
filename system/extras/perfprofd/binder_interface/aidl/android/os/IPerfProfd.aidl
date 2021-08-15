/*
 * Copyright (C) 2017 The Android Open Source Project
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

package android.os;

/** {@hide} */
interface IPerfProfd {
    /**
     * Start continuous profiling with the given parameters.
     */
    void startProfiling(int collectionInterval, int iterations,
            int process, int samplingPeriod, int samplingFrequency,
            int sampleDuration, boolean stackProfile,
            boolean useElfSymbolizer, boolean sendToDropbox);

    /**
     * Start continuous profiling with the given encoded parameters.
     * Parameters should be encoded in the ConfigReader syntax,
     * separated by colons.
     */
    void startProfilingString(String config);

    /**
     * Start profiling with the parameters in the given protobuf.
     */
    void startProfilingProtobuf(in byte[] config_proto);

    /**
     * Stop an active profiling session.
     */
    void stopProfiling();
}
