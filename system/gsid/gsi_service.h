/*
 * Copyright (C) 2019 The Android Open Source Project
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
#pragma once

#include <map>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <vector>

#include <android-base/unique_fd.h>
#include <android/gsi/BnGsiService.h>
#include <binder/BinderService.h>
#include <libfiemap_writer/split_fiemap_writer.h>
#include <liblp/builder.h>
#include "libgsi/libgsi.h"

namespace android {
namespace gsi {

class GsiService : public BinderService<GsiService>, public BnGsiService {
  public:
    static void Register();

    GsiService();
    ~GsiService() override;

    binder::Status startGsiInstall(int64_t gsiSize, int64_t userdataSize, bool wipeUserdata,
                                   int* _aidl_return) override;
    binder::Status beginGsiInstall(const GsiInstallParams& params, int* _aidl_return) override;
    binder::Status commitGsiChunkFromStream(const ::android::os::ParcelFileDescriptor& stream,
                                            int64_t bytes, bool* _aidl_return) override;
    binder::Status getInstallProgress(::android::gsi::GsiProgress* _aidl_return) override;
    binder::Status commitGsiChunkFromMemory(const ::std::vector<uint8_t>& bytes,
                                            bool* _aidl_return) override;
    binder::Status cancelGsiInstall(bool* _aidl_return) override;
    binder::Status setGsiBootable(bool oneShot, int* _aidl_return) override;
    binder::Status isGsiEnabled(bool* _aidl_return) override;
    binder::Status removeGsiInstall(bool* _aidl_return) override;
    binder::Status disableGsiInstall(bool* _aidl_return) override;
    binder::Status isGsiRunning(bool* _aidl_return) override;
    binder::Status isGsiInstalled(bool* _aidl_return) override;
    binder::Status isGsiInstallInProgress(bool* _aidl_return) override;
    binder::Status getUserdataImageSize(int64_t* _aidl_return) override;
    binder::Status getGsiBootStatus(int* _aidl_return) override;
    binder::Status getInstalledGsiImageDir(std::string* _aidl_return) override;
    binder::Status wipeGsiUserdata(int* _aidl_return) override;

    static char const* getServiceName() { return kGsiServiceName; }

    static void RunStartupTasks();

    // This helper class will redirect writes to either a SplitFiemap or
    // device-mapper.
    class WriteHelper {
      public:
        virtual ~WriteHelper() {};
        virtual bool Write(const void* data, uint64_t bytes) = 0;
        virtual bool Flush() = 0;
        virtual uint64_t Size() = 0;

        WriteHelper() = default;
        WriteHelper(const WriteHelper&) = delete;
        WriteHelper& operator=(const WriteHelper&) = delete;
        WriteHelper& operator=(WriteHelper&&) = delete;
        WriteHelper(WriteHelper&&) = delete;
    };

  private:
    using LpMetadata = android::fs_mgr::LpMetadata;
    using MetadataBuilder = android::fs_mgr::MetadataBuilder;
    using SplitFiemap = android::fiemap_writer::SplitFiemap;

    struct Image {
        std::unique_ptr<SplitFiemap> writer;
        uint64_t actual_size;
    };

    int ValidateInstallParams(GsiInstallParams* params);
    int StartInstall(const GsiInstallParams& params);
    int PerformSanityChecks();
    int PreallocateFiles();
    int PreallocateUserdata();
    int PreallocateSystem();
    int DetermineReadWriteMethod();
    bool FormatUserdata();
    bool CommitGsiChunk(int stream_fd, int64_t bytes);
    bool CommitGsiChunk(const void* data, size_t bytes);
    int SetGsiBootable(bool one_shot);
    int ReenableGsi(bool one_shot);
    int WipeUserdata();
    bool DisableGsiInstall();
    bool AddPartitionFiemap(android::fs_mgr::MetadataBuilder* builder,
                            android::fs_mgr::Partition* partition, const Image& image,
                            const std::string& block_device);
    std::unique_ptr<LpMetadata> CreateMetadata();
    std::unique_ptr<SplitFiemap> CreateFiemapWriter(const std::string& path, uint64_t size,
                                                    int* error);
    bool CreateInstallStatusFile();
    bool CreateMetadataFile();
    bool SetBootMode(bool one_shot);
    void PostInstallCleanup();

    void StartAsyncOperation(const std::string& step, int64_t total_bytes);
    void UpdateProgress(int status, int64_t bytes_processed);
    int GetExistingImage(const LpMetadata& metadata, const std::string& name, Image* image);
    std::unique_ptr<WriteHelper> OpenPartition(const std::string& name);

    enum class AccessLevel {
        System,
        SystemOrShell
    };
    binder::Status CheckUid(AccessLevel level = AccessLevel::System);

    static bool RemoveGsiFiles(const std::string& install_dir, bool wipeUserdata);
    static std::string GetImagePath(const std::string& image_dir, const std::string& name);
    static std::string GetInstalledImagePath(const std::string& name);
    static std::string GetInstalledImageDir();

    std::mutex main_lock_;

    // Set before installation starts, to determine whether or not to delete
    // the userdata image if installation fails.
    bool wipe_userdata_on_failure_;

    // These are initialized or set in StartInstall().
    bool installing_ = false;
    std::atomic<bool> should_abort_ = false;
    std::string install_dir_;
    std::string userdata_gsi_path_;
    std::string system_gsi_path_;
    uint64_t userdata_block_size_;
    uint64_t system_block_size_;
    uint64_t gsi_size_;
    uint64_t userdata_size_;
    bool can_use_devicemapper_;
    bool wipe_userdata_;
    // Remaining data we're waiting to receive for the GSI image.
    uint64_t gsi_bytes_written_;

    // Progress bar state.
    std::mutex progress_lock_;
    GsiProgress progress_;

    std::unique_ptr<WriteHelper> system_writer_;

    // This is used to track which GSI partitions have been created.
    std::map<std::string, Image> partitions_;
    std::unique_ptr<LpMetadata> metadata_;
};

}  // namespace gsi
}  // namespace android
