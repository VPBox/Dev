/*
 * Copyright (C) 2009 The Android Open Source Project
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

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <vector>

#include <android/security/keystore/IKeystoreService.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>

#include <keystore/keystore.h>

using namespace android;
using namespace keystore;
using android::security::keystore::IKeystoreService;

static const char* responses[] = {
    nullptr,
    /* [NO_ERROR]           = */ "No error",
    /* [LOCKED]             = */ "Locked",
    /* [UNINITIALIZED]      = */ "Uninitialized",
    /* [SYSTEM_ERROR]       = */ "System error",
    /* [PROTOCOL_ERROR]     = */ "Protocol error",
    /* [PERMISSION_DENIED]  = */ "Permission denied",
    /* [KEY_NOT_FOUND]      = */ "Key not found",
    /* [VALUE_CORRUPTED]    = */ "Value corrupted",
    /* [UNDEFINED_ACTION]   = */ "Undefined action",
    /* [WRONG_PASSWORD]     = */ "Wrong password (last chance)",
    /* [WRONG_PASSWORD + 1] = */ "Wrong password (2 tries left)",
    /* [WRONG_PASSWORD + 2] = */ "Wrong password (3 tries left)",
    /* [WRONG_PASSWORD + 3] = */ "Wrong password (4 tries left)",
};

