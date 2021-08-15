/*
 * Copyright (C) 2016, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "aidl.h"
#include "aidl_to_java.h"
#include "generate_java.h"
#include "options.h"
#include "type_java.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <unordered_set>
#include <utility>
#include <vector>

#include <android-base/macros.h>
#include <android-base/stringprintf.h>

using android::base::Join;
using android::base::StringPrintf;

using std::string;
using std::unique_ptr;
using std::vector;

namespace android {
namespace aidl {
namespace java {

// =================================================
class VariableFactory {
 public:
  using Variable = ::android::aidl::java::Variable;
  using Type = ::android::aidl::java::Type;

  explicit VariableFactory(const std::string& base) : base_(base), index_(0) {}
  Variable* Get(const Type* type) {
    Variable* v = new Variable(type->JavaType(), StringPrintf("%s%d", base_.c_str(), index_));
    vars_.push_back(v);
    index_++;
    return v;
  }

  Variable* Get(int index) { return vars_[index]; }

 private:
  std::vector<Variable*> vars_;
  std::string base_;
  int index_;

  DISALLOW_COPY_AND_ASSIGN(VariableFactory);
};

// =================================================
class StubClass : public Class {
 public:
  StubClass(const Type* type, const InterfaceType* interfaceType, JavaTypeNamespace* types,
            const Options& options);
  ~StubClass() override = default;

  Variable* transact_code;
  Variable* transact_data;
  Variable* transact_reply;
  Variable* transact_flags;
  SwitchStatement* transact_switch;
  StatementBlock* transact_statements;
  SwitchStatement* code_to_method_name_switch;

  // Where onTransact cases should be generated as separate methods.
  bool transact_outline;
  // Specific methods that should be outlined when transact_outline is true.
  std::unordered_set<const AidlMethod*> outline_methods;
  // Number of all methods.
  size_t all_method_count;

  // Finish generation. This will add a default case to the switch.
  void finish();

  Expression* get_transact_descriptor(const JavaTypeNamespace* types,
                                      const AidlMethod* method);

 private:
  void make_as_interface(const InterfaceType* interfaceType,
                         JavaTypeNamespace* types);

  Variable* transact_descriptor;
  const Options& options_;

  DISALLOW_COPY_AND_ASSIGN(StubClass);
};

StubClass::StubClass(const Type* type, const InterfaceType* interfaceType, JavaTypeNamespace* types,
                     const Options& options)
    : Class(), options_(options) {
  transact_descriptor = nullptr;
  transact_outline = false;
  all_method_count = 0;  // Will be set when outlining may be enabled.

  this->comment = "/** Local-side IPC implementation stub class. */";
  this->modifiers = PUBLIC | ABSTRACT | STATIC;
  this->what = Class::CLASS;
  this->type = type->JavaType();
  this->extends = types->BinderNativeType()->JavaType();
  this->interfaces.push_back(interfaceType->JavaType());

  // descriptor
  Field* descriptor = new Field(STATIC | FINAL | PRIVATE,
                                new Variable(types->StringType()->JavaType(), "DESCRIPTOR"));
  descriptor->value = "\"" + interfaceType->JavaType() + "\"";
  this->elements.push_back(descriptor);

  // ctor
  Method* ctor = new Method;
  ctor->modifiers = PUBLIC;
  ctor->comment =
      "/** Construct the stub at attach it to the "
      "interface. */";
  ctor->name = "Stub";
  ctor->statements = new StatementBlock;
  MethodCall* attach =
      new MethodCall(THIS_VALUE, "attachInterface", 2, THIS_VALUE,
                     new LiteralExpression("DESCRIPTOR"));
  ctor->statements->Add(attach);
  this->elements.push_back(ctor);

  // asInterface
  make_as_interface(interfaceType, types);

  // asBinder
  Method* asBinder = new Method;
  asBinder->modifiers = PUBLIC | OVERRIDE;
  asBinder->returnType = types->IBinderType()->JavaType();
  asBinder->name = "asBinder";
  asBinder->statements = new StatementBlock;
  asBinder->statements->Add(new ReturnStatement(THIS_VALUE));
  this->elements.push_back(asBinder);

  if (options_.GenTransactionNames()) {
    // getDefaultTransactionName
    Method* getDefaultTransactionName = new Method;
    getDefaultTransactionName->comment = "/** @hide */";
    getDefaultTransactionName->modifiers = PUBLIC | STATIC;
    getDefaultTransactionName->returnType = types->StringType()->JavaType();
    getDefaultTransactionName->name = "getDefaultTransactionName";
    Variable* code = new Variable(types->IntType()->JavaType(), "transactionCode");
    getDefaultTransactionName->parameters.push_back(code);
    getDefaultTransactionName->statements = new StatementBlock;
    this->code_to_method_name_switch = new SwitchStatement(code);
    getDefaultTransactionName->statements->Add(this->code_to_method_name_switch);
    this->elements.push_back(getDefaultTransactionName);

    // getTransactionName
    Method* getTransactionName = new Method;
    getTransactionName->comment = "/** @hide */";
    getTransactionName->modifiers = PUBLIC;
    getTransactionName->returnType = types->StringType()->JavaType();
    getTransactionName->name = "getTransactionName";
    Variable* code2 = new Variable(types->IntType()->JavaType(), "transactionCode");
    getTransactionName->parameters.push_back(code2);
    getTransactionName->statements = new StatementBlock;
    getTransactionName->statements->Add(
        new ReturnStatement(new MethodCall(THIS_VALUE, "getDefaultTransactionName", 1, code2)));
    this->elements.push_back(getTransactionName);
  }

  // onTransact
  this->transact_code = new Variable(types->IntType()->JavaType(), "code");
  this->transact_data = new Variable(types->ParcelType()->JavaType(), "data");
  this->transact_reply = new Variable(types->ParcelType()->JavaType(), "reply");
  this->transact_flags = new Variable(types->IntType()->JavaType(), "flags");
  Method* onTransact = new Method;
  onTransact->modifiers = PUBLIC | OVERRIDE;
  onTransact->returnType = types->BoolType()->JavaType();
  onTransact->name = "onTransact";
  onTransact->parameters.push_back(this->transact_code);
  onTransact->parameters.push_back(this->transact_data);
  onTransact->parameters.push_back(this->transact_reply);
  onTransact->parameters.push_back(this->transact_flags);
  onTransact->statements = new StatementBlock;
  transact_statements = onTransact->statements;
  onTransact->exceptions.push_back(types->RemoteExceptionType()->JavaType());
  this->elements.push_back(onTransact);
  this->transact_switch = new SwitchStatement(this->transact_code);
}

