/*
 * Copyright (C) 2016 The Android Open Source Project
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

package com.android.server.wifi;

import android.os.Binder;

/**
 * Utilities for faking the calling uid in Binder.
 */
public class BinderUtil {
    /**
     * Fake the calling uid in Binder.
     * @param uid the calling uid that Binder should return from now on
     */
    public static void setUid(int uid) {
        Binder.restoreCallingIdentity((((long) uid) << 32) | Binder.getCallingPid());
    }
}
