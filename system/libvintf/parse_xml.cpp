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

// Convert objects from and to xml.

#define LOG_TAG "libvintf"
#include <android-base/logging.h>

#include "parse_xml.h"

#include <type_traits>

#include <tinyxml2.h>

#include "Regex.h"
#include "constants.h"
#include "parse_string.h"

namespace android {
namespace vintf {

// --------------- tinyxml2 details

using NodeType = tinyxml2::XMLElement;
using DocType = tinyxml2::XMLDocument;

// caller is responsible for deleteDocument() call
inline DocType *createDocument() {
    return new tinyxml2::XMLDocument();
}

// caller is responsible for deleteDocument() call
inline DocType *createDocument(const std::string &xml) {
    DocType *doc = new tinyxml2::XMLDocument();
    if (doc->Parse(xml.c_str()) == tinyxml2::XML_SUCCESS) {
        return doc;
    }
    delete doc;
    return nullptr;
}

inline void deleteDocument(DocType *d) {
    delete d;
}

inline std::string printDocument(DocType *d) {
    tinyxml2::XMLPrinter p;
    d->Print(&p);
    return std::string{p.CStr()};
}

inline NodeType *createNode(const std::string &name, DocType *d) {
    return d->NewElement(name.c_str());
}

inline void appendChild(NodeType *parent, NodeType *child) {
    parent->InsertEndChild(child);
}

inline void appendChild(DocType *parent, NodeType *child) {
    parent->InsertEndChild(child);
}

inline void appendStrAttr(NodeType *e, const std::string &attrName, const std::string &attr) {
    e->SetAttribute(attrName.c_str(), attr.c_str());
}

// text -> text
inline void appendText(NodeType *parent, const std::string &text, DocType *d) {
    parent->InsertEndChild(d->NewText(text.c_str()));
}

inline std::string nameOf(NodeType *root) {
    return root->Name() == NULL ? "" : root->Name();
}

inline std::string getText(NodeType *root) {
    return root->GetText() == NULL ? "" : root->GetText();
}

inline NodeType *getChild(NodeType *parent, const std::string &name) {
    return parent->FirstChildElement(name.c_str());
}

inline NodeType *getRootChild(DocType *parent) {
    return parent->FirstChildElement();
}

inline std::vector<NodeType *> getChildren(NodeType *parent, const std::string &name) {
    std::vector<NodeType *> v;
    for (NodeType *child = parent->FirstChildElement(name.c_str());
         child != nullptr;
         child = child->NextSiblingElement(name.c_str())) {
        v.push_back(child);
    }
    return v;
}

inline bool getAttr(NodeType *root, const std::string &attrName, std::string *s) {
    const char *c = root->Attribute(attrName.c_str());
    if (c == NULL)
        return false;
    *s = c;
    return true;
}

// --------------- tinyxml2 details end.

// Helper functions for XmlConverter
static bool parse(const std::string &attrText, bool *attr) {
    if (attrText == "true" || attrText == "1") {
        *attr = true;
        return true;
    }
    if (attrText == "false" || attrText == "0") {
        *attr = false;
        return true;
    }
    return false;
}

// ---------------------- XmlNodeConverter definitions

template<typename Object>
struct XmlNodeConverter : public XmlConverter<Object> {
    XmlNodeConverter() {}
    virtual ~XmlNodeConverter() {}

    // sub-types should implement these.
    virtual void mutateNode(const Object &o, NodeType *n, DocType *d) const = 0;
    virtual void mutateNode(const Object& o, NodeType* n, DocType* d, SerializeFlags::Type) const {
        mutateNode(o, n, d);
    }
    virtual bool buildObject(Object* o, NodeType* n, std::string* error) const = 0;
    virtual std::string elementName() const = 0;

    // convenience methods for user
    inline const std::string& lastError() const override { return mLastError; }
    inline NodeType* serialize(const Object& o, DocType* d,
                               SerializeFlags::Type flags = SerializeFlags::EVERYTHING) const {
        NodeType *root = createNode(this->elementName(), d);
        this->mutateNode(o, root, d, flags);
        return root;
    }
    inline std::string serialize(const Object& o, SerializeFlags::Type flags) const override {
        DocType *doc = createDocument();
        appendChild(doc, serialize(o, doc, flags));
        std::string s = printDocument(doc);
        deleteDocument(doc);
        return s;
    }
    inline bool deserialize(Object* object, NodeType* root) {
        bool ret = deserialize(object, root, &mLastError);
        return ret;
    }
    inline bool deserialize(Object* o, const std::string& xml) override {
        bool ret = (*this)(o, xml, &mLastError);
        return ret;
    }
    inline bool deserialize(Object* object, NodeType* root, std::string* error) const {
        if (nameOf(root) != this->elementName()) {
            return false;
        }
        return this->buildObject(object, root, error);
    }
    inline bool operator()(Object* o, const std::string& xml, std::string* error) const override {
        std::string errorBuffer;
        if (error == nullptr) error = &errorBuffer;

        auto doc = createDocument(xml);
        if (doc == nullptr) {
            *error = "Not a valid XML";
            return false;
        }
        bool ret = deserialize(o, getRootChild(doc), error);
        deleteDocument(doc);
        return ret;
    }
    inline NodeType *operator()(const Object &o, DocType *d) const {
        return serialize(o, d);
    }
    inline std::string operator()(const Object& o, SerializeFlags::Type flags) const override {
        return serialize(o, flags);
    }
    inline bool operator()(Object* o, NodeType* node) { return deserialize(o, node); }
    inline bool operator()(Object* o, const std::string& xml) override {
        return deserialize(o, xml);
    }

    // convenience methods for implementor.

    // All append* functions helps mutateNode() to serialize the object into XML.
    template <typename T>
    inline void appendAttr(NodeType *e, const std::string &attrName, const T &attr) const {
        return appendStrAttr(e, attrName, ::android::vintf::to_string(attr));
    }

