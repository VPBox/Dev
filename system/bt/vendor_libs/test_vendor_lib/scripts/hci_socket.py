#
# Copyright 2015 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
"""Script for sending data to a port.

This script provides a simple shell interface for sending data at run-time to a
port.

Usage:
    1. Choose a port to use. Use 'adb forward tcp:<port>
    tcp:<port>' to forward the port to the device.
    2. In a separate shell, build and push the test vendor library to the device
    using the script mentioned in option A (i.e. without the --test-channel flag
    set).
    3. Once logcat has started, turn Bluetooth on from the device.
    4. Run this program, in the shell from step 1,  the port, also from step 1,
    as arguments.

    scapy is the tool we use to build packets in Python.

    >>> d = HCI_Hdr(type=1) / HCI_Command_Hdr(opcode = 0x1004) /
    Raw(load='\x01')
    >>> print(d)
    <HCI_Hdr  type=Command |<HCI_Command_Hdr  opcode=0x1004 |<Raw  load='\x01'
    |>>>
    >>> raw(d)
    '\x01\x04\x10\x01\x01'
    >>> hexdump(d)
    0000  0104100101                       .....


    >>> pkt = HCI_Hdr('\x02\x02\x20\x0a\x00\x06\x00\x01\x00') /
    L2CAP_CmdHdr(code=10, id=2, len=2) /L2CAP_InfoReq(type=2)
    >>> pkt
    <HCI_Hdr  type=ACL Data |<HCI_ACL_Hdr  handle=2 PB=0 BC=2 len=10 |<L2CAP_Hdr
    len=6 cid=control |<L2CAP_CmdHdr  code=info_req id=2 len=2 |<L2CAP_InfoReq
    type=FEAT_MASK |>>>>>
    >>> pkt = HCI_Hdr(type='ACL Data') / HCI_ACL_Hdr(handle=2, PB=0, BC=2,
    len=10) / L2CAP_Hdr(len=6, cid='control') / L2CAP_CmdHdr(code='info_req',
    id=2, len=2) / L2CAP_InfoReq(type='FEAT_MASK')
    >>> raw(pkt)
    '\x02\x02 \n\x00\x06\x00\x01\x00\n\x02\x02\x00\x02\x00'
    >>> hexdump(pkt)
    0000  0202200A00060001000A0202000200   .. ............


"""

#!/usr/bin/env python

import binascii
import cmd
import queue
import random
import socket
import string
import struct
import sys
from scapy.all import *
""" Add some more SCAPY stuff"""


class HCI_Cmd_Create_Connection(Packet):
  name = 'Create Connection'
  fields_desc = [
      LEMACField('addr', None),
      LEShortField('packet_type', 8),
      ByteEnumField('page_scan_repetition_mode', 0, {
          0: 'R0',
          1: 'R1',
          2: 'R2'
      }),
      ByteEnumField('rsvd', 0, {0: 'Reserved'}),
      LEShortField('clock_offset', 0),
      ByteEnumField('allow_role_switch', 1, {
          0: 'false',
          1: 'true'
      }),
  ]


class HCI_Cmd_Inquiry(Packet):
  name = 'Inquiry'
  fields_desc = [
      X3BytesField('LAP', 0x9e8b0),
      ByteField('length', 1),
      ByteField('max_responses', 0),
  ]


bind_layers(HCI_Command_Hdr, HCI_Cmd_Inquiry, opcode=0x0401)
bind_layers(HCI_Command_Hdr, HCI_Cmd_Create_Connection, opcode=0x0405)


class HCI_Event_Inquiry_Result(Packet):
  name = 'Inquiry Result'
  fields_desc = [
      ByteField('num_responses', 0),
      LEMACField('addr', None),
      ByteEnumField('page_scan_repetition_mode', 0, {
          0: 'R0',
          1: 'R1',
          2: 'R2'
      }),
      LEShortEnumField('rsvd', 0, {0: 'Reserved'}),
      X3BytesField('class_of_device', 0),
      LEShortField('clock_offset', 0),
  ]


