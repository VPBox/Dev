#!/usr/bin/env python
#
# Copyright 2018, The Android Open Source Project
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

import argparse
import datetime
import logging
import json
import os
import shutil
import subprocess
import sys
import webbrowser

from run_host_unit_tests import *

"""
This script is used to generate code coverage results host supported libraries.
The script by default will generate an html report that summarizes the coverage
results of the specified tests. The results can also be browsed to provide a
report of which lines have been traveled upon execution of the binary.

NOTE: Code that is compiled out or hidden by a #DEFINE will be listed as
having been executed 0 times, thus reducing overall coverage.

The steps in order to add coverage support to a new library and its
corrisponding host test are as follows.

1. Add "clang_file_coverage" (defined in //build/Android.bp) as a default to the
   source library(s) you want statistics for.
   NOTE: Forgoing this default will cause no coverage data to be generated for
         the source files in the library.

2. Add "clang_coverage_bin" as a default to the host supported test binary that
   excercises the libraries that you covered in step 1.
   NOTE: Forgetting to add this will cause there to be *NO* coverage data
         generated when the binary is run.

3. Add the host test binary name and the files/directories you want coverage
   statistics for to the COVERAGE_TESTS variable defined below. You may add
   individual filenames or a directory to be tested.
   NOTE: Avoid using a / at the beginning of a covered_files entry as this
         breaks how the coverage generator resolves filenames.

TODO: Support generating XML data and printing results to standard out.
"""

COVERAGE_TESTS = [
    {
        "test_name": "net_test_avrcp",
        "covered_files": [
            "system/bt/profile/avrcp",
        ],
    }, {
        "test_name": "bluetooth_test_sdp",
        "covered_files": [
            "system/bt/profile/sdp",
        ],
    }, {
        "test_name": "test-vendor_test_host",
        "covered_files": [
            "system/bt/vendor_libs/test_vendor_lib/include",
            "system/bt/vendor_libs/test_vendor_lib/src",
        ],
    }, {
        "test_name": "rootcanal-packets_test_host",
        "covered_files": [
            "system/bt/vendor_libs/test_vendor_lib/packets",
        ],
    }, {
        "test_name": "bluetooth_test_common",
        "covered_files": [
            "system/bt/common",
        ],
    },
]

WORKING_DIR = '/tmp/coverage'
SOONG_UI_BASH = 'build/soong/soong_ui.bash'
LLVM_DIR = 'prebuilts/clang/host/linux-x86/clang-r328903/bin'
LLVM_MERGE = LLVM_DIR + '/llvm-profdata'
LLVM_COV = LLVM_DIR + '/llvm-cov'

def write_root_html_head(f):
  # Write the header part of the root html file. This was pulled from the
  # page source of one of the generated html files.
  f.write("<!doctype html><html><head>" \
    "<meta name='viewport' content='width=device-width,initial-scale=1'><met" \
    "a charset='UTF-8'><link rel='stylesheet' type='text/css' href='style.cs" \
    "s'></head><body><h2>Coverage Report</h2><h4>Created: " +
    str(datetime.datetime.now().strftime('%Y-%m-%d %H:%M')) +
    "</h4><p>Click <a href='http://clang.llvm.org/docs/SourceBasedCodeCovera" \
    "ge.html#interpreting-reports'>here</a> for information about interpreti" \
    "ng this report.</p><div class='centered'><table><tr><td class='column-e" \
    "ntry-bold'>Filename</td><td class='column-entry-bold'>Function Coverage" \
    "</td><td class='column-entry-bold'>Instantiation Coverage</td><td class" \
    "='column-entry-bold'>Line Coverage</td><td class='column-entry-bold'>Re" \
    "gion Coverage</td></tr>"
  )


def write_root_html_column(f, covered, count):
  percent = covered * 100.0 / count
  value = "%.2f%% (%d/%d) " % (percent, covered, count)
  color = 'column-entry-yellow'
  if percent == 100:
    color = 'column-entry-green'
  if percent < 80.0:
    color = 'column-entry-red'
  f.write("<td class=\'" + color + "\'><pre>" + value + "</pre></td>")


def write_root_html_rows(f, tests):
  totals = {
      "functions":{
          "covered": 0,
          "count": 0
      },
      "instantiations":{
          "covered": 0,
          "count": 0
      },
      "lines":{
          "covered": 0,
          "count": 0
      },
      "regions":{
          "covered": 0,
          "count": 0
      }
  }

  # Write the tests with their coverage summaries.
  for test in tests:
    test_name = test['test_name']
    covered_files = test['covered_files']
    json_results = generate_coverage_json(test)
    test_totals = json_results['data'][0]['totals']

    f.write("<tr class='light-row'><td><pre><a href=\'" +
        os.path.join(test_name, "index.html") + "\'>" + test_name +
        "</a></pre></td>")
    for field_name in ['functions', 'instantiations', 'lines', 'regions']:
      field = test_totals[field_name]
      totals[field_name]['covered'] += field['covered']
      totals[field_name]['count'] += field['count']
      write_root_html_column(f, field['covered'], field['count'])
    f.write("</tr>");

  #Write the totals row.
  f.write("<tr class='light-row-bold'><td><pre>Totals</a></pre></td>")
  for field_name in ['functions', 'instantiations', 'lines', 'regions']:
    field = totals[field_name]
    write_root_html_column(f, field['covered'], field['count'])
  f.write("</tr>");


