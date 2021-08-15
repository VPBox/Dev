#ifndef __ICELLSSERVICE_H__
#define __ICELLSSERVICE_H__

#include <binder/IInterface.h>
#include <binder/Parcel.h>
#include <binder/BinderService.h>
#include <cutils/properties.h>
#include <utils/String16.h>

namespace android
{

class String16;
class ICellsPrivateService : public IInterface
{
protected:
    enum {
        SETPROPERTY = IBinder::FIRST_CALL_TRANSACTION,
        STARTCELLSVM = IBinder::FIRST_CALL_TRANSACTION + 1,
        STOPCELLSVM = IBinder::FIRST_CALL_TRANSACTION + 2,
        SWITCHCELLSVM = IBinder::FIRST_CALL_TRANSACTION + 3,
        UPLOADCELLSVM = IBinder::FIRST_CALL_TRANSACTION + 4,
        DOWNLOADCELLSVM = IBinder::FIRST_CALL_TRANSACTION + 5,
        UNTARCELLSVM = IBinder::FIRST_CALL_TRANSACTION + 6,
        TARCELLSVM = IBinder::FIRST_CALL_TRANSACTION + 7,
        SYSTEMREADY = IBinder::FIRST_CALL_TRANSACTION + 8,
        CELLSSWITCHVM = IBinder::FIRST_CALL_TRANSACTION + 9,
        CELLSSWITCHHOST = IBinder::FIRST_CALL_TRANSACTION + 10,
        ENTERCELL = IBinder::FIRST_CALL_TRANSACTION + 11,
        ENTERHOST = IBinder::FIRST_CALL_TRANSACTION + 12,
        EXITCELL = IBinder::FIRST_CALL_TRANSACTION + 13,
        EXITHOST = IBinder::FIRST_CALL_TRANSACTION + 14,
        SENDCELLSVM = IBinder::FIRST_CALL_TRANSACTION + 15,
    };

public:
    DECLARE_META_INTERFACE(CellsPrivateService);

    virtual status_t setProperty(const String16& name,const String16& value) = 0;
    virtual status_t startCellsVM(const String16& name) = 0;
    virtual status_t stopCellsVM(const String16& name) = 0;
    virtual status_t cellsSwitchVM(const String16& name) = 0;
    virtual status_t cellsSwitchHOST(const String16& name) = 0;
    virtual status_t enterCell(const String16& name) = 0;
    virtual status_t enterHost(const String16& name) = 0;
    virtual status_t exitCell(const String16& name) = 0;
    virtual status_t exitHost(const String16& name) = 0;
    virtual status_t switchCellsVM(const String16& name) = 0;
    virtual status_t uploadCellsVM(const String16& name) = 0;
    virtual status_t downloadCellsVM(const String16& name) = 0;
    virtual status_t untarCellsVM(const String16& name) = 0;
    virtual status_t tarCellsVM(const String16& name) = 0;
    virtual status_t vmSystemReady(const String16& name) = 0;
    virtual status_t sendCellsVM(const String16& path, const String16& address) = 0;
};

class BnCellsPrivateService : public BnInterface<ICellsPrivateService>
{
    virtual status_t onTransact(uint32_t code,
                                const Parcel& data,
                                Parcel* reply,
                                uint32_t flags = 0);
};

};

#endif
