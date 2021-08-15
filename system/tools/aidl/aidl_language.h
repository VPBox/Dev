#pragma once

#include "aidl_typenames.h"
#include "code_writer.h"
#include "io_delegate.h"

#include <cassert>
#include <memory>
#include <string>
#include <vector>

#include <android-base/macros.h>
#include <android-base/strings.h>

struct yy_buffer_state;
typedef yy_buffer_state* YY_BUFFER_STATE;

using android::aidl::AidlTypenames;
using android::aidl::CodeWriter;
using std::shared_ptr;
using std::string;
using std::unique_ptr;
using std::vector;

class AidlNode;

namespace android {
namespace aidl {
namespace mappings {
std::string dump_location(const AidlNode& method);
}  // namespace mappings
}  // namespace aidl
}  // namespace android

class AidlToken {
 public:
  AidlToken(const std::string& text, const std::string& comments);

  const std::string& GetText() const { return text_; }
  const std::string& GetComments() const { return comments_; }

 private:
  std::string text_;
  std::string comments_;

  DISALLOW_COPY_AND_ASSIGN(AidlToken);
};

class AidlLocation {
 public:
  struct Point {
    unsigned int line;
    unsigned int column;
  };

  AidlLocation(const std::string& file, Point begin, Point end);

  friend std::ostream& operator<<(std::ostream& os, const AidlLocation& l);
  friend class AidlNode;

 private:
  const std::string file_;
  Point begin_;
  Point end_;
};

#define AIDL_LOCATION_HERE                   \
  AidlLocation {                             \
    __FILE__, {__LINE__, 0}, { __LINE__, 0 } \
  }

std::ostream& operator<<(std::ostream& os, const AidlLocation& l);

// Anything that is locatable in a .aidl file.
class AidlNode {
 public:
  AidlNode(const AidlLocation& location);

  AidlNode(const AidlNode&) = default;
  AidlNode(AidlNode&&) = default;
  virtual ~AidlNode() = default;

  // DO NOT ADD. This is intentionally omitted. Nothing should refer to the location
  // for a functional purpose. It is only for error messages.
  // NO const AidlLocation& GetLocation() const { return location_; } NO

  // To be able to print AidlLocation (nothing else should use this information)
  friend class AidlError;
  friend std::string android::aidl::mappings::dump_location(const AidlNode&);

 private:
  std::string PrintLocation() const;
  const AidlLocation location_;
};

// Generic point for printing any error in the AIDL compiler.
class AidlError {
 public:
  AidlError(bool fatal, const std::string& filename) : AidlError(fatal) { os_ << filename << ": "; }
  AidlError(bool fatal, const AidlLocation& location) : AidlError(fatal) {
    os_ << location << ": ";
  }
  AidlError(bool fatal, const AidlNode& node) : AidlError(fatal, node.location_) {}
  AidlError(bool fatal, const AidlNode* node) : AidlError(fatal, *node) {}

  template <typename T>
  AidlError(bool fatal, const std::unique_ptr<T>& node) : AidlError(fatal, *node) {}
  ~AidlError() {
    os_ << std::endl;
    if (fatal_) abort();
  }

  std::ostream& os_;

 private:
  AidlError(bool fatal);

  bool fatal_;

  DISALLOW_COPY_AND_ASSIGN(AidlError);
};

#define AIDL_ERROR(CONTEXT) ::AidlError(false /*fatal*/, (CONTEXT)).os_
#define AIDL_FATAL(CONTEXT) ::AidlError(true /*fatal*/, (CONTEXT)).os_
#define AIDL_FATAL_IF(CONDITION, CONTEXT) \
  if (CONDITION) AIDL_FATAL(CONTEXT) << "Bad internal state: " << #CONDITION << ": "

namespace android {
namespace aidl {

class ValidatableType;
class AidlTypenames;

}  // namespace aidl
}  // namespace android

class AidlAnnotation : public AidlNode {
 public:
  static AidlAnnotation* Parse(const AidlLocation& location, const string& name);

  AidlAnnotation(const AidlAnnotation&) = default;
  AidlAnnotation(AidlAnnotation&&) = default;
  virtual ~AidlAnnotation() = default;