    inline void appendAttr(NodeType *e, const std::string &attrName, bool attr) const {
        return appendStrAttr(e, attrName, attr ? "true" : "false");
    }

    // text -> <name>text</name>
    inline void appendTextElement(NodeType *parent, const std::string &name,
                const std::string &text, DocType *d) const {
        NodeType *c = createNode(name, d);
        appendText(c, text, d);
        appendChild(parent, c);
    }

    // text -> <name>text</name>
    template<typename Array>
    inline void appendTextElements(NodeType *parent, const std::string &name,
                const Array &array, DocType *d) const {
        for (const std::string &text : array) {
            NodeType *c = createNode(name, d);
            appendText(c, text, d);
            appendChild(parent, c);
        }
    }

    template <typename T, typename Array>
    inline void appendChildren(NodeType* parent, const XmlNodeConverter<T>& conv,
                               const Array& array, DocType* d,
                               SerializeFlags::Type flags = SerializeFlags::EVERYTHING) const {
        for (const T &t : array) {
            appendChild(parent, conv.serialize(t, d, flags));
        }
    }

    // All parse* functions helps buildObject() to deserialize XML to the object. Returns
    // true if deserialization is successful, false if any error, and "error" will be
    // set to error message.
    template <typename T>
    inline bool parseOptionalAttr(NodeType* root, const std::string& attrName, T&& defaultValue,
                                  T* attr, std::string* /* error */) const {
        std::string attrText;
        bool success = getAttr(root, attrName, &attrText) &&
                       ::android::vintf::parse(attrText, attr);
        if (!success) {
            *attr = std::move(defaultValue);
        }
        return true;
    }

    template <typename T>
    inline bool parseAttr(NodeType* root, const std::string& attrName, T* attr,
                          std::string* error) const {
        std::string attrText;
        bool ret = getAttr(root, attrName, &attrText) && ::android::vintf::parse(attrText, attr);
        if (!ret) {
            *error = "Could not find/parse attr with name \"" + attrName + "\" and value \"" +
                     attrText + "\" for element <" + elementName() + ">";
        }
        return ret;
    }

    inline bool parseAttr(NodeType* root, const std::string& attrName, std::string* attr,
                          std::string* error) const {
        bool ret = getAttr(root, attrName, attr);
        if (!ret) {
            *error = "Could not find attr with name \"" + attrName + "\" for element <" +
                     elementName() + ">";
        }
        return ret;
    }

    inline bool parseTextElement(NodeType* root, const std::string& elementName, std::string* s,
                                 std::string* error) const {
        NodeType *child = getChild(root, elementName);
        if (child == nullptr) {
            *error = "Could not find element with name <" + elementName + "> in element <" +
                     this->elementName() + ">";
            return false;
        }
        *s = getText(child);
        return true;
    }

    inline bool parseOptionalTextElement(NodeType* root, const std::string& elementName,
                                         std::string&& defaultValue, std::string* s,
                                         std::string* /* error */) const {
        NodeType* child = getChild(root, elementName);
        *s = child == nullptr ? std::move(defaultValue) : getText(child);
        return true;
    }

    inline bool parseTextElements(NodeType* root, const std::string& elementName,
                                  std::vector<std::string>* v, std::string* /* error */) const {
        auto nodes = getChildren(root, elementName);
        v->resize(nodes.size());
        for (size_t i = 0; i < nodes.size(); ++i) {
            v->at(i) = getText(nodes[i]);
        }
        return true;
    }

    template <typename T>
    inline bool parseChild(NodeType* root, const XmlNodeConverter<T>& conv, T* t,
                           std::string* error) const {
        NodeType *child = getChild(root, conv.elementName());
        if (child == nullptr) {
            *error = "Could not find element with name <" + conv.elementName() + "> in element <" +
                     this->elementName() + ">";
            return false;
        }
        return conv.deserialize(t, child, error);
    }

    template <typename T>
    inline bool parseOptionalChild(NodeType* root, const XmlNodeConverter<T>& conv,
                                   T&& defaultValue, T* t, std::string* error) const {
        NodeType *child = getChild(root, conv.elementName());
        if (child == nullptr) {
            *t = std::move(defaultValue);
            return true;
        }
        return conv.deserialize(t, child, error);
    }

    template <typename T>
    inline bool parseOptionalChild(NodeType* root, const XmlNodeConverter<T>& conv,
                                   std::optional<T>* t, std::string* error) const {
        NodeType* child = getChild(root, conv.elementName());
        if (child == nullptr) {
            *t = std::nullopt;
            return true;
        }
        *t = std::make_optional<T>();
        return conv.deserialize(&**t, child, error);
    }

    template <typename T>
    inline bool parseChildren(NodeType* root, const XmlNodeConverter<T>& conv, std::vector<T>* v,
                              std::string* error) const {
        auto nodes = getChildren(root, conv.elementName());
        v->resize(nodes.size());
        for (size_t i = 0; i < nodes.size(); ++i) {
            if (!conv.deserialize(&v->at(i), nodes[i], error)) {
                *error = "Could not parse element with name <" + conv.elementName() +
                         "> in element <" + this->elementName() + ">: " + *error;
                return false;
            }
        }
        return true;
    }

    template <typename Container, typename T = typename Container::value_type,
              typename = typename Container::key_compare>
    inline bool parseChildren(NodeType* root, const XmlNodeConverter<T>& conv, Container* s,
                              std::string* error) const {
        std::vector<T> vec;
        if (!parseChildren(root, conv, &vec, error)) {
            return false;
        }
        s->clear();
        s->insert(vec.begin(), vec.end());
        if (s->size() != vec.size()) {
            *error = "Duplicated elements <" + conv.elementName() + "> in element <" +
                     this->elementName() + ">";
            s->clear();
            return false;
        }
        return true;
    }

