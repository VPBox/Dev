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

#include <iterator>

#include "perf_data.pb.h"

namespace android {
namespace perfprofd {
namespace quipper {

template<typename Iterator, typename Predicate>
class FilteredIterator {
 public:
  using value_type =      typename std::iterator_traits<Iterator>::value_type;
  using difference_type = typename std::iterator_traits<Iterator>::difference_type;
  using reference =       typename std::iterator_traits<Iterator>::reference;
  using pointer =         typename std::iterator_traits<Iterator>::pointer;

  FilteredIterator(const Iterator& begin, const Iterator& end, const Predicate& pred)
      : iter_(begin), end_(end), pred_(pred) {
    filter();
  }

  reference operator*() const {
    return *iter_;
  }
  pointer operator->() const {
    return std::addressof(*iter_);
  }

  FilteredIterator& operator++() {
    ++iter_;
    filter();
    return *this;
  }

  FilteredIterator end() {
    return FilteredIterator(end_, end_, pred_);
  }

  bool operator==(const FilteredIterator& rhs) const {
    return iter_ == rhs.iter_;
  }
  bool operator!=(const FilteredIterator& rhs) const {
    return !(operator==(rhs));
  }

private:
  void filter() {
    while (iter_ != end_ && !pred_(*iter_)) {
      ++iter_;
    }
  }

  Iterator iter_;
  Iterator end_;
  Predicate pred_;
};

template <typename Predicate>
using EventFilteredIterator = FilteredIterator<
    decltype(static_cast<::quipper::PerfDataProto*>(nullptr)->events().begin()),
    Predicate>;

struct CommEventPredicate {
  bool operator()(const ::quipper::PerfDataProto_PerfEvent& evt) {
    return evt.has_comm_event();
  }
};
struct CommEventIterator : public EventFilteredIterator<CommEventPredicate> {
  explicit CommEventIterator(const ::quipper::PerfDataProto& proto)
      : EventFilteredIterator<CommEventPredicate>(proto.events().begin(),
                                                  proto.events().end(),
                                                  CommEventPredicate()) {
  }
};

struct MmapEventPredicate {
  bool operator()(const ::quipper::PerfDataProto_PerfEvent& evt) {
    return evt.has_mmap_event();
  }
};
struct MmapEventIterator : public EventFilteredIterator<MmapEventPredicate> {
  explicit MmapEventIterator(const ::quipper::PerfDataProto& proto)
      : EventFilteredIterator<MmapEventPredicate>(proto.events().begin(),
                                                  proto.events().end(),
                                                  MmapEventPredicate()) {
  }
};

struct SampleEventPredicate {
  bool operator()(const ::quipper::PerfDataProto_PerfEvent& evt) {
    return evt.has_sample_event();
  }
};
struct SampleEventIterator : public EventFilteredIterator<SampleEventPredicate> {
  explicit SampleEventIterator(const ::quipper::PerfDataProto& proto)
      : EventFilteredIterator<SampleEventPredicate>(proto.events().begin(),
                                                    proto.events().end(),
                                                    SampleEventPredicate()) {
  }
};

struct ForkEventPredicate {
  bool operator()(const ::quipper::PerfDataProto_PerfEvent& evt) {
    return evt.has_fork_event();
  }
};
struct ForkEventIterator : public EventFilteredIterator<ForkEventPredicate> {
  explicit ForkEventIterator(const ::quipper::PerfDataProto& proto)
      : EventFilteredIterator<ForkEventPredicate>(proto.events().begin(),
                                                  proto.events().end(),
                                                  ForkEventPredicate()) {
  }
};

struct ExitEventPredicate {
  bool operator()(const ::quipper::PerfDataProto_PerfEvent& evt) {
    return evt.has_exit_event();
  }
};
struct ExitEventIterator : public EventFilteredIterator<ExitEventPredicate> {
  explicit ExitEventIterator(const ::quipper::PerfDataProto& proto)
      : EventFilteredIterator<ExitEventPredicate>(proto.events().begin(),
                                                  proto.events().end(),
                                                  ExitEventPredicate()) {
  }
};

}  // namespace quipper
}  // namespace perfprofd
}  // namespace android