  const string& GetName() const { return name_; }
  string ToString() const { return "@" + name_; }
  const string& GetComments() const { return comments_; }
  void SetComments(const string& comments) { comments_ = comments; }

 private:
  AidlAnnotation(const AidlLocation& location, const string& name);
  const string name_;
  string comments_;
};

static inline bool operator<(const AidlAnnotation& lhs, const AidlAnnotation& rhs) {
  return lhs.GetName() < rhs.GetName();
}
static inline bool operator==(const AidlAnnotation& lhs, const AidlAnnotation& rhs) {
  return lhs.GetName() == rhs.GetName();
}

class AidlAnnotatable : public AidlNode {
 public:
  AidlAnnotatable(const AidlLocation& location);

  AidlAnnotatable(const AidlAnnotatable&) = default;
  AidlAnnotatable(AidlAnnotatable&&) = default;
  virtual ~AidlAnnotatable() = default;

  void Annotate(vector<AidlAnnotation>&& annotations) { annotations_ = std::move(annotations); }
  bool IsNullable() const;
  bool IsUtf8InCpp() const;
  bool IsUnsupportedAppUsage() const;
  bool IsSystemApi() const;
  bool IsStableParcelable() const;
  std::string ToString() const;

  const vector<AidlAnnotation>& GetAnnotations() const { return annotations_; }

 private:
  vector<AidlAnnotation> annotations_;
};

class AidlQualifiedName;

// AidlTypeSpecifier represents a reference to either a built-in type,
// a defined type, or a variant (e.g., array of generic) of a type.
class AidlTypeSpecifier final : public AidlAnnotatable {
 public:
  AidlTypeSpecifier(const AidlLocation& location, const string& unresolved_name, bool is_array,
                    vector<unique_ptr<AidlTypeSpecifier>>* type_params, const string& comments);
  virtual ~AidlTypeSpecifier() = default;

  // Copy of this type which is not an array.
  AidlTypeSpecifier ArrayBase() const;

  // Returns the full-qualified name of the base type.
  // int -> int
  // int[] -> int
  // List<String> -> List
  // IFoo -> foo.bar.IFoo (if IFoo is in package foo.bar)
  const string& GetName() const {
    if (IsResolved()) {
      return fully_qualified_name_;
    } else {
      return GetUnresolvedName();
    }
  }

  // Returns string representation of this type specifier.
  // This is GetBaseTypeName() + array modifieir or generic type parameters
  string ToString() const;

  std::string Signature() const;

  const string& GetUnresolvedName() const { return unresolved_name_; }

  const string& GetComments() const { return comments_; }

  void SetComments(const string& comment) { comments_ = comment; }

  bool IsResolved() const { return fully_qualified_name_ != ""; }

  bool IsArray() const { return is_array_; }

  bool IsGeneric() const { return type_params_ != nullptr; }

  const vector<unique_ptr<AidlTypeSpecifier>>& GetTypeParameters() const { return *type_params_; }

  // Resolve the base type name to a fully-qualified name. Return false if the
  // resolution fails.
  bool Resolve(android::aidl::AidlTypenames& typenames);

  bool CheckValid(const AidlTypenames& typenames) const;

  void SetLanguageType(const android::aidl::ValidatableType* language_type) {
    language_type_ = language_type;
  }

  template<typename T>
  const T* GetLanguageType() const {
    return reinterpret_cast<const T*>(language_type_);
  }
 private:
  AidlTypeSpecifier(const AidlTypeSpecifier&) = default;

  const string unresolved_name_;
  string fully_qualified_name_;
  bool is_array_;
  const shared_ptr<vector<unique_ptr<AidlTypeSpecifier>>> type_params_;
  string comments_;
  const android::aidl::ValidatableType* language_type_ = nullptr;
};

// Transforms a value string into a language specific form. Raw value as produced by
// AidlConstantValue.
using ConstantValueDecorator =
    std::function<std::string(const AidlTypeSpecifier& type, const std::string& raw_value)>;

// Returns the universal value unaltered.
std::string AidlConstantValueDecorator(const AidlTypeSpecifier& type, const std::string& raw_value);

