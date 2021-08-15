package android.app;

import android.content.Context;
import android.os.Binder;
import android.os.Bundle;
import android.os.IBinder;
import android.os.Parcel;
import android.os.Parcelable;
import android.os.PooledStringReader;
import android.os.PooledStringWriter;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.util.Log;

import android.os.SystemProperties;


public class CellsPrivateServiceManager {
    static String TAG = "CellsPrivateServiceManager";
    static final int  SETPROPERTY = Binder.FIRST_CALL_TRANSACTION;
    static final int  STARTCELLSVM = Binder.FIRST_CALL_TRANSACTION + 1;
    static final int  STOPCELLSVM = Binder.FIRST_CALL_TRANSACTION + 2;
    static final int  SWITCHCELLSVM = Binder.FIRST_CALL_TRANSACTION + 3;
    static final int  UPLOADCELLSVM = Binder.FIRST_CALL_TRANSACTION + 4;
    static final int  DOWNLOADCELLSVM = Binder.FIRST_CALL_TRANSACTION + 5;
    static final int  UNTARCELLSVM = Binder.FIRST_CALL_TRANSACTION + 6;
    static final int  TARCELLSVM = Binder.FIRST_CALL_TRANSACTION + 7;
    static final int  SYSTEMREADY = Binder.FIRST_CALL_TRANSACTION + 8;
    static final int  SENDCELLSVM = Binder.FIRST_CALL_TRANSACTION + 15;
    private final Context mContext;
    private final IBinder mChannel;
    public CellsPrivateServiceManager(Context context, IBinder channel ) {
        mContext = context; 
        mChannel = channel;
    }

    public int startCellsVM(String name) throws RemoteException {
        if(mChannel == null) return 0;

        int ret = 0;
        Parcel data = Parcel.obtain();
        Parcel reply = Parcel.obtain();
        Log.e(TAG,"startCellsVM start ");
        data.writeInterfaceToken("CellsPrivateService");
        data.writeString(name);
        mChannel.transact(STARTCELLSVM, data, reply, 0);
        reply.readException();
        Log.e(TAG,"startCellsVM end ");
        data.recycle();
        ret = reply.readInt();
        reply.recycle();
        return ret;
    }

    public int stopCellsVM(String name) throws RemoteException {
        if(mChannel == null) return 0;

        int ret = 0;
        Parcel data = Parcel.obtain();
        Parcel reply = Parcel.obtain();
        Log.e(TAG,"stopCellsVM start ");
        data.writeInterfaceToken("CellsPrivateService");
        data.writeString(name);
        mChannel.transact(STOPCELLSVM, data, reply, 0);
        reply.readException();
        Log.e(TAG,"stopCellsVM end ");
        data.recycle();
        ret = reply.readInt();
        reply.recycle();
        return ret;
    }

    public int switchCellsVM(String name) throws RemoteException {
        if(mChannel == null) return 0;

        int ret = 0;
        Parcel data = Parcel.obtain();
        Parcel reply = Parcel.obtain();
        Log.e(TAG,"switchCellsVM start ");
        data.writeInterfaceToken("CellsPrivateService");
        data.writeString(name);
        mChannel.transact(SWITCHCELLSVM, data, reply, 0);
        reply.readException();
        Log.e(TAG,"switchCellsVM end ");
        data.recycle();
        ret = reply.readInt();
        reply.recycle();
        return ret;
    }

    public int uploadCellsVM(String name) throws RemoteException {
        if(mChannel == null) return 0;

        int ret = 0;
        Parcel data = Parcel.obtain();
        Parcel reply = Parcel.obtain();
        Log.e(TAG,"uploadCellsVM start ");
        data.writeInterfaceToken("CellsPrivateService");
        data.writeString(name);
        mChannel.transact(UPLOADCELLSVM, data, reply, 0);
        reply.readException();
        Log.e(TAG,"uploadCellsVM end ");
        data.recycle();
        ret = reply.readInt();
        reply.recycle();
        return ret;
    }

    public int downloadCellsVM(String name) throws RemoteException {
        if(mChannel == null) return 0;

        int ret = 0;
        Parcel data = Parcel.obtain();
        Parcel reply = Parcel.obtain();
        Log.e(TAG,"downloadCellsVM start ");
        data.writeInterfaceToken("CellsPrivateService");
        data.writeString(name);
        mChannel.transact(DOWNLOADCELLSVM, data, reply, 0);
        reply.readException();
        Log.e(TAG,"downloadCellsVM end ");
        data.recycle();
        ret = reply.readInt();
        reply.recycle();
        return ret;
    }

    public int untarCellsVM(String name) throws RemoteException {
        if(mChannel == null) return 0;

        int ret = 0;
        Parcel data = Parcel.obtain();
        Parcel reply = Parcel.obtain();
        Log.e(TAG,"untarCellsVM start ");
        data.writeInterfaceToken("CellsPrivateService");
        data.writeString(name);
        mChannel.transact(UNTARCELLSVM, data, reply, 0);
        reply.readException();
        Log.e(TAG,"untarCellsVM end ");
        data.recycle();
        ret = reply.readInt();
        reply.recycle();
        return ret;
    }

    public int tarCellsVM(String name) throws RemoteException {
        if(mChannel == null) return 0;

        int ret = 0;
        Parcel data = Parcel.obtain();
        Parcel reply = Parcel.obtain();
        Log.e(TAG,"tarCellsVM start ");
        data.writeInterfaceToken("CellsPrivateService");
        data.writeString(name);
        mChannel.transact(TARCELLSVM, data, reply, 0);
        reply.readException();
        Log.e(TAG,"tarCellsVM end ");
        data.recycle();
        ret = reply.readInt();
        reply.recycle();
        return ret;
    }

    public int sendCellsVM(String path, String address) throws RemoteException {
        if(mChannel == null) return 0;

        int ret = 0;
        Parcel data = Parcel.obtain();
        Parcel reply = Parcel.obtain();
        Log.e(TAG,"sendCellsVM start ");
        data.writeInterfaceToken("CellsPrivateService");
        data.writeString(path);
        data.writeString(address);
        mChannel.transact(SENDCELLSVM, data, reply, 0);
        //reply.readException();
        Log.e(TAG,"sendCellsVM end ");
        data.recycle();
        ret = reply.readInt();
        reply.recycle();
        return ret;
    }

    public int vmSystemReady() throws RemoteException {
        if(mChannel == null) return 0;

        String vmname = SystemProperties.get("persist.sys.vm.name","");
        if(vmname == null || vmname.length() <= 3){
            return 0;
        }

        int ret = 0;
        Parcel data = Parcel.obtain();
        Parcel reply = Parcel.obtain();
        Log.e(TAG,"systemReady start ");
        data.writeInterfaceToken("CellsPrivateService");
        data.writeString(vmname);
        mChannel.transact(SYSTEMREADY, data, reply, 0);
        reply.readException();
        Log.e(TAG,"systemReady end ");
        data.recycle();
        ret = reply.readInt();
        reply.recycle();
        return ret;
    }

}