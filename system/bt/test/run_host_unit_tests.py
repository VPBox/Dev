#!/usr/bin/env python
#
# Copyright 2017, The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
import sys
import subprocess
import os
import argparse

# Registered host based unit tests
# Must have 'host_supported: true'
HOST_TESTS = [
  'bluetooth_test_common',
  'bluetoothtbd_test',
  'net_test_avrcp',
  'net_test_btcore',
  'net_test_types',
  'net_test_btpackets',
]

SOONG_UI_BASH = 'build/soong/soong_ui.bash'

def str2bool(argument, default=False):
  """ Convert a string to a booleen value. """
  argument = str(argument)
  if argument.lower() in ['0', 'f', 'false', 'off', 'no', 'n']:
    return False
  elif argument.lower() in ['1', 't', 'true', 'on', 'yes', 'y']:
    return True
  return default


def check_dir_exists(dir, dirname):
  if not os.path.isdir(dir):
    print "Couldn't find %s (%s)!" % (dirname, dir)
    sys.exit(0)


def get_output_from_command(cmd):
  try:
    return subprocess.check_output(cmd).strip()
  except subprocess.CalledProcessError as e:
    print 'Failed to call {cmd}, return code {code}'.format(cmd=cmd,
                                                            code=e.returncode)
    print e
    return None


def get_android_root_or_die():
  value = os.environ.get('ANDROID_BUILD_TOP')
  if not value:
    # Try to find build/soong/soong_ui.bash upwards until root directory
    current_path = os.path.abspath(os.getcwd())
    while current_path and os.path.isdir(current_path):
      soong_ui_bash_path = os.path.join(current_path, SOONG_UI_BASH)
      if os.path.isfile(soong_ui_bash_path):
        # Use value returned from Soong UI instead in case definition to TOP
        # changes in the future
        value = get_output_from_command((soong_ui_bash_path,
                                         '--dumpvar-mode',
                                         '--abs',
                                         'TOP'))
        break
      parent_path = os.path.abspath(os.path.join(current_path, os.pardir))
      if parent_path == current_path:
        current_path = None
      else:
        current_path = parent_path
    if not value:
      print 'Cannot determine ANDROID_BUILD_TOP'
      sys.exit(1)
  check_dir_exists(value, '$ANDROID_BUILD_TOP')
  return value


def get_android_host_out_or_die():
  value = os.environ.get('ANDROID_HOST_OUT')
  if not value:
    ANDROID_BUILD_TOP = get_android_root_or_die()
    value = get_output_from_command((os.path.join(ANDROID_BUILD_TOP,
                                                  SOONG_UI_BASH),
                                     '--dumpvar-mode',
                                     '--abs',
                                     'HOST_OUT'))
    if not value:
      print 'Cannot determine ANDROID_HOST_OUT'
      sys.exit(1)
  check_dir_exists(value, '$ANDROID_HOST_OUT')
  return value


def get_android_dist_dir_or_die():
  # Check if $DIST_DIR is predefined as environment variable
  value = os.environ.get('DIST_DIR')
  if not value:
    # If not use the default path
    ANDROID_BUILD_TOP = get_android_root_or_die()
    value = os.path.join(os.path.join(ANDROID_BUILD_TOP, 'out'), 'dist')
  if not os.path.isdir(value):
    if os.path.exists(value):
      print '%s is not a directory!' % (value)
      sys.exit(1)
    os.makedirs(value)
  return value


def get_native_test_root_or_die():
  android_host_out = get_android_host_out_or_die()
  test_root = os.path.join(android_host_out, 'nativetest64')
  if not os.path.isdir(test_root):
    test_root = os.path.join(android_host_out, 'nativetest')
    if not os.path.isdir(test_root):
      print 'Neither nativetest64 nor nativetest directory exist,' \
            ' please compile first'
      sys.exit(1)
  return test_root


def get_test_cmd_or_die(test_root, test_name, enable_xml, test_filter):
  test_path = os.path.join(os.path.join(test_root, test_name), test_name)
  if not os.path.isfile(test_path):
    print 'Cannot find: ' + test_path
    sys.exit(1)
  cmd = [test_path]
  if enable_xml:
    dist_dir = get_android_dist_dir_or_die()
    log_output_path = os.path.join(dist_dir, 'gtest/{0}_test_details.xml'
                                   .format(test_name))
    cmd.append('--gtest_output=xml:{0}'.format(log_output_path))
  if test_filter:
    cmd.append('--gtest_filter=%s' % test_filter)
  return cmd


# path is relative to Android build top
def build_target(target, num_tasks):
  ANDROID_BUILD_TOP = get_android_root_or_die()
  build_cmd = [SOONG_UI_BASH, '--make-mode']
  if num_tasks > 1:
    build_cmd.append('-j' + str(num_tasks))
  build_cmd.append(target)
  p = subprocess.Popen(build_cmd, cwd=ANDROID_BUILD_TOP, env=os.environ.copy())
  return_code = p.wait()
  if return_code != 0:
    print 'BUILD FAILED, return code: {0}'.format(str(return_code))
    sys.exit(1)
  return


def main():
  """ run_host_unit_tests.py - Run registered host based unit tests
  """
  parser = argparse.ArgumentParser(description='Run host based unit tests.')
  parser.add_argument(
      '--enable_xml',
      type=str2bool,
      dest='enable_xml',
      nargs='?',
      const=True,
      default=False,
      help=
      'Whether to output structured XML log output in out/dist/gtest directory')
  parser.add_argument(
      '-j',
      type=int,
      nargs='?',
      dest='num_tasks',
      const=-1,
      default=-1,
      help='Number of tasks to run at the same time')
  parser.add_argument(
      'rest',
      nargs=argparse.REMAINDER,
      help='-- args, other gtest arguments for each individual test')
  args = parser.parse_args()

  build_target('MODULES-IN-system-bt', args.num_tasks)
  TEST_ROOT = get_native_test_root_or_die()
  test_results = []
  for test in HOST_TESTS:
    test_cmd = get_test_cmd_or_die(TEST_ROOT, test, args.enable_xml, args.rest)
    if subprocess.call(test_cmd) != 0:
      test_results.append(False)
    else:
      test_results.append(True)
  if not all(test_results):
    failures = [i for i, x in enumerate(test_results) if not x]
    for index in failures:
      print 'TEST FAILLED: ' + HOST_TESTS[index]
    sys.exit(0)
  print 'TEST PASSED ' + str(len(test_results)) + ' tests were run'

  dist_dir = get_android_dist_dir_or_die()
  log_output_path = os.path.join(dist_dir, 'gtest/coverage')
  cmd_path = os.path.join(get_android_root_or_die(), 'system/bt/test')
  print cmd_path
  cmd = [
    os.path.join(cmd_path, 'gen_coverage.py'),
    '--skip-html',
    '--json-file',
    '-o',
    log_output_path,
  ]
  subprocess.call(cmd)

  sys.exit(0)


if __name__ == '__main__':
  main()
