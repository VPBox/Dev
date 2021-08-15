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

#include "inode2filename/search_directories.h"
#include "inode2filename/system_call.h"

#include <android-base/logging.h>
#include <android-base/strings.h>

#include <fruit/fruit.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <optional>

#include <sys/sysmacros.h>


// Set this to 1 when debugging by hand to get more output.
// Otherwise the spam might be too much when most tests are failing.
#define LOG_WITH_VERBOSE 0
// Set this to 1 when debugging by hand to have the logging output go to stderr.
// TODO: I think the automated test bots have problems capturing non-logcat output.
#define LOG_TO_STDERR 1

// TODO: Might be nice to have these controlled by command line.

using namespace std::literals::string_literals;  // NOLINT
using namespace std::literals::string_view_literals;  // NOLINT
using namespace iorap::inode2filename;  // NOLINT
using namespace testing;  // NOLINT

static void ConfigureLogging() {
  if (LOG_TO_STDERR) {
    ::android::base::SetLogger(::android::base::StderrLogger);
  }
  if (LOG_WITH_VERBOSE) {
    ::android::base::SetMinimumLogSeverity(::android::base::VERBOSE);
  } else {
    ::android::base::SetMinimumLogSeverity(::android::base::DEBUG);
  }
}

// Iterate substrings in 'what' that are separated by 'separator'.
// Should be similar to the python 'str.split' behavior.
//
// Empty separators will have 0 iterations.
//
// NOTE: this could end up returning empty strings, e.g. '/'.split('/') -> ('', '')
// Think of it more like splitting on "$/^" except the $ and ^ become empty strings in the end.
//
// Zero-copy guarantee (and no dynamic allocation).
struct StringSplit {
  struct SplitIterable;

  // Return a 0-length substring whose address range is one past the end of 'what'.
  // Logically equivalent to a "", but its real address will be within 'what'.
  //
  // Repeatedly applying this function on itself will return the same value.
  //
  // Do not use operator[] on the returned substring, as that would cause undefined
  // behavior.
  //
  // To safely access the pointer, use #data(). The pointer must not be dereferenced,
  // which would cause undefined behavior.
  static constexpr std::string_view EmptySubstringAtEnd(std::string_view what) {
    return what.substr(/*pos*/what.size(), /*count*/0);
  }

  // Create an Iterable that will iterate over substrings in 'what' separated by 'separator'.
  //
  // Each such 'value' emitted is guaranteed to be:
  //  - a substring of 'what'
  //  - not have any 'separator' substrings
  //  - the address range of 'value' is within the address range of 'what' (or one-past-the-end)
  //
  // For example:
  //
  //   for (std::string_view substr : StringSplit("hello/world"sv, "/"sv)) {
  //     ... // loop 0: substr == "hello"
  //     ... // loop 1: substr == "world"
  //   }
  static constexpr SplitIterable Iterable(std::string_view what,
                                          std::string_view separator) {
    return SplitIterable{what, separator};
  }

  // Implement LegacyForwardIterator concept.
  struct SplitIterator {
    using value_type = std::string_view;
    using reference = value_type&;
    using pointer = value_type*;
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;  // required by concept, but its meaningless.

    constexpr bool operator==(const SplitIterator& other) const {
      if (state != other.state) {
        return false;
      }
      switch (state) {
        case kNormal:
        case kNearEnd:
          return current_split.data() == other.current_split.data();
        case kAtEnd:
          return true;
      }
    }

    constexpr bool operator!=(const SplitIterator& other) const {
      return !(*this == other);
    }

    constexpr std::string_view& operator*() {
        DCHECK(state != kAtEnd) << "Undefined behavior to dereference end() iterators";
        return current_split;
    }

    constexpr std::string_view* operator->() {
        DCHECK(state != kAtEnd) << "Undefined behavior to dereference end() iterators";
        return &current_split;
    }

    /*
    constexpr const std::string_view& operator*() const {
        return current_split;
    }

    constexpr const std::string_view* operator->() const {
        return &current_split;
    }
    */

    constexpr SplitIterator& operator++() {
      UpdateValues();
      return *this;
    }

    constexpr SplitIterator operator++(int) {
      SplitIterator copy{*this};
      ++(*this);
      return copy;
    }

   private:
    // Avoid defining constructors etc. We get the default constructors and operator= then.

    friend struct SplitIterable;  // Use below Make functions.

    constexpr static SplitIterator MakeBegin(std::string_view whole, std::string_view separator) {
      SplitIterator it;
      it.state = kNormal;

      if (separator == "") {
        it.rest_of_string = StringSplit::EmptySubstringAtEnd(whole);
        // point to one past-the end (which is legal), also equivalent to ""
        // the difference being that the address range is guaranteed to be within 'whole'
        // actually any 0-length subrange would be appropriate here, but just go with the 'end'
        // because dereferencing it would be obviously bad.
        it.state = kAtEnd;
        // Empty separator -> empty # of visits. This seems the most composable.
        // Note: Need to handle this case especially since find_first_of("") would return the
        // entire string.
      } else {
        it.rest_of_string = whole;
        it.separator = separator;
        it.UpdateValues();
      }
      return it;
    }

    constexpr static SplitIterator MakeEnd() {
      SplitIterator it;
      it.state = kAtEnd;
      return it;
    }

    constexpr void UpdateValues() {
      switch (state) {
        case kNormal:
          break;
        case kNearEnd:
          // Address of emitted value is always within subrange of 'whole'.
          current_split = StringSplit::EmptySubstringAtEnd(rest_of_string);
          state = kAtEnd;
          return;
        case kAtEnd:
          // Incrementing the 'end()' operator is undefined behavior.
          DCHECK(false) << "Undefined behavior: Cannot increment end() iterator.";
          return;
      }

      DCHECK(state == kNormal);

      size_t pos = rest_of_string.find_first_of(separator);
      if (std::string_view::npos == pos) {
        // Always visit at least once for non-empty separators, even if the string is empty.

        current_split = rest_of_string;
        // Address of emitted value is always within subrange of 'whole'.
        rest_of_string = rest_of_string.substr(/*pos*/0, /*count*/0);  // = ""
        state = kNearEnd;
      } else {
        // includes the starting position of the needle
        // e.g. "+x-".find_first_of('x') -> 1

        // current_split = rest_of_string[0..pos)
        current_split = rest_of_string.substr(/*pos*/0, pos);

        // strip '${left}${separator}' from the left hand side.
        // continue iterating.
        rest_of_string = rest_of_string.substr(pos + separator.size());
      }
    }

 public:

    void PrintToStream(std::ostream& os) const {
      os << "SplitIterator{";
      os << "current_split:\"" << current_split << "\",";
      os << "rest_of_string:\"" << rest_of_string << "\",";
      os << "separator:\"" << separator << "\",";
      os << "state:";
      switch (state) {
        case kNormal:
          os << "kNormal";
          break;
        case kNearEnd:
          os << "kNearEnd";
          break;
        case kAtEnd:
          os << "kAtEnd";
          break;
      }
      os << "}";
    }
 private:
    // Not intended to be used directly.
    // Public visibility to avoid making extra constructors.
    std::string_view current_split;
    std::string_view rest_of_string;
    std::string_view separator;

    enum State {
      kNormal,
      kNearEnd,
      kAtEnd
    };
    State state{kNormal};
    // This cannot have a field initializer due to a clang bug,
    // https://bugs.llvm.org/show_bug.cgi?id=36684
    // So define an explicit constructor below.

    // This needs to go last:
    // undefined constructor 'SplitIterator' cannot be used in a constant expression
    // constexpr SplitIterator() : state{kNormal} {}
   // constexpr SplitIterator() {}
  };

  friend struct SplitIterable;

  struct SplitIterable {
    std::string_view whole;
    std::string_view separator;

    constexpr SplitIterator begin() {
      return SplitIterator::MakeBegin(whole, separator);
    }

    constexpr SplitIterator end() {
      return SplitIterator::MakeEnd();
    }
  };
};

std::ostream& operator<<(std::ostream& os, const StringSplit::SplitIterator& it) {
  it.PrintToStream(os);
  return os;
}

static constexpr  const StringSplit::SplitIterator kBlankSplit;

// Visit substrings in 'what' that are separated by 'separator'.
// Should be similar to the python 'str.split' behavior.
//
// Empty separators will have 0 visits.
//
// 'f' is called back for each visit of a substring, this means there's 0 allocations here.
//
// NOTE: this could end up returning empty strings, e.g. '/'.split('/') -> ('', '')
// Think of it more like splitting on "$/^" except the $ and ^ become empty strings in the end.
//
// (Dynamic allocation free)
template <typename Fn>
static constexpr void VisitSplitStringView(std::string_view what,
                                           std::string_view separator,
                                           Fn f) {
  // Empty separator -> empty # of visits. This seems the most composable.
  if (separator == "") {
    // Note: Need to handle this case especially since find_first_of("") would return the
    // entire string.
    return;
  }

  size_t sep_length = separator.size();

  do {
    size_t pos = what.find_first_of(separator);
    if (std::string_view::npos == pos) {
      // Always visit at least once for non-empty separators, even if the string is empty.
      f(what);
      break;
    } else {
      // includes the starting position of the needle
      // e.g. "+x-".find_first_of('x') -> 1

      // left = what[0..pos)
      std::string_view left_split = what.substr(/*pos*/0, pos);
      f(left_split);

      // strip '${left}${separator}' from the left hand side.
      // continue iterating.
      what = what.substr(pos + sep_length);
    }
  }
  while (true);
}

std::vector<std::string> VisitSplitStringViewVec(std::string_view what,
                                                 std::string_view separator) {
  std::vector<std::string> vec;
  VisitSplitStringView(what, separator, [&vec](auto&& part) {
                         vec.push_back(std::string{part});
                       });
  return vec;
}

std::vector<std::string> IterableSplitStringViewVec(std::string_view what,
                                                    std::string_view separator) {
  auto iterable = StringSplit::Iterable(what, separator);
  std::vector<std::string> vec{iterable.begin(), iterable.end()};

  return vec;
}

TEST(SplitStringView, Tests) {
  EXPECT_THAT(VisitSplitStringViewVec("", ""), IsEmpty());
  EXPECT_THAT(VisitSplitStringViewVec("abcdef", ""), IsEmpty());
  EXPECT_THAT(VisitSplitStringViewVec("", "/"), ElementsAre(""s));
  EXPECT_THAT(VisitSplitStringViewVec("/", "/"), ElementsAre(""s, ""s));
  EXPECT_THAT(VisitSplitStringViewVec("//", "/"), ElementsAre(""s, ""s, ""s));
  EXPECT_THAT(VisitSplitStringViewVec("/hello", "/"), ElementsAre(""s, "hello"s));
  EXPECT_THAT(VisitSplitStringViewVec("/hello/world", "/"), ElementsAre(""s, "hello"s, "world"s));
  EXPECT_THAT(VisitSplitStringViewVec("bar", "/"), ElementsAre("bar"s));
  EXPECT_THAT(VisitSplitStringViewVec("bar/baz", "/"), ElementsAre("bar"s, "baz"s));

  EXPECT_THAT(IterableSplitStringViewVec("", ""), IsEmpty());
  EXPECT_THAT(IterableSplitStringViewVec("abcdef", ""), IsEmpty());
  EXPECT_THAT(IterableSplitStringViewVec("", "/"), ElementsAre(""s));
  EXPECT_THAT(IterableSplitStringViewVec("/", "/"), ElementsAre(""s, ""s));
  EXPECT_THAT(IterableSplitStringViewVec("//", "/"), ElementsAre(""s, ""s, ""s));
  EXPECT_THAT(IterableSplitStringViewVec("/hello", "/"), ElementsAre(""s, "hello"s));
  EXPECT_THAT(IterableSplitStringViewVec("/hello/world", "/"), ElementsAre(""s, "hello"s, "world"s));
  EXPECT_THAT(IterableSplitStringViewVec("bar", "/"), ElementsAre("bar"s));
  EXPECT_THAT(IterableSplitStringViewVec("bar/baz", "/"), ElementsAre("bar"s, "baz"s));

  EXPECT_THAT(IterableSplitStringViewVec("/hello", "/"), ElementsAre(""sv, "hello"sv));
  EXPECT_THAT(IterableSplitStringViewVec("/hello///", "/"), ElementsAre(""sv, "hello"sv, ""sv, ""sv, ""sv));
 
}


