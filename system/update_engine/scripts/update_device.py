#!/usr/bin/python2
#
# Copyright (C) 2017 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

"""Send an A/B update to an Android device over adb."""

import argparse
import BaseHTTPServer
import hashlib
import logging
import os
import socket
import subprocess
import sys
import threading
import xml.etree.ElementTree
import zipfile

import update_payload.payload


# The path used to store the OTA package when applying the package from a file.
OTA_PACKAGE_PATH = '/data/ota_package'

# The path to the payload public key on the device.
PAYLOAD_KEY_PATH = '/etc/update_engine/update-payload-key.pub.pem'

# The port on the device that update_engine should connect to.
DEVICE_PORT = 1234

def CopyFileObjLength(fsrc, fdst, buffer_size=128 * 1024, copy_length=None):
  """Copy from a file object to another.

  This function is similar to shutil.copyfileobj except that it allows to copy
  less than the full source file.

  Args:
    fsrc: source file object where to read from.
    fdst: destination file object where to write to.
    buffer_size: size of the copy buffer in memory.
    copy_length: maximum number of bytes to copy, or None to copy everything.

  Returns:
    the number of bytes copied.
  """
  copied = 0
  while True:
    chunk_size = buffer_size
    if copy_length is not None:
      chunk_size = min(chunk_size, copy_length - copied)
      if not chunk_size:
        break
    buf = fsrc.read(chunk_size)
    if not buf:
      break
    fdst.write(buf)
    copied += len(buf)
  return copied


class AndroidOTAPackage(object):
  """Android update payload using the .zip format.

  Android OTA packages traditionally used a .zip file to store the payload. When
  applying A/B updates over the network, a payload binary is stored RAW inside
  this .zip file which is used by update_engine to apply the payload. To do
  this, an offset and size inside the .zip file are provided.
  """

  # Android OTA package file paths.
  OTA_PAYLOAD_BIN = 'payload.bin'
  OTA_PAYLOAD_PROPERTIES_TXT = 'payload_properties.txt'

  def __init__(self, otafilename):
    self.otafilename = otafilename

    otazip = zipfile.ZipFile(otafilename, 'r')
    payload_info = otazip.getinfo(self.OTA_PAYLOAD_BIN)
    self.offset = payload_info.header_offset
    self.offset += zipfile.sizeFileHeader
    self.offset += len(payload_info.extra) + len(payload_info.filename)
    self.size = payload_info.file_size
    self.properties = otazip.read(self.OTA_PAYLOAD_PROPERTIES_TXT)


