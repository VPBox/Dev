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

import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;

import android.app.Activity;
import android.car.Car;
import android.car.CarNotConnectedException;
import android.car.drivingstate.CarUxRestrictions;
import android.car.drivingstate.CarUxRestrictionsManager;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;

import androidx.annotation.StyleRes;
import androidx.fragment.app.Fragment;

import com.android.car.setupwizardlib.robolectric.BaseRobolectricTest;
import com.android.car.setupwizardlib.robolectric.TestHelper;
import com.android.car.setupwizardlib.shadows.ShadowCar;

import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.mockito.Mockito;
import org.robolectric.Robolectric;
import org.robolectric.RobolectricTestRunner;
import org.robolectric.Shadows;
import org.robolectric.android.controller.ActivityController;
import org.robolectric.annotation.Config;
import org.robolectric.shadows.ShadowTextView;

/**
 * Unit tests for the {@link BaseCompatActivity}.
 */
@RunWith(RobolectricTestRunner.class)
@Config(shadows = ShadowCar.class)
public class BaseCompatActivityTest extends BaseRobolectricTest {
    private BaseCompatActivity mBaseCompatActivity;
    private CarSetupWizardBaseLayout mCarSetupWizardLayout;
    private ActivityController<BaseCompatActivity> mActivityController;
    @Mock
    private CarUxRestrictionsManager mMockRestrictionsManager;
    @Mock
    private CarUxRestrictions mMockRestrictions;

    @Before
    public void setupBaseCompatActivityAndLayout() throws CarNotConnectedException {
        mActivityController = Robolectric.buildActivity(BaseCompatActivity.class).create();
        mBaseCompatActivity = mActivityController.get();
        mCarSetupWizardLayout = mBaseCompatActivity.getCarSetupWizardLayout();
        ShadowCar.setCarManager(Car.CAR_UX_RESTRICTION_SERVICE, mMockRestrictionsManager);
        doReturn(mMockRestrictions).when(mMockRestrictionsManager).getCurrentCarUxRestrictions();
        doReturn(CarUxRestrictions.UX_RESTRICTIONS_BASELINE).when(mMockRestrictions)
                .getActiveRestrictions();
    }

    @Test
    public void testLayoutIsCompat() {
        assertThat(mBaseCompatActivity.getLayout()).isEqualTo(R.layout.base_compat_activity);
    }

    @Test
    public void testGetCarSetupWizardLayout() {
        assertThat(mBaseCompatActivity.getCarSetupWizardLayout())
                .isInstanceOf(CarSetupWizardCompatLayout.class);
    }

    /**
     * Test that the BaseCompatActivity's content view is set to be a CarSetupWizardLayout
     */
    @Test
    public void testContentViewIsCarSetupWizardLayout() {
        View contentView = mBaseCompatActivity.findViewById(R.id.car_setup_wizard_layout);
        assertThat(contentView).isNotNull();
        assertThat(contentView instanceof CarSetupWizardCompatLayout).isTrue();
    }

    private BaseCompatActivity createSpyBaseCompatActivity() {
        BaseCompatActivity spyBaseCompatActivity = Mockito.spy(
                (BaseCompatActivity) Robolectric.buildActivity(BaseCompatActivity.class).get());
        spyBaseCompatActivity.onCreate(null);

        return spyBaseCompatActivity;
    }

    /**
     * Test that the BaseCompatActivity sets the back button listener to call
     * {@link BaseCompatActivity#handleBackButton()} when created.
     */
    @Test
    public void testBackButtonListenerIsDefault() {
        BaseCompatActivity spyBaseCompatActivity = createSpyBaseCompatActivity();

        ImageView backButton = (ImageView) spyBaseCompatActivity.findViewById(
                R.id.back_button);
        backButton.performClick();

        verify(spyBaseCompatActivity).handleBackButton();
    }

