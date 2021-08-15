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

package com.android.xsdc;

import com.android.xsdc.tag.*;

import org.xml.sax.Attributes;
import org.xml.sax.Locator;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Stack;
import java.util.stream.Collectors;

import javax.xml.namespace.QName;

public class XsdHandler extends DefaultHandler {
    private static class State {
        final String name;
        final Map<String, String> attributeMap;
        final List<XsdTag> tags;
        boolean deprecated;

        State(String name, Map<String, String> attributeMap) {
            this.name = name;
            this.attributeMap = Collections.unmodifiableMap(attributeMap);
            tags = new ArrayList<>();
            deprecated = false;
        }
    }

    private XmlSchema schema;
    private final Stack<State> stateStack;
    private final Map<String, String> namespaces;
    private Locator locator;
    private boolean documentationFlag;
    private boolean enumerationFlag;

    public XsdHandler() {
        stateStack = new Stack<>();
        namespaces = new HashMap<>();
        documentationFlag = false;
        enumerationFlag = false;
    }

    public XmlSchema getSchema() {
        return schema;
    }

    @Override
    public void setDocumentLocator(Locator locator) {
        this.locator = locator;
    }

    @Override
    public void startPrefixMapping(String prefix, String uri) {
        namespaces.put(prefix, uri);
    }

    @Override
    public void endPrefixMapping(String prefix) {
        namespaces.remove(prefix);
    }

    private QName parseQName(String str) throws XsdParserException {
        if (str == null) return null;
        String[] parsed = str.split(":");
        if (parsed.length == 2) {
            return new QName(namespaces.get(parsed[0]), parsed[1]);
        } else if (parsed.length == 1) {
            return new QName(null, str);
        }
        throw new XsdParserException(String.format("QName parse error : %s", str));
    }

    private List<QName> parseQNames(String str) throws XsdParserException {
        List<QName> qNames = new ArrayList<>();
        if (str == null) return qNames;
        String[] parsed = str.split("\\s+");
        for (String s : parsed) {
            qNames.add(parseQName(s));
        }
        return qNames;
    }

    @Override
    public void startElement(
            String uri, String localName, String qName, Attributes attributes) {
        // we need to copy attributes because it is mutable..
        Map<String, String> attributeMap = new HashMap<>();
        for (int i = 0; i < attributes.getLength(); ++i) {
            attributeMap.put(attributes.getLocalName(i), attributes.getValue(i));
        }
        if (!documentationFlag) {
            stateStack.push(new State(localName, attributeMap));
        }
        if (localName == "documentation") {
            documentationFlag = true;
        }
    }

    @Override
    public void endElement(String uri, String localName, String qName) throws SAXException {
        if (documentationFlag && localName != "documentation") {
            return;
        }
        try {
            State state = stateStack.pop();
            switch (state.name) {
                case "schema":
                    schema = makeSchema(state);
                    break;
                case "element":
                    stateStack.peek().tags.add(makeElement(state));
                    break;
                case "attribute":
                    stateStack.peek().tags.add(makeAttribute(state));
                    break;
                case "complexType":
                    stateStack.peek().tags.add(makeComplexType(state));
                    break;
                case "complexContent":
                    stateStack.peek().tags.add(makeComplexContent(state));
                    break;
                case "simpleContent":
                    stateStack.peek().tags.add(makeSimpleContent(state));
                    break;
                case "restriction":
                    if (enumerationFlag) {
                        stateStack.peek().tags.add(makeEnumRestriction(state));
                        enumerationFlag = false;
                    } else {
                        stateStack.peek().tags.add(makeGeneralRestriction(state));
                    }
                    break;
                case "extension":
                    stateStack.peek().tags.add(makeGeneralExtension(state));
                    break;
                case "simpleType":
                    stateStack.peek().tags.add(makeSimpleType(state));
                    break;
                case "list":
                    stateStack.peek().tags.add(makeSimpleTypeList(state));
                    break;
                case "union":
                    stateStack.peek().tags.add(makeSimpleTypeUnion(state));
                    break;
                case "sequence":
                    stateStack.peek().tags.addAll(makeSequence(state));
                    break;
                case "choice":
                    stateStack.peek().tags.addAll(makeChoice(state));
                    break;
                case "all":
                    stateStack.peek().tags.addAll(makeAll(state));
                    break;
                case "enumeration":
                    stateStack.peek().tags.add(makeEnumeration(state));
                    enumerationFlag = true;
                    break;
                case "fractionDigits":
                case "length":
                case "maxExclusive":
                case "maxInclusive":
                case "maxLength":
                case "minExclusive":
                case "minInclusive":
                case "minLength":
                case "pattern":
                case "totalDigits":
                case "whiteSpace":
                    // Tags under simpleType <restriction>. They are ignored.
                    break;
                case "annotation":
                    stateStack.peek().deprecated = isDeprecated(state.attributeMap, state.tags);
                    break;
                case "appinfo":
                    // They function like comments, so are ignored.
                    break;
                case "documentation":
                    documentationFlag = false;
                    break;
                case "key":
                case "keyref":
                case "selector":
                case "field":
                case "unique":
                    // These tags are not related to xml parsing.
                    // They are using when validating xml files via xsd file.
                    // So they are ignored.
                    break;
                default:
                    throw new XsdParserException(String.format("unsupported tag : %s", state.name));
            }
        } catch (XsdParserException e) {
            throw new SAXException(
                    String.format("Line %d, Column %d - %s",
                            locator.getLineNumber(), locator.getColumnNumber(), e.getMessage()));
        }
    }

