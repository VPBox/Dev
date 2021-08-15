#!/usr/bin/python2.7

# Copyright 2010, The Android Open Source Project
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


"""RenderScript Compiler Test.

Runs subdirectories of tests for the RenderScript compiler.
"""

import filecmp
import glob
import os
import re
import shutil
import subprocess
import sys
import unittest


__author__ = 'Android'


DOTTED_LINE = '................'


class SlangTests(unittest.TestCase):
  """Class to contain all the unittest test cases.

  Tests will be dynamically added to this class as methods.
  No static tests, so this class is initially empty.
  See GenerateSlangTest() and AddSlangUnitTests().

  """
  pass


def GenerateSlangTest(dir_name):
  """Creates a test method that can be added as method to SlangTests."""
  cwd = os.getcwd()
  def SlangTest(self):
    os.chdir(cwd)
    ExecTest(dir_name, self)
  return SlangTest


def AddSlangUnitTests(test_dirs):
  """Adds a test to SlangTests for each directory in test_dirs."""

  for t in test_dirs:
    # Must start with 'test_' according to unittest
    test_name = 'test_%s' % t
    test = GenerateSlangTest(t)
    # Add test as method to SlangTests with test_name as method name
    setattr(SlangTests, test_name, test)


class Options(object):
  verbose = 0
  cleanup = 1
  update_cts = 0
  zero_return = 0


def CompareFiles(actual, expect):
  """Compares actual and expect for equality."""
  if not os.path.isfile(actual):
    if Options.verbose:
      print 'Could not find %s' % actual
    return False
  if not os.path.isfile(expect):
    if Options.verbose:
      print 'Could not find %s' % expect
    return False

  return filecmp.cmp(actual, expect, False)


def CopyIfDifferent(src, dst):
  """Updates dst if it is different from src."""
  if not CompareFiles(src, dst):
    if Options.verbose:
      print 'Copying from %s to %s' % (src, dst)
    shutil.copyfile(src, dst)


def GetCommandLineArgs(filename):
  """Extracts command line arguments from first comment line in a file."""
  f = open(filename, 'r')
  line = f.readline()
  f.close()
  if line[0] == '/' and line[1] == '/':
    return line[2:].strip()
  else:
    return ''


def ReadFileToStr(filename):
  """Returns contents of file as a str."""
  with open(filename, 'r') as f:
    return f.read()


def ReportIfDifferFromExpected(tests, name, file1, file2):
  """Fails tests if file1 and file2 differ."""
  if not CompareFiles(file1, file2):
    if Options.verbose:
      err_message = ('%s is different:\n'
                     'expected:\n%s\n%s%s\n\n'
                     'actual:\n%s\n%s%s\n') % (
                         name,
                         DOTTED_LINE, ReadFileToStr(file1), DOTTED_LINE,
                         DOTTED_LINE, ReadFileToStr(file2), DOTTED_LINE)
    else:
      err_message = '%s is different' % name
    tests.fail(err_message)


def GetRSFiles():
  """Returns a list of files in cwd with extension '.rs' or '.fs'."""
  rs_files = glob.glob('*.rs')
  fs_files = glob.glob('*.fs')
  rs_files += fs_files
  rs_files.sort()
  return rs_files


def GetOutDir():
  """Returns the directory with llvm-rs-cc."""
  # If cache has not yet been calculated, do that
  if GetOutDir.cache is None:
    try:
      # ANDROID_HOST_OUT is set after `lunch` on local builds
      GetOutDir.cache = os.environ['ANDROID_HOST_OUT']
    except KeyError:
      # On build server, we need to get the HOST_OUT Makefile variable
      # because ANDROID_HOST_OUT is not exported on build server
      GetOutDir.cache = subprocess.check_output(['bash', '-c',
                                         'cd ../../../../.. ; '
                                         'source build/envsetup.sh '
                                         '1> /dev/null 2> /dev/null ; '
                                         'realpath `get_build_var HOST_OUT`'])
      GetOutDir.cache = GetOutDir.cache.strip()
  return GetOutDir.cache


