#!/usr/bin/env python
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
"""test.py: Tests for simpleperf python scripts.

These are smoke tests Using examples to run python scripts.
For each example, we go through the steps of running each python script.
Examples are collected from simpleperf/demo, which includes:
  SimpleperfExamplePureJava
  SimpleperfExampleWithNative
  SimpleperfExampleOfKotlin

Tested python scripts include:
  app_profiler.py
  report.py
  annotate.py
  report_sample.py
  pprof_proto_generator.py
  report_html.py

Test using both `adb root` and `adb unroot`.

"""
from __future__ import print_function
import argparse
import filecmp
import fnmatch
import inspect
import os
import re
import shutil
import signal
import subprocess
import sys
import time
import types
import unittest

from app_profiler import NativeLibDownloader
from binary_cache_builder import BinaryCacheBuilder
from simpleperf_report_lib import ReportLib
from utils import log_exit, log_info, log_fatal
from utils import AdbHelper, Addr2Nearestline, bytes_to_str, find_tool_path, get_script_dir
from utils import is_python3, is_windows, Objdump, ReadElf, remove, SourceFileSearcher

try:
    # pylint: disable=unused-import
    import google.protobuf
    HAS_GOOGLE_PROTOBUF = True
except ImportError:
    HAS_GOOGLE_PROTOBUF = False

INFERNO_SCRIPT = os.path.join(get_script_dir(), "inferno.bat" if is_windows() else "./inferno.sh")

def get_device_features():
    adb = AdbHelper()
    adb.check_run_and_return_output(['push',
                                     'bin/android/%s/simpleperf' % adb.get_device_arch(),
                                     '/data/local/tmp'])
    adb.check_run_and_return_output(['shell', 'chmod', 'a+x', '/data/local/tmp/simpleperf'])
    return adb.check_run_and_return_output(['shell', '/data/local/tmp/simpleperf', 'list',
                                            '--show-features'])

def is_trace_offcpu_supported():
    if not hasattr(is_trace_offcpu_supported, 'value'):
        is_trace_offcpu_supported.value = 'trace-offcpu' in get_device_features()
    return is_trace_offcpu_supported.value


def build_testdata():
    """ Collect testdata from ../testdata and ../demo. """
    from_testdata_path = os.path.join('..', 'testdata')
    from_demo_path = os.path.join('..', 'demo')
    from_script_testdata_path = 'script_testdata'
    if (not os.path.isdir(from_testdata_path) or not os.path.isdir(from_demo_path) or
            not from_script_testdata_path):
        return
    copy_testdata_list = ['perf_with_symbols.data', 'perf_with_trace_offcpu.data',
                          'perf_with_tracepoint_event.data', 'perf_with_interpreter_frames.data']
    copy_demo_list = ['SimpleperfExamplePureJava', 'SimpleperfExampleWithNative',
                      'SimpleperfExampleOfKotlin']

    testdata_path = "testdata"
    remove(testdata_path)
    os.mkdir(testdata_path)
    for testdata in copy_testdata_list:
        shutil.copy(os.path.join(from_testdata_path, testdata), testdata_path)
    for demo in copy_demo_list:
        shutil.copytree(os.path.join(from_demo_path, demo), os.path.join(testdata_path, demo))
    for f in os.listdir(from_script_testdata_path):
        shutil.copy(os.path.join(from_script_testdata_path, f), testdata_path)

class TestBase(unittest.TestCase):
    def run_cmd(self, args, return_output=False):
        if args[0].endswith('.py'):
            args = [sys.executable] + args
        use_shell = args[0].endswith('.bat')
        try:
            if not return_output:
                returncode = subprocess.call(args, shell=use_shell)
            else:
                subproc = subprocess.Popen(args, stdout=subprocess.PIPE, shell=use_shell)
                (output_data, _) = subproc.communicate()
                output_data = bytes_to_str(output_data)
                returncode = subproc.returncode
        except OSError:
            returncode = None
        self.assertEqual(returncode, 0, msg="failed to run cmd: %s" % args)
        if return_output:
            return output_data
        return ''