def write_root_html_tail(f):
  # Pulled from the generated html coverage report.
  f.write("</table></div><h5>Generated by llvm-cov -- llvm version 7.0.2svn<" \
    "/h5></body></html>")


def generate_root_html(tests):
  # Copy the css file from one of the coverage reports.
  source_file = os.path.join(os.path.join(WORKING_DIR, tests[0]['test_name']), "style.css")
  dest_file = os.path.join(WORKING_DIR, "style.css")
  shutil.copy2(source_file, dest_file)

  # Write the root index.html file that sumarizes all the tests.
  f = open(os.path.join(WORKING_DIR, "index.html"), "w")
  write_root_html_head(f)
  write_root_html_rows(f, tests)
  write_root_html_tail(f)


def get_profraw_for_test(test_name):
  test_root = get_native_test_root_or_die()
  test_cmd = os.path.join(os.path.join(test_root, test_name), test_name)
  if not os.path.isfile(test_cmd):
    logging.error('The test ' + test_name + ' does not exist, please compile first')
    sys.exit(1)

  profraw_file_name = test_name + ".profraw"
  profraw_path = os.path.join(WORKING_DIR, os.path.join(test_name, profraw_file_name))
  llvm_env_var = "LLVM_PROFILE_FILE=\"" + profraw_path + "\""

  test_cmd = llvm_env_var + " " + test_cmd
  logging.info('Generating profraw data for ' + test_name)
  logging.debug('cmd: ' + test_cmd)
  if subprocess.call(test_cmd, shell=True) != 0:
    logging.error('Test ' + test_name + ' failed. Please fix the test before generating coverage.')
    sys.exit(1)

  if not os.path.isfile(profraw_path):
    logging.error('Generating the profraw file failed. Did you remember to add the proper compiler flags to your build?')
    sys.exit(1)

  return profraw_file_name


def merge_profraw_data(test_name):
  cmd = []
  cmd.append(os.path.join(get_android_root_or_die(), LLVM_MERGE + " merge "))

  test_working_dir = os.path.join(WORKING_DIR, test_name);
  cmd.append(os.path.join(test_working_dir, test_name + ".profraw"))
  profdata_file = os.path.join(test_working_dir, test_name + ".profdata")

  cmd.append('-o ' + profdata_file)
  logging.info('Combining profraw files into profdata for ' + test_name)
  logging.debug('cmd: ' + " ".join(cmd))
  if subprocess.call(" ".join(cmd), shell=True) != 0:
    logging.error('Failed to merge profraw files for ' + test_name)
    sys.exit(1)


def generate_coverage_html(test):
  COVERAGE_ROOT = '/proc/self/cwd'

  test_name = test['test_name']
  file_list = test['covered_files']

  test_working_dir = os.path.join(WORKING_DIR, test_name)
  test_profdata_file = os.path.join(test_working_dir, test_name + ".profdata")

  cmd = [
    os.path.join(get_android_root_or_die(), LLVM_COV),
    "show",
    "-format=html",
    "-summary-only",
    "-show-line-counts-or-regions",
    "-show-instantiation-summary",
    "-instr-profile=" + test_profdata_file,
    "-path-equivalence=\"" + COVERAGE_ROOT + "\",\"" +
        get_android_root_or_die() + "\"",
    "-output-dir=" + test_working_dir
  ]

  # We have to have one object file not as an argument otherwise we can't specify source files.
  test_cmd = os.path.join(os.path.join(get_native_test_root_or_die(), test_name), test_name)
  cmd.append(test_cmd)

  # Filter out the specific files we want coverage for
  for filename in file_list:
    cmd.append(os.path.join(get_android_root_or_die(), filename))

  logging.info('Generating coverage report for ' + test['test_name'])
  logging.debug('cmd: ' + " ".join(cmd))
  if subprocess.call(" ".join(cmd), shell=True) != 0:
    logging.error('Failed to generate coverage for ' + test['test_name'])
    sys.exit(1)