class UpdateHandler(BaseHTTPServer.BaseHTTPRequestHandler):
  """A HTTPServer that supports single-range requests.

  Attributes:
    serving_payload: path to the only payload file we are serving.
    serving_range: the start offset and size tuple of the payload.
  """

  @staticmethod
  def _parse_range(range_str, file_size):
    """Parse an HTTP range string.

    Args:
      range_str: HTTP Range header in the request, not including "Header:".
      file_size: total size of the serving file.

    Returns:
      A tuple (start_range, end_range) with the range of bytes requested.
    """
    start_range = 0
    end_range = file_size

    if range_str:
      range_str = range_str.split('=', 1)[1]
      s, e = range_str.split('-', 1)
      if s:
        start_range = int(s)
        if e:
          end_range = int(e) + 1
      elif e:
        if int(e) < file_size:
          start_range = file_size - int(e)
    return start_range, end_range


  def do_GET(self):  # pylint: disable=invalid-name
    """Reply with the requested payload file."""
    if self.path != '/payload':
      self.send_error(404, 'Unknown request')
      return

    if not self.serving_payload:
      self.send_error(500, 'No serving payload set')
      return

    try:
      f = open(self.serving_payload, 'rb')
    except IOError:
      self.send_error(404, 'File not found')
      return
    # Handle the range request.
    if 'Range' in self.headers:
      self.send_response(206)
    else:
      self.send_response(200)

    serving_start, serving_size = self.serving_range
    start_range, end_range = self._parse_range(self.headers.get('range'),
                                               serving_size)
    logging.info('Serving request for %s from %s [%d, %d) length: %d',
                 self.path, self.serving_payload, serving_start + start_range,
                 serving_start + end_range, end_range - start_range)

    self.send_header('Accept-Ranges', 'bytes')
    self.send_header('Content-Range',
                     'bytes ' + str(start_range) + '-' + str(end_range - 1) +
                     '/' + str(end_range - start_range))
    self.send_header('Content-Length', end_range - start_range)

    stat = os.fstat(f.fileno())
    self.send_header('Last-Modified', self.date_time_string(stat.st_mtime))
    self.send_header('Content-type', 'application/octet-stream')
    self.end_headers()

    f.seek(serving_start + start_range)
    CopyFileObjLength(f, self.wfile, copy_length=end_range - start_range)


  def do_POST(self):  # pylint: disable=invalid-name
    """Reply with the omaha response xml."""
    if self.path != '/update':
      self.send_error(404, 'Unknown request')
      return

    if not self.serving_payload:
      self.send_error(500, 'No serving payload set')
      return

    try:
      f = open(self.serving_payload, 'rb')
    except IOError:
      self.send_error(404, 'File not found')
      return

    content_length = int(self.headers.getheader('Content-Length'))
    request_xml = self.rfile.read(content_length)
    xml_root = xml.etree.ElementTree.fromstring(request_xml)
    appid = None
    for app in xml_root.iter('app'):
      if 'appid' in app.attrib:
        appid = app.attrib['appid']
        break
    if not appid:
      self.send_error(400, 'No appid in Omaha request')
      return

    self.send_response(200)
    self.send_header("Content-type", "text/xml")
    self.end_headers()

    serving_start, serving_size = self.serving_range
    sha256 = hashlib.sha256()
    f.seek(serving_start)
    bytes_to_hash = serving_size
    while bytes_to_hash:
      buf = f.read(min(bytes_to_hash, 1024 * 1024))
      if not buf:
        self.send_error(500, 'Payload too small')
        return
      sha256.update(buf)
      bytes_to_hash -= len(buf)

    payload = update_payload.Payload(f, payload_file_offset=serving_start)
    payload.Init()

    response_xml = '''
        <?xml version="1.0" encoding="UTF-8"?>
        <response protocol="3.0">
          <app appid="{appid}">
            <updatecheck status="ok">
              <urls>
                <url codebase="http://127.0.0.1:{port}/"/>
              </urls>
              <manifest version="0.0.0.1">
                <actions>
                  <action event="install" run="payload"/>
                  <action event="postinstall" MetadataSize="{metadata_size}"/>
                </actions>
                <packages>
                  <package hash_sha256="{payload_hash}" name="payload" size="{payload_size}"/>
                </packages>
              </manifest>
            </updatecheck>
          </app>
        </response>
    '''.format(appid=appid, port=DEVICE_PORT,
               metadata_size=payload.metadata_size,
               payload_hash=sha256.hexdigest(),
               payload_size=serving_size)
    self.wfile.write(response_xml.strip())
    return


class ServerThread(threading.Thread):
  """A thread for serving HTTP requests."""

  def __init__(self, ota_filename, serving_range):
    threading.Thread.__init__(self)
    # serving_payload and serving_range are class attributes and the
    # UpdateHandler class is instantiated with every request.
    UpdateHandler.serving_payload = ota_filename
    UpdateHandler.serving_range = serving_range
    self._httpd = BaseHTTPServer.HTTPServer(('127.0.0.1', 0), UpdateHandler)
    self.port = self._httpd.server_port

  def run(self):
    try:
      self._httpd.serve_forever()
    except (KeyboardInterrupt, socket.error):
      pass
    logging.info('Server Terminated')

  def StopServer(self):
    self._httpd.socket.close()


def StartServer(ota_filename, serving_range):
  t = ServerThread(ota_filename, serving_range)
  t.start()
  return t


def AndroidUpdateCommand(ota_filename, payload_url, extra_headers):
  """Return the command to run to start the update in the Android device."""
  ota = AndroidOTAPackage(ota_filename)
  headers = ota.properties
  headers += 'USER_AGENT=Dalvik (something, something)\n'
  headers += 'NETWORK_ID=0\n'
  headers += extra_headers

  return ['update_engine_client', '--update', '--follow',
          '--payload=%s' % payload_url, '--offset=%d' % ota.offset,
          '--size=%d' % ota.size, '--headers="%s"' % headers]


def OmahaUpdateCommand(omaha_url):
  """Return the command to run to start the update in a device using Omaha."""
  return ['update_engine_client', '--update', '--follow',
          '--omaha_url=%s' % omaha_url]


class AdbHost(object):
  """Represents a device connected via ADB."""

  def __init__(self, device_serial=None):
    """Construct an instance.

    Args:
        device_serial: options string serial number of attached device.
    """
    self._device_serial = device_serial
    self._command_prefix = ['adb']
    if self._device_serial:
      self._command_prefix += ['-s', self._device_serial]

  def adb(self, command):
    """Run an ADB command like "adb push".

    Args:
      command: list of strings containing command and arguments to run

    Returns:
      the program's return code.

    Raises:
      subprocess.CalledProcessError on command exit != 0.
    """
    command = self._command_prefix + command
    logging.info('Running: %s', ' '.join(str(x) for x in command))
    p = subprocess.Popen(command, universal_newlines=True)
    p.wait()
    return p.returncode

  def adb_output(self, command):
    """Run an ADB command like "adb push" and return the output.

    Args:
      command: list of strings containing command and arguments to run

    Returns:
      the program's output as a string.

    Raises:
      subprocess.CalledProcessError on command exit != 0.
    """
    command = self._command_prefix + command
    logging.info('Running: %s', ' '.join(str(x) for x in command))
    return subprocess.check_output(command, universal_newlines=True)