    /**
     * Test that the BaseCompatActivity sets the secondary toolbar button listener to the default
     * when created.
     */
    @Test
    public void testSecondaryToolbarButtonListenerIsDefault() {
        BaseCompatActivity spyBaseCompatActivity = createSpyBaseCompatActivity();

        Button secondaryToolBarButton = (Button) spyBaseCompatActivity.findViewById(
                R.id.secondary_toolbar_button);
        secondaryToolBarButton.performClick();

        verify(spyBaseCompatActivity).nextAction(Activity.RESULT_OK);
    }

    /**
     * Test that the BaseCompatActivity sets the primary toolbar button listener to the default when
     * created.
     */
    @Test
    public void testPrimaryToolbarButtonListenerIsDefault() {
        BaseCompatActivity spyBaseCompatActivity = createSpyBaseCompatActivity();

        Button primaryToolBarButton = (Button) spyBaseCompatActivity.findViewById(
                R.id.primary_toolbar_button);
        primaryToolBarButton.performClick();

        verify(spyBaseCompatActivity).nextAction(Activity.RESULT_OK);
    }

    private BaseCompatActivity getStartedBaseCompatActivity() {
        return (BaseCompatActivity)
                Robolectric.buildActivity(BaseCompatActivity.class).create().start().get();
    }

    private BaseCompatActivity getSavedInstanceStateBaseCompatActivity() {
        return (BaseCompatActivity) Robolectric.buildActivity(
                BaseCompatActivity.class).create().saveInstanceState(new Bundle()).get();
    }

    /**
     * Test that fragment commits are allowed after {@link BaseCompatActivity#onStart()} is called.
     */
    @Test
    public void testFragmentCommitsAllowedAfterOnStart() {
        assertThat(getStartedBaseCompatActivity().getAllowFragmentCommits()).isTrue();
    }

    /**
     * Test that fragment commits are not allowed after {@link
     * BaseCompatActivity#onSaveInstanceState} is called.
     */
    @Test
    public void testFragmentCommitsNotAllowedAfterOnSavedInstanceState() {
        assertThat(getSavedInstanceStateBaseCompatActivity().getAllowFragmentCommits()).isFalse();
    }

    /**
     * Test that {@link BaseCompatActivity#setContentFragment} sets the content fragment and calls
     * the expected methods when fragment commits are allowed.
     */
    @Test
    public void testSetContentFragmentWhenFragmentCommitsAllowed() {
        BaseCompatActivity spyBaseCompatActivity = Mockito.spy(getStartedBaseCompatActivity());

        Fragment fragment = new Fragment();
        spyBaseCompatActivity.setContentFragment(fragment);

        assertThat(spyBaseCompatActivity.getContentFragment()).isEqualTo(fragment);
        assertThat(spyBaseCompatActivity.getSupportFragmentManager().getBackStackEntryCount())
                .isEqualTo(0);
        // Verify that onContentFragmentSet is called with the test fragment
        verify(spyBaseCompatActivity).onContentFragmentSet(fragment);
    }

    /**
     * Test that {@link BaseCompatActivity#setContentFragment} does nothing when fragment commits
     * are not allowed.
     */
    @Test
    public void testSetContentFragmentWhenFragmentCommitsNotAllowed() {
        BaseCompatActivity spyBaseCompatActivity =
                Mockito.spy(getSavedInstanceStateBaseCompatActivity());

        Fragment fragment = new Fragment();
        spyBaseCompatActivity.setContentFragment(fragment);

        assertThat(spyBaseCompatActivity.getContentFragment()).isEqualTo(null);
        assertThat(spyBaseCompatActivity.getSupportFragmentManager().getBackStackEntryCount())
                .isEqualTo(0);
        // Verify that onContentFragmentSet is not called
        verify(spyBaseCompatActivity, times(0)).onContentFragmentSet(fragment);
    }