# Declare/define cache variable for GetOutDir to cache results
# This way we only need to call subprocesses once to get the directory
GetOutDir.cache = None


def CreateCmd():
  """Creates the test command to run for the current test."""
  cmd_string = ('%s/bin/llvm-rs-cc -o tmp/ -p tmp/ -MD '
                '-I ../../../../../frameworks/rs/script_api/include/ '
                '-I ../../../../../external/clang/lib/Headers/') % GetOutDir()
  base_args = cmd_string.split()
  rs_files = GetRSFiles()

  # Extra command line arguments can be placed as // comments at the start of
  # any .rs file. We automatically bundle up all of these extra args and invoke
  # llvm-rs-cc with them.
  extra_args_str = ''
  for rs_file in rs_files:
    extra_args_str += GetCommandLineArgs(rs_file)
  extra_args = extra_args_str.split()

  args = base_args + extra_args + rs_files
  return args


def UpdateCTS():
  """Copies resulting files to appropriate CTS directory (if different)."""
  if glob.glob('IN_CTS'):
    cts_path = '../../../../../cts/'
    cts_res_raw_path = cts_path + 'tests/tests/renderscriptlegacy/res/raw/'
    cts_src_path = cts_path + 'tests/tests/renderscript/src/'
    for bc_src in glob.glob('tmp/*.bc'):
      bc_dst = re.sub(r'tmp\/', cts_res_raw_path, bc_src, 1)
      CopyIfDifferent(bc_src, bc_dst)
    for java_src in glob.glob('tmp/android/renderscript/cts/*.java'):
      java_dst = re.sub(r'tmp\/', cts_src_path, java_src, 1)
      CopyIfDifferent(java_src, java_dst)


def Cleanup():
  """Cleans up the cwd of any tmp files created in current test."""
  try:
    os.remove('stdout.txt')
    os.remove('stderr.txt')
    shutil.rmtree('tmp/')
  except OSError:
    pass


def CheckTestResult(dir_name, subprocess_ret, tests, args):
  """Checks the result of the subprocess command to see if it passed/failed.

  If dir_name starts with 'F_', then subprocess is expected to fail.
  If it instead succeeded, then this test is failed.
  Vice versa with a dir_name starting with 'P_'.

  Args:
    dir_name: name of current directory/test name
    subprocess_ret: return code of subprocess
    tests: unittest, call tests.fail(reason) when failure
    args: the arguments for the command that was run
  """
  if dir_name[0:2] == 'F_':
    if subprocess_ret == 0:
      if Options.verbose:
        err_message = ('Command (%s) passed on invalid input\n'
                       'stdout:\n%s\n%s%s\n') % (
                           ' '.join(args),
                           DOTTED_LINE, ReadFileToStr('stdout.txt'), DOTTED_LINE
                       )
      else:
        err_message = 'Command passed on invalid input'
      tests.fail(err_message)
  elif dir_name[0:2] == 'P_':
    if subprocess_ret != 0:
      if Options.verbose:
        err_message = ('Command (%s) failed on valid input\n'
                       'stderr:\n%s\n%s%s\n') % (
                           ' '.join(args),
                           DOTTED_LINE, ReadFileToStr('stderr.txt'), DOTTED_LINE
                       )
      else:
        err_message = 'Command failed on valid input'
      tests.fail(err_message)
  else:
    tests.fail('Invalid test name: ' + dir_name +
               ', should start with F_ or P_')


def CheckJavaOutput(tests):
  """Check that the Java output files are as expected.

  Each 'Script*.java.expect' file should have exactly one corresponding file.
  The two files should match exactly.

  Args:
    tests: unittest, call tests.fail(reason) when failure
  """
  java_expect = glob.glob('Script*.java.expect')
  for expect in java_expect:
    expect_base = expect[:-7]  # strip ".expect" suffix
    find = 'tmp/*/' + expect_base
    found = glob.glob(find)
    if len(found) != 1:
      if not found:
        tests.fail('%s not found' % find)
      else:
        tests.fail('multiple %s found' % find)
    elif not CompareFiles(found[0], expect):
      tests.fail('%s and %s are different' % (found[0], expect))


