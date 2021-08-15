# Copyright (C) 2016 The Android Open Source Project
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

'''Module that contains TestBase, the base class of all tests.'''

from __future__ import absolute_import

import logging
import os
import re
import tempfile
import inspect
import traceback

from .exception import DisconnectedException, TestSuiteException

from . import util_log


class TestBase(object):
    '''Base class for all tests. Provides some common functionality.'''

    bundle_target = {}

    class TestFail(Exception):
        '''Exception that is thrown when a line in a test fails.

        This exception is thrown if a lldb command does not return the expected
        string.
        '''
        pass

    def __init__(self, device_port, device, timer, app_type, wimpy=False, **kwargs):
        # Keep argument names for documentation purposes. This method is
        # overwritten by test_base_remote.
        # pylint: disable=unused-argument
        self._lldb = None # handle to the lldb module
        self._ci = None # instance of the lldb command interpreter for this test
        self._timer = timer # timer instance, to check whether the test froze
        self.app_type = app_type # The type of bundle that is being executed
        self.wimpy = wimpy

    def setup(self, android):
        '''Set up environment for the test.

        Override to specify commands to be run before the test APK launch.
        Useful for setting Android properties or environment variables. See also
        the teardown method.

        Args:
            android: Handler to the android device, see the UtilAndroid class.
        '''
        pass

    def teardown(self, android):
        '''Clean up environment after test.

        Override this procedure to specify commands to be run after the test has
        finished. This method is run regardless the outcome of the test.

        Args:
            android: Handler to the android device, see the UtilAndroid class.
        '''
        pass

    def run(self, dbg, remote_pid, lldb):
        '''Execute the actual test suite.

        Args:
            dbg: The instance of the SBDebugger that is used to test commands.
            remote_pid: The integer that is the process id of the binary that
                        the debugger is attached to.
            lldb: A handle to the lldb module.

        Returns:
            A list of (test, failure) tuples.
        '''
        log = util_log.get_logger()

        def predicate(obj):
            '''check whether we're interested in the function'''
            if not callable(obj):
                return False
            if self.wimpy and not getattr(obj, 'wimpy', False):
                log.debug("skipping non-wimpy test in wimpy mode:%r", obj)
                return False
            return True

        test_methods = [
            method for name, method in inspect.getmembers(self, predicate)
            if name.startswith('test_')
        ]
        log.debug("Found the following tests %r", test_methods)
        test_errors = []

        for test in sorted(
            test_methods,
            key=lambda item: getattr(item, 'test_order', float('Inf'))
        ):
            try:
                log.info("running test %r", test.__name__)
                result = test()
            except (self.TestFail, TestSuiteException) as e:
                test_errors.append((method, e))

        return test_errors

    def post_run(self):
        '''Clean up after test execution.'''
        pass

    def assert_true(self, cond):
        '''Check a given condition and raise TestFail if it is False.

        Args:
            cond: The boolean condition to check.

        Raises:
            TestFail: The condition was false.
        '''
        if not cond:
            raise self.TestFail()

    def assert_lang_renderscript(self):
        '''Check that LLDB is stopped in a RenderScript frame

        Use the LLDB API to check that the language of the current frame
        is RenderScript, fail otherwise.

        Raises:
            TestFail: Detected language not RenderScript.
        '''
        assert self._lldb
        assert self._ci

        proc = self._ci.GetProcess()
        frame = proc.GetSelectedThread().GetSelectedFrame()
        lang = frame.GetCompileUnit().GetLanguage()

        if lang != self._lldb.eLanguageTypeExtRenderScript:
            raise self.TestFail('Frame language not RenderScript, instead {0}'
                                .format(lang))

    def do_command(self, cmd):
        '''Run an lldb command and return the output.

        Args:
            cmd: The string representing the lldb command to run.

        Raises:
            TestFail: The lldb command failed.
        '''
        assert self._lldb
        assert self._ci

        log = util_log.get_logger()
        res = self._lldb.SBCommandReturnObject()

        log.info('[Command] {0}'.format(cmd))

        # before issuing the command, restart the current timer to check
        # whether the command is going to freeze the test
        if self._timer:
            self._timer.reset()

        self._ci.HandleCommand(cmd, res)

        if not res.Succeeded():
            error = res.GetError()
            error = error if error else res.GetOutput()
            raise self.TestFail('The command "{0}" failed with the error: {1}'
                                .format(cmd, error if error else '<N/a>'))

        output = res.GetOutput() or ''
        log.debug('[Output] {0}'.format(output.rstrip()))

        return output

    def try_command(self, cmd, expected=None, expected_regex=None):
        '''Run an lldb command and match the expected response.

        Args:
            cmd: The string representing the lldb command to run.
            expected: A list of strings that should be present in lldb's
                      output.
            expected_regex: A list of regular expressions that should
                            match lldb's output.

        Raises:
            TestFail: One of the expected strings were not found in the lldb
            output.

        Returns:
            str: raw lldb command output.
        '''
        assert self._lldb
        assert self._ci
        log = util_log.get_logger()
        output = ''
        try:
            output = self.do_command(cmd)

            if 'lost connection' in output:
                raise DisconnectedException('Lost connection to lldb-server.')

            # check the expected strings
            if expected:
                self._match_literals(output, expected)

            # check the regexp patterns
            if expected_regex:
                self._match_regexp_patterns(output, expected_regex)

        except self.TestFail as exception:
            # if the command failed, ensure the output retrieved from the
            # command is printed even in verbose mode
            if log.getEffectiveLevel() > logging.DEBUG:
                log.error('[Output] {0}'.format(output.rstrip() if output
                                                else '<empty>'))

            # print the back trace, it should help to identify the error in
            # the test
            backtrace = ['[Back trace]']
            for (filename, line, function, text) in \
                    traceback.extract_stack()[:-1]:
                backtrace.append('  [{0} line: {2} fn: {1}] {3}'.format(
                            filename, function, line, text
                    )
                )
            log.error('\n'.join(backtrace))
            log.error('[TEST ERROR] {0}'.format(exception.message))
            raise  # pass through

        return output

    def _match_literals(self, text, literals):
        '''Checks the text against the array of literals.

        Raises a TestFail exception in case one of the literals is not contained
        in the text.

        Args:
            text: String, it represents the text to match.
            literals: an array of string literals to match in the output.

        Throws: self.TestFail: if it cannot match one of the literals in
                the output.
        '''
        for string in literals:
            if string not in text:
                raise self.TestFail('Cannot find "{0}" in the output'
                                    .format(string))

    def _match_regexp_patterns(self, text, patterns):
        '''Checks the text against the array of regular expression patterns.

        Raises a TestFail exception in case one of the patterns is not matched
        in the given text.

        Args:
            text: String, it represents the text to match.
            patterns: an array of strings, each of them representing a regular
                      expression to match in text.

        Throws: self.TestFail: if it cannot match one of the literals in
                the output.
        '''
        log = util_log.get_logger()

        for regex in patterns:
            match = re.search(regex, text)
            if not match:
                raise self.TestFail('Cannot match the regexp "{0}" in '
                                    'the output'.format(regex))
            else:
                msg = 'Found match to regex {0}: {1}'.format(regex,
                                     match.group())
                log.debug(msg)

    @staticmethod
    def get_tmp_file_path():
        '''Get the path of a temporary file that is then deleted.

        Returns:
            A string that is the path to a temporary file.
        '''
        file_desc, name = tempfile.mkstemp()
        os.close(file_desc)
        os.remove(name)
        return name


class TestBaseNoTargetProcess(TestBase):
    '''lldb target that doesn't require a binary to be running.'''

    def get_bundle_target(self):
        '''Get bundle executable to run.

        Returns: None
        '''
        return None

    @property
    def bundle_target(self):
        return self.get_bundle_target()

    def run(self, dbg, remote_pid, lldb):
        '''Execute the test case.

        Args:
            dbg: The instance of the SBDebugger that is used to test commands.
            lldb: A handle to the lldb module.

        Returns:
            True: test passed, False: test failed.
        '''
        self._lldb = lldb
        self._dbg = dbg
        self._ci = dbg.GetCommandInterpreter()
        assert self._ci.IsValid()
        return super(TestBaseNoTargetProcess, self).run(self, dbg, remote_pid)
