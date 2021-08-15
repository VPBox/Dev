package com.android.server.wifi;

import static org.mockito.Matchers.anyInt;
import static org.mockito.Matchers.anyString;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

import android.security.KeyStore;
import android.util.SparseArray;

import org.mockito.Matchers;
import org.mockito.invocation.InvocationOnMock;
import org.mockito.stubbing.Answer;

import java.util.Arrays;
import java.util.HashMap;

class MockKeyStore {

    public static class KeyBlob {
        public byte[] blob;
        public int flag;

        public void update(byte[] blob, int flag) {
            this.blob = Arrays.copyOf(blob, blob.length);
            this.flag = flag;
        }
    }
    private SparseArray<HashMap<String, KeyBlob>> mStore;

    public MockKeyStore() {
        mStore = new SparseArray<HashMap<String, KeyBlob>>();
    }

    public KeyStore createMock() {
        KeyStore mock = mock(KeyStore.class);
        when(mock.state()).thenReturn(KeyStore.State.UNLOCKED);

        when(mock.put(anyString(), Matchers.any(byte[].class), anyInt(), anyInt()))
                .thenAnswer(new Answer<Boolean>() {

                    @Override
                    public Boolean answer(InvocationOnMock invocation) throws Throwable {
                        Object[] args = invocation.getArguments();
                        return put((String) args[0], (byte[]) args[1], (Integer) args[2],
                                (Integer) args[3]);
                    }
                });

        when(mock.importKey(anyString(), Matchers.any(byte[].class), anyInt(), anyInt()))
                .thenAnswer(new Answer<Boolean>() {

                    @Override
                    public Boolean answer(InvocationOnMock invocation) throws Throwable {
                        Object[] args = invocation.getArguments();
                        return importKey((String) args[0], (byte[]) args[1], (Integer) args[2],
                                (Integer) args[3]);
                    }
                });

        when(mock.delete(anyString(), anyInt())).thenAnswer(new Answer<Boolean>() {

            @Override
            public Boolean answer(InvocationOnMock invocation) throws Throwable {
                Object[] args = invocation.getArguments();
                return delete((String) args[0], (Integer) args[1]);
            }
        });

        when(mock.contains(anyString(), anyInt())).thenAnswer(new Answer<Boolean>() {

            @Override
            public Boolean answer(InvocationOnMock invocation) throws Throwable {
                Object[] args = invocation.getArguments();
                return contains((String) args[0], (Integer) args[1]);
            }
        });

        return mock;
    }

    private KeyBlob access(int uid, String key, boolean createIfNotExist) {
        if (mStore.get(uid) == null) {
            mStore.put(uid, new HashMap<String, KeyBlob>());
        }
        HashMap<String, KeyBlob> map = mStore.get(uid);
        if (map.containsKey(key)) {
            return map.get(key);
        } else {
            if (createIfNotExist) {
                KeyBlob blob = new KeyBlob();
                map.put(key, blob);
                return blob;
            } else {
                return null;
            }
        }
    }

    public KeyBlob getKeyBlob(int uid, String key) {
        return access(uid, key, false);
    }

    private boolean put(String key, byte[] value, int uid, int flags) {
        access(uid, key, true).update(value,  flags);
        return true;
    }

    private boolean importKey(String keyName, byte[] key, int uid, int flags) {
        return put(keyName, key, uid, flags);
    }

    private boolean delete(String key, int uid) {
        if (mStore.get(uid) != null) {
            mStore.get(uid).remove(key);
        }
        return true;
    }

    private boolean contains(String key, int uid) {
        return access(uid, key, false) != null;
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("KeyStore {");
        for (int i = 0; i < mStore.size(); i++) {
            int uid = mStore.keyAt(i);
            for (String keyName : mStore.get(uid).keySet()) {
                sb.append(String.format("%d:%s, ", uid, keyName));
            }
        }
        sb.append("}");
        return sb.toString();
    }
}