class TestExampleBase(TestBase):
    @classmethod
    def prepare(cls, example_name, package_name, activity_name, abi=None, adb_root=False):
        cls.adb = AdbHelper(enable_switch_to_root=adb_root)
        cls.example_path = os.path.join("testdata", example_name)
        if not os.path.isdir(cls.example_path):
            log_fatal("can't find " + cls.example_path)
        for root, _, files in os.walk(cls.example_path):
            if 'app-profiling.apk' in files:
                cls.apk_path = os.path.join(root, 'app-profiling.apk')
                break
        if not hasattr(cls, 'apk_path'):
            log_fatal("can't find app-profiling.apk under " + cls.example_path)
        cls.package_name = package_name
        cls.activity_name = activity_name
        cls.abi = "arm64"
        if abi and abi != "arm64" and abi.find("arm") != -1:
            cls.abi = "arm"
        args = ["install", "-r"]
        if abi:
            args += ["--abi", abi]
        args.append(cls.apk_path)
        cls.adb.check_run(args)
        cls.adb_root = adb_root
        cls.compiled = False
        cls.has_perf_data_for_report = False
        android_version = cls.adb.get_android_version()
        # On Android >= P (version 9), we can profile JITed and interpreted Java code.
        # So only compile Java code on Android <= O (version 8).
        cls.use_compiled_java_code = android_version <= 8

    def setUp(self):
        if self.id().find('TraceOffCpu') != -1 and not is_trace_offcpu_supported():
            self.skipTest('trace-offcpu is not supported on device')
        cls = self.__class__
        if not cls.has_perf_data_for_report:
            cls.has_perf_data_for_report = True
            self.run_app_profiler()
            shutil.copy('perf.data', 'perf.data_for_report')
            remove('binary_cache_for_report')
            shutil.copytree('binary_cache', 'binary_cache_for_report')
        else:
            shutil.copy('perf.data_for_report', 'perf.data')
            remove('binary_cache')
            shutil.copytree('binary_cache_for_report', 'binary_cache')

    @classmethod
    def tearDownClass(cls):
        if hasattr(cls, 'test_result') and cls.test_result and not cls.test_result.wasSuccessful():
            return
        if hasattr(cls, 'package_name'):
            cls.adb.check_run(["uninstall", cls.package_name])
        remove("binary_cache")
        remove("annotated_files")
        remove("perf.data")
        remove("report.txt")
        remove("pprof.profile")
        if cls.has_perf_data_for_report:
            cls.has_perf_data_for_report = False
            remove('perf.data_for_report')
            remove('binary_cache_for_report')

    def run(self, result=None):
        self.__class__.test_result = result
        super(TestExampleBase, self).run(result)

    def run_app_profiler(self, record_arg="-g --duration 10", build_binary_cache=True,
                         start_activity=True):
        args = ['app_profiler.py', '--app', self.package_name, '-r', record_arg, '-o', 'perf.data']
        if not build_binary_cache:
            args.append("-nb")
        if self.use_compiled_java_code and not self.__class__.compiled:
            args.append('--compile_java_code')
            self.__class__.compiled = True
        if start_activity:
            args += ["-a", self.activity_name]
        args += ["-lib", self.example_path]
        if not self.adb_root:
            args.append("--disable_adb_root")
        self.run_cmd(args)
        self.check_exist(filename="perf.data")
        if build_binary_cache:
            self.check_exist(dirname="binary_cache")

    def check_exist(self, filename=None, dirname=None):
        if filename:
            self.assertTrue(os.path.isfile(filename), filename)
        if dirname:
            self.assertTrue(os.path.isdir(dirname), dirname)

    def check_file_under_dir(self, dirname, filename):
        self.check_exist(dirname=dirname)
        for _, _, files in os.walk(dirname):
            for f in files:
                if f == filename:
                    return
        self.fail("Failed to call check_file_under_dir(dir=%s, file=%s)" % (dirname, filename))


    def check_strings_in_file(self, filename, strings):
        self.check_exist(filename=filename)
        with open(filename, 'r') as fh:
            self.check_strings_in_content(fh.read(), strings)

    def check_strings_in_content(self, content, strings):
        for s in strings:
            self.assertNotEqual(content.find(s), -1, "s: %s, content: %s" % (s, content))

    def check_annotation_summary(self, summary_file, check_entries):
        """ check_entries is a list of (name, accumulated_period, period).
            This function checks for each entry, if the line containing [name]
            has at least required accumulated_period and period.
        """
        self.check_exist(filename=summary_file)
        with open(summary_file, 'r') as fh:
            summary = fh.read()
        fulfilled = [False for x in check_entries]
        summary_check_re = re.compile(r'accumulated_period:\s*([\d.]+)%.*period:\s*([\d.]+)%')
        for line in summary.split('\n'):
            for i, (name, need_acc_period, need_period) in enumerate(check_entries):
                if not fulfilled[i] and name in line:
                    m = summary_check_re.search(line)
                    if m:
                        acc_period = float(m.group(1))
                        period = float(m.group(2))
                        if acc_period >= need_acc_period and period >= need_period:
                            fulfilled[i] = True
        self.assertEqual(len(fulfilled), sum([int(x) for x in fulfilled]), fulfilled)

    def check_inferno_report_html(self, check_entries, filename="report.html"):
        self.check_exist(filename=filename)
        with open(filename, 'r') as fh:
            data = fh.read()
        fulfilled = [False for _ in check_entries]
        for line in data.split('\n'):
            # each entry is a (function_name, min_percentage) pair.
            for i, entry in enumerate(check_entries):
                if fulfilled[i] or line.find(entry[0]) == -1:
                    continue
                m = re.search(r'(\d+\.\d+)%', line)
                if m and float(m.group(1)) >= entry[1]:
                    fulfilled[i] = True
                    break
        self.assertEqual(fulfilled, [True for _ in check_entries])

    def common_test_app_profiler(self):
        self.run_cmd(["app_profiler.py", "-h"])
        remove("binary_cache")
        self.run_app_profiler(build_binary_cache=False)
        self.assertFalse(os.path.isdir("binary_cache"))
        args = ["binary_cache_builder.py"]
        if not self.adb_root:
            args.append("--disable_adb_root")
        self.run_cmd(args)
        self.check_exist(dirname="binary_cache")
        remove("binary_cache")
        self.run_app_profiler(build_binary_cache=True)
        self.run_app_profiler()
        self.run_app_profiler(start_activity=False)

    def common_test_report(self):
        self.run_cmd(["report.py", "-h"])
        self.run_cmd(["report.py"])
        self.run_cmd(["report.py", "-i", "perf.data"])
        self.run_cmd(["report.py", "-g"])
        self.run_cmd(["report.py", "--self-kill-for-testing", "-g", "--gui"])

    def common_test_annotate(self):
        self.run_cmd(["annotate.py", "-h"])
        remove("annotated_files")
        self.run_cmd(["annotate.py", "-s", self.example_path])
        self.check_exist(dirname="annotated_files")

    def common_test_report_sample(self, check_strings):
        self.run_cmd(["report_sample.py", "-h"])
        self.run_cmd(["report_sample.py"])
        output = self.run_cmd(["report_sample.py", "perf.data"], return_output=True)
        self.check_strings_in_content(output, check_strings)

    def common_test_pprof_proto_generator(self, check_strings_with_lines,
                                          check_strings_without_lines):
        if not HAS_GOOGLE_PROTOBUF:
            log_info('Skip test for pprof_proto_generator because google.protobuf is missing')
            return
        self.run_cmd(["pprof_proto_generator.py", "-h"])
        self.run_cmd(["pprof_proto_generator.py"])
        remove("pprof.profile")
        self.run_cmd(["pprof_proto_generator.py", "-i", "perf.data", "-o", "pprof.profile"])
        self.check_exist(filename="pprof.profile")
        self.run_cmd(["pprof_proto_generator.py", "--show"])
        output = self.run_cmd(["pprof_proto_generator.py", "--show", "pprof.profile"],
                              return_output=True)
        self.check_strings_in_content(output, check_strings_with_lines + ["has_line_numbers: True"])
        remove("binary_cache")
        self.run_cmd(["pprof_proto_generator.py"])
        output = self.run_cmd(["pprof_proto_generator.py", "--show", "pprof.profile"],
                              return_output=True)
        self.check_strings_in_content(output, check_strings_without_lines +
                                      ["has_line_numbers: False"])

    def common_test_inferno(self):
        self.run_cmd([INFERNO_SCRIPT, "-h"])
        remove("perf.data")
        append_args = [] if self.adb_root else ["--disable_adb_root"]
        self.run_cmd([INFERNO_SCRIPT, "-p", self.package_name, "-t", "3"] + append_args)
        self.check_exist(filename="perf.data")
        self.run_cmd([INFERNO_SCRIPT, "-p", self.package_name, "-f", "1000", "-du", "-t", "1"] +
                     append_args)
        self.run_cmd([INFERNO_SCRIPT, "-p", self.package_name, "-e", "100000 cpu-cycles",
                      "-t", "1"] + append_args)
        self.run_cmd([INFERNO_SCRIPT, "-sc"])

    def common_test_report_html(self):
        self.run_cmd(['report_html.py', '-h'])
        self.run_cmd(['report_html.py'])
        self.run_cmd(['report_html.py', '--add_source_code', '--source_dirs', 'testdata'])
        self.run_cmd(['report_html.py', '--add_disassembly'])
        # Test with multiple perf.data.
        shutil.move('perf.data', 'perf2.data')
        self.run_app_profiler(record_arg='-g -f 1000 --duration 3 -e task-clock:u')
        self.run_cmd(['report_html.py', '-i', 'perf.data', 'perf2.data'])
        remove('perf2.data')


