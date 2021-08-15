// Copyright (C) 2018 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "common/debug.h"
#include "inode2filename/search_directories.h"
#include "inode2filename/system_call.h"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/scopeguard.h>
#include <android-base/stringprintf.h>
#include <android-base/unique_fd.h>

#include "rxcpp/rx.hpp"

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <optional>

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>

#ifdef __ANDROID__
#include <sys/sysmacros.h>
#endif

#include <sys/stat.h>
#include <fcntl.h>
#include <poll.h>
#include <dirent.h>

#include <unordered_map>

namespace rx = rxcpp;
using android::base::unique_fd;  // NOLINT
using android::base::StringPrintf;  // NOLINT

namespace iorap::inode2filename {

// A multimap of 'ino_t -> List[Inode]' (where the value Inodes have the same ino_t as the key).
//
// A flat list of Inodes is turned into the above map, then keys can be removed one at a time
// until the InodeSet eventually becomes empty.
struct InodeSet {
  struct ValueRange {
    auto/*Iterable<Inode>*/ begin() {
      return begin_;
    }

    auto/*Iterable<Inode>*/ end() {
      return end_;
    }

    bool empty() const {
      return begin_ == end_;
    }

    explicit operator bool() const {
      return !empty();
    }

    std::unordered_multimap<ino_t, Inode>::iterator begin_, end_;

    friend std::ostream& operator<<(std::ostream& os, const ValueRange& s);
  };

  // Create an observable that emits the remaining inodes in the map.
  //
  // Mutation functions must not be called until this observable
  // has been finished emitting all values (e.g. with on_completed) since that
  // would cause the underlying iterators to go into an undefined state.
  auto/*observable<Inode>*/ IterateValues() const {
    return rxcpp::observable<>::iterate(set_).map(  // XX: should we use identity_immediate here?
        [](const std::pair<const ino_t, Inode>& pair) {
          return pair.second;
        }
    );
    // TODO: this would be more efficient as a range-v3 view.
  }

  constexpr bool Empty() const {
    return set_.empty();
  }

  static InodeSet OfList(const std::vector<Inode>& list) {
    InodeSet new_inode_set;
    std::unordered_multimap<ino_t, Inode>* map = &new_inode_set.set_;

    for (const Inode& inode : list) {
      map->insert({inode.inode, inode});
    }

    return new_inode_set;
  }

  // Return an optional list of 'Inode' structs whose 'inode' field matches the 'inode' parameter.
  // Returns an empty range if there was nothing found.
  ValueRange FindInodeList(ino_t inode) {
    auto range = set_.equal_range(inode);
    return ValueRange{range.first, range.second};
  }

  // Match all fields of an Inode against a 'struct stat' stat_buf.
  //
  // The returned Inode (if any) is removed from the InodeSet; it will not be returned by
  // FindInodeList in future calls.
  std::optional<Inode> FindAndRemoveInodeInList(ValueRange inode_list,
                                                const struct stat& stat_buf) {
    LOG(VERBOSE) << "FindAndRemoveInodeInList " << inode_list << ", "
                 << "stat_buf{st_dev=" << stat_buf.st_dev << ",st_ino=" << stat_buf.st_ino << "}";

    auto /*iterator*/ found = std::find_if(inode_list.begin(),
                                           inode_list.end(),
                                           [&](const std::pair<ino_t, Inode>& pair) {
      const Inode& inode = pair.second;
      if (inode.inode != stat_buf.st_ino) {
        return false;
      }

      dev_t inode_dev =
          makedev(static_cast<int>(inode.device_major), static_cast<int>(inode.device_minor));

      // Inodes could be the same across different devices.
      // Also match the device id.
      if (inode_dev != stat_buf.st_dev) {
        LOG(VERBOSE) << "InodeSet:FindAndRemoveInodeInList matched ino: " << inode.inode
                     << " but not device"
                     << ", expected dev: " << stat_buf.st_dev
                     << ", actual dev: " << inode_dev;
        return false;
      }
      return true;
    });

    if (found != inode_list.end()) {
      const Inode& inode = found->second;
      LOG(VERBOSE) << "InodeSet:FindAndRemoveInodeInList *success* inode+device " << inode;
      DCHECK(found->second.inode == stat_buf.st_ino);
      // Erase the inode from the list. This is important.
      set_.erase(found);
      return inode;
    }

    return std::nullopt;
  }

