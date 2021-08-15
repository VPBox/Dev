#ifndef SYSTEM_EXTRAS_PERFPROFD_MAP_UTILS_H_
#define SYSTEM_EXTRAS_PERFPROFD_MAP_UTILS_H_

#include <map>
#include <set>

#include <android-base/logging.h>

namespace android {
namespace perfprofd {

template <typename T, typename U>
decltype(static_cast<T*>(nullptr)->begin()) GetLeqIterator(T& map, U key) {
  if (map.empty()) {
    return map.end();
  }
  auto it = map.upper_bound(key);
  if (it == map.begin()) {
    return map.end();
  }
  --it;
  return it;
}

template <typename SymType, typename ValType>
class RangeMap {
 public:
  struct AggregatedSymbol {
    SymType symbol;
    std::set<ValType> offsets;
    AggregatedSymbol(const SymType& sym, const ValType& offset) : symbol(sym) {
      offsets.insert(offset);
    }
  };

 public:
  void Insert(const SymType& sym, const ValType& val) {
    auto aggr_it = GetLeqIterator(map_, val);
    if (aggr_it == map_.end()) {
      // Maybe we need to extend the first one.
      if (!map_.empty()) {
        AggregatedSymbol& first = map_.begin()->second;
        CHECK_LT(val, map_.begin()->first);
        if (first.symbol == sym) {
          ExtendLeft(map_.begin(), val);
          return;
        }
      }
      // Nope, new entry needed.
      map_.emplace(val, AggregatedSymbol(sym, val));
      return;
    }

    AggregatedSymbol& maybe_match = aggr_it->second;

    if (maybe_match.symbol == sym) {
      // Same symbol, just insert. This is true for overlap as well as extension.
      maybe_match.offsets.insert(val);
      return;
    }

    // Is there overlap?
    if (*maybe_match.offsets.rbegin() < val) {
      // No. See if it can be merged with the next one.
      ++aggr_it;
      if (aggr_it != map_.end() && aggr_it->second.symbol == sym) {
        ExtendLeft(aggr_it, val);
        return;
      }

      // Just add a new symbol entry.
      map_.emplace(val, AggregatedSymbol(sym, val));
      return;
    }

    // OK, we have an overlapping non-symbol-equal AggregatedSymbol. Need to break
    // things up.
    AggregatedSymbol left(maybe_match.symbol, *maybe_match.offsets.begin());
    auto offset_it = maybe_match.offsets.begin();
    for (; *offset_it < val; ++offset_it) {
      left.offsets.insert(*offset_it);
    }

    if (*offset_it == val) {
      // This should not happen.
      LOG(ERROR) << "Unexpected overlap!";
      return;
    }

    AggregatedSymbol right(maybe_match.symbol, *offset_it);
    for (; offset_it != maybe_match.offsets.end(); ++offset_it) {
      right.offsets.insert(*offset_it);
    }

    map_.erase(aggr_it);
    map_.emplace(*left.offsets.begin(), std::move(left));
    map_.emplace(val, AggregatedSymbol(sym, val));
    map_.emplace(*right.offsets.begin(), std::move(right));
  }

  using RangeMapType = std::map<ValType, AggregatedSymbol>;

  typename RangeMapType::const_iterator begin() const {
    return map_.begin();
  }
  typename RangeMapType::const_iterator end() const {
    return map_.end();
  }

  bool empty() const {
    return map_.empty();
  }

 private:
  void ExtendLeft(typename RangeMapType::iterator it, const ValType& val) {
    CHECK(val < *it->second.offsets.begin());
    AggregatedSymbol copy = std::move(it->second);
    map_.erase(it);
    copy.offsets.insert(val);
    map_.emplace(val, std::move(copy));
  }

  RangeMapType map_;
};

}  // namespace perfprofd
}  // namespace android

#endif  // SYSTEM_EXTRAS_PERFPROFD_MAP_UTILS_H_
