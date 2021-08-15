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
    >>> pkt = HCI_Hdr(type="ACL Data") / HCI_ACL_Hdr(handle=2, PB=0, BC=2,
    len=10) / L2CAP_Hdr(len=6, cid="control") / L2CAP_CmdHdr(code="info_req",
    id=2, len=2) / L2CAP_InfoReq(type="FEAT_MASK")
    >>> raw(pkt)
    '\x02\x02 \n\x00\x06\x00\x01\x00\n\x02\x02\x00\x02\x00'
    >>> hexdump(pkt)
    0000  0202200A00060001000A0202000200   .. ............


"""

#!/usr/bin/env python

import binascii
import cmd
import random
import socket
import string
import struct
import sys
from scapy.all import *
""" Add some more SCAPY stuff"""


class HCI_Cmd_Connect(Packet):
  name = "Connect"
  fields_desc = [
      ByteEnumField("filter", 0, {0: "address"}),
      LEShortField("packet_type", 8),
      ByteEnumField("page_scan_repetition_mode", 0, {
          0: "R0",
          1: "R1",
          2: "R2"
      }),
      ByteEnumField("page_scan_repetition_mode", 0, {0: "Reserved"}),
      LEShortField("clock_offset", 0),
      ByteEnumField("allow_role_switch", 0, {
          0: "false",
          1: "true"
      }),
  ]


class HCI_Cmd_Inquiry(Packet):
  name = "Inquiry"
  fields_desc = [
      XByteField("LAP0", 0),
      XByteField("LAP1", 0x8B),
      XByteField("LAP2", 0x9E),
      ByteField("length", 1),
      ByteField("max_responses", 0),
  ]


""" END SCAPY stuff"""


class Connection(object):
  """Simple wrapper class for a socket object.

  Attributes:
    socket: The underlying socket created for the specified address and port.
  """

  def __init__(self, port):
    self._socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    self._socket.connect(("localhost", port))

  def close(self):
    self._socket.close()

  def send(self, data):
    self._socket.sendall(data)

  def receive(self, size):
    return self._socket.recv(size)


class RawPort(object):
  """Converts outgoing packets to binary and sends them.

  Attributes:
    connection: The connection to the HCI port.
  """

  def __init__(self, port):
    self._connection = Connection(port)

  def close(self):
    self._connection.close()

  def send_binary(self, args):
    joined_args = "".join(arg for arg in args)
    print(joined_args)
    packet = binascii.a2b_hex(joined_args)
    self._connection.send(packet)

  def receive_response(self):
    ready_to_read, ready_to_write, in_error = \
               select.select(
                  [ self._connection._socket ],
                  [ ],
                  [ self._connection._socket ],
                  1.5)
    if len(in_error) > 0:
      print("Error")
      return False
    if len(ready_to_read) > 0:
      print("Ready to Read")
      type_str = self._connection.receive(512)
      print(len(type_str))
      print(type_str)
      return type_str
    print("Returning false at the end")
    return False


class RawPortShell(cmd.Cmd):
  """Shell for sending binary data to a port.

  """

  def __init__(self, raw_port):
    cmd.Cmd.__init__(self)
    self._raw_port = raw_port

  def do_send(self, args):
    """Arguments: dev_type_str Add a new device of type dev_type_str.

    """
    self._raw_port.send_binary(args.split())

  def do_scan(self, args):
    """Arguments: timeout (seconds) Print the scan responses from reachable devices

    """
    self._raw_port.send_binary(args.split())

  def do_quit(self, args):
    """Arguments: None.

    Exits.
    """
    self._raw_port.close()
    print("Goodbye.")
    return True

  def do_help(self, args):
    """Arguments: [dev_num [attr]] List the commands available, optionally filtered by device and attr.

    """
    if (len(args) == 0):
      cmd.Cmd.do_help(self, args)

  def postcmd(self, stop, line):
    """Called after each command stop : whether we will stop after this command line : the previous input line Return True to stop, False to continue

    """
    if stop:
      return True
    response = self._raw_port.receive_response()
    print(response)
    return False


def main(argv):
  if len(argv) != 2:
    print("Usage: python raw_port.py [port]")
    return
  try:
    port = int(argv[1])
  except ValueError:
    print("Error parsing port.")
  else:
    try:
      raw_port = RawPort(port)
    except (socket.error, e):
      print("Error connecting to socket: %s" % e)
    except:
      print("Error creating (check arguments).")
    else:
      raw_port_shell = RawPortShell(raw_port)
      raw_port_shell.prompt = "$ "
      raw_port_shell.cmdloop("Welcome to the RootCanal Console \n" +
                             'Type \'help\' for more information.')


if __name__ == "__main__":
  main(sys.argv)