  // TODO: equality and string operators for testing/logging.
 private:
  // Explanation: readdir returns a 'file' -> 'ino_t inode' mapping.
  //
  // However inodes can be reused on different partitions (but they have a different device number).
  // To handle this edge case, and to avoid calling stat whenever the inode definitely doesn't match
  // store the inodes into a single-key,multi-value container.
  //
  // This enables fast scanning of readdir results by matching just the 'inode' portion,
  // then calling stat only when the inode portion definitely matches to confirm the device.

  // There are no single-key multi-value containers in standard C++, so pretend
  // we have one by writing this simple facade around an unordered set.
  //
  // We expect that the vector size is usually size=1 (or 2 or 3) since the # of devices
  // is fixed by however many partitions there are on the system, AND the same inode #
  // would have to be reused across a different file.
  std::unordered_multimap<ino_t, Inode> set_;  // TODO: Rename to map_.

  friend std::ostream& operator<<(std::ostream& os, const InodeSet& s);
};

std::ostream& operator<<(std::ostream& os, const InodeSet& s) {
  os << "InodeSet{";
  for (const auto& kv : s.set_) {
    // e.g. "123=>(1:2:123)" ... its expected for the 'ino_t' portion to be repeated.
    os << "" << kv.first << "=>(" << kv.second << "),";
  }
  os << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, const InodeSet::ValueRange& v) {
  // Don't want to make a const and non const version of ValueRange.
  InodeSet::ValueRange& s = const_cast<InodeSet::ValueRange&>(v);

  os << "InodeSet::ValueRange{";
  for (const auto& kv : s) {
    // e.g. "123=>(1:2:123)" ... its expected for the 'ino_t' portion to be repeated.
    os << "" << kv.first << "=>(" << kv.second << "),";
  }
  os << "}";
  return os;
}

void search_for_inodes_in(std::vector<Inode>& inode_list, const std::string& dirpath);

enum DirectoryEntryErrorCode {
  kInvalid,    // not a real error code. to detect bad initialization.
  kOpenDir,    // opendir failed.
  kReadDir,    // readdir failed.
  kDtUnknown,  // d_type was DT_UNKNOWN error.
};

struct DirectoryEntryError {
  DirectoryEntryErrorCode code;
  int err_no;
  std::string filename;
};

std::ostream& operator<<(std::ostream& os, const DirectoryEntryError& e) {
  os << "DirectoryEntryError{"
     << static_cast<int>(e.code) << "," << e.err_no << "," << e.filename << "}";
  return os;
  // TODO: pretty-print code and err-no
}

static common::DebugCounter gDebugDirectoryEntryCounter{};
static constexpr bool kDebugDirectoryEntry = false;

#define DIRECTORY_ENTRY_MOVE_DCHECK() \
    DCHECK_EQ(other.moved_from_, false) << __PRETTY_FUNCTION__ << "CNT:" << other.debug_counter_;
#define DIRECTORY_ENTRY_TRACE_CTOR() \
    if (kDebugDirectoryEntry) LOG(VERBOSE) << __PRETTY_FUNCTION__ << "@CNT:" << debug_counter_

struct DirectoryEntry {
  using ResultT = iorap::expected<DirectoryEntry, DirectoryEntryError>;
  using ObservableT = rx::observable<ResultT>;

  static constexpr ino_t kInvalidIno = std::numeric_limits<ino_t>::max();
  static constexpr auto kInvalidFileName = "";

  // Path to file, the prefix is one of the root directories.
  std::string filename{kInvalidFileName};
  // Inode number of the file. Not unique across different devices.
  ino_t d_ino{kInvalidIno};
  // File type (DT_LNK, DT_REG, DT_DIR, or DT_UNKNOWN)
  unsigned char d_type{DT_UNKNOWN};  // Note: not seen outside of sentinel roots.
  // TODO: Consider invariant checks for valid combinations of above fields?

  // Debug-only flags.
  bool moved_from_{false};
  size_t debug_counter_{0};

