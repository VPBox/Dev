#define LOG_TAG "GuiExt"

#include <cutils/log.h>
#include <binder/BinderService.h>
#include "ICellsPrivateService.h"

using namespace android;

int main(int argc, char** argv)
{
   const sp<IServiceManager> sm = defaultServiceManager();
    if (sm != NULL) {
        sp<IBinder> binder = sm->checkService(String16("CellsPrivateService"));
        if (binder != NULL) {
            sp<ICellsPrivateService> pCellsPrivateService = interface_cast<ICellsPrivateService>(binder);
            if(pCellsPrivateService == NULL){
                ALOGE("could not get service CellsPrivateService \n");
                return 0;
            }

            char value[PROPERTY_VALUE_MAX];
            property_get("ro.boot.vm", value, "1");
            if((strcmp(value, "0") == 0)){
                if(argc == 2){
                    pCellsPrivateService->switchCellsVM(android::String16(argv[1]));
                }
            }else{
                pCellsPrivateService->switchCellsVM(android::String16("host"));
            }
        }
    }

    return 0;
}
