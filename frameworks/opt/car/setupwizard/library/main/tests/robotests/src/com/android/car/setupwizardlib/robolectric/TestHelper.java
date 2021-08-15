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
package com.android.car.setupwizardlib.robolectric;

import static com.google.common.truth.Truth.assertThat;

import android.view.View;
import android.widget.TextView;

import org.mockito.Mockito;

/**
 * Helper for commonly used assertions and mocks/spies.
 */
public final class TestHelper {

    private TestHelper() {
    }

    // View testing helpers.
    public static void assertViewVisible(View view) {
        assertThat(view.getVisibility()).isEqualTo(View.VISIBLE);
    }

    public static void assertViewNotVisible(View view) {
        assertThat(view.getVisibility()).isEqualTo(View.GONE);
    }

    public static void assertViewEnabled(View view) {
        assertThat(view.isEnabled()).isTrue();
    }

    public static void assertViewNotEnabled(View view) {
        assertThat(view.isEnabled()).isFalse();
    }

    public static void assertTextEqual(TextView view, String text) {
        assertThat(view.getText()).isEqualTo(text);
    }

    public static View.OnClickListener createSpyListener() {
        return Mockito.spy(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
            }
        });
    }
}
