/*
 * Copyright (C) 2015 The Android Open Source Project
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

package com.android.internal.telephony;

/**
 * An object that provides supporting methods, fields, and other functionality for configuring
 * and inspecting the results of operations on a test double (mock, fake or stub). The test double
 * is an object of type {@code T}.
 */
public interface TestFixture<T> {

    /**
     * Obtain the actual test double provided by this holder. It is a requirement of this API
     * that the test double as returned from this method be a Mockito mock or spy, so that a test
     * can use Mockito APIs to directly instrument its behavior where needed.
     *
     * @return the test double.
     */
    T getTestDouble();
}