class HCI_Event_Connection_Complete(Packet):
  name = 'Connection Complete'
  fields_desc = [
      ByteField('status', 0),
      LEShortField('handle', 0xffff),
      LEMACField('addr', None),
      ByteField('link_type', 1),
      ByteField('encryption_mode', 0),
  ]


class HCI_Event_Remote_Name_Request_Complete(Packet):
  name = 'Remote Name Request Complete'
  fields_desc = [
      ByteField('status', 0),
      LEMACField('addr', None),
  ]


class HCI_Event_Read_Remote_Supported_Features_Complete(Packet):
  name = 'Read Remote Supported Features Complete'
  fields_desc = [
      ByteField('status', 0),
      LEShortField('handle', 0xffff),
      XLELongField('features', 0x0123456789abcdef),
  ]


class HCI_Event_Read_Remote_Version_Information_Complete(Packet):
  name = 'Read Remote Version Information Complete'
  fields_desc = [
      ByteField('status', 0),
      LEShortField('handle', 0xffff),
      ByteField('version', 0),
      LEShortField('manufacturer_name', 0),
      LEShortField('subversion', 0),
  ]


class HCI_Event_Read_Clock_Offset_Complete(Packet):
  name = 'Read Clock Offset Complete'
  fields_desc = [
      ByteField('status', 0),
      LEShortField('handle', 0xffff),
      LEShortField('offset', 0xffff),
  ]


class HCI_Event_Read_Remote_Extended_Features_Complete(Packet):
  name = 'Read Remote Supported Features Complete'
  fields_desc = [
      ByteField('status', 0),
      LEShortField('handle', 0xffff),
      ByteField('page_number', 0),
      ByteField('max_page_number', 0),
      XLELongField('features', 0x0123456789abcdef),
  ]


class HCI_Event_Extended_Inquiry_Result(Packet):
  name = 'Extended Inquiry Result'
  fields_desc = [
      ByteField('num_responses', 1),
      LEMACField('addr', None),
      ByteEnumField('page_scan_repetition_mode', 0, {
          0: 'R0',
          1: 'R1',
          2: 'R2'
      }),
      ByteEnumField('rsvd', 0, {0: 'Reserved'}),
      X3BytesField('class_of_device', 0),
      LEShortField('clock_offset', 0),
      SignedByteField('rssi', -20),
      PacketListField('extended_inquiry_response', [], EIR_Hdr, 1),
  ]


bind_layers(HCI_Event_Hdr, HCI_Event_Inquiry_Result, code=0x02)
bind_layers(HCI_Event_Hdr, HCI_Event_Connection_Complete, code=0x03)
bind_layers(HCI_Event_Hdr, HCI_Event_Remote_Name_Request_Complete, code=0x07)
bind_layers(
    HCI_Event_Hdr, HCI_Event_Read_Remote_Supported_Features_Complete, code=0x0b)
bind_layers(
    HCI_Event_Hdr,
    HCI_Event_Read_Remote_Version_Information_Complete,
    code=0x0c)
bind_layers(HCI_Event_Hdr, HCI_Event_Read_Clock_Offset_Complete, code=0x1c)
bind_layers(
    HCI_Event_Hdr, HCI_Event_Read_Remote_Extended_Features_Complete, code=0x23)
bind_layers(HCI_Event_Hdr, HCI_Event_Extended_Inquiry_Result, code=0x2f)
""" END SCAPY stuff"""