 private:
  // TODO: remove default constructor?
  //
  // SEEMS TO BE USED by std::vector etc. FIX DAT.
  DirectoryEntry() noexcept {
    debug_counter_ = gDebugDirectoryEntryCounter++;
    DIRECTORY_ENTRY_TRACE_CTOR();
  }
 public:
  DirectoryEntry(std::string filename, ino_t d_ino, unsigned char d_type) noexcept
    : filename{std::move(filename)},
      d_ino{d_ino},
      d_type{d_type} {
    debug_counter_ = gDebugDirectoryEntryCounter++;
    DIRECTORY_ENTRY_TRACE_CTOR();
  }

  DirectoryEntry(const DirectoryEntry& other) noexcept {
    // Do not use member-initialization syntax so that this DCHECK can execute first.
    DIRECTORY_ENTRY_MOVE_DCHECK();

    filename = other.filename;
    d_ino = other.d_ino;
    d_type = other.d_type;
    children_paths_ = other.children_paths_;
    children_initialized_ = other.children_initialized_;
    debug_counter_ = other.debug_counter_;
    DIRECTORY_ENTRY_TRACE_CTOR();
  }

  DirectoryEntry& operator=(const DirectoryEntry& other) noexcept {
    if (this == &other) {
      return *this;
    }

    DIRECTORY_ENTRY_MOVE_DCHECK();

    filename = other.filename;
    d_ino = other.d_ino;
    d_type = other.d_type;
    children_paths_ = other.children_paths_;
    children_initialized_ = other.children_initialized_;
    debug_counter_ = other.debug_counter_;
    DIRECTORY_ENTRY_TRACE_CTOR();

    return *this;
  }

  DirectoryEntry& operator=(DirectoryEntry&& other) noexcept {
    if (this == &other) {
      return *this;
    }

    DIRECTORY_ENTRY_MOVE_DCHECK();

    filename = std::move(other.filename);
    d_ino = other.d_ino;
    d_type = other.d_type;
    children_paths_ = std::move(other.children_paths_);
    children_initialized_ = other.children_initialized_;
    debug_counter_ = other.debug_counter_;
    DIRECTORY_ENTRY_TRACE_CTOR();

    return *this;
  }

  DirectoryEntry(DirectoryEntry&& other) noexcept {
    DIRECTORY_ENTRY_MOVE_DCHECK();
    other.moved_from_ = true;

    filename = std::move(other.filename);
    d_ino = other.d_ino;
    d_type = other.d_type;
    children_paths_ = std::move(other.children_paths_);
    children_initialized_ = other.children_initialized_;
    debug_counter_ = other.debug_counter_;
    DIRECTORY_ENTRY_TRACE_CTOR();
  }

  // Create a sentinel (root of roots) whose children entries are those specified by
  // children_paths.
  static DirectoryEntry CreateSentinel(std::vector<std::string> children_paths) {
    DirectoryEntry e;
    e.d_type = DT_DIR;
    ++gDebugDirectoryEntryCounter;

    for (std::string& child_path : children_paths) {
      // TODO: Should we call Stat on the child path here to reconstitute the ino_t for a root dir?
      // Otherwise it can look a little strange (i.e. the root dir itself will never match
      // the searched inode).
      //
      // Probably not too big of a problem in practice.
      DirectoryEntry child_entry{std::move(child_path), kInvalidIno, DT_DIR};
      ResultT child_entry_as_result{std::move(child_entry)};
      e.children_paths_.push_back(std::move(child_entry_as_result));
    }

    e.children_initialized_ = true;

    return e;
  }

  // Return an observable which emits the direct children only.
  // The children entries are now read from disk (with readdir) if they weren't read previously.
  std::vector<ResultT> GetChildrenEntries(borrowed<SystemCall*> system_call) const& {
    BuildChildrenPaths(system_call);
    return children_paths_;
  }

  // Return an observable which emits the direct children only.
  // The children entries are now read from disk (with readdir) if they weren't read previously.
  // Movable overload.
  std::vector<ResultT> GetChildrenEntries(borrowed<SystemCall*> system_call) && {
    BuildChildrenPaths(system_call);
    return std::move(children_paths_);
  }

