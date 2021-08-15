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
"""Script for sending testing parameters and commands to a Bluetooth device.

This script provides a simple shell interface for sending data at run-time to a
Bluetooth device. It is intended to be used in tandem with the test vendor
library project.

Usage:
  Option A: Script
    1. Run build_and_run.sh in scripts/ with the --test-channel flag set and the
    port to use for the test channel.
  Option B: Manual
    1. Choose a port to use for the test channel. Use 'adb forward tcp:<port>
    tcp:<port>' to forward the port to the device.
    2. In a separate shell, build and push the test vendor library to the device
    using the script mentioned in option A (i.e. without the --test-channel flag
    set).
    3. Once logcat has started, turn Bluetooth on from the device.
    4. Run this program, in the shell from step 1,  the port, also from step 1,
    as arguments.
"""

#!/usr/bin/env python

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

  def close(self):
    self._socket.close()

  def send(self, data):
    self._socket.sendall(data)

  def receive(self, size):
    return self._socket.recv(size)


class TestChannel(object):
  """Checks outgoing commands and sends them once verified.

  Attributes:
    connection: The connection to the test vendor library that commands are sent
      on.
  """

  def __init__(self, port):
    self._connection = Connection(port)
    self._closed = False

  def close(self):
    self._connection.close()
    self._closed = True

  def send_command(self, name, args):
    name_size = len(name)
    args_size = len(args)
    self.lint_command(name, args, name_size, args_size)
    encoded_name = chr(name_size) + name
    encoded_args = chr(args_size) + ''.join(chr(len(arg)) + arg for arg in args)
    command = encoded_name + encoded_args
    if self._closed:
      return
    self._connection.send(command)
    if name != 'CLOSE_TEST_CHANNEL':
      print self.receive_response()

  def receive_response(self):
    if self._closed:
      return
    size_chars = self._connection.receive(4)
    size_bytes = bytearray(size_chars)
    if not size_chars:
      print 'No response, assuming that the connection is broken'
      return False
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
      print 'Unrecognized characters.'
      raise
    if name_size > 255 or args_size > 255:
      raise ValueError  # Size must be encodable in one octet.
    for arg in args:
      if len(arg) > 255:
        raise ValueError  # Size must be encodable in one octet.


class TestChannelShell(cmd.Cmd):
  """Shell for sending test channel data to controller.

  Manages the test channel to the controller and defines a set of commands the
  user can send to the controller as well. These commands are processed parallel
  to commands sent from the device stack and used to provide additional
  debugging/testing capabilities.

  Attributes:
    test_channel: The communication channel to send data to the controller.
  """

  def __init__(self, test_channel):
    cmd.Cmd.__init__(self)
    self._test_channel = test_channel

  def do_add(self, args):
    """Arguments: dev_type_str Add a new device of type dev_type_str.

    """
    self._test_channel.send_command('add', args.split())

  def do_del(self, args):
    """Arguments: device index Delete the device with the specified index.

    """
    self._test_channel.send_command('del', args.split())

  def do_add_phy(self, args):
    """Arguments: dev_type_str Add a new device of type dev_type_str.

    """
    self._test_channel.send_command('add_phy', args.split())

  def do_del_phy(self, args):
    """Arguments: phy index Delete the phy with the specified index.

    """
    self._test_channel.send_command('del_phy', args.split())

  def do_add_device_to_phy(self, args):
    """Arguments: device index phy index Add a new device of type dev_type_str.

    """
    self._test_channel.send_command('add_device_to_phy', args.split())

  def do_del_device_from_phy(self, args):
    """Arguments: phy index Delete the phy with the specified index.

    """
    self._test_channel.send_command('del_device_from_phy', args.split())

  def do_add_remote(self, args):
    """Arguments: dev_type_str Connect to a remote device at arg1@arg2.

    """
    self._test_channel.send_command('add_remote', args.split())

  def do_get(self, args):
    """Arguments: dev_num attr_str Get the value of the attribute attr_str from device dev_num.

    """
    self._test_channel.send_command('get', args.split())

  def do_set(self, args):
    """Arguments: dev_num attr_str val Set the value of the attribute attr_str from device dev_num equal to val.

    """
    self._test_channel.send_command('set', args.split())

  def do_list(self, args):
    """Arguments: [dev_num [attr]] List the devices from the controller, optionally filtered by device and attr.

    """
    self._test_channel.send_command('list', args.split())

  def do_quit(self, args):
    """Arguments: None.

    Exits the test channel.
    """
    self._test_channel.send_command('CLOSE_TEST_CHANNEL', [])
    self._test_channel.close()
    print 'Goodbye.'
    return True

  def do_help(self, args):
    """Arguments: [dev_num [attr]] List the commands available, optionally filtered by device and attr.

    """
    if (len(args) == 0):
      cmd.Cmd.do_help(self, args)
    else:
      self._test_channel.send_command('help', args.split())

  def preloop(self):
    """Clear out the buffer

    """
    response = self._test_channel.receive_response()

  #def postcmd(self, stop, line):
  #"""
  #Called after each command
  #stop : whether we will stop after this command
  #line : the previous input line
  #Return True to stop, False to continue
  #"""
  #if stop:
  #return True
  #response = self._test_channel.receive_response()
  #if not response:
  #return True
  #print response
  #return False


def main(argv):
  if len(argv) != 2:
    print 'Usage: python test_channel.py [port]'
    return
  try:
    port = int(argv[1])
  except ValueError:
    print 'Error parsing port.'
  else:
    try:
      test_channel = TestChannel(port)
    except socket.error, e:
      print 'Error connecting to socket: %s' % e
    except:
      print 'Error creating test channel (check argument).'
    else:
      test_channel_shell = TestChannelShell(test_channel)
      test_channel_shell.prompt = '$ '
      test_channel_shell.cmdloop('Welcome to the RootCanal Console \n' +
                                 'Type \'help\' for more information.')


if __name__ == '__main__':
  main(sys.argv)
