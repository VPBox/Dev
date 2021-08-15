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

package com.android.server.wifi.hotspot2.anqp;

import java.io.ByteArrayOutputStream;
import java.io.IOException;

/**
 * Utility class for formatting IEI (Information Element Identity) data for testing.
 */
public class CellularNetworkTestUtil {
    /**
     * Format and return PLMN List IEI with the given PLMN list data.
     *
     * @param plmnList The array of PLMN data
     * @return byte[]
     * @throws IOException
     */
    public static byte[] formatPLMNListIEI(byte[][] plmnList) throws IOException {
        return formatPLMNListIEI(CellularNetwork.IEI_TYPE_PLMN_LIST, plmnList);
    }

    /**
     * Format and return PLMN List IEI with the given IEI type and PLMN list data.  This
     * allows the test to use an invalid IEI type for testing purpose.
     *
     * @param ieiType The IEI type
     * @param plmnList The array of PLMN data
     * @return byte[]
     * @throws IOException
     */
    public static byte[] formatPLMNListIEI(int ieiType, byte[][] plmnList) throws IOException {
        return formatPLMNListIEI(ieiType, plmnList, false);
    }

    /**
     * Format and return PLMN List IEI with the given IEI type and PLMN list data.  This also
     * allows the test to intentionally setting an incorrect size value.
     *
     * @param ieiType The IEI type
     * @param plmnList The array of PLMN data
     * @param setWrongSize Flag for setting incorrect IEI size
     * @return byte[]
     * @throws IOException
     */
    public static byte[] formatPLMNListIEI(int ieiType, byte[][] plmnList, boolean setWrongSize)
            throws IOException {
        ByteArrayOutputStream stream = new ByteArrayOutputStream();

        // Calculate the total bytes for all the PLMNs.
        int plmnsSize = getDataSize(plmnList);

        // Use incorrect size intentionally.
        if (setWrongSize) plmnsSize -= 1;

        stream.write((byte) ieiType);
        // One extra byte for the PLMN count field.
        stream.write((byte) ((plmnsSize + 1) & CellularNetwork.IEI_CONTENT_LENGTH_MASK));
        stream.write((byte) plmnList.length);
        for (byte[] plmn : plmnList) {
            stream.write(plmn);
        }

        return stream.toByteArray();
    }

    /**
     * Return the number of bytes in a 2D array.
     *
     * @param dataArray The 2D array
     * @return The number of bytes in the 2D array
     */
    public static int getDataSize(byte[][] dataArray) {
        int size = 0;
        for (byte[] data : dataArray) {
            size += data.length;
        }
        return size;
    }
}