void StubClass::finish() {
  Case* default_case = new Case;

  MethodCall* superCall = new MethodCall(
        SUPER_VALUE, "onTransact", 4, this->transact_code, this->transact_data,
        this->transact_reply, this->transact_flags);
  default_case->statements->Add(new ReturnStatement(superCall));
  transact_switch->cases.push_back(default_case);

  transact_statements->Add(this->transact_switch);

  // getTransactionName
  if (options_.GenTransactionNames()) {
    // Some transaction codes are common, e.g. INTERFACE_TRANSACTION or DUMP_TRANSACTION.
    // Common transaction codes will not be resolved to a string by getTransactionName. The method
    // will return NULL in this case.
    Case* code_switch_default_case = new Case;
    code_switch_default_case->statements->Add(new ReturnStatement(NULL_VALUE));
    this->code_to_method_name_switch->cases.push_back(code_switch_default_case);
  }
}

// The the expression for the interface's descriptor to be used when
// generating code for the given method. Null is acceptable for method
// and stands for synthetic cases.
Expression* StubClass::get_transact_descriptor(const JavaTypeNamespace* types,
                                               const AidlMethod* method) {
  if (transact_outline) {
    if (method != nullptr) {
      // When outlining, each outlined method needs its own literal.
      if (outline_methods.count(method) != 0) {
        return new LiteralExpression("DESCRIPTOR");
      }
    } else {
      // Synthetic case. A small number is assumed. Use its own descriptor
      // if there are only synthetic cases.
      if (outline_methods.size() == all_method_count) {
        return new LiteralExpression("DESCRIPTOR");
      }
    }
  }

  // When not outlining, store the descriptor literal into a local variable, in
  // an effort to save const-string instructions in each switch case.
  if (transact_descriptor == nullptr) {
    transact_descriptor = new Variable(types->StringType()->JavaType(), "descriptor");
    transact_statements->Add(
        new VariableDeclaration(transact_descriptor,
                                new LiteralExpression("DESCRIPTOR")));
  }
  return transact_descriptor;
}

void StubClass::make_as_interface(const InterfaceType* interfaceType,
                                  JavaTypeNamespace* types) {
  Variable* obj = new Variable(types->IBinderType()->JavaType(), "obj");

  Method* m = new Method;
  m->comment = "/**\n * Cast an IBinder object into an ";
  m->comment += interfaceType->JavaType();
  m->comment += " interface,\n";
  m->comment += " * generating a proxy if needed.\n */";
  m->modifiers = PUBLIC | STATIC;
  m->returnType = interfaceType->JavaType();
  m->name = "asInterface";
  m->parameters.push_back(obj);
  m->statements = new StatementBlock;

  IfStatement* ifstatement = new IfStatement();
  ifstatement->expression = new Comparison(obj, "==", NULL_VALUE);
  ifstatement->statements = new StatementBlock;
  ifstatement->statements->Add(new ReturnStatement(NULL_VALUE));
  m->statements->Add(ifstatement);

  // IInterface iin = obj.queryLocalInterface(DESCRIPTOR)
  MethodCall* queryLocalInterface = new MethodCall(obj, "queryLocalInterface");
  queryLocalInterface->arguments.push_back(new LiteralExpression("DESCRIPTOR"));
  IInterfaceType iinType(types);
  Variable* iin = new Variable(iinType.JavaType(), "iin");
  VariableDeclaration* iinVd = new VariableDeclaration(iin, queryLocalInterface);
  m->statements->Add(iinVd);

  // Ensure the instance type of the local object is as expected.
  // One scenario where this is needed is if another package (with a
  // different class loader) runs in the same process as the service.

  // if (iin != null && iin instanceof <interfaceType>) return (<interfaceType>)
  // iin;
  Comparison* iinNotNull = new Comparison(iin, "!=", NULL_VALUE);
  Comparison* instOfCheck =
      new Comparison(iin, " instanceof ",
                     new LiteralExpression(interfaceType->JavaType()));
  IfStatement* instOfStatement = new IfStatement();
  instOfStatement->expression = new Comparison(iinNotNull, "&&", instOfCheck);
  instOfStatement->statements = new StatementBlock;
  instOfStatement->statements->Add(new ReturnStatement(new Cast(interfaceType->JavaType(), iin)));
  m->statements->Add(instOfStatement);

  NewExpression* ne = new NewExpression(interfaceType->GetProxy()->InstantiableName());
  ne->arguments.push_back(obj);
  m->statements->Add(new ReturnStatement(ne));

  this->elements.push_back(m);
}

