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

#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include "nested_type.h"
#include "purchase_simple.h"
#include "simple_complex_content.h"

#include "predefined_types.h"
#include "reference.h"
#include "simple_type.h"

using namespace std;

class XmlTest : public ::testing::Test {
public:
    virtual void SetUp() override {
    }
    virtual void TearDown() override {
    }
};

TEST_F(XmlTest, Simpletype) {
  using namespace simple::type;
  string file_name = "resources/simple_type.xml";
  SimpleTypes simple = *readSimpleTypes(file_name.c_str());

  for (int i = 0; i < simple.getListInt().size(); ++i) {
    EXPECT_EQ(simple.getListInt()[i], i + 1);
  }
  EXPECT_EQ(*simple.getFirstUnionTest(), "100");
  EXPECT_EQ(simple.getYesOrNo(), EnumType::YES);
}

TEST_F(XmlTest, Predefinedtypes) {
  using namespace predefined::types;
  Types type = *read("resources/predefined_types.xml");

  StringTypes stringTypes = *type.getFirstStringTypes();
  DateTypes dateTypes = *type.getFirstDateTypes();
  NumericTypes numericTypes = *type.getFirstNumericTypes();
  MiscTypes miscTypes = *type.getFirstMiscTypes();
  ListPrimitiveTypes listPrimitiveTypes = *type.getFirstListPrimitiveTypes();

  EXPECT_EQ(stringTypes.getString(), "abcd");
  EXPECT_EQ(stringTypes.getToken(), "abcd");
  EXPECT_EQ(stringTypes.getNormalizedString(), "abcd");
  EXPECT_EQ(stringTypes.getLanguage(), "abcd");
  EXPECT_EQ(stringTypes.getEntity(), "abcd");
  EXPECT_EQ(stringTypes.getEntities()[0], "a");
  EXPECT_EQ(stringTypes.getEntities()[1], "b");
  EXPECT_EQ(stringTypes.getEntities()[2], "c");
  EXPECT_EQ(stringTypes.getEntities()[3], "d");
  EXPECT_EQ(stringTypes.getId(), "abcd");
  EXPECT_EQ(stringTypes.getName(), "abcd");
  EXPECT_EQ(stringTypes.getNcname(), "abcd");
  EXPECT_EQ(stringTypes.getNmtoken(), "abcd");
  EXPECT_EQ(stringTypes.getNmtokens()[0], "a");
  EXPECT_EQ(stringTypes.getNmtokens()[1], "b");
  EXPECT_EQ(stringTypes.getNmtokens()[2], "c");
  EXPECT_EQ(stringTypes.getNmtokens()[3], "d");


  EXPECT_EQ(dateTypes.getDate(), "2018-06-18");
  EXPECT_EQ(dateTypes.getDateTime(), "2018-06-18T21:32:52");
  EXPECT_EQ(dateTypes.getDuration(), "P3M");
  EXPECT_EQ(dateTypes.getGDay(), "---18");
  EXPECT_EQ(dateTypes.getGMonth(), "--06");
  EXPECT_EQ(dateTypes.getGMonthDay(), "--06-18");
  EXPECT_EQ(dateTypes.getGYear(), "2018");
  EXPECT_EQ(dateTypes.getGYearMonth(), "2018-06");
  EXPECT_EQ(dateTypes.getTime(), "21:32:52");

  EXPECT_EQ(numericTypes.getDecimal(), 1234.57);
  EXPECT_EQ(numericTypes.getInteger(), 1234567890123456789);
  EXPECT_EQ(numericTypes.get_long(), 9223372036854775807);
  EXPECT_EQ(numericTypes.get_int(), 2147483647);
  EXPECT_EQ(numericTypes.get_short(), 32767);
  EXPECT_EQ((int)numericTypes.getByte(), 127);
  EXPECT_EQ(numericTypes.getNegativeInteger(), -1234);
  EXPECT_EQ(numericTypes.getNonNegativeInteger(), 1234);
  EXPECT_EQ(numericTypes.getPositiveInteger(), 1234);
  EXPECT_EQ(numericTypes.getNonPositiveInteger(), -1234);
  EXPECT_EQ(numericTypes.getUnsignedLong(), 1234);
  EXPECT_EQ(numericTypes.getUnsignedInt(), 1234);
  EXPECT_EQ(numericTypes.getUnsignedShort(), 1234);
  EXPECT_EQ((int)(numericTypes.getUnsignedByte()), 255);

  EXPECT_EQ(miscTypes.get_double(), 1234.57);
  EXPECT_EQ(miscTypes.getAnyURI(), "https://www.google.com");
  EXPECT_EQ(miscTypes.getBase64Binary(), "Z29vZ2xl");
  EXPECT_TRUE(miscTypes.getBoolean());
  EXPECT_EQ(miscTypes.getHexBinary(), "516a75cb56d7e7");
  EXPECT_EQ(miscTypes.getQName(), "abcd");
  EXPECT_EQ(miscTypes.getIDREF(), "abcd");
  EXPECT_EQ(miscTypes.getIDREFS()[0], "abcd");
  EXPECT_EQ(miscTypes.getIDREFS()[1], "abcd");
  EXPECT_EQ(miscTypes.getAnyType(), "abcd");

  EXPECT_EQ(listPrimitiveTypes.getListInt()[0], -2147483648);
  EXPECT_EQ(listPrimitiveTypes.getListInt()[1], 2147483647);
  EXPECT_EQ(listPrimitiveTypes.getListShort()[0], -32768);
  EXPECT_EQ(listPrimitiveTypes.getListShort()[1], 32767);
  EXPECT_EQ((int)listPrimitiveTypes.getListByte()[0], -128);
  EXPECT_EQ((int)listPrimitiveTypes.getListByte()[1], 127);
  EXPECT_EQ(listPrimitiveTypes.getListDouble()[0], 1234.56);
  EXPECT_EQ(listPrimitiveTypes.getListDouble()[1], 5678.12);
  EXPECT_TRUE(listPrimitiveTypes.getListBoolean()[0]);
  EXPECT_FALSE(listPrimitiveTypes.getListBoolean()[1]);

}