class AidlConstantValue;
class AidlVariableDeclaration : public AidlNode {
 public:
  AidlVariableDeclaration(const AidlLocation& location, AidlTypeSpecifier* type,
                          const std::string& name);
  AidlVariableDeclaration(const AidlLocation& location, AidlTypeSpecifier* type,
                          const std::string& name, AidlConstantValue* default_value);
  virtual ~AidlVariableDeclaration() = default;

  std::string GetName() const { return name_; }
  const AidlTypeSpecifier& GetType() const { return *type_; }
  const AidlConstantValue* GetDefaultValue() const { return default_value_.get(); }

  AidlTypeSpecifier* GetMutableType() { return type_.get(); }

  bool CheckValid(const AidlTypenames& typenames) const;
  std::string ToString() const;
  std::string Signature() const;

  std::string ValueString(const ConstantValueDecorator& decorator) const;

 private:
  std::unique_ptr<AidlTypeSpecifier> type_;
  std::string name_;
  std::unique_ptr<AidlConstantValue> default_value_;

  DISALLOW_COPY_AND_ASSIGN(AidlVariableDeclaration);
};

class AidlArgument : public AidlVariableDeclaration {
 public:
  enum Direction { IN_DIR = 1, OUT_DIR = 2, INOUT_DIR = 3 };

  AidlArgument(const AidlLocation& location, AidlArgument::Direction direction,
               AidlTypeSpecifier* type, const std::string& name);
  AidlArgument(const AidlLocation& location, AidlTypeSpecifier* type, const std::string& name);
  virtual ~AidlArgument() = default;

  Direction GetDirection() const { return direction_; }
  bool IsOut() const { return direction_ & OUT_DIR; }
  bool IsIn() const { return direction_ & IN_DIR; }
  bool DirectionWasSpecified() const { return direction_specified_; }
  string GetDirectionSpecifier() const;

  std::string ToString() const;
  std::string Signature() const;

 private:
  Direction direction_;
  bool direction_specified_;

  DISALLOW_COPY_AND_ASSIGN(AidlArgument);
};

class AidlMethod;
class AidlConstantDeclaration;
class AidlMember : public AidlNode {
 public:
  AidlMember(const AidlLocation& location);
  virtual ~AidlMember() = default;

  virtual AidlMethod* AsMethod() { return nullptr; }
  virtual AidlConstantDeclaration* AsConstantDeclaration() { return nullptr; }

 private:
  DISALLOW_COPY_AND_ASSIGN(AidlMember);
};

class AidlConstantValue : public AidlNode {
 public:
  enum class Type { ERROR, ARRAY, BOOLEAN, CHARACTER, FLOATING, HEXIDECIMAL, INTEGRAL, STRING };

  virtual ~AidlConstantValue() = default;

  static AidlConstantValue* Boolean(const AidlLocation& location, bool value);
  static AidlConstantValue* Character(const AidlLocation& location, char value);
  // example: "0x4f"
  static AidlConstantValue* Floating(const AidlLocation& location, const std::string& value);
  static AidlConstantValue* Hex(const AidlLocation& location, const std::string& value);
  // example: 123, -5498, maybe any size
  static AidlConstantValue* Integral(const AidlLocation& location, const std::string& value);
  static AidlConstantValue* Array(const AidlLocation& location,
                                  std::vector<std::unique_ptr<AidlConstantValue>>* values);
  // example: "\"asdf\""
  static AidlConstantValue* String(const AidlLocation& location, const std::string& value);

  Type GetType() const { return type_; }

  bool CheckValid() const;

  // Raw value of type (currently valid in C++ and Java). Empty string on error.
  string As(const AidlTypeSpecifier& type, const ConstantValueDecorator& decorator) const;

 private:
  AidlConstantValue(const AidlLocation& location, Type type, const std::string& checked_value);
  AidlConstantValue(const AidlLocation& location, Type type,
                    std::vector<std::unique_ptr<AidlConstantValue>>* values);
  static string ToString(Type type);

  const Type type_ = Type::ERROR;
  const std::vector<std::unique_ptr<AidlConstantValue>> values_;  // if type_ == ARRAY
  const std::string value_;                                       // otherwise

  DISALLOW_COPY_AND_ASSIGN(AidlConstantValue);
};

