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

package com.android.car.setupwizardlib;

import static com.google.common.truth.Truth.assertThat;

import android.app.Activity;
import android.graphics.drawable.ColorDrawable;

import com.android.car.setupwizardlib.partner.FakeOverrideContentProvider;
import com.android.car.setupwizardlib.robolectric.BaseRobolectricTest;

import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.robolectric.Robolectric;
import org.robolectric.RobolectricTestRunner;

/**
 * Tests for the CarSetupWizardDesignLayout
 */
@RunWith(RobolectricTestRunner.class)
public class CarSetupWizardDesignLayoutTest extends BaseRobolectricTest {

    private CarSetupWizardDesignLayout mCarSetupWizardLayout;

    @Before
    public void setupLayout() {
        FakeOverrideContentProvider.installDefaultProvider();
        Activity activity = Robolectric
                .buildActivity(BaseDesignActivity.class)
                .create()
                .get();
        mCarSetupWizardLayout = activity.findViewById(R.id.car_setup_wizard_layout);
    }

    @Test
    public void testShouldApplyLayoutBackground() {
        ColorDrawable bg = (ColorDrawable) mCarSetupWizardLayout.getBackground();
        assertThat(bg.getColor()).isEqualTo(FakeOverrideContentProvider.ANDROID_COLOR_DARK_GRAY);
    }
}
