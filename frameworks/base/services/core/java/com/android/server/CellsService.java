package com.android.server;

import android.os.ICellsService;
import android.content.Context;
import android.os.SystemProperties;
import android.os.Looper;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.IBinder;
import android.os.INetworkManagementService;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.os.Message;
import android.os.SystemClock;
import android.util.Log;

import android.net.ConnectivityManager;
import android.net.DhcpResults;
import android.net.Network;
//import android.net.dhcp.DhcpClient;
import android.net.InterfaceConfiguration;
import android.net.LinkAddress;
import android.net.LinkProperties;
import android.net.NetworkAgent;
import android.net.IConnectivityManager;
import android.net.NetworkCapabilities;
import android.net.NetworkFactory;
import android.net.NetworkInfo;
import android.net.NetworkInfo.DetailedState;
import android.net.NetworkRequest;
import android.net.NetworkUtils;
import android.net.RouteInfo;
import android.net.StaticIpConfiguration;
import android.net.NetworkMisc;
import android.net.TrafficStats;
import android.app.CellsPrivateServiceManager;

import android.provider.Settings;

import com.android.server.net.NetlinkTracker;
import com.android.internal.util.StateMachine;
import com.android.internal.util.State;

import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.net.InetAddress;

public class CellsService extends ICellsService.Stub {

    private final Context mContext;
    private boolean mSystemReady = false;

    private static final String DNS1 = "net.dns1";
    private static final String DNS2 = "net.dns2";

    private CellsNetworkAgent mCellsNetworkAgent;

    public CellsService(Context context) {
        mContext = context;
    }

    @Override
    public boolean isSystemReady(){
        return mSystemReady;
    }

    @Override
    public String getDns1(){
        return SystemProperties.get(DNS1, "");
    }

    @Override
    public String getDns2(){
        return SystemProperties.get(DNS2, "");
    }

    public  void systemReady(){
        if(SystemProperties.get("ro.boot.vm","0").equals("1")){
              mCellsNetworkAgent = new CellsNetworkAgent(mContext);

              CellsPrivateServiceManager mCellsService = new CellsPrivateServiceManager(mContext,
                        ServiceManager.getInitService("CellsPrivateService"));
              try{
                    mCellsService.vmSystemReady();
              }catch(RemoteException e){
                    e.printStackTrace();
              }
        }

        mSystemReady = true;
    }

    private class CellsNetworkAgent {
        private static final String LOG_TAG = "CellsNetworkAgent";
        private final Context mContext;

        private NetworkAgent mSystemAgent = null;

        private static final String VM_INTERFACENAME = "wlan0";
        private static final String NETWORKTYPE = "WIFI";
        private static final String TCPBUFFERSIZES = "524288,2097152,4194304,262144,524288,1048576";

        private static final int VM_BASE_ADDR                   = 16 ;
        private static final String VM_NEAR_IFACE_ADDR    = "172.%d.3.3";
        private static final String VM_GATEWAY_ADDR       = "172.%d.3.2";
        private static final String VM_DEST_ADDR               = "172.%d.0.0";
        private static final int VM_PREFIX_LENGTH              = 16;

        private NetworkInfo mNetworkInfo;
        private NetworkCapabilities mNetworkCapabilities;
        private LinkProperties mLinkProperties;
        private int mScore;
        private NetworkMisc mNetworkMisc = new NetworkMisc();

        //private NetlinkTracker mNetlinkTracker = null;
        //private INetworkManagementService mNwService = null;

        //private CellsDhcpStateMachine mCellsDhcpStateMachine;
        //private DhcpResults mDhcpResults;

        private int mIndex = 0;