class AidlConstantDeclaration : public AidlMember {
 public:
  AidlConstantDeclaration(const AidlLocation& location, AidlTypeSpecifier* specifier,
                          const std::string& name, AidlConstantValue* value);
  virtual ~AidlConstantDeclaration() = default;

  const AidlTypeSpecifier& GetType() const { return *type_; }
  AidlTypeSpecifier* GetMutableType() { return type_.get(); }
  const std::string& GetName() const { return name_; }
  const AidlConstantValue& GetValue() const { return *value_; }
  bool CheckValid(const AidlTypenames& typenames) const;

  std::string ToString() const;
  std::string Signature() const;
  string ValueString(const ConstantValueDecorator& decorator) const {
    return GetValue().As(GetType(), decorator);
  }

  AidlConstantDeclaration* AsConstantDeclaration() override { return this; }

 private:
  const unique_ptr<AidlTypeSpecifier> type_;
  const std::string name_;
  const unique_ptr<AidlConstantValue> value_;

  DISALLOW_COPY_AND_ASSIGN(AidlConstantDeclaration);
};

class AidlMethod : public AidlMember {
 public:
  AidlMethod(const AidlLocation& location, bool oneway, AidlTypeSpecifier* type,
             const std::string& name, std::vector<std::unique_ptr<AidlArgument>>* args,
             const std::string& comments);
  AidlMethod(const AidlLocation& location, bool oneway, AidlTypeSpecifier* type,
             const std::string& name, std::vector<std::unique_ptr<AidlArgument>>* args,
             const std::string& comments, int id, bool is_user_defined = true);
  virtual ~AidlMethod() = default;

  AidlMethod* AsMethod() override { return this; }

  const std::string& GetComments() const { return comments_; }
  const AidlTypeSpecifier& GetType() const { return *type_; }
  AidlTypeSpecifier* GetMutableType() { return type_.get(); }

  // set if this method is part of an interface that is marked oneway
  void ApplyInterfaceOneway(bool oneway) { oneway_ = oneway_ || oneway; }
  bool IsOneway() const { return oneway_; }

  const std::string& GetName() const { return name_; }
  bool HasId() const { return has_id_; }
  int GetId() const { return id_; }
  void SetId(unsigned id) { id_ = id; }

  bool IsUserDefined() const { return is_user_defined_; }

  const std::vector<std::unique_ptr<AidlArgument>>& GetArguments() const {
    return arguments_;
  }
  // An inout parameter will appear in both GetInArguments()
  // and GetOutArguments().  AidlMethod retains ownership of the argument
  // pointers returned in this way.
  const std::vector<const AidlArgument*>& GetInArguments() const {
    return in_arguments_;
  }
  const std::vector<const AidlArgument*>& GetOutArguments() const {
    return out_arguments_;
  }

  // name + type parameter types
  // i.e, foo(int, String)
  std::string Signature() const;

  // return type + name + type parameter types + annotations
  // i.e, boolean foo(int, @Nullable String)
  std::string ToString() const;

 private:
  bool oneway_;
  std::string comments_;
  std::unique_ptr<AidlTypeSpecifier> type_;
  std::string name_;
  const std::vector<std::unique_ptr<AidlArgument>> arguments_;
  std::vector<const AidlArgument*> in_arguments_;
  std::vector<const AidlArgument*> out_arguments_;
  bool has_id_;
  int id_;
  bool is_user_defined_ = true;

  DISALLOW_COPY_AND_ASSIGN(AidlMethod);
};

class AidlDefinedType;
class AidlInterface;
class AidlParcelable;
class AidlStructuredParcelable;

class AidlQualifiedName : public AidlNode {
 public:
  AidlQualifiedName(const AidlLocation& location, const std::string& term,
                    const std::string& comments);
  virtual ~AidlQualifiedName() = default;

  const std::vector<std::string>& GetTerms() const { return terms_; }
  const std::string& GetComments() const { return comments_; }
  std::string GetDotName() const { return android::base::Join(terms_, '.'); }
  std::string GetColonName() const { return android::base::Join(terms_, "::"); }

  void AddTerm(const std::string& term);

 private:
  std::vector<std::string> terms_;
  std::string comments_;

