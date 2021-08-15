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

package android.content.res;

import com.android.resources.ResourceType;
import com.android.resources.ResourceUrl;

import org.junit.Test;

import android.annotation.NonNull;
import android.annotation.Nullable;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.fail;

public class Resources_DelegateTest {
    private static void assertResourceUrl(@Nullable String pkg, @NonNull String name,
            @NonNull ResourceType type, @Nullable ResourceUrl url) {
        assertNotNull(url);
        assertEquals(type, url.type);
        assertEquals(pkg, url.namespace);
        assertEquals(name, url.name);
    }

    @Test
    public void resourceUrlFromName() {
        try {
            Resources_Delegate.resourceUrlFromName("pkg:name", null, null);
            fail("Expected IllegalArgumentException since no type was defined");
        } catch (IllegalArgumentException ignored) {
        }

        assertNull(Resources_Delegate.resourceUrlFromName("package:invalid/name", null, null));
        assertNull(Resources_Delegate.resourceUrlFromName("package:name", "invalid", null));
        assertResourceUrl("package", "name", ResourceType.ID,
                Resources_Delegate.resourceUrlFromName("package:name", "id", null));
        assertResourceUrl("package", "name", ResourceType.ID,
                Resources_Delegate.resourceUrlFromName("name", "id", "package"));
        assertResourceUrl("package", "test", ResourceType.STRING,
                Resources_Delegate.resourceUrlFromName("package:string/test", null, null));
        assertResourceUrl(null, "test", ResourceType.STRING,
                Resources_Delegate.resourceUrlFromName("string/test", null, null));


        // Type and package in the name take precedence over the passed defType and defPackage
        assertResourceUrl("p1", "r1", ResourceType.STRING,
                Resources_Delegate.resourceUrlFromName("p1:string/r1", "id", "p2"));
        assertResourceUrl("p2", "r1", ResourceType.STRING,
                Resources_Delegate.resourceUrlFromName("string/r1", "id", "p2"));
        assertResourceUrl("p1", "r1", ResourceType.ID,
                Resources_Delegate.resourceUrlFromName("p1:r1", "id", "p2"));
    }
}