class TestExamplePureJava(TestExampleBase):
    @classmethod
    def setUpClass(cls):
        cls.prepare("SimpleperfExamplePureJava",
                    "com.example.simpleperf.simpleperfexamplepurejava",
                    ".MainActivity")

    def test_app_profiler(self):
        self.common_test_app_profiler()

    def test_app_profiler_profile_from_launch(self):
        self.run_app_profiler(start_activity=True, build_binary_cache=False)
        self.run_cmd(["report.py", "-g", "-o", "report.txt"])
        self.check_strings_in_file("report.txt", [
            "com.example.simpleperf.simpleperfexamplepurejava.MainActivity$1.run",
            "__start_thread"])

    def test_app_profiler_multiprocesses(self):
        self.adb.check_run(['shell', 'am', 'force-stop', self.package_name])
        self.adb.check_run(['shell', 'am', 'start', '-n',
                            self.package_name + '/.MultiProcessActivity'])
        # Wait until both MultiProcessActivity and MultiProcessService set up.
        time.sleep(3)
        self.run_app_profiler(start_activity=False)
        self.run_cmd(["report.py", "-o", "report.txt"])
        self.check_strings_in_file("report.txt", ["BusyService", "BusyThread"])

    def test_app_profiler_with_ctrl_c(self):
        if is_windows():
            return
        self.adb.check_run(['shell', 'am', 'start', '-n', self.package_name + '/.MainActivity'])
        time.sleep(1)
        args = [sys.executable, "app_profiler.py", "--app", self.package_name,
                "-r", "--duration 10000", "--disable_adb_root"]
        subproc = subprocess.Popen(args)
        time.sleep(3)

        subproc.send_signal(signal.SIGINT)
        subproc.wait()
        self.assertEqual(subproc.returncode, 0)
        self.run_cmd(["report.py"])

    def test_app_profiler_stop_after_app_exit(self):
        self.adb.check_run(['shell', 'am', 'start', '-n', self.package_name + '/.MainActivity'])
        time.sleep(1)
        subproc = subprocess.Popen([sys.executable, 'app_profiler.py', '--app', self.package_name,
                                    '-r', '--duration 10000', '--disable_adb_root'])
        time.sleep(3)
        self.adb.check_run(['shell', 'am', 'force-stop', self.package_name])
        subproc.wait()
        self.assertEqual(subproc.returncode, 0)
        self.run_cmd(["report.py"])

    def test_app_profiler_with_ndk_path(self):
        # Although we pass an invalid ndk path, it should be able to find tools in default ndk path.
        self.run_cmd(['app_profiler.py', '--app', self.package_name, '-a', self.activity_name,
                      '--ndk_path', '.'])

    def test_report(self):
        self.common_test_report()
        self.run_cmd(["report.py", "-g", "-o", "report.txt"])
        self.check_strings_in_file("report.txt", [
            "com.example.simpleperf.simpleperfexamplepurejava.MainActivity$1.run",
            "__start_thread"])

    def test_profile_with_process_id(self):
        self.adb.check_run(['shell', 'am', 'start', '-n', self.package_name + '/.MainActivity'])
        time.sleep(1)
        pid = self.adb.check_run_and_return_output([
            'shell', 'pidof', 'com.example.simpleperf.simpleperfexamplepurejava']).strip()
        self.run_app_profiler(start_activity=False, record_arg='-g --duration 10 -p ' + pid)
        self.run_cmd(["report.py", "-g", "-o", "report.txt"])
        self.check_strings_in_file("report.txt", [
            "com.example.simpleperf.simpleperfexamplepurejava.MainActivity$1.run",
            "__start_thread"])

    def test_annotate(self):
        self.common_test_annotate()
        if not self.use_compiled_java_code:
            # Currently annotating Java code is only supported when the Java code is compiled.
            return
        self.check_file_under_dir("annotated_files", "MainActivity.java")
        summary_file = os.path.join("annotated_files", "summary")
        self.check_annotation_summary(summary_file, [
            ("MainActivity.java", 80, 80),
            ("run", 80, 0),
            ("callFunction", 0, 0),
            ("line 23", 80, 0)])

    def test_report_sample(self):
        self.common_test_report_sample(
            ["com.example.simpleperf.simpleperfexamplepurejava.MainActivity$1.run",
             "__start_thread"])

    def test_pprof_proto_generator(self):
        check_strings_with_lines = []
        if self.use_compiled_java_code:
            check_strings_with_lines = [
                "com/example/simpleperf/simpleperfexamplepurejava/MainActivity.java",
                "run"]
        self.common_test_pprof_proto_generator(
            check_strings_with_lines=check_strings_with_lines,
            check_strings_without_lines=[
                "com.example.simpleperf.simpleperfexamplepurejava.MainActivity$1.run"])

    def test_inferno(self):
        self.common_test_inferno()
        self.run_app_profiler()
        self.run_cmd([INFERNO_SCRIPT, "-sc"])
        self.check_inferno_report_html(
            [('com.example.simpleperf.simpleperfexamplepurejava.MainActivity$1.run', 80)])
        self.run_cmd([INFERNO_SCRIPT, "-sc", "-o", "report2.html"])
        self.check_inferno_report_html(
            [('com.example.simpleperf.simpleperfexamplepurejava.MainActivity$1.run', 80)],
            "report2.html")
        remove("report2.html")

    def test_inferno_in_another_dir(self):
        test_dir = 'inferno_testdir'
        saved_dir = os.getcwd()
        remove(test_dir)
        os.mkdir(test_dir)
        os.chdir(test_dir)
        self.run_cmd(['python', os.path.join(saved_dir, 'app_profiler.py'),
                      '--app', self.package_name, '-r', '-e task-clock:u -g --duration 3'])
        self.check_exist(filename="perf.data")
        self.run_cmd([INFERNO_SCRIPT, "-sc"])
        os.chdir(saved_dir)
        remove(test_dir)

    def test_report_html(self):
        self.common_test_report_html()

    def test_run_simpleperf_without_usb_connection(self):
        self.adb.check_run(['shell', 'am', 'start', '-n', self.package_name + '/.MainActivity'])
        self.run_cmd(['run_simpleperf_without_usb_connection.py', 'start', '-p',
                      self.package_name, '--size_limit', '1M'])
        self.adb.check_run(['kill-server'])
        time.sleep(3)
        self.run_cmd(['run_simpleperf_without_usb_connection.py', 'stop'])
        self.check_exist(filename="perf.data")
        self.run_cmd(["report.py", "-g", "-o", "report.txt"])


class TestExamplePureJavaRoot(TestExampleBase):
    @classmethod
    def setUpClass(cls):
        cls.prepare("SimpleperfExamplePureJava",
                    "com.example.simpleperf.simpleperfexamplepurejava",
                    ".MainActivity",
                    adb_root=True)

    def test_app_profiler(self):
        self.common_test_app_profiler()