def ExecTest(dir_name, tests):
  """Executes an llvm-rs-cc test from dir_name."""

  os.chdir(dir_name)
  stdout_file = open('stdout.txt', 'w+')
  stderr_file = open('stderr.txt', 'w+')

  args = CreateCmd()

  if Options.verbose > 1:
    print 'Executing:', ' '.join(args)

  # Execute the command and check the resulting shell return value.
  # All tests that are expected to FAIL have directory names that
  # start with 'F_'. Other tests that are expected to PASS have
  # directory names that start with 'P_'.
  ret = 0
  try:
    ret = subprocess.call(args, stdout=stdout_file, stderr=stderr_file)
  except OSError:
    tests.fail('subprocess.call failed: ' + ' '.join(args))

  stdout_file.close()
  stderr_file.close()

  CheckTestResult(dir_name, ret, tests, args)

  ReportIfDifferFromExpected(tests, 'stdout', 'stdout.txt.expect', 'stdout.txt')
  ReportIfDifferFromExpected(tests, 'stderr', 'stderr.txt.expect', 'stderr.txt')

  CheckJavaOutput(tests)

  if Options.update_cts:
    UpdateCTS()

  if Options.cleanup:
    Cleanup()


def Usage():
  """Print out usage information."""
  print ('Usage: %s [OPTION]... [TESTNAME]...'
         'Renderscript Compiler Test Harness\n'
         'Runs TESTNAMEs (all tests by default)\n'
         'Available Options:\n'
         '  -h, --help          Help message\n'
         '  -n, --no-cleanup    Don\'t clean up after running tests\n'
         '  -u, --update-cts    Update CTS test versions\n'
         '  -v, --verbose       Verbose output.  Enter multiple -v to get more verbose.\n'
         '  -z, --zero-return   Return 0 as exit code no matter if tests fail. Required for TreeHugger.\n'
        ) % (sys.argv[0]),
  return


def main():
  """Runs the unittest suite.

  Parses command line arguments, adds test directories as tests.

  Returns:
    0 if '-z' flag is set.
    Else unittest.main() returns with its own error code.
  """

  # Chdir to the directory this file is in since tests are in this directory
  os.chdir(os.path.dirname(os.path.abspath(__file__)))
  files = []
  for arg in sys.argv[1:]:
    if arg in ('-h', '--help'):
      Usage()
      return 0
    elif arg in ('-n', '--no-cleanup'):
      Options.cleanup = 0
    elif arg in ('-u', '--update-cts'):
      Options.update_cts = 1
    elif arg in ('-v', '--verbose'):
      Options.verbose += 1
    elif arg in ('-z', '--zero-return'):
      Options.zero_return = 1
    else:
      # Test list to run
      if os.path.isdir(arg):
        files.append(arg)
      else:
        print >> sys.stderr, 'Invalid test or option: %s' % arg
        return 1

  if not files:
    file_names = os.listdir('.')
    # Test names must start with 'F_' or 'P_'
    # 'F_' tests are expected to fail
    # 'P_' tests are expected to pass
    for f in file_names:
      if os.path.isdir(f) and (f[0:2] == 'F_' or f[0:2] == 'P_'):
        files.append(f)
    files.sort()

  AddSlangUnitTests(files)

  # verbosity=2 is necessary for PythonUnitTestRunner to parse the results
  # Otherwise verbosity does not matter
  # If Options.zero_return is set, do not let unittest.main() exit
  #  This is necessary in TreeHugger to distinguish between failing tests and
  #  failing to execute the python script
  # If Options.zero_return is not set, let unittest.main() exit
  #  In this case it will return a non-zero code if any tests fail
  unittest_exit = Options.zero_return == 0
  unittest.main(verbosity=2,
                argv=[sys.argv[0]] + ['SlangTests'],
                exit=unittest_exit)

  return 0


if __name__ == '__main__':
  sys.exit(main())