// =================================================
class ProxyClass : public Class {
 public:
  ProxyClass(const JavaTypeNamespace* types, const Type* type, const InterfaceType* interfaceType,
             const Options& options);
  ~ProxyClass() override;

  Variable* mRemote;
};

ProxyClass::ProxyClass(const JavaTypeNamespace* types, const Type* type,
                       const InterfaceType* interfaceType, const Options& options)
    : Class() {
  this->modifiers = PRIVATE | STATIC;
  this->what = Class::CLASS;
  this->type = type->JavaType();
  this->interfaces.push_back(interfaceType->JavaType());

  // IBinder mRemote
  mRemote = new Variable(types->IBinderType()->JavaType(), "mRemote");
  this->elements.push_back(new Field(PRIVATE, mRemote));

  // Proxy()
  Variable* remote = new Variable(types->IBinderType()->JavaType(), "remote");
  Method* ctor = new Method;
  ctor->name = "Proxy";
  ctor->statements = new StatementBlock;
  ctor->parameters.push_back(remote);
  ctor->statements->Add(new Assignment(mRemote, remote));
  this->elements.push_back(ctor);

  if (options.Version() > 0) {
    std::ostringstream code;
    code << "private int mCachedVersion = -1;\n";
    this->elements.emplace_back(new LiteralClassElement(code.str()));
  }

  // IBinder asBinder()
  Method* asBinder = new Method;
  asBinder->modifiers = PUBLIC | OVERRIDE;
  asBinder->returnType = types->IBinderType()->JavaType();
  asBinder->name = "asBinder";
  asBinder->statements = new StatementBlock;
  asBinder->statements->Add(new ReturnStatement(mRemote));
  this->elements.push_back(asBinder);
}

ProxyClass::~ProxyClass() {}

// =================================================
static void generate_new_array(const Type* t, StatementBlock* addTo,
                               Variable* v, Variable* parcel,
                               JavaTypeNamespace* types) {
  Variable* len = new Variable(types->IntType()->JavaType(), v->name + "_length");
  addTo->Add(new VariableDeclaration(len, new MethodCall(parcel, "readInt")));
  IfStatement* lencheck = new IfStatement();
  lencheck->expression = new Comparison(len, "<", new LiteralExpression("0"));
  lencheck->statements->Add(new Assignment(v, NULL_VALUE));
  lencheck->elseif = new IfStatement();
  lencheck->elseif->statements->Add(new Assignment(v, new NewArrayExpression(t->JavaType(), len)));
  addTo->Add(lencheck);
}

static void generate_write_to_parcel(const AidlTypeSpecifier& type, StatementBlock* addTo,
                                     Variable* v, Variable* parcel, bool is_return_value,
                                     const AidlTypenames& typenames) {
  string code;
  CodeWriterPtr writer = CodeWriter::ForString(&code);
  CodeGeneratorContext context{
      .writer = *(writer.get()),
      .typenames = typenames,
      .type = type,
      .var = v->name,
      .parcel = parcel->name,
      .is_return_value = is_return_value,
  };
  WriteToParcelFor(context);
  writer->Close();
  addTo->Add(new LiteralStatement(code));
}

static void generate_int_constant(Class* interface, const std::string& name,
                                  const std::string& value) {
  auto code = StringPrintf("public static final int %s = %s;\n", name.c_str(), value.c_str());
  interface->elements.push_back(new LiteralClassElement(code));
}

static void generate_string_constant(Class* interface, const std::string& name,
                                     const std::string& value) {
  auto code = StringPrintf("public static final String %s = %s;\n", name.c_str(), value.c_str());
  interface->elements.push_back(new LiteralClassElement(code));
}

static std::unique_ptr<Method> generate_interface_method(
    const AidlMethod& method, JavaTypeNamespace* types) {
  std::unique_ptr<Method> decl(new Method);
  decl->comment = method.GetComments();
  decl->modifiers = PUBLIC;
  decl->returnType = method.GetType().GetLanguageType<Type>()->JavaType();
  decl->returnTypeDimension = method.GetType().IsArray() ? 1 : 0;
  decl->name = method.GetName();
  decl->annotations = generate_java_annotations(method.GetType());

  for (const std::unique_ptr<AidlArgument>& arg : method.GetArguments()) {
    decl->parameters.push_back(new Variable(arg->GetType().GetLanguageType<Type>()->JavaType(),
                                            arg->GetName(), arg->GetType().IsArray() ? 1 : 0));
  }

  decl->exceptions.push_back(types->RemoteExceptionType()->JavaType());

  return decl;
}