class TestExamplePureJavaTraceOffCpu(TestExampleBase):
    @classmethod
    def setUpClass(cls):
        cls.prepare("SimpleperfExamplePureJava",
                    "com.example.simpleperf.simpleperfexamplepurejava",
                    ".SleepActivity")

    def test_smoke(self):
        self.run_app_profiler(record_arg="-g -f 1000 --duration 10 -e cpu-cycles:u --trace-offcpu")
        self.run_cmd(["report.py", "-g", "-o", "report.txt"])
        self.check_strings_in_file("report.txt", [
            "com.example.simpleperf.simpleperfexamplepurejava.SleepActivity$1.run",
            "com.example.simpleperf.simpleperfexamplepurejava.SleepActivity$1.RunFunction",
            "com.example.simpleperf.simpleperfexamplepurejava.SleepActivity$1.SleepFunction"
            ])
        remove("annotated_files")
        self.run_cmd(["annotate.py", "-s", self.example_path])
        self.check_exist(dirname="annotated_files")
        if self.use_compiled_java_code:
            self.check_file_under_dir("annotated_files", "SleepActivity.java")
            summary_file = os.path.join("annotated_files", "summary")
            self.check_annotation_summary(summary_file, [
                ("SleepActivity.java", 80, 20),
                ("run", 80, 0),
                ("RunFunction", 20, 20),
                ("SleepFunction", 20, 0),
                ("line 24", 20, 0),
                ("line 32", 20, 0)])
        self.run_cmd([INFERNO_SCRIPT, "-sc"])
        self.check_inferno_report_html(
            [('com.example.simpleperf.simpleperfexamplepurejava.SleepActivity$1.run', 80),
             ('com.example.simpleperf.simpleperfexamplepurejava.SleepActivity$1.RunFunction',
              20),
             ('com.example.simpleperf.simpleperfexamplepurejava.SleepActivity$1.SleepFunction',
              20)])


class TestExampleWithNative(TestExampleBase):
    @classmethod
    def setUpClass(cls):
        cls.prepare("SimpleperfExampleWithNative",
                    "com.example.simpleperf.simpleperfexamplewithnative",
                    ".MainActivity")

    def test_app_profiler(self):
        self.common_test_app_profiler()

    def test_app_profiler_profile_from_launch(self):
        self.run_app_profiler(start_activity=True, build_binary_cache=False)
        self.run_cmd(["report.py", "-g", "-o", "report.txt"])
        self.check_strings_in_file("report.txt", ["BusyLoopThread", "__start_thread"])

    def test_report(self):
        self.common_test_report()
        self.run_cmd(["report.py", "-g", "-o", "report.txt"])
        self.check_strings_in_file("report.txt", ["BusyLoopThread", "__start_thread"])

    def test_annotate(self):
        self.common_test_annotate()
        self.check_file_under_dir("annotated_files", "native-lib.cpp")
        summary_file = os.path.join("annotated_files", "summary")
        self.check_annotation_summary(summary_file, [
            ("native-lib.cpp", 20, 0),
            ("BusyLoopThread", 20, 0),
            ("line 46", 20, 0)])

    def test_report_sample(self):
        self.common_test_report_sample(
            ["BusyLoopThread",
             "__start_thread"])

    def test_pprof_proto_generator(self):
        self.common_test_pprof_proto_generator(
            check_strings_with_lines=["native-lib.cpp", "BusyLoopThread"],
            check_strings_without_lines=["BusyLoopThread"])

    def test_inferno(self):
        self.common_test_inferno()
        self.run_app_profiler()
        self.run_cmd([INFERNO_SCRIPT, "-sc"])
        self.check_inferno_report_html([('BusyLoopThread', 20)])

    def test_report_html(self):
        self.common_test_report_html()
        self.run_cmd(['report_html.py', '--add_source_code', '--source_dirs', 'testdata',
                      '--add_disassembly', '--binary_filter', "libnative-lib.so"])


class TestExampleWithNativeRoot(TestExampleBase):
    @classmethod
    def setUpClass(cls):
        cls.prepare("SimpleperfExampleWithNative",
                    "com.example.simpleperf.simpleperfexamplewithnative",
                    ".MainActivity",
                    adb_root=True)

    def test_app_profiler(self):
        self.common_test_app_profiler()


class TestExampleWithNativeTraceOffCpu(TestExampleBase):
    @classmethod
    def setUpClass(cls):
        cls.prepare("SimpleperfExampleWithNative",
                    "com.example.simpleperf.simpleperfexamplewithnative",
                    ".SleepActivity")

    def test_smoke(self):
        self.run_app_profiler(record_arg="-g -f 1000 --duration 10 -e cpu-cycles:u --trace-offcpu")
        self.run_cmd(["report.py", "-g", "--comms", "SleepThread", "-o", "report.txt"])
        self.check_strings_in_file("report.txt", [
            "SleepThread(void*)",
            "RunFunction()",
            "SleepFunction(unsigned long long)"])
        remove("annotated_files")
        self.run_cmd(["annotate.py", "-s", self.example_path, "--comm", "SleepThread"])
        self.check_exist(dirname="annotated_files")
        self.check_file_under_dir("annotated_files", "native-lib.cpp")
        summary_file = os.path.join("annotated_files", "summary")
        self.check_annotation_summary(summary_file, [
            ("native-lib.cpp", 80, 20),
            ("SleepThread", 80, 0),
            ("RunFunction", 20, 20),
            ("SleepFunction", 20, 0),
            ("line 73", 20, 0),
            ("line 83", 20, 0)])
        self.run_cmd([INFERNO_SCRIPT, "-sc"])
        self.check_inferno_report_html([('SleepThread', 80),
                                        ('RunFunction', 20),
                                        ('SleepFunction', 20)])


class TestExampleWithNativeJniCall(TestExampleBase):
    @classmethod
    def setUpClass(cls):
        cls.prepare("SimpleperfExampleWithNative",
                    "com.example.simpleperf.simpleperfexamplewithnative",
                    ".MixActivity")

    def test_smoke(self):
        self.run_app_profiler()
        self.run_cmd(["report.py", "-g", "--comms", "BusyThread", "-o", "report.txt"])
        self.check_strings_in_file("report.txt", [
            "com.example.simpleperf.simpleperfexamplewithnative.MixActivity$1.run",
            "com.example.simpleperf.simpleperfexamplewithnative.MixActivity.callFunction",
            "Java_com_example_simpleperf_simpleperfexamplewithnative_MixActivity_callFunction"])
        remove("annotated_files")
        self.run_cmd(["annotate.py", "-s", self.example_path, "--comm", "BusyThread"])
        self.check_exist(dirname="annotated_files")
        self.check_file_under_dir("annotated_files", "native-lib.cpp")
        summary_file = os.path.join("annotated_files", "summary")
        self.check_annotation_summary(summary_file, [("native-lib.cpp", 5, 0), ("line 40", 5, 0)])
        if self.use_compiled_java_code:
            self.check_file_under_dir("annotated_files", "MixActivity.java")
            self.check_annotation_summary(summary_file, [
                ("MixActivity.java", 80, 0),
                ("run", 80, 0),
                ("line 26", 20, 0),
                ("native-lib.cpp", 5, 0),
                ("line 40", 5, 0)])

        self.run_cmd([INFERNO_SCRIPT, "-sc"])


class TestExampleWithNativeForceArm(TestExampleWithNative):
    @classmethod
    def setUpClass(cls):
        cls.prepare("SimpleperfExampleWithNative",
                    "com.example.simpleperf.simpleperfexamplewithnative",
                    ".MainActivity",
                    abi="armeabi-v7a")


