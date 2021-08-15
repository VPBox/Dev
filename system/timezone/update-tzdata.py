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

"""Generates the timezone data files used by Android."""

from __future__ import print_function

import glob
import os
import re
import subprocess
import sys
import tarfile
import tempfile

sys.path.append('%s/external/icu/tools' % os.environ.get('ANDROID_BUILD_TOP'))
import i18nutil
import icuutil
import tzdatautil


# Calculate the paths that are referred to by multiple functions.
android_build_top = i18nutil.GetAndroidRootOrDie()
timezone_dir = os.path.realpath('%s/system/timezone' % android_build_top)
i18nutil.CheckDirExists(timezone_dir, 'system/timezone')

android_host_out = i18nutil.GetAndroidHostOutOrDie()

zone_compactor_dir = os.path.realpath('%s/system/timezone/zone_compactor' % android_build_top)
i18nutil.CheckDirExists(timezone_dir, 'system/timezone/zone_zompactor')

timezone_input_tools_dir = os.path.realpath('%s/input_tools' % timezone_dir)
timezone_input_data_dir = os.path.realpath('%s/input_data' % timezone_dir)

timezone_output_data_dir = '%s/output_data' % timezone_dir
i18nutil.CheckDirExists(timezone_output_data_dir, 'output_data')

tmp_dir = tempfile.mkdtemp('-tzdata')

def GenerateZicInputFile(extracted_iana_data_dir):
  # Android APIs assume DST means "summer time" so we follow the rearguard format
  # introduced in 2018e.
  zic_input_file_name = 'rearguard.zi'

  # 'NDATA=' is used to remove unnecessary rules files.
  subprocess.check_call(['make', '-C', extracted_iana_data_dir, 'NDATA=', zic_input_file_name])

  zic_input_file = '%s/%s' % (extracted_iana_data_dir, zic_input_file_name)
  if not os.path.exists(zic_input_file):
    print('Could not find %s' % zic_input_file)
    sys.exit(1)
  return zic_input_file


def WriteSetupFile(zic_input_file):
  """Writes the list of zones that ZoneCompactor should process."""
  links = []
  zones = []
  for line in open(zic_input_file):
    fields = line.split()
    if fields:
      if fields[0] == 'Link':
        links.append('%s %s %s' % (fields[0], fields[1], fields[2]))
        zones.append(fields[2])
      elif fields[0] == 'Zone':
        zones.append(fields[1])
  zones.sort()

  zone_compactor_setup_file = '%s/setup' % tmp_dir
  setup = open(zone_compactor_setup_file, 'w')
  for link in sorted(set(links)):
    setup.write('%s\n' % link)
  for zone in sorted(set(zones)):
    setup.write('%s\n' % zone)
  setup.close()
  return zone_compactor_setup_file


def BuildIcuData(iana_data_tar_file):
  icu_build_dir = '%s/icu' % tmp_dir

  icuutil.PrepareIcuBuild(icu_build_dir)
  icuutil.MakeTzDataFiles(icu_build_dir, iana_data_tar_file)

  # Create ICU system image files.
  icuutil.MakeAndCopyIcuDataFiles(icu_build_dir)

  icu_overlay_dir = '%s/icu_overlay' % timezone_output_data_dir

  # Create the ICU overlay time zone file.
  icu_overlay_dat_file = '%s/icu_tzdata.dat' % icu_overlay_dir
  icuutil.MakeAndCopyOverlayTzIcuData(icu_build_dir, icu_overlay_dat_file)

  # Copy ICU license file(s)
  icuutil.CopyLicenseFiles(icu_overlay_dir)


def GetIanaVersion(iana_tar_file):
  iana_tar_filename = os.path.basename(iana_tar_file)
  iana_version = re.search('tz(?:data|code)(.+)\\.tar\\.gz', iana_tar_filename).group(1)
  return iana_version


def ExtractTarFile(tar_file, dir):
  print('Extracting %s...' % tar_file)
  if not os.path.exists(dir):
    os.mkdir(dir)
  tar = tarfile.open(tar_file, 'r')
  tar.extractall(dir)


def BuildZic(iana_tools_dir):
  iana_zic_code_tar_file = tzdatautil.GetIanaTarFile(iana_tools_dir, 'code')
  iana_zic_code_version = GetIanaVersion(iana_zic_code_tar_file)
  iana_zic_data_tar_file = tzdatautil.GetIanaTarFile(iana_tools_dir, 'data')
  iana_zic_data_version = GetIanaVersion(iana_zic_data_tar_file)

  print('Found IANA zic release %s/%s in %s/%s ...' \
      % (iana_zic_code_version, iana_zic_data_version, iana_zic_code_tar_file, iana_zic_data_tar_file))

  zic_build_dir = '%s/zic' % tmp_dir
  ExtractTarFile(iana_zic_code_tar_file, zic_build_dir)
  ExtractTarFile(iana_zic_data_tar_file, zic_build_dir)

  # zic
  print('Building zic...')
  # VERSION_DEPS= is to stop the build process looking for files that might not
  # be present across different versions.
  subprocess.check_call(['make', '-C', zic_build_dir, 'zic'])

  zic_binary_file = '%s/zic' % zic_build_dir
  if not os.path.exists(zic_binary_file):
    print('Could not find %s' % zic_binary_file)
    sys.exit(1)
  return zic_binary_file