  DISALLOW_COPY_AND_ASSIGN(AidlQualifiedName);
};

class AidlInterface;
class AidlParcelable;
class AidlStructuredParcelable;
// AidlDefinedType represents either an interface or a parcelable that is
// defined in the source file.
class AidlDefinedType : public AidlAnnotatable {
 public:
  AidlDefinedType(const AidlLocation& location, const std::string& name,
                  const std::string& comments, const std::vector<std::string>& package);
  virtual ~AidlDefinedType() = default;

  const std::string& GetName() const { return name_; };
  const std::string& GetComments() const { return comments_; }
  void SetComments(const std::string comments) { comments_ = comments; }

  /* dot joined package, example: "android.package.foo" */
  std::string GetPackage() const;
  /* dot joined package and name, example: "android.package.foo.IBar" */
  std::string GetCanonicalName() const;
  const std::vector<std::string>& GetSplitPackage() const { return package_; }

  virtual std::string GetPreprocessDeclarationName() const = 0;

  virtual const AidlStructuredParcelable* AsStructuredParcelable() const { return nullptr; }
  virtual const AidlParcelable* AsParcelable() const { return nullptr; }
  virtual const AidlInterface* AsInterface() const { return nullptr; }
  virtual bool CheckValid(const AidlTypenames&) const { return true; }

  AidlStructuredParcelable* AsStructuredParcelable() {
    return const_cast<AidlStructuredParcelable*>(
        const_cast<const AidlDefinedType*>(this)->AsStructuredParcelable());
  }
  AidlParcelable* AsParcelable() {
    return const_cast<AidlParcelable*>(const_cast<const AidlDefinedType*>(this)->AsParcelable());
  }
  AidlInterface* AsInterface() {
    return const_cast<AidlInterface*>(const_cast<const AidlDefinedType*>(this)->AsInterface());
  }

  const AidlParcelable* AsUnstructuredParcelable() const {
    if (this->AsStructuredParcelable() != nullptr) return nullptr;
    return this->AsParcelable();
  }
  AidlParcelable* AsUnstructuredParcelable() {
    return const_cast<AidlParcelable*>(
        const_cast<const AidlDefinedType*>(this)->AsUnstructuredParcelable());
  }

  void SetLanguageType(const android::aidl::ValidatableType* language_type) {
    language_type_ = language_type;
  }

  template <typename T>
  const T* GetLanguageType() const {
    return reinterpret_cast<const T*>(language_type_);
  }

  virtual void Write(CodeWriter* writer) const = 0;

 private:
  std::string name_;
  std::string comments_;
  const android::aidl::ValidatableType* language_type_ = nullptr;
  const std::vector<std::string> package_;

  DISALLOW_COPY_AND_ASSIGN(AidlDefinedType);
};

class AidlParcelable : public AidlDefinedType {
 public:
  AidlParcelable(const AidlLocation& location, AidlQualifiedName* name,
                 const std::vector<std::string>& package, const std::string& comments,
                 const std::string& cpp_header = "");
  virtual ~AidlParcelable() = default;

  // C++ uses "::" instead of "." to refer to a inner class.
  std::string GetCppName() const { return name_->GetColonName(); }
  std::string GetCppHeader() const { return cpp_header_; }

  bool CheckValid(const AidlTypenames& typenames) const override;

  const AidlParcelable* AsParcelable() const override { return this; }
  std::string GetPreprocessDeclarationName() const override { return "parcelable"; }

  void Write(CodeWriter* writer) const override;

 private:
  std::unique_ptr<AidlQualifiedName> name_;
  std::string cpp_header_;

  DISALLOW_COPY_AND_ASSIGN(AidlParcelable);
};

class AidlStructuredParcelable : public AidlParcelable {
 public:
  AidlStructuredParcelable(const AidlLocation& location, AidlQualifiedName* name,
                           const std::vector<std::string>& package, const std::string& comments,
                           std::vector<std::unique_ptr<AidlVariableDeclaration>>* variables);

  const std::vector<std::unique_ptr<AidlVariableDeclaration>>& GetFields() const {
    return variables_;
  }

  const AidlStructuredParcelable* AsStructuredParcelable() const override { return this; }
  std::string GetPreprocessDeclarationName() const override { return "structured_parcelable"; }