class TestExampleWithNativeForceArmRoot(TestExampleWithNativeRoot):
    @classmethod
    def setUpClass(cls):
        cls.prepare("SimpleperfExampleWithNative",
                    "com.example.simpleperf.simpleperfexamplewithnative",
                    ".MainActivity",
                    abi="armeabi-v7a",
                    adb_root=False)


class TestExampleWithNativeTraceOffCpuForceArm(TestExampleWithNativeTraceOffCpu):
    @classmethod
    def setUpClass(cls):
        cls.prepare("SimpleperfExampleWithNative",
                    "com.example.simpleperf.simpleperfexamplewithnative",
                    ".SleepActivity",
                    abi="armeabi-v7a")


class TestExampleOfKotlin(TestExampleBase):
    @classmethod
    def setUpClass(cls):
        cls.prepare("SimpleperfExampleOfKotlin",
                    "com.example.simpleperf.simpleperfexampleofkotlin",
                    ".MainActivity")

    def test_app_profiler(self):
        self.common_test_app_profiler()

    def test_app_profiler_profile_from_launch(self):
        self.run_app_profiler(start_activity=True, build_binary_cache=False)
        self.run_cmd(["report.py", "-g", "-o", "report.txt"])
        self.check_strings_in_file("report.txt", [
            "com.example.simpleperf.simpleperfexampleofkotlin.MainActivity$createBusyThread$1." +
            "run", "__start_thread"])

    def test_report(self):
        self.common_test_report()
        self.run_cmd(["report.py", "-g", "-o", "report.txt"])
        self.check_strings_in_file("report.txt", [
            "com.example.simpleperf.simpleperfexampleofkotlin.MainActivity$createBusyThread$1." +
            "run", "__start_thread"])

    def test_annotate(self):
        if not self.use_compiled_java_code:
            return
        self.common_test_annotate()
        self.check_file_under_dir("annotated_files", "MainActivity.kt")
        summary_file = os.path.join("annotated_files", "summary")
        self.check_annotation_summary(summary_file, [
            ("MainActivity.kt", 80, 80),
            ("run", 80, 0),
            ("callFunction", 0, 0),
            ("line 19", 80, 0),
            ("line 25", 0, 0)])

    def test_report_sample(self):
        self.common_test_report_sample([
            "com.example.simpleperf.simpleperfexampleofkotlin.MainActivity$createBusyThread$1." +
            "run", "__start_thread"])

    def test_pprof_proto_generator(self):
        check_strings_with_lines = []
        if self.use_compiled_java_code:
            check_strings_with_lines = [
                "com/example/simpleperf/simpleperfexampleofkotlin/MainActivity.kt",
                "run"]
        self.common_test_pprof_proto_generator(
            check_strings_with_lines=check_strings_with_lines,
            check_strings_without_lines=["com.example.simpleperf.simpleperfexampleofkotlin." +
                                         "MainActivity$createBusyThread$1.run"])

    def test_inferno(self):
        self.common_test_inferno()
        self.run_app_profiler()
        self.run_cmd([INFERNO_SCRIPT, "-sc"])
        self.check_inferno_report_html([('com.example.simpleperf.simpleperfexampleofkotlin.' +
                                         'MainActivity$createBusyThread$1.run', 80)])

    def test_report_html(self):
        self.common_test_report_html()


class TestExampleOfKotlinRoot(TestExampleBase):
    @classmethod
    def setUpClass(cls):
        cls.prepare("SimpleperfExampleOfKotlin",
                    "com.example.simpleperf.simpleperfexampleofkotlin",
                    ".MainActivity",
                    adb_root=True)

    def test_app_profiler(self):
        self.common_test_app_profiler()


class TestExampleOfKotlinTraceOffCpu(TestExampleBase):
    @classmethod
    def setUpClass(cls):
        cls.prepare("SimpleperfExampleOfKotlin",
                    "com.example.simpleperf.simpleperfexampleofkotlin",
                    ".SleepActivity")

    def test_smoke(self):
        self.run_app_profiler(record_arg="-g -f 1000 --duration 10 -e cpu-cycles:u --trace-offcpu")
        self.run_cmd(["report.py", "-g", "-o", "report.txt"])
        function_prefix = "com.example.simpleperf.simpleperfexampleofkotlin." + \
                          "SleepActivity$createRunSleepThread$1."
        self.check_strings_in_file("report.txt", [
            function_prefix + "run",
            function_prefix + "RunFunction",
            function_prefix + "SleepFunction"
            ])
        if self.use_compiled_java_code:
            remove("annotated_files")
            self.run_cmd(["annotate.py", "-s", self.example_path])
            self.check_exist(dirname="annotated_files")
            self.check_file_under_dir("annotated_files", "SleepActivity.kt")
            summary_file = os.path.join("annotated_files", "summary")
            self.check_annotation_summary(summary_file, [
                ("SleepActivity.kt", 80, 20),
                ("run", 80, 0),
                ("RunFunction", 20, 20),
                ("SleepFunction", 20, 0),
                ("line 24", 20, 0),
                ("line 32", 20, 0)])

        self.run_cmd([INFERNO_SCRIPT, "-sc"])
        self.check_inferno_report_html([
            (function_prefix + 'run', 80),
            (function_prefix + 'RunFunction', 20),
            (function_prefix + 'SleepFunction', 20)])


class TestNativeProfiling(TestBase):
    def setUp(self):
        self.adb = AdbHelper()
        self.is_rooted_device = self.adb.switch_to_root()

    def test_profile_cmd(self):
        self.run_cmd(["app_profiler.py", "-cmd", "pm -l", "--disable_adb_root"])
        self.run_cmd(["report.py", "-g", "-o", "report.txt"])

    def test_profile_native_program(self):
        if not self.is_rooted_device:
            return
        self.run_cmd(["app_profiler.py", "-np", "surfaceflinger"])
        self.run_cmd(["report.py", "-g", "-o", "report.txt"])
        self.run_cmd([INFERNO_SCRIPT, "-sc"])
        self.run_cmd([INFERNO_SCRIPT, "-np", "surfaceflinger"])

    def test_profile_pids(self):
        if not self.is_rooted_device:
            return
        pid = int(self.adb.check_run_and_return_output(['shell', 'pidof', 'system_server']))
        self.run_cmd(['app_profiler.py', '--pid', str(pid), '-r', '--duration 1'])
        self.run_cmd(['app_profiler.py', '--pid', str(pid), str(pid), '-r', '--duration 1'])
        self.run_cmd(['app_profiler.py', '--tid', str(pid), '-r', '--duration 1'])
        self.run_cmd(['app_profiler.py', '--tid', str(pid), str(pid), '-r', '--duration 1'])
        self.run_cmd([INFERNO_SCRIPT, '--pid', str(pid), '-t', '1'])

    def test_profile_system_wide(self):
        if not self.is_rooted_device:
            return
        self.run_cmd(['app_profiler.py', '--system_wide', '-r', '--duration 1'])