def generate_coverage_json(test):
  COVERAGE_ROOT = '/proc/self/cwd'
  test_name = test['test_name']
  file_list = test['covered_files']

  test_working_dir = os.path.join(WORKING_DIR, test_name)
  test_profdata_file = os.path.join(test_working_dir, test_name + ".profdata")

  cmd = [
    os.path.join(get_android_root_or_die(), LLVM_COV),
    "export",
    "-summary-only",
    "-show-region-summary",
    "-instr-profile=" + test_profdata_file,
    "-path-equivalence=\"" + COVERAGE_ROOT + "\",\"" + get_android_root_or_die() + "\"",
  ]

  test_cmd = os.path.join(os.path.join(get_native_test_root_or_die(), test_name), test_name)
  cmd.append(test_cmd)

  # Filter out the specific files we want coverage for
  for filename in file_list:
    cmd.append(os.path.join(get_android_root_or_die(), filename))

  logging.info('Generating coverage json for ' + test['test_name'])
  logging.debug('cmd: ' + " ".join(cmd))

  json_str = subprocess.check_output(" ".join(cmd), shell=True)
  return json.loads(json_str)


def write_json_summary(test):
  test_name = test['test_name']
  test_working_dir = os.path.join(WORKING_DIR, test_name)
  test_json_summary_file = os.path.join(test_working_dir, test_name + '.json')
  logging.debug('Writing json summary file: ' + test_json_summary_file)
  json_file = open(test_json_summary_file, 'w')
  json.dump(generate_coverage_json(test), json_file)
  json_file.close()


def list_tests():
  for test in COVERAGE_TESTS:
    print "Test Name: " + test['test_name']
    print "Covered Files: "
    for covered_file in test['covered_files']:
        print "  " + covered_file
    print


def main():
  parser = argparse.ArgumentParser(description='Generate code coverage for enabled tests.')
  parser.add_argument(
    '-l', '--list-tests',
    action='store_true',
    dest='list_tests',
    help='List all the available tests to be run as well as covered files.')
  parser.add_argument(
    '-a', '--all',
    action='store_true',
    help='Runs all available tests and prints their outputs. If no tests ' \
         'are specified via the -t option all tests will be run.')
  parser.add_argument(
    '-t', '--test',
    dest='tests',
    action='append',
    type=str,
    metavar='TESTNAME',
    default=[],
    help='Specifies a test to be run. Multiple tests can be specified by ' \
         'using this option multiple times. ' \
         'Example: \"gen_coverage.py -t test1 -t test2\"')
  parser.add_argument(
    '-o', '--output',
    type=str,
    metavar='DIRECTORY',
    default='/tmp/coverage',
    help='Specifies the directory to store all files. The directory will be ' \
         'created if it does not exist. Default is \"/tmp/coverage\"')
  parser.add_argument(
    '-s', '--skip-html',
    dest='skip_html',
    action='store_true',
    help='Skip opening up the results of the coverage report in a browser.')
  parser.add_argument(
    '-j', '--json-file',
    dest='json_file',
    action='store_true',
    help='Write out summary results to json file in test directory.')

  logging.basicConfig(stream=sys.stderr, level=logging.DEBUG, format='%(levelname)s %(message)s')
  logging.addLevelName(logging.DEBUG, "[\033[1;34m%s\033[0m]" % logging.getLevelName(logging.DEBUG))
  logging.addLevelName(logging.INFO, "[\033[1;34m%s\033[0m]" % logging.getLevelName(logging.INFO))
  logging.addLevelName(logging.WARNING, "[\033[1;31m%s\033[0m]" % logging.getLevelName(logging.WARNING))
  logging.addLevelName(logging.ERROR, "[\033[1;31m%s\033[0m]" % logging.getLevelName(logging.ERROR))

  args = parser.parse_args()
  logging.debug("Args: " + str(args))

  # Set the working directory
  global WORKING_DIR
  WORKING_DIR = os.path.abspath(args.output)
  logging.debug("Working Dir: " + WORKING_DIR)

  # Print out the list of tests then exit
  if args.list_tests:
    list_tests()
    sys.exit(0)

  # Check to see if a test was specified and if so only generate coverage for
  # that test.
  if len(args.tests) == 0:
    args.all = True

  tests_to_run = []
  for test in COVERAGE_TESTS:
    if args.all or test['test_name'] in args.tests:
      tests_to_run.append(test)
    if test['test_name'] in args.tests:
      args.tests.remove(test['test_name'])

  # Error if a test was specified but doesn't exist.
  if len(args.tests) != 0:
    for test_name in args.tests:
        logging.error('\"' + test_name + '\" was not found in the list of available tests.')
    sys.exit(1)

  # Generate the info for the tests
  for test in tests_to_run:
    logging.info('Getting coverage for ' + test['test_name'])
    get_profraw_for_test(test['test_name'])
    merge_profraw_data(test['test_name'])
    if args.json_file:
      write_json_summary(test)
    generate_coverage_html(test)

  # Generate the root index.html page that sumarizes all of the coverage reports.
  generate_root_html(tests_to_run)

  # Open the results in a browser.
  if not args.skip_html:
    webbrowser.open('file://' + os.path.join(WORKING_DIR, 'index.html'))


if __name__ == '__main__':
  main()
