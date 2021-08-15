#define LOG_TAG "GuiExt"

#include <cutils/log.h>
#include <binder/BinderService.h>
#include "ICellsPrivateService.h"

using namespace android;

int main(int argc, char** argv)
{
    const sp<IServiceManager> sm = initdefaultServiceManager();
    if (sm != NULL) {
        sp<IBinder> binder = sm->checkService(String16("CellsPrivateService"));
        if (binder != NULL) {
            sp<ICellsPrivateService> pCellsPrivateService = interface_cast<ICellsPrivateService>(binder);
            if(pCellsPrivateService == NULL){
                ALOGE("could not get service CellsPrivateService \n");
                return 0;
            }

            if (argc == 2) {
                char value[PROPERTY_VALUE_MAX];
                property_get(argv[1], value, "");
                pCellsPrivateService->setProperty(android::String16(argv[1]),android::String16(value));
            }else if (argc == 3) {
                pCellsPrivateService->setProperty(android::String16(argv[1]),android::String16(argv[2]));
            }
        }
    }

    return 0;
}
