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

import static org.robolectric.RuntimeEnvironment.application;

import android.app.Activity;
import android.content.res.ColorStateList;
import android.graphics.drawable.ColorDrawable;
import android.graphics.drawable.Drawable;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import androidx.annotation.ColorRes;
import androidx.annotation.StyleRes;

import com.android.car.setupwizardlib.partner.ExternalResources;
import com.android.car.setupwizardlib.partner.FakeOverrideContentProvider;
import com.android.car.setupwizardlib.partner.PartnerConfig;
import com.android.car.setupwizardlib.partner.ResourceEntry;
import com.android.car.setupwizardlib.robolectric.BaseRobolectricTest;
import com.android.car.setupwizardlib.robolectric.TestHelper;

import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mockito;
import org.robolectric.Robolectric;
import org.robolectric.RobolectricTestRunner;
import org.robolectric.Shadows;
import org.robolectric.shadows.ShadowTextView;
import org.robolectric.util.ReflectionHelpers;

import java.util.Arrays;
import java.util.List;
import java.util.Locale;

/**
 * Tests for the CarSetupWizardCompatLayout
 */
@RunWith(RobolectricTestRunner.class)
public class CarSetupWizardCompatLayoutTest extends BaseRobolectricTest {
    private static final Locale LOCALE_EN_US = new Locale("en", "US");
    // Hebrew locale can be used to test RTL.
    private static final Locale LOCALE_IW_IL = new Locale("iw", "IL");

    private CarSetupWizardCompatLayout mCarSetupWizardCompatLayout;

    private static final String TEST_PACKAGE_NAME = "test.packageName";

    private static final PartnerConfig TEST_TOOLBAR_BUTTON_TEXT_SIZE_RESOURCE_NAME =
            PartnerConfig.CONFIG_TOOLBAR_BUTTON_TEXT_SIZE;

    private static final float TOLERANCE = 0.001f;
    // A small value is picked so that it's not likely to coincide with the default font size
    private static final float EXCEPTED_TEXT_SIZE = 4;

    @Before
    public void setUp() {
        FakeOverrideContentProvider fakeOverrideDataProvider =
                FakeOverrideContentProvider.installEmptyProvider();
        List<ResourceEntry> resourceEntries = prepareFakeData();
        for (ResourceEntry entry : resourceEntries) {
            fakeOverrideDataProvider.injectResourceEntry(entry);
        }

        mCarSetupWizardCompatLayout = createCarSetupWizardCompatLayout();
        // Have to make this call first to ensure secondaryToolbar button is created from stub.
        mCarSetupWizardCompatLayout.setSecondaryToolbarButtonVisible(true);
        mCarSetupWizardCompatLayout.setSecondaryToolbarButtonVisible(false);
    }

    /**
     * Test that {@link CarSetupWizardCompatLayout#setBackButtonListener} does set the back button
     * listener.
     */
    @Test
    public void testSetBackButtonListener() {
        View.OnClickListener spyListener = TestHelper.createSpyListener();

        mCarSetupWizardCompatLayout.setBackButtonListener(spyListener);
        mCarSetupWizardCompatLayout.getBackButton().performClick();
        Mockito.verify(spyListener).onClick(mCarSetupWizardCompatLayout.getBackButton());
    }

    /**
     * Test that {@link CarSetupWizardCompatLayout#setBackButtonVisible} does set the view
     * visible/not visible and calls updateBackButtonTouchDelegate.
     */
    @Test
    public void testSetBackButtonVisibleTrue() {
        CarSetupWizardCompatLayout spyCarSetupWizardCompatLayout =
                Mockito.spy(mCarSetupWizardCompatLayout);

        spyCarSetupWizardCompatLayout.setBackButtonVisible(true);
        View backButton = spyCarSetupWizardCompatLayout.getBackButton();
        TestHelper.assertViewVisible(backButton);
        Mockito.verify(spyCarSetupWizardCompatLayout).updateBackButtonTouchDelegate(true);
    }