    template <typename K, typename V>
    inline bool parseChildren(NodeType* root, const XmlNodeConverter<std::pair<K, V>>& conv,
                              std::map<K, V>* s, std::string* error) const {
        return parseChildren<std::map<K, V>, std::pair<K, V>>(root, conv, s, error);
    }

    inline bool parseText(NodeType* node, std::string* s, std::string* /* error */) const {
        *s = getText(node);
        return true;
    }

    template <typename T>
    inline bool parseText(NodeType* node, T* s, std::string* error) const {
        std::string text = getText(node);
        bool ret = ::android::vintf::parse(text, s);
        if (!ret) {
            *error = "Could not parse text \"" + text + "\" in element <" + elementName() + ">";
        }
        return ret;
    }

   private:
    mutable std::string mLastError;
};

template<typename Object>
struct XmlTextConverter : public XmlNodeConverter<Object> {
    XmlTextConverter(const std::string &elementName)
        : mElementName(elementName) {}

    virtual void mutateNode(const Object &object, NodeType *root, DocType *d) const override {
        appendText(root, ::android::vintf::to_string(object), d);
    }
    virtual bool buildObject(Object* object, NodeType* root, std::string* error) const override {
        return this->parseText(root, object, error);
    }
    virtual std::string elementName() const { return mElementName; }

   private:
    std::string mElementName;
};

template <typename Pair>
struct XmlPairConverter : public XmlNodeConverter<Pair> {
    XmlPairConverter(
        const std::string& elementName,
        std::unique_ptr<XmlNodeConverter<typename Pair::first_type>>&& firstConverter,
        std::unique_ptr<XmlNodeConverter<typename Pair::second_type>>&& secondConverter)
        : mElementName(elementName),
          mFirstConverter(std::move(firstConverter)),
          mSecondConverter(std::move(secondConverter)) {}

    virtual void mutateNode(const Pair& pair, NodeType* root, DocType* d) const override {
        appendChild(root, mFirstConverter->serialize(pair.first, d));
        appendChild(root, mSecondConverter->serialize(pair.second, d));
    }
    virtual bool buildObject(Pair* pair, NodeType* root, std::string* error) const override {
        return this->parseChild(root, *mFirstConverter, &pair->first, error) &&
               this->parseChild(root, *mSecondConverter, &pair->second, error);
    }
    virtual std::string elementName() const { return mElementName; }

