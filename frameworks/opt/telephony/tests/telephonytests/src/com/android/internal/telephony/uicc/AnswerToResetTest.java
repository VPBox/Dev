/*
 * Copyright 2018 The Android Open Source Project
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
package com.android.internal.telephony.uicc;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;

import android.test.suitebuilder.annotation.SmallTest;

import org.junit.Test;

import java.util.ArrayList;
import java.util.Arrays;


public class AnswerToResetTest {

    @Test
    @SmallTest
    public void testAnswerToRestNullString() {
        AnswerToReset atr = AnswerToReset.parseAtr(null);
        assertNull(atr);
    }

    @Test
    @SmallTest
    public void testAnswerToRestOddLength() {
        String str = "3B02145";
        AnswerToReset atr = AnswerToReset.parseAtr(str);
        assertNull(atr);
    }

    @Test
    @SmallTest
    public void testAnswerToRestTooShortLength() {
        String str = "3B";
        AnswerToReset atr = AnswerToReset.parseAtr(str);
        assertNull(atr);
    }

    @Test
    @SmallTest
    public void testAnswerToRestNoInterfaceByteNoHistoricalByte() {
        String str = "3B00";
        AnswerToReset atr = AnswerToReset.parseAtr(str);
        assertNotNull(atr);
        assertEquals(atr.getConventionByte(), (byte) 0x3B);
        assertEquals(atr.getFormatByte(), (byte) 0x00);
        assertTrue(atr.getInterfaceBytes().isEmpty());
        assertNull(atr.getHistoricalBytes());
        assertNull(atr.getCheckByte());
    }

    @Test
    @SmallTest
    public void testAnswerToRestNoHistoricalByte() {
        String str = "3F909580B1FE001F4297";
        AnswerToReset atr = AnswerToReset.parseAtr(str);
        assertNotNull(atr);
        assertEquals(atr.getConventionByte(), (byte) 0x3F);
        assertEquals(atr.getFormatByte(), (byte) 0x90);

        assertEquals(atr.getInterfaceBytes().size(), 4);
        AnswerToReset.InterfaceByte expect_t1 =
                new AnswerToReset.InterfaceByte((byte) 0x95, null, null, (byte) 0x80);
        AnswerToReset.InterfaceByte expect_t2 =
                new AnswerToReset.InterfaceByte(null, null, null, (byte) 0xB1);
        AnswerToReset.InterfaceByte expect_t3 =
                new AnswerToReset.InterfaceByte((byte) 0xFE, (byte) 0x00, null, (byte) 0x1F);
        AnswerToReset.InterfaceByte expect_t4 =
                new AnswerToReset.InterfaceByte((byte) 0x42, null, null, null);
        ArrayList<AnswerToReset.InterfaceByte> expect = new ArrayList<>(
                Arrays.asList(expect_t1, expect_t2, expect_t3, expect_t4)
        );
        assertEquals(expect, atr.getInterfaceBytes());

        assertNull(atr.getHistoricalBytes());
        assertEquals(atr.getCheckByte(), Byte.valueOf((byte) 0x97));
    }

    @Test
    @SmallTest
    public void testAnswerToRestNoInterfaceByte() {
        String str = "3F078031A073BE211797";
        AnswerToReset atr = AnswerToReset.parseAtr(str);
        assertNotNull(atr);
        assertEquals(atr.getConventionByte(), (byte) 0x3F);
        assertEquals(atr.getFormatByte(), (byte) 0x07);
        assertTrue(atr.getInterfaceBytes().isEmpty());
        assertEquals(atr.getHistoricalBytes().getRawData().length, 7);
        byte[] expect = new byte[]{
                (byte) 0x80, (byte) 0x31, (byte) 0xA0, (byte) 0x73,
                (byte) 0xBE, (byte) 0x21, (byte) 0x17};
        assertTrue(Arrays.equals(atr.getHistoricalBytes().getRawData(), expect));
        assertEquals(atr.getCheckByte(), Byte.valueOf((byte) 0x97));
    }

    @Test
    @SmallTest
    public void testAnswerToRestSuccess() {
        String str = "3F979580B1FE001F428031A073BE211797";
        AnswerToReset atr = AnswerToReset.parseAtr(str);
        assertNotNull(atr);
        assertEquals(atr.getConventionByte(), (byte) 0x3F);
        assertEquals(atr.getFormatByte(), (byte) 0x97);

        assertEquals(atr.getInterfaceBytes().size(), 4);
        AnswerToReset.InterfaceByte expect_t1 =
                new AnswerToReset.InterfaceByte((byte) 0x95, null, null, (byte) 0x80);
        AnswerToReset.InterfaceByte expect_t2 =
                new AnswerToReset.InterfaceByte(null, null, null, (byte) 0xB1);
        AnswerToReset.InterfaceByte expect_t3 =
                new AnswerToReset.InterfaceByte((byte) 0xFE, (byte) 0x00, null, (byte) 0x1F);
        AnswerToReset.InterfaceByte expect_t4 =
                new AnswerToReset.InterfaceByte((byte) 0x42, null, null, null);
        ArrayList<AnswerToReset.InterfaceByte> expect_ib = new ArrayList<>(
                Arrays.asList(expect_t1, expect_t2, expect_t3, expect_t4)
        );
        assertEquals(expect_ib, atr.getInterfaceBytes());
        assertEquals(atr.getHistoricalBytes().getRawData().length, 7);
        byte[] expect_hb = new byte[]{
                (byte) 0x80, (byte) 0x31, (byte) 0xA0, (byte) 0x73,
                (byte) 0xBE, (byte) 0x21, (byte) 0x17};
        assertTrue(Arrays.equals(atr.getHistoricalBytes().getRawData(), expect_hb));
        assertEquals(atr.getCheckByte(), Byte.valueOf((byte) 0x97));
    }

    @Test
    @SmallTest
    public void testAnswerToRestSuccessWithoutCheckByte() {
        String str = "3F979580B0FE0010428031A073BE2117";
        AnswerToReset atr = AnswerToReset.parseAtr(str);
        assertNotNull(atr);
        assertEquals(atr.getConventionByte(), (byte) 0x3F);
        assertEquals(atr.getFormatByte(), (byte) 0x97);

        assertEquals(atr.getInterfaceBytes().size(), 4);
        AnswerToReset.InterfaceByte expect_t1 =
                new AnswerToReset.InterfaceByte((byte) 0x95, null, null, (byte) 0x80);
        AnswerToReset.InterfaceByte expect_t2 =
                new AnswerToReset.InterfaceByte(null, null, null, (byte) 0xB0);
        AnswerToReset.InterfaceByte expect_t3 =
                new AnswerToReset.InterfaceByte((byte) 0xFE, (byte) 0x00, null, (byte) 0x10);
        AnswerToReset.InterfaceByte expect_t4 =
                new AnswerToReset.InterfaceByte((byte) 0x42, null, null, null);
        ArrayList<AnswerToReset.InterfaceByte> expect_ib = new ArrayList<>(
                Arrays.asList(expect_t1, expect_t2, expect_t3, expect_t4)
        );
        assertEquals(expect_ib, atr.getInterfaceBytes());

        assertEquals(atr.getHistoricalBytes().getRawData().length, 7);
        byte[] expect_hb = new byte[]{
                (byte) 0x80, (byte) 0x31, (byte) 0xA0, (byte) 0x73,
                (byte) 0xBE, (byte) 0x21, (byte) 0x17};
        assertTrue(Arrays.equals(atr.getHistoricalBytes().getRawData(), expect_hb));

        assertEquals(atr.getCheckByte(), null);
        assertFalse(atr.isEuiccSupported());
    }

    @Test
    @SmallTest
    public void testAnswerToRestFailWithoutCheckByte() {
        String str = "3F979581B0FE0010428031A073BE2117";
        AnswerToReset atr = AnswerToReset.parseAtr(str);
        assertNull(atr);
    }

    @Test
    @SmallTest
    public void testAnswerToRestFailWithExtraByte() {
        String str = "3F979580B1FE001F428031A073BE21179718";
        AnswerToReset atr = AnswerToReset.parseAtr(str);
        assertNull(atr);
    }

    @Test
    @SmallTest
    public void testAnswerToRestEuiccSupported() {
        String str = "3F979580BFFE8210428031A073BE211797";
        AnswerToReset atr = AnswerToReset.parseAtr(str);
        assertNotNull(atr);
        assertEquals(atr.getConventionByte(), (byte) 0x3F);
        assertEquals(atr.getFormatByte(), (byte) 0x97);

        assertEquals(atr.getInterfaceBytes().size(), 4);
        AnswerToReset.InterfaceByte expect_t1 =
                new AnswerToReset.InterfaceByte((byte) 0x95, null, null, (byte) 0x80);
        AnswerToReset.InterfaceByte expect_t2 =
                new AnswerToReset.InterfaceByte(null, null, null, (byte) 0xBF);
        AnswerToReset.InterfaceByte expect_t3 =
                new AnswerToReset.InterfaceByte((byte) 0xFE, (byte) 0x82, null, (byte) 0x10);
        AnswerToReset.InterfaceByte expect_t4 =
                new AnswerToReset.InterfaceByte((byte) 0x42, null, null, null);
        ArrayList<AnswerToReset.InterfaceByte> expect_ib = new ArrayList<>(
                Arrays.asList(expect_t1, expect_t2, expect_t3, expect_t4)
        );
        assertEquals(expect_ib, atr.getInterfaceBytes());

        assertEquals(atr.getHistoricalBytes().getRawData().length, 7);
        byte[] expect_hb = new byte[]{
                (byte) 0x80, (byte) 0x31, (byte) 0xA0, (byte) 0x73,
                (byte) 0xBE, (byte) 0x21, (byte) 0x17};
        assertTrue(Arrays.equals(atr.getHistoricalBytes().getRawData(), expect_hb));

        assertEquals(atr.getCheckByte(), Byte.valueOf((byte) 0x97));

        assertTrue(atr.isEuiccSupported());
    }

    @Test
    @SmallTest
    public void testAnswerToRestEuiccSupportedWithLowerCaseString() {
        String str = "3f979580bffe8210428031a073be211797";
        AnswerToReset atr = AnswerToReset.parseAtr(str);
        assertNotNull(atr);
        assertEquals(atr.getConventionByte(), (byte) 0x3F);
        assertEquals(atr.getFormatByte(), (byte) 0x97);

        assertEquals(atr.getInterfaceBytes().size(), 4);
        AnswerToReset.InterfaceByte expect_t1 =
                new AnswerToReset.InterfaceByte((byte) 0x95, null, null, (byte) 0x80);
        AnswerToReset.InterfaceByte expect_t2 =
                new AnswerToReset.InterfaceByte(null, null, null, (byte) 0xBF);
        AnswerToReset.InterfaceByte expect_t3 =
                new AnswerToReset.InterfaceByte((byte) 0xFE, (byte) 0x82, null, (byte) 0x10);
        AnswerToReset.InterfaceByte expect_t4 =
                new AnswerToReset.InterfaceByte((byte) 0x42, null, null, null);
        ArrayList<AnswerToReset.InterfaceByte> expect_ib = new ArrayList<>(
                Arrays.asList(expect_t1, expect_t2, expect_t3, expect_t4)
        );
        assertEquals(expect_ib, atr.getInterfaceBytes());

        assertEquals(atr.getHistoricalBytes().getRawData().length, 7);
        byte[] expect_hb = new byte[]{
            (byte) 0x80, (byte) 0x31, (byte) 0xA0, (byte) 0x73,
            (byte) 0xBE, (byte) 0x21, (byte) 0x17};
        assertTrue(Arrays.equals(atr.getHistoricalBytes().getRawData(), expect_hb));

        assertEquals(atr.getCheckByte(), Byte.valueOf((byte) 0x97));

        assertTrue(atr.isEuiccSupported());
    }

    @Test
    @SmallTest
    public void testAnswerToRestEuiccNotSupportedDueToIncorrectT() {
        String str = "3F979580BEFE8210428031A073BE211797";
        AnswerToReset atr = AnswerToReset.parseAtr(str);
        assertNotNull(atr);
        assertEquals(atr.getConventionByte(), (byte) 0x3F);
        assertEquals(atr.getFormatByte(), (byte) 0x97);

        assertEquals(atr.getInterfaceBytes().size(), 4);
        AnswerToReset.InterfaceByte expect_t1 =
                new AnswerToReset.InterfaceByte((byte) 0x95, null, null, (byte) 0x80);
        AnswerToReset.InterfaceByte expect_t2 =
                new AnswerToReset.InterfaceByte(null, null, null, (byte) 0xBE);
        AnswerToReset.InterfaceByte expect_t3 =
                new AnswerToReset.InterfaceByte((byte) 0xFE, (byte) 0x82, null, (byte) 0x10);
        AnswerToReset.InterfaceByte expect_t4 =
                new AnswerToReset.InterfaceByte((byte) 0x42, null, null, null);
        ArrayList<AnswerToReset.InterfaceByte> expect_ib = new ArrayList<>(
                Arrays.asList(expect_t1, expect_t2, expect_t3, expect_t4)
        );
        assertEquals(expect_ib, atr.getInterfaceBytes());

        assertEquals(atr.getHistoricalBytes().getRawData().length, 7);
        byte[] expect_hb = new byte[]{
                (byte) 0x80, (byte) 0x31, (byte) 0xA0, (byte) 0x73,
                (byte) 0xBE, (byte) 0x21, (byte) 0x17};
        assertTrue(Arrays.equals(atr.getHistoricalBytes().getRawData(), expect_hb));

        assertEquals(atr.getCheckByte(), Byte.valueOf((byte) 0x97));

        assertFalse(atr.isEuiccSupported());
    }

    @Test
    @SmallTest
    public void testAnswerToRestEuiccNotSupportedDueToIncorrectTB() {
        String str = "3F979580BFFE8110428031A073BE211797";
        AnswerToReset atr = AnswerToReset.parseAtr(str);
        assertNotNull(atr);
        assertEquals(atr.getConventionByte(), (byte) 0x3F);
        assertEquals(atr.getFormatByte(), (byte) 0x97);

        assertEquals(atr.getInterfaceBytes().size(), 4);
        AnswerToReset.InterfaceByte expect_t1 =
                new AnswerToReset.InterfaceByte((byte) 0x95, null, null, (byte) 0x80);
        AnswerToReset.InterfaceByte expect_t2 =
                new AnswerToReset.InterfaceByte(null, null, null, (byte) 0xBF);
        AnswerToReset.InterfaceByte expect_t3 =
                new AnswerToReset.InterfaceByte((byte) 0xFE, (byte) 0x81, null, (byte) 0x10);
        AnswerToReset.InterfaceByte expect_t4 =
                new AnswerToReset.InterfaceByte((byte) 0x42, null, null, null);
        ArrayList<AnswerToReset.InterfaceByte> expect_ib = new ArrayList<>(
                Arrays.asList(expect_t1, expect_t2, expect_t3, expect_t4)
        );
        assertEquals(expect_ib, atr.getInterfaceBytes());

        assertEquals(atr.getHistoricalBytes().getRawData().length, 7);
        byte[] expect_hb = new byte[]{
                (byte) 0x80, (byte) 0x31, (byte) 0xA0, (byte) 0x73,
                (byte) 0xBE, (byte) 0x21, (byte) 0x17};
        assertTrue(Arrays.equals(atr.getHistoricalBytes().getRawData(), expect_hb));

        assertEquals(atr.getCheckByte(), Byte.valueOf((byte) 0x97));

        assertFalse(atr.isEuiccSupported());
    }

    @Test
    @SmallTest
    public void testAnswerToResetExtendedApduSupported() {
        String str = "3B9F96803FC7828031E073F62158574A4D020C6030005F";
        AnswerToReset atr = AnswerToReset.parseAtr(str);
        assertTrue(atr.isExtendedApduSupported());

        str = "3B9F96803FC7828031E073F62182574A4D020C6030005F";
        atr = AnswerToReset.parseAtr(str);
        assertFalse(atr.isExtendedApduSupported());

        str = "3F9F96803FC7828031E073F62158574A4D020C6030005F";
        atr = AnswerToReset.parseAtr(str);
        assertFalse(atr.isExtendedApduSupported());

        str = "3F9F96803FC7828031E073F62182574A4D020C6030005F";
        atr = AnswerToReset.parseAtr(str);
        assertTrue(atr.isExtendedApduSupported());
    }

    @Test
    @SmallTest
    public void testAnswerToResetExtendedApduNotSupportedDueToNoTag() {
        String str = "3F6D000080318065B00501025E83009000";
        AnswerToReset atr = AnswerToReset.parseAtr(str);
        assertFalse(atr.isExtendedApduSupported());

        str = "3B6D000080318065B00501025E83009000";
        atr = AnswerToReset.parseAtr(str);
        assertFalse(atr.isExtendedApduSupported());
    }

    @Test
    @SmallTest
    public void testAnswerToResetExtendedApduNotSupportedDueToLessLength() {
        String str = "3B9E96803FC7828031E072F621574A4D020C6030005F";
        AnswerToReset atr = AnswerToReset.parseAtr(str);
        assertFalse(atr.isExtendedApduSupported());

        str = "3F9D96803FC7828031E071F6574A4D020C6030005F";
        atr = AnswerToReset.parseAtr(str);
        assertFalse(atr.isExtendedApduSupported());
    }

    @Test
    @SmallTest
    public void testAnswerToResetParseLtvNodeWithIncorrectLength() {
        String str = "3B9E96803FC7828031E073F621574A4D020C6030005F";
        AnswerToReset atr = AnswerToReset.parseAtr(str);
        assertNull(atr.getHistoricalBytes());

        str = "3B9E96803FC7828031E071F621574A4D020C6030005F";
        atr = AnswerToReset.parseAtr(str);
        assertNull(atr.getHistoricalBytes());
    }
}