class TestReportLib(unittest.TestCase):
    def setUp(self):
        self.report_lib = ReportLib()
        self.report_lib.SetRecordFile(os.path.join('testdata', 'perf_with_symbols.data'))

    def tearDown(self):
        self.report_lib.Close()

    def test_build_id(self):
        build_id = self.report_lib.GetBuildIdForPath('/data/t2')
        self.assertEqual(build_id, '0x70f1fe24500fc8b0d9eb477199ca1ca21acca4de')

    def test_symbol(self):
        found_func2 = False
        while self.report_lib.GetNextSample():
            symbol = self.report_lib.GetSymbolOfCurrentSample()
            if symbol.symbol_name == 'func2(int, int)':
                found_func2 = True
                self.assertEqual(symbol.symbol_addr, 0x4004ed)
                self.assertEqual(symbol.symbol_len, 0x14)
        self.assertTrue(found_func2)

    def test_sample(self):
        found_sample = False
        while self.report_lib.GetNextSample():
            sample = self.report_lib.GetCurrentSample()
            if sample.ip == 0x4004ff and sample.time == 7637889424953:
                found_sample = True
                self.assertEqual(sample.pid, 15926)
                self.assertEqual(sample.tid, 15926)
                self.assertEqual(sample.thread_comm, 't2')
                self.assertEqual(sample.cpu, 5)
                self.assertEqual(sample.period, 694614)
                event = self.report_lib.GetEventOfCurrentSample()
                self.assertEqual(event.name, 'cpu-cycles')
                callchain = self.report_lib.GetCallChainOfCurrentSample()
                self.assertEqual(callchain.nr, 0)
        self.assertTrue(found_sample)

    def test_meta_info(self):
        self.report_lib.SetRecordFile(os.path.join('testdata', 'perf_with_trace_offcpu.data'))
        meta_info = self.report_lib.MetaInfo()
        self.assertTrue("simpleperf_version" in meta_info)
        self.assertEqual(meta_info["system_wide_collection"], "false")
        self.assertEqual(meta_info["trace_offcpu"], "true")
        self.assertEqual(meta_info["event_type_info"], "cpu-cycles,0,0\nsched:sched_switch,2,47")
        self.assertTrue("product_props" in meta_info)

    def test_event_name_from_meta_info(self):
        self.report_lib.SetRecordFile(os.path.join('testdata', 'perf_with_tracepoint_event.data'))
        event_names = set()
        while self.report_lib.GetNextSample():
            event_names.add(self.report_lib.GetEventOfCurrentSample().name)
        self.assertTrue('sched:sched_switch' in event_names)
        self.assertTrue('cpu-cycles' in event_names)

    def test_record_cmd(self):
        self.report_lib.SetRecordFile(os.path.join('testdata', 'perf_with_trace_offcpu.data'))
        self.assertEqual(self.report_lib.GetRecordCmd(),
                         "/data/local/tmp/simpleperf record --trace-offcpu --duration 2 -g " +
                         "./simpleperf_runtest_run_and_sleep64")

    def test_offcpu(self):
        self.report_lib.SetRecordFile(os.path.join('testdata', 'perf_with_trace_offcpu.data'))
        total_period = 0
        sleep_function_period = 0
        sleep_function_name = "SleepFunction(unsigned long long)"
        while self.report_lib.GetNextSample():
            sample = self.report_lib.GetCurrentSample()
            total_period += sample.period
            if self.report_lib.GetSymbolOfCurrentSample().symbol_name == sleep_function_name:
                sleep_function_period += sample.period
                continue
            callchain = self.report_lib.GetCallChainOfCurrentSample()
            for i in range(callchain.nr):
                if callchain.entries[i].symbol.symbol_name == sleep_function_name:
                    sleep_function_period += sample.period
                    break
            self.assertEqual(self.report_lib.GetEventOfCurrentSample().name, 'cpu-cycles')
        sleep_percentage = float(sleep_function_period) / total_period
        self.assertGreater(sleep_percentage, 0.30)

    def test_show_art_frames(self):
        def has_art_frame(report_lib):
            report_lib.SetRecordFile(os.path.join('testdata', 'perf_with_interpreter_frames.data'))
            result = False
            while report_lib.GetNextSample():
                callchain = report_lib.GetCallChainOfCurrentSample()
                for i in range(callchain.nr):
                    if callchain.entries[i].symbol.symbol_name == 'artMterpAsmInstructionStart':
                        result = True
                        break
            report_lib.Close()
            return result

        report_lib = ReportLib()
        self.assertFalse(has_art_frame(report_lib))
        report_lib = ReportLib()
        report_lib.ShowArtFrames(False)
        self.assertFalse(has_art_frame(report_lib))
        report_lib = ReportLib()
        report_lib.ShowArtFrames(True)
        self.assertTrue(has_art_frame(report_lib))

    def test_tracing_data(self):
        self.report_lib.SetRecordFile(os.path.join('testdata', 'perf_with_tracepoint_event.data'))
        has_tracing_data = False
        while self.report_lib.GetNextSample():
            event = self.report_lib.GetEventOfCurrentSample()
            tracing_data = self.report_lib.GetTracingDataOfCurrentSample()
            if event.name == 'sched:sched_switch':
                self.assertIsNotNone(tracing_data)
                self.assertIn('prev_pid', tracing_data)
                self.assertIn('next_comm', tracing_data)
                if tracing_data['prev_pid'] == 9896 and tracing_data['next_comm'] == 'swapper/4':
                    has_tracing_data = True
            else:
                self.assertIsNone(tracing_data)
        self.assertTrue(has_tracing_data)


class TestRunSimpleperfOnDevice(TestBase):
    def test_smoke(self):
        self.run_cmd(['run_simpleperf_on_device.py', 'list', '--show-features'])


