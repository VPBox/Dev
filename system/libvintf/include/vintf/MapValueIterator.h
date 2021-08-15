/*
 * Copyright (C) 2017 The Android Open Source Project
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


#ifndef ANDROID_VINTF_MAP_VALUE_ITERATOR_H
#define ANDROID_VINTF_MAP_VALUE_ITERATOR_H

#include <iterator>
#include <map>

namespace android {
namespace vintf {

template<typename Map>
struct MapIterTypes {
    using K = typename Map::key_type;
    using V = typename Map::mapped_type;

    // Iterator over all values of a Map
    template<bool is_const>
    struct IteratorImpl : public std::iterator <
            std::bidirectional_iterator_tag, /* Category */
            V,
            ptrdiff_t, /* Distance */
            typename std::conditional<is_const, const V *, V *>::type /* Pointer */,
            typename std::conditional<is_const, const V &, V &>::type /* Reference */
        >
    {
        using traits = std::iterator_traits<IteratorImpl>;
        using ptr_type = typename traits::pointer;
        using ref_type = typename traits::reference;
        using diff_type = typename traits::difference_type;

        using map_iter = typename std::conditional<is_const,
                typename Map::const_iterator, typename Map::iterator>::type;

        IteratorImpl(map_iter i) : mIter(i) {}

        inline IteratorImpl &operator++()    {
            mIter++;
            return *this;
        }
        inline IteratorImpl  operator++(int) {
            IteratorImpl i = *this;
            mIter++;
            return i;
        }
        inline IteratorImpl &operator--()    {
            mIter--;
            return *this;
        }
        inline IteratorImpl  operator--(int) {
            IteratorImpl i = *this;
            mIter--;
            return i;
        }
        inline ref_type operator*() const  { return mIter->second; }
        inline ptr_type operator->() const { return &(mIter->second); }
        inline bool operator==(const IteratorImpl &rhs) const { return mIter == rhs.mIter; }
        inline bool operator!=(const IteratorImpl &rhs) const { return mIter != rhs.mIter; }

    private:
        map_iter mIter;
    };

    using ValueIterator = IteratorImpl<false>;
    using ConstValueIterator = IteratorImpl<true>;

    template<bool is_const>
    struct IterableImpl {
        using map_ref = typename std::conditional<is_const, const Map &, Map &>::type;
        IterableImpl(map_ref map) : mMap(map) {}

        IteratorImpl<is_const> begin() const {
            return IteratorImpl<is_const>(mMap.begin());
        }

        IteratorImpl<is_const> end() const {
            return IteratorImpl<is_const>(mMap.end());
        }

        bool empty() const { return begin() == end(); }

       private:
        map_ref mMap;
    };

    template <bool is_const>
    struct RangeImpl {
        using iter_type = typename std::conditional<is_const, typename Map::const_iterator,
                                                    typename Map::iterator>::type;
        using range_type = std::pair<iter_type, iter_type>;
        RangeImpl(range_type r) : mRange(r) {}
        IteratorImpl<is_const> begin() const { return mRange.first; }
        IteratorImpl<is_const> end() const { return mRange.second; }
        bool empty() const { return begin() == end(); }

       private:
        range_type mRange;
    };

    using ValueIterable = IterableImpl<false>;
    using ConstValueIterable = IterableImpl<true>;
};

template<typename K, typename V>
using ConstMapValueIterable = typename MapIterTypes<std::map<K, V>>::ConstValueIterable;
template<typename K, typename V>
using ConstMultiMapValueIterable = typename MapIterTypes<std::multimap<K, V>>::ConstValueIterable;
template <typename K, typename V>
using MapValueIterable = typename MapIterTypes<std::map<K, V>>::ValueIterable;
template <typename K, typename V>
using MultiMapValueIterable = typename MapIterTypes<std::multimap<K, V>>::ValueIterable;

template<typename K, typename V>
ConstMapValueIterable<K, V> iterateValues(const std::map<K, V> &map) {
    return map;
}
template<typename K, typename V>
ConstMultiMapValueIterable<K, V> iterateValues(const std::multimap<K, V> &map) {
    return map;
}
template <typename K, typename V>
MapValueIterable<K, V> iterateValues(std::map<K, V>& map) {
    return map;
}
template <typename K, typename V>
MultiMapValueIterable<K, V> iterateValues(std::multimap<K, V>& map) {
    return map;
}

template <typename K, typename V>
typename MapIterTypes<std::multimap<K, V>>::template RangeImpl<true> iterateValues(
    const std::multimap<K, V>& map, const K& key) {
    return map.equal_range(key);
}

} // namespace vintf
} // namespace android

#endif // ANDROID_VINTF_MAP_VALUE_ITERATOR_H
