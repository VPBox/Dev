package com.cells.cellswitch.secure.wifip2p;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.net.NetworkInfo;
import android.net.wifi.p2p.WifiP2pDevice;
import android.net.wifi.p2p.WifiP2pDeviceList;
import android.net.wifi.p2p.WifiP2pInfo;
import android.net.wifi.p2p.WifiP2pManager;
import android.util.Log;

public class Wifip2pReceiver extends BroadcastReceiver {

    public static final String TAG  = "Wifip2pReceiver";

    private WifiP2pManager mWifiP2pManager;
    private WifiP2pManager.Channel mChannel;
    private Wifip2pActionListener mListener;


    public Wifip2pReceiver(WifiP2pManager wifiP2pManager, WifiP2pManager.Channel channel,
                           Wifip2pActionListener listener) {
        mWifiP2pManager = wifiP2pManager;
        mChannel = channel;
        mListener = listener;

        mWifiP2pManager.requestPeers(mChannel, new WifiP2pManager.PeerListListener() {
            @Override
            public void onPeersAvailable(WifiP2pDeviceList peers) {
                mListener.onPeersInfo(peers.getDeviceList());
            }
        });
    }

    @Override
    public void onReceive(Context context, Intent intent) {
        Log.e(TAG, "Broadcast received： " + intent.getAction());

        int state = intent.getIntExtra(WifiP2pManager.EXTRA_WIFI_STATE, -1);
        switch (intent.getAction()) {
            case WifiP2pManager.WIFI_P2P_STATE_CHANGED_ACTION:
                if (state == WifiP2pManager.WIFI_P2P_STATE_ENABLED) {
                    mListener.wifiP2pEnabled(true);
                } else {
                    mListener.wifiP2pEnabled(false);
                }
                break;
            case WifiP2pManager.WIFI_P2P_PEERS_CHANGED_ACTION:
                WifiP2pDeviceList peers = new WifiP2pDeviceList();
                peers = (WifiP2pDeviceList) intent.getParcelableExtra(
                                    WifiP2pManager.EXTRA_P2P_DEVICE_LIST);
                mListener.onPeersInfo(peers.getDeviceList());
                break;
            case WifiP2pManager.WIFI_P2P_CONNECTION_CHANGED_ACTION:
                NetworkInfo networkInfo = intent.getParcelableExtra(WifiP2pManager.EXTRA_NETWORK_INFO);
                if (networkInfo.isConnected()){
                    mWifiP2pManager.requestConnectionInfo(mChannel, new WifiP2pManager.ConnectionInfoListener() {
                        @Override
                        public void onConnectionInfoAvailable(WifiP2pInfo info) {
                            mListener.onConnection(info);
                        }
                    });
                }else {
                    mListener.onDisconnection();
                }
                break;
            case WifiP2pManager.WIFI_P2P_THIS_DEVICE_CHANGED_ACTION:
                WifiP2pDevice device = intent.getParcelableExtra(WifiP2pManager.EXTRA_WIFI_P2P_DEVICE);
                mListener.onDeviceInfo(device);
                break;
            default:
                break;
        }
    }
}