    /**
     * Test that {@link BaseCompatActivity#setContentFragmentWithBackstack)} sets the content
     * fragment, adds it to the fragment backstack, and calls the expected methods when fragment
     * commits are allowed.
     */
    @Test
    public void testSetContentFragmentWithBackstackWhenFragmentCommitsAllowed() {
        BaseCompatActivity spyBaseCompatActivity = Mockito.spy(getStartedBaseCompatActivity());

        Fragment fragment = new Fragment();
        spyBaseCompatActivity.setContentFragmentWithBackstack(fragment);

        assertThat(spyBaseCompatActivity.getContentFragment()).isEqualTo(fragment);
        assertThat(spyBaseCompatActivity.getSupportFragmentManager().getBackStackEntryCount())
                .isEqualTo(1);
        // Verify that onContentFragmentSet is called with the test fragment
        verify(spyBaseCompatActivity).onContentFragmentSet(fragment);
    }

    /**
     * Test that {@link BaseCompatActivity#setContentFragmentWithBackstack)} does nothing when
     * fragment commits are not allowed.
     */
    @Test
    public void testSetContentFragmentWithBackstackWhenFragmentCommitsNotAllowed() {
        BaseCompatActivity spyBaseCompatActivity =
                Mockito.spy(getSavedInstanceStateBaseCompatActivity());

        Fragment fragment = new Fragment();
        spyBaseCompatActivity.setContentFragment(fragment);

        assertThat(spyBaseCompatActivity.getContentFragment()).isEqualTo(null);
        assertThat(spyBaseCompatActivity.getSupportFragmentManager().getBackStackEntryCount())
                .isEqualTo(0);
        // Verify that onContentFragmentSet is not called
        verify(spyBaseCompatActivity, times(0)).onContentFragmentSet(fragment);
    }

    /**
     * Test that {@link BaseCompatActivity#popBackStackImmediate()} returns false when no fragment
     * is added to the backstack.
     */
    @Test
    public void testPopBackStackImmediateWithEmptyStack() {
        assertThat(mBaseCompatActivity.popBackStackImmediate()).isEqualTo(false);
    }

    /**
     * Test that {@link BaseCompatActivity#popBackStackImmediate()} returns true when a fragment is
     * added to the backstack and that the fragment is popped off of the backstack.
     */
    @Test
    public void testPopBackStackImmediateWithFragmentInStack() {
        Fragment fragment = new Fragment();
        mBaseCompatActivity.setContentFragmentWithBackstack(fragment);
        assertThat(mBaseCompatActivity.popBackStackImmediate()).isEqualTo(true);

        assertThat(mBaseCompatActivity.getContentFragment()).isNull();
    }

    /**
     * Test that {@link BaseCompatActivity#getContentFragment()} returns the content fragment.
     */
    @Test
    public void testGetContentFragment() {
        Fragment fragment = new Fragment();
        mBaseCompatActivity.setContentFragment(fragment);

        assertThat(mBaseCompatActivity.getContentFragment()).isEqualTo(
                mBaseCompatActivity.getSupportFragmentManager().findFragmentByTag(
                        mBaseCompatActivity.CONTENT_FRAGMENT_TAG));
    }

    /**
     * Test that {@link BaseCompatActivity#setContentLayout} adds the specified layout to the
     * BaseCompatActivity.
     */
    @Test
    public void testSetContentLayout() {
        mBaseCompatActivity.setContentLayout(R.layout.base_activity_test_layout);
        View contentLayout = mBaseCompatActivity.findViewById(R.id.content_layout);
        assertThat(contentLayout).isNotNull();
    }

    /**
     * Test that {@link BaseCompatActivity#finishAction()} results in a call to
     * {@link BaseCompatActivity#finish}.
     */
    @Test
    public void testFinishAction() {
        BaseCompatActivity spyBaseCompatActivity = Mockito.spy(mBaseCompatActivity);
        spyBaseCompatActivity.finishAction();

        verify(spyBaseCompatActivity).finish();
    }