    private XmlSchema makeSchema(State state) {
        Map<String, XsdElement> elementMap = new LinkedHashMap<>();
        Map<String, XsdType> typeMap = new LinkedHashMap<>();
        Map<String, XsdAttribute> attrMap = new LinkedHashMap<>();

        for (XsdTag tag : state.tags) {
            if (tag == null) continue;
            if (tag instanceof XsdElement) {
                elementMap.put(tag.getName(), (XsdElement) tag);
            } else if (tag instanceof XsdAttribute) {
                attrMap.put(tag.getName(), (XsdAttribute) tag);
            } else if (tag instanceof XsdType) {
                typeMap.put(tag.getName(), (XsdType) tag);
            }
        }

        return new XmlSchema(elementMap, typeMap, attrMap);
    }

    private XsdElement makeElement(State state) throws XsdParserException {
        String name = state.attributeMap.get("name");
        QName typename = parseQName(state.attributeMap.get("type"));
        QName ref = parseQName(state.attributeMap.get("ref"));
        String isAbstract = state.attributeMap.get("abstract");
        String defVal = state.attributeMap.get("default");
        String substitutionGroup = state.attributeMap.get("substitutionGroup");
        String maxOccurs = state.attributeMap.get("maxOccurs");

        if ("true".equals(isAbstract)) {
            throw new XsdParserException("abstract element is not supported.");
        }
        if (defVal != null) {
            throw new XsdParserException("default value of an element is not supported.");
        }
        if (substitutionGroup != null) {
            throw new XsdParserException("substitution group of an element is not supported.");
        }

        boolean multiple = false;
        if (maxOccurs != null) {
            if (maxOccurs.equals("0")) return null;
            if (maxOccurs.equals("unbounded") || Integer.parseInt(maxOccurs) > 1) multiple = true;
        }

        XsdType type = null;
        if (typename != null) {
            type = new XsdType(null, typename);
        }
        for (XsdTag tag : state.tags) {
            if (tag == null) continue;
            if (tag instanceof XsdType) {
                type = (XsdType) tag;
            }
        }

        return setDeprecated(new XsdElement(name, ref, type, multiple), state.deprecated);
    }

    private XsdAttribute makeAttribute(State state) throws XsdParserException {
        String name = state.attributeMap.get("name");
        QName typename = parseQName(state.attributeMap.get("type"));
        QName ref = parseQName(state.attributeMap.get("ref"));
        String defVal = state.attributeMap.get("default");
        String use = state.attributeMap.get("use");

        if (use != null && use.equals("prohibited")) return null;

        XsdType type = null;
        if (typename != null) {
            type = new XsdType(null, typename);
        }
        for (XsdTag tag : state.tags) {
            if (tag == null) continue;
            if (tag instanceof XsdType) {
                type = (XsdType) tag;
            }
        }

        return setDeprecated(new XsdAttribute(name, ref, type), state.deprecated);
    }

    private XsdComplexType makeComplexType(State state) throws XsdParserException {
        String name = state.attributeMap.get("name");
        String isAbstract = state.attributeMap.get("abstract");
        String mixed = state.attributeMap.get("mixed");

        if ("true".equals(isAbstract)) {
            throw new XsdParserException("abstract complex type is not supported.");
        }
        if ("true".equals(mixed)) {
            throw new XsdParserException("mixed option of a complex type is not supported.");
        }

        List<XsdAttribute> attributes = new ArrayList<>();
        List<XsdElement> elements = new ArrayList<>();
        XsdComplexType type = null;

        for (XsdTag tag : state.tags) {
            if (tag == null) continue;
            if (tag instanceof XsdAttribute) {
                attributes.add((XsdAttribute) tag);
            } else if (tag instanceof XsdElement) {
                elements.add((XsdElement) tag);
            } else if (tag instanceof XsdComplexContent) {
                XsdComplexContent child = (XsdComplexContent) tag;
                type = setDeprecated(new XsdComplexContent(name, child.getBase(),
                        child.getAttributes(), child.getElements()), state.deprecated);
            } else if (tag instanceof XsdSimpleContent) {
                XsdSimpleContent child = (XsdSimpleContent) tag;
                type = setDeprecated(new XsdSimpleContent(name, child.getBase(),
                        child.getAttributes()), state.deprecated);
            }
        }

        return (type != null) ? type : setDeprecated(new XsdComplexContent(name, null, attributes,
                elements), state.deprecated);
    }