class TestTools(unittest.TestCase):
    def test_addr2nearestline(self):
        self.run_addr2nearestline_test(True)
        self.run_addr2nearestline_test(False)

    def run_addr2nearestline_test(self, with_function_name):
        binary_cache_path = 'testdata'
        test_map = {
            '/simpleperf_runtest_two_functions_arm64': [
                {
                    'func_addr': 0x668,
                    'addr': 0x668,
                    'source': 'system/extras/simpleperf/runtest/two_functions.cpp:20',
                    'function': 'main',
                },
                {
                    'func_addr': 0x668,
                    'addr': 0x6a4,
                    'source': """system/extras/simpleperf/runtest/two_functions.cpp:7
                                 system/extras/simpleperf/runtest/two_functions.cpp:22""",
                    'function': """Function1()
                                   main""",
                },
            ],
            '/simpleperf_runtest_two_functions_arm': [
                {
                    'func_addr': 0x784,
                    'addr': 0x7b0,
                    'source': """system/extras/simpleperf/runtest/two_functions.cpp:14
                                 system/extras/simpleperf/runtest/two_functions.cpp:23""",
                    'function': """Function2()
                                   main""",
                },
                {
                    'func_addr': 0x784,
                    'addr': 0x7d0,
                    'source': """system/extras/simpleperf/runtest/two_functions.cpp:15
                                 system/extras/simpleperf/runtest/two_functions.cpp:23""",
                    'function': """Function2()
                                   main""",
                }
            ],
            '/simpleperf_runtest_two_functions_x86_64': [
                {
                    'func_addr': 0x840,
                    'addr': 0x840,
                    'source': 'system/extras/simpleperf/runtest/two_functions.cpp:7',
                    'function': 'Function1()',
                },
                {
                    'func_addr': 0x920,
                    'addr': 0x94a,
                    'source': """system/extras/simpleperf/runtest/two_functions.cpp:7
                                 system/extras/simpleperf/runtest/two_functions.cpp:22""",
                    'function': """Function1()
                                   main""",
                }
            ],
            '/simpleperf_runtest_two_functions_x86': [
                {
                    'func_addr': 0x6d0,
                    'addr': 0x6da,
                    'source': 'system/extras/simpleperf/runtest/two_functions.cpp:14',
                    'function': 'Function2()',
                },
                {
                    'func_addr': 0x710,
                    'addr': 0x749,
                    'source': """system/extras/simpleperf/runtest/two_functions.cpp:8
                                 system/extras/simpleperf/runtest/two_functions.cpp:22""",
                    'function': """Function1()
                                   main""",
                }
            ],
        }
        addr2line = Addr2Nearestline(None, binary_cache_path, with_function_name)
        for dso_path in test_map:
            test_addrs = test_map[dso_path]
            for test_addr in test_addrs:
                addr2line.add_addr(dso_path, test_addr['func_addr'], test_addr['addr'])
        addr2line.convert_addrs_to_lines()
        for dso_path in test_map:
            dso = addr2line.get_dso(dso_path)
            self.assertTrue(dso is not None)
            test_addrs = test_map[dso_path]
            for test_addr in test_addrs:
                expected_files = []
                expected_lines = []
                expected_functions = []
                for line in test_addr['source'].split('\n'):
                    items = line.split(':')
                    expected_files.append(items[0].strip())
                    expected_lines.append(int(items[1]))
                for line in test_addr['function'].split('\n'):
                    expected_functions.append(line.strip())
                self.assertEquals(len(expected_files), len(expected_functions))

                actual_source = addr2line.get_addr_source(dso, test_addr['addr'])
                self.assertTrue(actual_source is not None)
                self.assertEqual(len(actual_source), len(expected_files))
                for i, source in enumerate(actual_source):
                    self.assertEqual(len(source), 3 if with_function_name else 2)
                    self.assertEqual(source[0], expected_files[i])
                    self.assertEqual(source[1], expected_lines[i])
                    if with_function_name:
                        self.assertEqual(source[2], expected_functions[i])

    def test_objdump(self):
        binary_cache_path = 'testdata'
        test_map = {
            '/simpleperf_runtest_two_functions_arm64': {
                'start_addr': 0x668,
                'len': 116,
                'expected_items': [
                    ('main():', 0),
                    ('system/extras/simpleperf/runtest/two_functions.cpp:20', 0),
                    (' 694:	add	x20, x20, #0x6de', 0x694),
                ],
            },
            '/simpleperf_runtest_two_functions_arm': {
                'start_addr': 0x784,
                'len': 80,
                'expected_items': [
                    ('main():', 0),
                    ('system/extras/simpleperf/runtest/two_functions.cpp:20', 0),
                    ('     7ae:	bne.n	7a6 <main+0x22>', 0x7ae),
                ],
            },
            '/simpleperf_runtest_two_functions_x86_64': {
                'start_addr': 0x920,
                'len': 201,
                'expected_items': [
                    ('main():', 0),
                    ('system/extras/simpleperf/runtest/two_functions.cpp:20', 0),
                    (' 96e:	mov    %edx,(%rbx,%rax,4)', 0x96e),
                ],
            },
            '/simpleperf_runtest_two_functions_x86': {
                'start_addr': 0x710,
                'len': 98,
                'expected_items': [
                    ('main():', 0),
                    ('system/extras/simpleperf/runtest/two_functions.cpp:20', 0),
                    (' 748:	cmp    $0x5f5e100,%ebp', 0x748),
                ],
            },
        }
        objdump = Objdump(None, binary_cache_path)
        for dso_path in test_map:
            dso = test_map[dso_path]
            dso_info = objdump.get_dso_info(dso_path)
            self.assertIsNotNone(dso_info)
            disassemble_code = objdump.disassemble_code(dso_info, dso['start_addr'], dso['len'])
            self.assertTrue(disassemble_code)
            for item in dso['expected_items']:
                self.assertTrue(item in disassemble_code)

    def test_readelf(self):
        test_map = {
            '/simpleperf_runtest_two_functions_arm64': {
                'arch': 'arm64',
                'build_id': '0xe8ecb3916d989dbdc068345c30f0c24300000000',
                'sections': ['.interp', '.note.android.ident', '.note.gnu.build-id', '.dynsym',
                             '.dynstr', '.gnu.hash', '.gnu.version', '.gnu.version_r', '.rela.dyn',
                             '.rela.plt', '.plt', '.text', '.rodata', '.eh_frame', '.eh_frame_hdr',
                             '.preinit_array', '.init_array', '.fini_array', '.dynamic', '.got',
                             '.got.plt', '.data', '.bss', '.comment', '.debug_str', '.debug_loc',
                             '.debug_abbrev', '.debug_info', '.debug_ranges', '.debug_macinfo',
                             '.debug_pubnames', '.debug_pubtypes', '.debug_line',
                             '.note.gnu.gold-version', '.symtab', '.strtab', '.shstrtab'],
            },
            '/simpleperf_runtest_two_functions_arm': {
                'arch': 'arm',
                'build_id': '0x718f5b36c4148ee1bd3f51af89ed2be600000000',
            },
            '/simpleperf_runtest_two_functions_x86_64': {
                'arch': 'x86_64',
            },
            '/simpleperf_runtest_two_functions_x86': {
                'arch': 'x86',
            }
        }
        readelf = ReadElf(None)
        for dso_path in test_map:
            dso_info = test_map[dso_path]
            path = 'testdata' + dso_path
            self.assertEqual(dso_info['arch'], readelf.get_arch(path))
            if 'build_id' in dso_info:
                self.assertEqual(dso_info['build_id'], readelf.get_build_id(path))
            if 'sections' in dso_info:
                self.assertEqual(dso_info['sections'], readelf.get_sections(path))
        self.assertEqual(readelf.get_arch('not_exist_file'), 'unknown')
        self.assertEqual(readelf.get_build_id('not_exist_file'), '')
        self.assertEqual(readelf.get_sections('not_exist_file'), [])

    def test_source_file_searcher(self):
        searcher = SourceFileSearcher(['testdata'])
        def format_path(path):
            return path.replace('/', os.sep)
        # Find a C++ file with pure file name.
        self.assertEquals(
            format_path('testdata/SimpleperfExampleWithNative/app/src/main/cpp/native-lib.cpp'),
            searcher.get_real_path('native-lib.cpp'))
        # Find a C++ file with an absolute file path.
        self.assertEquals(
            format_path('testdata/SimpleperfExampleWithNative/app/src/main/cpp/native-lib.cpp'),
            searcher.get_real_path('/data/native-lib.cpp'))
        # Find a Java file.
        self.assertEquals(
            format_path('testdata/SimpleperfExampleWithNative/app/src/main/java/com/example/' +
                        'simpleperf/simpleperfexamplewithnative/MainActivity.java'),
            searcher.get_real_path('simpleperfexamplewithnative/MainActivity.java'))
        # Find a Kotlin file.
        self.assertEquals(
            format_path('testdata/SimpleperfExampleOfKotlin/app/src/main/java/com/example/' +
                        'simpleperf/simpleperfexampleofkotlin/MainActivity.kt'),
            searcher.get_real_path('MainActivity.kt'))