    /**
     * Test that {@link BaseCompatActivity#finishAction(int)} )} results in a call to
     * {@link BaseCompatActivity#nextAction} and {@link BaseCompatActivity#finish}.
     */
    @Test
    public void testFinishActionWithResultCode() {
        BaseCompatActivity spyBaseCompatActivity = Mockito.spy(mBaseCompatActivity);
        spyBaseCompatActivity.finishAction(BaseCompatActivity.RESULT_OK);

        verify(spyBaseCompatActivity).nextAction(BaseCompatActivity.RESULT_OK, null);
        verify(spyBaseCompatActivity).finish();
    }

    /**
     * Test that {@link BaseCompatActivity#setBackButtonVisible} sets the back button visible/not
     * visible.
     */
    @Test
    public void testSetBackButtonVisibleTrue() {
        mBaseCompatActivity.setBackButtonVisible(true);
        TestHelper.assertViewVisible(mCarSetupWizardLayout.getBackButton());
    }

    /**
     * Test that {@link BaseCompatActivity#setBackButtonVisible} sets the back button visible/not
     * visible.
     */
    @Test
    public void testSetBackButtonVisibleFalse() {
        mBaseCompatActivity.setBackButtonVisible(false);
        TestHelper.assertViewNotVisible(mCarSetupWizardLayout.getBackButton());
    }

    /**
     * Test that {@link BaseCompatActivity#setToolbarTitleVisible} sets the toolbar title
     * visible/not visible.
     */
    @Test
    public void testSetToolbarTitleVisibleTrue() {
        mBaseCompatActivity.setToolbarTitleVisible(true);
        TestHelper.assertViewVisible(mCarSetupWizardLayout.getToolbarTitle());
    }

    /**
     * Test that {@link BaseCompatActivity#setToolbarTitleVisible} sets the toolbar button
     * visible/not visible.
     */
    @Test
    public void testSetToolbarTitleVisibleFalse() {
        mBaseCompatActivity.setToolbarTitleVisible(false);
        TestHelper.assertViewNotVisible(mCarSetupWizardLayout.getToolbarTitle());
    }

    /**
     * Test that {@link BaseCompatActivity#setToolbarTitleText(String)} sets the toolbar title text.
     */
    @Test
    public void testSetToolbarTitleText() {
        mBaseCompatActivity.setToolbarTitleText("title text");
        TestHelper.assertTextEqual(mCarSetupWizardLayout.getToolbarTitle(), "title text");
    }

    /**
     * Test that a call to setToolbarTitleStyle sets the text appearance on the toolbar title.
     */
    @Test
    public void testSetToolbarStyle() {
        @StyleRes int newStyle = R.style.TextAppearance_Car_Body2;
        mCarSetupWizardLayout.setToolbarTitleStyle(newStyle);
        ShadowTextView shadowTextView = Shadows.shadowOf(mCarSetupWizardLayout.getToolbarTitle());
        assertThat(shadowTextView.getTextAppearanceId()).isEqualTo(newStyle);
    }

    /**
     * Test that {@link BaseCompatActivity#setPrimaryToolbarButtonVisible} sets the primary toolbar
     * button visible/not visible.
     */
    @Test
    public void testSetPrimaryToolbarButtonVisibleTrue() {
        mBaseCompatActivity.setPrimaryToolbarButtonVisible(true);
        TestHelper.assertViewVisible(mCarSetupWizardLayout.getPrimaryToolbarButton());
    }

    /**
     * Test that {@link BaseCompatActivity#setPrimaryToolbarButtonVisible} sets the primary toolbar
     * button visible/not visible.
     */
    @Test
    public void testSetPrimaryToolbarButtonVisibleFalse() {
        mBaseCompatActivity.setPrimaryToolbarButtonVisible(false);
        TestHelper.assertViewNotVisible(mCarSetupWizardLayout.getPrimaryToolbarButton());
    }