static void generate_stub_code(const AidlInterface& iface, const AidlMethod& method, bool oneway,
                               Variable* transact_data, Variable* transact_reply,
                               JavaTypeNamespace* types, StatementBlock* statements,
                               StubClass* stubClass, const Options& options) {
  TryStatement* tryStatement = nullptr;
  FinallyStatement* finallyStatement = nullptr;
  MethodCall* realCall = new MethodCall(THIS_VALUE, method.GetName());

  // interface token validation is the very first thing we do
  statements->Add(new MethodCall(transact_data,
                                 "enforceInterface", 1,
                                 stubClass->get_transact_descriptor(types,
                                                                    &method)));

  // args
  VariableFactory stubArgs("_arg");
  {
    // keep this across different args in order to create the classloader
    // at most once.
    bool is_classloader_created = false;
    for (const std::unique_ptr<AidlArgument>& arg : method.GetArguments()) {
      const Type* t = arg->GetType().GetLanguageType<Type>();
      Variable* v = stubArgs.Get(t);
      v->dimension = arg->GetType().IsArray() ? 1 : 0;

      statements->Add(new VariableDeclaration(v));

      if (arg->GetDirection() & AidlArgument::IN_DIR) {
        string code;
        CodeWriterPtr writer = CodeWriter::ForString(&code);
        CodeGeneratorContext context{.writer = *(writer.get()),
                                     .typenames = types->typenames_,
                                     .type = arg->GetType(),
                                     .var = v->name,
                                     .parcel = transact_data->name,
                                     .is_classloader_created = &is_classloader_created};
        CreateFromParcelFor(context);
        writer->Close();
        statements->Add(new LiteralStatement(code));
      } else {
        if (!arg->GetType().IsArray()) {
          statements->Add(new Assignment(v, new NewExpression(t->InstantiableName())));
        } else {
          generate_new_array(t, statements, v, transact_data, types);
        }
      }

      realCall->arguments.push_back(v);
    }
  }

  if (options.GenTraces()) {
    // try and finally, but only when generating trace code
    tryStatement = new TryStatement();
    finallyStatement = new FinallyStatement();

    tryStatement->statements->Add(new MethodCall(
        new LiteralExpression("android.os.Trace"), "traceBegin", 2,
        new LiteralExpression("android.os.Trace.TRACE_TAG_AIDL"),
        new StringLiteralExpression(iface.GetName() + "::"
            + method.GetName() + "::server")));

    finallyStatement->statements->Add(new MethodCall(
        new LiteralExpression("android.os.Trace"), "traceEnd", 1,
        new LiteralExpression("android.os.Trace.TRACE_TAG_AIDL")));
  }

  // the real call
  if (method.GetType().GetName() == "void") {
    if (options.GenTraces()) {
      statements->Add(tryStatement);
      tryStatement->statements->Add(realCall);
      statements->Add(finallyStatement);
    } else {
      statements->Add(realCall);
    }

    if (!oneway) {
      // report that there were no exceptions
      MethodCall* ex =
          new MethodCall(transact_reply, "writeNoException", 0);
      statements->Add(ex);
    }
  } else {
    Variable* _result = new Variable(method.GetType().GetLanguageType<Type>()->JavaType(),
                                     "_result", method.GetType().IsArray() ? 1 : 0);
    if (options.GenTraces()) {
      statements->Add(new VariableDeclaration(_result));
      statements->Add(tryStatement);
      tryStatement->statements->Add(new Assignment(_result, realCall));
      statements->Add(finallyStatement);
    } else {
      statements->Add(new VariableDeclaration(_result, realCall));
    }

    if (!oneway) {
      // report that there were no exceptions
      MethodCall* ex =
          new MethodCall(transact_reply, "writeNoException", 0);
      statements->Add(ex);
    }

    // marshall the return value
    generate_write_to_parcel(method.GetType(), statements, _result, transact_reply, true,
                             types->typenames_);
  }

  // out parameters
  int i = 0;
  for (const std::unique_ptr<AidlArgument>& arg : method.GetArguments()) {
    Variable* v = stubArgs.Get(i++);

    if (arg->GetDirection() & AidlArgument::OUT_DIR) {
      generate_write_to_parcel(arg->GetType(), statements, v, transact_reply, true,
                               types->typenames_);
    }
  }

  // return true
  statements->Add(new ReturnStatement(TRUE_VALUE));
}

static void generate_stub_case(const AidlInterface& iface, const AidlMethod& method,
                               const std::string& transactCodeName, bool oneway,
                               StubClass* stubClass, JavaTypeNamespace* types,
                               const Options& options) {
  Case* c = new Case(transactCodeName);

  generate_stub_code(iface, method, oneway, stubClass->transact_data, stubClass->transact_reply,
                     types, c->statements, stubClass, options);

  stubClass->transact_switch->cases.push_back(c);
}