  void Write(CodeWriter* writer) const override;

  bool CheckValid(const AidlTypenames& typenames) const override;

 private:
  const std::vector<std::unique_ptr<AidlVariableDeclaration>> variables_;

  DISALLOW_COPY_AND_ASSIGN(AidlStructuredParcelable);
};

class AidlInterface final : public AidlDefinedType {
 public:
  AidlInterface(const AidlLocation& location, const std::string& name, const std::string& comments,
                bool oneway_, std::vector<std::unique_ptr<AidlMember>>* members,
                const std::vector<std::string>& package);
  virtual ~AidlInterface() = default;

  const std::vector<std::unique_ptr<AidlMethod>>& GetMethods() const
      { return methods_; }
  std::vector<std::unique_ptr<AidlMethod>>& GetMutableMethods() { return methods_; }
  const std::vector<std::unique_ptr<AidlConstantDeclaration>>& GetConstantDeclarations() const {
    return constants_;
  }

  const AidlInterface* AsInterface() const override { return this; }
  std::string GetPreprocessDeclarationName() const override { return "interface"; }

  void Write(CodeWriter* writer) const override;

  bool CheckValid(const AidlTypenames& typenames) const override;

 private:
  std::vector<std::unique_ptr<AidlMethod>> methods_;
  std::vector<std::unique_ptr<AidlConstantDeclaration>> constants_;

  DISALLOW_COPY_AND_ASSIGN(AidlInterface);
};

class AidlImport : public AidlNode {
 public:
  AidlImport(const AidlLocation& location, const std::string& needed_class);
  virtual ~AidlImport() = default;

  const std::string& GetFilename() const { return filename_; }
  const std::string& GetNeededClass() const { return needed_class_; }

 private:
  std::string filename_;
  std::string needed_class_;

  DISALLOW_COPY_AND_ASSIGN(AidlImport);
};

class Parser {
 public:
  ~Parser();

  // Parse contents of file |filename|. Should only be called once.
  static std::unique_ptr<Parser> Parse(const std::string& filename,
                                       const android::aidl::IoDelegate& io_delegate,
                                       AidlTypenames& typenames);

  void AddError() { error_++; }
  bool HasError() { return error_ != 0; }

  const std::string& FileName() const { return filename_; }
  void* Scanner() const { return scanner_; }

  void AddImport(AidlImport* import);
  const std::vector<std::unique_ptr<AidlImport>>& GetImports() {
    return imports_;
  }
  void ReleaseImports(std::vector<std::unique_ptr<AidlImport>>* ret) {
    *ret = std::move(imports_);
    imports_.clear();
  }

  void SetPackage(unique_ptr<AidlQualifiedName> name) { package_ = std::move(name); }
  std::vector<std::string> Package() const;

  void DeferResolution(AidlTypeSpecifier* typespec) {
    unresolved_typespecs_.emplace_back(typespec);
  }

  const vector<AidlTypeSpecifier*>& GetUnresolvedTypespecs() const { return unresolved_typespecs_; }

  bool Resolve();

  void AddDefinedType(unique_ptr<AidlDefinedType> type) {
    // Parser does NOT own AidlDefinedType, it just has references to the types
    // that it encountered while parsing the input file.
    defined_types_.emplace_back(type.get());

    // AidlDefinedType IS owned by AidlTypenames
    if (!typenames_.AddDefinedType(std::move(type))) {
      AddError();
    }
  }

  vector<AidlDefinedType*>& GetDefinedTypes() { return defined_types_; }

 private:
  explicit Parser(const std::string& filename, std::string& raw_buffer,
                  android::aidl::AidlTypenames& typenames);

  std::string filename_;
  std::unique_ptr<AidlQualifiedName> package_;
  AidlTypenames& typenames_;

  void* scanner_ = nullptr;
  YY_BUFFER_STATE buffer_;
  int error_ = 0;

  std::vector<std::unique_ptr<AidlImport>> imports_;
  vector<AidlDefinedType*> defined_types_;
  vector<AidlTypeSpecifier*> unresolved_typespecs_;

  DISALLOW_COPY_AND_ASSIGN(Parser);
};
