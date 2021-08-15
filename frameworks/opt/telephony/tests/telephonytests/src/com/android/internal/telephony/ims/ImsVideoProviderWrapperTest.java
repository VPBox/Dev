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

package com.android.internal.telephony.ims;

import static org.mockito.ArgumentMatchers.anyInt;
import static org.mockito.Mockito.when;

import android.telecom.VideoProfile;

import androidx.test.filters.SmallTest;
import androidx.test.runner.AndroidJUnit4;

import com.android.ims.internal.ImsVideoCallProviderWrapper;
import com.android.ims.internal.VideoPauseTracker;

import junit.framework.TestCase;

import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

/**
 * Tests for the {@link com.android.ims.internal.VideoPauseTracker} class.
 */
@RunWith(AndroidJUnit4.class)
public class ImsVideoProviderWrapperTest extends TestCase {
    private ImsVideoCallProviderWrapper mImsVideoCallProviderWrapper;
    @Mock
    VideoPauseTracker mVideoPauseTracker;

    @Before
    @Override
    public void setUp() throws Exception {
        super.setUp();
        MockitoAnnotations.initMocks(this);
        mImsVideoCallProviderWrapper = new ImsVideoCallProviderWrapper(null, mVideoPauseTracker);
        when(mVideoPauseTracker.shouldPauseVideoFor(anyInt())).thenReturn(true);
        when(mVideoPauseTracker.shouldResumeVideoFor(anyInt())).thenReturn(true);
    }

    @SmallTest
    @Test
    public void testIsPause() {
        assertTrue(ImsVideoCallProviderWrapper.isPauseRequest(VideoProfile.STATE_BIDIRECTIONAL,
                VideoProfile.STATE_BIDIRECTIONAL | VideoProfile.STATE_PAUSED));
        assertTrue(ImsVideoCallProviderWrapper.isPauseRequest(VideoProfile.STATE_BIDIRECTIONAL,
                VideoProfile.STATE_PAUSED));
        assertFalse(ImsVideoCallProviderWrapper.isPauseRequest(VideoProfile.STATE_PAUSED,
                VideoProfile.STATE_PAUSED));
        assertFalse(ImsVideoCallProviderWrapper.isPauseRequest(VideoProfile.STATE_AUDIO_ONLY,
                VideoProfile.STATE_AUDIO_ONLY));
    }

    @SmallTest
    @Test
    public void testIsResume() {
        assertTrue(ImsVideoCallProviderWrapper.isResumeRequest(
                VideoProfile.STATE_BIDIRECTIONAL | VideoProfile.STATE_PAUSED,
                VideoProfile.STATE_BIDIRECTIONAL));
        assertTrue(ImsVideoCallProviderWrapper.isResumeRequest(VideoProfile.STATE_PAUSED,
                VideoProfile.STATE_AUDIO_ONLY));
        assertFalse(ImsVideoCallProviderWrapper.isResumeRequest(
                VideoProfile.STATE_BIDIRECTIONAL | VideoProfile.STATE_PAUSED,
                VideoProfile.STATE_BIDIRECTIONAL | VideoProfile.STATE_PAUSED));
        assertFalse(ImsVideoCallProviderWrapper.isResumeRequest(VideoProfile.STATE_PAUSED,
                VideoProfile.STATE_AUDIO_ONLY | VideoProfile.STATE_PAUSED));
    }

    @SmallTest
    @Test
    public void testIsTurnOffCameraRequest() {
        assertTrue(ImsVideoCallProviderWrapper.isTurnOffCameraRequest(
                VideoProfile.STATE_BIDIRECTIONAL, VideoProfile.STATE_RX_ENABLED));
        assertTrue(ImsVideoCallProviderWrapper.isTurnOffCameraRequest(
                VideoProfile.STATE_BIDIRECTIONAL | VideoProfile.STATE_PAUSED,
                VideoProfile.STATE_RX_ENABLED));
        assertFalse(ImsVideoCallProviderWrapper.isTurnOffCameraRequest(
                VideoProfile.STATE_BIDIRECTIONAL | VideoProfile.STATE_PAUSED,
                VideoProfile.STATE_BIDIRECTIONAL));
    }

