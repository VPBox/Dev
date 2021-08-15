#pragma GCC system_header
#ifndef __GUIEXT_SERVICE_H__
#define __GUIEXT_SERVICE_H__

#include <utils/threads.h>
#include "ICellsPrivateService.h"

namespace android
{

class String16;
class CellsPrivateService :
        public BinderService<CellsPrivateService>,
        public BnCellsPrivateService
//        public Thread
{
    friend class BinderService<CellsPrivateService>;
public:

    CellsPrivateService();
    ~CellsPrivateService();

    static char const* getServiceName() { return "CellsPrivateService"; }

    virtual status_t setProperty(const String16& name,const String16& value);
    virtual status_t startCellsVM(const String16& name);
    virtual status_t stopCellsVM(const String16& name);
    virtual status_t cellsSwitchVM(const String16& name);
    virtual status_t cellsSwitchHOST(const String16& name);
    virtual status_t enterCell(const String16& name);
    virtual status_t enterHost(const String16& name);
    virtual status_t exitCell(const String16& name);
    virtual status_t exitHost(const String16& name);
    virtual status_t switchCellsVM(const String16& name);
    virtual status_t uploadCellsVM(const String16& name);
    virtual status_t downloadCellsVM(const String16& name);
    virtual status_t untarCellsVM(const String16& name);
    virtual status_t tarCellsVM(const String16& name);
    virtual status_t vmSystemReady(const String16& name);
    virtual status_t sendCellsVM(const String16& path, const String16& address);

    int isInCellstar();
    void setCellstaring();
    void setCellstared();
    void startCellstar();

private:
    int mtar_pthread_t;
};
};
#endif
