package android.app;

import android.os.ICellsService;
import android.content.Context;
import android.os.RemoteException;
import android.util.Log;

public class CellsServiceManager {
    private static String TAG = "CellsServiceManager";
    private final Context mContext;
    private final ICellsService mService;
    public CellsServiceManager(Context context,ICellsService service) {
        mContext = context; 
        mService = service;
    }

    public boolean isSystemReady(){
        try{
            return mService.isSystemReady();
        }catch(RemoteException e){
            Log.e(TAG,e.toString());
            e.printStackTrace();
        }
        return false;
    }

    public String getDns1(){
        try{
            return mService.getDns1();
        }catch(RemoteException e){
            Log.e(TAG,e.toString());
            e.printStackTrace();
        }
        return "";
    }

    public String getDns2(){
        try{
            return mService.getDns2();
        }catch(RemoteException e){
            Log.e(TAG,e.toString());
            e.printStackTrace();
        }
        return "";
    }
}