def BuildTzdata(zic_binary_file, extracted_iana_data_dir, iana_data_version):
  print('Generating zic input file...')
  zic_input_file = GenerateZicInputFile(extracted_iana_data_dir)

  print('Calling zic...')
  zic_output_dir = '%s/data' % tmp_dir
  os.mkdir(zic_output_dir)
  zic_cmd = [zic_binary_file, '-d', zic_output_dir, zic_input_file]
  subprocess.check_call(zic_cmd)

  # ZoneCompactor
  zone_compactor_setup_file = WriteSetupFile(zic_input_file)

  print('Calling ZoneCompactor to update tzdata to %s...' % iana_data_version)
  subprocess.check_call(['make', '-C', android_build_top, '-j30', 'zone_compactor'])

  # Create args for ZoneCompactor
  zone_tab_file = '%s/zone.tab' % extracted_iana_data_dir
  jar_file = '%s/framework/zone_compactor.jar' % android_host_out
  header_string = 'tzdata%s' % iana_data_version

  print('Executing ZoneCompactor...')
  iana_output_data_dir = '%s/iana' % timezone_output_data_dir
  subprocess.check_call(['java', '-jar', jar_file,
                         zone_compactor_setup_file, zic_output_dir, zone_tab_file,
                         iana_output_data_dir, header_string])


def BuildTzlookup(iana_data_dir):
  countryzones_source_file = '%s/android/countryzones.txt' % timezone_input_data_dir
  tzlookup_dest_file = '%s/android/tzlookup.xml' % timezone_output_data_dir

  print('Calling TzLookupGenerator to create tzlookup.xml...')
  subprocess.check_call(['make', '-C', android_build_top, '-j30', 'tzlookup_generator'])

  jar_file = '%s/framework/tzlookup_generator.jar' % android_host_out
  zone_tab_file = '%s/zone.tab' % iana_data_dir
  subprocess.check_call(['java', '-jar', jar_file,
                         countryzones_source_file, zone_tab_file, tzlookup_dest_file])


def CreateDistroFiles(iana_data_version, output_distro_dir, output_version_file):
  create_distro_script = '%s/distro/tools/create-distro.py' % timezone_dir

  tzdata_file = '%s/iana/tzdata' % timezone_output_data_dir
  icu_file = '%s/icu_overlay/icu_tzdata.dat' % timezone_output_data_dir
  tzlookup_file = '%s/android/tzlookup.xml' % timezone_output_data_dir

  distro_file_pattern = '%s/*.zip' % output_distro_dir
  existing_files = glob.glob(distro_file_pattern)

  print('Removing %s' % existing_files)
  for existing_file in existing_files:
    os.remove(existing_file)

  subprocess.check_call([create_distro_script,
      '-iana_version', iana_data_version,
      '-tzdata', tzdata_file,
      '-icu', icu_file,
      '-tzlookup', tzlookup_file,
      '-output_distro_dir', output_distro_dir,
      '-output_version_file', output_version_file])

def UpdateTestFiles():
  testing_data_dir = '%s/testing/data' % timezone_dir
  update_test_files_script = '%s/create-test-data.sh' % testing_data_dir
  subprocess.check_call([update_test_files_script], cwd=testing_data_dir)


# Run with no arguments from any directory, with no special setup required.
# See http://www.iana.org/time-zones/ for more about the source of this data.
def main():
  print('Source data file structure: %s' % timezone_input_data_dir)
  print('Source tools file structure: %s' % timezone_input_tools_dir)
  print('Output data file structure: %s' % timezone_output_data_dir)

  iana_input_data_dir = '%s/iana' % timezone_input_data_dir
  iana_data_tar_file = tzdatautil.GetIanaTarFile(iana_input_data_dir, 'data')
  iana_data_version = GetIanaVersion(iana_data_tar_file)
  print('IANA time zone data release %s in %s ...' % (iana_data_version, iana_data_tar_file))

  icu_dir = icuutil.icuDir()
  print('Found icu in %s ...' % icu_dir)

  BuildIcuData(iana_data_tar_file)

  iana_tools_dir = '%s/iana' % timezone_input_tools_dir
  zic_binary_file = BuildZic(iana_tools_dir)

  iana_data_dir = '%s/iana_data' % tmp_dir
  ExtractTarFile(iana_data_tar_file, iana_data_dir)
  BuildTzdata(zic_binary_file, iana_data_dir, iana_data_version)
  BuildTzlookup(iana_data_dir)

  # Create a distro file and version file from the output from prior stages.
  output_distro_dir = '%s/distro' % timezone_output_data_dir
  output_version_file = '%s/version/tz_version' % timezone_output_data_dir
  CreateDistroFiles(iana_data_version, output_distro_dir, output_version_file)

  # Update test versions of distro files too.
  UpdateTestFiles()

  print('Look in %s and %s for new files' % (timezone_output_data_dir, icu_dir))
  sys.exit(0)


if __name__ == '__main__':
  main()
