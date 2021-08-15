/*
 * Copyright (C) 2018 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.xsdc.cpp;

import com.android.xsdc.CodeWriter;
import com.android.xsdc.FileSystem;
import com.android.xsdc.XmlSchema;
import com.android.xsdc.XsdConstants;
import com.android.xsdc.tag.*;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import javax.xml.namespace.QName;

public class CppCodeGenerator {
    private XmlSchema xmlSchema;
    private String fileName;
    private Map<String, CppSimpleType> cppSimpleTypeMap;
    private CodeWriter cppFile;
    private CodeWriter headerFile;
    private boolean hasAttr;

    public CppCodeGenerator(XmlSchema xmlSchema, String fileName)
            throws CppCodeGeneratorException {
        this.xmlSchema = xmlSchema;
        this.fileName = fileName;

        // class naming validation
        {
            Set<String> nameSet = new HashSet<>();
            nameSet.add("XmlParser");
            for (XsdType type : xmlSchema.getTypeMap().values()) {
                if ((type instanceof XsdComplexType) || (type instanceof XsdRestriction &&
                        ((XsdRestriction)type).getEnums() != null)) {
                    String name = Utils.toClassName(type.getName());
                    if (nameSet.contains(name)) {
                        throw new CppCodeGeneratorException(
                                String.format("duplicate class name : %s", name));
                    }
                    nameSet.add(name);
                    if (type instanceof XsdComplexType && !hasAttr) {
                        hasAttr = hasAttribute((XsdComplexType)type);
                    }
                }
            }
            for (XsdElement element : xmlSchema.getElementMap().values()) {
                XsdType type = element.getType();
                if (type.getRef() == null && type instanceof XsdComplexType) {
                    String name = Utils.toClassName(element.getName());
                    if (nameSet.contains(name)) {
                        throw new CppCodeGeneratorException(
                                String.format("duplicate class name : %s", name));
                    }
                    nameSet.add(name);
                    if (!hasAttr) {
                        hasAttr = hasAttribute((XsdComplexType)type);
                    }
                }
            }
        }

        cppSimpleTypeMap = new HashMap<>();
        for (XsdType type : xmlSchema.getTypeMap().values()) {
            if (type instanceof XsdSimpleType) {
                XsdType refType = new XsdType(null, new QName(type.getName()));
                parseSimpleType(refType, true);
            }
        }
    }

    public void print(FileSystem fs)
            throws CppCodeGeneratorException, IOException {
        // cpp file, headr file init
        cppFile =  new CodeWriter(fs.getPrintWriter(fileName + ".cpp"));
        headerFile = new CodeWriter(fs.getPrintWriter("include/" + fileName + ".h"));

        headerFile.printf("#ifndef %s_H\n", fileName.toUpperCase());
        headerFile.printf("#define %s_H\n\n", fileName.toUpperCase());
        headerFile.printf("#include <libxml/parser.h>\n");
        headerFile.printf("#include <libxml/xinclude.h>\n\n");
        headerFile.printf("#include <map>\n");
        headerFile.printf("#include <optional>\n");
        headerFile.printf("#include <string>\n");
        headerFile.printf("#include <vector>\n\n");

        cppFile.printf("#define LOG_TAG \"%s\"\n\n", fileName);
        cppFile.printf("#include <android/log.h>\n");
        cppFile.printf("#include <android-base/strings.h>\n\n");
        cppFile.printf("#include <libxml/parser.h>\n");
        cppFile.printf("#include <libxml/xinclude.h>\n\n");
        cppFile.printf("#include \"%s.h\"\n\n",fileName);

        List<String> namespace = new java.util.ArrayList<>();
        for (String token : fileName.split("_")) {
            namespace.add(token);
            headerFile.printf("namespace %s {\n", token);
            cppFile.printf("namespace %s {\n", token);
        }

        printPrototype();
        printXmlParser();

        for (XsdType type : xmlSchema.getTypeMap().values()) {
            if (type instanceof XsdComplexType) {
                String name = Utils.toClassName(type.getName());
                XsdComplexType complexType = (XsdComplexType) type;
                printClass(name, "", complexType);
            } else if (type instanceof XsdRestriction &&
                  ((XsdRestriction)type).getEnums() != null) {
                String name = Utils.toClassName(type.getName());
                XsdRestriction restrictionType = (XsdRestriction) type;
                printEnum(name, restrictionType);
            }
        }
        for (XsdElement element : xmlSchema.getElementMap().values()) {
            XsdType type = element.getType();
            if (type.getRef() == null && type instanceof XsdComplexType) {
                String name = Utils.toClassName(element.getName());
                XsdComplexType complexType = (XsdComplexType) type;
                printClass(name, "", complexType);
            }
        }

        Collections.reverse(namespace);
        for (String token : namespace) {
            headerFile.printf("} // %s\n", token);
            cppFile.printf("} // %s\n", token);
        }

        headerFile.printf("#endif // %s_H\n",fileName.toUpperCase().replace(".", "_"));
        cppFile.close();
        headerFile.close();
    }

    private void printEnum(String name, XsdRestriction restrictionType)
            throws CppCodeGeneratorException {
        headerFile.printf("enum class %s {\n", name);
        cppFile.printf("const std::map<std::string, %s> %sString {\n", name, name);
        List<XsdEnumeration> enums = restrictionType.getEnums();

        for (XsdEnumeration tag : enums) {
            headerFile.printf("%s,\n", Utils.toEnumName(tag.getValue()));
            cppFile.printf("{ \"%s\", %s::%s },\n", tag.getValue(), name,
                    Utils.toEnumName(tag.getValue()));
        }
        headerFile.printf("UNKNOWN\n};\n\n");
        cppFile.printf("};\n\n");

        cppFile.printf("static %s stringTo%s(std::string value) {\n"
                + "auto enumValue =  %sString.find(value);\n"
                + "return enumValue == %sString.end() ? %s::UNKNOWN : enumValue->second;\n"
                + "}\n\n", name, name, name, name, name);
    }


    private void printPrototype() throws CppCodeGeneratorException {
        for (XsdType type : xmlSchema.getTypeMap().values()) {
            if (type instanceof XsdComplexType) {
                String name = Utils.toClassName(type.getName());
                headerFile.printf("class %s;\n", name);
            }
        }
        for (XsdElement element : xmlSchema.getElementMap().values()) {
            XsdType type = element.getType();
            if (type.getRef() == null && type instanceof XsdComplexType) {
                String name = Utils.toClassName(element.getName());
                headerFile.printf("class %s;\n", name);
            }
        }
    }

    private void printClass(String name, String nameScope, XsdComplexType complexType)
            throws CppCodeGeneratorException {
        assert name != null;
        // need element, attribute name duplicate validation?

        String baseName = getBaseName(complexType);
        CppSimpleType valueType = (complexType instanceof XsdSimpleContent) ?
                getValueType((XsdSimpleContent) complexType, false) : null;

        headerFile.printf("class %s ", name);

        if (baseName != null) {
            headerFile.printf(": public %s {\n", baseName);
        } else {
            headerFile.println("{");
        }

        // parse types for elements and attributes
        List<CppType> elementTypes = new ArrayList<>();
        for (XsdElement element : complexType.getElements()) {
            CppType cppType;
            XsdElement elementValue = resolveElement(element);
            if (element.getRef() == null && element.getType().getRef() == null
                    && element.getType() instanceof XsdComplexType) {
                // print inner class for anonymous types
                headerFile.printf("public:\n");
                String innerName = Utils.toClassName(getElementName(element));
                XsdComplexType innerType = (XsdComplexType) element.getType();
                printClass(innerName, nameScope + name + "::", innerType);
                headerFile.println();
                cppType = new CppComplexType(nameScope + name + "::"+ innerName);
            } else {
                cppType = parseType(elementValue.getType(), getElementName(elementValue));
            }
            elementTypes.add(cppType);
        }
        List<CppSimpleType> attributeTypes = new ArrayList<>();
        for (XsdAttribute attribute : complexType.getAttributes()) {
            XsdType type = resolveAttribute(attribute).getType();
            attributeTypes.add(parseSimpleType(type, false));
        }

        // print member variables

        headerFile.printf("private:\n");
        for (int i = 0; i < elementTypes.size(); ++i) {
            CppType type = elementTypes.get(i);
            XsdElement element = complexType.getElements().get(i);
            XsdElement elementValue = resolveElement(element);
            String typeName = element.isMultiple() || type instanceof CppComplexType ?
                    String.format("std::vector<%s>", type.getName()) : type.getName();
            headerFile.printf("%s %s;\n", typeName,
                    Utils.toVariableName(getElementName(elementValue)));
        }
        for (int i = 0; i < attributeTypes.size(); ++i) {
            CppType type = attributeTypes.get(i);
            XsdAttribute attribute = resolveAttribute(complexType.getAttributes().get(i));
            headerFile.printf("%s %s;\n", type.getName(),
                    Utils.toVariableName(attribute.getName()));
        }
        if (valueType != null) {
            headerFile.printf("%s value;\n", valueType.getName());
        }

        // print getters and setters

        headerFile.printf("public:\n");
        for (int i = 0; i < elementTypes.size(); ++i) {
            CppType type = elementTypes.get(i);
            XsdElement element = complexType.getElements().get(i);
            XsdElement elementValue = resolveElement(element);
            printGetterAndSetter(nameScope + name, type,
                    Utils.toVariableName(getElementName(elementValue)),
                    type instanceof CppComplexType ? true : element.isMultiple(),
                    type instanceof CppComplexType ? false : ((CppSimpleType)type).isList());
        }
        for (int i = 0; i < attributeTypes.size(); ++i) {
            CppType type = attributeTypes.get(i);
            XsdAttribute attribute = resolveAttribute(complexType.getAttributes().get(i));
            printGetterAndSetter(nameScope + name, type,
                    Utils.toVariableName(attribute.getName()), false, false);
        }
        if (valueType != null) {
            printGetterAndSetter(nameScope + name, valueType, "value", false, false);
        }

        printParser(name, nameScope, complexType);
        headerFile.println("};\n");
    }

    private void printParser(String name, String nameScope, XsdComplexType complexType)
            throws CppCodeGeneratorException {
        CppSimpleType baseValueType = (complexType instanceof XsdSimpleContent) ?
                getValueType((XsdSimpleContent) complexType, true) : null;
        List<XsdElement> allElements = new ArrayList<>();
        List<XsdAttribute> allAttributes = new ArrayList<>();
        stackComponents(complexType, allElements, allAttributes);

        // parse types for elements and attributes
        List<CppType> allElementTypes = new ArrayList<>();
        for (XsdElement element : allElements) {
            XsdElement elementValue = resolveElement(element);
            CppType cppType = parseType(elementValue.getType(), elementValue.getName());
            allElementTypes.add(cppType);
        }
        List<CppSimpleType> allAttributeTypes = new ArrayList<>();
        for (XsdAttribute attribute : allAttributes) {
            XsdType type = resolveAttribute(attribute).getType();
            allAttributeTypes.add(parseSimpleType(type, false));
        }

        String fullName = nameScope + name;
        headerFile.printf("static %s read(xmlNode *root);\n", fullName, Utils.lowerize(name));
        cppFile.printf("\n%s %s::read(xmlNode *root) {\n", fullName, fullName);

        cppFile.printf("%s instance;\n std::string raw;\n", fullName, fullName);

        for (int i = 0; i < allAttributes.size(); ++i) {
            CppType type = allAttributeTypes.get(i);
            XsdAttribute attribute = resolveAttribute(allAttributes.get(i));
            String variableName = Utils.toVariableName(attribute.getName());
            cppFile.printf("raw = getXmlAttribute(root, \"%s\");\n", attribute.getName());
            cppFile.printf("if (raw != \"\") {\n");
            cppFile.print(type.getParsingExpression());
            cppFile.printf("instance.set%s(value);\n}\n", Utils.capitalize(variableName));
        }

        if (baseValueType != null) {
            cppFile.printf("auto xmlValue = make_xmlUnique(xmlNodeListGetString("
                    + "root->doc, root->xmlChildrenNode, 1));\n"
                    + "if (xmlValue != nullptr) {\n"
                    + "raw = reinterpret_cast<const char*>(xmlValue.get());\n");

            cppFile.print(baseValueType.getParsingExpression());
            cppFile.printf("instance.setValue(value);\n");
            cppFile.printf("}\n");
        } else if (!allElements.isEmpty()) {
            cppFile.print("for (xmlNode *child = root->xmlChildrenNode; child != nullptr;"
                    + " child = child->next) {\n");
            for (int i = 0; i < allElements.size(); ++i) {
                CppType type = allElementTypes.get(i);
                XsdElement element = allElements.get(i);
                XsdElement elementValue = resolveElement(element);
                String variableName = Utils.toVariableName(getElementName(elementValue));
                if (i != 0) cppFile.printf("} else ");
                cppFile.printf("if (!xmlStrcmp(child->name, reinterpret_cast<const xmlChar*>");
                cppFile.printf("(\"%s\"))) {\n", elementValue.getName());

                if (type instanceof CppSimpleType) {
                    cppFile.printf("auto xmlValue = make_xmlUnique(xmlNodeListGetString(");
                    cppFile.printf("child->doc, child->xmlChildrenNode, 1));\n");
                    cppFile.printf("if (xmlValue == nullptr) {\ncontinue;\n}\n");
                    cppFile.printf("raw = reinterpret_cast<const char*>(xmlValue.get());\n");
                }

                cppFile.print(type.getParsingExpression());

                if (element.isMultiple() || type instanceof CppComplexType) {
                    cppFile.printf("instance.get%s().push_back(std::move(value));\n",
                            Utils.capitalize(variableName));
                } else {
                    cppFile.printf("instance.set%s(value);\n", Utils.capitalize(variableName));
                }
            }
            cppFile.printf("}\n}\n");
        }
        cppFile.printf("return instance;\n"
                + "}\n");
    }

    private void printGetterAndSetter(String name, CppType type, String variableName,
            boolean isMultiple, boolean isMultipleType) {
        String typeName = isMultiple ? String.format("std::vector<%s>", type.getName())
                : type.getName();

        headerFile.printf("%s& get%s();\n", typeName, Utils.capitalize(variableName));

        cppFile.println();
        cppFile.printf("%s& %s::get%s() {\n"
                + "return %s;\n}\n",
                typeName, name, Utils.capitalize(variableName), variableName);

        if (isMultiple || isMultipleType) {
            String elementTypeName = type instanceof CppComplexType ? type.getName() :
                    ((CppSimpleType)type).getTypeName();
            if (elementTypeName.equals("bool")) {
                headerFile.printf("%s getFirst%s();\n",
                        elementTypeName, Utils.capitalize(variableName));
                cppFile.println();
                cppFile.printf("%s %s::getFirst%s() {\n"
                        + "if (%s.empty()) {\n"
                        + "return false;\n"
                        + "}\n"
                        + "return %s[0];\n"
                        + "}\n",
                        elementTypeName, name, Utils.capitalize(variableName), variableName,
                        variableName);
            } else {
                headerFile.printf("%s* getFirst%s();\n",
                        elementTypeName, Utils.capitalize(variableName));
                cppFile.println();
                cppFile.printf("%s* %s::getFirst%s() {\n"
                        + "if (%s.empty()) {\n"
                        + "return nullptr;\n"
                        + "}\n"
                        + "return &%s[0];\n"
                        + "}\n",
                        elementTypeName, name, Utils.capitalize(variableName), variableName,
                        variableName);
            }
        }

        if (isMultiple) return;
        headerFile.printf("void set%s(%s);\n", Utils.capitalize(variableName), typeName);
        cppFile.println();
        cppFile.printf("void %s::set%s(%s %s) {\n"
                + "this->%s = std::move(%s);\n"
                + "}\n",
                name, Utils.capitalize(variableName), typeName, variableName,
                variableName, variableName);
    }

    private void printXmlParser() throws CppCodeGeneratorException {
        cppFile.printf("template <class T>\n"
                + "constexpr void (*xmlDeleter)(T* t);\n"
                + "template <>\nconstexpr auto xmlDeleter<xmlDoc> = xmlFreeDoc;\n"
                + "template <>\nauto xmlDeleter<xmlChar> = [](xmlChar *s) { xmlFree(s); };\n\n"
                + "template <class T>\n"
                + "constexpr auto make_xmlUnique(T *t) {\n"
                + "auto deleter = [](T *t) { xmlDeleter<T>(t); };\n"
                + "return std::unique_ptr<T, decltype(deleter)>{t, deleter};\n"
                + "}\n\n");

        if (hasAttr) {
            cppFile.printf("static std::string getXmlAttribute"
                    + "(const xmlNode *cur, const char *attribute) {\n"
                    + "auto xmlValue = make_xmlUnique(xmlGetProp(cur, "
                    + "reinterpret_cast<const xmlChar*>(attribute)));\n"
                    + "if (xmlValue == nullptr) {\n"
                    + "return \"\";\n"
                    + "}\n"
                    + "std::string value(reinterpret_cast<const char*>(xmlValue.get()));\n"
                    + "return value;\n"
                    + "}\n\n");
        }

        String className = Utils.toClassName(fileName);

        boolean isMultiRootElement = xmlSchema.getElementMap().values().size() > 1;
        for (XsdElement element : xmlSchema.getElementMap().values()) {
            CppType cppType = parseType(element.getType(), element.getName());
            String elementName = element.getName();
            String VariableName = Utils.toVariableName(elementName);
            String typeName = cppType instanceof CppSimpleType ? cppType.getName() :
                    Utils.toClassName(cppType.getName());

            headerFile.printf("std::optional<%s> read%s(const char* configFile);\n\n",
                    typeName, isMultiRootElement ? Utils.capitalize(typeName) : "");
            cppFile.printf("std::optional<%s> read%s(const char* configFile) {\n",
                    typeName, isMultiRootElement ? Utils.capitalize(typeName) : "");
            cppFile.printf("auto doc = make_xmlUnique(xmlParseFile(configFile));\n"
                    + "if (doc == nullptr) {\n"
                    + "return std::nullopt;\n"
                    + "}\n"
                    + "xmlNodePtr child = xmlDocGetRootElement(doc.get());\n"
                    + "if (child == NULL) {\n"
                    + "return std::nullopt;\n"
                    + "}\n\n"
                    + "if (!xmlStrcmp(child->name, reinterpret_cast<const xmlChar*>"
                    + "(\"%s\"))) {\n",
                    elementName);

            if (cppType instanceof CppSimpleType) {
                cppFile.printf("%s value = getXmlAttribute(child, \"%s\");\n",
                        elementName, elementName);
            } else {
                cppFile.printf(cppType.getParsingExpression());
            }
            cppFile.printf("return value;\n}\n");
            cppFile.printf("return std::nullopt;\n");
            cppFile.printf("}\n\n");
        }
    }

    private String getElementName(XsdElement element) {
        if (element instanceof XsdChoice) {
            return element.getName() + "_optional";
        } else if (element instanceof XsdAll) {
            return element.getName() + "_all";
        }
        return element.getName();
    }

    private void stackComponents(XsdComplexType complexType, List<XsdElement> elements,
            List<XsdAttribute> attributes) throws CppCodeGeneratorException {
        if (complexType.getBase() != null) {
            QName baseRef = complexType.getBase().getRef();
            if (baseRef != null && !baseRef.getNamespaceURI().equals(XsdConstants.XSD_NAMESPACE)) {
                XsdType parent = getType(baseRef.getLocalPart());
                if (parent instanceof XsdComplexType) {
                    stackComponents((XsdComplexType) parent, elements, attributes);
                }
            }
        }
        elements.addAll(complexType.getElements());
        attributes.addAll(complexType.getAttributes());
    }

    private String getBaseName(XsdComplexType complexType) throws CppCodeGeneratorException {
        if (complexType.getBase() == null) return null;
        if (complexType.getBase().getRef().getNamespaceURI().equals(XsdConstants.XSD_NAMESPACE)) {
            return null;
        }
        XsdType base = getType(complexType.getBase().getRef().getLocalPart());
        if (base instanceof XsdComplexType) {
            return Utils.toClassName(base.getName());
        }
        return null;
    }

    private CppSimpleType getValueType(XsdSimpleContent simpleContent, boolean traverse)
            throws CppCodeGeneratorException {
        assert simpleContent.getBase() != null;
        QName baseRef = simpleContent.getBase().getRef();
        assert baseRef != null;
        if (baseRef.getNamespaceURI().equals(XsdConstants.XSD_NAMESPACE)) {
            return predefinedType(baseRef.getLocalPart());
        } else {
            XsdType parent = getType(baseRef.getLocalPart());
            if (parent instanceof XsdSimpleType) {
                return parseSimpleTypeReference(baseRef, false);
            }
            if (!traverse) return null;
            if (parent instanceof XsdSimpleContent) {
                return getValueType((XsdSimpleContent) parent, true);
            } else {
                throw new CppCodeGeneratorException(
                        String.format("base not simple : %s", baseRef.getLocalPart()));
            }
        }
    }

    private CppType parseType(XsdType type, String defaultName) throws CppCodeGeneratorException {
        if (type.getRef() != null) {
            String name = type.getRef().getLocalPart();
            if (type.getRef().getNamespaceURI().equals(XsdConstants.XSD_NAMESPACE)) {
                return predefinedType(name);
            } else {
                XsdType typeValue = getType(name);
                if (typeValue instanceof XsdSimpleType) {
                    return parseSimpleTypeReference(type.getRef(), false);
                }
                return parseType(typeValue, name);
            }
        }
        if (type instanceof XsdComplexType) {
            return new CppComplexType(Utils.toClassName(defaultName));
        } else if (type instanceof XsdSimpleType) {
            return parseSimpleTypeValue((XsdSimpleType) type, false);
        } else {
            throw new CppCodeGeneratorException(
                    String.format("unknown type name : %s", defaultName));
        }
    }

    private CppSimpleType parseSimpleType(XsdType type, boolean traverse)
            throws CppCodeGeneratorException {
        if (type.getRef() != null) {
            return parseSimpleTypeReference(type.getRef(), traverse);
        } else {
            return parseSimpleTypeValue((XsdSimpleType) type, traverse);
        }
    }

    private CppSimpleType parseSimpleTypeReference(QName typeRef, boolean traverse)
            throws CppCodeGeneratorException {
        assert typeRef != null;
        String typeName = typeRef.getLocalPart();
        if (typeRef.getNamespaceURI().equals(XsdConstants.XSD_NAMESPACE)) {
            return predefinedType(typeName);
        }
        if (cppSimpleTypeMap.containsKey(typeName)) {
            return cppSimpleTypeMap.get(typeName);
        } else if (traverse) {
            XsdSimpleType simpleType = getSimpleType(typeName);
            CppSimpleType ret = parseSimpleTypeValue(simpleType, true);
            cppSimpleTypeMap.put(typeName, ret);
            return ret;
        } else {
            throw new CppCodeGeneratorException(String.format("unknown type name : %s", typeName));
        }
    }

    private CppSimpleType parseSimpleTypeValue(XsdSimpleType simpleType, boolean traverse)
            throws CppCodeGeneratorException {
        if (simpleType instanceof XsdList) {
            XsdList list = (XsdList) simpleType;
            return parseSimpleType(list.getItemType(), traverse).newListType();
        } else if (simpleType instanceof XsdRestriction) {
            // we don't consider any restrictions.
            XsdRestriction restriction = (XsdRestriction) simpleType;
            if (restriction.getEnums() != null) {
                String name = Utils.toClassName(restriction.getName());
                return new CppSimpleType(name, "stringTo" + name + "(%s)", false);
            }
            return parseSimpleType(restriction.getBase(), traverse);
        } else if (simpleType instanceof XsdUnion) {
            // unions are almost always interpreted as java.lang.String
            // Exceptionally, if any of member types of union are 'list', then we interpret it as
            // List<String>
            XsdUnion union = (XsdUnion) simpleType;
            for (XsdType memberType : union.getMemberTypes()) {
                if (parseSimpleType(memberType, traverse).isList()) {
                    return new CppSimpleType("std::string", "%s", true);
                }
            }
            return new CppSimpleType("std::string", "%s", false);
        } else {
            // unreachable
            throw new IllegalStateException("unknown simple type");
        }
    }

    private XsdElement resolveElement(XsdElement element) throws CppCodeGeneratorException {
        if (element.getRef() == null) return element;
        String name = element.getRef().getLocalPart();
        XsdElement ret = xmlSchema.getElementMap().get(name);
        if (ret != null) return ret;
        throw new CppCodeGeneratorException(String.format("no element named : %s", name));
    }

    private XsdAttribute resolveAttribute(XsdAttribute attribute)
            throws CppCodeGeneratorException {
        if (attribute.getRef() == null) return attribute;
        String name = attribute.getRef().getLocalPart();
        XsdAttribute ret = xmlSchema.getAttributeMap().get(name);
        if (ret != null) return ret;
        throw new CppCodeGeneratorException(String.format("no attribute named : %s", name));
    }

    private XsdType getType(String name) throws CppCodeGeneratorException {
        XsdType type = xmlSchema.getTypeMap().get(name);
        if (type != null) return type;
        throw new CppCodeGeneratorException(String.format("no type named : %s", name));
    }

    private XsdSimpleType getSimpleType(String name) throws CppCodeGeneratorException {
        XsdType type = getType(name);
        if (type instanceof XsdSimpleType) return (XsdSimpleType) type;
        throw new CppCodeGeneratorException(String.format("not a simple type : %s", name));
    }

    private boolean hasAttribute(XsdComplexType complexType) throws CppCodeGeneratorException {
        if (complexType.getAttributes().size() > 0) {
            return true;
        }
        boolean results = false;
        for (XsdElement element : complexType.getElements()) {
            XsdElement elementValue = resolveElement(element);
            if (element.getRef() == null && element.getType().getRef() == null
                    && element.getType() instanceof XsdComplexType) {
                results = hasAttribute((XsdComplexType) element.getType());
                if (results) {
                    return results;
                }
            }
        }
        return results;
    }

    private static CppSimpleType predefinedType(String name) throws CppCodeGeneratorException {
        switch (name) {
            case "string":
            case "token":
            case "normalizedString":
            case "language":
            case "ENTITY":
            case "ID":
            case "Name":
            case "NCName":
            case "NMTOKEN":
            case "anyURI":
            case "anyType":
            case "QName":
            case "NOTATION":
            case "IDREF":
                return new CppSimpleType("std::string", "%s", false);
            case "ENTITIES":
            case "NMTOKENS":
            case "IDREFS":
                return new CppSimpleType("std::string", "%s", true);
            case "date":
            case "dateTime":
            case "time":
            case "gDay":
            case "gMonth":
            case "gYear":
            case "gMonthDay":
            case "gYearMonth":
            case "duration":
                return new CppSimpleType("std::string", "%s", false);
            case "decimal":
                return new CppSimpleType("double", "std::stod(%s)", false);
            case "integer":
            case "negativeInteger":
            case "nonNegativeInteger":
            case "positiveInteger":
            case "nonPositiveInteger":
                return new CppSimpleType("long long", "std::stoll(%s)", false);
            case "unsignedLong":
                return new CppSimpleType("unsigned long long", "std::stoull(%s)", false);
            case "long":
                return new CppSimpleType("long long", "std::stoll(%s)", false);
            case "unsignedInt":
                return new CppSimpleType("unsigned int",
                        "static_cast<unsigned int>(stoul(%s))", false);
            case "int":
                return new CppSimpleType("int", "std::stoi(%s)", false);
            case "unsignedShort":
                return new CppSimpleType("unsigned short",
                        "static_cast<unsigned short>(std::stoi(%s))", false);
            case "short":
                return new CppSimpleType("short", "static_cast<short>(std::stoi(%s))", false);
            case "unsignedByte":
                return new CppSimpleType("unsigned char",
                        "static_cast<unsigned char>(std::stoi(%s))", false);
            case "byte":
                return new CppSimpleType("char", "static_cast<char>(std::stoi(%s))", false);
            case "boolean":
                return new CppSimpleType("bool", "%s == \"true\"", false);
            case "double":
                return new CppSimpleType("double", "std::stod(%s)", false);
            case "float":
                return new CppSimpleType("float", "std::stof(%s)", false);
            case "base64Binary":
            case "hexBinary":
                return new CppSimpleType("std::string", "%s", false);
        }
        throw new CppCodeGeneratorException("unknown xsd predefined type : " + name);
    }
}
