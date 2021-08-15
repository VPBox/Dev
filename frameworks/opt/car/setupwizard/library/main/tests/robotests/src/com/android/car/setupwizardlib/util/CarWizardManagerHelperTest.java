/*
 * Copyright (C) 2018 The Android Open Source Project
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
package com.android.car.setupwizardlib.util;

import static com.google.common.truth.Truth.assertThat;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.provider.Settings;

import com.android.car.setupwizardlib.robolectric.BaseRobolectricTest;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.robolectric.RobolectricTestRunner;
import org.robolectric.RuntimeEnvironment;

/**
 * Tests for the {@link CarWizardManagerHelper}.
 */
@RunWith(RobolectricTestRunner.class)
public class CarWizardManagerHelperTest extends BaseRobolectricTest {
    private static final String EXTRA_RESULT_CODE = "com.android.setupwizard.ResultCode";

    /**
     * Test that calling {@link CarWizardManagerHelper#getNextIntent} with null data works as
     * intended.
     */
    @Test
    public void testGetNextIntentWithNullData() {
        Intent intent = new Intent();

        Intent nextIntent = CarWizardManagerHelper.getNextIntent(intent, 1);

        assertThat(nextIntent.getIntExtra(EXTRA_RESULT_CODE, 0)).isEqualTo(1);
    }

    /**
     * Test that calling {@link CarWizardManagerHelper#getNextIntent} with data creates an intent
     * that carries over the data.
     */
    @Test
    public void testGetNextIntentWithData() {
        Intent intent = new Intent();
        Intent data = new Intent();
        data.putExtra("testExtra", 1);

        Intent nextIntent = CarWizardManagerHelper.getNextIntent(intent, 1, data);

        assertThat(nextIntent.getIntExtra(EXTRA_RESULT_CODE, 0)).isEqualTo(1);
        assertThat(nextIntent.getIntExtra("testExtra", 0)).isEqualTo(1);
    }

    /**
     * Test that calling {@link CarWizardManagerHelper#copyWizardManagerExtras} copies over the
     * car wizard-related extras.
     */
    @Test
    public void testCopyWizardManagerExtras() {
        Intent srcIntent = new Intent();
        Bundle extraBundle = new Bundle();
        srcIntent.putExtra(CarWizardManagerHelper.EXTRA_WIZARD_BUNDLE, extraBundle);
        srcIntent.putExtra(CarWizardManagerHelper.EXTRA_IS_FIRST_RUN, true);
        srcIntent.putExtra(CarWizardManagerHelper.EXTRA_IS_DEALER, true);
        srcIntent.putExtra(CarWizardManagerHelper.EXTRA_IS_DEFERRED_SETUP, true);

        Intent disIntent = new Intent();
        CarWizardManagerHelper.copyWizardManagerExtras(srcIntent, disIntent);

        assertThat(disIntent.getBundleExtra(CarWizardManagerHelper.EXTRA_WIZARD_BUNDLE)).isEqualTo(
                extraBundle);
        assertThat(disIntent.getBooleanExtra(CarWizardManagerHelper.EXTRA_IS_FIRST_RUN,
                false)).isTrue();
        assertThat(
                disIntent.getBooleanExtra(CarWizardManagerHelper.EXTRA_IS_DEALER, false)).isTrue();
        assertThat(disIntent.getBooleanExtra(CarWizardManagerHelper.EXTRA_IS_DEFERRED_SETUP,
                false)).isTrue();
    }

    /**
     * Test that {@link CarWizardManagerHelper#isSetupWizardIntent} works.
     */
    @Test
    public void testIsSetupWizardIntentTrue() {
        Intent intent = new Intent();
        intent.putExtra(CarWizardManagerHelper.EXTRA_IS_FIRST_RUN, true);

        assertThat(CarWizardManagerHelper.isSetupWizardIntent(intent)).isTrue();
    }

    /**
     * Test that {@link CarWizardManagerHelper#isSetupWizardIntent} works.
     */
    @Test
    public void testIsSetupWizardIntentFalse() {
        Intent intent = new Intent();
        intent.putExtra(CarWizardManagerHelper.EXTRA_IS_FIRST_RUN, false);

        assertThat(CarWizardManagerHelper.isSetupWizardIntent(intent)).isFalse();
    }

    /**
     * Test that {@link CarWizardManagerHelper#isDeferredIntent} works.
     */
    @Test
    public void testIsDeferredIntent_doesNotHaveExtra_shouldBeFalse() {
        Intent intent = new Intent();
        intent.putExtra(CarWizardManagerHelper.EXTRA_IS_DEFERRED_SETUP, false);

        assertThat(CarWizardManagerHelper.isDeferredIntent(intent)).isFalse();
    }

    /**
     * Test that {@link CarWizardManagerHelper#isDeferredIntent} works.
     */
    @Test
    public void testIsDeferredIntent_doesHaveExtra_shouldBeTrue() {
        Intent intent = new Intent();
        intent.putExtra(CarWizardManagerHelper.EXTRA_IS_DEFERRED_SETUP, true);

        assertThat(CarWizardManagerHelper.isDeferredIntent(intent)).isTrue();
    }

    /**
     * Test that {@link CarWizardManagerHelper#isDealerIntent} works.
     */
    @Test
    public void testIsDealerIntentTrue() {
        Intent intent = new Intent();
        intent.putExtra(CarWizardManagerHelper.EXTRA_IS_DEALER, true);

        assertThat(CarWizardManagerHelper.isDealerIntent(intent)).isTrue();
    }

    /**
     * Test that {@link CarWizardManagerHelper#isDealerIntent} works.
     */
    @Test
    public void testIsDealerIntentFalse() {
        Intent intent = new Intent();
        intent.putExtra(CarWizardManagerHelper.EXTRA_IS_DEALER, false);

        assertThat(CarWizardManagerHelper.isDealerIntent(intent)).isFalse();
    }

    /**
     * Test that {@link CarWizardManagerHelper#isUserSetupComplete} works.
     */
    @Test
    public void testIsUserSetupCompleteTrue() {
        Context context = RuntimeEnvironment.application;
        Settings.Secure.putInt(context.getContentResolver(),
                Settings.Secure.USER_SETUP_COMPLETE, 1);
        assertThat(CarWizardManagerHelper.isUserSetupComplete(context)).isTrue();
    }

    /**
     * Test that {@link CarWizardManagerHelper#isUserSetupComplete} works.
     */
    @Test
    public void testIsUserSetupCompleteFalse() {
        Context context = RuntimeEnvironment.application;
        Settings.Secure.putInt(context.getContentResolver(),
                Settings.Secure.USER_SETUP_COMPLETE, 0);
        assertThat(CarWizardManagerHelper.isUserSetupComplete(context)).isFalse();
    }

    /**
     * Test that {@link CarWizardManagerHelper#isDeviceProvisioned} works.
     */
    @Test
    public void testIsDeviceProvisionedTrue() {
        Context context = RuntimeEnvironment.application;
        Settings.Global.putInt(context.getContentResolver(),
                Settings.Global.DEVICE_PROVISIONED, 1);
        assertThat(CarWizardManagerHelper.isDeviceProvisioned(context)).isTrue();
    }

    /**
     * Test that {@link CarWizardManagerHelper#isDeviceProvisioned} works.
     */
    @Test
    public void testIsDeviceProvisionedFalse() {
        Context context = RuntimeEnvironment.application;
        Settings.Global.putInt(context.getContentResolver(),
                Settings.Global.DEVICE_PROVISIONED, 0);
        assertThat(CarWizardManagerHelper.isDeviceProvisioned(context)).isFalse();
    }
}
