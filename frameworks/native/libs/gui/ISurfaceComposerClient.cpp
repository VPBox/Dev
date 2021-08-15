/*
 * Copyright (C) 2007 The Android Open Source Project
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

// tag as surfaceflinger
#define LOG_TAG "SurfaceFlinger"

#include <gui/ISurfaceComposerClient.h>

#include <gui/IGraphicBufferProducer.h>

#include <binder/SafeInterface.h>

#include <ui/FrameStats.h>

#include <cutils/properties.h>

namespace android {

namespace { // Anonymous

enum class Tag : uint32_t {
    CREATE_SURFACE = IBinder::FIRST_CALL_TRANSACTION,
    CREATE_WITH_SURFACE_PARENT,
    CLEAR_LAYER_FRAME_STATS,
    GET_LAYER_FRAME_STATS,
    LAST = GET_LAYER_FRAME_STATS,
};

} // Anonymous namespace

class BpSurfaceComposerClient : public SafeBpInterface<ISurfaceComposerClient> {
public:
    explicit BpSurfaceComposerClient(const sp<IBinder>& impl)
          : SafeBpInterface<ISurfaceComposerClient>(impl, "BpSurfaceComposerClient") {}

    ~BpSurfaceComposerClient() override;

    status_t createSurface(const String8& name, uint32_t width, uint32_t height, PixelFormat format,
                           uint32_t flags, const sp<IBinder>& parent, LayerMetadata metadata,
                           sp<IBinder>* handle, sp<IGraphicBufferProducer>* gbp) override {
        char systemname[PROPERTY_VALUE_MAX]={0};
        property_get("persist.sys.vm.name", systemname, "");

        Parcel data, reply;
        data.writeInterfaceToken(ISurfaceComposerClient::getInterfaceDescriptor());
        data.writeString8(name);
        data.writeString8(String8(systemname));
        data.writeUint32(width);
        data.writeUint32(height);
        data.writeInt32(static_cast<int32_t>(format));
        data.writeUint32(flags);
        data.writeStrongBinder(parent);
        metadata.writeToParcel(&data);
        remote()->transact(static_cast<uint32_t>(Tag::CREATE_SURFACE), data, &reply);
        *handle = reply.readStrongBinder();
        *gbp = interface_cast<IGraphicBufferProducer>(reply.readStrongBinder());
        return reply.readInt32();
    }

    status_t createWithSurfaceParent(const String8& name, uint32_t width, uint32_t height,
                                     PixelFormat format, uint32_t flags,
                                     const sp<IGraphicBufferProducer>& parent,
                                     LayerMetadata metadata, sp<IBinder>* handle,
                                     sp<IGraphicBufferProducer>* gbp) override {
        char systemname[PROPERTY_VALUE_MAX]={0};
        property_get("persist.sys.vm.name", systemname, "");

        Parcel data, reply;
        data.writeInterfaceToken(ISurfaceComposerClient::getInterfaceDescriptor());
        data.writeString8(name);
        data.writeString8(String8(systemname));
        data.writeUint32(width);
        data.writeUint32(height);
        data.writeInt32(static_cast<int32_t>(format));
        data.writeUint32(flags);
        data.writeStrongBinder(IInterface::asBinder(parent));
        metadata.writeToParcel(&data);
        remote()->transact(static_cast<uint32_t>(Tag::CREATE_WITH_SURFACE_PARENT), data, &reply);
        *handle = reply.readStrongBinder();
        *gbp = interface_cast<IGraphicBufferProducer>(reply.readStrongBinder());
        return reply.readInt32();
    }

    status_t clearLayerFrameStats(const sp<IBinder>& handle) const override {
        return callRemote<decltype(
                &ISurfaceComposerClient::clearLayerFrameStats)>(Tag::CLEAR_LAYER_FRAME_STATS,
                                                                handle);
    }

    status_t getLayerFrameStats(const sp<IBinder>& handle, FrameStats* outStats) const override {
        return callRemote<decltype(
                &ISurfaceComposerClient::getLayerFrameStats)>(Tag::GET_LAYER_FRAME_STATS, handle,
                                                              outStats);
    }
};

// Out-of-line virtual method definition to trigger vtable emission in this
// translation unit (see clang warning -Wweak-vtables)
BpSurfaceComposerClient::~BpSurfaceComposerClient() {}

IMPLEMENT_META_INTERFACE(SurfaceComposerClient, "android.ui.ISurfaceComposerClient");

// ----------------------------------------------------------------------

status_t BnSurfaceComposerClient::onTransact(uint32_t code, const Parcel& data, Parcel* reply,
                                             uint32_t flags) {
    if (code < IBinder::FIRST_CALL_TRANSACTION || code > static_cast<uint32_t>(Tag::LAST)) {
        return BBinder::onTransact(code, data, reply, flags);
    }
    auto tag = static_cast<Tag>(code);
    switch (tag) {
        case Tag::CREATE_SURFACE:{
            CHECK_INTERFACE(ISurfaceComposerClient, data, reply);
            String8 name = data.readString8();
            String8 systemname = data.readString8();
            uint32_t width = data.readUint32();
            uint32_t height = data.readUint32();
            PixelFormat format = static_cast<PixelFormat>(data.readInt32());
            uint32_t createFlags = data.readUint32();
            sp<IBinder> parent = data.readStrongBinder();
            LayerMetadata metadata;
            metadata.readFromParcel(&data);
            sp<IBinder> handle;
            sp<IGraphicBufferProducer> gbp;
            status_t result = createSurfaceX(name, systemname, width, height, format,
                    createFlags, parent, metadata, &handle, &gbp);
            reply->writeStrongBinder(handle);
            reply->writeStrongBinder(IInterface::asBinder(gbp));
            reply->writeInt32(result);
            return NO_ERROR;
        }
        case Tag::CREATE_WITH_SURFACE_PARENT:{
            CHECK_INTERFACE(ISurfaceComposerClient, data, reply);
            String8 name = data.readString8();
            String8 systemname = data.readString8();
            uint32_t width = data.readUint32();
            uint32_t height = data.readUint32();
            PixelFormat format = static_cast<PixelFormat>(data.readInt32());
            uint32_t createFlags = data.readUint32();
            sp<IGraphicBufferProducer> parent = interface_cast<IGraphicBufferProducer>(data.readStrongBinder());
            LayerMetadata metadata;
            metadata.readFromParcel(&data);
            sp<IBinder> handle;
            sp<IGraphicBufferProducer> gbp;
            status_t result = createWithSurfaceParentX(name, systemname, width, height, format,
                    createFlags, parent, metadata, &handle, &gbp);
            reply->writeStrongBinder(handle);
            reply->writeStrongBinder(IInterface::asBinder(gbp));
            reply->writeInt32(result);
            return NO_ERROR;
        }
        case Tag::CLEAR_LAYER_FRAME_STATS:
            return callLocal(data, reply, &ISurfaceComposerClient::clearLayerFrameStats);
        case Tag::GET_LAYER_FRAME_STATS:
            return callLocal(data, reply, &ISurfaceComposerClient::getLayerFrameStats);
    }
}

} // namespace android