// Allocation-free immutable path representation and manipulation.
//
// A PurePath is logically represented by its 'parts', which is a view of each component.
//
// Examples:
//   parts('foo/bar') -> ['foo', 'bar']
//   parts('/bar') -> ['/', 'bar']
//   parts('') -> []
//   parts('.') -> []
//   parts('baz//') -> ['baz']
//   parts('hello/././world') -> ['hello', 'world']
//   parts('../down/../down2') -> ['..', 'down', '..', 'down2']
//
// See also #VisitParts which allows an allocation-free traversal of the parts.
//
// Memory allocation/ownership guarantees:
// * Functions marked as 'constexpr' are guaranteed never to allocate (zero-copy).
// * Functions not marked as 'constexpr' and returning a PurePath will always return an object
//   with its own internal copy of the underlying data (i.e. the memory is not borrowed).
struct PurePath {
  using part_type = std::string_view;

  struct PartIterable;

  // Create an empty PurePath.
  //
  // Empty paths are considered to have 0 parts, i.e.
  //   PurePath{}.VisitParts() -> []
  constexpr PurePath() : path_(".") {
  }

  // Create a PurePath from a string view.
  //
  // This borrows memory ownership of the string view. If you wish to make a copy,
  // use the PurePath(std::string) constructor.
  //
  // Paths are non-normalized (i.e. redundant up-references, "..", are not stripped),
  // you may wish to call 'NormalizePath' if this is important.
  constexpr PurePath(std::string_view path)  : path_(path) {
    /// : owner_(std::string(path)), path_(owner_.value()) {
    // TODO: no copy
  }

  constexpr PurePath(const char* path) : PurePath(std::string_view(path)) {}

  // Creates a PurePath from a string.
  //
  // The PurePath owns the memory of the string path.
  //
  // Only accepts movable strings, so that the cheaper borrowing (string_view)
  // constructor is used by default.
  PurePath(std::string&& path) : owner_(std::move(path)), path_(owner_.value()) {
  }

  // Return an Iterable, which upon traversing would
  // return each part as an std::string_view.
  //
  // Empty and '.' path components are not visited,
  // effectively ignoring redundant // and intermediate '.' components.
  //
  // To also ignore redundant up-references, see #NormalizePath.
  //
  // Example:
  //   for (std::string_view part : PurePath("hello//world/./").IterateParts()) {
  //     // loop 0, part == "hello"sv
  //     // loop 1, part == "world"sv
  //   }
  constexpr PartIterable IterateParts() const {
    return PartIterable::FromPath(*this);
  }

  // f is a function<void(std::string_view part)>
  //
  // Invoke 'f' repeatedly on each logical part of this path.
  //
  // Empty and '.' path components are not visited,
  // effectively ignoring redundant // and intermediate '.' components.
  //
  // To also ignore redundant up-references, see #NormalizePath.
  template <typename Fn>
  constexpr void VisitParts(Fn f) const {
    // Note: Near the top to avoid -Wundefined-inline warnings.
    if (IsAbsolute()) {
      f(kRoot);  // When we split, we no longer visit the '/' tokens. Handle root explicitly.
    }
    VisitSplitStringView(path_,
                         kRoot,
                         [&f](auto&& substr) {
                           // Ignore duplicate /// and also .
                           //
                           // e.g.
                           //   '//foo' -> ['/', 'foo']
                           //   './foo' -> ['foo']
                           //
                           // This is consistent with PurePath.parts implementation.
                           //
                           // Note that redundant .. are not removed, e.g.
                           //   '../foo/..' is not rewritten to ['..']
                           //
                           // Use 'NormalizePath' to do this explicitly.
                           if (!substr.empty() && substr != ".") {
                             f(substr);
                           }
                         });
  }


  // A path is considered equal to another path if all of the parts are identical.
  /*constexpr*/ bool operator==(const PurePath& other) const {
    /*if (path_ == other.path_) {
      return true;
    } else*/ {
      auto this_range = IterateParts();
      auto other_range = other.IterateParts();

      return std::equal(this_range.begin(),
                        this_range.end(),
                        other_range.begin(),
                        other_range.end());
    }
  }

  // Returns the name component (if any).
  //
  // Logically equivalent to returning the last part unless:
  //   - the last part is the root '/'
  //   - there are no parts
  //
  // If the above conditions do not hold, return the empty string.
  constexpr std::string_view Name() const {
    std::string_view component = StringSplit::EmptySubstringAtEnd(path_);

    size_t count = 0;
    for (auto&& part : IterateParts()) {
      if (count++ == 0 && part == kRoot) {
        continue;  // '/' does not count as a name.
      } else {
        DCHECK_NE(part, kRoot);
      }

      component = part;
    }

    return component;
  }

  // Find the parent of this path.
  //
  // This is usually the path with the last part stripped off, with some special cases:
  // - The parent of '/' is always '/' (recursive).
  // - The parent of '' is always '..'.
  // - The parent of '..[/..]*' is an additional '/..' appended.
  //
  // The parent is always distinct (i.e. not equal to this) except for '/', whose parent
  // is itself.
  /*constexpr*/ PurePath Parent() const {
    size_t parts_count = 0;
    size_t upreference_count = 0;
    // TODO: this should be constexpr, but it complains about PurePath not being a literal type.

    for (auto&& part : IterateParts()) {
      ++parts_count;

      if (part == "..") {
        ++upreference_count;
      }
    }

    if (upreference_count == parts_count) {  // Could also have 0 parts.
      // "../../../" etc. No other parts are there.
      // We need to add another '..'

      // Explicitly handle a few iterations to remain constexpr.
      switch (upreference_count) {
        case 0:
          return {".."};
        case 1:
          return {"../.."};
        case 2:
          return {"../../.."};
        case 3:
          return {"../../../.."};
        default:
          break;
      }

      // As a special case, this part of the function is not constexpr.
      std::string built_parent_string = "..";
      for (size_t i = 0; i < upreference_count; ++i) {
        built_parent_string += kRoot;
        built_parent_string += "..";
      }

      return PurePath{std::move(built_parent_string)};

    } else if (parts_count == 1) {
      if (IsAbsolute()) {
        // "/" + ".." is still "/"
        return {kRoot};
      } else {
        // <NOT-ROOT-OR-UP-REFERENCE> + ".." is just "."
        return {};
      }
    } else {
      DCHECK_GE(parts_count, 2u);

      // Find the last iterator before we hit the end.
      std::optional<std::string_view> last;
      std::optional<std::string_view> prev_last;
      for (auto&& part : IterateParts()) {
        prev_last = last;
        last = part;
      }

      DCHECK(last.has_value());
      DCHECK(prev_last.has_value());

      std::string_view& prev_last_view = *prev_last;
      // prev_last_view must be within address of subrange_.
      DCHECK_GE(prev_last_view.data(), path_.data());
      DCHECK_LE(prev_last_view.data() + prev_last_view.size(), path_.data() + path_.size());

      // take advantage of the address subrange property by calculating a new substring
      // for the parent.
      size_t length = prev_last_view.data() + prev_last_view.size() - path_.data();
      std::string_view parent = std::string_view{path_.data(), length} ;

      if ((false)) {
        LOG(DEBUG) << "PurePath::Parent of \"" << path_ << "\" returns \"" << parent << "\"";
      }
      return { parent };
    }
  }

  // A path is considered non-equal to another path if one or more of the parts differ.
  constexpr bool operator!=(const PurePath& other) const {
    return !(*this == other);
  }

  // Return the string view, i.e. to pass to other classes that need a string-like type.
  //
  // This passes in the original string as was passed into the constructor.
  // The exact char-by-char representation may be different than concatenating all the parts
  // together.
  //
  // See also #NormalizePath if you want to get a 1:1 mapping between a PurePath
  // and a string.
  constexpr std::string_view AsStringView() const {
    // This is slightly inconsistent with PurePath#bytes because it actually collapses the string
    // to the equivalent of concatenating the parts together. But we prefer not to do that,
    // since it just causes more work and more allocations unnecessarily.
    //
    // This is generally not-noticeable when operating with the path at the logical layer.
    return path_;
  }

  constexpr bool IsAbsolute() const {
    return !path_.empty() && path_[0] == '/';  // left-whitespace is considered significant.
  }

  // Join one or more paths together.
  //
  // Logically equivalent to calling JoinPath(other) repeatedly.
  template <typename It>
  PurePath JoinPath(It begin, It end) const {
    std::vector<std::string_view> parts_stack = PartsList();

    while (begin != end) {
      const PurePath& path = *begin;

      if (path.IsAbsolute()) {
        parts_stack = path.PartsList();
      } else {
        path.VisitParts([&parts_stack](auto&& part) {
          parts_stack.push_back(part);
        });
      }

      ++begin;
    }

    return {JoinPartsList(parts_stack)};
  }

  // Join two paths together:
  //
  // If 'other' is an absolute path, it is returned.
  //
  // Otherwise, return the concatenation of the parts (this and other) as a new path.
  // (The returned path is always owned by the caller -- this is triggering an allocation every
  // time).
  PurePath JoinPath(const PurePath& other) const {
    if (other.IsAbsolute()) {
      return other.OwningCopy();
    } else {
      std::vector<std::string_view> parts_stack = PartsList();
      other.VisitParts([&parts_stack](auto&& part) {
        parts_stack.push_back(part);
      });
      return {JoinPartsList(parts_stack)};
    }
  }

  constexpr PurePath(const PurePath& other) {
    if (this == &other) {
      return;
    }
    if (other.owner_) {  // stay constexpr for non-owning paths.
      owner_ = other.owner_;
      path_ = *owner_;   // path_ always points to owner if possible.
    } else {
      path_ = other.path_;
    }
  }

  constexpr PurePath(PurePath&& other) {
    if (this == &other) {
      return;
    }
    if (other.owner_) {  // stay constexpr for non-owning paths.
      owner_ = std::move(other.owner_);
      path_ = *owner_;   // path_ always points to owner if possible.
    } else {
      path_ = std::move(other.path_);
    }
  }

  // "/.." -> "/"
  // "../foo/.." -> ".."
  // etc.
  //
  // PurePath returned always owns its own memory (this always triggers an allocation).
  PurePath NormalizePath() const {
    if (IsNormalized()) {
      return OwningCopy();  // Don't call this function if you want to avoid copies!
    } else {
      // Invariant: [/]? <UP-REFERENCE>* <NOT-AN-UP-REFERENCE>*
      std::vector<std::string_view> parts_stack;
      size_t not_an_up_reference = 0;

      // Special handling of absolute paths:
      //   '/' '..'* -> '/'
      //
      // Otherwise, remove the last part when encountering redundant up-references:
      //   e.g. '../foo/bar/baz/..' -> '../foo/bar'
      VisitParts([&](auto&& part) {
                   if (part == "..") {
                     // <UP-REFERENCE>
                     if (not_an_up_reference > 0) {
                       // Remove redundant up-references.
                       DCHECK(!parts_stack.empty());

                       // Could trigger de-normalization, remove redundant part from stack.

                       if (parts_stack.back() != kRoot) {  // '/' '..'* -> '/'
                         parts_stack.pop_back();
                         --not_an_up_reference;            // '../foo/..' -> '..'
                       }
                     } else {
                       // Did not trigger a denormalization.
                       parts_stack.push_back(part);
                     }
                   } else {
                     // <NOT-AN-UP-REFERENCE> or '/' (note: / is only visited the first time).
                     parts_stack.push_back(part);
                     ++not_an_up_reference;
                   }
                 });

      // join again with empty delimiter.
      std::string concat = JoinPartsList(std::move(parts_stack));

      return PurePath(std::move(concat));
    }
  }

  // Returns true if 'NormalizePath' would return a Path with a different parts representation.
  //
  // (This is not as strict as normalizing the underlying string, i.e. redundant '.' and "//"
  // in AsStringView() could still be seen).
  //
  // A path is considered non-normalized unless all up-references are at the start.
  //
  //   NormalizedString := <UP-REFERENCE>* <NOT-AN-UP-REFERENCE>*
  //
  // where each token is a 'part' returned by VisitParts.
  //
  // Returning false here means that 'NormalizePath' will also trigger an extra allocation.
  constexpr bool IsNormalized() const {
    size_t not_an_up_reference = 0;
    bool is_normalized = true;

    // Note that this also handles '/' [..]* because '/' is treated identically to non-up-refs.
    VisitParts([&](auto&& part) {
                // Remove redundant up-references.
                if (part != "..") {
                  ++not_an_up_reference;
                } else {  // part == ".."
                  if (not_an_up_reference > 0) {   // <not-an-up-reference> <up-reference>
                    is_normalized = false;
                  }
                }
               });

    return is_normalized;
  }

  // Implement LegacyForwardIterator concept.
  struct PartIterator {
    using value_type = std::string_view;
    using reference = value_type&;
    using pointer = value_type*;
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;  // required by concept, but its meaningless.

   private:
    enum State {
      kUninitialized,
      kAtRoot,
      kInitialized,
      kAtEnd
    };

    using SplitIterable = StringSplit::SplitIterable;
    using SplitIterator = StringSplit::SplitIterator;

    State state{kUninitialized};
    value_type cur_value;
    SplitIterator cur;
    SplitIterator end;

    friend std::ostream& operator<<(std::ostream& os, const PartIterator& it);