  // Returns a (lazy) observable that emits every single node, in pre-order,
  // rooted at this tree.
  //
  // New entries are only read from disk (with e.g. readdir) when more values are pulled
  // from the observable. Only the direct children of any entry are read at any time.
  //
  // The emission can be stopped prematurely by unsubscribing from the observable.
  // This means the maximum amount of 'redundant' IO reads is bounded by the children count
  // of all entries emitted thus far minus entries actually emitted.
  ObservableT GetSubTreePreOrderEntries(borrowed<SystemCall*> system_call) const;

 private:
  // Out-of-line definition to avoid circular type dependency.
  void BuildChildrenPaths(borrowed<SystemCall*> system_call) const;

  // We need to lazily initialize children_paths_ only when we try to read them.
  //
  // Assuming the underlying file system doesn't change (which isn't strictly true),
  // the directory children are referentially transparent.
  //
  // In practice we do not need to distinguish between the file contents changing out
  // from under us in this code, so we don't need the more strict requirements.
  mutable std::vector<ResultT> children_paths_;
  mutable bool children_initialized_{false};

  friend std::ostream& operator<<(std::ostream& os, const DirectoryEntry& d);
};

std::ostream& operator<<(std::ostream& os, const DirectoryEntry& d) {
  os << "DirectoryEntry{" << d.filename << ",ino:" << d.d_ino << ",type:" << d.d_type << "}";
  return os;
}

using DirectoryEntryResult = DirectoryEntry::ResultT;

// Read all directory entries and return it as a vector. This must be an eager operation,
// as readdir is not re-entrant.
//
// This could be considered as a limitation from the 'observable' perspective since
// one can end up reading unnecessary extra directory entries that are then never consumed.
//
// The following entries are skipped:
//  - '.' self
//  - ".." parent
//
// All DT types except the following are removed:
//  * DT_LNK - symbolic link (empty children)
//  * DT_REG - regular file  (empty children)
//  * DT_DIR - directory     (has children)
static std::vector<DirectoryEntryResult>
    ReadDirectoryEntriesFromDirectoryPath(std::string dirpath, borrowed<SystemCall*> system_call) {
  DIR *dirp;
  struct dirent *dp;

  LOG(VERBOSE) << "ReadDirectoryEntriesFromDirectoryPath(" << dirpath << ")";

  if ((dirp = system_call->opendir(dirpath.c_str())) == nullptr) {
    PLOG(ERROR) << "Couldn't open directory: " << dirpath;
    return {DirectoryEntryError{kOpenDir, errno, dirpath}};
  }

  // Read all the results up front because readdir is not re-entrant.
  std::vector<DirectoryEntryResult> results;

  // Get full path + the directory entry path.
  auto child_path = [&] { return dirpath + "/" + dp->d_name; };

  do {
    errno = 0;
    if ((dp = system_call->readdir(dirp)) != nullptr) {
      if (dp->d_type == DT_DIR) {
        if (strcmp(".", dp->d_name) == 0 || strcmp("..", dp->d_name) == 0) {
          LOG(VERBOSE) << "Skip self/parent: " << dp->d_name;
          continue;
        }

        LOG(VERBOSE) << "Find entry " << child_path()
                     << ", ino: " << dp->d_ino << ", type: " << dp->d_type;
        results.push_back(DirectoryEntry{child_path(),
                                         static_cast<ino_t>(dp->d_ino),
                                         dp->d_type});
      } else if (dp->d_type == DT_UNKNOWN) {
        // This seems bad if it happens. We should probably do something about this.
        LOG(WARNING) << "Found unknown DT entry: " << child_path();

        results.push_back(DirectoryEntryError{kDtUnknown, /*errno*/0, child_path()});
      } else if (dp->d_type == DT_LNK || dp->d_type == DT_REG) {
        // Regular non-directory file entry.
        results.push_back(DirectoryEntry{child_path(),
                                         static_cast<ino_t>(dp->d_ino),
                                         dp->d_type});
      } else {
        // Block device, character device, socket, etc...
        LOG(VERBOSE) << "Skip DT entry of type: " << dp->d_type << " " << child_path();
      }
    } else if (errno != 0) {
      PLOG(ERROR) << "Error reading directory entry in " << dirpath;

      results.push_back(DirectoryEntryError{kReadDir, errno, dirpath});
    }
  } while (dp != nullptr);

  if (system_call->closedir(dirp) < 0) {
    PLOG(ERROR) << "Failed to close directory " << dirpath;
  }

  return results;
}

void DirectoryEntry::BuildChildrenPaths(borrowed<SystemCall*> system_call) const {
  if (children_initialized_) {
    return;
  }

  if (d_type == DT_DIR) {
    children_paths_ = ReadDirectoryEntriesFromDirectoryPath(filename, system_call);
    // TODO: consider using dependency injection here to substitute this function during testing?
  }
}

struct InodeSearchParameters {
  std::vector<Inode> inode_list;
  std::vector<std::string> root_dirs;
};

// [IN]
// observable: expected<Value, Error>, ...
// [OUT]
// observable: Value, ...
//
// Any encountered 'Error' items are dropped after logging.
template <typename T>
auto MapExpectedOrLog(T&& observable,
                      ::android::base::LogSeverity log_level) {
  return observable.filter([log_level](const auto& result) {
    if (result) {
      return true;
    } else {
      LOG(log_level) << result.error();
      return false;
    }
  }).map([](auto&& result) {
    return IORAP_FORWARD_LAMBDA(result).value();
  });
}

template <typename T>
auto MapExpectedOrLogError(T&& observable) {
  return MapExpectedOrLog(std::forward<T>(observable), ::android::base::ERROR);
}

template <typename T>
auto MapOptionalOrDrop(T&& observable) {
  return observable.filter([](const auto& result) {
    return result.has_value();
  }).map([](auto&& result) {
    return IORAP_FORWARD_LAMBDA(result).value();
  });
  // TODO: static_assert this isn't used with an unexpected.
}

template <typename T, typename F>
auto VisitValueOrLogError(T&& expected, F&& visit_func, const char* error_prefix = "") {
  if (!expected) {
    LOG(ERROR) << error_prefix << " " << expected.error();
  } else {
    visit_func(std::forward<T>(expected).value());
  }
  // TODO: Could be good to make this more monadic by returning an optional.
}

template <typename TSimple, typename T, typename F>
void TreeTraversalPreOrderObservableImpl(rx::subscriber<TSimple> dest, T&& node, F&& fn) {
  LOG(VERBOSE) << "TreeTraversalPreOrderObservableImpl (begin) " << __PRETTY_FUNCTION__;

  if (!dest.is_subscribed()) {
    LOG(VERBOSE) << "TreeTraversalPreOrderObservableImpl (unsubscribed)";
    return;
  } else {
    LOG(VERBOSE) << "TreeTraversalPreOrderObservableImpl (on_next node)";

    // Copy the node here. This is less bad than it seems since we haven't yet
    // calculated its children (except in the root), so its just doing a shallow memcpy (sizeof(T)).
    //
    // This assumes the children are calculated lazily, otherwise we'd need to have a separate
    // NodeBody class which only holds the non-children elements.

    TSimple copy = std::forward<T>(node);
    dest.on_next(std::move(copy));

    if (!node.has_value()) {
      return;
    }

    // Whenever we call 'on_next' also check if we end up unsubscribing.
    // This avoids the expensive call into the children.
    if (!dest.is_subscribed()) {
      LOG(VERBOSE) << "TreeTraversalPreOrderObservableImpl (post-self unsubscribe)";
      return;
    }

    // Eagerly get the childrem, moving them instead of copying them.
    auto&& children = fn(std::forward<T>(node));
    for (auto&& child : children) {
      TreeTraversalPreOrderObservableImpl(dest, IORAP_FORWARD_LAMBDA(child), fn);
      // TODO: double check this is doing the std::move properly for rvalues.

      if (!dest.is_subscribed()) {
        LOG(VERBOSE) << "TreeTraversalPreOrderObservableImpl (unsubscribed in children)";
        break;
      }
    };
  }
}

// Creates an observable over all the nodes in the tree rooted at node.
// fn is a function that returns the children of that node.
//
// The items are emitted left-to-right pre-order, and stop early if the
// observable is unsubscribed from.
//
// Implementation requirement:
//    typeof(node) -> expected<V, E> or optional<V> or similar.
//    fn(node) -> iterable<typeof(node)>
//
// preorder(self):
//   visit(self)
//   for child in fn(self):
//     preorder(child)
template <typename T, typename F>
auto/*observable<T>*/ TreeTraversalPreOrderObservable(T&& node, F&& fn) {
  LOG(VERBOSE) << "TreeTraversalPreOrderObservable: " << __PRETTY_FUNCTION__;

  using T_simple = std::decay_t<T>;
  return rx::observable<>::create<T_simple>(
    // Copy node to avoid lifetime issues.
    [node=node,fn=std::forward<F>(fn)](rx::subscriber<T_simple> dest) {
      LOG(VERBOSE) << "TreeTraversalPreOrderObservable (lambda)";
      TreeTraversalPreOrderObservableImpl<T_simple>(dest,
                                                    std::move(node),
                                                    std::move(fn));
      dest.on_completed();
    }
  );
}

DirectoryEntry::ObservableT
    DirectoryEntry::GetSubTreePreOrderEntries(borrowed<SystemCall*> system_call) const {
  return TreeTraversalPreOrderObservable(
      DirectoryEntryResult{*this},
      [system_call=system_call](auto/*DirectoryEntryResult*/&& result)
          -> std::vector<DirectoryEntryResult> {
        if (!result) {
          LOG(VERBOSE) << "GetSubTreePreOrderEntries (no value return)";
          // Cannot have children when it was an error.
          return {};
        }
        return
            IORAP_FORWARD_LAMBDA(result)
            .value()
            .GetChildrenEntries(system_call);
      });
}

struct StatError {
  int err_no;
  std::string path_name;
};

std::ostream& operator<<(std::ostream& os, const StatError& e) {
  os << "StatError{" << e.err_no << "," << e.path_name << "}";
  return os;
}

template <typename U = void>  // suppress unused warning.
static iorap::expected<struct stat, StatError> Stat(const std::string& path_name,
                                                    borrowed<SystemCall*> system_call) {
  struct stat statbuf{};

  // Call stat(2) in live code. Overridden in test code.
  if (system_call->stat(path_name.c_str(), /*out*/&statbuf) == 0) {
    return statbuf;
  } else {
    return iorap::unexpected(StatError{errno, path_name});
  }
}

using StatResult = iorap::expected<struct stat, StatError>;

// An inode's corresponding filename on the system.
struct SearchMatch {
  Inode inode;
  // Relative path joined with a root directory.
  //
  // Use absolute path root dirs to get back absolute path filenames.
  // If relative, this is relative to the current working directory.
  std::string filename;
};

std::ostream& operator<<(std::ostream& os, const SearchMatch& s) {
  os << "SearchMatch{" << s.inode << ", " << s.filename << "}";
  return os;
}

struct SearchState {
  // Emit 'match' Inodes corresponding to the ones here.
  InodeSet inode_set;