   private:
    std::string mElementName;
    std::unique_ptr<XmlNodeConverter<typename Pair::first_type>> mFirstConverter;
    std::unique_ptr<XmlNodeConverter<typename Pair::second_type>> mSecondConverter;
};

// ---------------------- XmlNodeConverter definitions end

XmlTextConverter<Version> versionConverter{"version"};

XmlTextConverter<VersionRange> versionRangeConverter{"version"};

struct TransportArchConverter : public XmlNodeConverter<TransportArch> {
    std::string elementName() const override { return "transport"; }
    void mutateNode(const TransportArch &object, NodeType *root, DocType *d) const override {
        if (object.arch != Arch::ARCH_EMPTY) {
            appendAttr(root, "arch", object.arch);
        }
        appendText(root, ::android::vintf::to_string(object.transport), d);
    }
    bool buildObject(TransportArch* object, NodeType* root, std::string* error) const override {
        if (!parseOptionalAttr(root, "arch", Arch::ARCH_EMPTY, &object->arch, error) ||
            !parseText(root, &object->transport, error)) {
            return false;
        }
        if (!object->isValid()) {
            *error = "transport == " + ::android::vintf::to_string(object->transport) +
                     " and arch == " + ::android::vintf::to_string(object->arch) +
                     " is not a valid combination.";
            return false;
        }
        return true;
    }
};

TransportArchConverter transportArchConverter{};

struct KernelConfigTypedValueConverter : public XmlNodeConverter<KernelConfigTypedValue> {
    std::string elementName() const override { return "value"; }
    void mutateNode(const KernelConfigTypedValue &object, NodeType *root, DocType *d) const override {
        appendAttr(root, "type", object.mType);
        appendText(root, ::android::vintf::to_string(object), d);
    }
    bool buildObject(KernelConfigTypedValue* object, NodeType* root,
                     std::string* error) const override {
        std::string stringValue;
        if (!parseAttr(root, "type", &object->mType, error) ||
            !parseText(root, &stringValue, error)) {
            return false;
        }
        if (!::android::vintf::parseKernelConfigValue(stringValue, object)) {
            *error = "Could not parse kernel config value \"" + stringValue + "\"";
            return false;
        }
        return true;
    }
};

KernelConfigTypedValueConverter kernelConfigTypedValueConverter{};

XmlPairConverter<KernelConfig> matrixKernelConfigConverter{
    "config", std::make_unique<XmlTextConverter<KernelConfigKey>>("key"),
    std::make_unique<KernelConfigTypedValueConverter>(kernelConfigTypedValueConverter)};

struct HalInterfaceConverter : public XmlNodeConverter<HalInterface> {
    std::string elementName() const override { return "interface"; }
    void mutateNode(const HalInterface &intf, NodeType *root, DocType *d) const override {
        appendTextElement(root, "name", intf.name(), d);
        appendTextElements(root, "instance", intf.mInstances, d);
        appendTextElements(root, "regex-instance", intf.mRegexes, d);
    }
    bool buildObject(HalInterface* intf, NodeType* root, std::string* error) const override {
        std::vector<std::string> instances;
        std::vector<std::string> regexes;
        if (!parseTextElement(root, "name", &intf->mName, error) ||
            !parseTextElements(root, "instance", &instances, error) ||
            !parseTextElements(root, "regex-instance", &regexes, error)) {
            return false;
        }
        bool success = true;
        for (const auto& e : instances) {
            if (!intf->insertInstance(e, false /* isRegex */)) {
                if (!error->empty()) *error += "\n";
                *error += "Duplicated instance '" + e + "' in " + intf->name();
                success = false;
            }
        }
        for (const auto& e : regexes) {
            details::Regex regex;
            if (!regex.compile(e)) {
                if (!error->empty()) *error += "\n";
                *error += "Invalid regular expression '" + e + "' in " + intf->name();
                success = false;
            }
            if (!intf->insertInstance(e, true /* isRegex */)) {
                if (!error->empty()) *error += "\n";
                *error += "Duplicated regex-instance '" + e + "' in " + intf->name();
                success = false;
            }
        }
        return success;
    }
};

HalInterfaceConverter halInterfaceConverter{};

struct MatrixHalConverter : public XmlNodeConverter<MatrixHal> {
    std::string elementName() const override { return "hal"; }
    void mutateNode(const MatrixHal &hal, NodeType *root, DocType *d) const override {
        appendAttr(root, "format", hal.format);
        appendAttr(root, "optional", hal.optional);
        appendTextElement(root, "name", hal.name, d);
        appendChildren(root, versionRangeConverter, hal.versionRanges, d);
        appendChildren(root, halInterfaceConverter, iterateValues(hal.interfaces), d);
    }
    bool buildObject(MatrixHal* object, NodeType* root, std::string* error) const override {
        std::vector<HalInterface> interfaces;
        if (!parseOptionalAttr(root, "format", HalFormat::HIDL, &object->format, error) ||
            !parseOptionalAttr(root, "optional", false /* defaultValue */, &object->optional,
                               error) ||
            !parseTextElement(root, "name", &object->name, error) ||
            !parseChildren(root, versionRangeConverter, &object->versionRanges, error) ||
            !parseChildren(root, halInterfaceConverter, &interfaces, error)) {
            return false;
        }
        for (auto&& interface : interfaces) {
            std::string name{interface.name()};
            auto res = object->interfaces.emplace(std::move(name), std::move(interface));
            if (!res.second) {
                *error = "Duplicated interface entry \"" + res.first->first +
                         "\"; if additional instances are needed, add them to the "
                         "existing <interface> node.";
                return false;
            }
        }
// Do not check for target-side libvintf to avoid restricting ability for upgrade accidentally.
#ifndef LIBVINTF_TARGET
        if (!checkAdditionalRestrictionsOnHal(*object, error)) {
            return false;
        }
#endif
        return true;
    }

#ifndef LIBVINTF_TARGET
   private:
    bool checkAdditionalRestrictionsOnHal(const MatrixHal& hal, std::string* error) const {
        if (hal.getName() == "netutils-wrapper") {
            if (hal.versionRanges.size() != 1) {
                *error =
                    "netutils-wrapper HAL must specify exactly one version x.0, "
                    "but multiple <version> element is specified.";
                return false;
            }
            const VersionRange& v = hal.versionRanges.at(0);
            if (!v.isSingleVersion()) {
                *error =
                    "netutils-wrapper HAL must specify exactly one version x.0, "
                    "but a range is provided. Perhaps you mean '" +
                    to_string(Version{v.majorVer, 0}) + "'?";
                return false;
            }
            if (v.minMinor != 0) {
                *error =
                    "netutils-wrapper HAL must specify exactly one version x.0, "
                    "but minor version is not 0. Perhaps you mean '" +
                    to_string(Version{v.majorVer, 0}) + "'?";
                return false;
            }
        }
        return true;
    }
#endif
};

MatrixHalConverter matrixHalConverter{};

struct MatrixKernelConditionsConverter : public XmlNodeConverter<std::vector<KernelConfig>> {
    std::string elementName() const override { return "conditions"; }
    void mutateNode(const std::vector<KernelConfig>& conds, NodeType* root,
                    DocType* d) const override {
        appendChildren(root, matrixKernelConfigConverter, conds, d);
    }
    bool buildObject(std::vector<KernelConfig>* object, NodeType* root,
                     std::string* error) const override {
        return parseChildren(root, matrixKernelConfigConverter, object, error);
    }
};

MatrixKernelConditionsConverter matrixKernelConditionsConverter{};

struct MatrixKernelConverter : public XmlNodeConverter<MatrixKernel> {
    std::string elementName() const override { return "kernel"; }
    void mutateNode(const MatrixKernel& kernel, NodeType* root, DocType* d) const override {
        mutateNode(kernel, root, d, SerializeFlags::EVERYTHING);
    }
    void mutateNode(const MatrixKernel& kernel, NodeType* root, DocType* d,
                    SerializeFlags::Type flags) const override {
        KernelVersion kv = kernel.mMinLts;
        if (!flags.isKernelMinorRevisionEnabled()) {
            kv.minorRev = 0u;
        }
        appendAttr(root, "version", kv);

        if (!kernel.mConditions.empty()) {
            appendChild(root, matrixKernelConditionsConverter(kernel.mConditions, d));
        }
        if (flags.isKernelConfigsEnabled()) {
            appendChildren(root, matrixKernelConfigConverter, kernel.mConfigs, d);
        }
    }
    bool buildObject(MatrixKernel* object, NodeType* root, std::string* error) const override {
        if (!parseAttr(root, "version", &object->mMinLts, error) ||
            !parseOptionalChild(root, matrixKernelConditionsConverter, {}, &object->mConditions,
                                error) ||
            !parseChildren(root, matrixKernelConfigConverter, &object->mConfigs, error)) {
            return false;
        }
        return true;
    }
};

MatrixKernelConverter matrixKernelConverter{};

XmlTextConverter<FqInstance> fqInstanceConverter{"fqname"};

struct ManifestHalConverter : public XmlNodeConverter<ManifestHal> {
    std::string elementName() const override { return "hal"; }
    void mutateNode(const ManifestHal& m, NodeType* root, DocType* d) const override {
        mutateNode(m, root, d, SerializeFlags::EVERYTHING);
    }
    void mutateNode(const ManifestHal& hal, NodeType* root, DocType* d,
                    SerializeFlags::Type flags) const override {
        appendAttr(root, "format", hal.format);
        appendTextElement(root, "name", hal.name, d);
        appendChild(root, transportArchConverter(hal.transportArch, d));
        appendChildren(root, versionConverter, hal.versions, d);
        appendChildren(root, halInterfaceConverter, iterateValues(hal.interfaces), d);
        if (hal.isOverride()) {
            appendAttr(root, "override", hal.isOverride());
        }

        if (flags.isFqnameEnabled()) {
            std::set<FqInstance> fqInstances;
            hal.forEachInstance([&fqInstances](const auto& manifestInstance) {
                fqInstances.emplace(manifestInstance.getFqInstanceNoPackage());
                return true;
            });
            appendChildren(root, fqInstanceConverter, fqInstances, d);
        }
    }
    bool buildObject(ManifestHal* object, NodeType* root, std::string* error) const override {
        std::vector<HalInterface> interfaces;
        if (!parseOptionalAttr(root, "format", HalFormat::HIDL, &object->format, error) ||
            !parseOptionalAttr(root, "override", false, &object->mIsOverride, error) ||
            !parseTextElement(root, "name", &object->name, error) ||
            !parseOptionalChild(root, transportArchConverter, {}, &object->transportArch, error) ||
            !parseChildren(root, versionConverter, &object->versions, error) ||
            !parseChildren(root, halInterfaceConverter, &interfaces, error)) {
            return false;
        }

        switch (object->format) {
            case HalFormat::HIDL: {
                if (object->transportArch.empty()) {
                    *error = "HIDL HAL '" + object->name + "' should have <transport> defined.";
                    return false;
                }
            } break;
            case HalFormat::NATIVE: {
                if (!object->transportArch.empty()) {
                    *error =
                        "Native HAL '" + object->name + "' should not have <transport> defined.";
                    return false;
                }
            } break;
            default: {
                LOG(FATAL) << "Unhandled HalFormat "
                           << static_cast<typename std::underlying_type<HalFormat>::type>(
                                  object->format);
            } break;
        }
        if (!object->transportArch.isValid()) return false;

        object->interfaces.clear();
        for (auto &&interface : interfaces) {
            auto res = object->interfaces.emplace(interface.name(), std::move(interface));
            if (!res.second) {
                *error = "Duplicated interface entry \"" + res.first->first +
                         "\"; if additional instances are needed, add them to the "
                         "existing <interface> node.";
                return false;
            }
        }
        if (!object->isValid()) {
            *error = "'" + object->name + "' is not a valid Manifest HAL.";
            return false;
        }
// Do not check for target-side libvintf to avoid restricting upgrade accidentally.
#ifndef LIBVINTF_TARGET
        if (!checkAdditionalRestrictionsOnHal(*object, error)) {
            return false;
        }
#endif

        std::set<FqInstance> fqInstances;
        if (!parseChildren(root, fqInstanceConverter, &fqInstances, error)) {
            return false;
        }
        for (const auto& e : fqInstances) {
            if (e.hasPackage()) {
                *error = "Should not specify package: \"" + e.string() + "\"";
                return false;
            }
        }
        if (!object->insertInstances(fqInstances, error)) {
            return false;
        }

        return true;
    }

#ifndef LIBVINTF_TARGET
   private:
    bool checkAdditionalRestrictionsOnHal(const ManifestHal& hal, std::string* error) const {
        if (hal.getName() == "netutils-wrapper") {
            for (const Version& v : hal.versions) {
                if (v.minorVer != 0) {
                    *error =
                        "netutils-wrapper HAL must specify exactly one version x.0, "
                        "but minor version is not 0. Perhaps you mean '" +
                        to_string(Version{v.majorVer, 0}) + "'?";
                    return false;
                }
            }
        }
        return true;
    }
#endif
};

// Convert ManifestHal from and to XML. Returned object is guaranteed to have
// .isValid() == true.
ManifestHalConverter manifestHalConverter{};

XmlTextConverter<KernelSepolicyVersion> kernelSepolicyVersionConverter{"kernel-sepolicy-version"};
XmlTextConverter<VersionRange> sepolicyVersionConverter{"sepolicy-version"};

struct SepolicyConverter : public XmlNodeConverter<Sepolicy> {
    std::string elementName() const override { return "sepolicy"; }
    void mutateNode(const Sepolicy &object, NodeType *root, DocType *d) const override {
        appendChild(root, kernelSepolicyVersionConverter(object.kernelSepolicyVersion(), d));
        appendChildren(root, sepolicyVersionConverter, object.sepolicyVersions(), d);
    }
    bool buildObject(Sepolicy* object, NodeType* root, std::string* error) const override {
        if (!parseChild(root, kernelSepolicyVersionConverter, &object->mKernelSepolicyVersion,
                        error) ||
            !parseChildren(root, sepolicyVersionConverter, &object->mSepolicyVersionRanges,
                           error)) {
            return false;
        }
        return true;
    }
};
SepolicyConverter sepolicyConverter{};

[[deprecated]] XmlTextConverter<VndkVersionRange> vndkVersionRangeConverter{"version"};

XmlTextConverter<std::string> vndkVersionConverter{"version"};
XmlTextConverter<std::string> vndkLibraryConverter{"library"};

struct [[deprecated]] VndkConverter : public XmlNodeConverter<Vndk> {
    std::string elementName() const override { return "vndk"; }
    void mutateNode(const Vndk &object, NodeType *root, DocType *d) const override {
        appendChild(root, vndkVersionRangeConverter(object.mVersionRange, d));
        appendChildren(root, vndkLibraryConverter, object.mLibraries, d);
    }
    bool buildObject(Vndk* object, NodeType* root, std::string* error) const override {
        if (!parseChild(root, vndkVersionRangeConverter, &object->mVersionRange, error) ||
            !parseChildren(root, vndkLibraryConverter, &object->mLibraries, error)) {
            return false;
        }
        return true;
    }
};

[[deprecated]] VndkConverter vndkConverter{};

struct VendorNdkConverter : public XmlNodeConverter<VendorNdk> {
    std::string elementName() const override { return "vendor-ndk"; }
    void mutateNode(const VendorNdk& object, NodeType* root, DocType* d) const override {
        appendChild(root, vndkVersionConverter(object.mVersion, d));
        appendChildren(root, vndkLibraryConverter, object.mLibraries, d);
    }
    bool buildObject(VendorNdk* object, NodeType* root, std::string* error) const override {
        if (!parseChild(root, vndkVersionConverter, &object->mVersion, error) ||
            !parseChildren(root, vndkLibraryConverter, &object->mLibraries, error)) {
            return false;
        }
        return true;
    }
};

VendorNdkConverter vendorNdkConverter{};

XmlTextConverter<std::string> systemSdkVersionConverter{"version"};

struct SystemSdkConverter : public XmlNodeConverter<SystemSdk> {
    std::string elementName() const override { return "system-sdk"; }
    void mutateNode(const SystemSdk& object, NodeType* root, DocType* d) const override {
        appendChildren(root, systemSdkVersionConverter, object.versions(), d);
    }
    bool buildObject(SystemSdk* object, NodeType* root, std::string* error) const override {
        return parseChildren(root, systemSdkVersionConverter, &object->mVersions, error);
    }
};

SystemSdkConverter systemSdkConverter{};

struct HalManifestSepolicyConverter : public XmlNodeConverter<Version> {
    std::string elementName() const override { return "sepolicy"; }
    void mutateNode(const Version &m, NodeType *root, DocType *d) const override {
        appendChild(root, versionConverter(m, d));
    }
    bool buildObject(Version* object, NodeType* root, std::string* error) const override {
        return parseChild(root, versionConverter, object, error);
    }
};
HalManifestSepolicyConverter halManifestSepolicyConverter{};

struct ManifestXmlFileConverter : public XmlNodeConverter<ManifestXmlFile> {
    std::string elementName() const override { return "xmlfile"; }
    void mutateNode(const ManifestXmlFile& f, NodeType* root, DocType* d) const override {
        appendTextElement(root, "name", f.name(), d);
        appendChild(root, versionConverter(f.version(), d));
        if (!f.overriddenPath().empty()) {
            appendTextElement(root, "path", f.overriddenPath(), d);
        }
    }
    bool buildObject(ManifestXmlFile* object, NodeType* root, std::string* error) const override {
        if (!parseTextElement(root, "name", &object->mName, error) ||
            !parseChild(root, versionConverter, &object->mVersion, error) ||
            !parseOptionalTextElement(root, "path", {}, &object->mOverriddenPath, error)) {
            return false;
        }
        return true;
    }
};
ManifestXmlFileConverter manifestXmlFileConverter{};

XmlPairConverter<std::pair<std::string, std::string>> kernelConfigConverter{
    "config", std::make_unique<XmlTextConverter<std::string>>("key"),
    std::make_unique<XmlTextConverter<std::string>>("value")};

struct KernelInfoConverter : public XmlNodeConverter<KernelInfo> {
    std::string elementName() const override { return "kernel"; }
    void mutateNode(const KernelInfo& o, NodeType* root, DocType* d) const override {
        mutateNode(o, root, d, SerializeFlags::EVERYTHING);
    }
    void mutateNode(const KernelInfo& o, NodeType* root, DocType* d,
                    SerializeFlags::Type flags) const override {
        if (o.version() != KernelVersion{}) {
            appendAttr(root, "version", o.version());
        }
        if (flags.isKernelConfigsEnabled()) {
            appendChildren(root, kernelConfigConverter, o.configs(), d);
        }
    }
    bool buildObject(KernelInfo* o, NodeType* root, std::string* error) const override {
        return parseOptionalAttr(root, "version", {}, &o->mVersion, error) &&
               parseChildren(root, kernelConfigConverter, &o->mConfigs, error);
    }
};

KernelInfoConverter kernelInfoConverter{};

struct HalManifestConverter : public XmlNodeConverter<HalManifest> {
    std::string elementName() const override { return "manifest"; }
    void mutateNode(const HalManifest &m, NodeType *root, DocType *d) const override {
        mutateNode(m, root, d, SerializeFlags::EVERYTHING);
    }
    void mutateNode(const HalManifest& m, NodeType* root, DocType* d,
                    SerializeFlags::Type flags) const override {
        if (flags.isMetaVersionEnabled()) {
            appendAttr(root, "version", m.getMetaVersion());
        }
        if (flags.isSchemaTypeEnabled()) {
            appendAttr(root, "type", m.mType);
        }

        if (flags.isHalsEnabled()) {
            appendChildren(root, manifestHalConverter, m.getHals(), d, flags);
        }
        if (m.mType == SchemaType::DEVICE) {
            if (flags.isSepolicyEnabled()) {
                if (m.device.mSepolicyVersion != Version{}) {
                    appendChild(root, halManifestSepolicyConverter(m.device.mSepolicyVersion, d));
                }
            }
            if (m.mLevel != Level::UNSPECIFIED) {
                this->appendAttr(root, "target-level", m.mLevel);
            }

            if (flags.isKernelEnabled()) {
                if (!!m.kernel()) {
                    appendChild(root, kernelInfoConverter.serialize(*m.kernel(), d, flags));
                }
            }
        } else if (m.mType == SchemaType::FRAMEWORK) {
            if (flags.isVndkEnabled()) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
                appendChildren(root, vndkConverter, m.framework.mVndks, d);
#pragma clang diagnostic pop

                appendChildren(root, vendorNdkConverter, m.framework.mVendorNdks, d);
            }
            if (flags.isSsdkEnabled()) {
                if (!m.framework.mSystemSdk.empty()) {
                    appendChild(root, systemSdkConverter(m.framework.mSystemSdk, d));
                }
            }
        }