    // Print out extra debugging information when looping through the iterator.
    static constexpr bool kLogDebug = false;

   public:
    void PrintToStream(std::ostream& os) const {
      os << "PartIterator{";
      os << "state:";
      switch (state) {
        case kUninitialized:
          os << "kUninitialized";
          break;
        case kAtRoot:
          os << "kAtRoot";
          break;
        case kInitialized:
          os << "kInitialized";
          break;
        case kAtEnd:
          os << "kAtEnd";
          break;
      }
      os << ",";
      os << "cur_value:\"" << cur_value << "\",";
      os << "cur:" << cur << ",";
      os << "end:" << end << ",";
      os << "}";
    }

    /*constexpr*/ bool operator==(const PartIterator& other) const {
      DCHECK(state != kUninitialized) << "State must be initialized";
      DCHECK(other.state != kUninitialized) << "Other state must be initialized";

      if (kLogDebug) {
        LOG(DEBUG) << "PartIterator ==";
      }

      if (state != other.state) {
        if (kLogDebug) {
          LOG(DEBUG) << "State: " << static_cast<int>(state);
          LOG(DEBUG) << "Other State: " << static_cast<int>(other.state);

          LOG(DEBUG) << "== states differ (&self=" << this << ",&other=" << &other << ")";
          LOG(DEBUG) << "Self=" << *this;
          LOG(DEBUG) << "Other=" << other;
        }
        return false;
      }

      switch (state) {
        case kAtRoot:
          DCHECK(cur != end);
          return cur == other.cur;
        case kInitialized:
          DCHECK(cur != end);
          return cur == other.cur;
        case kAtEnd:
          DCHECK(cur == end);
          DCHECK(cur == other.cur);
          return true;
        default:
          DCHECK(false);  // -Werror -Wswitch
          return true;
      }
    }

    constexpr bool operator!=(const PartIterator& other) const {
      return !(*this == other);
    }

    constexpr reference operator*() {
        DCHECK(state != kAtEnd) << "Undefined behavior to dereference end() iterators";
        return cur_value;  // Can't use *cur because we could yield a '/'.
    }

    constexpr pointer operator->() {
        DCHECK(state != kAtEnd) << "Undefined behavior to dereference end() iterators";
        return &cur_value;  // Can't use &*cur because we could yield a '/'.
    }

    /*
    constexpr const reference operator*() const {
        return *cur;
    }

    constexpr const pointer operator->() const {
        return &*cur;
    }*/

    constexpr PartIterator& operator++() {
      DCHECK(state != kAtEnd) << "Undefined behavior to increment end() iterators";
      UpdateValues();
      return *this;
    }

    constexpr PartIterator operator++(int) {
      PartIterator copy{*this};
      ++(*this);
      return copy;
    }

    constexpr static PartIterator MakeBegin(SplitIterable& split_iterable,
                                            std::string_view whole_path) {
      SplitIterator begin = split_iterable.begin();
      SplitIterator end = split_iterable.end();

      PartIterator it;
      it.end = end;

      const bool is_absolute = !whole_path.empty() && whole_path[0] == '/';

      if (begin == end) {
        it.cur = end;
        it.state = kAtEnd;
        // I'm not sure this path is actually possible due to the nature of how StringSplit
        // works, but it's better to cover this case just to be safe.
        DCHECK(false) << "unreachable code, splitting by '/' always returns at least 1 split";
      } else {
        it.cur = begin;

        if (is_absolute) {
          // When we split, we no longer visit the '/' tokens. Handle root explicitly.
          //
          // All emitted values must be within the address range of the whole path.
          it.cur_value = whole_path.substr(0, /*count*/1);  // '/'
          DCHECK_EQ(it.cur_value, "/"sv);
          it.state = kAtRoot;
        } else {
          it.state = kUninitialized;
          it.UpdateValues();
        }
      }

      return it;
    }

    constexpr static PartIterator MakeEnd(SplitIterable& split_iterable) {
      SplitIterator end = split_iterable.end();

      PartIterator it;
      it.cur = end;
      it.end = end;
      it.state = kAtEnd;

      return it;
    }

   private:
    void UpdateValues() {
      State previous_state = state;

      if (kLogDebug) {
        LOG(DEBUG) << "operator ++ // UpdateValues (&this=" << this << ")";
      }

      if (state == kAtEnd) {
        return;
      }

      if (state == kInitialized) {
        DCHECK(IsValidCurrent());
      }

      // '/' has no corresponding split, so it's handled as a special case.
      // Furthermore, any splits that are empty or "." are skipped since they aren't
      // considered to be a valid path component.
      //
      // The below code handles these special cases.

      if (state == kAtRoot) {
        state = kUninitialized;
      }

      if (state == kUninitialized) {
        // If we are already at a valid value stop.
        if (cur != end && IsValidCurrent()) {
          state = kInitialized;
          cur_value = *cur;
          return;
        }

        // Otherwise we are either at the end, or
        // the current value is invalid (e.g. empty or '.').
        state = kInitialized;
      }

      DCHECK(state == kInitialized) << static_cast<int>(state);
      if (previous_state == kInitialized) {
        // If we fell-through from kAtRoot or kUninitialized
        // then there's no guarantee that the current value is valid.
        DCHECK(IsValidCurrent());
      }

      auto old_cur_value = *cur;

      // Already at the end. Switch to end state.
      if (cur == end) {
        state = kAtEnd;
        LOG(DEBUG) << "Updated state is: kAtEnd (1)";
        return;
      }

      // Skip ahead.
      // We may or may not be at a valid value now.
      ++cur;

      // If we aren't at a valid value yet, then keep going forward
      // until we hit a valid value (or we exhaust the iterator).
      while (cur != end && !IsValidCurrent()) {
        ++cur;
      }

      if (cur == end) {
        state = kAtEnd;
      } else {
        // We reached a valid value before exhausting the iterator.

        // Stay in the 'Initialized' state.
        DCHECK(IsValidCurrent()) << *cur;
        cur_value = *cur;

        // After we go forward, the old and current value cannot match.
        DCHECK_NE(&cur_value[0], &old_cur_value[0]);
      }

      if (kLogDebug) {
        LOG(DEBUG) << "Updated state is: " << state;
      }
    }

    constexpr bool IsValidCurrent() {
      if (cur->empty()) {
        return false;
      } else if (*cur == ".") {
        return false;
      }

      return true;
    }
  };

  friend struct PartIterable;

  struct PartIterable {
    constexpr static PartIterable FromPath(const PurePath& path) {
      return PartIterable{
          path.AsStringView(),
          StringSplit::Iterable(path.AsStringView(), PurePath::kRoot),
      };
    }

    constexpr PartIterator begin() {
      return PartIterator::MakeBegin(split_iterable, whole_path);
    }

    constexpr PartIterator end() {
      return PartIterator::MakeEnd(split_iterable);
    }

    std::string_view whole_path;
    StringSplit::SplitIterable split_iterable;
  };

  // This isn't performance-efficient, but it might be needed by some functions
  // that have to allocate anyway such as JoinPaths.
  //
  // Intended only for testing.
  std::vector<std::string_view> PartsList() const {
    PartIterable iterable = IterateParts();

    std::vector<std::string_view> parts{iterable.begin(), iterable.end()};
    return parts;
  }

  // Does this PurePath own the underlying memory?
  //
  // true = borrowing memory from someone else (might not be safe to retain this object)
  // false = owns its own memory (can keep this object indefinitely long)
  //
  // Currently intended only for testing.
  constexpr bool IsBorrowed() const {
    return !owner_.has_value();
  }

 private:
  // Return a PurePath that owns its own memory.
  //
  // This way functions which 'may' allocate memory turn into functions
  // that always allocate memory, and avoid a dangling reference.
  const PurePath OwningCopy() const {
    std::string make_copy{path_};
    return PurePath{std::move(make_copy)};
  }

  constexpr size_t PartsCount() const {
    size_t count = 0;
    VisitParts([&count](auto&& /*part*/) {
      ++count;
    });
    return count;
  }

  // Basically a string join with an empty delimiter.
  template <typename Container>
  static std::string JoinPartsList(Container&& c) {
    std::string build;
    for (auto begin = c.begin(), end = c.end(); begin != end; ++begin) {
      build += *begin;

      // TODO: use forward_dependent here.
    }

    return build;
  }

  // This might be empty, in which case path_ is just a temporary borrow of path_.
  std::optional<std::string> owner_;
  std::string_view path_;  // points to owner_ if there's a value there.

  // TODO: this is a bit error-prone, so we might want to refactor into a
  // never-owning PathView and an always-owning PurePath.

  static constexpr std::string_view kRoot = "/";
};

std::ostream& operator<<(std::ostream& os, const PurePath::PartIterator& it) {
  it.PrintToStream(os);
  return os;
}

static constexpr const PurePath::PartIterator kMakeMeABlank;

std::ostream& operator<<(std::ostream& os, const PurePath& path) {
  os << path.AsStringView();
  return os;
}

TEST(PurePathTest, Ctor) {
  ConfigureLogging();

  EXPECT_EQ(PurePath{}.AsStringView(), "."sv);
  EXPECT_EQ(PurePath{""}.AsStringView(), ""sv);
  EXPECT_EQ(PurePath{""sv}.AsStringView(), ""sv);
  EXPECT_EQ(PurePath{""s}.AsStringView(), ""sv);
  EXPECT_EQ(PurePath{"/hello/world"}.AsStringView(), "/hello/world"sv);
  EXPECT_EQ(PurePath{"/hello/world"s}.AsStringView(), "/hello/world"sv);
  EXPECT_EQ(PurePath{"/hello/world"sv}.AsStringView(), "/hello/world"sv);
  EXPECT_EQ(PurePath{"hello/world"}.AsStringView(), "hello/world"sv);
  EXPECT_EQ(PurePath{"hello/world"s}.AsStringView(), "hello/world"sv);
  EXPECT_EQ(PurePath{"hello/world"sv}.AsStringView(), "hello/world"sv);

  // Ensure that std::string is only owning memory when we move a string into it.
  // Otherwise, always take the string_view constructor.
  EXPECT_FALSE(PurePath{std::string{"hello"}}.IsBorrowed());
  std::string hello{"hello"};
  EXPECT_TRUE(PurePath{hello}.IsBorrowed());
  EXPECT_FALSE(PurePath{std::move(hello)}.IsBorrowed());
}

TEST(PurePathTest, Parts) {
  ConfigureLogging();

  EXPECT_THAT(PurePath{}.PartsList(), IsEmpty());
  EXPECT_THAT(PurePath{"."}.PartsList(), IsEmpty());
  EXPECT_THAT(PurePath{"./"}.PartsList(), IsEmpty());
  EXPECT_THAT(PurePath{"./."}.PartsList(), IsEmpty());
  EXPECT_THAT(PurePath{".///"}.PartsList(), IsEmpty());
  EXPECT_THAT(PurePath{"./././."}.PartsList(), IsEmpty());
  EXPECT_THAT(PurePath{"/"s}.PartsList(), ElementsAre("/"sv));
  EXPECT_THAT(PurePath{"///"s}.PartsList(), ElementsAre("/"sv));
  EXPECT_THAT(PurePath{"/hello"s}.PartsList(), ElementsAre("/"sv, "hello"sv));
  EXPECT_THAT(PurePath{"/hello///"s}.PartsList(), ElementsAre("/"sv, "hello"sv));
  EXPECT_THAT(PurePath{"/hello/world"s}.PartsList(), ElementsAre("/"sv, "hello"sv, "world"sv));
  EXPECT_THAT(PurePath{"hello/world"sv}.PartsList(), ElementsAre("hello"sv, "world"sv));
  EXPECT_THAT(PurePath{"hello/world"sv}.PartsList(), ElementsAre("hello"sv, "world"sv));
  EXPECT_THAT(PurePath{"hello//world"sv}.PartsList(), ElementsAre("hello"sv, "world"sv));
  EXPECT_THAT(PurePath{"hello/./world"sv}.PartsList(), ElementsAre("hello"sv, "world"sv));
  EXPECT_THAT(PurePath{"hello/./world/././"sv}.PartsList(), ElementsAre("hello"sv, "world"sv));
}

#define EXPECT_PATH_EQ(lhs, rhs) EXPECT_EQ(PurePath{lhs}, PurePath{rhs})
#define EXPECT_PATH_NE(lhs, rhs) EXPECT_NE(PurePath{lhs}, PurePath{rhs})