        private LinkProperties reBuildVMLinkProperties(int index){
            mLinkProperties = new LinkProperties();

            mLinkProperties.setInterfaceName(VM_INTERFACENAME);
            mLinkProperties.setTcpBufferSizes(TCPBUFFERSIZES);

            {
                InetAddress gateway = NetworkUtils.numericToInetAddress(String.format(VM_GATEWAY_ADDR,VM_BASE_ADDR + index));
                InetAddress dest    = NetworkUtils.numericToInetAddress("0.0.0.0");
                RouteInfo route = new RouteInfo(new LinkAddress(dest, 0),gateway,mLinkProperties.getInterfaceName());
                mLinkProperties.addRoute(route);
            }

            {
                InetAddress gateway = NetworkUtils.numericToInetAddress("0.0.0.0");
                InetAddress dest    = NetworkUtils.numericToInetAddress(String.format(VM_DEST_ADDR,VM_BASE_ADDR + index));
                RouteInfo route = new RouteInfo(new LinkAddress(dest, VM_PREFIX_LENGTH),gateway,mLinkProperties.getInterfaceName());
                mLinkProperties.addRoute(route);
            }

            {
                /*if(mNetlinkTracker != null){
                    LinkProperties netlinkLinkProperties = mNetlinkTracker.getLinkProperties();
                    for (InetAddress dns : netlinkLinkProperties.getDnsServers()) {
                        if (mLinkProperties.isReachable(dns)) {
                            mLinkProperties.addDnsServer(dns);
                        }
                    }
                }*/

                /*if(mDhcpResults != null){
                    for (InetAddress dns : mDhcpResults.dnsServers) {
                        if (mLinkProperties.isReachable(dns)) {
                            mLinkProperties.addDnsServer(dns);
                        }
                    }
                    mLinkProperties.setDomains(mDhcpResults.domains);
                }*/

                IBinder b = ServiceManager.getInitService(Context.CELLS_SERVICE);
                if(b != null){
                    ICellsService service = ICellsService.Stub.asInterface(b);

                    String dns1 = null;
                    try {
                        dns1 = service.getDns1();
                    } catch (RemoteException e) {
                        Log.e(LOG_TAG, "Couldn't getDns1: " + e.toString());
                    }
                    if(dns1 != null && dns1.length() > 0 ){
                        mLinkProperties.addDnsServer(NetworkUtils.numericToInetAddress(dns1));
                    }

                    String dns2 = null;
                    try {
                        dns2 = service.getDns2();
                    } catch (RemoteException e) {
                        Log.e(LOG_TAG, "Couldn't getDns2: " + e.toString());
                    }
                    if(dns2 != null && dns2.length() > 0 ){
                        mLinkProperties.addDnsServer(NetworkUtils.numericToInetAddress(dns2));
                    }
                }
            }

            InetAddress addr = NetworkUtils.numericToInetAddress(String.format(VM_NEAR_IFACE_ADDR,VM_BASE_ADDR + index));
            mLinkProperties.addLinkAddress(new LinkAddress(addr, VM_PREFIX_LENGTH));

            return mLinkProperties;
        }

        private NetworkCapabilities reBuildVMNetworkCapabilities(int index){
            mNetworkCapabilities = new NetworkCapabilities();
            mNetworkCapabilities.addTransportType(NetworkCapabilities.TRANSPORT_WIFI);
            mNetworkCapabilities.addCapability(NetworkCapabilities.NET_CAPABILITY_INTERNET);
            mNetworkCapabilities.addCapability(NetworkCapabilities.NET_CAPABILITY_NOT_RESTRICTED);
            mNetworkCapabilities.addCapability(NetworkCapabilities.NET_CAPABILITY_TRUSTED);
            mNetworkCapabilities.addCapability(NetworkCapabilities.NET_CAPABILITY_NOT_VPN);
            mNetworkCapabilities.setLinkUpstreamBandwidthKbps(1024 * 1024);
            mNetworkCapabilities.setLinkDownstreamBandwidthKbps(1024 * 1024);
            mNetworkCapabilities.setSignalStrength(-60);
            return mNetworkCapabilities;
        }

        private NetworkInfo reBuildVMNetworkInfo(int index){
            mNetworkInfo = new NetworkInfo(ConnectivityManager.TYPE_WIFI, 0, NETWORKTYPE, "");
            mNetworkInfo.setIsAvailable(true);
            mNetworkInfo.setDetailedState(DetailedState.CONNECTED, null, "cells-ap");
            return mNetworkInfo;
        }

        private int reBuildVMScore(int index){
            mScore = 60;
            return mScore;
        }

        private NetworkMisc reBuildVMNetworkMisc(int index){
            return mNetworkMisc;
        }

        private void createVMNetworkAgent(final int index){
            Thread vm = new Thread(new Runnable(){
                @Override
                public void run() {
                    Looper.prepare();
                    Handler mHandler = new Handler(Looper.myLooper());
                    mSystemAgent = new NetworkAgent(mHandler.getLooper(), mContext, "CellsNetworkAgent" + index, 
                                    reBuildVMNetworkInfo(index), reBuildVMNetworkCapabilities(index), reBuildVMLinkProperties(index), reBuildVMScore(index), reBuildVMNetworkMisc(index)){
                        public void unwanted(){
                            Log.d(LOG_TAG, "CellsNetworkAgent unwanted.");
                        };
                    };
                    Log.d(LOG_TAG, "mLinkProperties=" + mLinkProperties.toString());
                    Log.d(LOG_TAG, "mNetworkCapabilities=" + mNetworkCapabilities.toString());
                    Log.d(LOG_TAG, "mNetworkInfo=" + mNetworkInfo.toString());
                    Looper.loop();
                }
            });
            vm.setDefaultUncaughtExceptionHandler(null);
            vm.start();

            updateVMLinkProperties(index);
        }