static void generate_stub_case_outline(const AidlInterface& iface, const AidlMethod& method,
                                       const std::string& transactCodeName, bool oneway,
                                       StubClass* stubClass, JavaTypeNamespace* types,
                                       const Options& options) {
  std::string outline_name = "onTransact$" + method.GetName() + "$";
  // Generate an "outlined" method with the actual code.
  {
    Variable* transact_data = new Variable(types->ParcelType()->JavaType(), "data");
    Variable* transact_reply = new Variable(types->ParcelType()->JavaType(), "reply");
    Method* onTransact_case = new Method;
    onTransact_case->modifiers = PRIVATE;
    onTransact_case->returnType = types->BoolType()->JavaType();
    onTransact_case->name = outline_name;
    onTransact_case->parameters.push_back(transact_data);
    onTransact_case->parameters.push_back(transact_reply);
    onTransact_case->statements = new StatementBlock;
    onTransact_case->exceptions.push_back(types->RemoteExceptionType()->JavaType());
    stubClass->elements.push_back(onTransact_case);

    generate_stub_code(iface, method, oneway, transact_data, transact_reply, types,
                       onTransact_case->statements, stubClass, options);
  }

  // Generate the case dispatch.
  {
    Case* c = new Case(transactCodeName);

    MethodCall* helper_call = new MethodCall(THIS_VALUE,
                                             outline_name,
                                             2,
                                             stubClass->transact_data,
                                             stubClass->transact_reply);
    c->statements->Add(new ReturnStatement(helper_call));

    stubClass->transact_switch->cases.push_back(c);
  }
}

static std::unique_ptr<Method> generate_proxy_method(
    const AidlInterface& iface, const AidlMethod& method, const std::string& transactCodeName,
    bool oneway, ProxyClass* proxyClass, JavaTypeNamespace* types, const Options& options) {
  std::unique_ptr<Method> proxy(new Method);
  proxy->comment = method.GetComments();
  proxy->modifiers = PUBLIC | OVERRIDE;
  proxy->returnType = method.GetType().GetLanguageType<Type>()->JavaType();
  proxy->returnTypeDimension = method.GetType().IsArray() ? 1 : 0;
  proxy->name = method.GetName();
  proxy->statements = new StatementBlock;
  for (const std::unique_ptr<AidlArgument>& arg : method.GetArguments()) {
    proxy->parameters.push_back(new Variable(arg->GetType().GetLanguageType<Type>()->JavaType(),
                                             arg->GetName(), arg->GetType().IsArray() ? 1 : 0));
  }
  proxy->exceptions.push_back(types->RemoteExceptionType()->JavaType());

  // the parcels
  Variable* _data = new Variable(types->ParcelType()->JavaType(), "_data");
  proxy->statements->Add(
      new VariableDeclaration(_data, new MethodCall(types->ParcelType()->JavaType(), "obtain")));
  Variable* _reply = nullptr;
  if (!oneway) {
    _reply = new Variable(types->ParcelType()->JavaType(), "_reply");
    proxy->statements->Add(
        new VariableDeclaration(_reply, new MethodCall(types->ParcelType()->JavaType(), "obtain")));
  }

  // the return value
  Variable* _result = nullptr;
  if (method.GetType().GetName() != "void") {
    _result = new Variable(*proxy->returnType, "_result", method.GetType().IsArray() ? 1 : 0);
    proxy->statements->Add(new VariableDeclaration(_result));
  }

  // try and finally
  TryStatement* tryStatement = new TryStatement();
  proxy->statements->Add(tryStatement);
  FinallyStatement* finallyStatement = new FinallyStatement();
  proxy->statements->Add(finallyStatement);

  if (options.GenTraces()) {
    tryStatement->statements->Add(new MethodCall(
          new LiteralExpression("android.os.Trace"), "traceBegin", 2,
          new LiteralExpression("android.os.Trace.TRACE_TAG_AIDL"),
          new StringLiteralExpression(iface.GetName() + "::" +
                                      method.GetName() + "::client")));
  }

  // the interface identifier token: the DESCRIPTOR constant, marshalled as a
  // string
  tryStatement->statements->Add(new MethodCall(
      _data, "writeInterfaceToken", 1, new LiteralExpression("DESCRIPTOR")));

  // the parameters
  for (const std::unique_ptr<AidlArgument>& arg : method.GetArguments()) {
    const Type* t = arg->GetType().GetLanguageType<Type>();
    Variable* v = new Variable(t->JavaType(), arg->GetName(), arg->GetType().IsArray() ? 1 : 0);
    AidlArgument::Direction dir = arg->GetDirection();
    if (dir == AidlArgument::OUT_DIR && arg->GetType().IsArray()) {
      IfStatement* checklen = new IfStatement();
      checklen->expression = new Comparison(v, "==", NULL_VALUE);
      checklen->statements->Add(
          new MethodCall(_data, "writeInt", 1, new LiteralExpression("-1")));
      checklen->elseif = new IfStatement();
      checklen->elseif->statements->Add(
          new MethodCall(_data, "writeInt", 1, new FieldVariable(v, "length")));
      tryStatement->statements->Add(checklen);
    } else if (dir & AidlArgument::IN_DIR) {
      generate_write_to_parcel(arg->GetType(), tryStatement->statements, v, _data, false,
                               types->typenames_);
    } else {
      delete v;
    }
  }

  // the transact call
  unique_ptr<MethodCall> call(new MethodCall(
      proxyClass->mRemote, "transact", 4, new LiteralExpression("Stub." + transactCodeName), _data,
      _reply ? _reply : NULL_VALUE,
      new LiteralExpression(oneway ? "android.os.IBinder.FLAG_ONEWAY" : "0")));
  unique_ptr<Variable> _status(new Variable(types->BoolType()->JavaType(), "_status"));
  tryStatement->statements->Add(new VariableDeclaration(_status.release(), call.release()));

  // If the transaction returns false, which means UNKNOWN_TRANSACTION, fall
  // back to the local method in the default impl, if set before.
  vector<string> arg_names;
  for (const auto& arg : method.GetArguments()) {
    arg_names.emplace_back(arg->GetName());
  }
  bool has_return_type = method.GetType().GetName() != "void";
  tryStatement->statements->Add(new LiteralStatement(
      android::base::StringPrintf(has_return_type ? "if (!_status && getDefaultImpl() != null) {\n"
                                                    "  return getDefaultImpl().%s(%s);\n"
                                                    "}\n"
                                                  : "if (!_status && getDefaultImpl() != null) {\n"
                                                    "  getDefaultImpl().%s(%s);\n"
                                                    "  return;\n"
                                                    "}\n",
                                  method.GetName().c_str(), Join(arg_names, ", ").c_str())));

  // throw back exceptions.
  if (_reply) {
    MethodCall* ex = new MethodCall(_reply, "readException", 0);
    tryStatement->statements->Add(ex);
  }

  // returning and cleanup
  if (_reply != nullptr) {
    // keep this across return value and arguments in order to create the
    // classloader at most once.
    bool is_classloader_created = false;
    if (_result != nullptr) {
      string code;
      CodeWriterPtr writer = CodeWriter::ForString(&code);
      CodeGeneratorContext context{.writer = *(writer.get()),
                                   .typenames = types->typenames_,
                                   .type = method.GetType(),
                                   .var = _result->name,
                                   .parcel = _reply->name,
                                   .is_classloader_created = &is_classloader_created};
      CreateFromParcelFor(context);
      writer->Close();
      tryStatement->statements->Add(new LiteralStatement(code));
    }

    // the out/inout parameters
    for (const std::unique_ptr<AidlArgument>& arg : method.GetArguments()) {
      if (arg->GetDirection() & AidlArgument::OUT_DIR) {
        string code;
        CodeWriterPtr writer = CodeWriter::ForString(&code);
        CodeGeneratorContext context{.writer = *(writer.get()),
                                     .typenames = types->typenames_,
                                     .type = arg->GetType(),
                                     .var = arg->GetName(),
                                     .parcel = _reply->name,
                                     .is_classloader_created = &is_classloader_created};
        ReadFromParcelFor(context);
        writer->Close();
        tryStatement->statements->Add(new LiteralStatement(code));
      }
    }

    finallyStatement->statements->Add(new MethodCall(_reply, "recycle"));
  }
  finallyStatement->statements->Add(new MethodCall(_data, "recycle"));

  if (options.GenTraces()) {
    finallyStatement->statements->Add(new MethodCall(
        new LiteralExpression("android.os.Trace"), "traceEnd", 1,
        new LiteralExpression("android.os.Trace.TRACE_TAG_AIDL")));
  }

  if (_result != nullptr) {
    proxy->statements->Add(new ReturnStatement(_result));
  }

  return proxy;
}