TEST(PurePathTest, Equals) {
  ConfigureLogging();

  EXPECT_PATH_EQ("", "");
  EXPECT_PATH_EQ(".", ".");
  EXPECT_PATH_EQ("", ".");
  EXPECT_PATH_EQ("./", ".");
  EXPECT_PATH_EQ(".////", ".");
  EXPECT_PATH_EQ(".//././", ".");
  EXPECT_PATH_EQ("hello/world//", "hello/world");
  EXPECT_PATH_EQ("hello/world//", "./hello/world");
  EXPECT_PATH_EQ("//hello/world//", "/hello/world");
  EXPECT_PATH_EQ("/./hello/world//", "/hello/world/./");
  EXPECT_PATH_EQ("..", ".././.");
  EXPECT_PATH_EQ("../..//", "../..");

  // Also make sure that the path is not equal to its parent [which is a substring].
  EXPECT_PATH_NE("/data", "/data/baz");
  EXPECT_PATH_NE("/data/././baz", "/data/baz/bar");

  // Also make sure its not equal when the other path shares the same underlying starting data().
  {
    std::string_view view = "/data/bar";
    EXPECT_PATH_NE(PurePath{view}, PurePath{view.substr(/*pos*/0, /*count*/5)});
  }
}

// A parent is always different than its child (except for '/').
#define EXPECT_PATH_PARENT_EQ(actual, expected) \
    EXPECT_EQ(PurePath{actual}.Parent(), PurePath{expected}); \
    { auto act = PurePath{actual};   \
      EXPECT_NE(act, act.Parent());  \
    }
TEST(PurePathTest, Parent) {
  ConfigureLogging();

  // Special recursive case: parent of '/' is still '/'.
  EXPECT_EQ(PurePath{"/"}, PurePath{"/"}.Parent());
  EXPECT_NE(PurePath{""}, PurePath{"/"}.Parent());

  // All other cases are non-recursive.
  EXPECT_PATH_PARENT_EQ("", "..");
  EXPECT_PATH_PARENT_EQ("..", "../..");
  EXPECT_PATH_PARENT_EQ("../..", "../../..");
  EXPECT_PATH_PARENT_EQ("../../../../../../../../..", "../../../../../../../../../..");

  EXPECT_PATH_PARENT_EQ("/abc", "/");
  EXPECT_PATH_PARENT_EQ("abc", "");

  EXPECT_PATH_PARENT_EQ("/foo/bar", "/foo");
  EXPECT_PATH_PARENT_EQ("/foo/bar/b", "/foo/bar");
  EXPECT_PATH_PARENT_EQ("/foo/bar///baz///././/nay", "/foo/bar/baz");

  EXPECT_PATH_PARENT_EQ("foo/bar", "foo");
  EXPECT_PATH_PARENT_EQ("foo/bar/b", "foo/bar");
  EXPECT_PATH_PARENT_EQ("foo/bar///baz///././/nay", "foo/bar/baz");

  EXPECT_PATH_PARENT_EQ("../foo/bar", "../foo");
  EXPECT_PATH_PARENT_EQ("../foo/bar/b", "../foo/bar");
  EXPECT_PATH_PARENT_EQ("../foo/bar///baz///././/nay", "../foo/bar/baz");
}

#define EXPECT_PATH_NAME_EQ(expected, actual) EXPECT_EQ(PurePath{actual}, PurePath{expected}.Name())
TEST(PurePathTest, Name) {
  ConfigureLogging();

  EXPECT_PATH_NAME_EQ("", "");
  EXPECT_PATH_NAME_EQ("..", "..");
  EXPECT_PATH_NAME_EQ("../..", "..");
  EXPECT_PATH_NAME_EQ("../../../../../../../../..", "..");

  EXPECT_PATH_NAME_EQ("/", "");
  EXPECT_PATH_NAME_EQ("/abc", "abc");
  EXPECT_PATH_NAME_EQ("abc", "abc");

  EXPECT_PATH_NAME_EQ("/foo/bar", "bar");
  EXPECT_PATH_NAME_EQ("/foo/bar/b", "b");
  EXPECT_PATH_NAME_EQ("/foo/bar///baz///././/nay", "nay");
  EXPECT_PATH_NAME_EQ("/foo/bar///baz///././/nay//./.", "nay");

  EXPECT_PATH_NAME_EQ("foo/bar", "bar");
  EXPECT_PATH_NAME_EQ("foo/bar/b", "b");
  EXPECT_PATH_NAME_EQ("foo/bar///baz///././/nay", "nay");

  EXPECT_PATH_NAME_EQ("../foo/bar", "bar");
  EXPECT_PATH_NAME_EQ("../foo/bar/b", "b");
  EXPECT_PATH_NAME_EQ("../foo/bar///baz///././/nay", "nay");
}


struct PathEntry {
  Inode inode;
  PurePath path;  // full path

  static std::vector<PathEntry> Zip(std::vector<Inode>& inodes, std::vector<std::string>& paths) {
    CHECK_EQ(inodes.size(), paths.size());

    std::vector<PathEntry> entries;

    static bool debug = true;  // Print only once.

    if (debug) {
      LOG(DEBUG) << "PathEntry::Zip (begin)";
    }

    for (size_t i = 0; i < inodes.size(); ++i) {
      entries.push_back(PathEntry{inodes[i], PurePath{std::string{paths[i]}}});

      // TODO: this seems awkward, maybe refactor into PurePath + PurePathView ?
      DCHECK(entries[i].path.IsBorrowed() == false);

      if (debug) {
        LOG(DEBUG) << "PathEntry - add " << inodes[i] << " at '" << paths[i] << "'";
      }
    }

    debug = false;

    return entries;
  }
};

std::ostream& operator<<(std::ostream& os, const PathEntry& path_entry) {
  os << "PathEntry{inode=" << path_entry.inode << ",path=\"" << path_entry.path << "\"}";
  return os;
}

// This super-inefficient class models a Tree to a list of absolute path names.
// Obviously intended only for testing, since its algorithmically suboptimal.
struct PathEntryTree {
  std::vector<PathEntry> entries;

  static constexpr bool debug{false};
#define PET_LOG_DEBUG if (debug) LOG(DEBUG)

  std::optional<PathEntry> GetEntryFor(const std::string& path_name) {
    PurePath path{path_name};
    for (auto&& entry : entries) {
      if (entry.path == path) {
        return entry;
      }
    }
    return {};
  }

  bool HasDirectory(const std::string& path_name) {
    PurePath path{path_name};
    for (auto&& entry : entries) {
      if (entry.path == path) {
        return true;
      }
    }
    return false;
  }

  std::vector<PathEntry> OpenDirectory(const std::string& path_name) {
    PurePath path{path_name};
    return OpenDirectory(path);
  }

  std::vector<PathEntry> OpenDirectory(const PurePath& path) {
    std::vector<PathEntry> children;

    PET_LOG_DEBUG << "OpenDirectory(" << path << ")";

    for (auto&& entry : entries) {
      // Only find the immediate children, don't find any other offspring.
      PurePath parent = entry.path.Parent();
      if (parent == path) {
        if (parent == entry.path) {
          // Ignore recursive parents, e.g. '/'
          PET_LOG_DEBUG << "OpenDirectory - Ignore recursive parent " << parent;
          continue;
        }

        children.push_back(entry);

        DCHECK(!children.back().path.IsBorrowed());

        PET_LOG_DEBUG << "OpenDirectory - Child added = " << entry;
      }
    }


    return children;
  }

  size_t size() const {
    return entries.size();
  }
};


static std::vector<std::string> ParseLines(const char* what) {
  std::vector<std::string> do_split = android::base::Split(what, "\n");

  std::vector<std::string> output;
  for (std::string& s : do_split) {
    if (s.size() != 0) {
      output.push_back(s);
    }
  }

  return output;
}

static std::vector<Inode> ParseInodes(std::vector<std::string> inode_strings) {
  std::vector<Inode> results;

  for (std::string& s : inode_strings) {
    Inode inode;

    std::string error_msg;
    bool inode_parse_succeeded = Inode::Parse(s, /*out*/&inode, /*out*/&error_msg);
    CHECK(inode_parse_succeeded) << s << ", error: " << error_msg;

    results.push_back(inode);
  }

  return results;
}



static PathEntryTree CreateFakePathEntries() {
#if 1
    // adb shell 'find /data/data/com.google.android.googlequicksearchbox/ | xargs stat -c "%d@%i"'
    static const char* kInodeValues = R"1N0D3(
66323@1117133
66323@1127133
66323@1137133
66323@1327133
66323@1336383
66323@1376559
66323@1376448
66323@1376446
66323@1376596
66323@1376638
66323@1376438
66323@1376444
66323@1376563
66323@1376434
66323@1376439
66323@1336384
66323@1335704
66323@1336031
66323@1335751
66323@1337692
66323@1336090
66323@1336385
66323@1376543
66323@1376449
66323@1376544
66323@1376547
66323@1376436
66323@1336619
66323@1336070
66323@1336681
66323@1336064
66323@1336088
66323@1336470
66323@1335570
66323@1335668
66323@1336471
66323@1335514
66323@1376475
66323@1376462
66323@1376435
66323@1376476
66323@1376632
66323@1351934
66323@1351948
66323@1351949
66323@1351950
66323@1351939
66323@1376479
66323@1376437
66323@1376450
66323@1376480
66323@1376442
66323@1376451
66323@1376454
66323@1376457
66323@1376452
66323@1376546
66323@1335629
66323@1343800
66323@1343801
66323@1336890
66323@1336616
66323@1336921
66323@1327135
66323@1335862
66323@1336547
66323@1351681
66323@1351684
66323@1351744
66323@1351705
66323@1351699
66323@1351711
66323@1351748
66323@1351734
66323@1351682
66323@1351683
66323@1351719
66323@1351739
66323@1351689
66323@1351724
66323@1351690
66323@1351745
66323@1351686
66323@1351691
66323@1351741
66323@1351687
66323@1351747
66323@1351736
66323@1351698
66323@1351697
66323@1351730
66323@1351712
66323@1351703
66323@1351721
66323@1351701
66323@1351717
66323@1351716
66323@1351695
66323@1351720
66323@1351688
66323@1351685
66323@1351727
66323@1351738
66323@1351729
66323@1351704
66323@1351743
66323@1351723
66323@1351700
66323@1351713
66323@1351707
66323@1351709
66323@1351731
66323@1351732
66323@1351693
66323@1351726
66323@1351708
66323@1351714
66323@1351728
66323@1351694
66323@1351706
66323@1351722
66323@1351696
66323@1351715
66323@1351740
66323@1351725
66323@1351702
66323@1351710
66323@1351737
66323@1351742
66323@1351746
66323@1351735
66323@1351733
66323@1351692
66323@1351718
66323@1336864
66323@1335446
66323@1337584
66323@1335740
66323@1335854
66323@1336644
66323@1376553
66323@1376554
66323@1376469
66323@1376637
66323@1376555
66323@1376556
66323@1376570
66323@1376565
66323@1376557
66323@1376558
66323@1376432
66323@1376567
66323@1376440
66323@1343805
66323@1336646
66323@1336947
66323@1336393
66323@1336394
66323@1335920
66323@1336041
66323@1335650
66323@1336667
66323@1336665
66323@1335760
66323@1343802
66323@1343803
66323@1344013
66323@1344134
66323@1376276
66323@1336598
66323@1336634
66323@1336652
66323@1336656
66323@1336446
66323@1336863
66323@1337682
66323@1336866
66323@1336867
66323@1335678
66323@1336865
66323@1327631
66323@1327664
66323@1327660
66323@1327134
66323@1336825
66323@1337969
66323@1335938
66323@1337849
66323@1337839
66323@1337866
66323@1337122
66323@1337756
66323@1336966
66323@1337982
66323@1337097
66323@1336683
66323@1337824
66323@1337460
66323@1337775
66323@1337810
66323@1337847
66323@1335853
66323@1337594
66323@1337808
66323@1337817
66323@1337092
66323@1337699
66323@1337593
66323@1337089
66323@1335959
66323@1337788
66323@1337181
66323@1337610
66323@1336980
66323@1337972
66323@1337554
66323@1337661
66323@1337770
66323@1335951
66323@1337984
66323@1336061
66323@1337497
66323@1337835
66323@1337805
66323@1336557
66323@1336780
66323@1337816
66323@1337732
66323@1337983
66323@1337954
66323@1337713
66323@1337687
66323@1337597
66323@1337466
66323@1337814
66323@1337603
66323@1337031
66323@1336784
66323@1337534
66323@1337727
66323@1337693
66323@1337791
66323@1337567
66323@1337748
66323@1337777
66323@1336194
66323@1337843
66323@1336971
66323@1337974
66323@1336785
66323@1337871
66323@1337815
66323@1337709
66323@1337551
66323@1337088
66323@1337776
66323@1337672
66323@1335979
66323@1337823
66323@1336028
66323@1337526
66323@1337971
66323@1337853
66323@1337596
66323@1337901
66323@1337572
66323@1335921
66323@1336954
66323@1337820
66323@1335492
66323@1337809
66323@1337696
66323@1335636
66323@1337608
66323@1335746
66323@1337731
66323@1337967
66323@1337769
66323@1337751
66323@1337973
66323@1337697
66323@1335939
66323@1336001
66323@1337598
66323@1336713
66323@1337702
66323@1337844
66323@1337862
66323@1336978
66323@1337975
66323@1336798
66323@1337858
66323@1337605
66323@1337510
66323@1337914
66323@1376548
66323@1376549
66323@1376550
66323@1376564
66323@1376571
66323@1376683
66323@1376681
66323@1376652
66323@1376682
66323@1376684
66323@1376649
66323@1376568
66323@1376569
66323@1376576
66323@1376578
66323@1376579
66323@1376581
66323@1376582
66323@1376577
66323@1376580
66323@1376597
66323@1376598
66323@1376602
66323@1376599
66323@1376600
66323@1376601
66323@1376583
66323@1376551
66323@1376552
66323@1376560
66323@1376561
66323@1376562
66323@1376591
66323@1376497
66323@1376482
66323@1376536
66323@1376533
66323@1376532
66323@1336380
66323@1336425
66323@1337738
66323@1337978
66323@1337796
66323@1337819
66323@1337781
66323@1337857
66323@1337963
66323@1335777
66323@1337569
66323@1337818
66323@1337758
66323@1337742
66323@1336950
66323@1337730
66323@1337021
66323@1335774
66323@1337813
66323@1337755
66323@1337964
66323@1337860
66323@1338005
66323@1336592
66323@1336428
66323@1335779
66323@1337976
66323@1337461
66323@1337789
66323@1337745
66323@1337602
66323@1337698
66323@1336813
66323@1337606
66323@1337896
66323@1337712
66323@1337970
66323@1337981
66323@1335435
66323@1337587
66323@1337821
66323@1337716
66323@1337754
66323@1337786
66323@1337778
66323@1336032
66323@1338029
66323@1337550
66323@1337783
66323@1337609
66323@1337107
66323@1337841
66323@1337557
66323@1337700
66323@1337604
66323@1337920
66323@1337469
66323@1337811
66323@1337715
66323@1337980
66323@1336949
66323@1337812
66323@1337806
66323@1337779
66323@1337600
66323@1336080
66323@1337601
66323@1336920
66323@1337703
66323@1337033
66323@1336824
66323@1337104
66323@1337854
66323@1336078
66323@1336970
66323@1337917
66323@1337671
66323@1337926
66323@1336802
66323@1337797
66323@1338031
66323@1337095
66323@1337676
66323@1337708
66323@1335905
66323@1336124
66323@1337859
66323@1337784
66323@1337795
66323@1337724
66323@1337822
66323@1336426
66323@1337852
66323@1337856
66323@1337855
66323@1337780
66323@1337607
66323@1336956
66323@1337038
66323@1336513
66323@1336918
66323@1336739
66323@1337924
66323@1337530
66323@1337757
66323@1337850
66323@1337701
66323@1336613
66323@1337737
66323@1336817
66323@1337977
66323@1336314
66323@1337465
66323@1336991
66323@1337279
66323@1337922
66323@1337710
66323@1337599
66323@1337861
66323@1336388
66323@1336389
66323@1336084
66323@1335615
66323@1336375
66323@1335759
66323@1336036
66323@1336433
66323@1335649
66323@1337744
66323@1336008
66323@1336004
66323@1336026
66323@1335834
66323@1336376
66323@1336377
66323@1336505
66323@1336378
66323@1335382
66323@1337015
66323@1336108
66323@1337103
66323@1335413
66323@1335935
66323@1335429
66323@1337733
66323@1336382
66323@1336381
66323@1336633
66323@1337522
66323@1336694
66323@1335428
)1N0D3";

    const char* kPathNames = R"F1L3N4M3(