    /**
     * Test that {@link CarSetupWizardCompatLayout#setBackButtonVisible} does set the view
     * visible/not visible and calls updateBackButtonTouchDelegate.
     */
    @Test
    public void testSetBackButtonVisibleFalse() {
        CarSetupWizardCompatLayout spyCarSetupWizardCompatLayout =
                Mockito.spy(mCarSetupWizardCompatLayout);

        spyCarSetupWizardCompatLayout.setBackButtonVisible(false);
        View backButton = spyCarSetupWizardCompatLayout.getBackButton();
        TestHelper.assertViewNotVisible(backButton);
        Mockito.verify(spyCarSetupWizardCompatLayout).updateBackButtonTouchDelegate(false);
    }

    /**
     * Test that {@link CarSetupWizardCompatLayout#setToolbarTitleVisible} does set the view
     * visible/not visible.
     */
    @Test
    public void testSetToolbarTitleVisibleTrue() {
        View toolbarTitle = mCarSetupWizardCompatLayout.getToolbarTitle();

        mCarSetupWizardCompatLayout.setToolbarTitleVisible(true);
        TestHelper.assertViewVisible(toolbarTitle);
    }

    /**
     * Test that {@link CarSetupWizardCompatLayout#setToolbarTitleVisible} does set the view
     * visible/not visible.
     */
    @Test
    public void testSetToolbarTitleVisibleFalse() {
        View toolbarTitle = mCarSetupWizardCompatLayout.getToolbarTitle();

        mCarSetupWizardCompatLayout.setToolbarTitleVisible(false);
        TestHelper.assertViewNotVisible(toolbarTitle);
    }

    /**
     * Tests that {@link CarSetupWizardCompatLayout#setToolbarTitleText(String)} does set the
     * toolbar title text.
     */
    @Test
    public void testSetToolbarTitleText() {
        mCarSetupWizardCompatLayout.setToolbarTitleText("test title");
        TestHelper.assertTextEqual(mCarSetupWizardCompatLayout.getToolbarTitle(), "test title");
    }

    /**
     * Test that a call to setToolbarTitleStyle sets the text appearance on the toolbar title.
     */
    @Test
    public void testSetToolbarStyle() {
        @StyleRes int newStyle = R.style.TextAppearance_Car_Body2;
        mCarSetupWizardCompatLayout.setToolbarTitleStyle(newStyle);
        ShadowTextView shadowTextView =
                Shadows.shadowOf(mCarSetupWizardCompatLayout.getToolbarTitle());
        assertThat(shadowTextView.getTextAppearanceId()).isEqualTo(newStyle);
    }

    /**
     * Test that {@link CarSetupWizardCompatLayout#setPrimaryToolbarButtonVisible} does set the view
     * visible/not visible.
     */
    @Test
    public void testSetPrimaryToolbarButtonVisibleTrue() {
        View toolbarTitle = mCarSetupWizardCompatLayout.getPrimaryToolbarButton();

        mCarSetupWizardCompatLayout.setPrimaryToolbarButtonVisible(true);
        TestHelper.assertViewVisible(toolbarTitle);
    }

    /**
     * Test that {@link CarSetupWizardCompatLayout#setPrimaryToolbarButtonVisible} does set the view
     * visible/not visible.
     */
    @Test
    public void testSetPrimaryToolbarButtonVisibleFalse() {
        View toolbarTitle = mCarSetupWizardCompatLayout.getPrimaryToolbarButton();

        mCarSetupWizardCompatLayout.setPrimaryToolbarButtonVisible(false);
        TestHelper.assertViewNotVisible(toolbarTitle);
    }

    /**
     * Test that {@link CarSetupWizardCompatLayout#setPrimaryToolbarButtonEnabled} does set the view
     * enabled/not enabled.
     */
    @Test
    public void testSetPrimaryToolbarButtonEnabledTrue() {
        View toolbarTitle = mCarSetupWizardCompatLayout.getPrimaryToolbarButton();

        mCarSetupWizardCompatLayout.setPrimaryToolbarButtonEnabled(true);
        TestHelper.assertViewEnabled(toolbarTitle);
    }