class HCISocket(SuperSocket):
  """Simple wrapper class for a socket object.

  Attributes:
    socket: The underlying socket created for the specified address and port.
  """

  def __init__(self, port):
    self.done_ = False
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(('localhost', port))
    self.ins = self.outs = s
    self.packets_ = queue.Queue()
    self.rx_thread_ = threading.Thread(target=self.rx_thread_body)
    self.rx_thread_.start()

  def rx_bytes(self, size):
    while not self.done_:
      raw_bytes = b''
      while len(raw_bytes) < size and not self.done_:
        more_raw_bytes = self.ins.recv(min(size - len(raw_bytes), 2048))
        if more_raw_bytes:
          raw_bytes += more_raw_bytes
      return raw_bytes

  def rx_thread_body(self):
    while not self.done_:
      payload_length = 0
      # Read the type
      type_byte = self.rx_bytes(1)
      if not type_byte:
        continue
      # Read the Header
      header = b''
      if type_byte == b'\x01':  # Command
        header = self.rx_bytes(3)
        if not header:
          continue
        payload_length = header[2]
      elif type_byte == b'\x02':  # ACL
        header = self.rx_bytes(4)
        if not header:
          continue
        payload_length = header[3] << 8
        payload_length |= header[2]
      elif type_byte == b'\x03':  # SCO
        header = self.rx_bytes(3)
        if not header:
          continue
        payload_length = header[2]
      elif type_byte == b'\x04':  # Event
        header = self.rx_bytes(2)
        if not header:
          continue
        payload_length = header[1]
      else:
        self.done_ = True
        print('Rx: type_byte ' + hex(type_byte[0]))
      # Read the Payload
      payload = self.rx_bytes(payload_length) if payload_length != 0 else b''
      packet_bytes = type_byte + header + payload
      packet = HCI_Hdr(packet_bytes)
      print('Rx: ' + packet.__repr__())
      self.packets_.put(packet)

  def get_packet(self):
    if self.packets_.empty():
      return False
    return self.packets_.get()

  def tell_rx_thread_to_quit(self):
    self.done_ = True
    self.rx_thread_.join()