/
/data/
/data/data/
/data/data/com.google.android.googlequicksearchbox/
/data/data/com.google.android.googlequicksearchbox/app_si
/data/data/com.google.android.googlequicksearchbox/app_si/searchbox_stats_content_store
/data/data/com.google.android.googlequicksearchbox/app_si/searchbox_stats_content_store/content_store.db-shm
/data/data/com.google.android.googlequicksearchbox/app_si/searchbox_stats_content_store/content_store.db-wal
/data/data/com.google.android.googlequicksearchbox/app_si/searchbox_stats_content_store/content_store.db
/data/data/com.google.android.googlequicksearchbox/app_si/searchbox_stats_content_store/content_store.db-wipecheck
/data/data/com.google.android.googlequicksearchbox/app_si/shortcuts_content_store
/data/data/com.google.android.googlequicksearchbox/app_si/shortcuts_content_store/content_store.db-shm
/data/data/com.google.android.googlequicksearchbox/app_si/shortcuts_content_store/content_store.db-wipecheck
/data/data/com.google.android.googlequicksearchbox/app_si/shortcuts_content_store/content_store.db-wal
/data/data/com.google.android.googlequicksearchbox/app_si/shortcuts_content_store/content_store.db
/data/data/com.google.android.googlequicksearchbox/app_si/now_content_store
/data/data/com.google.android.googlequicksearchbox/app_si/now_content_store/content_store.db-wal
/data/data/com.google.android.googlequicksearchbox/app_si/now_content_store/content_store.db-shm
/data/data/com.google.android.googlequicksearchbox/app_si/now_content_store/now_content_store_blob_9060309284749123123.bin
/data/data/com.google.android.googlequicksearchbox/app_si/now_content_store/content_store.db-wipecheck
/data/data/com.google.android.googlequicksearchbox/app_si/now_content_store/now_content_store_blob_9184734810098631032.bin
/data/data/com.google.android.googlequicksearchbox/app_si/now_content_store/content_store.db
/data/data/com.google.android.googlequicksearchbox/app_si/proactive_key_value_content_store
/data/data/com.google.android.googlequicksearchbox/app_si/proactive_key_value_content_store/content_store.db-shm
/data/data/com.google.android.googlequicksearchbox/app_si/proactive_key_value_content_store/content_store.db
/data/data/com.google.android.googlequicksearchbox/app_si/proactive_key_value_content_store/content_store.db-wipecheck
/data/data/com.google.android.googlequicksearchbox/app_si/proactive_key_value_content_store/content_store.db-wal
/data/data/com.google.android.googlequicksearchbox/app_si/srp_content_store
/data/data/com.google.android.googlequicksearchbox/app_si/srp_content_store/content_store.db-wal
/data/data/com.google.android.googlequicksearchbox/app_si/srp_content_store/content_store.db
/data/data/com.google.android.googlequicksearchbox/app_si/srp_content_store/content_store.db-wipecheck
/data/data/com.google.android.googlequicksearchbox/app_si/srp_content_store/content_store.db-shm
/data/data/com.google.android.googlequicksearchbox/app_si/state_dump_event_content_store
/data/data/com.google.android.googlequicksearchbox/app_si/state_dump_event_content_store/content_store.db-wipecheck
/data/data/com.google.android.googlequicksearchbox/app_si/state_dump_event_content_store/content_store.db-shm
/data/data/com.google.android.googlequicksearchbox/app_si/state_dump_event_content_store/content_store.db
/data/data/com.google.android.googlequicksearchbox/app_si/state_dump_event_content_store/content_store.db-wal
/data/data/com.google.android.googlequicksearchbox/app_si/homescreen_shortcut_content_store
/data/data/com.google.android.googlequicksearchbox/app_si/homescreen_shortcut_content_store/content_store.db-shm
/data/data/com.google.android.googlequicksearchbox/app_si/homescreen_shortcut_content_store/content_store.db-wal
/data/data/com.google.android.googlequicksearchbox/app_si/homescreen_shortcut_content_store/content_store.db
/data/data/com.google.android.googlequicksearchbox/app_si/homescreen_shortcut_content_store/content_store.db-wipecheck
/data/data/com.google.android.googlequicksearchbox/app_si/search_widget_overlay_content_store
/data/data/com.google.android.googlequicksearchbox/app_si/search_widget_overlay_content_store/content_store.db-wal
/data/data/com.google.android.googlequicksearchbox/app_si/search_widget_overlay_content_store/content_store.db-shm
/data/data/com.google.android.googlequicksearchbox/app_si/search_widget_overlay_content_store/content_store.db-wipecheck
/data/data/com.google.android.googlequicksearchbox/app_si/search_widget_overlay_content_store/content_store.db
/data/data/com.google.android.googlequicksearchbox/app_si/opa_content_store
/data/data/com.google.android.googlequicksearchbox/app_si/opa_content_store/content_store.db-wal
/data/data/com.google.android.googlequicksearchbox/app_si/opa_content_store/content_store.db-wipecheck
/data/data/com.google.android.googlequicksearchbox/app_si/opa_content_store/content_store.db
/data/data/com.google.android.googlequicksearchbox/app_si/opa_content_store/content_store.db-shm
/data/data/com.google.android.googlequicksearchbox/app_si/accl_conv_client_content_store
/data/data/com.google.android.googlequicksearchbox/app_si/accl_conv_client_content_store/content_store.db-shm
/data/data/com.google.android.googlequicksearchbox/app_si/accl_conv_client_content_store/content_store.db
/data/data/com.google.android.googlequicksearchbox/app_si/accl_conv_client_content_store/content_store.db-wal
/data/data/com.google.android.googlequicksearchbox/app_si/accl_conv_client_content_store/content_store.db-wipecheck
/data/data/com.google.android.googlequicksearchbox/app_session
/data/data/com.google.android.googlequicksearchbox/app_monet_init_data
/data/data/com.google.android.googlequicksearchbox/app_monet_init_data/search.TYPE_SEARCHNOW.binarypb
/data/data/com.google.android.googlequicksearchbox/no_backup
/data/data/com.google.android.googlequicksearchbox/no_backup/com.google.InstanceId.properties
/data/data/com.google.android.googlequicksearchbox/no_backup/com.google.android.gms.appid-no-backup
/data/data/com.google.android.googlequicksearchbox/code_cache
/data/data/com.google.android.googlequicksearchbox/app_sid
/data/data/com.google.android.googlequicksearchbox/app_g3_models
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/CLG.prewalk.fst
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/verbalizer_terse.mfar
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/en-US_app-actions_prompted-app-name_TWIDDLER_FST.fst
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/contacts.abnf
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/en-US_monastery_contact-disambig-static_TWIDDLER_FST.fst
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/wordlist.syms
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/norm_fst
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/APP_NAME.fst
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/APP_NAME.syms
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/ep_portable_mean_stddev
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/portable_meanstddev
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/SONG_NAME.syms
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/g2p_phonemes.syms
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/TERSE_LSTM_LM.lstm_lm.main_model.uint8.data
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/voice_actions.config
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/CONTACT_NAME.fst
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/TERSE_LSTM_LM.lstm_lm.self_normalized_model.uint8.data
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/pumpkin.mmap
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/CONTACT_NAME.syms
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/word_confidence_classifier
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/offline_action_data.pb
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/config.pumpkin
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/compile_grammar.config
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/lstm_model.uint8.data
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/en-US_read-items_SearchMessageAction-Prompted-Read_TWIDDLER_FST.fst
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/embedded_class_denorm.mfar
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/g2p.data
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/dictation.config
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/endpointer_model.mmap
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/endpointer_model
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/c_fst
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/ep_portable_model.uint8.mmap
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/SONG_NAME.fst
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/CONTACT.transform.mfar
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/hmmlist
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/portable_lstm
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/lexicon.U.fst
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/embedded_normalizer.mfar
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/semantics.pumpkin
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/g2p_graphemes.syms
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/dict
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/en-US_read-items_SearchMessageAction-Prompted-Skip_TWIDDLER_FST.fst
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/en-US_confirmation_confirmation-cancellation_TWIDDLER_FST.fst
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/en-US_media-actions_music-service-controllable_TWIDDLER_FST.fst
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/magic_mic.config
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/metadata
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/am_phonemes.syms
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/hmm_symbols
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/en-US_gmm-actions_gmm-nav-actions_TWIDDLER_FST.fst
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/en-US_time-actions_time-context_TWIDDLER_FST.fst
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/input_mean_std_dev
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/benchmark.volvo.txt
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/en-US_calendar-actions_AddCalendarEvent-Prompted-FieldToChange_TWIDDLER_FST.fst
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/g2p_fst
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/commands.abnf
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/endpointer_dictation.config
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/prons_exception_dictionary_file.txt
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/grammar.config
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/dnn
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/en-US_monastery_GenericAction-Prompted-ContactName_TWIDDLER_FST.fst
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/phonelist
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/rescoring.fst.compact
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/voice_actions_compiler.config
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/offensive_word_normalizer.mfar
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/monastery_config.pumpkin
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/TERSE_LSTM_LM.lstm_lm.syms
/data/data/com.google.android.googlequicksearchbox/app_g3_models/en-US/endpointer_voicesearch.config
/data/data/com.google.android.googlequicksearchbox/app_textures
/data/data/com.google.android.googlequicksearchbox/files
/data/data/com.google.android.googlequicksearchbox/files/current_experiments.bin
/data/data/com.google.android.googlequicksearchbox/files/training_question_data
/data/data/com.google.android.googlequicksearchbox/files/now_request_queue
/data/data/com.google.android.googlequicksearchbox/files/velour
/data/data/com.google.android.googlequicksearchbox/files/velour/preferences
/data/data/com.google.android.googlequicksearchbox/files/velour/preferences/ipa
/data/data/com.google.android.googlequicksearchbox/files/velour/preferences/ipa/IpaBgTask
/data/data/com.google.android.googlequicksearchbox/files/velour/preferences/wernicke_player
/data/data/com.google.android.googlequicksearchbox/files/velour/feature_data
/data/data/com.google.android.googlequicksearchbox/files/velour/feature_data/ipa
/data/data/com.google.android.googlequicksearchbox/files/velour/feature_data/ipa/ipa_0p_instant_cache
/data/data/com.google.android.googlequicksearchbox/files/velour/feature_data/ipa/ZeroPrefixContacts
/data/data/com.google.android.googlequicksearchbox/files/velour/feature_data/ipa/ipa_content_store
/data/data/com.google.android.googlequicksearchbox/files/velour/feature_data/ipa/ipa_content_store/content_store.db
/data/data/com.google.android.googlequicksearchbox/files/velour/feature_data/ipa/ipa_content_store/content_store.db-wal
/data/data/com.google.android.googlequicksearchbox/files/velour/feature_data/ipa/ipa_content_store/content_store.db-wipecheck
/data/data/com.google.android.googlequicksearchbox/files/velour/feature_data/ipa/ipa_content_store/content_store.db-shm
/data/data/com.google.android.googlequicksearchbox/files/velour/jar_data
/data/data/com.google.android.googlequicksearchbox/files/velour/verified_jars
/data/data/com.google.android.googlequicksearchbox/files/velour/dex_cache
/data/data/com.google.android.googlequicksearchbox/files/native_crash_dir
/data/data/com.google.android.googlequicksearchbox/files/native_crash_dir/com.google.android.googlequicksearchbox:search
/data/data/com.google.android.googlequicksearchbox/files/current_configuration.bin
/data/data/com.google.android.googlequicksearchbox/files/dynamic_update_config_log
/data/data/com.google.android.googlequicksearchbox/files/brainsuggest
/data/data/com.google.android.googlequicksearchbox/files/brainsuggest/libbrainsuggest.so
/data/data/com.google.android.googlequicksearchbox/files/brainsuggest/tensors.bin
/data/data/com.google.android.googlequicksearchbox/files/persisted_profiling_statistics
/data/data/com.google.android.googlequicksearchbox/files/en-US
/data/data/com.google.android.googlequicksearchbox/files/en-US/x_hotword.data
/data/data/com.google.android.googlequicksearchbox/files/recently
/data/data/com.google.android.googlequicksearchbox/files/recently/libcore.test@gmail.com
/data/data/com.google.android.googlequicksearchbox/files/dump
/data/data/com.google.android.googlequicksearchbox/files/bloblobber
/data/data/com.google.android.googlequicksearchbox/files/bloblobber/pending
/data/data/com.google.android.googlequicksearchbox/files/web_suggest_model
/data/data/com.google.android.googlequicksearchbox/files/web_suggest_model/index.bin
/data/data/com.google.android.googlequicksearchbox/files/client_data_request_log
/data/data/com.google.android.googlequicksearchbox/app_webview
/data/data/com.google.android.googlequicksearchbox/app_webview/variations_seed_new
/data/data/com.google.android.googlequicksearchbox/app_webview/Cookies-journal
/data/data/com.google.android.googlequicksearchbox/app_webview/variations_stamp
/data/data/com.google.android.googlequicksearchbox/app_webview/variations_seed
/data/data/com.google.android.googlequicksearchbox/app_webview/Cookies
/data/data/com.google.android.googlequicksearchbox/app_shared_prefs
/data/data/com.google.android.googlequicksearchbox/app_shared_prefs/StartupSettings.bin
/data/data/com.google.android.googlequicksearchbox/app_shared_prefs/SearchSettings.bin
/data/data/com.google.android.googlequicksearchbox/cache
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/fd12e9a1ba593cbe075c925a95626534054861f9dd82fa27f656ac0088648fd9.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/4a19d733c917d730443eaff509ee0496e116f79c69d0d2fa54a594f5accd19d1.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/76f6a9848373162cd602a03e00e363ad8455e62293e9218d57da728f7382ee34.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/666d7a8c0d257a9a9f1457a1bb04b8eda821966283d466db872d5693de42d29b.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/0f714cd570228ce48e2741fd6ff959bcbbab49e40427b6eb5c4b1ff3aae4ad40.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/458acc9b996dc815a7259a2c9dbf5b94ae549da3d66f3649d1e0a1e239214390.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/13ec5eaf61460a0be11467ba2e0efad6602142e45fd1c97988bc42aa54832407.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/99b468ff1549d8e260ce274f7afdaaf32fb70064c31596173b812ea2d11c8097.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/40c737d31b2d843c5772648820deeb4c8d5bef9426b025f75bdc72ba7913e0fd.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/41186543405abfea16b57da566ce9e509826f9b1b6337473d05d9421f53912aa.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/cda361ffde7db6bedfeb9f01a63dd51ebbe4b733d3c6be69cede7a681d20b583.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/108adf201cd9b479777504c8e7fb74299bbc2b51082d2872a34616abe6c743fb.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/47a3358d7989bf06c4ce1ecb350149b1669bf16073ea1d321371a02ad3507d63.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/0c25c0bd4df514cdd4531872553e053b74a3b9a60476871184b7e8c2a1b67048.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/936a9280b8b33ffaf98e9497927d7d84cfc87757bf235569fa950c55922ab76c.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/4cb136284aa9e19be2492e23d421297736f888ddf03cd7eebdb690d4f3b929c1.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/43f3dcad386912254bc1b2a6cd8414c3373f465665655584a0cf31b9c2f6ce6f.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/73c62366e6dda1ee960692d71e4ff9ba92d193b966986866793e64dec10fdc9b.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/d361cbbc4c1c8a5eeda8dda6173926c112f28f0bc49efc7946a0c218b4195fa3.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/eb0939417658eea85bdccd5b4c1c460f836649e442cad3100393e039f8f82fe6.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/5979b3d43ade0ddbf8aa86e3ef5e2951fb677bcc0a39d133433bd58989328130.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/82f2aaeedc6ddb9a8086912bd8064c5aac85437814d7ef6e5a6fb5e22bd71491.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/cf4ed99e5d88c5bbf18d812c6eb7a5b90f12e47f346b33061f6ad6c073d81be7.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/f5f95fe8ca532f13faeb1209981138d786f0df2e061d151fe655a571a8ddd88c.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/5a5f20fcb280a697e2f22c460d796fbd291481760480a764d6fe6a6c83e6380a.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/dba86bd8a4f75d2148b80fb04d525193b29b060fbf8a4743fe1b41e39c4fb25d.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/d7017ff3c9fbda9a376ff72d8a95eb9e0a5858cf50ee82a5b92d6e995550867d.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/a6bf758115a73beafa9124803667e93729442e7cad367a86608ad9ad8655b08b.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/24ae44003669f9f9640832b4c9cf9acb8ac3c2adf5ab5a2444a7715b090b3f67.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/b39aac6d9d0b5ea2ce78831602e48e0a48f7f2c792697e9c58de1d45b27a792a.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/9194df1b37d6a7da9ee8fd03abebdc3e81ec6ea366224eecb0cd7d3d66024062.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/f64b3e72b098f53f10ca3f3031b93df60c8ef407510bab8a003c9747e82f6043.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/6ff297c691797ab5fd5222b0c1fa13abc750fc031685a29589eace7748864318.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/8704bbe8a29455b6034d773c57246b1633da5393fd102f87fcb6eca819b82753.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/ef4146d94b8e32988b3cb0eba7e967cfa9627a683a1359cf00a1d76aa5022680.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/ebf974f9350e2f784145463f0afcccac69f265af0e8b233813617829684a290f.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/06a6aaede02e9527e1eb6dad977a7889e22d2dfcc098f9342eacc134c31363df.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/d915555e83e27c4d5f6dcd1badaaed666fa80e5ae11d6d2382e666efe606bd1f.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/a189554aa8b3185799fdb5bfb89cc42698c544a1041e65709b0e79d267cabdc5.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/0f310a87cae45484d5da35274ff89463eb966a1aeb32d53a2fb8350cf9d836b2.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/df7598066371b7da446954feb42a1febdb8921cacf436285e85606cae9de4bb5.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/ff8d8869f35a9a4243fac1ce8ab5deff7ee161dfc8c2ea1107099ec1cf74e100.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/75d36a907297689726ca96ef721c091c04a879f1f096f503076dd172834a27dc.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/5e6fee09c93c6f1d493bd04dd18cc8043c0b40093d85ed94c2df28ab129b226b.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/d7ddd06a2070b76dc05fe12741d7882df5a4312b174a11ce3d98d059fcc17173.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/7bcac404ee6981364837ac1a89033184ff65939507a81caa7c43ea52a195e215.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/392ac948386e63063f941449eeb9199c3f1a05959934c47c5987bbf6aa0721a3.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/df13e0aac1211176a1939bf2198f9e0e7dedd1f043875a4093ce0265cd02744e.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/052649971efbb0e18631044219b92ab68f12dc244041042b24203c88a62377b6.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/1e86ba82b4061a7ef799089ce29826fcba0ba07c77aec6638f20e850d1864144.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/7fe25319e6b2049b96c6659264568defe6a7e21bb3817685970b3a3aa511d8c8.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/3a66049907f84778aa548c747d9a52c2e67ce880c19fa4b0a8b4e58ae96fd9ef.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/54f622cb04525cc1953cc1ffbe12646be3290de6ca378ce5508869860df761e5.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/6fa117d5fcf115733a154f5d0911ce05b103ea33d5eee65d2b08a83605cf6e80.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/cc1d2b8828dc4dba9f0ea7d980eb8b24d2b0792c3282725552a56f7c8929459f.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/9b59194c025f9e6fc5c1d60ee444be69f14546b7efa4389a30ee6db88fbc207f.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/8686d00f5e82bb2343d8154fad3d66afca1420c45e4f63fe5decb6b9f5b84d2b.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/6b7ac224bb235b03798c358125eadc5d805445744543c368ebc0a4f7bb7a4328.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/dfb89354e0b433989eab276004296575f5b5e3ecb8c700cfcee620765ef0e74d.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/ef0656c2dce6c462c2e2591b5c43b76c1fe83ebebbaa778c1194706577c46d5c.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/7bfb6c4c45d0dcfbd66456456c5300b3ba83d52f37c5434e3e78baf0b54e5c07.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/c61ee02616c8d1c87be89499cb1ebdcd7267e47e51fa53e10578e8935a8b7aa9.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/f32062ec857d3d40d3a82359511167113065019d09d10755764aec91ba37bb32.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/d678ca3b06ea1872ebb20236ed1099e1b6e1451c51d78ab98d914abead7e4651.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/ee4c2e6785e3062be57305190a480ee437f6569471534533dd3524794b125ace.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/9a3ee31c30754a89da9280cbff44440e8e974bfac4c815376a0df768ef926590.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/334ae045a86998d77ed3fca093d7c43dfd5be53f939d156d9e71a885263739c1.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/65b4c081cd081eb8716e435c51abf7882f9aacb47005d77aa336ddf8190f2249.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/e2d38a33de7d378bf8a2989ae6db7b20d9168dd9b4c078225ced7aef9154a370.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/988ece4e5c191b30ff71dd2ce3fa3dc16f22dd3076702c17e9f3373612765c9c.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/272e01e003b0a4ed9b8ebb908d88cf1fbc841f65cf3c5994e99c7e5b10332209.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/eb1fd91a2d9b5feba4504c7ee8182ec680121e83a1571a292d52bf7cac12c396.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/ea4967bd3dec3fdcf48ba9682777ae09b1be48ae861a5fb55f8650fccb24aeb5.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/23653a7282567de2cc678619554153f1aff5409061b2a08ffedf208b10c7fd9f.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/6e4b9df052f0c4c0b726301c66e4aedc596a01416268857f3742c1eaf6760c64.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/7f82912066c187783d04de0b189314fcde9d33208335827a6d0fa8755637a136.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/f07293e9a6dc9a826363cb52fbc0db1f75fbba49814cb626db63affa74dbc9bc.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/46df84d2163240ae59efe0c91b94b3d23018daa816de1d44405275e17b5f4e77.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/1101fd494d1a226a790d5091f04fae9bcab5543eec8c80a0d3dd8b83a8d31c14.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/2f405ef994ff2c7470116d092ec7e9a8833400354479760742e616f800e19831.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/5f1efddc7e6c6bb7984813794cd275f0cf46d2bf598ac16cc7adc05c9878eb22.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/c641b68789fcc05feb518d6ea7dd5ffa1344c124263e67573c86786766547fa8.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/f9b35c5e0adbd9f0084559cd909d57a1dc8928b5c48887a1119d226226664270.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/3ab2ef4b9b5f1900ff51f892d531b2abc539f9acfb728c841f35dab93bde160f.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/c92c58cb868b0c698c9e24ea9dfb63f1f4587a04fbc0cc6d495058ebb7534f69.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/46cb9725c59d409c7e7603f9eab6d2dbd3b29e5d4aef2fb154d5ceda40a33f85.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/b4fd15d151bbcf2f299a24ccd2c7e94afae7d6eccf7208f65b06febc5958d95d.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/191e06e43ac8a9618a5c1f10178e7cdf6e609f14dc7ec56be2ea89ac19b5e253.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/69191aa596bfd2633279e0488152f67565ae47bf3e9e728b9c57376bfd2abdc3.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/222870114e13280aa20077b14588e6d2fa8e7a7b347cde4a01553e395fb40a22.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/ba3636d73e375a6f16a878752464adaa57a03614dbb3e2d68e26d08d686262c3.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/b9b7fb42891473d4906c7acb11f8680565eb02eabce71a8645f917bb1375c0d2.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/7d62853edb09b73996a0d4bb369067e45fc229926a8961207596a3162941dccd.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/c0bcad354275f905e235af359dd789db4110201a4ef1fd7f8d4aae3563ff06a5.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/66397bf25b427e4d910ca117e3ce8afb8e19012403c7a1716696eca2da261884.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/487b35586c4591772393a2d2f430d00f97c3d36ad8ee7be784f130249cac7c31.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/6b664a4b20fde7754d5448f129532fded9103284aa101b50b79f810246f75a3b.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/2752173075a4ef17451a0a3db546956389eff82db209711e4a1fff47e90b6065.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/265252b3db2c2501483502b6aadab3ea891f32cae539b5ccc7ef8295b63f4018.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/b5cc6b97c28bd853ebf7d853a7b19e4b5018c01eae8823ac537800c2cbb06011.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/c9b094709f2b2773b5e5258716df0663b2aee98a6ea47c3cb4040322123cb99c.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/7f158d1bfbafe42c7b3118d2b9ea701bda16df10dbdf9c4f2779ea15595a331f.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/518548841153bce3488afa3a36ad6e6cbddb4f1689f5e9366ee80e206b6a1ffe.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/629e460d17bf23cce5d75bbe8672e037db86d8d757cc4efd9a1a0f53d435425b.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/667ddeda218b4f95a47792345550d546e00fe2a52a505437d30608cefd0fc4bd.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/20d9f1018284a761162108e9a82d6a73b1fa8a9fd6866a506db77ed07ec5e578.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/1f35f42d565dd11860e30c41241c78bc5f06d724117bfc83b3784c66d52e332f.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/58cdc6432a1cd95f7f6427744019b59d164384edfc54aa51537d4685f847ba39.0
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/journal
/data/data/com.google.android.googlequicksearchbox/cache/image_manager_disk_cache/61f774ab3a005d56366b267b08b994e5b035ff8eab1e454f00fb2cf7b356a46f.0
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars/en-US
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars/en-US/contacts
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars/en-US/contacts/prons.cache
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars/en-US/contacts/v1539635905559
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars/en-US/contacts/v1539635905559/digest
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars/en-US/contacts/v1539635905559/semantic_fst
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars/en-US/contacts/v1539635905559/grammar_symbols
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars/en-US/contacts/v1539635905559/semantic_symbols
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars/en-US/contacts/v1539635905559/metadata
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars/en-US/contacts/v1539635905559/grammar_clg
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars/en-US/hands_free_commands
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars/en-US/hands_free_commands/v1536705472984
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars/en-US/hands_free_commands/v1536705472984/grammar_clg
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars/en-US/hands_free_commands/v1536705472984/semantic_fst
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars/en-US/hands_free_commands/v1536705472984/semantic_symbols
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars/en-US/hands_free_commands/v1536705472984/digest
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars/en-US/hands_free_commands/v1536705472984/metadata
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars/en-US/hands_free_commands/v1536705472984/grammar_symbols
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars/en-US/hands_free_commands/prons.cache
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars/en-US/music_names
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars/en-US/music_names/v1536705480879
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars/en-US/music_names/v1536705480879/metadata
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars/en-US/music_names/v1536705480879/SONG_NAME.fst
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars/en-US/music_names/v1536705480879/SONG_NAME.syms
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars/en-US/music_names/v1536705480879/digest
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars/en-US/contact_names
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars/en-US/contact_names/v1539635914600
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars/en-US/contact_names/v1539635914600/CONTACT_NAME.fst
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars/en-US/contact_names/v1539635914600/CONTACT_NAME.syms
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars/en-US/contact_names/v1539635914600/digest
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars/en-US/contact_names/v1539635914600/metadata
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars/en-US/app_names
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars/en-US/app_names/v1543480552712
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars/en-US/app_names/v1543480552712/APP_NAME.fst
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars/en-US/app_names/v1543480552712/digest
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars/en-US/app_names/v1543480552712/metadata
/data/data/com.google.android.googlequicksearchbox/cache/g3_grammars/en-US/app_names/v1543480552712/APP_NAME.syms
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/79b5269c206115a4_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/aa9db037f918da1f_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/aebae57f6f7dcdb9_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/e446f170a9c613a1_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/2f06680d22ff6fbf_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/ffa3f495612db016_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/69c20684e88c955b_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/ef65bf506ba3e339_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/014821f96953c508_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/1180e087d9bd1160_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/3b36cd7b2f6df416_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/e5701f55e9ce22c8_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/9ec568d6b3dc0762_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/88f57d1088993219_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/83fd8318538fbe29_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/22930ed83887868c_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/333bf7ac47cc9770_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/adfd903f6a8ce876_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/2a1237e13688c120_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/3f3f18bf8e704931_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/b8262fc8c9591057_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/59829c5897cb9d93_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/index-dir
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/index-dir/the-real-index
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/feb5af6bca039e09_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/0050e1bcb6d6546c_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/3a3d22ec4fc7ad21_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/5f269f49d811cd82_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/e1cf52389fbebceb_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/359582e09cf26c7a_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/964347070fc23ea0_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/fb7d48b4e068afda_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/a5c586e8f0aeb850_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/37fadb6203e4e379_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/a7c25e80d95ef15d_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/a11922fc39ec0249_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/bebb870e573c852c_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/89c95cdfc9b59f48_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/fcb3fff3117a2d12_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/5c48229cf8e35d0b_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/55a3abf82b2a626c_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/f79ff1a77d9e9492_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/ca9ccf019443fb16_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/986246894e9084ec_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/e546ff051d5dbafc_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/23b6960b741da560_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/8c9509f47aa07ed8_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/81be3f3a1ebb3222_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/4edb09d9737acff2_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/e89950485ea68183_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/e40df7fec15afae9_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/cc338158aa28d723_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/4797a2fb8c7eac6b_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/411f64d386b7c4fe_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/29f6d5d8b27eb0c5_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/e99ae68f3e468751_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/502dafda143b5a74_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/d1783d0a170fdb8e_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/ac27a389f7bf6b67_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/40300177b5c0050d_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/4a8de756f1428237_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/e029cf1b0932611f_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/c69ff5c7e450ab22_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/bdfd0aa008d40005_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/40dd89dd968602d3_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/d04eb6456f31d2f7_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/ff4e7b79b6327627_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/b136f3771ffb9958_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/207bb56723cc5c3a_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/95035b9448e65cf2_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/73afbe7f6b7a496c_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/84b41c998e542199_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/d68d127f97a27059_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/037a0f2e4460355f_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/da584b3cb202e078_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/2d6c5245e29028b2_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/ca9b25d228896196_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/0e2708cf50936235_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/ab39283d30a39dd1_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/f721910d7c288b54_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/e1cd683779c2ea08_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/d1a8c9a323296d5b_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/4821c08320e603ae_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/01a2afcf422b3b4f_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/d41f0a4d475402fd_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/fcacc70d27c27f8b_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/f76d072b8c546a89_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/aff6b5b6e20cc2fa_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/8037b4d4c7774071_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/index
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/1e6d9e3ecd002bc9_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/3cfe648fbdd026a7_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/ab5d3ea4f0904068_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/9a785469b604c8af_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/472d78242cce2d22_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/503a7645e7f2d973_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/593a42d396c32634_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/b6ee82fb12843073_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/3561efa2281c73ed_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/318755c427839e86_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/3ef890a79ddb7e0c_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/d9772c6ee701ad39_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/5b3eff799688e021_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/30bb71565ae0cc27_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/54afe61c6fcf0e3f_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/8c9d078e6dbc501d_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/32a4f6fc17306385_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/0111bfd7286ca658_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/55ccaf33bd76fb46_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/65b80b5a552aaeaf_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/30fffcc41f7846bf_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/5377846224f95fc9_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/f9c93b74a177706b_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/94239554b50b59b5_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/14ced047ba93cdd3_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/2ceea49fe8c9e2fe_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/disk_cache/3ec7cdbc127eae35_0
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/version
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/prefs
/data/data/com.google.android.googlequicksearchbox/cache/cronet-async/prefs/local_prefs.json
/data/data/com.google.android.googlequicksearchbox/cache/si
/data/data/com.google.android.googlequicksearchbox/databases
/data/data/com.google.android.googlequicksearchbox/databases/icing-mdh.db-wal
/data/data/com.google.android.googlequicksearchbox/databases/google_app_measurement.db-shm
/data/data/com.google.android.googlequicksearchbox/databases/icing-mdh.db-shm
/data/data/com.google.android.googlequicksearchbox/databases/google_app_measurement_local.db
/data/data/com.google.android.googlequicksearchbox/databases/google_app_measurement_local.db-wipecheck
/data/data/com.google.android.googlequicksearchbox/databases/google_app_measurement.db-wal
/data/data/com.google.android.googlequicksearchbox/databases/google_app_measurement.db-wipecheck
/data/data/com.google.android.googlequicksearchbox/databases/google_app_measurement.db
/data/data/com.google.android.googlequicksearchbox/databases/icing-mdh.db-wipecheck
/data/data/com.google.android.googlequicksearchbox/databases/launcher.db
/data/data/com.google.android.googlequicksearchbox/databases/launcher.db-wal
/data/data/com.google.android.googlequicksearchbox/databases/icing-mdh.db
/data/data/com.google.android.googlequicksearchbox/databases/launcher.db-shm
/data/data/com.google.android.googlequicksearchbox/shared_prefs
/data/data/com.google.android.googlequicksearchbox/shared_prefs/AccountSwitcherDrawerPresenter.Prefs.xml
/data/data/com.google.android.googlequicksearchbox/shared_prefs/com.google.android.gms.appid.xml
/data/data/com.google.android.googlequicksearchbox/shared_prefs/consecutive_crash_stats.xml
/data/data/com.google.android.googlequicksearchbox/shared_prefs/uncaught_exception_handler_stats.xml
/data/data/com.google.android.googlequicksearchbox/shared_prefs/VoiceInteractionService.xml
/data/data/com.google.android.googlequicksearchbox/shared_prefs/interactor_process_uncaught_exception_handler_stats.xml
/data/data/com.google.android.googlequicksearchbox/shared_prefs/WebViewChromiumPrefs.xml
/data/data/com.google.android.googlequicksearchbox/shared_prefs/com.android.launcher3.managedusers.prefs.xml
/data/data/com.google.android.googlequicksearchbox/shared_prefs/com.android.launcher3.prefs.xml
/data/data/com.google.android.googlequicksearchbox/shared_prefs/GEL.GSAPrefs.xml
/data/data/com.google.android.googlequicksearchbox/shared_prefs/com.google.android.gms.measurement.prefs.xml
/data/data/com.google.android.googlequicksearchbox/shared_prefs/ThrottlingLogger.xml
/data/data/com.google.android.googlequicksearchbox/shared_prefs/default_process_uncaught_exception_handler_stats.xml
)F1L3N4M3";

