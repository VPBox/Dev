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

class LinkLayerSocket(object):
  """Simple wrapper class for a socket object.

  Attributes:
    socket: The underlying socket created for the specified address and port.
  """

  def __init__(self, port):
    print('port = ' + port)
    self.done_ = False
    self._socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    self._socket.connect(('localhost', port))
    # Should it be a non-blocking socket?
    # self._socket.setblocking(0)
    self.packets_ = queue.Queue()
    self.rx_thread_ = threading.Thread(target=self.rx_thread_body)
    self.rx_thread_.start()

  def rx_bytes(self, size):
    while not self.done_:
      raw_bytes = b''
      while len(raw_bytes) < size and not self.done_:
        more_raw_bytes = self._socket.recv(min(size - len(raw_bytes), 2048))
        if more_raw_bytes:
          raw_bytes += more_raw_bytes
      return raw_bytes

  def rx_thread_body(self):
    while not self.done_:
      payload_length = 0
      # Read the size (4B), the type (1B), and the addresses (2*6B)
      header = self.rx_bytes(17)
      if not header:
        continue
      payload_length = header[0]
      payload_length |= header[1] << 8
      payload_length |= header[2] << 16
      payload_length |= header[3] << 24
      print('Rx: type_byte ' + hex(header[4]))
      print('Rx: from ' + hex(header[5]) + ':' + hex(header[6]) + ':' + hex(header[7]) + ':' + hex(header[8]) + ':' + hex(header[9]) + ':' + hex(header[10]))
      print('Rx: to ' + hex(header[11]) + ':' + hex(header[12]) + ':' + hex(header[13]) + ':' + hex(header[14]) + ':' + hex(header[15]) + ':' + hex(header[16]))
      # Read the Payload
      payload = self.rx_bytes(payload_length) if payload_length != 0 else b''
      packet_bytes = header + payload
      self.packets_.put(packet_bytes)

  def get_packet(self):
    if self.packets_.empty():
      return False
    return self.packets_.get()

  def send_binary(self, args):
    joined_args = ''.join(arg for arg in args)
    print(joined_args)
    packet = binascii.a2b_hex(joined_args)
    if self._done:
      return
    self._connection.send(packet)

  def tell_rx_thread_to_quit(self):
    self.done_ = True
    self.rx_thread_.join()


class LinkLayerShell(cmd.Cmd):
  """Shell for sending binary data to a port.

  """

  def __init__(self, link_layer):
    cmd.Cmd.__init__(self)
    self._link_layer = link_layer

  def do_send(self, args):
    """Arguments: binary representation of a packet.

    """
    self._link_layer.send_binary(args.split())

  def do_quit(self, args):
    """Arguments: None.

    Exits.
    """
    self._link_layer.tell_rx_thread_to_quit()
    self._link_layer.close()
    print('Goodbye.')
    return True

  def do_help(self, args):
    """Arguments: [dev_num [attr]] List the commands available, optionally filtered by device and attr.

    """
    if (len(args) == 0):
      cmd.Cmd.do_help(self, args)


def main(argv):
  if len(argv) != 2:
    print('Usage: python link_layer_socket.py [port]')
    return
  try:
    port = int(argv[1])
  except ValueError:
    print('Error parsing port.')
  else:
    try:
      link_layer = LinkLayerSocket(port)
    except socket.error as e:
      print('Error connecting to socket: %s' % e)
    except:
      print('Error creating (check arguments).')
    else:
      link_layer_shell = LinkLayerShell(link_layer)
      link_layer_shell.prompt = '$ '
      link_layer_shell.cmdloop('Welcome to the RootCanal LinkLayer Console \n' +
                        'Type \'help\' for more information.')


if __name__ == '__main__':
  main(sys.argv)