  // An inode matching one of the ones in inode_set was discovered in the most-recently
  // emitted SearchState.
  //
  // The InodeSet removes any matching 'Inode'.
  std::optional<SearchMatch> match;

  // TODO: make sure this doesn't copy [inodes], as that would be unnecessarily expensive.
};

std::ostream& operator<<(std::ostream& os, const SearchState& s) {
  os << "SearchState{match:";
  // Print the 'match' first. The InodeSet could be very large so it could be truncated in logs.
  if (s.match) {
    os << s.match.value();
  } else {
    os << "(none)";
  }
  os << ", inode_set:" << s.inode_set << "}";
  return os;
}

// TODO: write operator<< etc.

// Return a lazy observable that will search for all filenames whose inodes
// match the inodes in inode_search_list.
//
// Every unmatched inode will be emitted as an unexpected at the end of the stream.
auto/*[observable<InodeResult>, connectable]*/ SearchDirectoriesForMatchingInodes(
    std::vector<std::string> root_dirs,
    std::vector<Inode> inode_search_list,
    borrowed<SystemCall*> system_call) {

  // Create a (lazy) observable that will emit each DirectoryEntry that is a recursive subchild
  // of root_dirs. Emission will be stopped when its unsubscribed from.
  //
  // This is done by calling readdir(3) lazily.
  auto/*obs<DirectoryEntry>*/ find_all_subdir_entries = ([&]() {
    DirectoryEntry sentinel = DirectoryEntry::CreateSentinel(std::move(root_dirs));
    auto/*obs<DirectoryEntryResult*/ results = sentinel.GetSubTreePreOrderEntries(system_call);

    // Drop any errors by logging them to logcat. "Unwrap" the expected into the underlying data.
    auto/*obs<DirectoryEntry*>*/ expected_drop_errors = MapExpectedOrLogError(std::move(results));
    return expected_drop_errors;
  })();

  // DirectoryEntry is missing the dev_t portion, so we may need to call scan(2) again
  // to confirm the dev_t. We skip calling scan(2) when the ino_t does not match.
  // InodeSet lets us optimally avoid calling scan(2).
  SearchState initial;
  initial.inode_set = InodeSet::OfList(inode_search_list);

  auto/*[observable<SearchState>,Connectable]*/ search_state_results = find_all_subdir_entries.scan(
      std::move(initial),
      [system_call=system_call](SearchState search_state, const DirectoryEntry& dir_entry) {
        LOG(VERBOSE) << "SearchDirectoriesForMatchingInodes#Scan "
                     << dir_entry << ", state: " << search_state;

        search_state.match = std::nullopt;

        InodeSet* inodes = &search_state.inode_set;

        // Find all the possible inodes across different devices.
        InodeSet::ValueRange inode_list = inodes->FindInodeList(dir_entry.d_ino);

        // This directory doesn't correspond to any inodes we are searching for.
        if (!inode_list) {
          return search_state;
        }

        StatResult maybe_stat = Stat(dir_entry.filename, system_call);
        VisitValueOrLogError(maybe_stat, [&](const struct stat& stat_buf) {
          // Try to match the specific inode. Usually this will not result in a match (nullopt).
          std::optional<Inode> inode = inodes->FindAndRemoveInodeInList(inode_list, stat_buf);

          if (inode) {
            search_state.match = SearchMatch{inode.value(), dir_entry.filename};
          }
        });

        return search_state;  // implicit move.
      }
  // Avoid exhausting a potentially 'infinite' stream of files by terminating as soon
  // as we find every single inode we care about.
  ).take_while([](const SearchState& state) {
      // Also emit the last item that caused the search set to go empty.
      bool cond = !state.inode_set.Empty() || state.match;

      if (WOULD_LOG(VERBOSE)) {
        static int kCounter = 0;
        LOG(VERBOSE) << "SearchDirectoriesForMatchingInodes#take_while (" << kCounter++ <<
                     ",is_empty:"
                     << state.inode_set.Empty() << ", match:" << state.match.has_value();
      }
      // Minor O(1) implementation inefficiency:
      // (Too minor to fix but it can be strange if looking at the logs or readdir traces).
      //
      // Note, because we return 'true' after the search set went empty,
      // the overall stream graph still pulls from search_state_results exactly once more:
      //
      // This means that for cond to go to false, we would've read one extra item and then discarded
      // it. If that item was the first child of a directory, that means we essentially did
      // one redundant pass of doing a readdir.
      //
      // In other words if the search set goes to empty while the current item is a directory,
      // it will definitely readdir on it at least once as we try to get the first child in
      // OnTreeTraversal.
      //
      // This could be fixed with a 'take_until(Predicate)' operator which doesn't discard
      // the last item when the condition becomes false. However rxcpp seems to lack this operator,
      // whereas RxJava has it.

      if (!cond) {
        LOG(VERBOSE) << "SearchDirectoriesForMatchingInodes#take_while "
                     << "should now terminate for " << state;
      }

      return cond;
  }).publish();
  // The publish here is mandatory. The stream is consumed twice (once by matched and once by
  // unmatched streams). Without the publish, once all items from 'matched' were consumed it would
  // start another instance of 'search_state_results' (i.e. it appears as if the search
  // is restarted).
  //
  // By using 'publish', the search_state_results is effectively shared by both downstream nodes.
  // Note that this also requires the subscriber to additionally call #connect on the above stream,
  // otherwise no work will happen.

  // Lifetime notes:
  //
  // The the 'SearchState' is emitted into both below streams simultaneously.
  //    The 'unmatched_inode_values' only touches the inode_set.
  //    The 'matched_inode_values' only touches the match.
  // Either stream can 'std::move' from those fields because they don't move each other's fields.
  auto/*observable<InodeResult>*/ matched_inode_values = search_state_results
      .filter([](const SearchState& search_state) { return search_state.match.has_value(); })
      .map([](SearchState& search_state) { return std::move(search_state.match.value()); })
                     // observable<SearchMatch>
      .map([](SearchMatch search_match) {
          return InodeResult::makeSuccess(search_match.inode, std::move(search_match.filename));
      });            // observable<InodeResult>

  auto/*observable<?>*/ unmatched_inode_values = search_state_results
      // The 'last' SearchState is the one that contains all the remaining inodes.
      .take_last(1)  // observable<SearchState>
      .flat_map([](const SearchState& search_state) {
          LOG(VERBOSE) << "SearchDirectoriesForMatchingInodes#unmatched -- flat_map";
          // Aside: Could've used a move here if the inodes weren't so lightweight already.
          return search_state.inode_set.IterateValues(); })
                     // observable<Inode>
      .map([](const Inode& inode) {
          LOG(VERBOSE) << "SearchDirectoriesForMatchingInodes#unmatched -- map";
          return InodeResult::makeFailure(inode, InodeResult::kCouldNotFindFilename);
      });
                     // observable<InodeResult>

  // The matched and unmatched InodeResults are emitted together.
  //   Use merge, not concat, because we need both observables to be subscribed to simultaneously.

  auto/*observable<InodeResult*/ all_inode_results =
      matched_inode_values.merge(unmatched_inode_values);

  // Now that all mid-stream observables have been connected, turn the Connectable observable
  // into a regular observable.

  // The caller has to call 'connect' on the search_state_results after subscribing
  // and before any work can actually start.
  return std::make_pair(all_inode_results, search_state_results);
}


rxcpp::observable<InodeResult> SearchDirectories::FindFilenamesFromInodes(
    std::vector<std::string> root_directories,
    std::vector<Inode> inode_list,
    SearchMode mode) {
  DCHECK(mode == SearchMode::kInProcessDirect) << " other modes not implemented yet";

  auto/*observable[2]*/ [inode_results, connectable] = SearchDirectoriesForMatchingInodes(
      std::move(root_directories),
      std::move(inode_list),
      system_call_);

  return inode_results;
}

// I think we could avoid this with auto_connect, which rxcpp doesn't seem to have.
//
// I can't figure out any other way to avoid this, or at least to allow connecting
// on the primary observable (instead of a secondary side-observable).
//
// If using the obvious publish+ref_count then the unmerged stream gets no items emitted into it.
// If tried to ref_count later, everything turns into no-op.
// If trying to call connect too early, the subscribe is missed.
template <typename T>
struct RxAnyConnectableFromObservable : public SearchDirectories::RxAnyConnectable {
  virtual void connect() override {
    observable.connect();
  }

