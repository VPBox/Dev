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

package com.android.internal.telephony.uicc.euicc.apdu;

import static org.mockito.ArgumentMatchers.anyInt;
import static org.mockito.ArgumentMatchers.anyString;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.doAnswer;

import android.annotation.Nullable;
import android.os.AsyncResult;
import android.os.Message;

import com.android.internal.telephony.CommandsInterface;
import com.android.internal.telephony.uicc.IccIoResult;
import com.android.internal.telephony.uicc.IccUtils;

import org.mockito.ArgumentCaptor;
import org.mockito.invocation.InvocationOnMock;
import org.mockito.stubbing.Answer;

/** Utility to set up mocks for communication with UICC through logical channel. */
public final class LogicalChannelMocker {
    private static final int LOGICAL_CHANNEL = 1;

    /**
     * @param responseObject Can be either a string or an exception.
     * @return The mock channel number.
     */
    public static int mockOpenLogicalChannelResponse(CommandsInterface mockCi,
            @Nullable Object responseObject) {
        boolean isException = responseObject instanceof Throwable;
        int[] responseInts = isException ? null : getSelectResponse(responseObject.toString());
        Throwable exception = isException ? (Throwable) responseObject : null;

        ArgumentCaptor<Message> response = ArgumentCaptor.forClass(Message.class);
        doAnswer((Answer<Void>) invocation -> {
            Message msg = response.getValue();
            AsyncResult.forMessage(msg, responseInts, exception);
            msg.sendToTarget();
            return null;
        }).when(mockCi).iccOpenLogicalChannel(anyString(), anyInt(), response.capture());
        return LOGICAL_CHANNEL;
    }

    /**
     * @param responseObjects Can be either a string or an exception. For string, the last 4
     *         digits are the status (sw1, sw1).
     */
    public static void mockSendToLogicalChannel(CommandsInterface mockCi, int channel,
            Object... responseObjects) {
        ArgumentCaptor<Message> response = ArgumentCaptor.forClass(Message.class);
        doAnswer(new Answer() {
            private int mIndex = 0;

            @Override
            public Object answer(InvocationOnMock invocation) throws Throwable {
                Object responseObject = responseObjects[mIndex++];
                boolean isException = responseObject instanceof Throwable;
                int sw1 = 0;
                int sw2 = 0;
                String hex = responseObject.toString();
                if (!isException) {
                    int l = hex.length();
                    sw1 = Integer.parseInt(hex.substring(l - 4, l - 2), 16);
                    sw2 = Integer.parseInt(hex.substring(l - 2), 16);
                    hex = hex.substring(0, l - 4);
                }
                IccIoResult result = isException ? null : new IccIoResult(sw1, sw2, hex);
                Throwable exception = isException ? (Throwable) responseObject : null;

                Message msg = response.getValue();
                AsyncResult.forMessage(msg, result, exception);
                msg.sendToTarget();
                return null;
            }
        }).when(mockCi).iccTransmitApduLogicalChannel(eq(channel), anyInt(), anyInt(), anyInt(),
                anyInt(), anyInt(), anyString(), response.capture());
    }

    public static void mockCloseLogicalChannel(CommandsInterface mockCi, int channel) {
        ArgumentCaptor<Message> response = ArgumentCaptor.forClass(Message.class);
        doAnswer((Answer<Void>) invocation -> {
            Message msg = response.getValue();
            AsyncResult.forMessage(msg);
            msg.sendToTarget();
            return null;
        }).when(mockCi).iccCloseLogicalChannel(eq(channel), response.capture());
    }

    private static int[] getSelectResponse(String responseHex) {
        byte[] responseBytes = IccUtils.hexStringToBytes("00" + responseHex);
        int[] responseInts = new int[responseBytes.length];
        responseInts[0] = LOGICAL_CHANNEL;
        for (int i = 1; i < responseInts.length; ++i) {
            responseInts[i] = responseBytes[i];
        }
        return responseInts;
    }

    private LogicalChannelMocker() {
    }
}
