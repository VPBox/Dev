#!/usr/bin/python -B

# Copyright 2017 The Android Open Source Project
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

"""Downloads the latest IANA timezone data."""

from __future__ import print_function

import ftplib
import os
import shutil
import subprocess
import sys

sys.path.append('%s/external/icu/tools' % os.environ.get('ANDROID_BUILD_TOP'))
import i18nutil
import tzdatautil

# Calculate the paths that are referred to by multiple functions.
android_build_top = i18nutil.GetAndroidRootOrDie()
iana_data_dir = os.path.realpath('%s/system/timezone/input_data/iana' % android_build_top)

def FtpRetrieveFile(ftp, filename):
  ftp.retrbinary('RETR %s' % filename, open(filename, 'wb').write)

def CheckSignature(data_filename, signature_filename):
  """Checks the signature of a file."""
  print('Verifying signature...')
  try:
    subprocess.check_call(['gpg', '--trusted-key=ED97E90E62AA7E34', '--verify',
                          signature_filename, data_filename])
  except subprocess.CalledProcessError as err:
    print('Unable to verify signature')
    print('\n\n******')
    print('If this fails for you, you probably need to import Paul Eggert''s public key:')
    print('  gpg --receive-keys ED97E90E62AA7E34')
    print('******\n\n')
    raise

# Run with no arguments from any directory, with no special setup required.
# See http://www.iana.org/time-zones/ for more about the source of this data.
def main():
  print('Looking for new IANA tzdata...')

  tar_prefix = 'tzdata20'
  # Use the input tzdata version name if it exists
  if len(sys.argv) > 1:
    tar_prefix = 'tzdata' + sys.argv[1]

  iana_tar_filenames = []

  ftp = ftplib.FTP('ftp.iana.org')
  ftp.login()
  ftp.cwd('tz/releases')
  for filename in ftp.nlst():
    if "/" in filename:
      print("FTP server returned bogus file name")
      sys.exit(1)

    if filename.startswith(tar_prefix) and filename.endswith('.tar.gz'):
      iana_tar_filenames.append(filename)
  iana_tar_filenames.sort(reverse=True)

  if len(iana_tar_filenames) == 0:
    print('No tzdata files found')
    sys.exit(1)

  latest_iana_tar_filename = iana_tar_filenames[0]

  local_iana_tar_file = tzdatautil.GetIanaTarFile(iana_data_dir, 'data')

  if local_iana_tar_file:
    local_iana_tar_filename = os.path.basename(local_iana_tar_file)
    if latest_iana_tar_filename <= local_iana_tar_filename:
      print('Available data %s is older or the same as current data %s' % (latest_iana_tar_filename, local_iana_tar_filename))
      sys.exit(0)

  print('Found new tzdata: %s' % latest_iana_tar_filename)
  i18nutil.SwitchToNewTemporaryDirectory()

  print('Downloading data (%s)...' % latest_iana_tar_filename)
  FtpRetrieveFile(ftp, latest_iana_tar_filename)

  signature_filename = '%s.asc' % latest_iana_tar_filename
  print('Downloading signature (%s)...' % signature_filename)
  FtpRetrieveFile(ftp, signature_filename)

  CheckSignature(latest_iana_tar_filename, signature_filename)

  new_local_iana_tar_file = '%s/%s' % (iana_data_dir, latest_iana_tar_filename)
  shutil.copyfile(latest_iana_tar_filename, new_local_iana_tar_file)
  new_local_signature_file = '%s/%s' % (iana_data_dir, signature_filename)
  shutil.copyfile(signature_filename, new_local_signature_file)

  # Delete the existing local IANA tar file, if there is one.
  if local_iana_tar_file:
    os.remove(local_iana_tar_file)

  local_signature_file = '%s.asc' % local_iana_tar_file
  if os.path.exists(local_signature_file):
    os.remove(local_signature_file)

  print('Look in %s for new IANA data files' % new_local_iana_tar_file)
  sys.exit(0)


if __name__ == '__main__':
  main()
