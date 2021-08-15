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

import com.android.car.setupwizardlib.robolectric.BaseRobolectricTest;
import com.android.car.setupwizardlib.shadows.ShadowCar;

import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.robolectric.Robolectric;
import org.robolectric.RobolectricTestRunner;
import org.robolectric.annotation.Config;

/**
 * Unit tests for the {@link BaseDesignActivity}.
 */
@RunWith(RobolectricTestRunner.class)
@Config(shadows = ShadowCar.class)
public class BaseDesignActivityTest extends BaseRobolectricTest {

    private BaseDesignActivity mDesignActivity;

    @Before
    public void setupLayout() {
        mDesignActivity = Robolectric
                .buildActivity(BaseDesignActivity.class)
                .create()
                .get();
    }

    @Test
    public void testLayoutIsDesign() {
        assertThat(mDesignActivity.getLayout()).isEqualTo(R.layout.base_design_activity);
    }

    @Test
    public void testGetCarSetupWizardLayout() {
        assertThat(mDesignActivity.getCarSetupWizardLayout())
                .isInstanceOf(CarSetupWizardDesignLayout.class);
    }
}