#else

    static const char* kInodeValues = R"1N0D3(
66323@1117133
66323@1127134
66323@1137135
66323@1137136
66323@1137137
)1N0D3";

    const char* kPathNames = R"F1L3N4M3(
/
/data/
/data/data/
/data/data/file
/data/data/last_file
)F1L3N4M3";

#endif


    std::vector<std::string> inode_values = ParseLines(kInodeValues);
    std::vector<std::string> path_names = ParseLines(kPathNames);

    std::vector<Inode> inodes = ParseInodes(inode_values);

    return PathEntryTree{ PathEntry::Zip(inodes, path_names) };
}

class FakeSystemCall : public SystemCall {
 public:
  // stat(2)
  virtual int stat(const char *pathname, struct stat *statbuf) override {
    if (pathname == nullptr || statbuf == nullptr) {
      errno = EINVAL;
      return -1;
    }

    std::optional<PathEntry> maybe_path_entry = path_entries_.GetEntryFor(pathname);

    if (!maybe_path_entry) {
      errno = ENOENT;
      return -1;
    }

    memset(statbuf, 0, sizeof(*statbuf));

    Inode inode = maybe_path_entry->inode;
    statbuf->st_dev = makedev(static_cast<int>(inode.device_major),
                              static_cast<int>(inode.device_minor));
    statbuf->st_ino = static_cast<ino_t>(inode.inode);

    return 0;
  }