        if (flags.isXmlFilesEnabled()) {
            appendChildren(root, manifestXmlFileConverter, m.getXmlFiles(), d);
        }
    }
    bool buildObject(HalManifest* object, NodeType* root, std::string* error) const override {
        std::vector<ManifestHal> hals;
        if (!parseAttr(root, "version", &object->mMetaVersion, error) ||
            !parseAttr(root, "type", &object->mType, error) ||
            !parseChildren(root, manifestHalConverter, &hals, error)) {
            return false;
        }
        if (!kMetaVersion.minorAtLeast(object->mMetaVersion)) {
            *error = "Unrecognized manifest.version " + to_string(object->mMetaVersion) +
                     " (libvintf@" + to_string(kMetaVersion) + ")";
            return false;
        }
        if (object->mType == SchemaType::DEVICE) {
            // tags for device hal manifest only.
            // <sepolicy> can be missing because it can be determined at build time, not hard-coded
            // in the XML file.
            if (!parseOptionalChild(root, halManifestSepolicyConverter, {},
                                    &object->device.mSepolicyVersion, error)) {
                return false;
            }

            if (!parseOptionalAttr(root, "target-level", Level::UNSPECIFIED, &object->mLevel,
                                   error)) {
                return false;
            }

            if (!parseOptionalChild(root, kernelInfoConverter, &object->device.mKernel, error)) {
                return false;
            }
        } else if (object->mType == SchemaType::FRAMEWORK) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
            if (!parseChildren(root, vndkConverter, &object->framework.mVndks, error)) {
                return false;
            }
            for (const auto &vndk : object->framework.mVndks) {
                if (!vndk.mVersionRange.isSingleVersion()) {
                    *error = "vndk.version " + to_string(vndk.mVersionRange) +
                             " cannot be a range for manifests";
                    return false;
                }
            }
#pragma clang diagnostic pop

            if (!parseChildren(root, vendorNdkConverter, &object->framework.mVendorNdks, error)) {
                return false;
            }

            std::set<std::string> vendorNdkVersions;
            for (const auto& vendorNdk : object->framework.mVendorNdks) {
                if (vendorNdkVersions.find(vendorNdk.version()) != vendorNdkVersions.end()) {
                    *error = "Duplicated manifest.vendor-ndk.version " + vendorNdk.version();
                    return false;
                }
                vendorNdkVersions.insert(vendorNdk.version());
            }

            if (!parseOptionalChild(root, systemSdkConverter, {}, &object->framework.mSystemSdk,
                                    error)) {
                return false;
            }
        }
        for (auto &&hal : hals) {
            std::string description{hal.name};
            if (!object->add(std::move(hal))) {
                *error = "Duplicated manifest.hal entry " + description;
                return false;
            }
        }

