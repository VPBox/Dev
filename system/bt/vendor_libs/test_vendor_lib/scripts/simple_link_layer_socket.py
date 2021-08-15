#
# Copyright 2018 The Android Open Source Project
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


     length   type   source    dest      addr_type  adv_type     EIR Data

              adv                          public  connectable  Name (TATA)
send 18000000 07 010203040506 000000000000   00      00         050954415441
                                                                020106

     length   adv   source    dest        public   non-conn     Name (TETE)
send 18000000 07 010203040507 000000000000   00       03        050954455445
                                                                020106

     length scan_rsp  source    dest        public  scan_rsp    Name (TFTF)
send 18000000 09 010203040506 4de24c67454b    00      04        050954465446
                                                                020106

"""

#!/usr/bin/env python

import binascii
import cmd
import random
import socket
import string
import struct
import sys

DEVICE_NAME_LENGTH = 6
DEVICE_ADDRESS_LENGTH = 6


# Used to generate fake device names and addresses during discovery.
def generate_random_name():
  return ''.join(random.SystemRandom().choice(string.ascii_uppercase + \
    string.digits) for _ in range(DEVICE_NAME_LENGTH))


def generate_random_address():
  return ''.join(random.SystemRandom().choice(string.digits) for _ in \
    range(DEVICE_ADDRESS_LENGTH))


class Connection(object):
  """Simple wrapper class for a socket object.

  Attributes:
    socket: The underlying socket created for the specified address and port.
  """

  def __init__(self, port):
    self._socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    self._socket.connect(('localhost', port))
    self._socket.setblocking(0)

  def close(self):
    self._socket.close()

  def send(self, data):
    self._socket.sendall(data)

  def receive(self, size):
    return self._socket.recv(size)


class RawPort(object):
  """Checks outgoing commands and sends them once verified.

  Attributes:
    connection: The connection to the HCI port.
  """

  def __init__(self, port):
    self._connection = Connection(port)
    self._closed = False

  def close(self):
    self._connection.close()
    self._closed = True

  def send_binary(self, args):
    joined_args = ''.join(arg for arg in args)
    print(joined_args)
    packet = binascii.a2b_hex(joined_args)
    if self._closed:
      return
    self._connection.send(packet)

  def receive_response(self):
    if self._closed:
      return
    size_chars = self._connection.receive(4)
    if not size_chars:
      print('Debug: No response')
      return False
    size_bytes = bytearray(size_chars)
    response_size = 0
    for i in range(0, len(size_chars) - 1):
      response_size |= ord(size_chars[i]) << (8 * i)
    response = self._connection.receive(response_size)
    return response

  def lint_command(self, name, args, name_size, args_size):
    assert name_size == len(name) and args_size == len(args)
    try:
      name.encode('utf-8')
      for arg in args:
        arg.encode('utf-8')
    except UnicodeError:
      print('Unrecognized characters.')
      raise
    if name_size > 255 or args_size > 255:
      raise ValueError  # Size must be encodable in one octet.
    for arg in args:
      if len(arg) > 255:
        raise ValueError  # Size must be encodable in one octet.


class RawPortShell(cmd.Cmd):
  """Shell for sending binary data to a port."""

  def __init__(self, raw_port):
    cmd.Cmd.__init__(self)
    self._raw_port = raw_port

  def do_send(self, args):
    """Arguments: dev_type_str Add a new device of type dev_type_str."""
    self._raw_port.send_binary(args.split())

  def do_quit(self, args):
    """Arguments: None.

    Exits.
    """
    self._raw_port.close()
    print('Goodbye.')
    return True

  def do_help(self, args):
    """Arguments: [dev_num [attr]] List the commands available, optionally filtered by device and attr."""
    if (len(args) == 0):
      cmd.Cmd.do_help(self, args)


def main(argv):
  if len(argv) != 2:
    print('Usage: python raw_port.py [port]')
    return
  try:
    port = int(argv[1])
  except ValueError:
    print('Error parsing port.')
  else:
    try:
      raw_port = RawPort(port)
    except (socket.error, e):
      print('Error connecting to socket: %s' % e)
    except:
      print('Error creating (check arguments).')
    else:
      raw_port_shell = RawPortShell(raw_port)
      raw_port_shell.prompt = '$ '
      raw_port_shell.cmdloop('Welcome to the RootCanal Console \n' +
                             'Type \'help\' for more information.')


if __name__ == '__main__':
  main(sys.argv)
