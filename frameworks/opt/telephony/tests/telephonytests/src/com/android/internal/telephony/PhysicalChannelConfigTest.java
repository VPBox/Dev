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
package com.android.internal.telephony;

import static com.google.common.truth.Truth.assertThat;

import android.os.Parcel;
import android.telephony.PhysicalChannelConfig;
import android.telephony.PhysicalChannelConfig.Builder;
import android.telephony.ServiceState;

import org.junit.Test;

/** Unit test for {@link android.telephony.PhysicalChannelConfig}. */
public class PhysicalChannelConfigTest {

    private static final int RAT = ServiceState.RIL_RADIO_TECHNOLOGY_LTE;
    private static final int CONNECTION_STATUS = PhysicalChannelConfig.CONNECTION_PRIMARY_SERVING;
    private static final int CELL_BANDWIDTH = 12345;
    private static final int FREQUENCY_RANGE = 1;
    private static final int CHANNEL_NUMBER = 1234;
    private static final int[] CONTEXT_IDS = new int[] {123, 555, 1, 0};
    private static final int PHYSICAL_CELL_ID = 502;

    @Test
    public void testBuilder() {
        PhysicalChannelConfig config = new Builder()
                .setRat(RAT)
                .setCellConnectionStatus(CONNECTION_STATUS)
                .setCellBandwidthDownlinkKhz(CELL_BANDWIDTH)
                .setFrequencyRange(FREQUENCY_RANGE)
                .setChannelNumber(CHANNEL_NUMBER)
                .setContextIds(CONTEXT_IDS)
                .setPhysicalCellId(PHYSICAL_CELL_ID)
                .build();

        assertThat(config.getRat()).isEqualTo(RAT);
        assertThat(config.getConnectionStatus()).isEqualTo(CONNECTION_STATUS);
        assertThat(config.getCellBandwidthDownlink()).isEqualTo(CELL_BANDWIDTH);
        assertThat(config.getFrequencyRange()).isEqualTo(FREQUENCY_RANGE);
        assertThat(config.getChannelNumber()).isEqualTo(CHANNEL_NUMBER);
        assertThat(config.getContextIds()).isEqualTo(CONTEXT_IDS);
        assertThat(config.getPhysicalCellId()).isEqualTo(PHYSICAL_CELL_ID);
    }

    @Test
    public void testParcel() {
        PhysicalChannelConfig config = new Builder()
                .setRat(RAT)
                .setCellConnectionStatus(CONNECTION_STATUS)
                .setCellBandwidthDownlinkKhz(CELL_BANDWIDTH)
                .setFrequencyRange(FREQUENCY_RANGE)
                .setChannelNumber(CHANNEL_NUMBER)
                .setContextIds(CONTEXT_IDS)
                .setPhysicalCellId(PHYSICAL_CELL_ID)
                .build();

        Parcel parcel = Parcel.obtain();
        config.writeToParcel(parcel, 0 /* flags */);
        parcel.setDataPosition(0);

        PhysicalChannelConfig fromParcel = PhysicalChannelConfig.CREATOR.createFromParcel(parcel);

        assertThat(fromParcel).isEqualTo(config);
    }
}
