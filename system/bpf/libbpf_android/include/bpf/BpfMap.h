/*
 * Copyright (C) 2018 The Android Open Source Project
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

#ifndef BPF_BPFMAP_H
#define BPF_BPFMAP_H

#include <linux/bpf.h>

#include <android-base/stringprintf.h>
#include <android-base/unique_fd.h>
#include <utils/Log.h>
#include "bpf/BpfUtils.h"
#include "netdutils/Status.h"
#include "netdutils/StatusOr.h"

namespace android {
namespace bpf {

// This is a class wrapper for eBPF maps. The eBPF map is a special in-kernel
// data structure that stores data in <Key, Value> pairs. It can be read/write
// from userspace by passing syscalls with the map file descriptor. This class
// is used to generalize the procedure of interacting with eBPF maps and hide
// the implementation detail from other process. Besides the basic syscalls
// wrapper, it also provides some useful helper functions as well as an iterator
// nested class to iterate the map more easily.
//
// NOTE: A kernel eBPF map may be accessed by both kernel and userspace
// processes at the same time. Or if the map is pinned as a virtual file, it can
// be obtained by multiple eBPF map class object and accessed concurrently.
// Though the map class object and the underlying kernel map are thread safe, it
// is not safe to iterate over a map while another thread or process is deleting
// from it. In this case the iteration can return duplicate entries.
template <class Key, class Value>
class BpfMap {
  public:
    BpfMap<Key, Value>() : mMapFd(-1){};
    explicit BpfMap<Key, Value>(int fd) : mMapFd(fd){};
    BpfMap<Key, Value>(bpf_map_type map_type, uint32_t max_entries, uint32_t map_flags) {
        int map_fd = createMap(map_type, sizeof(Key), sizeof(Value), max_entries, map_flags);
        if (map_fd < 0) {
            mMapFd.reset(-1);
        } else {
            mMapFd.reset(map_fd);
        }
    }

    netdutils::StatusOr<Key> getFirstKey() const {
        Key firstKey;
        if (getFirstMapKey(mMapFd, &firstKey)) {
            return netdutils::statusFromErrno(
                errno, base::StringPrintf("Get firstKey map %d failed", mMapFd.get()));
        }
        return firstKey;
    }

    netdutils::StatusOr<Key> getNextKey(const Key& key) const {
        Key nextKey;
        if (getNextMapKey(mMapFd, const_cast<Key*>(&key), &nextKey)) {
            return netdutils::statusFromErrno(
                errno, base::StringPrintf("Get next key of map %d failed", mMapFd.get()));
        }
        return nextKey;
    }

    netdutils::Status writeValue(const Key& key, const Value& value, uint64_t flags) {
        if (writeToMapEntry(mMapFd, const_cast<Key*>(&key), const_cast<Value*>(&value), flags)) {
            return netdutils::statusFromErrno(
                errno, base::StringPrintf("write to map %d failed", mMapFd.get()));
        }
        return netdutils::status::ok;
    }

    netdutils::StatusOr<Value> readValue(const Key key) const {
        Value value;
        if (findMapEntry(mMapFd, const_cast<Key*>(&key), &value)) {
            return netdutils::statusFromErrno(
                errno, base::StringPrintf("read value of map %d failed", mMapFd.get()));
        }
        return value;
    }

    netdutils::Status deleteValue(const Key& key) {
        if (deleteMapEntry(mMapFd, const_cast<Key*>(&key))) {
            return netdutils::statusFromErrno(
                errno, base::StringPrintf("delete entry from map %d failed", mMapFd.get()));
        }
        return netdutils::status::ok;
    }

    // Function that tries to get map from a pinned path.
    netdutils::Status init(const char* path);

    // Iterate through the map and handle each key retrieved based on the filter
    // without modification of map content.
    netdutils::Status iterate(
        const std::function<netdutils::Status(const Key& key, const BpfMap<Key, Value>& map)>&
            filter) const;

    // Iterate through the map and get each <key, value> pair, handle each <key,
    // value> pair based on the filter without modification of map content.
    netdutils::Status iterateWithValue(
        const std::function<netdutils::Status(const Key& key, const Value& value,
                                              const BpfMap<Key, Value>& map)>& filter) const;

    // Iterate through the map and handle each key retrieved based on the filter
    netdutils::Status iterate(
        const std::function<netdutils::Status(const Key& key, BpfMap<Key, Value>& map)>& filter);

    // Iterate through the map and get each <key, value> pair, handle each <key,
    // value> pair based on the filter.
    netdutils::Status iterateWithValue(
        const std::function<netdutils::Status(const Key& key, const Value& value,
                                              BpfMap<Key, Value>& map)>& filter);

    const base::unique_fd& getMap() const { return mMapFd; };

    // Move constructor
    void operator=(BpfMap<Key, Value>&& other) noexcept {
        mMapFd = std::move(other.mMapFd);
        other.reset();
    }

    void reset(int fd = -1) {
        mMapFd.reset(fd);
    }

    bool isValid() const { return mMapFd != -1; }

    // It is only safe to call this method if it is guaranteed that nothing will concurrently
    // iterate over the map in any process.
    netdutils::Status clear() {
        const auto deleteAllEntries = [](const Key& key, BpfMap<Key, Value>& map) {
            netdutils::Status res = map.deleteValue(key);
            if (!isOk(res) && (res.code() != ENOENT)) {
                ALOGE("Failed to delete data %s\n", strerror(res.code()));
            }
            return netdutils::status::ok;
        };
        RETURN_IF_NOT_OK(iterate(deleteAllEntries));
        return netdutils::status::ok;
    }

    netdutils::StatusOr<bool> isEmpty() const {
        auto key = this->getFirstKey();
        // Return error code ENOENT means the map is empty
        if (!isOk(key) && key.status().code() == ENOENT) return true;
        RETURN_IF_NOT_OK(key);
        return false;
    }

  private:
    base::unique_fd mMapFd;
};

template <class Key, class Value>
netdutils::Status BpfMap<Key, Value>::init(const char* path) {
    mMapFd = base::unique_fd(mapRetrieve(path, 0));
    if (mMapFd == -1) {
        reset();
        return netdutils::statusFromErrno(
                errno,
                base::StringPrintf("pinned map not accessible or does not exist: (%s)\n", path));
    }
    return netdutils::status::ok;
}

template <class Key, class Value>
netdutils::Status BpfMap<Key, Value>::iterate(
    const std::function<netdutils::Status(const Key& key, const BpfMap<Key, Value>& map)>& filter)
    const {
    netdutils::StatusOr<Key> curKey = getFirstKey();
    while (isOk(curKey)) {
        const netdutils::StatusOr<Key>& nextKey = getNextKey(curKey.value());
        RETURN_IF_NOT_OK(filter(curKey.value(), *this));
        curKey = nextKey;
    }
    return curKey.status().code() == ENOENT ? netdutils::status::ok : curKey.status();
}

template <class Key, class Value>
netdutils::Status BpfMap<Key, Value>::iterateWithValue(
    const std::function<netdutils::Status(const Key& key, const Value& value,
                                          const BpfMap<Key, Value>& map)>& filter) const {
    netdutils::StatusOr<Key> curKey = getFirstKey();
    while (isOk(curKey)) {
        const netdutils::StatusOr<Key>& nextKey = getNextKey(curKey.value());
        Value curValue;
        ASSIGN_OR_RETURN(curValue, this->readValue(curKey.value()));
        RETURN_IF_NOT_OK(filter(curKey.value(), curValue, *this));
        curKey = nextKey;
    }
    return curKey.status().code() == ENOENT ? netdutils::status::ok : curKey.status();
}

template <class Key, class Value>
netdutils::Status BpfMap<Key, Value>::iterate(
    const std::function<netdutils::Status(const Key& key, BpfMap<Key, Value>& map)>& filter) {
    netdutils::StatusOr<Key> curKey = getFirstKey();
    while (isOk(curKey)) {
        const netdutils::StatusOr<Key>& nextKey = getNextKey(curKey.value());
        RETURN_IF_NOT_OK(filter(curKey.value(), *this));
        curKey = nextKey;
    }
    return curKey.status().code() == ENOENT ? netdutils::status::ok : curKey.status();
}

template <class Key, class Value>
netdutils::Status BpfMap<Key, Value>::iterateWithValue(
    const std::function<netdutils::Status(const Key& key, const Value& value,
                                          BpfMap<Key, Value>& map)>& filter) {
    netdutils::StatusOr<Key> curKey = getFirstKey();
    while (isOk(curKey)) {
        const netdutils::StatusOr<Key>& nextKey = getNextKey(curKey.value());
        Value curValue;
        ASSIGN_OR_RETURN(curValue, this->readValue(curKey.value()));
        RETURN_IF_NOT_OK(filter(curKey.value(), curValue, *this));
        curKey = nextKey;
    }
    return curKey.status().code() == ENOENT ? netdutils::status::ok : curKey.status();
}

}  // namespace bpf
}  // namespace android

#endif