static void generate_methods(const AidlInterface& iface, const AidlMethod& method, Class* interface,
                             StubClass* stubClass, ProxyClass* proxyClass, int index,
                             JavaTypeNamespace* types, const Options& options) {
  const bool oneway = method.IsOneway();

  // == the TRANSACT_ constant =============================================
  string transactCodeName = "TRANSACTION_";
  transactCodeName += method.GetName();

  Field* transactCode =
      new Field(STATIC | FINAL, new Variable(types->IntType()->JavaType(), transactCodeName));
  transactCode->value =
      StringPrintf("(android.os.IBinder.FIRST_CALL_TRANSACTION + %d)", index);
  stubClass->elements.push_back(transactCode);

  // getTransactionName
  if (options.GenTransactionNames()) {
    Case* c = new Case(transactCodeName);
    c->statements->Add(new ReturnStatement(new StringLiteralExpression(method.GetName())));
    stubClass->code_to_method_name_switch->cases.push_back(c);
  }

  // == the declaration in the interface ===================================
  ClassElement* decl;
  if (method.IsUserDefined()) {
    decl = generate_interface_method(method, types).release();
  } else {
    if (method.GetName() == kGetInterfaceVersion && options.Version() > 0) {
      std::ostringstream code;
      code << "public int " << kGetInterfaceVersion << "() "
           << "throws android.os.RemoteException;\n";
      decl = new LiteralClassElement(code.str());
    }
  }
  interface->elements.push_back(decl);

  // == the stub method ====================================================
  if (method.IsUserDefined()) {
    bool outline_stub =
        stubClass->transact_outline && stubClass->outline_methods.count(&method) != 0;
    if (outline_stub) {
      generate_stub_case_outline(iface, method, transactCodeName, oneway, stubClass, types,
                                 options);
    } else {
      generate_stub_case(iface, method, transactCodeName, oneway, stubClass, types, options);
    }
  } else {
    if (method.GetName() == kGetInterfaceVersion && options.Version() > 0) {
      Case* c = new Case(transactCodeName);
      std::ostringstream code;
      code << "data.enforceInterface(descriptor);\n"
           << "reply.writeNoException();\n"
           << "reply.writeInt(" << kGetInterfaceVersion << "());\n"
           << "return true;\n";
      c->statements->Add(new LiteralStatement(code.str()));
      stubClass->transact_switch->cases.push_back(c);
    }
  }

  // == the proxy method ===================================================
  ClassElement* proxy = nullptr;
  if (method.IsUserDefined()) {
    proxy =
        generate_proxy_method(iface, method, transactCodeName, oneway, proxyClass, types, options)
            .release();

  } else {
    if (method.GetName() == kGetInterfaceVersion && options.Version() > 0) {
      std::ostringstream code;
      code << "@Override\n"
           << "public int " << kGetInterfaceVersion << "()"
           << " throws "
           << "android.os.RemoteException {\n"
           << "  if (mCachedVersion == -1) {\n"
           << "    android.os.Parcel data = android.os.Parcel.obtain();\n"
           << "    android.os.Parcel reply = android.os.Parcel.obtain();\n"
           << "    try {\n"
           << "      data.writeInterfaceToken(DESCRIPTOR);\n"
           << "      mRemote.transact(Stub." << transactCodeName << ", "
           << "data, reply, 0);\n"
           << "      reply.readException();\n"
           << "      mCachedVersion = reply.readInt();\n"
           << "    } finally {\n"
           << "      reply.recycle();\n"
           << "      data.recycle();\n"
           << "    }\n"
           << "  }\n"
           << "  return mCachedVersion;\n"
           << "}\n";
      proxy = new LiteralClassElement(code.str());
    }
  }
  if (proxy != nullptr) {
    proxyClass->elements.push_back(proxy);
  }
}