#define NO_ARG_INT_RETURN(cmd) \
    do { \
        if (strcmp(argv[1], #cmd) == 0) { \
            int32_t ret = -1; \
            service->cmd(&ret); \
            if (ret < 0) { \
                fprintf(stderr, "%s: could not connect: %d\n", argv[0], ret); \
                return 1; \
            } else { \
                printf(#cmd ": %s (%d)\n", responses[ret], ret); \
                return 0; \
            } \
        } \
    } while (0)

#define SINGLE_ARG_INT_RETURN(cmd) \
    do { \
        if (strcmp(argv[1], #cmd) == 0) { \
            if (argc < 3) { \
                fprintf(stderr, "Usage: %s " #cmd " <name>\n", argv[0]); \
                return 1; \
            } \
            int32_t ret = -1; \
            service->cmd(String16(argv[2]), &ret); \
            if (ret < 0) { \
                fprintf(stderr, "%s: could not connect: %d\n", argv[0], ret); \
                return 1; \
            } else { \
                printf(#cmd ": %s (%d)\n", responses[ret], ret); \
                return 0; \
            } \
        } \
    } while (0)

#define SINGLE_INT_ARG_INT_RETURN(cmd) \
    do { \
        if (strcmp(argv[1], #cmd) == 0) { \
            if (argc < 3) { \
                fprintf(stderr, "Usage: %s " #cmd " <name>\n", argv[0]); \
                return 1; \
            } \
            int32_t ret = -1; \
            service->cmd(atoi(argv[2]), &ret); \
            if (ret < 0) { \
                fprintf(stderr, "%s: could not connect: %d\n", argv[0], ret); \
                return 1; \
            } else { \
                printf(#cmd ": %s (%d)\n", responses[ret], ret); \
                return 0; \
            } \
        } \
    } while (0)

#define SINGLE_ARG_PLUS_UID_INT_RETURN(cmd) \
    do { \
        if (strcmp(argv[1], #cmd) == 0) { \
            if (argc < 3) { \
                fprintf(stderr, "Usage: %s " #cmd " <name> <uid>\n", argv[0]); \
                return 1; \
            } \
            int uid = -1; \
            if (argc > 3) { \
                uid = atoi(argv[3]); \
                fprintf(stderr, "Running as uid %d\n", uid); \
            } \
            int32_t ret = -1; \
            service->cmd(String16(argv[2]), uid, &ret); \
            if (ret < 0) { \
                fprintf(stderr, "%s: could not connect: %d\n", argv[0], ret); \
                return 1; \
            } else { \
                printf(#cmd ": %s (%d)\n", responses[ret], ret); \
                return 0; \
            } \
        } \
    } while (0)

#define SINGLE_ARG_PLUS_UID_DATA_RETURN(cmd) \
    do { \
        if (strcmp(argv[1], #cmd) == 0) { \
            if (argc < 3) { \
                fprintf(stderr, "Usage: %s " #cmd " <name> <uid>\n", argv[0]); \
                return 1; \
            } \
            std::vector<uint8_t> data; \
            int uid = -1; \
            if (argc > 3) { \
                uid = atoi(argv[3]); \
                fprintf(stderr, "Running as uid %d\n", uid); \
            } \
            ::android::binder::Status ret = service->cmd(String16(argv[2]), uid, &data); \
            if (!ret.isOk()) { \
                fprintf(stderr, "Exception code: %d\n", ret.exceptionCode()); \
                return 1; \
            } else { \
                fwrite(&data[0], data.size(), 1, stdout); \
                fflush(stdout); \
                return 0; \
            } \
        } \
    } while (0)

#define STRING_ARG_DATA_STDIN_INT_RETURN(cmd) \
    do { \
        if (strcmp(argv[1], #cmd) == 0) { \
            if (argc < 3) { \
                fprintf(stderr, "Usage: %s " #cmd " <name>\n", argv[0]); \
                return 1; \
            } \
            uint8_t* data; \
            size_t dataSize; \
            read_input(&data, &dataSize); \
            int32_t ret = -1; \
            service->cmd(String16(argv[2]), data, dataSize, &ret); \
            if (ret < 0) { \
                fprintf(stderr, "%s: could not connect: %d\n", argv[0], ret); \
                return 1; \
            } else { \
                printf(#cmd ": %s (%d)\n", responses[ret], ret); \
                return 0; \
            } \
        } \
    } while (0)

#define SINGLE_ARG_DATA_RETURN(cmd) \
    do { \
        if (strcmp(argv[1], #cmd) == 0) { \
            if (argc < 3) { \
                fprintf(stderr, "Usage: %s " #cmd " <name>\n", argv[0]); \
                return 1; \
            } \
            std::vector<uint8_t> data; \
            ::android::binder::Status ret = service->cmd(String16(argv[2]), &data); \
            if (!ret.isOk()) { \
                fprintf(stderr, "Exception code: %d\n", ret.exceptionCode()); \
                return 1; \
            } else { \
                fwrite(&data[0], data.size(), 1, stdout); \
                fflush(stdout); \
                return 0; \
            } \
        } \
    } while (0)

static int list(const sp<IKeystoreService>& service, const String16& name, int uid) {
    std::vector<String16> matches;
    ::android::binder::Status ret = service->list(name, uid, &matches);

    if (!ret.isOk()) {
        fprintf(stderr, "list: exception (%d)\n", ret.exceptionCode());
        return 1;
    } else {
        std::vector<String16>::const_iterator it = matches.begin();
        for (; it != matches.end(); ++it) {
            printf("%s\n", String8(*it).string());
        }
        return 0;
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s action [parameter ...]\n", argv[0]);
        return 1;
    }

    sp<IServiceManager> sm = initdefaultServiceManager();
    sp<IBinder> binder = sm->getService(String16("android.security.keystore"));
    sp<IKeystoreService> service = interface_cast<IKeystoreService>(binder);

    if (service == nullptr) {
        fprintf(stderr, "%s: error: could not connect to keystore service\n", argv[0]);
        return 1;
    }

    /*
     * All the commands should return a value
     */

    SINGLE_INT_ARG_INT_RETURN(getState);

    SINGLE_ARG_PLUS_UID_DATA_RETURN(get);

    // TODO: insert

    SINGLE_ARG_PLUS_UID_INT_RETURN(del);

    SINGLE_ARG_PLUS_UID_INT_RETURN(exist);

    if (strcmp(argv[1], "list") == 0) {
        return list(service, argc < 3 ? String16("") : String16(argv[2]),
                argc < 4 ? -1 : atoi(argv[3]));
    }

    NO_ARG_INT_RETURN(reset);

    // TODO: notifyUserPasswordChanged

    SINGLE_INT_ARG_INT_RETURN(lock);

    // TODO: unlock

    SINGLE_INT_ARG_INT_RETURN(isEmpty);

    // TODO: generate

    // TODO: grant

    // TODO: ungrant

    // TODO: getmtime

    fprintf(stderr, "%s: unknown command: %s\n", argv[0], argv[1]);
    return 1;
}
