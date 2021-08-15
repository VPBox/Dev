//
// Copyright (C) 2014 The Android Open Source Project
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
//

#ifndef UPDATE_ENGINE_UPDATE_MANAGER_BOXED_VALUE_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_BOXED_VALUE_H_

#include <memory>
#include <string>

#include <base/macros.h>

namespace chromeos_update_manager {

// BoxedValue is a class to hold pointers of a given type that deletes them when
// the instance goes out of scope, as std::unique_ptr<T> does. The main
// difference with it is that the type T is not part of the class, i.e., this
// isn't a parametric class. The class has a parametric constructor that accepts
// a const T* which will define the type of the object passed on delete.
//
// It is safe to use this class in linked containers such as std::list and
// std::map but the object can't be copied. This means that you need to
// construct the BoxedValue in place using a container method like emplace()
// or move it with std::move().
//
//   list<BoxedValue> lst;
//   lst.emplace_back(new const int(42));
//   lst.emplace_back(new const string("Hello world!"));
//
//   map<int, BoxedValue> m;
//   m.emplace(123, std::move(BoxedValue(new const string("Hola mundo!"))));
//
//   auto it = m.find(42);
//   if (it != m.end())
//     cout << "m[42] points to " << it->second.value() << endl;
//   cout << "m[33] points to " << m[33].value() << endl;
//
// Since copy and assign are not allowed, you can't create a copy of the
// BoxedValue which means that you can only use a reference to it.
//

class BoxedValue {
 public:
  // Creates an empty BoxedValue. Since the pointer can't be assigned from other
  // BoxedValues or pointers, this is only useful in places where a default
  // constructor is required, such as std::map::operator[].
  BoxedValue() : value_(nullptr), deleter_(nullptr), printer_(nullptr) {}

  // Creates a BoxedValue for the passed pointer |value|. The BoxedValue keeps
  // the ownership of this pointer and can't be released.
  template <typename T>
  explicit BoxedValue(const T* value)
      : value_(static_cast<const void*>(value)),
        deleter_(ValueDeleter<T>),
        printer_(ValuePrinter<T>) {}

  // The move constructor takes ownership of the pointer since the semantics of
  // it allows to render the passed BoxedValue undefined. You need to use the
  // move constructor explicitly preventing it from accidental references,
  // like in:
  //   BoxedValue new_box(std::move(other_box));
  BoxedValue(BoxedValue&& other) noexcept
      : value_(other.value_),
        deleter_(other.deleter_),
        printer_(other.printer_) {
    other.value_ = nullptr;
    other.deleter_ = nullptr;
    other.printer_ = nullptr;
  }

  // Deletes the |value| passed on construction using the delete for the passed
  // type.
  ~BoxedValue() {
    if (deleter_)
      deleter_(value_);
  }

  const void* value() const { return value_; }

  std::string ToString() const {
    if (!printer_)
      return "(no printer)";
    if (!value_)
      return "(no value)";
    return printer_(value_);
  }

  // Static method to call the destructor of the right type.
  template <typename T>
  static void ValueDeleter(const void* value) {
    delete reinterpret_cast<const T*>(value);
  }

  // Static method to print a type. See boxed_value.cc for common
  // instantiations.
  template <typename T>
  static std::string ValuePrinter(const void* value);

 private:
  // A pointer to the cached value.
  const void* value_;

  // A function that calls delete for the right type of value_.
  void (*deleter_)(const void*);

  // A function that converts value_ to a string.
  std::string (*printer_)(const void*);

  DISALLOW_COPY_AND_ASSIGN(BoxedValue);
};

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_BOXED_VALUE_H_
