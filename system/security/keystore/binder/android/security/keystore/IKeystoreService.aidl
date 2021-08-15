/**
 * Copyright (c) 2018, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package android.security.keystore;

import android.security.keymaster.KeymasterArguments;
import android.security.keymaster.KeymasterBlob;
import android.security.keymaster.OperationResult;
import android.security.keystore.IKeystoreResponseCallback;
import android.security.keystore.IKeystoreKeyCharacteristicsCallback;
import android.security.keystore.IKeystoreExportKeyCallback;
import android.security.keystore.IKeystoreOperationResultCallback;
import android.security.keystore.IKeystoreCertificateChainCallback;

/**
 * @hide
 */
interface IKeystoreService {
    int getState(int userId);
    byte[] get(String name, int uid);
    int insert(String name, in byte[] item, int uid, int flags);
    int del(String name, int uid);
    int exist(String name, int uid);
    String[] list(String namePrefix, int uid);
    int reset();
    int onUserPasswordChanged(int userId, String newPassword);
    int lock(int userId);
    int unlock(int userId, String userPassword);
    int isEmpty(int userId);
    String grant(String name, int granteeUid);
    int ungrant(String name, int granteeUid);
    long getmtime(String name, int uid);
    int is_hardware_backed(String string);
    int clear_uid(long uid);

    int addRngEntropy(IKeystoreResponseCallback cb, in byte[] data, int flags);
    int generateKey(IKeystoreKeyCharacteristicsCallback cb, String alias, in KeymasterArguments arguments, in byte[] entropy, int uid,
        int flags);
    int getKeyCharacteristics (IKeystoreKeyCharacteristicsCallback cb, String alias, in KeymasterBlob clientId, in KeymasterBlob appData,
        int uid);
    int importKey(IKeystoreKeyCharacteristicsCallback cb, String alias, in KeymasterArguments arguments, int format,
        in byte[] keyData, int uid, int flags);
    int exportKey(IKeystoreExportKeyCallback cb, String alias, int format, in KeymasterBlob clientId,
        in KeymasterBlob appData, int uid);
    int begin(in IKeystoreOperationResultCallback cb, IBinder appToken, String alias, int purpose, boolean pruneable,
        in KeymasterArguments params, in byte[] entropy, int uid);
    int update(in IKeystoreOperationResultCallback cb, IBinder token, in KeymasterArguments params, in byte[] input);
    int finish(in IKeystoreOperationResultCallback cb, IBinder token, in KeymasterArguments params, in byte[] signature,
        in byte[] entropy);
    int abort(in IKeystoreResponseCallback cb, IBinder token);
    int addAuthToken(in byte[] authToken);
    int onUserAdded(int userId, int parentId);
    int onUserRemoved(int userId);
    int attestKey(in IKeystoreCertificateChainCallback cb, String alias, in KeymasterArguments params);
    int attestDeviceIds(in IKeystoreCertificateChainCallback cb, in KeymasterArguments params);
    int onDeviceOffBody();
    int importWrappedKey(in IKeystoreKeyCharacteristicsCallback cb, String wrappedKeyAlias, in byte[] wrappedKey,
        in String wrappingKeyAlias, in byte[] maskingKey, in KeymasterArguments arguments,
        in long rootSid, in long fingerprintSid);
    int presentConfirmationPrompt(IBinder listener, String promptText, in byte[] extraData,
        in String locale, in int uiOptionsAsFlags);
    int cancelConfirmationPrompt(IBinder listener);
    boolean isConfirmationPromptSupported();
    int onKeyguardVisibilityChanged(in boolean isShowing, in int userId);
    int listUidsOfAuthBoundKeys(out @utf8InCpp List<String> uids);
}