        std::vector<ManifestXmlFile> xmlFiles;
        if (!parseChildren(root, manifestXmlFileConverter, &xmlFiles, error)) {
            return false;
        }
        for (auto&& xmlFile : xmlFiles) {
            std::string description{xmlFile.name()};
            if (!object->addXmlFile(std::move(xmlFile))) {
                *error = "Duplicated manifest.xmlfile entry " + description +
                         "; entries cannot have duplicated name and version";
                return false;
            }
        }

        return true;
    }
};

HalManifestConverter halManifestConverter{};

XmlTextConverter<Version> avbVersionConverter{"vbmeta-version"};
struct AvbConverter : public XmlNodeConverter<Version> {
    std::string elementName() const override { return "avb"; }
    void mutateNode(const Version &m, NodeType *root, DocType *d) const override {
        appendChild(root, avbVersionConverter(m, d));
    }
    bool buildObject(Version* object, NodeType* root, std::string* error) const override {
        return parseChild(root, avbVersionConverter, object, error);
    }
};
AvbConverter avbConverter{};

struct MatrixXmlFileConverter : public XmlNodeConverter<MatrixXmlFile> {
    std::string elementName() const override { return "xmlfile"; }
    void mutateNode(const MatrixXmlFile& f, NodeType* root, DocType* d) const override {
        appendTextElement(root, "name", f.name(), d);
        appendAttr(root, "format", f.format());
        appendAttr(root, "optional", f.optional());
        appendChild(root, versionRangeConverter(f.versionRange(), d));
        if (!f.overriddenPath().empty()) {
            appendTextElement(root, "path", f.overriddenPath(), d);
        }
    }
    bool buildObject(MatrixXmlFile* object, NodeType* root, std::string* error) const override {
        if (!parseTextElement(root, "name", &object->mName, error) ||
            !parseAttr(root, "format", &object->mFormat, error) ||
            !parseOptionalAttr(root, "optional", false, &object->mOptional, error) ||
            !parseChild(root, versionRangeConverter, &object->mVersionRange, error) ||
            !parseOptionalTextElement(root, "path", {}, &object->mOverriddenPath, error)) {
            return false;
        }
        return true;
    }
};
MatrixXmlFileConverter matrixXmlFileConverter{};