  virtual ~RxAnyConnectableFromObservable() {}

  RxAnyConnectableFromObservable(rxcpp::connectable_observable<T> observable)
    : observable(observable) {
  }

  rxcpp::connectable_observable<T> observable;
};

// Type deduction helper.
template <typename T>
std::unique_ptr<SearchDirectories::RxAnyConnectable>
    MakeRxAnyConnectableFromObservable(rxcpp::connectable_observable<T> observable) {
  SearchDirectories::RxAnyConnectable* ptr = new RxAnyConnectableFromObservable<T>{observable};
  return std::unique_ptr<SearchDirectories::RxAnyConnectable>{ptr};
}

std::pair<rxcpp::observable<InodeResult>, std::unique_ptr<SearchDirectories::RxAnyConnectable>>
    SearchDirectories::FindFilenamesFromInodesPair(
        std::vector<std::string> root_directories,
        std::vector<Inode> inode_list,
        SearchMode mode) {
  DCHECK(mode == SearchMode::kInProcessDirect) << " other modes not implemented yet";

  auto/*observable[2]*/ [inode_results, connectable] = SearchDirectoriesForMatchingInodes(
      std::move(root_directories),
      std::move(inode_list),
      system_call_);

  std::unique_ptr<SearchDirectories::RxAnyConnectable> connectable_ptr =
    MakeRxAnyConnectableFromObservable(connectable.as_dynamic());

  return {inode_results, std::move(connectable_ptr)};
}

}  // namespace iorap::inode2filename
