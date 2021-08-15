/*
 * Copyright (C) 2016 The Android Open Source Project
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
package com.android.internal.telephony;

import android.media.ToneGenerator;
import android.test.suitebuilder.annotation.SmallTest;
import com.android.internal.telephony.cdma.SignalToneUtil;
import org.junit.Test;
import static org.junit.Assert.assertEquals;

public class SignalToneUtilTest {
    /* no need to initialization, everything is static */
    @Test
    @SmallTest
    public void testSignalToneUtil() {
        int audioToneResult = SignalToneUtil.getAudioToneFromSignalInfo(
                SignalToneUtil.IS95_CONST_IR_SIGNAL_IS54B,
                SignalToneUtil.IS95_CONST_IR_ALERT_LOW,
                SignalToneUtil.IS95_CONST_IR_SIG_IS54B_L);
        assertEquals(ToneGenerator.TONE_CDMA_LOW_L, audioToneResult);

        audioToneResult = SignalToneUtil.getAudioToneFromSignalInfo(
                SignalToneUtil.IS95_CONST_IR_SIGNAL_TONE,
                SignalToneUtil.IS95_CONST_IR_ALERT_LOW,
                SignalToneUtil.IS95_CONST_IR_SIG_TONE_DIAL);
        assertEquals(ToneGenerator.TONE_CDMA_DIAL_TONE_LITE, audioToneResult);
        /* if signal type is not IS54B, different alert pitch all map to
           SIGNAL_PITCH_UNKNOWN, expect get same result
         */
        audioToneResult = SignalToneUtil.getAudioToneFromSignalInfo(
                SignalToneUtil.IS95_CONST_IR_SIGNAL_TONE,
                SignalToneUtil.IS95_CONST_IR_ALERT_HIGH,
                SignalToneUtil.IS95_CONST_IR_SIG_TONE_DIAL);
        assertEquals(ToneGenerator.TONE_CDMA_DIAL_TONE_LITE, audioToneResult);
        audioToneResult = SignalToneUtil.getAudioToneFromSignalInfo(
                SignalToneUtil.IS95_CONST_IR_SIGNAL_TONE,
                SignalToneUtil.TAPIAMSSCDMA_SIGNAL_PITCH_UNKNOWN,
                SignalToneUtil.IS95_CONST_IR_SIG_TONE_DIAL);
        assertEquals(ToneGenerator.TONE_CDMA_DIAL_TONE_LITE, audioToneResult);
    }

    @SmallTest
    @Test
    public void testSignalToneUtilInvalidInput() {
        int audioToneResult = SignalToneUtil.getAudioToneFromSignalInfo(
                SignalToneUtil.CDMA_INVALID_TONE,
                SignalToneUtil.IS95_CONST_IR_ALERT_HIGH,
                SignalToneUtil.IS95_CONST_IR_SIG_TONE_DIAL);
        assertEquals(SignalToneUtil.CDMA_INVALID_TONE, audioToneResult);

        audioToneResult = SignalToneUtil.getAudioToneFromSignalInfo(
                SignalToneUtil.IS95_CONST_IR_SIGNAL_TONE,
                SignalToneUtil.CDMA_INVALID_TONE,
                SignalToneUtil.IS95_CONST_IR_SIG_TONE_DIAL);
        assertEquals(SignalToneUtil.CDMA_INVALID_TONE, audioToneResult);

        audioToneResult = SignalToneUtil.getAudioToneFromSignalInfo(
                SignalToneUtil.IS95_CONST_IR_SIGNAL_TONE,
                SignalToneUtil.IS95_CONST_IR_ALERT_HIGH,
                SignalToneUtil.CDMA_INVALID_TONE);
        assertEquals(SignalToneUtil.CDMA_INVALID_TONE, audioToneResult);
    }
}
