/*
 * Copyright 2018 The Android Open Source Project
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

#include "packets/link_layer/link_layer_packet_builder.h"

#include <gtest/gtest.h>
#include <forward_list>
#include <memory>

#include "link.h"
#include "packets/link_layer/command_view.h"
#include "packets/link_layer/disconnect_view.h"
#include "packets/link_layer/encrypt_connection_view.h"
#include "packets/link_layer/inquiry_response_view.h"
#include "packets/link_layer/inquiry_view.h"
#include "packets/link_layer/io_capability_negative_response_view.h"
#include "packets/link_layer/io_capability_view.h"
#include "packets/link_layer/le_advertisement_view.h"
#include "packets/link_layer/page_response_view.h"
#include "packets/link_layer/page_view.h"
#include "packets/link_layer/response_view.h"

#include "base/logging.h"

using std::vector;

namespace {
vector<uint8_t> count = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
};

}  // namespace

namespace test_vendor_lib {
namespace packets {

class LinkLayerPacketBuilderTest : public ::testing::Test {
 public:
  LinkLayerPacketBuilderTest() = default;
  ~LinkLayerPacketBuilderTest() = default;

  Address source_{{0x01, 0x02, 0x03, 0x04, 0x05, 0x06}};
  Address dest_{{0x11, 0x12, 0x13, 0x14, 0x15, 0x16}};
};

TEST_F(LinkLayerPacketBuilderTest, constructorTest) {
  uint8_t reason = 0xf2;
  auto disconnect = DisconnectBuilder::Create(reason);
  ASSERT_EQ(disconnect->size(), sizeof(reason));
  auto wrapped_disconnect = LinkLayerPacketBuilder::WrapDisconnect(std::move(disconnect), source_, dest_);

  size_t wrapped_size = sizeof(uint8_t) + sizeof(uint32_t) + 2 * sizeof(Address) + sizeof(reason);
  ASSERT_EQ(wrapped_disconnect->size(), wrapped_size);
  std::vector<uint8_t> wrapped_vect;
  std::back_insert_iterator<std::vector<uint8_t>> it(wrapped_vect);
  wrapped_disconnect->Serialize(it);
  ASSERT_EQ(wrapped_size, wrapped_vect.size());

  std::vector<uint8_t> hand_wrapped_vect;
  // Add the size
  hand_wrapped_vect.push_back(sizeof(uint8_t) + 2 * sizeof(Address) + sizeof(reason));
  hand_wrapped_vect.push_back(0);
  hand_wrapped_vect.push_back(0);
  hand_wrapped_vect.push_back(0);

  hand_wrapped_vect.push_back(static_cast<uint8_t>(Link::PacketType::DISCONNECT));

  for (auto byte : source_.address) {
    hand_wrapped_vect.push_back(byte);
  }
  for (auto byte : dest_.address) {
    hand_wrapped_vect.push_back(byte);
  }
  hand_wrapped_vect.push_back(reason);
  ASSERT_EQ(wrapped_vect, hand_wrapped_vect);
}

TEST_F(LinkLayerPacketBuilderTest, disconnectTest) {
  uint8_t reason = 0x32;
  auto disconnect_builder = DisconnectBuilder::Create(reason);
  auto wrapped_disconnect = LinkLayerPacketBuilder::WrapDisconnect(std::move(disconnect_builder), source_, dest_);
  std::shared_ptr<std::vector<uint8_t>> packet_ptr = std::make_shared<std::vector<uint8_t>>();
  std::back_insert_iterator<std::vector<uint8_t>> it(*packet_ptr);
  wrapped_disconnect->Serialize(it);

  LinkLayerPacketView view = LinkLayerPacketView::Create(packet_ptr);
  ASSERT_EQ(view.size(), wrapped_disconnect->size());
  ASSERT_EQ(view.GetType(), Link::PacketType::DISCONNECT);
  ASSERT_EQ(source_, view.GetSourceAddress());
  ASSERT_EQ(dest_, view.GetDestinationAddress());
  DisconnectView disconnect = DisconnectView::GetDisconnect(view);
  ASSERT_EQ(disconnect.GetReason(), reason);
}

TEST_F(LinkLayerPacketBuilderTest, encryptConnectionTest) {
  std::vector<uint8_t> key = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
  auto encrypt_connection_builder = EncryptConnectionBuilder::Create(key);
  auto wrapped_encrypt_connection =
      LinkLayerPacketBuilder::WrapEncryptConnection(std::move(encrypt_connection_builder), source_, dest_);
  std::shared_ptr<std::vector<uint8_t>> packet_ptr = std::make_shared<std::vector<uint8_t>>();
  std::back_insert_iterator<std::vector<uint8_t>> it(*packet_ptr);
  wrapped_encrypt_connection->Serialize(it);

  LinkLayerPacketView view = LinkLayerPacketView::Create(packet_ptr);
  ASSERT_EQ(view.size(), wrapped_encrypt_connection->size());
  ASSERT_EQ(view.GetType(), Link::PacketType::ENCRYPT_CONNECTION);
  ASSERT_EQ(source_, view.GetSourceAddress());
  ASSERT_EQ(dest_, view.GetDestinationAddress());
  EncryptConnectionView encrypt_connection = EncryptConnectionView::GetEncryptConnection(view);
  auto key_itr = encrypt_connection.GetKey();
  ASSERT_EQ(key_itr.NumBytesRemaining(), key.size());
  for (size_t i = 0; i < key.size(); i++) {
    ASSERT_EQ(key[i], key_itr.extract<uint8_t>());
  }
}

TEST_F(LinkLayerPacketBuilderTest, encryptConnectionResponseTest) {
  std::vector<uint8_t> key = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
  auto encrypt_connection_builder = EncryptConnectionBuilder::Create(key);
  auto wrapped_encrypt_connection_response =
      LinkLayerPacketBuilder::WrapEncryptConnectionResponse(std::move(encrypt_connection_builder), source_, dest_);
  std::shared_ptr<std::vector<uint8_t>> packet_ptr = std::make_shared<std::vector<uint8_t>>();
  std::back_insert_iterator<std::vector<uint8_t>> it(*packet_ptr);
  wrapped_encrypt_connection_response->Serialize(it);

  LinkLayerPacketView view = LinkLayerPacketView::Create(packet_ptr);
  ASSERT_EQ(view.size(), wrapped_encrypt_connection_response->size());
  ASSERT_EQ(view.GetType(), Link::PacketType::ENCRYPT_CONNECTION_RESPONSE);
  ASSERT_EQ(source_, view.GetSourceAddress());
  ASSERT_EQ(dest_, view.GetDestinationAddress());
  EncryptConnectionView encrypt_connection = EncryptConnectionView::GetEncryptConnection(view);
  auto key_itr = encrypt_connection.GetKey();
  ASSERT_EQ(key_itr.NumBytesRemaining(), key.size());
  for (size_t i = 0; i < key.size(); i++) {
    ASSERT_EQ(key[i], key_itr.extract<uint8_t>());
  }
}

TEST_F(LinkLayerPacketBuilderTest, inquiryTest) {
  Inquiry::InquiryType inquiry_type = Inquiry::InquiryType::RSSI;
  auto inquiry_builder = InquiryBuilder::Create(inquiry_type);
  auto wrapped_inquiry = LinkLayerPacketBuilder::WrapInquiry(std::move(inquiry_builder), source_);
  std::shared_ptr<std::vector<uint8_t>> packet_ptr = std::make_shared<std::vector<uint8_t>>();
  std::back_insert_iterator<std::vector<uint8_t>> it(*packet_ptr);
  wrapped_inquiry->Serialize(it);

  LinkLayerPacketView view = LinkLayerPacketView::Create(packet_ptr);
  ASSERT_EQ(view.size(), wrapped_inquiry->size());
  ASSERT_EQ(view.GetType(), Link::PacketType::INQUIRY);
  ASSERT_EQ(source_, view.GetSourceAddress());
  ASSERT_EQ(Address::kEmpty, view.GetDestinationAddress());
  ASSERT_EQ(InquiryView::GetInquiry(view).GetType(), inquiry_type);
}

TEST_F(LinkLayerPacketBuilderTest, standardInquiryResponseTest) {
  uint8_t mode = 23;
  ClassOfDevice class_of_device{{0x11, 0x22, 0x33}};
  uint16_t offset = 0x3456;
  auto inquiry_response_builder = InquiryResponseBuilder::CreateStandard(mode, class_of_device, offset);
  auto wrapped_inquiry =
      LinkLayerPacketBuilder::WrapInquiryResponse(std::move(inquiry_response_builder), source_, dest_);
  std::shared_ptr<std::vector<uint8_t>> packet_ptr = std::make_shared<std::vector<uint8_t>>();
  std::back_insert_iterator<std::vector<uint8_t>> it(*packet_ptr);
  wrapped_inquiry->Serialize(it);
  ASSERT_EQ(packet_ptr->size(), 24u);

  LinkLayerPacketView view = LinkLayerPacketView::Create(packet_ptr);
  ASSERT_EQ(view.size(), wrapped_inquiry->size());
  ASSERT_EQ(view.GetType(), Link::PacketType::INQUIRY_RESPONSE);
  ASSERT_EQ(source_, view.GetSourceAddress());
  ASSERT_EQ(dest_, view.GetDestinationAddress());
  ASSERT_EQ(view.GetPayload().size(), 7u);
  InquiryResponseView inquiry_response = InquiryResponseView::GetInquiryResponse(view);
  ASSERT_EQ(inquiry_response.GetClockOffset(), offset);
  ASSERT_EQ(inquiry_response.GetType(), Inquiry::InquiryType::STANDARD);
  ASSERT_EQ(inquiry_response.GetPageScanRepetitionMode(), mode);
  ASSERT_EQ(inquiry_response.GetClassOfDevice(), class_of_device);
  ASSERT_EQ(inquiry_response.GetClockOffset(), offset);
}

TEST_F(LinkLayerPacketBuilderTest, rssiInquiryResponseTest) {
  uint8_t mode = 23;
  ClassOfDevice class_of_device{{0x11, 0x22, 0x33}};
  uint16_t offset = 0x3456;
  uint8_t rssi = 0x78;
  auto inquiry_response_builder = InquiryResponseBuilder::CreateRssi(mode, class_of_device, offset, rssi);
  auto wrapped_inquiry =
      LinkLayerPacketBuilder::WrapInquiryResponse(std::move(inquiry_response_builder), source_, dest_);
  std::shared_ptr<std::vector<uint8_t>> packet_ptr = std::make_shared<std::vector<uint8_t>>();
  std::back_insert_iterator<std::vector<uint8_t>> it(*packet_ptr);
  wrapped_inquiry->Serialize(it);

  LinkLayerPacketView view = LinkLayerPacketView::Create(packet_ptr);
  ASSERT_EQ(view.size(), wrapped_inquiry->size());
  ASSERT_EQ(view.GetType(), Link::PacketType::INQUIRY_RESPONSE);
  ASSERT_EQ(source_, view.GetSourceAddress());
  ASSERT_EQ(dest_, view.GetDestinationAddress());
  InquiryResponseView inquiry_response = InquiryResponseView::GetInquiryResponse(view);
  ASSERT_EQ(inquiry_response.GetClockOffset(), offset);
  ASSERT_EQ(inquiry_response.GetType(), Inquiry::InquiryType::RSSI);
  ASSERT_EQ(inquiry_response.GetPageScanRepetitionMode(), mode);
  ASSERT_EQ(inquiry_response.GetClassOfDevice(), class_of_device);
  ASSERT_EQ(inquiry_response.GetClockOffset(), offset);
  ASSERT_EQ(inquiry_response.GetRssi(), rssi);
}

TEST_F(LinkLayerPacketBuilderTest, extendedInquiryResponseTest) {
  uint8_t mode = 23;
  ClassOfDevice class_of_device{{0x11, 0x22, 0x33}};
  uint16_t offset = 0x3456;
  uint8_t rssi = 0x78;
  auto inquiry_response_builder = InquiryResponseBuilder::CreateExtended(mode, class_of_device, offset, rssi, count);
  auto wrapped_inquiry =
      LinkLayerPacketBuilder::WrapInquiryResponse(std::move(inquiry_response_builder), source_, dest_);
  std::shared_ptr<std::vector<uint8_t>> packet_ptr = std::make_shared<std::vector<uint8_t>>();
  std::back_insert_iterator<std::vector<uint8_t>> it(*packet_ptr);
  wrapped_inquiry->Serialize(it);

  LinkLayerPacketView view = LinkLayerPacketView::Create(packet_ptr);
  ASSERT_EQ(view.size(), wrapped_inquiry->size());
  ASSERT_EQ(view.GetType(), Link::PacketType::INQUIRY_RESPONSE);
  ASSERT_EQ(source_, view.GetSourceAddress());
  ASSERT_EQ(dest_, view.GetDestinationAddress());
  InquiryResponseView inquiry_response = InquiryResponseView::GetInquiryResponse(view);
  ASSERT_EQ(inquiry_response.GetClockOffset(), offset);
  ASSERT_EQ(inquiry_response.GetType(), Inquiry::InquiryType::EXTENDED);
  ASSERT_EQ(inquiry_response.GetPageScanRepetitionMode(), mode);
  ASSERT_EQ(inquiry_response.GetClassOfDevice(), class_of_device);
  ASSERT_EQ(inquiry_response.GetClockOffset(), offset);
  ASSERT_EQ(inquiry_response.GetRssi(), rssi);
  auto ext_it = inquiry_response.GetExtendedData();
  ASSERT_EQ(ext_it.NumBytesRemaining(), count.size());
  for (size_t i = 0; i < count.size(); i++) {
    ASSERT_EQ(count[i], *(ext_it++));
  }
}

TEST_F(LinkLayerPacketBuilderTest, ioCapabilityRequestTest) {
  uint8_t io_cap = 0x2;
  uint8_t oob_data_present = 0x1;
  uint8_t authentication_requirements = 0x5;
  auto io_capability_builder = IoCapabilityBuilder::Create(io_cap, oob_data_present, authentication_requirements);
  auto wrapped_io_capability_request =
      LinkLayerPacketBuilder::WrapIoCapabilityRequest(std::move(io_capability_builder), source_, dest_);
  std::shared_ptr<std::vector<uint8_t>> packet_ptr = std::make_shared<std::vector<uint8_t>>();
  std::back_insert_iterator<std::vector<uint8_t>> it(*packet_ptr);
  wrapped_io_capability_request->Serialize(it);

  LinkLayerPacketView view = LinkLayerPacketView::Create(packet_ptr);
  ASSERT_EQ(view.size(), wrapped_io_capability_request->size());
  ASSERT_EQ(view.GetType(), Link::PacketType::IO_CAPABILITY_REQUEST);
  ASSERT_EQ(source_, view.GetSourceAddress());
  ASSERT_EQ(dest_, view.GetDestinationAddress());
  IoCapabilityView io_capability = IoCapabilityView::GetIoCapability(view);
  ASSERT_EQ(io_capability.GetIoCapability(), io_cap);
  ASSERT_EQ(io_capability.GetOobDataPresent(), oob_data_present);
  ASSERT_EQ(io_capability.GetAuthenticationRequirements(), authentication_requirements);
}

TEST_F(LinkLayerPacketBuilderTest, ioCapabilityResponseTest) {
  uint8_t io_cap = 0x2;
  uint8_t oob_data_present = 0x1;
  uint8_t authentication_requirements = 0x5;
  auto io_capability_builder = IoCapabilityBuilder::Create(io_cap, oob_data_present, authentication_requirements);
  auto wrapped_io_capability_response =
      LinkLayerPacketBuilder::WrapIoCapabilityResponse(std::move(io_capability_builder), source_, dest_);
  std::shared_ptr<std::vector<uint8_t>> packet_ptr = std::make_shared<std::vector<uint8_t>>();
  std::back_insert_iterator<std::vector<uint8_t>> it(*packet_ptr);
  wrapped_io_capability_response->Serialize(it);

  LinkLayerPacketView view = LinkLayerPacketView::Create(packet_ptr);
  ASSERT_EQ(view.size(), wrapped_io_capability_response->size());
  ASSERT_EQ(view.GetType(), Link::PacketType::IO_CAPABILITY_RESPONSE);
  ASSERT_EQ(source_, view.GetSourceAddress());
  ASSERT_EQ(dest_, view.GetDestinationAddress());
  IoCapabilityView io_capability = IoCapabilityView::GetIoCapability(view);
  ASSERT_EQ(io_capability.GetIoCapability(), io_cap);
  ASSERT_EQ(io_capability.GetOobDataPresent(), oob_data_present);
  ASSERT_EQ(io_capability.GetAuthenticationRequirements(), authentication_requirements);
}

TEST_F(LinkLayerPacketBuilderTest, ioCapabilityNegativeResponseTest) {
  uint8_t reason = 23;
  auto io_capability_negative_response_builder = IoCapabilityNegativeResponseBuilder::Create(reason);
  auto wrapped_io_capability_negative_response = LinkLayerPacketBuilder::WrapIoCapabilityNegativeResponse(
      std::move(io_capability_negative_response_builder), source_, dest_);
  std::shared_ptr<std::vector<uint8_t>> packet_ptr = std::make_shared<std::vector<uint8_t>>();
  std::back_insert_iterator<std::vector<uint8_t>> it(*packet_ptr);
  wrapped_io_capability_negative_response->Serialize(it);

  LinkLayerPacketView view = LinkLayerPacketView::Create(packet_ptr);
  ASSERT_EQ(view.size(), wrapped_io_capability_negative_response->size());
  ASSERT_EQ(view.GetType(), Link::PacketType::IO_CAPABILITY_NEGATIVE_RESPONSE);
  ASSERT_EQ(source_, view.GetSourceAddress());
  ASSERT_EQ(dest_, view.GetDestinationAddress());
  IoCapabilityNegativeResponseView io_capability_negative_response =
      IoCapabilityNegativeResponseView::GetIoCapabilityNegativeResponse(view);
  ASSERT_EQ(io_capability_negative_response.GetReason(), reason);
}

TEST_F(LinkLayerPacketBuilderTest, pageTest) {
  uint8_t allow_role_switch = 1;
  ClassOfDevice class_of_device{{0x11, 0x22, 0x33}};
  auto page_builder = PageBuilder::Create(class_of_device, allow_role_switch);
  auto wrapped_page = LinkLayerPacketBuilder::WrapPage(std::move(page_builder), source_, dest_);
  std::shared_ptr<std::vector<uint8_t>> packet_ptr = std::make_shared<std::vector<uint8_t>>();
  std::back_insert_iterator<std::vector<uint8_t>> it(*packet_ptr);
  wrapped_page->Serialize(it);

  LinkLayerPacketView view = LinkLayerPacketView::Create(packet_ptr);
  ASSERT_EQ(view.size(), wrapped_page->size());
  ASSERT_EQ(view.GetType(), Link::PacketType::PAGE);
  ASSERT_EQ(source_, view.GetSourceAddress());
  ASSERT_EQ(dest_, view.GetDestinationAddress());
  PageView page = PageView::GetPage(view);
  ASSERT_EQ(page.GetAllowRoleSwitch(), allow_role_switch);
  ASSERT_EQ(page.GetClassOfDevice(), class_of_device);
}

TEST_F(LinkLayerPacketBuilderTest, pageResponseTest) {
  uint8_t try_role_switch = 2;
  auto page_response_builder = PageResponseBuilder::Create(try_role_switch);
  auto wrapped_page_response =
      LinkLayerPacketBuilder::WrapPageResponse(std::move(page_response_builder), source_, dest_);
  std::shared_ptr<std::vector<uint8_t>> packet_ptr = std::make_shared<std::vector<uint8_t>>();
  std::back_insert_iterator<std::vector<uint8_t>> it(*packet_ptr);
  wrapped_page_response->Serialize(it);

  LinkLayerPacketView view = LinkLayerPacketView::Create(packet_ptr);
  ASSERT_EQ(view.size(), wrapped_page_response->size());
  ASSERT_EQ(view.GetType(), Link::PacketType::PAGE_RESPONSE);
  ASSERT_EQ(source_, view.GetSourceAddress());
  ASSERT_EQ(dest_, view.GetDestinationAddress());
  PageResponseView page_response = PageResponseView::GetPageResponse(view);
  ASSERT_EQ(page_response.GetTryRoleSwitch(), try_role_switch);
}

TEST_F(LinkLayerPacketBuilderTest, responseTest) {
  uint16_t opcode = 0x1234;
  std::vector<uint64_t> data{
      0x7060504030201000, 0x7161514131211101, 0x7262524232221202, 0x7363534333231303,
      0x7464544434241404, 0x7565554535251505, 0x7666564636261606, 0x7767574737271707,
      0x7868584838281808, 0x7969594939291909, 0x7a6a5a4a3a2a1a0a, 0x7b6b5b4b3b2b1b0b,
      0x7c6c5c4c3c2c1c0c, 0x7d6d5d4d3d2d1d0d, 0x7e6e5e4e3e2e1e0e, 0x7f6f5f4f3f2f1f0f,
  };
  auto response_builder = ResponseBuilder::Create(opcode, data);
  auto wrapped_response = LinkLayerPacketBuilder::WrapResponse(std::move(response_builder), source_, dest_);
  std::shared_ptr<std::vector<uint8_t>> packet_ptr = std::make_shared<std::vector<uint8_t>>();
  std::back_insert_iterator<std::vector<uint8_t>> it(*packet_ptr);
  wrapped_response->Serialize(it);

  LinkLayerPacketView view = LinkLayerPacketView::Create(packet_ptr);
  ASSERT_EQ(view.size(), wrapped_response->size());
  ASSERT_EQ(view.GetType(), Link::PacketType::RESPONSE);
  ASSERT_EQ(source_, view.GetSourceAddress());
  ASSERT_EQ(dest_, view.GetDestinationAddress());
  ResponseView response = ResponseView::GetResponse(view);
  ASSERT_EQ(opcode, response.GetOpcode());
  auto data_it = response.GetResponseData();
  ASSERT_EQ(data.size(), data_it.NumBytesRemaining() / sizeof(uint64_t));
  ASSERT_EQ(0u, data_it.NumBytesRemaining() % sizeof(uint64_t));
  for (size_t i = 0; i < data.size(); i++) {
    ASSERT_EQ(data[i], data_it.extract<uint64_t>());
  }
}

TEST_F(LinkLayerPacketBuilderTest, wrapAclTest) {
  std::shared_ptr<std::vector<uint8_t>> count_shared = std::make_shared<std::vector<uint8_t>>(count);
  View count_view(count_shared, 0, count_shared->size());
  PacketView<true> count_packet_view({count_view});
  auto builder = ViewForwarderBuilder::Create(count_packet_view);
  auto wrapped_acl = LinkLayerPacketBuilder::WrapAcl(std::move(builder), source_, dest_);
  std::shared_ptr<std::vector<uint8_t>> packet_ptr = std::make_shared<std::vector<uint8_t>>();
  std::back_insert_iterator<std::vector<uint8_t>> it(*packet_ptr);
  wrapped_acl->Serialize(it);

  LinkLayerPacketView view = LinkLayerPacketView::Create(packet_ptr);
  ASSERT_EQ(view.size(), wrapped_acl->size());
  ASSERT_EQ(view.GetType(), Link::PacketType::ACL);
  ASSERT_EQ(source_, view.GetSourceAddress());
  ASSERT_EQ(dest_, view.GetDestinationAddress());
  auto acl_view = view.GetPayload();
  ASSERT_EQ(acl_view.size(), count_view.size());
  for (size_t i = 0; i < count_view.size(); i++) {
    ASSERT_EQ(acl_view[i], count_view[i]);
  }
}

TEST_F(LinkLayerPacketBuilderTest, wrapCommandTest) {
  uint16_t opcode = 0x0102;
  std::shared_ptr<std::vector<uint8_t>> count_shared = std::make_shared<std::vector<uint8_t>>(count);
  View count_view(count_shared, 0, count_shared->size());
  PacketView<true> args({count_view});
  auto builder = CommandBuilder::Create(opcode, args);
  auto wrapped_command = LinkLayerPacketBuilder::WrapCommand(std::move(builder), source_, dest_);
  std::shared_ptr<std::vector<uint8_t>> packet_ptr = std::make_shared<std::vector<uint8_t>>();
  std::back_insert_iterator<std::vector<uint8_t>> it(*packet_ptr);
  wrapped_command->Serialize(it);

  LinkLayerPacketView view = LinkLayerPacketView::Create(packet_ptr);
  ASSERT_EQ(view.size(), wrapped_command->size());
  ASSERT_EQ(view.GetType(), Link::PacketType::COMMAND);
  ASSERT_EQ(source_, view.GetSourceAddress());
  ASSERT_EQ(dest_, view.GetDestinationAddress());
  auto command_view = CommandView::GetCommand(view);
  ASSERT_EQ(opcode, command_view.GetOpcode());
  auto args_itr = command_view.GetData();
  ASSERT_EQ(args_itr.NumBytesRemaining(), count.size());
  for (size_t i = 0; i < count.size(); i++) {
    ASSERT_EQ(*args_itr++, count[i]);
  }
}

TEST_F(LinkLayerPacketBuilderTest, wrapLeAdvertisementTest) {
  LeAdvertisement::AddressType address_type = LeAdvertisement::AddressType::RANDOM;
  LeAdvertisement::AdvertisementType advertisement_type = LeAdvertisement::AdvertisementType::ADV_NONCONN_IND;
  auto builder = LeAdvertisementBuilder::Create(address_type, advertisement_type, count);
  auto wrapped_le_advertisement = LinkLayerPacketBuilder::WrapLeAdvertisement(std::move(builder), source_);
  std::shared_ptr<std::vector<uint8_t>> packet_ptr = std::make_shared<std::vector<uint8_t>>();
  std::back_insert_iterator<std::vector<uint8_t>> it(*packet_ptr);
  wrapped_le_advertisement->Serialize(it);

  LinkLayerPacketView view = LinkLayerPacketView::Create(packet_ptr);
  ASSERT_EQ(view.size(), wrapped_le_advertisement->size());
  ASSERT_EQ(view.GetType(), Link::PacketType::LE_ADVERTISEMENT);
  ASSERT_EQ(source_, view.GetSourceAddress());
  ASSERT_EQ(Address::kEmpty, view.GetDestinationAddress());
  LeAdvertisementView le_advertisement_view = LeAdvertisementView::GetLeAdvertisementView(view);
  ASSERT_EQ(address_type, le_advertisement_view.GetAddressType());
  ASSERT_EQ(advertisement_type, le_advertisement_view.GetAdvertisementType());
  auto le_advertisement_itr = le_advertisement_view.GetData();
  ASSERT_EQ(le_advertisement_itr.NumBytesRemaining(), count.size());
  for (size_t i = 0; i < count.size(); i++) {
    ASSERT_EQ(*(le_advertisement_itr++), count[i]);
  }
}

TEST_F(LinkLayerPacketBuilderTest, wrapLeScanTest) {
  auto le_scan = LinkLayerPacketBuilder::WrapLeScan(source_, dest_);
  std::shared_ptr<std::vector<uint8_t>> packet_ptr = std::make_shared<std::vector<uint8_t>>();
  std::back_insert_iterator<std::vector<uint8_t>> it(*packet_ptr);
  le_scan->Serialize(it);

  LinkLayerPacketView view = LinkLayerPacketView::Create(packet_ptr);
  ASSERT_EQ(view.size(), le_scan->size());
  ASSERT_EQ(view.GetType(), Link::PacketType::LE_SCAN);
  ASSERT_EQ(source_, view.GetSourceAddress());
  ASSERT_EQ(dest_, view.GetDestinationAddress());
  auto le_scan_view = view.GetPayload();
  ASSERT_EQ(0u, le_scan_view.size());
}

TEST_F(LinkLayerPacketBuilderTest, wrapLeScanResponseTest) {
  LeAdvertisement::AddressType address_type = LeAdvertisement::AddressType::PUBLIC_IDENTITY;
  LeAdvertisement::AdvertisementType advertisement_type = LeAdvertisement::AdvertisementType::SCAN_RESPONSE;
  auto builder = LeAdvertisementBuilder::Create(address_type, advertisement_type, count);
  auto wrapped_scan_response = LinkLayerPacketBuilder::WrapLeScanResponse(std::move(builder), source_, dest_);
  std::shared_ptr<std::vector<uint8_t>> packet_ptr = std::make_shared<std::vector<uint8_t>>();
  std::back_insert_iterator<std::vector<uint8_t>> it(*packet_ptr);
  wrapped_scan_response->Serialize(it);

  LinkLayerPacketView view = LinkLayerPacketView::Create(packet_ptr);
  ASSERT_EQ(view.size(), wrapped_scan_response->size());
  ASSERT_EQ(view.GetType(), Link::PacketType::LE_SCAN_RESPONSE);
  ASSERT_EQ(source_, view.GetSourceAddress());
  ASSERT_EQ(dest_, view.GetDestinationAddress());
  LeAdvertisementView le_advertisement_view = LeAdvertisementView::GetLeAdvertisementView(view);
  ASSERT_EQ(address_type, le_advertisement_view.GetAddressType());
  ASSERT_EQ(advertisement_type, le_advertisement_view.GetAdvertisementType());
  auto scan_response_itr = le_advertisement_view.GetData();
  ASSERT_EQ(scan_response_itr.NumBytesRemaining(), count.size());
  for (size_t i = 0; i < count.size(); i++) {
    ASSERT_EQ((*scan_response_itr++), count[i]);
  }
}

TEST_F(LinkLayerPacketBuilderTest, wrapScoTest) {
  std::shared_ptr<std::vector<uint8_t>> count_shared = std::make_shared<std::vector<uint8_t>>(count);
  View count_view(count_shared, 0, count_shared->size());
  PacketView<true> count_packet_view({count_view});
  auto builder = ViewForwarderBuilder::Create(count_packet_view);
  auto wrapped_sco = LinkLayerPacketBuilder::WrapSco(std::move(builder), source_, dest_);
  std::shared_ptr<std::vector<uint8_t>> packet_ptr = std::make_shared<std::vector<uint8_t>>();
  std::back_insert_iterator<std::vector<uint8_t>> it(*packet_ptr);
  wrapped_sco->Serialize(it);

  LinkLayerPacketView view = LinkLayerPacketView::Create(packet_ptr);
  ASSERT_EQ(view.size(), wrapped_sco->size());
  ASSERT_EQ(view.GetType(), Link::PacketType::SCO);
  ASSERT_EQ(source_, view.GetSourceAddress());
  ASSERT_EQ(dest_, view.GetDestinationAddress());
  auto sco_view = view.GetPayload();
  ASSERT_EQ(sco_view.size(), count.size());
  for (size_t i = 0; i < count.size(); i++) {
    ASSERT_EQ(sco_view[i], count[i]);
  }
}

}  // namespace packets
}  // namespace test_vendor_lib