class TestNativeLibDownloader(unittest.TestCase):
    def test_smoke(self):
        adb = AdbHelper()

        def is_lib_on_device(path):
            return adb.run(['shell', 'ls', path])

        # Sync all native libs on device.
        adb.run(['shell', 'rm', '-rf', '/data/local/tmp/native_libs'])
        downloader = NativeLibDownloader(None, 'arm64', adb)
        downloader.collect_native_libs_on_host(os.path.join(
            'testdata', 'SimpleperfExampleWithNative', 'app', 'build', 'intermediates', 'cmake',
            'profiling'))
        self.assertEqual(len(downloader.host_build_id_map), 2)
        for entry in downloader.host_build_id_map.values():
            self.assertEqual(entry.score, 3)
        downloader.collect_native_libs_on_device()
        self.assertEqual(len(downloader.device_build_id_map), 0)

        lib_list = list(downloader.host_build_id_map.items())
        for sync_count in [0, 1, 2]:
            build_id_map = {}
            for i in range(sync_count):
                build_id_map[lib_list[i][0]] = lib_list[i][1]
            downloader.host_build_id_map = build_id_map
            downloader.sync_natives_libs_on_device()
            downloader.collect_native_libs_on_device()
            self.assertEqual(len(downloader.device_build_id_map), sync_count)
            for i, item in enumerate(lib_list):
                build_id = item[0]
                name = item[1].name
                if i < sync_count:
                    self.assertTrue(build_id in downloader.device_build_id_map)
                    self.assertEqual(name, downloader.device_build_id_map[build_id])
                    self.assertTrue(is_lib_on_device(downloader.dir_on_device + name))
                else:
                    self.assertTrue(build_id not in downloader.device_build_id_map)
                    self.assertFalse(is_lib_on_device(downloader.dir_on_device + name))
            if sync_count == 1:
                adb.run(['pull', '/data/local/tmp/native_libs/build_id_list', 'build_id_list'])
                with open('build_id_list', 'rb') as fh:
                    self.assertEqual(bytes_to_str(fh.read()),
                                     '{}={}\n'.format(lib_list[0][0], lib_list[0][1].name))
                remove('build_id_list')
        adb.run(['shell', 'rm', '-rf', '/data/local/tmp/native_libs'])


class TestReportHtml(TestBase):
    def test_long_callchain(self):
        self.run_cmd(['report_html.py', '-i', 'testdata/perf_with_long_callchain.data'])


class TestBinaryCacheBuilder(TestBase):
    def test_copy_binaries_from_symfs_dirs(self):
        readelf = ReadElf(None)
        strip = find_tool_path('strip', arch='arm')
        self.assertIsNotNone(strip)
        symfs_dir = os.path.join('testdata', 'symfs_dir')
        remove(symfs_dir)
        os.mkdir(symfs_dir)
        filename = 'simpleperf_runtest_two_functions_arm'
        origin_file = os.path.join('testdata', filename)
        source_file = os.path.join(symfs_dir, filename)
        target_file = os.path.join('binary_cache', filename)
        expected_build_id = readelf.get_build_id(origin_file)
        binary_cache_builder = BinaryCacheBuilder(None, False)
        binary_cache_builder.binaries['simpleperf_runtest_two_functions_arm'] = expected_build_id

        # Copy binary if target file doesn't exist.
        remove(target_file)
        self.run_cmd([strip, '--strip-all', '-o', source_file, origin_file])
        binary_cache_builder.copy_binaries_from_symfs_dirs([symfs_dir])
        self.assertTrue(filecmp.cmp(target_file, source_file))

        # Copy binary if target file doesn't have .symtab and source file has .symtab.
        self.run_cmd([strip, '--strip-debug', '-o', source_file, origin_file])
        binary_cache_builder.copy_binaries_from_symfs_dirs([symfs_dir])
        self.assertTrue(filecmp.cmp(target_file, source_file))

        # Copy binary if target file doesn't have .debug_line and source_files has .debug_line.
        shutil.copy(origin_file, source_file)
        binary_cache_builder.copy_binaries_from_symfs_dirs([symfs_dir])
        self.assertTrue(filecmp.cmp(target_file, source_file))


def get_all_tests():
    tests = []
    for name, value in globals().items():
        if isinstance(value, type) and issubclass(value, unittest.TestCase):
            for member_name, member in inspect.getmembers(value):
                if isinstance(member, (types.MethodType, types.FunctionType)):
                    if member_name.startswith('test'):
                        tests.append(name + '.' + member_name)
    return sorted(tests)


def run_tests(tests):
    os.chdir(get_script_dir())
    build_testdata()
    log_info('Run tests with python%d\n%s' % (3 if is_python3() else 2, '\n'.join(tests)))
    argv = [sys.argv[0]] + tests
    unittest.main(argv=argv, failfast=True, verbosity=2, exit=False)


def main():
    parser = argparse.ArgumentParser(description='Test simpleperf scripts')
    parser.add_argument('--list-tests', action='store_true', help='List all tests.')
    parser.add_argument('--test-from', nargs=1, help='Run left tests from the selected test.')
    parser.add_argument('--python-version', choices=['2', '3', 'both'], default='both', help="""
                        Run tests on which python versions.""")
    parser.add_argument('pattern', nargs='*', help='Run tests matching the selected pattern.')
    args = parser.parse_args()
    tests = get_all_tests()
    if args.list_tests:
        print('\n'.join(tests))
        return
    if args.test_from:
        start_pos = 0
        while start_pos < len(tests) and tests[start_pos] != args.test_from[0]:
            start_pos += 1
        if start_pos == len(tests):
            log_exit("Can't find test %s" % args.test_from[0])
        tests = tests[start_pos:]
    if args.pattern:
        pattern = re.compile(fnmatch.translate(args.pattern[0]))
        new_tests = []
        for test in tests:
            if pattern.match(test):
                new_tests.append(test)
        tests = new_tests
        if not tests:
            log_exit('No tests are matched.')

    if AdbHelper().get_android_version() < 7:
        log_info("Skip tests on Android version < N.")
        sys.exit(0)

    if args.python_version == 'both':
        python_versions = [2, 3]
    else:
        python_versions = [int(args.python_version)]
    current_version = 3 if is_python3() else 2
    for version in python_versions:
        if version != current_version:
            argv = ['python3' if version == 3 else 'python']
            argv.append(os.path.join(get_script_dir(), 'test.py'))
            argv += sys.argv[1:]
            argv += ['--python-version', str(version)]
            subprocess.check_call(argv)
        else:
            run_tests(tests)


if __name__ == '__main__':
    main()