    /**
     * Test that {@link BaseCompatActivity#setPrimaryToolbarButtonEnabled} sets the primary toolbar
     * button visible/not visible.
     */
    @Test
    public void testSetPrimaryToolbarButtonEnabledTrue() {
        mBaseCompatActivity.setPrimaryToolbarButtonEnabled(true);
        TestHelper.assertViewEnabled(mCarSetupWizardLayout.getPrimaryToolbarButton());
    }

    /**
     * Test that {@link BaseCompatActivity#setPrimaryToolbarButtonEnabled} sets the primary toolbar
     * button visible/not visible.
     */
    @Test
    public void testSetPrimaryToolbarButtonEnabledFalse() {
        mBaseCompatActivity.setPrimaryToolbarButtonEnabled(false);
        TestHelper.assertViewNotEnabled(mCarSetupWizardLayout.getPrimaryToolbarButton());
    }

    /**
     * Test that {@link BaseCompatActivity#setPrimaryToolbarButtonText(String)} sets the primary
     * toolbar title text.
     */
    @Test
    public void testSetPrimaryToolbarButtonText() {
        mBaseCompatActivity.setPrimaryToolbarButtonText("button text");
        TestHelper.assertTextEqual(mCarSetupWizardLayout.getPrimaryToolbarButton(), "button text");
    }

    /**
     * Test that {@link BaseCompatActivity#setPrimaryToolbarButtonFlat(boolean)} sets the primary
     * toolbar button flat/not flat.
     */
    @Test
    public void testSetPrimaryToolbarButtonFlatTrue() {
        mBaseCompatActivity.setPrimaryToolbarButtonFlat(true);
        assertThat(mCarSetupWizardLayout.getPrimaryToolbarButtonFlat()).isTrue();
    }

    /**
     * Test that {@link BaseCompatActivity#setPrimaryToolbarButtonFlat(boolean)} sets the primary
     * toolbar button flat/not flat.
     */
    @Test
    public void testSetPrimaryToolbarButtonFlatFalse() {
        mBaseCompatActivity.setPrimaryToolbarButtonFlat(false);
        assertThat(mCarSetupWizardLayout.getPrimaryToolbarButtonFlat()).isFalse();
    }

    /**
     * Test that {@link BaseCompatActivity#setPrimaryToolbarButtonOnClickListener} sets the primary
     * toolbar button's click listener.
     */
    @Test
    public void testSetPrimaryToolbarButtonOnClickListener() {
        View.OnClickListener spyListener = TestHelper.createSpyListener();

        mBaseCompatActivity.setPrimaryToolbarButtonOnClickListener(spyListener);
        mBaseCompatActivity.getCarSetupWizardLayout().getPrimaryToolbarButton().performClick();
        verify(spyListener).onClick(Mockito.any());
    }

    /**
     * Test that {@link BaseCompatActivity#setSecondaryToolbarButtonVisible} sets the secondary
     * toolbar button visible/not visible.
     */
    @Test
    public void testSetSecondaryToolbarButtonVisibleTrue() {
        mBaseCompatActivity.setSecondaryToolbarButtonVisible(true);
        TestHelper.assertViewVisible(mCarSetupWizardLayout.getSecondaryToolbarButton());
    }

    /**
     * Test that {@link BaseCompatActivity#setSecondaryToolbarButtonVisible} sets the secondary
     * toolbar button visible/not visible.
     */
    @Test
    public void testSetSecondaryToolbarButtonVisibleFalse() {
        mBaseCompatActivity.setSecondaryToolbarButtonVisible(false);
        TestHelper.assertViewNotVisible(mCarSetupWizardLayout.getSecondaryToolbarButton());
    }