    /**
     * Test that {@link CarSetupWizardCompatLayout#setPrimaryToolbarButtonEnabled} does set the view
     * enabled/not enabled.
     */
    @Test
    public void testSetPrimaryToolbarButtonEnabledFalse() {
        View toolbarTitle = mCarSetupWizardCompatLayout.getPrimaryToolbarButton();

        mCarSetupWizardCompatLayout.setPrimaryToolbarButtonEnabled(false);
        TestHelper.assertViewNotEnabled(toolbarTitle);
    }

    /**
     * Tests that {@link CarSetupWizardCompatLayout#setPrimaryToolbarButtonText(String)} does set
     * the primary toolbar button text.
     */
    @Test
    public void testSetPrimaryToolbarButtonText() {
        mCarSetupWizardCompatLayout.setPrimaryToolbarButtonText("test title");
        TestHelper.assertTextEqual(
                mCarSetupWizardCompatLayout.getPrimaryToolbarButton(), "test title");
    }

    /**
     * Test that {@link CarSetupWizardCompatLayout#setPrimaryToolbarButtonListener} does set the
     * primary toolbar button listener.
     */
    @Test
    public void testSetPrimaryToolbarButtonListener() {
        View.OnClickListener spyListener = TestHelper.createSpyListener();

        mCarSetupWizardCompatLayout.setPrimaryToolbarButtonListener(spyListener);
        mCarSetupWizardCompatLayout.getPrimaryToolbarButton().performClick();
        Mockito.verify(spyListener).onClick(mCarSetupWizardCompatLayout.getPrimaryToolbarButton());
    }

    /**
     * Test that {@link CarSetupWizardCompatLayout#createPrimaryToolbarButton} creates a new button
     * but holds over the correct attributes.
     */
    @Test
    public void testCreatePrimaryButtonTrue() {
        Button currPrimaryToolbarButton = mCarSetupWizardCompatLayout.getPrimaryToolbarButton();
        Button primaryToolbarButton = mCarSetupWizardCompatLayout.createPrimaryToolbarButton(true);

        assertThat(primaryToolbarButton.getVisibility()).isEqualTo(
                currPrimaryToolbarButton.getVisibility());
        assertThat(primaryToolbarButton.isEnabled()).isEqualTo(
                currPrimaryToolbarButton.isEnabled());
        assertThat(primaryToolbarButton.getText()).isEqualTo(currPrimaryToolbarButton.getText());
        assertThat(primaryToolbarButton.getLayoutParams()).isEqualTo(
                currPrimaryToolbarButton.getLayoutParams());
    }

    /**
     * Test that {@link CarSetupWizardCompatLayout#setSecondaryToolbarButtonVisible} does set the
     * view visible/not visible.
     */
    @Test
    public void testSetSecondaryToolbarButtonVisibleTrue() {
        View toolbarTitle = mCarSetupWizardCompatLayout.getSecondaryToolbarButton();

        mCarSetupWizardCompatLayout.setSecondaryToolbarButtonVisible(true);
        TestHelper.assertViewVisible(toolbarTitle);
    }

    /**
     * Test that {@link CarSetupWizardCompatLayout#setSecondaryToolbarButtonVisible} does set the
     * view visible/not visible.
     */
    @Test
    public void testSetSecondaryToolbarButtonVisibleFalse() {
        View toolbarTitle = mCarSetupWizardCompatLayout.getSecondaryToolbarButton();

        mCarSetupWizardCompatLayout.setSecondaryToolbarButtonVisible(false);
        TestHelper.assertViewNotVisible(toolbarTitle);
    }

    /**
     * Test that {@link CarSetupWizardCompatLayout#setSecondaryToolbarButtonEnabled} does set the
     * view enabled/not enabled.
     */
    @Test
    public void testSetSecondaryToolbarButtonEnabledTrue() {
        View toolbarTitle = mCarSetupWizardCompatLayout.getSecondaryToolbarButton();

        mCarSetupWizardCompatLayout.setSecondaryToolbarButtonEnabled(true);
        TestHelper.assertViewEnabled(toolbarTitle);
    }

