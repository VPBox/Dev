# ConfigFile as API

The ConfigFile as API is a formal Treble interface describing schemas of
configuration files used across system and vendor partitions.
The Java APIs in the current.txt file are not Java APIs for apps. It's a proxy
for the schema of a xml file used between the system and vendor partition.
The xml files are only ever parsed by apps on the system partition.

## Add Schema
Add the schema (attribute, element or new complexType …) you want to add to the
xsd file.

#### before
```xml
<xs:element name="class">
  <xs:complexType>
    <xs:sequence>
      <xs:element name="student" type="xs:string"/>
    </xs:sequence>
    <xs:attribute name="name" type=”xs:string”/>
  </xs:complexType>
</xs:element>
```

#### after
```xml
<xs:element name="class">
  <xs:complexType>
    <xs:sequence>
      <xs:element name="student" type="xs:string"/>
    </xs:sequence>
    <xs:attribute name="name" type=”xs:string”/>
    <xs:attribute name="number" type="xs:int"/>
  </xs:complexType>
</xs:element>
```

Then run "make {xsd_config module_name} .docs-update-current-api" or "make
update-api" to update all the xsd_config modules.

In the above example, two functions are added as below.
* method public int getNumber();
* method public void setNumber(int);

## Remove Schema
To remove a tag, add an annotation tag with the name of "Deprecated" into the
tag to be deleted. Partners are not allowed to create new vendor images using
deprecated tags

#### before
```xml
<xs:element name="class">
  <xs:complexType>
    <xs:sequence>
      <xs:element name="student" type="xs:string"/>
    </xs:sequence>
    <xs:attribute name="name" type=”xs:string”/>
  </xs:complexType>
</xs:element>
```

#### after
```xml
<xs:element name="class">
  <xs:complexType>
    <xs:sequence>
      <xs:element name="student" type="xs:string">
        <annotation name=”Deprecated”/>
      </xs:element>
    </xs:sequence>
    <xs:attribute name="name" type=”xs:string”/>
  </xs:complexType>
</xs:element>
```

After adding “Deprecated” annotation, we need to update the api or schema just
like when adding a tag. In the above example, a @Deprecate annotation is added.
* method @Deprecated public java.util.List<java.lang.String> getStudent();
After 2 years we can delete the tag. When deleting, delete the tag in the xsd
file and the api in last_current.txt, and update it.

## Release Schema
If there are any changes, we update last_current.txt and last_removed.txt before
release by copying current.txt and removed.txt to last_current.txt and
last_removed.txt.

## Supported/Unsupported Tags
Only the follow tags and attributes are allowed:

#### Supported
```xml
"xsd:schema" [
]

"xsd:element" [
    "xsd:name" {
        values: any valid java name
    }
    "xsd:type" {
        values: built-in data type, simpleType or complexType
    },
    "xsd:ref" {
        values: another element
    },
    "xsd:minOccurs" {
        values: [ 0, maxOccurs ]
    },
    "xsd:maxOccurs" {
        values: [ 1, unbounded ]
    },
]

"xsd:attribute" [
    "xsd:name" {
        values: any valid java name
    }
    "xsd:type" {
        values: built-in data type, simpleType or complexType
    },
    "xsd:ref" {
        values: another element
    },
]

"xsd:complexType" [
    "xsd:name" {
        values: any valid java name
    }
]

"xsd:complexContent" [
]

"xsd:simpleContent" [
]

"xsd:restriction": [
    "xsd:base" {
        values: built-in data type
    }
]

"xsd:extension": [
    "xsd:base" {
        values: built-in data type, simpleType or complexType
    }
]

"xsd:simpleType": [
    "xsd:name" {
        values: any valid java name
    }
]

"xsd:list": [
    "xsd:itemType" {
        values: built-in data type, or simpleType
    }
]

"xsd:union": [
    "xsd:memberTypes" {
        values: built-in data type, or simpleType
    }
]

"xsd:sequence": [
]

"xsd:choice": [
]

"xsd:all": [
]

"xsd:enumeration": [
    "xsd:value" {
        values: built-in data type
    }
]
```
