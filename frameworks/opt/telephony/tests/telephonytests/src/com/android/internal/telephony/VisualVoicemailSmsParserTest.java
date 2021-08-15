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
 * limitations under the License
 */
package com.android.internal.telephony;

import android.test.suitebuilder.annotation.SmallTest;

import com.android.internal.telephony.VisualVoicemailSmsParser.WrappedMessageData;

import junit.framework.TestCase;

public class VisualVoicemailSmsParserTest extends TestCase {

    @SmallTest
    public void testParsing() {
        WrappedMessageData result = VisualVoicemailSmsParser.parse("//VVM",
                "//VVM:STATUS:st=R;rc=0;srv=1;dn=1;ipt=1;spt=0;u=eg@example.com;pw=1");

        assertEquals("STATUS", result.prefix);
        assertEquals("R", result.fields.getString("st"));
        assertEquals("0", result.fields.getString("rc"));
        assertEquals("1", result.fields.getString("srv"));
        assertEquals("1", result.fields.getString("dn"));
        assertEquals("1", result.fields.getString("ipt"));
        assertEquals("0", result.fields.getString("spt"));
        assertEquals("eg@example.com", result.fields.getString("u"));
        assertEquals("1", result.fields.getString("pw"));
    }

    @SmallTest
    public void testParsing_TrailingSemiColon() {
        WrappedMessageData result = VisualVoicemailSmsParser.parse("//VVM",
                "//VVM:STATUS:st=R;rc=0;srv=1;dn=1;ipt=1;spt=0;u=eg@example.com;pw=1;");

        assertEquals("STATUS", result.prefix);
        assertEquals("R", result.fields.getString("st"));
        assertEquals("0", result.fields.getString("rc"));
        assertEquals("1", result.fields.getString("srv"));
        assertEquals("1", result.fields.getString("dn"));
        assertEquals("1", result.fields.getString("ipt"));
        assertEquals("0", result.fields.getString("spt"));
        assertEquals("eg@example.com", result.fields.getString("u"));
        assertEquals("1", result.fields.getString("pw"));
    }

    @SmallTest
    public void testParsing_EmptyPrefix() {
        WrappedMessageData result = VisualVoicemailSmsParser.parse("//VVM",
                "//VVM::st=R;rc=0;srv=1;dn=1;ipt=1;spt=0;u=eg@example.com;pw=1");

        assertEquals("", result.prefix);
        assertEquals("R", result.fields.getString("st"));
        assertEquals("0", result.fields.getString("rc"));
        assertEquals("1", result.fields.getString("srv"));
        assertEquals("1", result.fields.getString("dn"));
        assertEquals("1", result.fields.getString("ipt"));
        assertEquals("0", result.fields.getString("spt"));
        assertEquals("eg@example.com", result.fields.getString("u"));
        assertEquals("1", result.fields.getString("pw"));
    }

    @SmallTest
    public void testParsing_EmptyField() {
        WrappedMessageData result = VisualVoicemailSmsParser.parse("//VVM",
                "//VVM:STATUS:");
        assertTrue(result.fields.isEmpty());
    }

    @SmallTest
    public void testParsingFail_NotVvm() {
        WrappedMessageData result = VisualVoicemailSmsParser.parse("//VVM",
                "helloworld");

        assertNull(result);
    }

    @SmallTest
    public void testParsingFail_PrefixMismatch() {
        WrappedMessageData result = VisualVoicemailSmsParser.parse("//VVM",
                "//ANDROIDVVM:STATUS:st=R;rc=0;srv=1;dn=1;ipt=1;spt=0;u=eg@example.com;pw=1");

        assertNull(result);
    }

    @SmallTest
    public void testParsingFail_MissingFirstColon() {
        WrappedMessageData result = VisualVoicemailSmsParser.parse("//VVM",
                "//VVMSTATUS:st=R;rc=0;srv=1;dn=1;ipt=1;spt=0;u=eg@example.com;pw=1");

        assertNull(result);
    }

    @SmallTest
    public void testParsingFail_MissingSecondColon() {
        WrappedMessageData result = VisualVoicemailSmsParser.parse("//VVM",
                "//VVM:STATUSst=R;rc=0;srv=1;dn=1;ipt=1;spt=0;u=eg@example.com;pw=1");

        assertNull(result);
    }

    @SmallTest
    public void testParsingFail_MessageEndAfterClientPrefix() {
        WrappedMessageData result = VisualVoicemailSmsParser.parse("//VVM",
                "//VVM:");

        assertNull(result);
    }

    @SmallTest
    public void testParsingFail_MessageEndAfterPrefix() {
        WrappedMessageData result = VisualVoicemailSmsParser.parse("//VVM",
                "//VVM:STATUS");

        assertNull(result);
    }

    @SmallTest
    public void testParsingFail_InvalidKeyValuePair() {
        WrappedMessageData result = VisualVoicemailSmsParser.parse("//VVM",
                "//VVM:STATUS:key");
        assertNull(result);
    }

    @SmallTest
    public void testParsingFail_InvalidMissingKey() {
        WrappedMessageData result = VisualVoicemailSmsParser.parse("//VVM",
                "//VVM:STATUS:=value");
        assertNull(result);
    }

    @SmallTest
    public void testParsingFail_MissingValue() {
        WrappedMessageData result = VisualVoicemailSmsParser.parse("//VVM",
                "//VVM:STATUS:key=");
        assertEquals("STATUS", result.prefix);
        assertEquals("", result.fields.getString("key"));
    }

    @SmallTest
    public void testAlternativeParsing_Mboxupdate() {
        WrappedMessageData result = VisualVoicemailSmsParser.parseAlternativeFormat(
            "MBOXUPDATE?m=1;server=example.com;port=143;name=foo@example.com;pw=bar");

        assertEquals("MBOXUPDATE", result.prefix);
        assertEquals("1", result.fields.getString("m"));
        assertEquals("example.com", result.fields.getString("server"));
        assertEquals("143", result.fields.getString("port"));
        assertEquals("foo@example.com", result.fields.getString("name"));
        assertEquals("bar", result.fields.getString("pw"));
    }

    @SmallTest
    public void testAlternativeParsing_Unrecognized() {
        WrappedMessageData result = VisualVoicemailSmsParser.parseAlternativeFormat(
            "UNRECOGNIZED?cmd=STATUS");

        assertEquals("UNRECOGNIZED", result.prefix);
        assertEquals("STATUS", result.fields.getString("cmd"));
    }

    @SmallTest
    public void testAlternativeParsingFail_MissingSeparator() {
        WrappedMessageData result = VisualVoicemailSmsParser.parseAlternativeFormat(
            "I send SMS in weird formats");

        assertNull(result);
    }

    @SmallTest
    public void testAlternativeParsingFail_NotWhitelistedEvent() {
        WrappedMessageData result = VisualVoicemailSmsParser.parseAlternativeFormat(
            "AreYouStillThere?");

        assertNull(result);
    }
}