    /**
     * Test that {@link CarSetupWizardCompatLayout#setSecondaryToolbarButtonEnabled} does set the
     * view enabled/not enabled.
     */
    @Test
    public void testSetSecondaryToolbarButtonEnabledFalse() {
        View toolbarTitle = mCarSetupWizardCompatLayout.getSecondaryToolbarButton();

        mCarSetupWizardCompatLayout.setSecondaryToolbarButtonEnabled(false);
        TestHelper.assertViewNotEnabled(toolbarTitle);
    }

    /**
     * Tests that {@link CarSetupWizardCompatLayout#setSecondaryToolbarButtonText(String)} does set
     * the secondary toolbar button text.
     */
    @Test
    public void testSetSecondaryToolbarButtonText() {
        mCarSetupWizardCompatLayout.setSecondaryToolbarButtonText("test title");
        TestHelper.assertTextEqual(
                mCarSetupWizardCompatLayout.getSecondaryToolbarButton(), "test title");
    }

    /**
     * Test that {@link CarSetupWizardCompatLayout#setSecondaryToolbarButtonListener} does set the
     * secondary toolbar button listener.
     */
    @Test
    public void testSetSecondaryToolbarButtonListener() {
        View.OnClickListener spyListener = TestHelper.createSpyListener();

        mCarSetupWizardCompatLayout.setSecondaryToolbarButtonListener(spyListener);
        mCarSetupWizardCompatLayout.getSecondaryToolbarButton().performClick();
        Mockito.verify(spyListener)
                .onClick(mCarSetupWizardCompatLayout.getSecondaryToolbarButton());
    }

    /**
     * Test that {@link CarSetupWizardCompatLayout#setProgressBarVisible} does set the view
     * visible/not visible.
     */
    @Test
    public void testSetProgressBarVisibleTrue() {
        View toolbarTitle = mCarSetupWizardCompatLayout.getProgressBar();

        mCarSetupWizardCompatLayout.setProgressBarVisible(true);
        TestHelper.assertViewVisible(toolbarTitle);
    }

    /**
     * Test that {@link CarSetupWizardCompatLayout#setProgressBarVisible} does set the view
     * visible/not visible.
     */
    @Test
    public void testSetProgressBarVisibleFalse() {
        View toolbarTitle = mCarSetupWizardCompatLayout.getProgressBar();

        mCarSetupWizardCompatLayout.setProgressBarVisible(false);
        TestHelper.assertViewNotVisible(toolbarTitle);
    }

    /**
     * Test that {@link CarSetupWizardCompatLayout#setProgressBarIndeterminate(boolean)}
     * does set the progress bar intermediate/not indeterminate.
     */
    @Test
    public void testSetProgressBarIndeterminateTrue() {
        mCarSetupWizardCompatLayout.setProgressBarIndeterminate(true);
        assertThat(mCarSetupWizardCompatLayout.getProgressBar().isIndeterminate()).isTrue();
    }

    /**
     * Test that {@link CarSetupWizardCompatLayout#setProgressBarIndeterminate(boolean)}
     * does set the progress bar intermediate/not indeterminate.
     */
    @Test
    public void testSetProgressBarIndeterminateFalse() {
        mCarSetupWizardCompatLayout.setProgressBarIndeterminate(false);
        assertThat(mCarSetupWizardCompatLayout.getProgressBar().isIndeterminate()).isFalse();
    }

    /**
     * Test that {@link CarSetupWizardCompatLayout#setProgressBarProgress} does set the progress.
     */
    @Test
    public void testSetProgressBarProgress() {
        mCarSetupWizardCompatLayout.setProgressBarProgress(80);
        assertThat(mCarSetupWizardCompatLayout.getProgressBar().getProgress()).isEqualTo(80);
    }