TEST_F(XmlTest, Nestedtype) {
  using namespace nested::type;
  Employee employee = *read("resources/nested_type.xml");

  Employee::Address address = *employee.getFirstAddress();
  Employee::Address::Extra extra = *address.getFirstExtra();

  EXPECT_EQ((int)employee.getId(), 1);
  EXPECT_EQ(employee.getName(), "Peter");
  EXPECT_EQ(address.getCountry(), "US");
  EXPECT_EQ(address.getState(), "Mountain View");
  EXPECT_EQ(address.getZip(), 3342);
  EXPECT_EQ(extra.getLine1(), "Donga 303-111");
  EXPECT_EQ(extra.getLine2(), "Good Street");
}

TEST_F(XmlTest, Purchasesimple) {
  using namespace purchase::simple;
  PurchaseOrderType orderType = *read("resources/purchase_simple.xml");

  EXPECT_EQ(orderType.getOrderDate(), "1900-01-01");

  EXPECT_EQ(orderType.getShipTo()[0].getName(), "name1");
  EXPECT_EQ(orderType.getShipTo()[0].getStreet(), "street1");
  EXPECT_EQ(orderType.getShipTo()[0].getCity(), "city1");
  EXPECT_EQ(orderType.getShipTo()[0].getState(), "state1");
  EXPECT_EQ(orderType.getShipTo()[0].getZip(), 1);
  EXPECT_EQ(orderType.getShipTo()[0].getCountry(), "US");
  EXPECT_EQ(orderType.getShipTo()[1].getName(), "name2");
  EXPECT_EQ(orderType.getShipTo()[1].getStreet(), "street2");
  EXPECT_EQ(orderType.getShipTo()[1].getCity(), "city2");
  EXPECT_EQ(orderType.getShipTo()[1].getState(), "state2");
  EXPECT_EQ(orderType.getShipTo()[1].getZip(), -7922816251426433759);
  EXPECT_EQ(orderType.getShipTo()[1].getCountry(), "US");

  EXPECT_EQ(orderType.getBillTo()[0].getName(), "billName");
  EXPECT_EQ(orderType.getBillTo()[0].getStreet(), "billStreet");
  EXPECT_EQ(orderType.getBillTo()[0].getCity(), "billCity");
  EXPECT_EQ(orderType.getBillTo()[0].getState(), "billState");
  EXPECT_EQ(orderType.getBillTo()[0].getZip(), 1);
  EXPECT_EQ(orderType.getBillTo()[0].getCountry(), "US");
}

TEST_F(XmlTest, Reference) {
  using namespace reference;
  Class _class = *read("resources/reference.xml");

  EXPECT_EQ(_class.getStudent()[0], "Sam");
  EXPECT_EQ(_class.getStudent()[1], "Paul");
  EXPECT_EQ(_class.getStudent()[2], "Peter");
}

TEST_F(XmlTest, Simplecomplexcontent) {
  using namespace simple::complex::content;
  Person person = *readPerson("resources/simple_complex_content.xml");
  USAddressP uSAddressP = *person.getFirstUSAddressP();
  KRAddress kRAddress = *person.getFirstKRAddress();
  SubAddress subAddress = *person.getFirstSubAddress();

  EXPECT_EQ(person.getName(), "Petr");

  EXPECT_EQ(uSAddressP.getName(), "404");
  EXPECT_EQ(uSAddressP.getStreet(), "street fighter");
  EXPECT_EQ(uSAddressP.getCity(), "New York");
  EXPECT_EQ(uSAddressP.getState(), "Washington");
  EXPECT_EQ(uSAddressP.getZipcode(), 323232318329852);

  EXPECT_EQ(kRAddress.getName(), "Donga Studio");
  EXPECT_EQ(kRAddress.getStreet(), "Nokdu Street");
  EXPECT_EQ(kRAddress.getCity(), "Seoul");

  EXPECT_EQ(subAddress.getChoice1_optional(), "Temp");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