static void generate_interface_descriptors(StubClass* stub, ProxyClass* proxy,
                                           const JavaTypeNamespace* types) {
  // the interface descriptor transaction handler
  Case* c = new Case("INTERFACE_TRANSACTION");
  c->statements->Add(new MethodCall(stub->transact_reply, "writeString", 1,
                                    stub->get_transact_descriptor(types,
                                                                  nullptr)));
  c->statements->Add(new ReturnStatement(TRUE_VALUE));
  stub->transact_switch->cases.push_back(c);

  // and the proxy-side method returning the descriptor directly
  Method* getDesc = new Method;
  getDesc->modifiers = PUBLIC;
  getDesc->returnType = types->StringType()->JavaType();
  getDesc->returnTypeDimension = 0;
  getDesc->name = "getInterfaceDescriptor";
  getDesc->statements = new StatementBlock;
  getDesc->statements->Add(
      new ReturnStatement(new LiteralExpression("DESCRIPTOR")));
  proxy->elements.push_back(getDesc);
}

// Check whether (some) methods in this interface should be "outlined," that
// is, have specific onTransact methods for certain cases. Set up StubClass
// metadata accordingly.
//
// Outlining will be enabled if the interface has more than outline_threshold
// methods. In that case, the methods are sorted by number of arguments
// (so that more "complex" methods come later), and the first non_outline_count
// number of methods not outlined (are kept in the onTransact() method).
//
// Requirements: non_outline_count <= outline_threshold.
static void compute_outline_methods(const AidlInterface* iface,
                                    StubClass* stub,
                                    size_t outline_threshold,
                                    size_t non_outline_count) {
  CHECK_LE(non_outline_count, outline_threshold);
  // We'll outline (create sub methods) if there are more than min_methods
  // cases.
  stub->transact_outline = iface->GetMethods().size() > outline_threshold;
  if (stub->transact_outline) {
    stub->all_method_count = iface->GetMethods().size();
    std::vector<const AidlMethod*> methods;
    methods.reserve(iface->GetMethods().size());
    for (const std::unique_ptr<AidlMethod>& ptr : iface->GetMethods()) {
      methods.push_back(ptr.get());
    }

    std::stable_sort(
        methods.begin(),
        methods.end(),
        [](const AidlMethod* m1, const AidlMethod* m2) {
          return m1->GetArguments().size() < m2->GetArguments().size();
        });

    stub->outline_methods.insert(methods.begin() + non_outline_count,
                                 methods.end());
  }
}

static unique_ptr<ClassElement> generate_default_impl_method(const AidlMethod& method) {
  unique_ptr<Method> default_method(new Method);
  default_method->comment = method.GetComments();
  default_method->modifiers = PUBLIC | OVERRIDE;
  default_method->returnType = method.GetType().GetLanguageType<Type>()->JavaType();
  default_method->returnTypeDimension = method.GetType().IsArray() ? 1 : 0;
  default_method->name = method.GetName();
  default_method->statements = new StatementBlock;
  for (const auto& arg : method.GetArguments()) {
    default_method->parameters.push_back(
        new Variable(arg->GetType().GetLanguageType<Type>()->JavaType(), arg->GetName(),
                     arg->GetType().IsArray() ? 1 : 0));
  }
  default_method->exceptions.push_back(method.GetType()
                                           .GetLanguageType<Type>()
                                           ->GetTypeNamespace()
                                           ->RemoteExceptionType()
                                           ->JavaType());

  if (method.GetType().GetName() != "void") {
    const string& defaultValue = DefaultJavaValueOf(method.GetType());
    default_method->statements->Add(
        new LiteralStatement(StringPrintf("return %s;\n", defaultValue.c_str())));
  }
  return default_method;
}