struct CompatibilityMatrixConverter : public XmlNodeConverter<CompatibilityMatrix> {
    std::string elementName() const override { return "compatibility-matrix"; }
    void mutateNode(const CompatibilityMatrix &m, NodeType *root, DocType *d) const override {
        mutateNode(m, root, d, SerializeFlags::EVERYTHING);
    }
    void mutateNode(const CompatibilityMatrix& m, NodeType* root, DocType* d,
                    SerializeFlags::Type flags) const override {
        if (flags.isMetaVersionEnabled()) {
            appendAttr(root, "version", m.getMinimumMetaVersion());
        }
        if (flags.isSchemaTypeEnabled()) {
            appendAttr(root, "type", m.mType);
        }

        if (flags.isHalsEnabled()) {
            appendChildren(root, matrixHalConverter, iterateValues(m.mHals), d);
        }
        if (m.mType == SchemaType::FRAMEWORK) {
            if (flags.isKernelEnabled()) {
                appendChildren(root, matrixKernelConverter, m.framework.mKernels, d, flags);
            }
            if (flags.isSepolicyEnabled()) {
                if (!(m.framework.mSepolicy == Sepolicy{})) {
                    appendChild(root, sepolicyConverter(m.framework.mSepolicy, d));
                }
            }
            if (flags.isAvbEnabled()) {
                if (!(m.framework.mAvbMetaVersion == Version{})) {
                    appendChild(root, avbConverter(m.framework.mAvbMetaVersion, d));
                }
            }
            if (m.mLevel != Level::UNSPECIFIED) {
                this->appendAttr(root, "level", m.mLevel);
            }
        } else if (m.mType == SchemaType::DEVICE) {
            if (flags.isVndkEnabled()) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
                if (!(m.device.mVndk == Vndk{})) {
                    appendChild(root, vndkConverter(m.device.mVndk, d));
                }
#pragma clang diagnostic pop

                if (!(m.device.mVendorNdk == VendorNdk{})) {
                    appendChild(root, vendorNdkConverter(m.device.mVendorNdk, d));
                }
            }

            if (flags.isSsdkEnabled()) {
                if (!m.device.mSystemSdk.empty()) {
                    appendChild(root, systemSdkConverter(m.device.mSystemSdk, d));
                }
            }
        }