    @Test
    public void testApplyUpdatedLocale() {
        mCarSetupWizardCompatLayout.applyLocale(LOCALE_IW_IL);
        TextView toolbarTitle = mCarSetupWizardCompatLayout.getToolbarTitle();
        Button primaryToolbarButton = mCarSetupWizardCompatLayout.getPrimaryToolbarButton();
        Button secondaryToolbarButton = mCarSetupWizardCompatLayout.getSecondaryToolbarButton();

        assertThat(toolbarTitle.getTextLocale()).isEqualTo(LOCALE_IW_IL);
        assertThat(primaryToolbarButton.getTextLocale()).isEqualTo(LOCALE_IW_IL);
        assertThat(secondaryToolbarButton.getTextLocale()).isEqualTo(LOCALE_IW_IL);

        mCarSetupWizardCompatLayout.applyLocale(LOCALE_EN_US);
        assertThat(toolbarTitle.getTextLocale()).isEqualTo(LOCALE_EN_US);
        assertThat(primaryToolbarButton.getTextLocale()).isEqualTo(LOCALE_EN_US);
        assertThat(secondaryToolbarButton.getTextLocale()).isEqualTo(LOCALE_EN_US);
    }

    @Test
    public void testGetBackButton() {
        assertThat(mCarSetupWizardCompatLayout.getPrimaryToolbarButton()).isEqualTo(
                mCarSetupWizardCompatLayout.findViewById(R.id.primary_toolbar_button));
    }

    @Test
    public void testGetToolBarTitle() {
        assertThat(mCarSetupWizardCompatLayout.getToolbarTitle()).isEqualTo(
                mCarSetupWizardCompatLayout.findViewById(R.id.toolbar_title));
    }

    @Test
    public void testGetPrimaryToolBarButton() {
        assertThat(mCarSetupWizardCompatLayout.getPrimaryToolbarButton()).isEqualTo(
                mCarSetupWizardCompatLayout.findViewById(R.id.primary_toolbar_button));
    }

    @Test
    public void testGetSecondaryToolBarButton() {
        assertThat(mCarSetupWizardCompatLayout.getSecondaryToolbarButton()).isEqualTo(
                mCarSetupWizardCompatLayout.findViewById(R.id.secondary_toolbar_button));
    }

    @Test
    public void testGetProgressBar() {
        assertThat(mCarSetupWizardCompatLayout.getProgressBar()).isEqualTo(
                mCarSetupWizardCompatLayout.findViewById(R.id.progress_bar));
    }

    @Test
    public void testTitleBarElevationChange() {
        mCarSetupWizardCompatLayout.addElevationToTitleBar(/*animate= */ false);
        View titleBar = mCarSetupWizardCompatLayout.findViewById(R.id.application_bar);
        assertThat(titleBar.getElevation()).isEqualTo(
                application.getResources().getDimension(
                        R.dimen.title_bar_drop_shadow_elevation));

        mCarSetupWizardCompatLayout.removeElevationFromTitleBar(/*animate= */ false);
        assertThat(titleBar.getElevation()).isEqualTo(0f);
    }

    @Test
    public void testPartnerResourcesAreApplied() {
        setupFakeContentProvider();

        CarSetupWizardCompatLayout layout = createCarSetupWizardCompatLayout();

        // Verify primary button background
        Button primary = layout.getPrimaryToolbarButton();
        Drawable expected = application.getResources().getDrawable(R.drawable.button_ripple_bg);
        assertThat(getDrawbleDefaultColor(primary.getBackground()))
                .isEqualTo(getDrawbleDefaultColor(expected));

        // Verify primary button text size
        assertThat(primary.getTextSize())
                .isEqualTo(FakeOverrideContentProvider.DEFAULT_DIMENSION);

        // Verify paddings
        assertThat(primary.getPaddingStart())
                .isEqualTo(FakeOverrideContentProvider.DEFAULT_H_PADDING);

        assertThat(primary.getPaddingEnd())
                .isEqualTo(FakeOverrideContentProvider.DEFAULT_H_PADDING);

        assertThat(primary.getPaddingTop())
                .isEqualTo(FakeOverrideContentProvider.DEFAULT_V_PADDING);

        assertThat(primary.getPaddingBottom())
                .isEqualTo(FakeOverrideContentProvider.DEFAULT_V_PADDING);
    }

    @Test
    public void testShouldNotApplyLayoutBackground() {
        setupFakeContentProvider();
        CarSetupWizardCompatLayout layout = createCarSetupWizardCompatLayout();

        ColorDrawable bg = (ColorDrawable) layout.getBackground();
        assertThat(bg).isNull();
    }