  static constexpr bool debug{false};

#define FS_LOG_DEBUG if (debug) LOG(DEBUG)

  // opendir(3)
  virtual DIR *opendir(const char *name) override {

    FS_LOG_DEBUG << "opendir(" << name << ")";

    std::string name_str{name};
    if (path_entries_.HasDirectory(name_str)) {
        CHECK(!state_.open_);

        std::vector<PathEntry> children = path_entries_.OpenDirectory(name_str);

        state_ = State::Open(name_str, std::move(children));

        FS_LOG_DEBUG << "opendir - success, state address: " << &state_;

        return get_state_as_dir();
    }

    FS_LOG_DEBUG << "opendir - no matching entry, scanned " << path_entries_.size();

    // TODO. errno.
    errno = EINVAL;
    return nullptr;
  }

  // readdir(3)
  virtual struct dirent *readdir(DIR *dirp) override {
    DCHECK(dirp != nullptr);
    // We could also errno=EBADF but this seems more apropro to test.

    State* state = dir_to_state(dirp);
    (void) state;
    DCHECK(state != nullptr);

    std::optional<PathEntry> path_entry_opt = state->ReadDir();

    if (!path_entry_opt) {
      FS_LOG_DEBUG << "readdir(" << &state << ") - no children left ";

      // No more children left. We have exhausted them all.
      return nullptr;
    }

    PathEntry path_entry = *path_entry_opt;

    FS_LOG_DEBUG << "readdir(" << &state << ") - called for " << path_entry.path;

    // TODO. impelment this.
    static struct dirent dir_ent{};

    // Clear it again.
    memset(&dir_ent, 0, sizeof(dir_ent));

    dir_ent.d_ino = path_entry.inode.inode;

    FS_LOG_DEBUG << "readdir(" << &state << ") - children check" << path_entry.path;

    // Is this a file (no children) or a directory (some children)?
    //
    // In reality some directories might be empty too, but lets not worry about it yet.
    std::vector<PathEntry> children = path_entries_.OpenDirectory(path_entry.path);

    if (children.empty()) {
      dir_ent.d_type = DT_REG;
    } else {
      dir_ent.d_type = DT_DIR;
    }

    // the d_name must be just the final name component of a path.
    // Do not include the full path.

    std::string_view name_view = path_entry.path.Name();
    DCHECK_LT(name_view.size(), sizeof(dir_ent.d_name));

    std::copy(name_view.begin(),
              name_view.end(),
              &dir_ent.d_name[0]);
    dir_ent.d_name[name_view.size()] = '\0';

    FS_LOG_DEBUG << "readdir(" << &state << ") - return , d_name=\"" << dir_ent.d_name << "\""
                 << ", d_type=" << (dir_ent.d_type == DT_REG ? "DT_REG" : "DT_DIR");

    return &dir_ent;
  }