def main():
  parser = argparse.ArgumentParser(description='Android A/B OTA helper.')
  parser.add_argument('otafile', metavar='PAYLOAD', type=str,
                      help='the OTA package file (a .zip file) or raw payload \
                      if device uses Omaha.')
  parser.add_argument('--file', action='store_true',
                      help='Push the file to the device before updating.')
  parser.add_argument('--no-push', action='store_true',
                      help='Skip the "push" command when using --file')
  parser.add_argument('-s', type=str, default='', metavar='DEVICE',
                      help='The specific device to use.')
  parser.add_argument('--no-verbose', action='store_true',
                      help='Less verbose output')
  parser.add_argument('--public-key', type=str, default='',
                      help='Override the public key used to verify payload.')
  parser.add_argument('--extra-headers', type=str, default='',
                      help='Extra headers to pass to the device.')
  args = parser.parse_args()
  logging.basicConfig(
      level=logging.WARNING if args.no_verbose else logging.INFO)

  dut = AdbHost(args.s)

  server_thread = None
  # List of commands to execute on exit.
  finalize_cmds = []
  # Commands to execute when canceling an update.
  cancel_cmd = ['shell', 'su', '0', 'update_engine_client', '--cancel']
  # List of commands to perform the update.
  cmds = []

  help_cmd = ['shell', 'su', '0', 'update_engine_client', '--help']
  use_omaha = 'omaha' in dut.adb_output(help_cmd)

  if args.file:
    # Update via pushing a file to /data.
    device_ota_file = os.path.join(OTA_PACKAGE_PATH, 'debug.zip')
    payload_url = 'file://' + device_ota_file
    if not args.no_push:
      data_local_tmp_file = '/data/local/tmp/debug.zip'
      cmds.append(['push', args.otafile, data_local_tmp_file])
      cmds.append(['shell', 'su', '0', 'mv', data_local_tmp_file,
                   device_ota_file])
      cmds.append(['shell', 'su', '0', 'chcon',
                   'u:object_r:ota_package_file:s0', device_ota_file])
    cmds.append(['shell', 'su', '0', 'chown', 'system:cache', device_ota_file])
    cmds.append(['shell', 'su', '0', 'chmod', '0660', device_ota_file])
  else:
    # Update via sending the payload over the network with an "adb reverse"
    # command.
    payload_url = 'http://127.0.0.1:%d/payload' % DEVICE_PORT
    if use_omaha and zipfile.is_zipfile(args.otafile):
      ota = AndroidOTAPackage(args.otafile)
      serving_range = (ota.offset, ota.size)
    else:
      serving_range = (0, os.stat(args.otafile).st_size)
    server_thread = StartServer(args.otafile, serving_range)
    cmds.append(
        ['reverse', 'tcp:%d' % DEVICE_PORT, 'tcp:%d' % server_thread.port])
    finalize_cmds.append(['reverse', '--remove', 'tcp:%d' % DEVICE_PORT])

  if args.public_key:
    payload_key_dir = os.path.dirname(PAYLOAD_KEY_PATH)
    cmds.append(
        ['shell', 'su', '0', 'mount', '-t', 'tmpfs', 'tmpfs', payload_key_dir])
    # Allow adb push to payload_key_dir
    cmds.append(['shell', 'su', '0', 'chcon', 'u:object_r:shell_data_file:s0',
                 payload_key_dir])
    cmds.append(['push', args.public_key, PAYLOAD_KEY_PATH])
    # Allow update_engine to read it.
    cmds.append(['shell', 'su', '0', 'chcon', '-R', 'u:object_r:system_file:s0',
                 payload_key_dir])
    finalize_cmds.append(['shell', 'su', '0', 'umount', payload_key_dir])

  try:
    # The main update command using the configured payload_url.
    if use_omaha:
      update_cmd = \
          OmahaUpdateCommand('http://127.0.0.1:%d/update' % DEVICE_PORT)
    else:
      update_cmd = \
          AndroidUpdateCommand(args.otafile, payload_url, args.extra_headers)
    cmds.append(['shell', 'su', '0'] + update_cmd)

    for cmd in cmds:
      dut.adb(cmd)
  except KeyboardInterrupt:
    dut.adb(cancel_cmd)
  finally:
    if server_thread:
      server_thread.StopServer()
    for cmd in finalize_cmds:
      dut.adb(cmd)

  return 0

if __name__ == '__main__':
  sys.exit(main())