    /**
     * Test that {@link BaseCompatActivity#setSecondaryToolbarButtonEnabled} sets the secondary
     * toolbar button visible/not visible.
     */
    @Test
    public void testSetSecondaryToolbarButtonEnabledTrue() {
        mBaseCompatActivity.setSecondaryToolbarButtonEnabled(true);
        TestHelper.assertViewEnabled(mCarSetupWizardLayout.getSecondaryToolbarButton());
    }

    /**
     * Test that {@link BaseCompatActivity#setSecondaryToolbarButtonEnabled} sets the secondary
     * toolbar button visible/not visible.
     */
    @Test
    public void testSetSecondaryToolbarButtonEnabledFalse() {
        mBaseCompatActivity.setSecondaryToolbarButtonEnabled(false);
        TestHelper.assertViewNotEnabled(mCarSetupWizardLayout.getSecondaryToolbarButton());
    }

    /**
     * Test that {@link BaseCompatActivity#setSecondaryToolbarButtonText(String)} sets the secondary
     * toolbar title text.
     */
    @Test
    public void testSetSecondaryToolbarButtonText() {
        mBaseCompatActivity.setSecondaryToolbarButtonText("button text");
        TestHelper.assertTextEqual(mCarSetupWizardLayout.getSecondaryToolbarButton(),
                "button text");
    }

    /**
     * Test that {@link BaseCompatActivity#setSecondaryToolbarButtonOnClickListener} sets the
     * secondary toolbar button's click listener.
     */
    @Test
    public void testSetSecondaryToolbarButtonOnClickListener() {
        View.OnClickListener spyListener = TestHelper.createSpyListener();

        mBaseCompatActivity.setSecondaryToolbarButtonOnClickListener(spyListener);
        mBaseCompatActivity.getCarSetupWizardLayout().getSecondaryToolbarButton().performClick();
        verify(spyListener).onClick(Mockito.any());
    }

    /**
     * Test that {@link BaseCompatActivity#setProgressBarVisible} sets the progressbar visible/not
     * visible.
     */
    @Test
    public void testSetProgressBarVisibleTrue() {
        mBaseCompatActivity.setProgressBarVisible(true);
        TestHelper.assertViewVisible(mCarSetupWizardLayout.getProgressBar());
    }

    /**
     * Test that {@link BaseCompatActivity#setProgressBarVisible} sets the progressbar visible/not
     * visible.
     */
    @Test
    public void testSetProgressBarVisibleFalse() {
        mBaseCompatActivity.setProgressBarVisible(false);
        TestHelper.assertViewNotVisible(mCarSetupWizardLayout.getProgressBar());
    }

    @Test
    public void testBaseCompatActivityOnStart_startsDrivingMonitor() {
        mActivityController.start();
        assertThat(ShadowCar.hasConnected()).isTrue();
    }

    @Test
    public void testBaseCompatActivityOnStop_stopsDrivingMonitor() {
        mActivityController.start().stop();
        assertThat(ShadowCar.hasDisconnected()).isFalse();
    }

    @Test
    public void testNextActionTwice_onlyTriggersOneStartActivity() {
        BaseCompatActivity spyBaseCompatActivity = createSpyBaseCompatActivity();
        spyBaseCompatActivity.nextAction(Activity.RESULT_OK);
        spyBaseCompatActivity.nextAction(Activity.RESULT_OK);
        verify(spyBaseCompatActivity, times(1)).startActivity(Mockito.any());
    }

    @Test
    public void testNextActionCanBeTriggeredAgain_onResume() {
        BaseCompatActivity spyBaseCompatActivity = createSpyBaseCompatActivity();
        spyBaseCompatActivity.onResume();
        spyBaseCompatActivity.nextAction(Activity.RESULT_OK);
        spyBaseCompatActivity.onPause();
        spyBaseCompatActivity.onResume();
        spyBaseCompatActivity.nextAction(Activity.RESULT_OK);
        verify(spyBaseCompatActivity, times(2)).startActivity(Mockito.any());
    }
}