  // closedir(3)
  virtual int closedir(DIR *dirp) override {
    CHECK(dirp != nullptr);
    State* state = dir_to_state(dirp);
    state->Close();

    return 0;
  }

  FakeSystemCall() {
    path_entries_ = CreateFakePathEntries();
  }

 private:
  struct State {
    std::string name_;
    bool open_{false};
    std::vector<PathEntry> children;

    static State Open(std::string name, std::vector<PathEntry> children) {
      return State{name, /*open*/true, std::move(children)};
    }

    std::optional<PathEntry> ReadDir() {
      if (children.empty()) {
        return {};
      }

      PathEntry last = children.back();
      children.pop_back();

      return { std::move(last) };
    }

    void Close() {
      CHECK(open_);
      open_ = false;
    }
  };

  DIR* get_state_as_dir() {
    return reinterpret_cast<DIR*>(reinterpret_cast<void*>(&state_));
  }

  State* dir_to_state(DIR* dirp) {
    return reinterpret_cast<State*>(reinterpret_cast<void*>(dirp));
  }

  State state_;

  PathEntryTree path_entries_;
};

class MockSystemCall : public SystemCall {
 public:
  INJECT(MockSystemCall()) {
    // Delegate calls to a fake (see the googlemock CookBook for more details).
    // https://github.com/google/googletest/blob/master/googlemock/docs/CookBook.md#delegating-calls-to-a-fake
    DelegateToFake();

    WorkAroundForNiceMock();
  }

  ~MockSystemCall() {
  }

  MOCK_METHOD2(stat, int(const char *, struct stat *));
  MOCK_METHOD1(opendir, DIR*(const char *));
  MOCK_METHOD1(readdir, struct dirent*(DIR*));
  MOCK_METHOD1(closedir, int(DIR*));

  // Delegates the default actions of the methods to a FakeSystemCall object.
  // This must be called *before* the custom ON_CALL() statements.
  void DelegateToFake() {
    ON_CALL(*this, stat(_,_))
        .WillByDefault(Invoke(&fake_, &FakeSystemCall::stat));
    ON_CALL(*this, opendir(_))
        .WillByDefault(Invoke(&fake_, &FakeSystemCall::opendir));
    ON_CALL(*this, readdir(_))
        .WillByDefault(Invoke(&fake_, &FakeSystemCall::readdir));
    ON_CALL(*this, closedir(_))
        .WillByDefault(Invoke(&fake_, &FakeSystemCall::closedir));
  }

  void WorkAroundForNiceMock();

 private:
  FakeSystemCall fake_;
};

// Don't print any warnings when methods are executed without EXPECT_CALL.
//using NiceMockSystemCall = NiceMock<MockSystemCall>;

// Can't use NiceMock<MockSystemCall> here, fails with this compilation error
//
// external/google-fruit/include/fruit/impl/injection_errors.h:107:3: error: static_assert failed due to requirement 'AlwaysFalse<NiceMock<MockSystemCall> >::value' "C::Inject is a signature, but does not return a C. Maybe the class C has no Inject typedef and inherited the base class' one? If that's not the case, make sure it returns just C, not C* or other types."
using NiceMockSystemCall = MockSystemCall;

void MockSystemCall::WorkAroundForNiceMock() {
  // Should be able to use NiceMock instead, but fruit is having problems compiling.
  EXPECT_CALL(*this, stat).Times(AtLeast(0));
  EXPECT_CALL(*this, opendir).Times(AtLeast(0));
  EXPECT_CALL(*this, readdir).Times(AtLeast(0));
  EXPECT_CALL(*this, closedir).Times(AtLeast(0));
}

fruit::Component<SearchDirectories, NiceMockSystemCall> getTestComponents() {
    return fruit::createComponent()
        .bind<SystemCall, NiceMockSystemCall>();
}

// TODO: there might be a helper or similar to do this instead.
template <typename T>
static std::vector<T> subscribe_drain(std::pair<rxcpp::observable<T>,
                                                std::unique_ptr<SearchDirectories::RxAnyConnectable>> pair) {
  rxcpp::observable<T>& obs = pair.first;
  std::unique_ptr<SearchDirectories::RxAnyConnectable>& connectable_ptr = pair.second;

  std::vector<T> vec;

  obs.subscribe([&vec](auto&& x) {
    vec.push_back(IORAP_FORWARD_LAMBDA(x));
  });

  CHECK(connectable_ptr != nullptr);

  // Execute above lambda, blocking until all values are drained.
  connectable_ptr->connect();

  return vec;
}

struct SearchDirectoriesParam {
  std::vector<std::string> root_directories;
  std::vector<Inode> search_inodes;
  std::vector<InodeResult> expected_results;
};

template <typename It>
std::ostream& iterator_to_stream(std::ostream& os, It begin, It end) {
  os << "{";
  while (begin != end) {
    os << *begin;
    os << ",";

    ++begin;
  }
  os << "}";

  return os;
}

template <typename T>
std::ostream& container_to_stream(std::ostream& os, T&& c) {
  return iterator_to_stream(os, c.begin(), c.end());
}

std::ostream& operator<<(std::ostream& os, const SearchDirectoriesParam& p) {
  os << "{";
  os << "root_directories:";
  container_to_stream(os, p.root_directories);
  os << ", ";
  os << "search_inodes:";
  container_to_stream(os, p.search_inodes) << ", ";
  os << "expected_results:";
  container_to_stream(os, p.expected_results);
  os << "}";
  return os;
}

struct SearchDirectoriesTest :
    public ::testing::TestWithParam<SearchDirectoriesParam> {

  static void SetUpTestCase() {
    ConfigureLogging();
  }

  virtual void SetUp() override {
    auto pair =
        search.FindFilenamesFromInodesPair(GetParam().root_directories,
                                           GetParam().search_inodes,
                                           SearchMode::kInProcessDirect);

    actual = subscribe_drain(std::move(pair));
    expected = GetParam().expected_results;
  }

  virtual void TearDown() override {
    // TODO.
  }

 protected:
  fruit::Injector<SearchDirectories, NiceMockSystemCall> injector{getTestComponents};

  SearchDirectories& search = injector.get<SearchDirectories&>();
  MockSystemCall& mock_syscall = injector.get<NiceMockSystemCall&>();

  std::vector<InodeResult> actual;
  std::vector<InodeResult> expected;
};

TEST_P(SearchDirectoriesTest, ElementsAreArrayMatcher) {
  EXPECT_THAT(actual, ElementsAreArray(expected));
}

auto MakeEmptyInodes(std::vector<std::string> root_dirs) {
  return SearchDirectoriesParam{root_dirs, /*inodes*/{}, /*actual*/{}};
}

// When are are 0 inodes to search for, the results will be empty.
INSTANTIATE_TEST_CASE_P(EmptyResults,
                        SearchDirectoriesTest,
                        ::testing::Values(
                            MakeEmptyInodes(/*root_dirs*/{}),
                            MakeEmptyInodes(/*root_dirs*/{""}),
                            MakeEmptyInodes(/*root_dirs*/{"/"}),
                            MakeEmptyInodes(/*root_dirs*/{"/abc"})
                        ));


auto MakeAllFailInodes(std::vector<std::string> root_dirs, std::vector<Inode> inodes) {
  std::vector<InodeResult> results;
  for (const Inode& inode : inodes) {
    results.push_back(InodeResult::makeFailure(inode, InodeResult::kCouldNotFindFilename));
  }

  return SearchDirectoriesParam{root_dirs, inodes, results};
}

// TODO: fixme

#if 1

// When none of the inodes can be found, all results will be failing results.
INSTANTIATE_TEST_CASE_P(AllResultsAreErrorCouldNotFindFilename,
                        SearchDirectoriesTest,
                        ::testing::Values(
                            // TODO: why is empty root dir failing?
                            // MakeAllFailInodes(/*root_dirs*/{}, {Inode{1,2,3}}),
                            MakeAllFailInodes(/*root_dirs*/{"/"}, {Inode{1,2,3}}),
                            MakeAllFailInodes(/*root_dirs*/{"/data"}, {Inode{1,2,3}}),
                            MakeAllFailInodes(/*root_dirs*/{"/data/data"}, {Inode{1,2,3}})
                        ));

auto MakeAllPassInodes(std::vector<std::string> root_dirs, std::vector<std::string> inodes, std::vector<std::string> paths) {
  std::vector<InodeResult> results;

  std::vector<Inode> inodes_actual;

  size_t i = 0;
  for (const std::string& inode_str : inodes) {
    Inode inode;
    std::string error_msg;

    CHECK(Inode::Parse(inode_str, &inode, &error_msg));

    inodes_actual.push_back(inode);

    std::string& path = paths[i];
    results.push_back(InodeResult::makeSuccess(inode, path));

    ++i;
  }

  return SearchDirectoriesParam{root_dirs, inodes_actual, results};
}

// Find all the inodes. Yay.
INSTANTIATE_TEST_CASE_P(AllResultsAreSuccess,
                        SearchDirectoriesTest,
                        ::testing::Values(
                            MakeAllPassInodes(/*root_dirs*/{"/"}, {"66323@1127133"}, {"/data"})
                        ));

#endif