static unique_ptr<Class> generate_default_impl_class(const AidlInterface& iface,
                                                     const Options& options) {
  unique_ptr<Class> default_class(new Class);
  default_class->comment = "/** Default implementation for " + iface.GetName() + ". */";
  default_class->modifiers = PUBLIC | STATIC;
  default_class->what = Class::CLASS;
  default_class->type = iface.GetLanguageType<InterfaceType>()->GetDefaultImpl()->JavaType();
  default_class->interfaces.emplace_back(iface.GetLanguageType<InterfaceType>()->JavaType());

  for (const auto& m : iface.GetMethods()) {
    if (m->IsUserDefined()) {
      default_class->elements.emplace_back(generate_default_impl_method(*(m.get())).release());
    } else {
      if (m->GetName() == kGetInterfaceVersion && options.Version() > 0) {
        // This is called only when the remote side is not implementing this
        // method, which is impossible in normal case, because this method is
        // automatically declared in the interface class and not implementing
        // it in the remote side is causing compilation error. But if the remote
        // side somehow managed to not implement it, that's an error and we
        // report the case by returning -1 here.
        std::ostringstream code;
        code << "@Override\n"
             << "public int " << kGetInterfaceVersion << "() {\n"
             << "  return -1;\n"
             << "}\n";
        default_class->elements.emplace_back(new LiteralClassElement(code.str()));
      }
    }
  }

  default_class->elements.emplace_back(
      new LiteralClassElement("@Override\n"
                              "public android.os.IBinder asBinder() {\n"
                              "  return null;\n"
                              "}\n"));

  return default_class;
}

Class* generate_binder_interface_class(const AidlInterface* iface, JavaTypeNamespace* types,
                                       const Options& options) {
  const InterfaceType* interfaceType = iface->GetLanguageType<InterfaceType>();

  // the interface class
  Class* interface = new Class;
  interface->comment = iface->GetComments();
  interface->modifiers = PUBLIC;
  interface->what = Class::INTERFACE;
  interface->type = interfaceType->JavaType();
  interface->interfaces.push_back(types->IInterfaceType()->JavaType());
  interface->annotations = generate_java_annotations(*iface);

  if (options.Version()) {
    std::ostringstream code;
    code << "/**\n"
         << " * The version of this interface that the caller is built against.\n"
         << " * This might be different from what {@link #getInterfaceVersion()\n"
         << " * getInterfaceVersion} returns as that is the version of the interface\n"
         << " * that the remote object is implementing.\n"
         << " */\n"
         << "public static final int VERSION = " << options.Version() << ";\n";
    interface->elements.emplace_back(new LiteralClassElement(code.str()));
  }

  // the default impl class
  Class* default_impl = generate_default_impl_class(*iface, options).release();
  interface->elements.emplace_back(default_impl);

  // the stub inner class
  StubClass* stub =
      new StubClass(interfaceType->GetStub(), interfaceType, types, options);
  interface->elements.push_back(stub);

  compute_outline_methods(iface,
                          stub,
                          options.onTransact_outline_threshold_,
                          options.onTransact_non_outline_count_);

  // the proxy inner class
  ProxyClass* proxy = new ProxyClass(types, interfaceType->GetProxy(), interfaceType, options);
  stub->elements.push_back(proxy);

  // stub and proxy support for getInterfaceDescriptor()
  generate_interface_descriptors(stub, proxy, types);

  // all the declared constants of the interface
  for (const auto& constant : iface->GetConstantDeclarations()) {
    const AidlConstantValue& value = constant->GetValue();

    switch (value.GetType()) {
      case AidlConstantValue::Type::STRING: {
        generate_string_constant(interface, constant->GetName(),
                                 constant->ValueString(ConstantValueDecorator));
        break;
      }
      case AidlConstantValue::Type::INTEGRAL:
      case AidlConstantValue::Type::HEXIDECIMAL: {
        generate_int_constant(interface, constant->GetName(),
                              constant->ValueString(ConstantValueDecorator));
        break;
      }
      default: {
        LOG(FATAL) << "Unrecognized constant type: " << static_cast<int>(value.GetType());
      }
    }
  }

  // all the declared methods of the interface

  for (const auto& item : iface->GetMethods()) {
    generate_methods(*iface,
                     *item,
                     interface,
                     stub,
                     proxy,
                     item->GetId(),
                     types,
                     options);
  }

  // additional static methods for the default impl set/get to the
  // stub class. Can't add them to the interface as the generated java files
  // may be compiled with Java < 1.7 where static interface method isn't
  // supported.
  // TODO(b/111417145) make this conditional depending on the Java language
  // version requested
  const string i_name = interfaceType->JavaType();
  stub->elements.emplace_back(new LiteralClassElement(
      StringPrintf("public static boolean setDefaultImpl(%s impl) {\n"
                   "  if (Stub.Proxy.sDefaultImpl == null && impl != null) {\n"
                   "    Stub.Proxy.sDefaultImpl = impl;\n"
                   "    return true;\n"
                   "  }\n"
                   "  return false;\n"
                   "}\n",
                   i_name.c_str())));
  stub->elements.emplace_back(
      new LiteralClassElement(StringPrintf("public static %s getDefaultImpl() {\n"
                                           "  return Stub.Proxy.sDefaultImpl;\n"
                                           "}\n",
                                           i_name.c_str())));

  // the static field is defined in the proxy class, not in the interface class
  // because all fields in an interface class are by default final.
  proxy->elements.emplace_back(new LiteralClassElement(
      StringPrintf("public static %s sDefaultImpl;\n", i_name.c_str())));

  stub->finish();

  return interface;
}

}  // namespace java
}  // namespace android
}  // namespace aidl