        private void updateVMLinkProperties(final int index){
            Thread vm = new Thread(new Runnable(){
                @Override
                public void run() {
                    do
                    {
                        SystemClock.sleep(5000);

                        IBinder b = ServiceManager.getInitService(Context.CELLS_SERVICE);
                        if(b != null){
                            ICellsService service = ICellsService.Stub.asInterface(b);
                            String dns1 = null;
                            try {
                                dns1 = service.getDns1();
                            } catch (RemoteException e) {
                                Log.e(LOG_TAG, "Couldn't getDns1: " + e.toString());
                            }

                            if(!dns1.equals(SystemProperties.get(DNS1, ""))){
                                if(mSystemAgent != null){
                                    mSystemAgent.sendLinkProperties(reBuildVMLinkProperties(mIndex));
                                }
                            }
                        }
                    }while(true);
                }
            });
            vm.setDefaultUncaughtExceptionHandler(null);
            vm.start();
        }

        /*private class CellsDhcpStateMachine extends StateMachine {
            private BaseDhcpStateMachine mDhcpStateMachine;

            DefaultState mDefaultState = new DefaultState();
            CellsDhcpStateMachine(Looper looper) {
                super("CellsDhcpStateMachine", looper);

                addState(mDefaultState);
                setInitialState(mDefaultState);
            }

            class DefaultState extends State {
                @Override
                public boolean processMessage(Message message) {
                    switch (message.what) {
                        case DhcpStateMachine.CMD_POST_DHCP_ACTION:
                            if (message.arg1 == DhcpStateMachine.DHCP_SUCCESS){
                                mDhcpResults = (DhcpResults) message.obj;
                            } else if (message.arg1 == DhcpStateMachine.DHCP_FAILURE) {
                                if (mDhcpResults != null) {
                                    mDhcpResults.clear();
                                }
                            }
                            if(mSystemAgent != null){
                                mSystemAgent.sendLinkProperties(reBuildVMLinkProperties(mIndex));
                            }
                            break;
                        default:
                            return NOT_HANDLED;
                    }
                    return HANDLED;
                }
            }

            private boolean useLegacyDhcpClient() {
                return Settings.Global.getInt(
                        mContext.getContentResolver(),
                        Settings.Global.LEGACY_DHCP_CLIENT, 0) == 1;
            }

            private void maybeInitDhcpStateMachine() {
                if (mDhcpStateMachine == null) {
                    if (useLegacyDhcpClient()) {
                        mDhcpStateMachine = DhcpStateMachine.makeDhcpStateMachine(
                                mContext, CellsDhcpStateMachine.this, VM_INTERFACENAME);
                    } else {
                        mDhcpStateMachine = DhcpClient.makeDhcpStateMachine(
                                mContext, CellsDhcpStateMachine.this, VM_INTERFACENAME);
                    }
                }
            }

            public void startDhcp() {
                maybeInitDhcpStateMachine();
                mDhcpStateMachine.registerForPreDhcpNotification();
                mDhcpStateMachine.sendMessage(DhcpStateMachine.CMD_START_DHCP);
            }

            public void renewDhcp() {
                maybeInitDhcpStateMachine();
                mDhcpStateMachine.registerForPreDhcpNotification();
                mDhcpStateMachine.sendMessage(DhcpStateMachine.CMD_RENEW_DHCP);
            }

            public void stopDhcp() {
                if (mDhcpStateMachine != null) {
                    mDhcpStateMachine.sendMessage(DhcpStateMachine.CMD_STOP_DHCP);
                }
            }
        }*/

        public CellsNetworkAgent(Context context) {
            mContext = context;

            String vmname = SystemProperties.get("persist.sys.vm.name","");
            if(vmname == null || vmname.length() <= 3){
                return ;
            }

            Matcher m = Pattern.compile("\\d+").matcher(vmname);
            if(m.find()){
                mIndex = Integer.parseInt(m.group());
                if(mIndex <= 0) return;
            }

            createVMNetworkAgent(mIndex);

            /*IBinder b = ServiceManager.getService(Context.NETWORKMANAGEMENT_SERVICE);
            if(b != null){
                mNwService = INetworkManagementService.Stub.asInterface(b);
                mNetlinkTracker = new NetlinkTracker(VM_INTERFACENAME, new NetlinkTracker.Callback() {
                    public void update() {
                        if(mSystemAgent != null){
                            mSystemAgent.sendLinkProperties(reBuildVMLinkProperties(mIndex));
                        }
                    }
                });
                try {
                    mNwService.registerObserver(mNetlinkTracker);
                } catch (RemoteException e) {
                    Log.e(LOG_TAG, "Couldn't register netlink tracker: " + e.toString());
                }
            }*/

            /*HandlerThread thread = new HandlerThread("CellsDhcpStateMachine");
            thread.start();
            mCellsDhcpStateMachine = new CellsDhcpStateMachine(thread.getLooper());
            mCellsDhcpStateMachine.start();
            mCellsDhcpStateMachine.startDhcp();*/
        }
    }
}