    @SmallTest
    @Test
    public void testIsTurnOnCameraRequest() {
        assertTrue(ImsVideoCallProviderWrapper.isTurnOnCameraRequest(
                VideoProfile.STATE_RX_ENABLED, VideoProfile.STATE_BIDIRECTIONAL));
        assertTrue(ImsVideoCallProviderWrapper.isTurnOnCameraRequest(
                VideoProfile.STATE_RX_ENABLED,
                VideoProfile.STATE_BIDIRECTIONAL | VideoProfile.STATE_PAUSED));
        assertFalse(ImsVideoCallProviderWrapper.isTurnOnCameraRequest(
                VideoProfile.STATE_BIDIRECTIONAL,
                VideoProfile.STATE_BIDIRECTIONAL | VideoProfile.STATE_PAUSED));
    }

    /**
     * Verifies that the to profile is not changed when a request to turn off the camera is sent
     * using the broken vendor-format request.
     */
    @SmallTest
    @Test
    public void testNoFilterWhenDisablingCamera() {
        mImsVideoCallProviderWrapper.setUseVideoPauseWorkaround(true);
        VideoProfile fromProfile = new VideoProfile(
                VideoProfile.STATE_BIDIRECTIONAL | VideoProfile.STATE_PAUSED);
        VideoProfile toProfile = new VideoProfile(VideoProfile.STATE_RX_ENABLED);

        VideoProfile filteredTo = mImsVideoCallProviderWrapper.maybeFilterPauseResume(fromProfile,
                toProfile, VideoPauseTracker.SOURCE_INCALL);
        assertEquals(filteredTo.getVideoState(), toProfile.getVideoState());
    }

    /**
     * Verifies that the to profile is not changed when a request to turn on the camera is sent
     * using the broken vendor-format request.
     */
    @SmallTest
    @Test
    public void testNoFilterWhenEnablingCamera() {
        mImsVideoCallProviderWrapper.setUseVideoPauseWorkaround(true);
        VideoProfile fromProfile = new VideoProfile(
                VideoProfile.STATE_RX_ENABLED | VideoProfile.STATE_PAUSED);
        VideoProfile toProfile = new VideoProfile(VideoProfile.STATE_BIDIRECTIONAL);

        VideoProfile filteredTo = mImsVideoCallProviderWrapper.maybeFilterPauseResume(fromProfile,
                toProfile, VideoPauseTracker.SOURCE_INCALL);
        assertEquals(filteredTo.getVideoState(), toProfile.getVideoState());
    }

    /**
     * Verifies normal operation of filtering of pause request.
     */
    @SmallTest
    @Test
    public void testNoFilteringOnPause() {
        VideoProfile fromProfile = new VideoProfile(VideoProfile.STATE_BIDIRECTIONAL);
        VideoProfile toProfile = new VideoProfile(
                VideoProfile.STATE_BIDIRECTIONAL | VideoProfile.STATE_PAUSED);

        VideoProfile filteredTo = mImsVideoCallProviderWrapper.maybeFilterPauseResume(fromProfile,
                toProfile, VideoPauseTracker.SOURCE_INCALL);
        assertEquals(filteredTo.getVideoState(), toProfile.getVideoState());
    }

    /**
     * Verifies normal operation of filtering of pause request.
     */
    @SmallTest
    @Test
    public void testNoFilteringOnResume() {
        VideoProfile fromProfile = new VideoProfile(
                VideoProfile.STATE_BIDIRECTIONAL | VideoProfile.STATE_PAUSED);
        VideoProfile toProfile = new VideoProfile(VideoProfile.STATE_BIDIRECTIONAL);

        VideoProfile filteredTo = mImsVideoCallProviderWrapper.maybeFilterPauseResume(fromProfile,
                toProfile, VideoPauseTracker.SOURCE_INCALL);
        assertEquals(filteredTo.getVideoState(), toProfile.getVideoState());
    }
}