    @Test
    public void testSetButtonTextColor() {
        setupFakeContentProvider();
        CarSetupWizardCompatLayout layout = createCarSetupWizardCompatLayout();
        Button primary = layout.getPrimaryToolbarButton();

        layout.setButtonTextColor(
                primary, PartnerConfig.CONFIG_LAYOUT_BG_COLOR);

        assertThat(primary.getCurrentTextColor())
                .isEqualTo(FakeOverrideContentProvider.ANDROID_COLOR_DARK_GRAY);
    }

    @Test
    public void testSetBackground() {
        setupFakeContentProvider();
        CarSetupWizardCompatLayout layout = createCarSetupWizardCompatLayout();
        layout.setSecondaryToolbarButtonVisible(true);
        Button secondary = layout.getSecondaryToolbarButton();

        layout.setBackground(
                secondary,
                PartnerConfig.CONFIG_TOOLBAR_PRIMARY_BUTTON_BG,
                PartnerConfig.CONFIG_TOOLBAR_SECONDARY_BUTTON_BG_COLOR);

        Drawable expected = application.getResources().getDrawable(R.drawable.button_ripple_bg);
        assertThat(getDrawbleDefaultColor(secondary.getBackground()))
                .isEqualTo(getDrawbleDefaultColor(expected));
    }

    @Test
    public void test_bothButtons_areStyled_inDefaultLayout() {
        Button primaryButton = mCarSetupWizardCompatLayout.getPrimaryToolbarButton();
        Button secondaryButton = mCarSetupWizardCompatLayout.getSecondaryToolbarButton();

        assertThat(primaryButton.getTextSize()).isWithin(TOLERANCE).of(EXCEPTED_TEXT_SIZE);
        assertThat(secondaryButton.getTextSize()).isWithin(TOLERANCE).of(EXCEPTED_TEXT_SIZE);
    }

    @Test
    public void test_bothButtons_areStyled_inAlternativeLayout() {
        Activity activity = Robolectric
                .buildActivity(CarSetupWizardLayoutAlternativeActivity.class)
                .create()
                .get();
        CarSetupWizardCompatLayout layout = activity.findViewById(R.id.car_setup_wizard_layout);

        Button primaryButton = layout.getPrimaryToolbarButton();
        Button secondaryButton = layout.getSecondaryToolbarButton();

        assertThat(primaryButton.getTextSize()).isWithin(TOLERANCE).of(EXCEPTED_TEXT_SIZE);
        assertThat(secondaryButton.getTextSize()).isWithin(TOLERANCE).of(EXCEPTED_TEXT_SIZE);
    }

    private void setupFakeContentProvider() {
        FakeOverrideContentProvider.installDefaultProvider();
    }

    private CarSetupWizardCompatLayout createCarSetupWizardCompatLayout() {
        Activity activity = Robolectric
                .buildActivity(CarSetupWizardLayoutTestActivity.class)
                .create()
                .get();

        return activity.findViewById(R.id.car_setup_wizard_layout);
    }

    private @ColorRes int getDrawbleDefaultColor(Drawable drawable) {
        Drawable.ConstantState state = drawable.getConstantState();
        ColorStateList colorStateList = ReflectionHelpers.getField(state, "mColor");
        return colorStateList.getDefaultColor();
    }

    private List<ResourceEntry> prepareFakeData() {
        ExternalResources.Resources testResources =
                ExternalResources.injectExternalResources(TEST_PACKAGE_NAME);

        testResources.putDimension(
                TEST_TOOLBAR_BUTTON_TEXT_SIZE_RESOURCE_NAME.getResourceName(), EXCEPTED_TEXT_SIZE);

        return Arrays.asList(
                new ResourceEntry(
                        TEST_PACKAGE_NAME,
                        TEST_TOOLBAR_BUTTON_TEXT_SIZE_RESOURCE_NAME.getResourceName(),
                        testResources.getIdentifier(
                                TEST_TOOLBAR_BUTTON_TEXT_SIZE_RESOURCE_NAME.getResourceName(),
                                /* defType= */ "dimen",
                                TEST_PACKAGE_NAME))
        );
    }
}