        if (flags.isXmlFilesEnabled()) {
            appendChildren(root, matrixXmlFileConverter, m.getXmlFiles(), d);
        }
    }
    bool buildObject(CompatibilityMatrix* object, NodeType* root,
                     std::string* error) const override {
        Version version;
        std::vector<MatrixHal> hals;
        if (!parseAttr(root, "version", &version, error) ||
            !parseAttr(root, "type", &object->mType, error) ||
            !parseChildren(root, matrixHalConverter, &hals, error)) {
            return false;
        }

        if (object->mType == SchemaType::FRAMEWORK) {
            // <avb> and <sepolicy> can be missing because it can be determined at build time, not
            // hard-coded in the XML file.
            if (!parseChildren(root, matrixKernelConverter, &object->framework.mKernels, error) ||
                !parseOptionalChild(root, sepolicyConverter, {}, &object->framework.mSepolicy,
                                    error) ||
                !parseOptionalChild(root, avbConverter, {}, &object->framework.mAvbMetaVersion,
                                    error)) {
                return false;
            }

            std::set<Version> seenKernelVersions;
            for (const auto& kernel : object->framework.mKernels) {
                Version minLts(kernel.minLts().version, kernel.minLts().majorRev);
                if (seenKernelVersions.find(minLts) != seenKernelVersions.end()) {
                    continue;
                }
                if (!kernel.conditions().empty()) {
                    *error = "First <kernel> for version " + to_string(minLts) +
                             " must have empty <conditions> for backwards compatibility.";
                    return false;
                }
                seenKernelVersions.insert(minLts);
            }

            if (!parseOptionalAttr(root, "level", Level::UNSPECIFIED, &object->mLevel, error)) {
                return false;
            }

        } else if (object->mType == SchemaType::DEVICE) {
            // <vndk> can be missing because it can be determined at build time, not hard-coded
            // in the XML file.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
            if (!parseOptionalChild(root, vndkConverter, {}, &object->device.mVndk, error)) {
                return false;
            }
#pragma clang diagnostic pop

            if (!parseOptionalChild(root, vendorNdkConverter, {}, &object->device.mVendorNdk,
                                    error)) {
                return false;
            }

            if (!parseOptionalChild(root, systemSdkConverter, {}, &object->device.mSystemSdk,
                                    error)) {
                return false;
            }
        }

        if (!kMetaVersion.minorAtLeast(version)) {
            *error = "Unrecognized compatibility-matrix.version " + to_string(version) +
                     " (libvintf@" + to_string(kMetaVersion) + ")";
            return false;
        }
        for (auto &&hal : hals) {
            if (!object->add(std::move(hal))) {
                *error = "Duplicated compatibility-matrix.hal entry";
                return false;
            }
        }

        std::vector<MatrixXmlFile> xmlFiles;
        if (!parseChildren(root, matrixXmlFileConverter, &xmlFiles, error)) {
            return false;
        }
        for (auto&& xmlFile : xmlFiles) {
            if (!xmlFile.optional()) {
                *error = "compatibility-matrix.xmlfile entry " + xmlFile.name() +
                         " has to be optional for compatibility matrix version 1.0";
                return false;
            }
            std::string description{xmlFile.name()};
            if (!object->addXmlFile(std::move(xmlFile))) {
                *error = "Duplicated compatibility-matrix.xmlfile entry " + description;
                return false;
            }
        }

        return true;
    }
};

CompatibilityMatrixConverter compatibilityMatrixConverter{};

// Publicly available as in parse_xml.h
XmlConverter<HalManifest>& gHalManifestConverter = halManifestConverter;
XmlConverter<CompatibilityMatrix>& gCompatibilityMatrixConverter = compatibilityMatrixConverter;

// For testing in LibVintfTest
XmlConverter<Version>& gVersionConverter = versionConverter;
XmlConverter<KernelConfigTypedValue>& gKernelConfigTypedValueConverter =
    kernelConfigTypedValueConverter;
XmlConverter<MatrixHal>& gMatrixHalConverter = matrixHalConverter;
XmlConverter<ManifestHal>& gManifestHalConverter = manifestHalConverter;
XmlConverter<KernelInfo>& gKernelInfoConverter = kernelInfoConverter;

} // namespace vintf
} // namespace android
