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

import static org.robolectric.RuntimeEnvironment.application;

import android.graphics.drawable.Drawable;


import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.robolectric.RobolectricTestRunner;

import java.util.Arrays;
import java.util.List;

/** Unit tests for {@link PartnerConfigHelper}. */
@RunWith(RobolectricTestRunner.class)
public class PartnerConfigHelperTest {

    private static final String TEST_PACKAGE_NAME = "test.packageName";

    private static final PartnerConfig TEST_COLOR_RESOURCE_NAME =
            PartnerConfig.CONFIG_TOOLBAR_BG_COLOR;

    private static final PartnerConfig TEST_DIMENSION_RESOURCE_NAME =
            PartnerConfig.CONFIG_TOOLBAR_BUTTON_RADIUS;

    private static final PartnerConfig TEST_STRING_RESOURCE_NAME =
            PartnerConfig.CONFIG_TOOLBAR_BUTTON_FONT_FAMILY;

    private static final PartnerConfig TEST_DRAWABLE_RESOURCE_NAME =
            PartnerConfig.CONFIG_TOOLBAR_BUTTON_ICON_BACK;

    private static final String EXCEPTED_STRING = "myFont";
    private static final float EXCEPTED_DIMENSION = 10;

    private Drawable mExpectedDrawable;
    private List<ResourceEntry> mResourceEntries;

    @Before
    public void setUp() {
        FakeOverrideContentProvider fakeOverrideDataProvider =
                FakeOverrideContentProvider.installEmptyProvider();
        mResourceEntries = prepareFakeData();
        for (ResourceEntry entry : mResourceEntries) {
            fakeOverrideDataProvider.injectResourceEntry(entry);
        }

        PartnerConfigHelper.resetForTesting();
    }

    @Test(expected = IllegalArgumentException.class)
    public void getDimension_withWrongConfigType_shouldThrowException() {
        PartnerConfigHelper helper = PartnerConfigHelper.get(application);
        assertThat(helper.mPartnerResourceCache).doesNotContainKey(TEST_COLOR_RESOURCE_NAME);
        helper.getDimension(application, TEST_COLOR_RESOURCE_NAME);
    }

    @Test
    public void getDimension_shouldReturnExpectedDimension() {
        PartnerConfigHelper helper = PartnerConfigHelper.get(application);
        assertThat(helper.mPartnerResourceCache).doesNotContainKey(TEST_DIMENSION_RESOURCE_NAME);

        float result = helper.getDimension(application, TEST_DIMENSION_RESOURCE_NAME);
        assertThat(result).isEqualTo(EXCEPTED_DIMENSION);
        assertThat(helper.mPartnerResourceCache).containsKey(TEST_DIMENSION_RESOURCE_NAME);
        assertThat(helper.mPartnerResourceCache.get(TEST_DIMENSION_RESOURCE_NAME))
                .isEqualTo(EXCEPTED_DIMENSION);
    }

    @Test
    public void getColor_shouldReturnExpectedColor() {
        PartnerConfigHelper helper = PartnerConfigHelper.get(application);
        assertThat(helper.mPartnerResourceCache).doesNotContainKey(TEST_COLOR_RESOURCE_NAME);

        int result = helper.getColor(application, TEST_COLOR_RESOURCE_NAME);
        assertThat(result).isEqualTo(android.R.color.darker_gray);
        assertThat(helper.mPartnerResourceCache).containsKey(TEST_COLOR_RESOURCE_NAME);
        assertThat(helper.mPartnerResourceCache.get(TEST_COLOR_RESOURCE_NAME))
                .isEqualTo(android.R.color.darker_gray);
    }

    @Test
    public void getString_shouldReturnExpectedString() {
        PartnerConfigHelper helper = PartnerConfigHelper.get(application);
        assertThat(helper.mPartnerResourceCache).doesNotContainKey(TEST_STRING_RESOURCE_NAME);

        String result = helper.getString(application, TEST_STRING_RESOURCE_NAME);
        assertThat(result).isEqualTo(EXCEPTED_STRING);
        assertThat(helper.mPartnerResourceCache).containsKey(TEST_STRING_RESOURCE_NAME);
        assertThat(helper.mPartnerResourceCache.get(TEST_STRING_RESOURCE_NAME))
                .isEqualTo(EXCEPTED_STRING);
    }

    private List<ResourceEntry> prepareFakeData() {
        ExternalResources.Resources testResources =
                ExternalResources.injectExternalResources(TEST_PACKAGE_NAME);

        testResources
                .putColor(TEST_COLOR_RESOURCE_NAME.getResourceName(), android.R.color.darker_gray);
        testResources
                .putDimension(TEST_DIMENSION_RESOURCE_NAME.getResourceName(), EXCEPTED_DIMENSION);
        testResources.putText(TEST_STRING_RESOURCE_NAME.getResourceName(), EXCEPTED_STRING);

        mExpectedDrawable =
                application
                        .getResources()
                        .getDrawable(android.R.drawable.arrow_up_float, null);
        testResources.putDrawable(TEST_DRAWABLE_RESOURCE_NAME.getResourceName(), mExpectedDrawable);

        return Arrays.asList(
                new ResourceEntry(
                        TEST_PACKAGE_NAME,
                        TEST_COLOR_RESOURCE_NAME.getResourceName(),
                        testResources.getIdentifier(
                                TEST_COLOR_RESOURCE_NAME.getResourceName(),
                                /* defType= */ "color",
                                TEST_PACKAGE_NAME)),
                new ResourceEntry(
                        TEST_PACKAGE_NAME,
                        TEST_DIMENSION_RESOURCE_NAME.getResourceName(),
                        testResources.getIdentifier(
                                TEST_DIMENSION_RESOURCE_NAME.getResourceName(),
                                /* defType= */ "dimen",
                                TEST_PACKAGE_NAME)),
                new ResourceEntry(
                        TEST_PACKAGE_NAME,
                        TEST_STRING_RESOURCE_NAME.getResourceName(),
                        testResources.getIdentifier(
                                TEST_STRING_RESOURCE_NAME.getResourceName(),
                                /* defType= */"string",
                                TEST_PACKAGE_NAME)),
                new ResourceEntry(
                        TEST_PACKAGE_NAME,
                        TEST_DRAWABLE_RESOURCE_NAME.getResourceName(),
                        testResources.getIdentifier(
                                TEST_DRAWABLE_RESOURCE_NAME.getResourceName(),
                                /* defType= */ "drawable",
                                TEST_PACKAGE_NAME))
        );
    }
}
