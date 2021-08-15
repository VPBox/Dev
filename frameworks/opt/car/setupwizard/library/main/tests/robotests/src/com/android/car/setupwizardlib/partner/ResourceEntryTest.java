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

package com.android.car.setupwizardlib.partner;

import static com.google.common.truth.Truth.assertThat;

import android.os.Bundle;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.robolectric.RobolectricTestRunner;

/** Unit tests for {@link ResourceEntry}. */
@RunWith(RobolectricTestRunner.class)
public final class ResourceEntryTest {
    private static final String TEST_PACKAGE_NAME = "test.packagename";
    private static final int TEST_RESOURCE_ID = 1234;

    @Test
    public void toBundle_getBundleWithCorrectData() {
        ResourceEntry resourceEntry =
                new ResourceEntry(
                        TEST_PACKAGE_NAME,
                        PartnerConfigKey.KEY_TOOLBAR_BG_COLOR,
                        TEST_RESOURCE_ID);
        Bundle result = resourceEntry.toBundle();

        assertThat(result.getString(ResourceEntry.KEY_PACKAGE_NAME)).isEqualTo(TEST_PACKAGE_NAME);
        assertThat(result.getString(ResourceEntry.KEY_RESOURCE_NAME))
                .isEqualTo(PartnerConfigKey.KEY_TOOLBAR_BG_COLOR);
        assertThat(result.getInt(ResourceEntry.KEY_RESOURCE_ID)).isEqualTo(TEST_RESOURCE_ID);
    }

    @Test
    public void fromBundle_bundleWithRequiredData_getResourceEntryWithCorrectData() {
        Bundle source = new Bundle();
        source.putString(ResourceEntry.KEY_PACKAGE_NAME, TEST_PACKAGE_NAME);
        source.putString(
                ResourceEntry.KEY_RESOURCE_NAME, PartnerConfigKey.KEY_TOOLBAR_BG_COLOR);
        source.putInt(ResourceEntry.KEY_RESOURCE_ID, TEST_RESOURCE_ID);

        ResourceEntry result = ResourceEntry.fromBundle(source);
        assertThat(result.getPackageName()).isEqualTo(TEST_PACKAGE_NAME);
        assertThat(result.getResourceName())
                .isEqualTo(PartnerConfigKey.KEY_TOOLBAR_BG_COLOR);
        assertThat(result.getResourceId()).isEqualTo(TEST_RESOURCE_ID);
    }

    @Test
    public void fromNullBundle_returnNull() {
        assertThat(ResourceEntry.fromBundle(null)).isNull();
    }

    @Test
    public void fromBundleWithoutRequiredData_returnNull() {
        Bundle source = new Bundle();
        source.putString("wrongKeyName", TEST_PACKAGE_NAME);

        ResourceEntry result = ResourceEntry.fromBundle(source);
        assertThat(result).isNull();
    }
}
