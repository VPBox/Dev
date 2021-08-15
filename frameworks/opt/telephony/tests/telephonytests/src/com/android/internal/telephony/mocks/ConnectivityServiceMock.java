/*
 * Copyright (C) 2008 The Android Open Source Project
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

package com.android.internal.telephony.mocks;

import static android.net.NetworkCapabilities.NET_CAPABILITY_INTERNET;
import static android.net.NetworkCapabilities.NET_CAPABILITY_NOT_RESTRICTED;

import android.app.PendingIntent;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.LinkProperties;
import android.net.NetworkCapabilities;
import android.net.NetworkFactory;
import android.net.NetworkInfo;
import android.net.NetworkMisc;
import android.net.NetworkRequest;
import android.os.Binder;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.IBinder;
import android.os.Looper;
import android.os.Message;
import android.os.Messenger;
import android.os.Process;
import android.os.RemoteException;
import android.util.Slog;

import com.android.internal.annotations.VisibleForTesting;
import com.android.internal.util.AsyncChannel;
import com.android.server.connectivity.NetworkAgentInfo;

import java.util.HashMap;

public class ConnectivityServiceMock {
    private static final String TAG = "ConnectivityServiceMock";
    private static final boolean DBG = true;
    private static final boolean VDBG = true;

    /**
     * used internally when registering NetworkFactories
     * obj = NetworkFactoryInfo
     */
    private static final int EVENT_REGISTER_NETWORK_FACTORY = 17;

    /**
     * used internally when registering NetworkAgents
     * obj = Messenger
     */
    private static final int EVENT_REGISTER_NETWORK_AGENT = 18;

    /**
     * used to add a network request
     * includes a NetworkRequestInfo
     */
    private static final int EVENT_REGISTER_NETWORK_REQUEST = 19;

    /**
     * used to add a network listener - no request
     * includes a NetworkRequestInfo
     */
    private static final int EVENT_REGISTER_NETWORK_LISTENER = 21;

    /**
     * used to remove a network request, either a listener or a real request
     * arg1 = UID of caller
     * obj  = NetworkRequest
     */
    private static final int EVENT_RELEASE_NETWORK_REQUEST = 22;

    /**
     * used internally when registering NetworkFactories
     * obj = Messenger
     */
    private static final int EVENT_UNREGISTER_NETWORK_FACTORY = 23;


    private final HandlerThread mHandlerThread;
    /** Handler used for internal events. */
    final private InternalHandler mHandler;
    /** Handler used for incoming {@link NetworkStateTracker} events. */
    final private NetworkStateTrackerHandler mTrackerHandler;

    final private Context mContext;

    public ConnectivityServiceMock(Context context) {
        if (DBG) log("starting up");

        mContext = context;
        mHandlerThread = new HandlerThread("ConnectivityServiceMock");
        mHandlerThread.start();
        mHandler = new InternalHandler(mHandlerThread.getLooper());
        mTrackerHandler = new NetworkStateTrackerHandler(mHandlerThread.getLooper());
    }

    public void die() {
        // clean up threads/handlers
        if (mHandlerThread != null) {
            mHandlerThread.quit();
        }
    }

    private class InternalHandler extends Handler {
        public InternalHandler(Looper looper) {
            super(looper);
        }

        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case EVENT_REGISTER_NETWORK_FACTORY: {
                    handleRegisterNetworkFactory((NetworkFactoryInfo)msg.obj);
                    break;
                }
                case EVENT_UNREGISTER_NETWORK_FACTORY: {
                    handleUnregisterNetworkFactory((Messenger)msg.obj);
                    break;
                }
                case EVENT_REGISTER_NETWORK_AGENT: {
                    handleRegisterNetworkAgent((NetworkAgentInfo)msg.obj);
                    break;
                }
                case EVENT_REGISTER_NETWORK_REQUEST:
                case EVENT_REGISTER_NETWORK_LISTENER: {
                    handleRegisterNetworkRequest((NetworkRequestInfo) msg.obj);
                    break;
                }
                case EVENT_RELEASE_NETWORK_REQUEST: {
                    handleReleaseNetworkRequest((NetworkRequest) msg.obj, msg.arg1);
                    break;
                }
            }
        }
    }

    private class NetworkStateTrackerHandler extends Handler {
        public NetworkStateTrackerHandler(Looper looper) {
            super(looper);
        }

        @Override
        public void handleMessage(Message msg) {
            NetworkInfo info;
            switch (msg.what) {
                case AsyncChannel.CMD_CHANNEL_HALF_CONNECTED: {
                    handleAsyncChannelHalfConnect(msg);
                    break;
                }
                case AsyncChannel.CMD_CHANNEL_DISCONNECT: {
                    NetworkAgentInfo nai = mNetworkAgentInfos.get(msg.replyTo);
                    if (nai != null) nai.asyncChannel.disconnect();
                    break;
                }
                case AsyncChannel.CMD_CHANNEL_DISCONNECTED: {
                    handleAsyncChannelDisconnected(msg);
                    break;
                }
            }
        }
    }

    private boolean isRequest(NetworkRequest request) {
        return mNetworkRequests.get(request).isRequest;
    }

    private void handleAsyncChannelHalfConnect(Message msg) {
        AsyncChannel ac = (AsyncChannel) msg.obj;
        if (mNetworkFactoryInfos.containsKey(msg.replyTo)) {
            if (msg.arg1 == AsyncChannel.STATUS_SUCCESSFUL) {
                if (VDBG) log("NetworkFactory connected");
                // A network factory has connected.  Send it all current NetworkRequests.
                for (NetworkRequestInfo nri : mNetworkRequests.values()) {
                    if (nri.isRequest == false) continue;
                    //NetworkAgentInfo nai = mNetworkForRequestId.get(nri.request.requestId);
                    NetworkAgentInfo nai = null;
                    ac.sendMessage(android.net.NetworkFactory.CMD_REQUEST_NETWORK,
                            (nai != null ? nai.getCurrentScore() : 0), 0, nri.request);
                }
            } else {
                loge("Error connecting NetworkFactory");
                mNetworkFactoryInfos.remove(msg.obj);
            }
        } else if (mNetworkAgentInfos.containsKey(msg.replyTo)) {
            if (msg.arg1 == AsyncChannel.STATUS_SUCCESSFUL) {
                if (VDBG) log("NetworkAgent connected");
                // A network agent has requested a connection.  Establish the connection.
                mNetworkAgentInfos.get(msg.replyTo).asyncChannel.
                        sendMessage(AsyncChannel.CMD_CHANNEL_FULL_CONNECTION);
            } else {
                loge("Error connecting NetworkAgent");
                NetworkAgentInfo nai = mNetworkAgentInfos.remove(msg.replyTo);
                //if (nai != null) {
                //    final boolean wasDefault = isDefaultNetwork(nai);
                //    synchronized (mNetworkForNetId) {
                //        mNetworkForNetId.remove(nai.network.netId);
                //        mNetIdInUse.delete(nai.network.netId);
                //    }
                //    // Just in case.
                //    mLegacyTypeTracker.remove(nai, wasDefault);
                //}
            }
        }
    }

    private void handleAsyncChannelDisconnected(Message msg) {
        NetworkAgentInfo nai = mNetworkAgentInfos.get(msg.replyTo);
        if (nai != null) {
            if (DBG) {
                log(nai.name() + " got DISCONNECTED, was satisfying " + nai.numNetworkRequests());
            }
            // A network agent has disconnected.
            // TODO - if we move the logic to the network agent (have them disconnect
            // because they lost all their requests or because their score isn't good)
            // then they would disconnect organically, report their new state and then
            // disconnect the channel.
            //if (nai.networkInfo.isConnected()) {
            //    nai.networkInfo.setDetailedState(NetworkInfo.DetailedState.DISCONNECTED,
            //            null, null);
            //}
            //final boolean wasDefault = isDefaultNetwork(nai);
            //if (wasDefault) {
            //    mDefaultInetConditionPublished = 0;
            //}
            //notifyIfacesChanged();
            // TODO - we shouldn't send CALLBACK_LOST to requests that can be satisfied
            // by other networks that are already connected. Perhaps that can be done by
            // sending all CALLBACK_LOST messages (for requests, not listens) at the end
            // of rematchAllNetworksAndRequests
            //notifyNetworkCallbacks(nai, ConnectivityManager.CALLBACK_LOST);
            //mKeepaliveTracker.handleStopAllKeepalives(nai,
            //       ConnectivityManager.PacketKeepalive.ERROR_INVALID_NETWORK);
            // nai.networkMonitor.sendMessage(NetworkMonitor.CMD_NETWORK_DISCONNECTED);
            mNetworkAgentInfos.remove(msg.replyTo);
            //updateClat(null, nai.linkProperties, nai);
            //synchronized (mNetworkForNetId) {
            //    // Remove the NetworkAgent, but don't mark the netId as
            //    // available until we've told netd to delete it below.
            //    mNetworkForNetId.remove(nai.network.netId);
            //}
            // Remove all previously satisfied requests.
            //for (int i = 0; i < nai.networkRequests.size(); i++) {
            //    NetworkRequest request = nai.networkRequests.valueAt(i);
            //    NetworkAgentInfo currentNetwork = mNetworkForRequestId.get(request.requestId);
            //    if (currentNetwork != null && currentNetwork.network.netId == nai.network.netId) {
            //        mNetworkForRequestId.remove(request.requestId);
            //        sendUpdatedScoreToFactories(request, 0);
            //    }
            //}
            //if (nai.networkRequests.get(mDefaultRequest.requestId) != null) {
            //    removeDataActivityTracking(nai);
            //    notifyLockdownVpn(nai);
            //    requestNetworkTransitionWakelock(nai.name());
            //}
            //mLegacyTypeTracker.remove(nai, wasDefault);
            //rematchAllNetworksAndRequests(null, 0);
            //if (nai.created) {
            //    // Tell netd to clean up the configuration for this network
            //    // (routing rules, DNS, etc).
            //    // This may be slow as it requires a lot of netd shelling out to ip and
            //    // ip[6]tables to flush routes and remove the incoming packet mark rule, so do it
            //    // after we've rematched networks with requests which should make a potential
            //    // fallback network the default or requested a new network from the
            //    // NetworkFactories, so network traffic isn't interrupted for an unnecessarily
            //    // long time.
            //    try {
            //        mNetd.removeNetwork(nai.network.netId);
            //    } catch (Exception e) {
            //        loge("Exception removing network: " + e);
            //    }
            //}
            //synchronized (mNetworkForNetId) {
            //    mNetIdInUse.delete(nai.network.netId);
            //}
        } else {
            NetworkFactoryInfo nfi = mNetworkFactoryInfos.remove(msg.replyTo);
            if (DBG && nfi != null) log("unregisterNetworkFactory for " + nfi.name);
        }
    }

    private void log(String str) {
        Slog.d(TAG, str);
    }
    private void loge(String str) {
        Slog.e(TAG, str);
    }

    // NetworkAgentInfo keyed off its connecting messenger
    // TODO - eval if we can reduce the number of lists/hashmaps/sparsearrays
    // NOTE: Only should be accessed on ConnectivityServiceThread, except dump().
    private final HashMap<Messenger, NetworkAgentInfo> mNetworkAgentInfos =
            new HashMap<Messenger, NetworkAgentInfo>();
    private final HashMap<Messenger, NetworkFactoryInfo> mNetworkFactoryInfos =
            new HashMap<Messenger, NetworkFactoryInfo>();
    private final HashMap<NetworkRequest, NetworkRequestInfo> mNetworkRequests =
            new HashMap<NetworkRequest, NetworkRequestInfo>();

    private static class NetworkFactoryInfo {
        public final String name;
        public final Messenger messenger;
        public final AsyncChannel asyncChannel;
        public final int factorySerialNumber;

        NetworkFactoryInfo(String name, Messenger messenger, AsyncChannel asyncChannel,
                int factorySerialNumber) {
            this.name = name;
            this.messenger = messenger;
            this.asyncChannel = asyncChannel;
            this.factorySerialNumber = factorySerialNumber;
        }
    }

    private int getCallingUid() {
        return Process.myUid();
    }

    private int getCallingPid() {
        return Process.myPid();
    }

    private class NetworkRequestInfo implements IBinder.DeathRecipient {
        static final boolean REQUEST = true;
        static final boolean LISTEN = false;

        final NetworkRequest request;
        final PendingIntent mPendingIntent;
        boolean mPendingIntentSent;
        private final IBinder mBinder;
        final int mPid;
        final int mUid;
        final Messenger messenger;
        final boolean isRequest;

        NetworkRequestInfo(NetworkRequest r, PendingIntent pi, boolean isRequest) {
            request = r;
            mPendingIntent = pi;
            messenger = null;
            mBinder = null;
            mPid = getCallingPid();
            mUid = getCallingUid();
            this.isRequest = isRequest;
        }

        NetworkRequestInfo(Messenger m, NetworkRequest r, IBinder binder, boolean isRequest) {
            super();
            messenger = m;
            request = r;
            mBinder = binder;
            mPid = getCallingPid();
            mUid = getCallingUid();
            this.isRequest = isRequest;
            mPendingIntent = null;

            try {
                mBinder.linkToDeath(this, 0);
            } catch (RemoteException e) {
                binderDied();
            }
        }

        void unlinkDeathRecipient() {
            if (mBinder != null) {
                mBinder.unlinkToDeath(this, 0);
            }
        }

        public void binderDied() {
            log("ConnectivityService NetworkRequestInfo binderDied(" +
                    request + ", " + mBinder + ")");
            releaseNetworkRequest(request);
        }

        public String toString() {
            return (isRequest ? "Request" : "Listen") +
                    " from uid/pid:" + mUid + "/" + mPid +
                    " for " + request +
                    (mPendingIntent == null ? "" : " to trigger " + mPendingIntent);
        }
    }


    // sequence number of NetworkRequests
    private int mNextNetworkRequestId = 1;
    private synchronized int nextNetworkRequestId() {
        return mNextNetworkRequestId++;
    }

    public NetworkRequest requestNetwork(NetworkCapabilities networkCapabilities,
            Messenger messenger, int timeoutMs, IBinder binder, int legacyType) {
        networkCapabilities = new NetworkCapabilities(networkCapabilities);

        if (timeoutMs < 0) {
            throw new IllegalArgumentException("Bad timeout specified");
        }

        NetworkRequest networkRequest = new NetworkRequest(networkCapabilities, legacyType,
                nextNetworkRequestId(), NetworkRequest.Type.REQUEST);
        NetworkRequestInfo nri = new NetworkRequestInfo(messenger, networkRequest, binder, true);
        if (DBG) log("requestNetwork for " + nri);

        mHandler.sendMessage(mHandler.obtainMessage(EVENT_REGISTER_NETWORK_REQUEST, nri));

        return networkRequest;
    }

    public void releaseNetworkRequest(NetworkRequest networkRequest) {
        mHandler.sendMessage(mHandler.obtainMessage(EVENT_RELEASE_NETWORK_REQUEST, getCallingUid(),
                0, networkRequest));
    }

    public int registerNetworkFactory(Messenger messenger, String name) {
        NetworkFactoryInfo nfi = new NetworkFactoryInfo(name, messenger, new AsyncChannel(),
                NetworkFactory.SerialNumber.nextSerialNumber());
        mHandler.sendMessage(mHandler.obtainMessage(EVENT_REGISTER_NETWORK_FACTORY, nfi));
        return nfi.factorySerialNumber;
    }

    private void handleRegisterNetworkFactory(NetworkFactoryInfo nfi) {
        if (DBG) log("Got NetworkFactory Messenger for " + nfi.name);
        mNetworkFactoryInfos.put(nfi.messenger, nfi);
        nfi.asyncChannel.connect(mContext, mTrackerHandler, nfi.messenger);
    }

    public void unregisterNetworkFactory(Messenger messenger) {
        mHandler.sendMessage(mHandler.obtainMessage(EVENT_UNREGISTER_NETWORK_FACTORY, messenger));
    }

    private void handleUnregisterNetworkFactory(Messenger messenger) {
        NetworkFactoryInfo nfi = mNetworkFactoryInfos.remove(messenger);
        if (nfi == null) {
            loge("Failed to find Messenger in unregisterNetworkFactory");
            return;
        }
        if (DBG) log("unregisterNetworkFactory for " + nfi.name);
    }

    public int registerNetworkAgent(Messenger messenger, NetworkInfo networkInfo,
            LinkProperties linkProperties, NetworkCapabilities networkCapabilities,
            int currentScore, NetworkMisc networkMisc, int factorySerialNumber) {
//        final NetworkAgentInfo nai = new NetworkAgentInfo(messenger, new AsyncChannel(),
//                new Network(reserveNetId()), new NetworkInfo(networkInfo), new LinkProperties(
//                linkProperties), new NetworkCapabilities(networkCapabilities), currentScore,
//                mContext, mTrackerHandler, new NetworkMisc(networkMisc), mDefaultRequest, this);
//        synchronized (this) {
//            nai.networkMonitor.systemReady = mSystemReady;
//        }
//        mHandler.sendMessage(mHandler.obtainMessage(EVENT_REGISTER_NETWORK_AGENT, nai));
//        return nai.network.netId;
        throw new RuntimeException("not implemented");
    }

    private void handleRegisterNetworkAgent(NetworkAgentInfo na) {
        if (VDBG) log("Got NetworkAgent Messenger");
//        mNetworkAgentInfos.put(na.messenger, na);
//        synchronized (mNetworkForNetId) {
//            mNetworkForNetId.put(na.network.netId, na);
//        }
//        na.asyncChannel.connect(mContext, mTrackerHandler, na.messenger);
//        NetworkInfo networkInfo = na.networkInfo;
//        na.networkInfo = null;
//        updateNetworkInfo(na, networkInfo);
    }


    private void handleRegisterNetworkRequest(NetworkRequestInfo nri) {
        mNetworkRequests.put(nri.request, nri);
        if (!nri.isRequest) {
            for (NetworkAgentInfo network : mNetworkAgentInfos.values()) {
                if (nri.request.networkCapabilities.hasSignalStrength() &&
                        network.satisfiesImmutableCapabilitiesOf(nri.request)) {
                }
            }
        }
        rematchAllNetworksAndRequests(null, 0);
        if (nri.isRequest) {
            sendUpdatedScoreToFactories(nri.request, 0);
        }
    }

    private void handleReleaseNetworkRequest(NetworkRequest request, int callingUid) {
        NetworkRequestInfo nri = mNetworkRequests.get(request);
        if (nri != null) {
            if (DBG) log("releasing NetworkRequest " + request);
            nri.unlinkDeathRecipient();
            mNetworkRequests.remove(request);
            if (nri.isRequest) {
                // Find all networks that are satisfying this request and remove the request
                // from their request lists.
                // TODO - it's my understanding that for a request there is only a single
                // network satisfying it, so this loop is wasteful
                //boolean wasKept = false;
                //for (NetworkAgentInfo nai : mNetworkAgentInfos.values()) {
                //    if (nai.networkRequests.get(nri.request.requestId) != null) {
                //        nai.networkRequests.remove(nri.request.requestId);
                //        if (DBG) {
                //            log(" Removing from current network " + nai.name() +
                //                    ", leaving " + nai.networkRequests.size() +
                //                    " requests.");
                //        }
                //        if (unneeded(nai)) {
                //            if (DBG) log("no live requests for " + nai.name() + "; disconnecting");
                //            teardownUnneededNetwork(nai);
                //        } else {
                //            // suspect there should only be one pass through here
                //            // but if any were kept do the check below
                //            wasKept |= true;
                //        }
                //    }
                //}

                //NetworkAgentInfo nai = mNetworkForRequestId.get(nri.request.requestId);
                //if (nai != null) {
                //    mNetworkForRequestId.remove(nri.request.requestId);
                //}
                // Maintain the illusion.  When this request arrived, we might have pretended
                // that a network connected to serve it, even though the network was already
                // connected.  Now that this request has gone away, we might have to pretend
                // that the network disconnected.  LegacyTypeTracker will generate that
                // phantom disconnect for this type.
                //if (nri.request.legacyType != TYPE_NONE && nai != null) {
                //    boolean doRemove = true;
                //    if (wasKept) {
                //        // check if any of the remaining requests for this network are for the
                //        // same legacy type - if so, don't remove the nai
                //        for (int i = 0; i < nai.networkRequests.size(); i++) {
                //            NetworkRequest otherRequest = nai.networkRequests.valueAt(i);
                //            if (otherRequest.legacyType == nri.request.legacyType &&
                //                    isRequest(otherRequest)) {
                //                if (DBG) log(" still have other legacy request - leaving");
                //                doRemove = false;
                //            }
                //        }
                //    }
                //
                //    if (doRemove) {
                //        mLegacyTypeTracker.remove(nri.request.legacyType, nai, false);
                //    }
                //}

                for (NetworkFactoryInfo nfi : mNetworkFactoryInfos.values()) {
                    nfi.asyncChannel.sendMessage(android.net.NetworkFactory.CMD_CANCEL_REQUEST,
                            nri.request);
                }
            } else {
                // listens don't have a singular affectedNetwork.  Check all networks to see
                // if this listen request applies and remove it.
                //for (NetworkAgentInfo nai : mNetworkAgentInfos.values()) {
                //    nai.networkRequests.remove(nri.request.requestId);
                //    if (nri.request.networkCapabilities.hasSignalStrength() &&
                //            nai.satisfiesImmutableCapabilitiesOf(nri.request)) {
                //        updateSignalStrengthThresholds(nai, "RELEASE", nri.request);
                //    }
                //}
            }
            //callCallbackForRequest(nri, null, ConnectivityManager.CALLBACK_RELEASED);
        }
    }

    private void sendUpdatedScoreToFactories(NetworkAgentInfo nai) {
        for (int i = 0; i < nai.numNetworkRequests(); i++) {
            NetworkRequest nr = nai.requestAt(i);
            // Don't send listening requests to factories. b/17393458
            if (!isRequest(nr)) continue;
                sendUpdatedScoreToFactories(nr, nai.getCurrentScore());
        }
    }

    private void sendUpdatedScoreToFactories(NetworkRequest networkRequest, int score) {
        if (VDBG) log("sending new Min Network Score(" + score + "): " + networkRequest.toString());
        for (NetworkFactoryInfo nfi : mNetworkFactoryInfos.values()) {
            nfi.asyncChannel.sendMessage(android.net.NetworkFactory.CMD_REQUEST_NETWORK, score, 0,
                    networkRequest);
        }
    }

    private void rematchAllNetworksAndRequests(NetworkAgentInfo changed, int oldScore) {
    }

    @VisibleForTesting
    public NetworkRequest defaultRequest = null;
    @VisibleForTesting
    public synchronized void addDefaultRequest() {
        if (defaultRequest != null) return;
        NetworkCapabilities netCap = new NetworkCapabilities();
        netCap.addCapability(NET_CAPABILITY_INTERNET);
        netCap.addCapability(NET_CAPABILITY_NOT_RESTRICTED);
        defaultRequest = requestNetwork(netCap, null, 0, new Binder(),
                ConnectivityManager.TYPE_NONE);
    }

    @VisibleForTesting
    public synchronized void setCurrentScoreForRequest(NetworkRequest nr, int score) {
        sendUpdatedScoreToFactories(nr, score);
    }

    @VisibleForTesting
    public synchronized void removeDefaultRequest() {
        if (defaultRequest == null) return;
        releaseNetworkRequest(defaultRequest);
        defaultRequest = null;
    }
}