    private XsdComplexContent makeComplexContent(State state) throws XsdParserException {
        String mixed = state.attributeMap.get("mixed");
        if ("true".equals(mixed)) {
            throw new XsdParserException("mixed option of a complex content is not supported.");
        }

        XsdComplexContent content = null;
        for (XsdTag tag : state.tags) {
            if (tag == null) continue;
            if (tag instanceof XsdGeneralExtension) {
                XsdGeneralExtension extension = (XsdGeneralExtension) tag;
                content = new XsdComplexContent(null, extension.getBase(),
                        extension.getAttributes(), extension.getElements());
            } else if (tag instanceof XsdGeneralRestriction) {
                XsdGeneralRestriction restriction = (XsdGeneralRestriction) tag;
                XsdType base = restriction.getBase();
                if (base.getRef() != null && base.getRef().getNamespaceURI().equals(
                        XsdConstants.XSD_NAMESPACE)) {
                    // restriction of base 'xsd:anyType' is equal to complex content definition
                    content = new XsdComplexContent(null, null, restriction.getAttributes(),
                            restriction.getElements());
                } else {
                    // otherwise ignore restrictions
                    content = new XsdComplexContent(null, base, null, null);
                }
            }
        }

        return setDeprecated(content, state.deprecated);
    }

    private XsdSimpleContent makeSimpleContent(State state) {
        XsdSimpleContent content = null;

        for (XsdTag tag : state.tags) {
            if (tag == null) continue;
            if (tag instanceof XsdGeneralExtension) {
                XsdGeneralExtension extension = (XsdGeneralExtension) tag;
                content = new XsdSimpleContent(null, extension.getBase(),
                        extension.getAttributes());
            } else if (tag instanceof XsdGeneralRestriction) {
                XsdGeneralRestriction restriction = (XsdGeneralRestriction) tag;
                content = new XsdSimpleContent(null, restriction.getBase(), null);
            }
        }

        return setDeprecated(content, state.deprecated);
    }

    private XsdGeneralRestriction makeGeneralRestriction(State state) throws XsdParserException {
        QName base = parseQName(state.attributeMap.get("base"));

        XsdType type = null;
        if (base != null) {
            type = new XsdType(null, base);
        }
        List<XsdAttribute> attributes = new ArrayList<>();
        List<XsdElement> elements = new ArrayList<>();
        for (XsdTag tag : state.tags) {
            if (tag == null) continue;
            if (tag instanceof XsdAttribute) {
                attributes.add((XsdAttribute) tag);
            } else if (tag instanceof XsdElement) {
                elements.add((XsdElement) tag);
            }
        }

        return setDeprecated(new XsdGeneralRestriction(type, attributes, elements),
                state.deprecated);
    }

    private XsdGeneralExtension makeGeneralExtension(State state) throws XsdParserException {
        QName base = parseQName(state.attributeMap.get("base"));

        List<XsdAttribute> attributes = new ArrayList<>();
        List<XsdElement> elements = new ArrayList<>();
        for (XsdTag tag : state.tags) {
            if (tag == null) continue;
            if (tag instanceof XsdAttribute) {
                attributes.add((XsdAttribute) tag);
            } else if (tag instanceof XsdElement) {
                elements.add((XsdElement) tag);
            }
        }
        return setDeprecated(new XsdGeneralExtension(new XsdType(null, base), attributes, elements),
                state.deprecated);
    }

    private XsdSimpleType makeSimpleType(State state) throws XsdParserException {
        String name = state.attributeMap.get("name");
        XsdSimpleType type = null;
        for (XsdTag tag : state.tags) {
            if (tag == null) continue;
            if (tag instanceof XsdList) {
                type = new XsdList(name, ((XsdList) tag).getItemType());
            } else if (tag instanceof XsdGeneralRestriction) {
                type = new XsdRestriction(name, ((XsdGeneralRestriction) tag).getBase(), null);
            } else if (tag instanceof XsdEnumRestriction) {
                type = new XsdRestriction(name, ((XsdEnumRestriction) tag).getBase(),
                        ((XsdEnumRestriction) tag).getEnums());
            } else if (tag instanceof XsdUnion) {
                type = new XsdUnion(name, ((XsdUnion) tag).getMemberTypes());
            }
        }
        return setDeprecated(type, state.deprecated);
    }

