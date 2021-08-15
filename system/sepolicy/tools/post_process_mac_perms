#!/usr/bin/env python
#
# Copyright (C) 2013 The Android Open Source Project
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

"""
Tool to help modify an existing mac_permissions.xml with additional app
certs not already found in that policy. This becomes useful when a directory
containing apps is searched and the certs from those apps are added to the
policy not already explicitly listed.
"""

import sys
import os
import argparse
from base64 import b16encode, b64decode
import fileinput
import re
import subprocess
import zipfile

PEM_CERT_RE = """-----BEGIN CERTIFICATE-----
(.+?)
-----END CERTIFICATE-----
"""
def collect_certs_for_app(filename):
  app_certs = set()
  with zipfile.ZipFile(filename, 'r') as apkzip:
    for info in apkzip.infolist():
      name = info.filename
      if name.startswith('META-INF/') and name.endswith(('.DSA', '.RSA')):
        cmd = ['openssl', 'pkcs7', '-inform', 'DER',
               '-outform', 'PEM', '-print_certs']
        p = subprocess.Popen(cmd, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE)
        pem_string, err = p.communicate(apkzip.read(name))
        if err and err.strip():
          raise RuntimeError('Problem running openssl on %s (%s)' % (filename, e))

        # turn multiline base64 to single line base16
        transform = lambda x: b16encode(b64decode(x.replace('\n', ''))).lower()
        results = re.findall(PEM_CERT_RE, pem_string, re.DOTALL)
        certs = [transform(i) for i in results]

        app_certs.update(certs)

  return app_certs

def add_leftover_certs(args):
  all_app_certs = set()
  for dirpath, _, files in os.walk(args.dir):
    transform = lambda x: os.path.join(dirpath, x)
    condition = lambda x: x.endswith('.apk')
    apps = [transform(i) for i in files if condition(i)]

    # Collect certs for each app found
    for app in apps:
      app_certs = collect_certs_for_app(app)
      all_app_certs.update(app_certs)

  if all_app_certs:
    policy_certs = set()
    with open(args.policy, 'r') as f:
      cert_pattern = 'signature="([a-fA-F0-9]+)"'
      policy_certs = re.findall(cert_pattern, f.read())

    cert_diff = all_app_certs.difference(policy_certs)

    # Build xml stanzas
    inner_tag = '<seinfo value="%s"/>' % args.seinfo
    stanza = '<signer signature="%s">%s</signer>'
    new_stanzas = [stanza % (cert, inner_tag) for cert in cert_diff]
    mac_perms_string = ''.join(new_stanzas)
    mac_perms_string += '</policy>'

    # Inline replace with new policy stanzas
    for line in fileinput.input(args.policy, inplace=True):
      sys.stdout.write(line.replace('</policy>', mac_perms_string))

def main(argv):
  parser = argparse.ArgumentParser(description=__doc__)

  parser.add_argument('-s', '--seinfo', dest='seinfo', required=True,
                      help='seinfo tag for each generated stanza')
  parser.add_argument('-d', '--dir', dest='dir', required=True,
                      help='Directory to search for apks')
  parser.add_argument('-f', '--file', dest='policy', required=True,
                      help='mac_permissions.xml policy file')

  parser.set_defaults(func=add_leftover_certs)
  args = parser.parse_args()
  args.func(args)

if __name__ == '__main__':
  main(sys.argv)