class HCIShell(cmd.Cmd):
  """Shell for sending binary data to a port.

  """

  def __init__(self, hci):
    cmd.Cmd.__init__(self)
    self._hci = hci

  def do_send(self, args):
    """Arguments: dev_type_str Add a new device of type dev_type_str.

    """
    self._hci.send_binary(args.split())

  def do_connect(self, args):
    """Arguments: bluetooth_address xx:xx:xx:xx:xx:xx, timeout (seconds)

    """
    split_args = args.split()
    address = split_args[0] if len(split_args) > 0 else 'NULL'
    timeout = int(split_args[1]) if len(split_args) > 1 else 2
    num_responses = 0
    connect = HCI_Hdr(type='Command') / HCI_Command_Hdr(
        opcode=0x0405) / HCI_Cmd_Create_Connection(addr=address)
    self._hci.send(connect)
    status = None
    while status == None:
      response = self._hci.get_packet()
      if response == False:
        continue
      if response[HCI_Hdr].type == HCI_Hdr(
          type='Event'
      ).type and response[HCI_Event_Hdr].code == 0xf and response[
          HCI_Event_Command_Status].opcode == connect[HCI_Command_Hdr].opcode:
        status = response[HCI_Event_Command_Status].status
    if status != HCI_Event_Command_Status(status='pending').status:
      print('Connection failed with status = ' + str(status))
      return

    handle = None
    while handle == None:
      connection_complete = self._hci.get_packet()
      if connection_complete == False:
        continue
      if (connection_complete[HCI_Hdr].type == HCI_Hdr(type='Event').type) and (
          connection_complete[HCI_Event_Hdr].code == 0x3):
        status = connection_complete[HCI_Event_Connection_Complete].status
        if status != 0:
          print('Connection complete with failed status = ' + str(status))
          return
        handle = connection_complete[HCI_Event_Connection_Complete].handle
        print('Connection established with handle ' + str(handle))
        connection_complete.show()
        hexdump(connection_complete)

    l2cap_done = False
    while l2cap_done == None:
      l2cap_req = self._hci.get_packet()
      if l2cap_req == False:
        continue
      if (l2cap_req[HCI_Hdr].type == HCI_Hdr(type='ACL Data').type) and (
          l2cap_req[L2CAP_Hdr].cid == L2CAP_Hdr(cid='control').cid) and (
              l2cap_req[L2CAP_CmdHdr].code == L2CAP_CmdHdr(code='info_req').code
          ) and (l2cap_req[L2CAP_InfoReq].type == L2CAP_InfoReq(
              type='FEAT_MASK').type):
        print('Send Features packet' + HCI_Hdr(type='ACL Data') / HCI_ACL_Hdr(
            handle=l2cap_req[HCI_ACL_Hdr].handle, PB=0, BC=2, len=16) /
              L2CAP_Hdr(len=12, cid='control') /
              L2CAP_CmdHdr(code='info_resp', id=146, len=8) / L2CAP_InfoResp(
                  type=l2cap_req[L2CAP_InfoResp].type,
                  result='success',
                  data=b'\xb8\x00\x00\x00').__repr__())
        self._hci.send(
            HCI_Hdr(type='ACL Data') / HCI_ACL_Hdr(
                handle=l2cap_req[HCI_ACL_Hdr].handle, PB=0, BC=2, len=16) /
            L2CAP_Hdr(len=12, cid='control') / L2CAP_CmdHdr(
                code='info_resp', id=146, len=8) / L2CAP_InfoResp(
                    type=l2cap_req[L2CAP_InfoResp].type,
                    result='success',
                    data=b'\xb8\x00\x00\x00'))

  def do_le_scan(self, args):
    """Arguments: enable (0 or 1), filter duplicates (0 or 1) Print the scan responses from reachable devices

    """
    split_args = args.split()
    enable = int(split_args[0]) if len(split_args) > 0 else 1
    filter_dups = int(split_args[1]) if len(split_args) > 1 else 1
    set_scan_enable = HCI_Hdr(type=1) / HCI_Command_Hdr(
        opcode=0x200c) / HCI_Cmd_LE_Set_Scan_Enable(
            enable=enable, filter_dups=filter_dups)
    print('Tx: ' + set_scan_enable.__repr__())
    self._hci.send(set_scan_enable)

  def do_scan(self, args):
    """Arguments: timeout (seconds), max_results Print the scan responses from reachable devices

    """
    split_args = args.split()
    scan_time = int(split_args[0]) if len(split_args) > 0 else 0
    max_responses = int(split_args[1]) if len(split_args) > 1 else 0
    num_responses = 0
    inquiry = HCI_Hdr(type='Command') / HCI_Command_Hdr(
        opcode=0x0401) / HCI_Cmd_Inquiry(
            length=scan_time, max_responses=max_responses)
    print('Tx: ' + inquiry.__repr__())
    self._hci.send(inquiry)

  def do_quit(self, args):
    """Arguments: None.

    Exits.
    """
    self._hci.tell_rx_thread_to_quit()
    self._hci.close()
    print('Goodbye.')
    return True

  def do_help(self, args):
    """Arguments: [dev_num [attr]] List the commands available, optionally filtered by device and attr.

    """
    if (len(args) == 0):
      cmd.Cmd.do_help(self, args)


def main(argv):
  if len(argv) != 2:
    print('Usage: python hci_socket.py [port]')
    return
  try:
    port = int(argv[1])
  except ValueError:
    print('Error parsing port.')
  else:
    try:
      hci = HCISocket(port)
    except socket.error as e:
      print('Error connecting to socket: %s' % e)
    except:
      print('Error creating (check arguments).')
    else:
      hci_shell = HCIShell(hci)
      hci_shell.prompt = '$ '
      hci_shell.cmdloop('Welcome to the RootCanal HCI Console \n' +
                        'Type \'help\' for more information.')


if __name__ == '__main__':
  main(sys.argv)