    private XsdList makeSimpleTypeList(State state) throws XsdParserException {
        QName itemTypeName = parseQName(state.attributeMap.get("itemType"));

        XsdType itemType = null;
        if (itemTypeName != null) {
            itemType = new XsdType(null, itemTypeName);
        }
        for (XsdTag tag : state.tags) {
            if (tag == null) continue;
            if (tag instanceof XsdType) {
                itemType = (XsdType) tag;
            }
        }
        return setDeprecated(new XsdList(null, itemType), state.deprecated);
    }

    private XsdUnion makeSimpleTypeUnion(State state) throws XsdParserException {
        List<QName> memberTypeNames = parseQNames(state.attributeMap.get("memberTypes"));
        List<XsdType> memberTypes = memberTypeNames.stream().map(
                ref -> new XsdType(null, ref)).collect(Collectors.toList());

        for (XsdTag tag : state.tags) {
            if (tag == null) continue;
            if (tag instanceof XsdType) {
                memberTypes.add((XsdType) tag);
            }
        }

        return setDeprecated(new XsdUnion(null, memberTypes), state.deprecated);
    }

    private static List<XsdElement> makeSequence(State state) throws XsdParserException {
        String minOccurs = state.attributeMap.get("minOccurs");
        String maxOccurs = state.attributeMap.get("maxOccurs");

        if (minOccurs != null || maxOccurs != null) {
            throw new XsdParserException(
                    "minOccurs, maxOccurs options of a sequence is not supported");
        }

        List<XsdElement> elements = new ArrayList<>();
        for (XsdTag tag : state.tags) {
            if (tag == null) continue;
            if (tag instanceof XsdElement) {
                if (maxOccurs != null && (maxOccurs.equals("unbounded")
                        || Integer.parseInt(maxOccurs) > 1)) {
                    ((XsdElement)tag).setMultiple(true);
                }
                elements.add((XsdElement) tag);
            }
        }
        return elements;
    }

    private static List<XsdElement> makeChoice(State state) throws XsdParserException {
        String maxOccurs = state.attributeMap.get("maxOccurs");
        List<XsdElement> elements = new ArrayList<>();

        for (XsdTag tag : state.tags) {
            if (tag == null) continue;
            if (tag instanceof XsdElement) {
                if (maxOccurs != null && (maxOccurs.equals("unbounded")
                        || Integer.parseInt(maxOccurs) > 1)) {
                    ((XsdElement)tag).setMultiple(true);
                }
                XsdElement element = (XsdElement)tag;
                elements.add(setDeprecated(new XsdChoice(element.getName(), element.getRef(),
                        element.getType(), element.isMultiple()), element.isDeprecated()));
            }
        }
        return elements;
    }

    private static List<XsdElement> makeAll(State state) throws XsdParserException {
        List<XsdElement> elements = new ArrayList<>();
        for (XsdTag tag : state.tags) {
            if (tag == null) continue;
            if (tag instanceof XsdElement) {
                XsdElement element = (XsdElement)tag;
                elements.add(setDeprecated(new XsdAll(element.getName(), element.getRef(),
                        element.getType(), element.isMultiple()), element.isDeprecated()));
            }
        }
        return elements;
    }

    private XsdEnumeration makeEnumeration(State state) throws XsdParserException {
        String value = state.attributeMap.get("value");
        return setDeprecated(new XsdEnumeration(value), state.deprecated);
    }

    private XsdEnumRestriction makeEnumRestriction(State state) throws XsdParserException {
        QName base = parseQName(state.attributeMap.get("base"));

        XsdType type = null;
        if (base != null) {
            type = new XsdType(null, base);
        }
        List<XsdEnumeration> enums = new ArrayList<>();
        for (XsdTag tag : state.tags) {
            if (tag == null) continue;
            if (tag instanceof XsdEnumeration) {
                enums.add((XsdEnumeration) tag);
            }
        }

        return setDeprecated(new XsdEnumRestriction(type, enums), state.deprecated);
    }

    private boolean isDeprecated(Map<String, String> attributeMap,List<XsdTag> tags)
            throws XsdParserException {
        String name = attributeMap.get("name");
        if ("Deprecated".equals(name)) {
            return true;
        }
        return false;
    }

    private static <T extends XsdTag> T setDeprecated(T tag, boolean deprecated) {
        if (tag != null) {
            tag.setDeprecated(deprecated);
        }
        return tag;
    }
}
